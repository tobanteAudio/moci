#pragma once

#include "moci/core/logging.hpp"

#include "moci/core/vector.hpp"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

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
    Scope<InstrumentationSession> currentSession_ {nullptr};
    std::ofstream outputStream_;
    Vector<std::string> buffer_;

public:
    Instrumentor() = default;

    void beginSession(std::string const& name, std::string const& filepath = "results.json")
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
            internalEndSession();
        }

        buffer_.reserve(1'000'000);

        outputStream_.open(filepath);
        if (outputStream_.is_open())
        {
            currentSession_       = makeScope<InstrumentationSession>();
            currentSession_->Name = name;
            writeHeader();
        }
        else
        {
            if (Log::GetCoreLogger())
            {  // Edge case: BeginSession() might be before Log::Init()
                MOCI_CORE_ERROR("Instrumentor could not open results file '{0}'.", filepath);
            }
        }
    }

    void endSession()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        internalEndSession();
    }

    void writeProfile(const ProfileResult& result)
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
        if (currentSession_ != nullptr) { buffer_.push_back(json); }
    }

    static auto get() -> Instrumentor&
    {
        static Instrumentor instance;
        return instance;
    }

private:
    void writeHeader()
    {
        outputStream_ << R"({"otherData": {},"traceEvents":[{})";
        outputStream_.flush();
    }

    void writeFooter()
    {
        outputStream_ << "]}";
        outputStream_.flush();
    }

    // Note: you must already own lock on mutex_ before
    // calling InternalEndSession()
    void internalEndSession()
    {

        if (currentSession_ != nullptr)
        {
            for (auto const& item : buffer_) { outputStream_ << item; }

            outputStream_.flush();
            writeFooter();
            outputStream_.close();
            currentSession_.reset(nullptr);
            currentSession_ = nullptr;
        }
    }
};

class InstrumentationTimer
{
public:
    explicit InstrumentationTimer(const char* name) : name_(name)
    {
        startTimepoint_ = std::chrono::steady_clock::now();
    }

    ~InstrumentationTimer()
    {
        if (!stopped_) { stop(); }
    }

    void stop()
    {
        auto endTimepoint = std::chrono::steady_clock::now();
        auto highResStart = FloatingPointMicroseconds {startTimepoint_.time_since_epoch()};
        auto elapsedTime
            = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch()
              - std::chrono::time_point_cast<std::chrono::microseconds>(startTimepoint_).time_since_epoch();

        Instrumentor::get().writeProfile({name_, highResStart, elapsedTime, std::this_thread::get_id()});

        stopped_ = true;
    }

private:
    const char* name_;
    std::chrono::time_point<std::chrono::steady_clock> startTimepoint_;
    bool stopped_ {false};
};
}  // namespace moci

#define MOCI_PROFILE 1
#if MOCI_PROFILE
#define MOCI_PROFILE_BEGIN_SESSION(name, filepath) ::moci::Instrumentor::Get().BeginSession(name, filepath)
#define MOCI_PROFILE_END_SESSION() ::moci::Instrumentor::Get().EndSession()
#define MOCI_PROFILE_SCOPE(name) ::moci::InstrumentationTimer MOCI_ANONYMOUS_VARIABLE(timer)(name);
#define MOCI_PROFILE_FUNCTION() MOCI_PROFILE_SCOPE(MOCI_FUNC_SIG)
#else
#define MOCI_PROFILE_BEGIN_SESSION(name, filepath)
#define MOCI_PROFILE_END_SESSION()
#define MOCI_PROFILE_SCOPE(name)
#define MOCI_PROFILE_FUNCTION()
#endif
