#pragma once

#include <utility>
namespace moci
{
template<typename Lambda>
class ScopeGuard
{
    bool committed;
    Lambda rollbackLambda;

public:
    ScopeGuard(const Lambda& _l) : committed(false), rollbackLambda(_l) { }

    ScopeGuard(const ScopeGuard& _sc) : committed(false), rollbackLambda(_sc.rollbackLambda)
    {
        if (_sc.committed) { committed = true; }
        else
        {
            _sc.commit();
        }
    }

    ScopeGuard(ScopeGuard&& _sc) noexcept : committed(false), rollbackLambda(_sc.rollbackLambda)
    {
        if (_sc.committed) { committed = true; }
        else
        {
            _sc.commit();
        }
    }

    // WARNING: only safe if adquire lambda does not throw, otherwise release lambda is never invoked, because the scope
    // guard never finished initialistion..
    template<typename AdquireLambda>
    ScopeGuard(const AdquireLambda& _al, const Lambda& _l) : committed(false), rollbackLambda(_l)
    {
        std::forward<AdquireLambda>(_al)();
    }

    // WARNING: only safe if adquire lambda does not throw, otherwise release lambda is never invoked, because the scope
    // guard never finished initialistion..
    template<typename AdquireLambda, typename L>
    ScopeGuard(AdquireLambda&& _al, L&& _l) : committed(false), rollbackLambda(std::forward<L>(_l))
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
ScopeGuard<rLambda> MakeScopeGuardThatDoesNOTRollbackIfAdquireThrows(aLambda&& _a, rLambda&& _r)
{
    return ScopeGuard<rLambda>(std::forward<aLambda>(_a), std::forward<rLambda>(_r));
}

template<typename aLambda, typename rLambda>
ScopeGuard<rLambda> MakeScopeGuardThatDoesRollbackIfAdquireThrows(aLambda&& _a, rLambda&& _r)
{
    auto scope = ScopeGuard<rLambda>(std::forward<rLambda>(_r));
    _a();
    return scope;
}

template<typename Lambda>
ScopeGuard<Lambda> MakeScopeGuard(Lambda&& _r)
{
    return ScopeGuard<Lambda>(std::forward<Lambda>(_r));
}
}  // namespace moci