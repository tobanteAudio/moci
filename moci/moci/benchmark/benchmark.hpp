#pragma once

#include "moci/core/logging.hpp"

#include <chrono>

namespace moci
{

#ifdef _MSC_VER

#pragma optimize("", off)
inline void DoNotOptimizeDependencySink(const void*) {}
#pragma optimize("", on)

/**
 * @brief Useful for writting benchmarks
 */
template<class T>
void DoNotOptimizeAway(const T& datum)
{
    DoNotOptimizeDependencySink(&datum);
}

#else

/**
 * @brief Useful for writting benchmarks
 */
template<typename T>
auto DoNotOptimizeAway(const T& datum) -> void
{
    // This version of DoNotOptimizeAway tells the compiler that the asm
    // block will read datum from memory, and that in addition it might read
    // or write from any memory location.  If the memory clobber could be
    // separated into input and output that would be preferrable.
    asm volatile("" ::"m"(datum) : "memory");
}
#endif

/**
 * @brief Simple RAII timer. Prints the measured time to log info.
 */
template<class Resolution = std::chrono::microseconds>
class ExecutionTimer final
{
public:
    using Clock = std::conditional_t<std::chrono::high_resolution_clock::is_steady, std::chrono::high_resolution_clock,
                                     std::chrono::steady_clock>;

public:
    ExecutionTimer() = default;
    ~ExecutionTimer()
    {
        auto const micros = std::chrono::duration_cast<Resolution>(Clock::now() - mStart).count();
        MOCI_CORE_INFO("Elapsed: {}μs", micros);
    }

private:
    Clock::time_point mStart = Clock::now();
};

}  // namespace moci
