#pragma once

#include<memory>
#include<functional>

namespace task {

    struct bad_function_call : std::exception
    {
        const char* what() const noexcept override
        {
            return "bad function call";
        }
    };

    template<class>
    class function;

    template<class R, class... Args>
    class function<R(Args...)> {
    public:
        function() noexcept {}

        function(std::nullptr_t) noexcept {}

        function(const function& other) {
            target = std::make_unique<callable<decltype(*other.target)>>(other.target->clone());
        }

        function(function&& other) noexcept : target(std::move(other.target)) {}

        template<class F>
        function(F f) {
            target = std::make_unique<callable<F>>(f);
        }

        function& operator=(const function& other) {
            target = std::make_unique<callable<decltype(*other.target)>>(other.target->clone());
            return *this;
        }

        function& operator=(function&& other) noexcept {
            target(std::move(other.target));
            return *this;
        }

        function& operator=(std::nullptr_t) noexcept {
            target = nullptr;
            return *this;
        }

        void swap(function& other) noexcept {
            std::swap(target, other.target);
        }

        explicit operator bool() const noexcept {
            return target != nullptr;
        }

        R operator ()(Args... args) const {
            if (!target)
                throw bad_function_call();
            return (*target)(args...);
        }
    private:
        struct callable_base {
            virtual R operator() (Args...) = 0;
            virtual callable_base& clone() const = 0;
        };

        template<class F>
        struct callable : callable_base {
            F functor;
            explicit callable(F functor) : functor(functor) {}
            R operator() (Args... args) override { return std::invoke(functor, args...); }
            callable<F>& clone()  const override {
                return *new callable(*this);
            }
        };

        std::unique_ptr<callable_base> target;
    };

}
