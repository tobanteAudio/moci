#pragma once

#include "moci/core/logging.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

namespace moci
{

using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

struct ProfileResult
{
    std::string Name;

    FloatingPointMicroseconds Start;
    std::chrono::microseconds ElapsedTime;
    std::thread::id ThreadID;
};

struct InstrumentationSession
{
    std::string Name;
};

class Instrumentor
{
private:
    std::mutex mutex_;
    std::unique_ptr<InstrumentationSession> currentSession_ {nullptr};
    std::ofstream outputStream_;
    std::vector<std::string> buffer_;

public:
    Instrumentor() = default;

    void BeginSession(std::string const& name, std::string const& filepath = "results.json")
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (currentSession_ != nullptr)
        {
            // If there is already a current session, then close it before beginning new one.
            // Subsequent profiling output meant for the original session will end up in the
            // newly opened session instead.  That's better than having badly formatted
            // profiling output.
            if (Log::GetCoreLogger())
            {  // Edge case: BeginSession() might be before Log::Init()
                MOCI_CORE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name,
                                currentSession_->Name);
            }
            InternalEndSession();
        }

        buffer_.reserve(1'000'000);

        outputStream_.open(filepath);
        if (outputStream_.is_open())
        {
            currentSession_       = std::make_unique<InstrumentationSession>();
            currentSession_->Name = name;
            WriteHeader();
        }
        else
        {
            if (Log::GetCoreLogger())
            {  // Edge case: BeginSession() might be before Log::Init()
                MOCI_CORE_ERROR("Instrumentor could not open results file '{0}'.", filepath);
            }
        }
    }

    void EndSession()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        InternalEndSession();
    }

    void WriteProfile(const ProfileResult& result)
    {
        auto name = result.Name;
        std::replace(name.begin(), name.end(), '"', '\'');

        auto const json = fmt::format(                                                                    //
            R"(,{{"cat":"function","dur": {0},"name": "{1}","ph":"X","pid":0,"tid": "{2}","ts": {3}}})",  //
            result.ElapsedTime.count(),                                                                   //
            name,                                                                                         //
            result.ThreadID,                                                                              //
            result.Start.count()                                                                          //
        );

        std::lock_guard<std::mutex> lock(mutex_);
        if (currentSession_ != nullptr)
        {
            buffer_.push_back(std::move(json));
        }
    }

    static auto Get() -> Instrumentor&
    {
        static Instrumentor instance;
        return instance;
    }

private:
    void WriteHeader()
    {
        outputStream_ << R"({"otherData": {},"traceEvents":[{})";
        outputStream_.flush();
    }

    void WriteFooter()
    {
        outputStream_ << "]}";
        outputStream_.flush();
    }

    // Note: you must already own lock on mutex_ before
    // calling InternalEndSession()
    void InternalEndSession()
    {

        if (currentSession_ != nullptr)
        {
            for (auto const& item : buffer_)
            {
                outputStream_ << item;
            }

            outputStream_.flush();
            WriteFooter();
            outputStream_.close();
            currentSession_.reset(nullptr);
            currentSession_ = nullptr;
        }
    }
};

class InstrumentationTimer
{
public:
    InstrumentationTimer(const char* name) : m_Name(name), m_Stopped(false)
    {
        m_StartTimepoint = std::chrono::steady_clock::now();
    }

    ~InstrumentationTimer()
    {
        if (!m_Stopped)
        {
            Stop();
        }
    }

    void Stop()
    {
        auto endTimepoint = std::chrono::steady_clock::now();
        auto highResStart = FloatingPointMicroseconds {m_StartTimepoint.time_since_epoch()};
        auto elapsedTime
            = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch()
              - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

        Instrumentor::Get().WriteProfile({m_Name, highResStart, elapsedTime, std::this_thread::get_id()});

        m_Stopped = true;
    }

private:
    const char* m_Name;
    std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
    bool m_Stopped;
};
}  // namespace moci

#define MOCI_PROFILE 1
#if MOCI_PROFILE
   // Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600))         \
    || defined(__ghs__)
#define MOCI_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define MOCI_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
#define MOCI_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define MOCI_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define MOCI_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define MOCI_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define MOCI_FUNC_SIG __func__
#else
#define MOCI_FUNC_SIG "MOCI_FUNC_SIG unknown!"
#endif

#define MOCI_PROFILE_BEGIN_SESSION(name, filepath) ::moci::Instrumentor::Get().BeginSession(name, filepath)
#define MOCI_PROFILE_END_SESSION() ::moci::Instrumentor::Get().EndSession()
#define MOCI_PROFILE_SCOPE(name) ::moci::InstrumentationTimer timer##__LINE__(name);
#define MOCI_PROFILE_FUNCTION() MOCI_PROFILE_SCOPE(MOCI_FUNC_SIG)
#else
#define MOCI_PROFILE_BEGIN_SESSION(name, filepath)
#define MOCI_PROFILE_END_SESSION()
#define MOCI_PROFILE_SCOPE(name)
#define MOCI_PROFILE_FUNCTION()
#endif