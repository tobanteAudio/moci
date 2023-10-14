#pragma once

#include <utility>

namespace moci {
template<typename Lambda>
class ScopeGuard
{
    bool _committed{false};
    Lambda _rollbackLambda;

public:
    explicit ScopeGuard(Lambda const& l) : _rollbackLambda(l) {}

    ScopeGuard(ScopeGuard const& sc) : _rollbackLambda(sc._rollbackLambda)
    {
        if (sc._committed) {
            _committed = true;
        } else {
            sc.commit();
        }
    }

    ScopeGuard(ScopeGuard&& sc) noexcept : _rollbackLambda(sc._rollbackLambda)
    {
        if (sc._committed) {
            _committed = true;
        } else {
            sc.commit();
        }
    }

    // WARNING: only safe if adquire lambda does not throw, otherwise release lambda is
    // never invoked, because the scope guard never finished initialistion..
    template<typename AdquireLambda>
    ScopeGuard(AdquireLambda const& al, Lambda const& l) : _rollbackLambda(l)
    {
        std::forward<AdquireLambda>(al)();
    }

    // WARNING: only safe if adquire lambda does not throw, otherwise release lambda is
    // never invoked, because the scope guard never finished initialistion..
    template<typename AdquireLambda, typename L>
    ScopeGuard(AdquireLambda&& al, L&& l) : _rollbackLambda(std::forward<L>(l))
    {
        std::forward<AdquireLambda>(al)(
        );  // just in case the functor has &&-qualified operator()
    }

    ~ScopeGuard()
    {
        if (!_committed) {
            _rollbackLambda();
        }
    }

    inline void commit() { _committed = true; }
};

// WARNING: only safe if adquire lambda does not throw, otherwise release lambda is never
// invoked, because the scope guard never finished initialistion..
template<typename aLambda, typename rLambda>
auto makeScopeGuardThatDoesNotRollbackIfAdquireThrows(aLambda&& a, rLambda&& r)
    -> ScopeGuard<rLambda>
{
    return ScopeGuard<rLambda>(std::forward<aLambda>(a), std::forward<rLambda>(r));
}

template<typename aLambda, typename rLambda>
auto makeScopeGuardThatDoesRollbackIfAdquireThrows(aLambda&& a, rLambda&& r)
    -> ScopeGuard<rLambda>
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
