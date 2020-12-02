#pragma once

#include "function.h"

namespace task {
    
    template<class... Args>
    struct composition_args;

    template<class...>
    struct get_composition_args;

    template<class... Args, class R>
    struct get_composition_args<function<R(Args...)>> {
        using args = composition_args<Args...>;
    };

    template<class... Args, class R, class... F>
    struct get_composition_args<function<R(Args...)>, F...> {
        using args = typename get_composition_args<F...>::args;
    };

    template<class... F>
    class composition;

    template<class... CompArgs, class... Args, class R, class...F>
    class composition<composition_args<CompArgs...>, function<R(Args...)>, F...> {
    public:
        composition(const function<R(Args...)>& head, const F&... tail) : head(head),
    	    tail(composition<composition_args<CompArgs...>, F...>(tail...)) {}

        R operator ()(CompArgs... args) const {
            return head(tail(args...));
        }
    private:
        function<R(Args...)> head;
        composition<composition_args<CompArgs...>, F...> tail;
    };

    template<class... CompArgs, class... Args, class R> // CompArgs == Args
    class composition<composition_args<CompArgs...>, function<R(Args...)>> {
    public:
        composition(const function<R(Args...)>& head) : head(head) {}

        R operator ()(CompArgs... args) const {
            return head(args...);
        }
    private:
        function<R(Args...)> head;
    };

    template<class...>
    struct get_composition_type;

    template<class... Args, class R>
    struct get_composition_type<function<R(Args...)>> {
        using type = function<R(Args...)>;
    };

    template<class... Args1, class R1, class... Args2, class R2>
    struct get_composition_type<function<R1(Args1...)>, function<R2(Args2...)>> {
        using type = function<R1(Args2...)>;
    };

    template<class... Args1, class R1, class... Args2, class R2, class... F>
    struct get_composition_type<function<R1(Args1...)>, function<R2(Args2...)>, F...> {
        using type = typename get_composition_type<function<R1(Args1...)>, F...>::type;
    };

    template<class... Args, class R, class... F>
    auto compose(const function<R(Args...)>& head, const F&... tail) {
        using type = typename get_composition_type<function<R(Args...)>, F...>::type;
        using args = typename get_composition_args<function<R(Args...)>, F...>::args;
        return type(composition<args, function<R(Args...)>, F...>(head, tail...));
    }
    
}
