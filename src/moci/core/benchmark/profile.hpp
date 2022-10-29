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
    std::mutex _mutex;
    Scope<InstrumentationSession> _currentSession {nullptr};
    std::ofstream _outputStream;
    Vector<std::string> _buffer;

public:
    Instrumentor() = default;

    void beginSession(std::string const& name, std::string const& filepath = "results.json")
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_currentSession != nullptr)
        {
            // If there is already a current session, then close it before beginning new one.
            // Subsequent profiling output meant for the original session will end up in the
            // newly opened session instead.  That's better than having badly formatted
            // profiling output.
            if (Log::getCoreLogger())
            {  // Edge case: BeginSession() might be before Log::Init()
                MOCI_CORE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name,
                                _currentSession->Name);
            }
            internalEndSession();
        }

        _buffer.reserve(1'000'000);

        _outputStream.open(filepath);
        if (_outputStream.is_open())
        {
            _currentSession       = makeScope<InstrumentationSession>();
            _currentSession->Name = name;
            writeHeader();
        }
        else
        {
            if (Log::getCoreLogger())
            {  // Edge case: BeginSession() might be before Log::Init()
                MOCI_CORE_ERROR("Instrumentor could not open results file '{0}'.", filepath);
            }
        }
    }

    void endSession()
    {
        std::lock_guard<std::mutex> lock(_mutex);
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

        std::lock_guard<std::mutex> lock(_mutex);
        if (_currentSession != nullptr) { _buffer.push_back(json); }
    }

    static auto get() -> Instrumentor&
    {
        static Instrumentor instance;
        return instance;
    }

private:
    void writeHeader()
    {
        _outputStream << R"({"otherData": {},"traceEvents":[{})";
        _outputStream.flush();
    }

    void writeFooter()
    {
        _outputStream << "]}";
        _outputStream.flush();
    }

    // Note: you must already own lock on mutex_ before
    // calling InternalEndSession()
    void internalEndSession()
    {

        if (_currentSession != nullptr)
        {
            for (auto const& item : _buffer) { _outputStream << item; }

            _outputStream.flush();
            writeFooter();
            _outputStream.close();
            _currentSession.reset(nullptr);
            _currentSession = nullptr;
        }
    }
};

class InstrumentationTimer
{
public:
    explicit InstrumentationTimer(const char* name) : _name(name)
    {
        _startTimepoint = std::chrono::steady_clock::now();
    }

    ~InstrumentationTimer()
    {
        if (!_stopped) { stop(); }
    }

    void stop()
    {
        auto endTimepoint = std::chrono::steady_clock::now();
        auto highResStart = FloatingPointMicroseconds {_startTimepoint.time_since_epoch()};
        auto elapsedTime
            = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch()
              - std::chrono::time_point_cast<std::chrono::microseconds>(_startTimepoint).time_since_epoch();

        Instrumentor::get().writeProfile({_name, highResStart, elapsedTime, std::this_thread::get_id()});

        _stopped = true;
    }

private:
    const char* _name;
    std::chrono::time_point<std::chrono::steady_clock> _startTimepoint;
    bool _stopped {false};
};
}  // namespace moci

#define MOCI_PROFILE 1
#if MOCI_PROFILE
#define MOCI_PROFILE_BEGIN_SESSION(name, filepath) ::moci::Instrumentor::get().beginSession(name, filepath)
#define MOCI_PROFILE_END_SESSION() ::moci::Instrumentor::get().endSession()
#define MOCI_PROFILE_SCOPE(name) ::moci::InstrumentationTimer MOCI_ANONYMOUS_VARIABLE(timer)(name);
#define MOCI_PROFILE_FUNCTION() MOCI_PROFILE_SCOPE(MOCI_FUNC_SIG)
#else
#define MOCI_PROFILE_BEGIN_SESSION(name, filepath)
#define MOCI_PROFILE_END_SESSION()
#define MOCI_PROFILE_SCOPE(name)
#define MOCI_PROFILE_FUNCTION()
#endif
