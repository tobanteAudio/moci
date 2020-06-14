#include "moci/moci.hpp"

#include <iostream>
#include <memory>
#include <type_traits>

template<class T>
class ObjectPool final
{
public:
    using value_type = T;
    using pointer    = value_type*;

public:
    ObjectPool()
    {
        for (std::size_t i = 1; i < Size; ++i)
        {
            mPool[i - 1].mNext = &mPool[i];
        }

        mNextFree = &mPool[0];
    }

    ObjectPool(const ObjectPool&) = delete;

    ObjectPool(ObjectPool&& other) noexcept : mPool {std::move(other.mPool)}, mNextFree {other.mNextFree}
    {
        other.mNextFree = nullptr;
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

private:
    const static std::size_t Size = 100000;

private:
    union Item
    {
        std::aligned_storage_t<sizeof(value_type), alignof(value_type)> mStorage;
        Item* mNext;
    };

    std::unique_ptr<Item[]> mPool = std::make_unique<Item[]>(Size);
    Item* mNextFree               = nullptr;
};

struct S
{
    float x;
    float y;
    float z;
    float w;
};

int main(int, char**)
{
    moci::Log::Init();

    {
        moci::ExecutionTimer<> timer;
        ObjectPool<S> pool;
        for (auto i = 0; i < 1'000'000; ++i)
        {
            auto* data = pool.construct();
            moci::DoNotOptimizeAway(*data);
            pool.destroy(data);
        }
    }

    {
        moci::ExecutionTimer<> timer;
        for (auto i = 0; i < 1'000'000; ++i)
        {
            auto* data = new S;
            moci::DoNotOptimizeAway(*data);
            delete data;
        }
    }

    return 0;
}