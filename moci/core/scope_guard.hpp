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
    explicit ScopeGuard(const Lambda& _l) : rollbackLambda(_l) { }

    ScopeGuard(const ScopeGuard& _sc) : rollbackLambda(_sc.rollbackLambda)
    {
        if (_sc.committed) { committed = true; }
        else { _sc.commit(); }
    }

    ScopeGuard(ScopeGuard&& _sc) noexcept : rollbackLambda(_sc.rollbackLambda)
    {
        if (_sc.committed) { committed = true; }
        else { _sc.commit(); }
    }

    // WARNING: only safe if adquire lambda does not throw, otherwise release lambda is never invoked, because the scope
    // guard never finished initialistion..
    template<typename AdquireLambda>
    ScopeGuard(const AdquireLambda& _al, const Lambda& _l) : rollbackLambda(_l)
    {
        std::forward<AdquireLambda>(_al)();
    }

    // WARNING: only safe if adquire lambda does not throw, otherwise release lambda is never invoked, because the scope
    // guard never finished initialistion..
    template<typename AdquireLambda, typename L>
    ScopeGuard(AdquireLambda&& _al, L&& _l) : rollbackLambda(std::forward<L>(_l))
    {
        std::forward<AdquireLambda>(_al)();  // just in case the functor has &&-qualified operator()
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
auto MakeScopeGuardThatDoesNOTRollbackIfAdquireThrows(aLambda&& _a, rLambda&& _r) -> ScopeGuard<rLambda>
{
    return ScopeGuard<rLambda>(std::forward<aLambda>(_a), std::forward<rLambda>(_r));
}

template<typename aLambda, typename rLambda>
auto MakeScopeGuardThatDoesRollbackIfAdquireThrows(aLambda&& _a, rLambda&& _r) -> ScopeGuard<rLambda>
{
    auto scope = ScopeGuard<rLambda>(std::forward<rLambda>(_r));
    _a();
    return scope;
}

template<typename Lambda>
auto MakeScopeGuard(Lambda&& _r) -> ScopeGuard<Lambda>
{
    return ScopeGuard<Lambda>(std::forward<Lambda>(_r));
}
}  // namespace moci