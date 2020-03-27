#include <chrono>
#include <iostream>
#include <memory>
#include <type_traits>

template<class T>
class ObjectPool final
{
public:
    const static std::size_t Size = 100000;

    using value_type = T;
    using pointer    = value_type*;

    ObjectPool()
    {
        for (auto i = 1; i < Size; ++i) mPool[i - 1].mNext = &mPool[i];

        mNextFree = &mPool[0];
    }

    ObjectPool(const ObjectPool&) = delete;

    ObjectPool(ObjectPool&& other) noexcept : mPool {std::move(other.mPool)}, mNextFree {other.mNextFree}
    {
        other.mNextFree = nullptr;
    }

    ~ObjectPool() = default;

    [[nodiscard]] pointer allocate()
    {
        if (mNextFree == nullptr) throw std::bad_alloc {};

        const auto item = mNextFree;
        mNextFree       = item->mNext;

        return reinterpret_cast<pointer>(&item->mStorage);
    }

    void deallocate(pointer p) noexcept
    {
        const auto item = reinterpret_cast<Item*>(p);

        item->mNext = mNextFree;
        mNextFree   = item;
    }

    template<class... Args>
    [[nodiscard]] pointer construct(Args&&... args)
    {
        return new (allocate()) value_type(std::forward<Args>(args)...);
    }

    void destroy(pointer p) noexcept
    {
        if (p == nullptr) return;

        p->~value_type();
        deallocate(p);
    }

    ObjectPool& operator=(const ObjectPool&) = delete;

    ObjectPool& operator=(ObjectPool&& other) noexcept
    {
        if (this == &other) return *this;

        mPool     = std::move(other.mPool);
        mNextFree = other.mNextFree;

        other.mNextFree = nullptr;

        return *this;
    }

private:
    union Item {
        std::aligned_storage_t<sizeof(value_type), alignof(value_type)> mStorage;
        Item* mNext;
    };

    std::unique_ptr<Item[]> mPool = std::make_unique<Item[]>(Size);
    Item* mNextFree               = nullptr;
};

template<class Resolution = std::chrono::microseconds>
class ExecutionTimer final
{
public:
    using Clock = std::conditional_t<std::chrono::high_resolution_clock::is_steady, std::chrono::high_resolution_clock,
                                     std::chrono::steady_clock>;

    ExecutionTimer() = default;

    ~ExecutionTimer()
    {
        std::cout << "Elapsed: " << std::chrono::duration_cast<Resolution>(Clock::now() - mStart).count() << "μs\n";
    }

private:
    Clock::time_point mStart = Clock::now();
};

template<class T>
void doNotOptimizeAway(T&& datum)
{
    asm volatile("" : "+r"(datum));
}

struct S
{
    float x;
    float y;
    float z;
    float w;
};

int main()
{

    {
        ExecutionTimer<> timer;
        ObjectPool<S> pool;
        for (auto i = 0; i < 1'000'000; ++i)
        {
            auto* data = pool.construct();
            doNotOptimizeAway(data);
            pool.destroy(data);
        }
    }

    {
        ExecutionTimer<> timer;
        for (auto i = 0; i < 1'000'000; ++i)
        {
            auto* data = new S;
            doNotOptimizeAway(data);
            delete data;
        }
    }

    return 0;
}