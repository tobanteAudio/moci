#pragma once

#include <utility>
namespace moci
{
template<typename Lambda>
class ScopeGuard
{
    bool committed {false};
    Lambda rollbackLambda;

public:
    explicit ScopeGuard(const Lambda& l) : rollbackLambda(l) { }

    ScopeGuard(const ScopeGuard& sc) : rollbackLambda(sc.rollbackLambda)
    {
        if (sc.committed) { committed = true; }
        else { sc.commit(); }
    }

    ScopeGuard(ScopeGuard&& sc) noexcept : rollbackLambda(sc.rollbackLambda)
    {
        if (sc.committed) { committed = true; }
        else { sc.commit(); }
    }

    // WARNING: only safe if adquire lambda does not throw, otherwise release lambda is never invoked, because the scope
    // guard never finished initialistion..
    template<typename AdquireLambda>
    ScopeGuard(const AdquireLambda& al, const Lambda& l) : rollbackLambda(l)
    {
        std::forward<AdquireLambda>(al)();
    }

    // WARNING: only safe if adquire lambda does not throw, otherwise release lambda is never invoked, because the scope
    // guard never finished initialistion..
    template<typename AdquireLambda, typename L>
    ScopeGuard(AdquireLambda&& al, L&& l) : rollbackLambda(std::forward<L>(l))
    {
        std::forward<AdquireLambda>(al)();  // just in case the functor has &&-qualified operator()
    }

    ~ScopeGuard()
    {
        if (!committed) { rollbackLambda(); }
    }

    inline void commit() { committed = true; }
};

// WARNING: only safe if adquire lambda does not throw, otherwise release lambda is never invoked, because the scope
// guard never finished initialistion..
template<typename aLambda, typename rLambda>
auto makeScopeGuardThatDoesNotRollbackIfAdquireThrows(aLambda&& a, rLambda&& r) -> ScopeGuard<rLambda>
{
    return ScopeGuard<rLambda>(std::forward<aLambda>(a), std::forward<rLambda>(r));
}

template<typename aLambda, typename rLambda>
auto makeScopeGuardThatDoesRollbackIfAdquireThrows(aLambda&& a, rLambda&& r) -> ScopeGuard<rLambda>
{
    auto scope = ScopeGuard<rLambda>(std::forward<rLambda>(r));
    a();
    return scope;
}

template<typename Lambda>
auto makeScopeGuard(Lambda&& r) -> ScopeGuard<Lambda>
{
    return ScopeGuard<Lambda>(std::forward<Lambda>(r));
}
}  // namespace moci