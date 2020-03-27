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

    explicit ThreadPool(std::size_t numThreads) { start(numThreads); }

    ~ThreadPool() { stop(); }

    template<class T>
    auto enqueue(T task) -> std::future<decltype(task())>
    {
        auto wrapper = std::make_shared<std::packaged_task<decltype(task())()>>(std::move(task));

        {
            std::unique_lock<std::mutex> lock {mEventMutex};
            mTasks.emplace([=] { (*wrapper)(); });
        }

        mEventVar.notify_one();
        return wrapper->get_future();
    }

private:
    std::vector<std::thread> mThreads;

    std::condition_variable mEventVar;

    std::mutex mEventMutex;
    bool mStopping = false;

    std::queue<Task> mTasks;

    void start(std::size_t numThreads)
    {
        for (auto i = 0u; i < numThreads; ++i)
        {
            mThreads.emplace_back([=] {
                while (true)
                {
                    Task task;

                    {
                        std::unique_lock<std::mutex> lock {mEventMutex};

                        mEventVar.wait(lock, [=] { return mStopping || !mTasks.empty(); });

                        if (mStopping && mTasks.empty()) break;

                        task = std::move(mTasks.front());
                        mTasks.pop();
                    }

                    task();
                }
            });
        }
    }

    void stop() noexcept
    {
        {
            std::unique_lock<std::mutex> lock {mEventMutex};
            mStopping = true;
        }

        mEventVar.notify_all();

        for (auto& thread : mThreads) thread.join();
    }
};

int main()
{
    {
        Timer t {};
        ThreadPool pool {4};

        for (auto i = 0; i < 36; ++i)
        {
            pool.enqueue([i] {
                std::vector<int> v(50'000'000);
                std::sort(std::begin(v), std::end(v));
                // std::printf("Tasked finished: #%d\n", i + 1);
            });
            // std::printf("Tasked enqueued: #%d\n", i + 1);
        }
    }

    {
        Timer t {};
        for (auto i = 0; i < 36; ++i)
        {
            std::vector<int> v(50'000'000);
            std::sort(std::begin(v), std::end(v));
        }
    }

    return 0;
}