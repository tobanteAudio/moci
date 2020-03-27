#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <queue>
#include <thread>
#include <vector>

class Timer
{
public:
    using Clock = std::conditional_t<std::chrono::high_resolution_clock::is_steady, std::chrono::high_resolution_clock,
                                     std::chrono::steady_clock>;

public:
    Timer() = default;
    ~Timer()
    {
        auto const micros = std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - start_).count();
        std::cout << static_cast<double>(micros) / 1000.0 << "ms" << '\n';
    }

private:
    Clock::time_point start_ = Clock::now();
};
class ThreadPool
{
public:
    using Task = std::function<void()>;

public:
    explicit ThreadPool(std::size_t const numthreads) { start(numthreads); }
    ~ThreadPool() { stop(); }

    template<class T>
    auto Enqueue(T task) -> std::future<decltype(task())>
    {
        auto wrapper = std::make_shared<std::packaged_task<decltype(task())()>>(std::move(task));

        {
            std::unique_lock<std::mutex> lock {eventMutex_};
            tasks_.emplace([=] { (*wrapper)(); });
        }

        eventVar_.notify_one();
        return wrapper->get_future();
    }

private:
    void start(std::size_t const numthreads)
    {
        for (auto i = 0u; i < numthreads; ++i)
        {
            threads_.emplace_back([=] {
                while (true)
                {
                    Task task;

                    {
                        std::unique_lock<std::mutex> lock {eventMutex_};

                        eventVar_.wait(lock, [=] { return shouldStop_ || !tasks_.empty(); });

                        if (shouldStop_ && tasks_.empty()) break;

                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }

                    task();
                }
            });
        }
    }

    void stop() noexcept
    {
        {
            std::unique_lock<std::mutex> lock {eventMutex_};
            shouldStop_ = true;
        }

        eventVar_.notify_all();

        for (auto& thread : threads_)
        {
            thread.join();
        }
    }

private:
    std::queue<Task> tasks_;
    std::vector<std::thread> threads_;
    std::condition_variable eventVar_;
    std::mutex eventMutex_;
    bool shouldStop_ = false;
};

template<class T>
void doNotOptimizeAway(T&& datum)
{
    asm volatile("" : "+r"(datum));
}

int main()
{
    {
        Timer t {};
        ThreadPool pool {1};

        for (auto i = 0; i < 36; ++i)
        {
            pool.Enqueue([] {
                std::vector<int> v(5'000'000);
                std::sort(std::begin(v), std::end(v));
                doNotOptimizeAway(v.data());
            });
        }
    }

    {
        Timer t {};
        for (auto i = 0; i < 36; ++i)
        {
            std::vector<int> v(5'000'000);
            std::sort(std::begin(v), std::end(v));
            doNotOptimizeAway(v.data());
        }
    }

    return 0;
}