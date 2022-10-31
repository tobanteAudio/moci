#include <moci/core/logging.hpp>
#include <moci/core/vector.hpp>

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace moci
{

Ref<spdlog::logger> Log::sCoreLogger;
Ref<spdlog::logger> Log::sClientLogger;

void Log::init()
{
    Vector<spdlog::sink_ptr> logSinks {};
    logSinks.emplace_back(makeRef<spdlog::sinks::stdout_color_sink_mt>());
    logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("moci.log", true));

    logSinks[0]->set_pattern("%^[%T] %n: %v%$");
    logSinks[1]->set_pattern("[%T] [%l] %n: %v");

    sCoreLogger = makeRef<spdlog::logger>("MOCI", begin(logSinks), end(logSinks));
    spdlog::register_logger(sCoreLogger);
    sCoreLogger->set_level(spdlog::level::trace);
    sCoreLogger->flush_on(spdlog::level::trace);

    sClientLogger = makeRef<spdlog::logger>("APP", begin(logSinks), end(logSinks));
    spdlog::register_logger(sClientLogger);
    sClientLogger->set_level(spdlog::level::trace);
    sClientLogger->flush_on(spdlog::level::trace);
}

}  // namespace moci