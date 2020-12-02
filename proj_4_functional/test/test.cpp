#include <iostream>
#include <string>
#include <cmath>
#include "../src/function.h"
#include "../src/compose.h"

const double EPS = 1e-7;

using namespace task;

int product(int i, int j)
{
    return i * j;
}

struct Number {
    Number(int num) : num(num) {}
    int add(int i) const { return num + i; }
private:
    int num;
};

struct String {
    String(std::string str) : str(str) {}
    size_t length() const {
        return str.length();
    }
private:
    std::string str;
};

struct Product {
    int operator()(int i, int j) const
    {
        return i * j;
    }
};

struct Sqrt {
    double operator()(double i) const
    {
        return std::sqrt(i);
    }
};

bool store_free_function_test() {
    function<int(int, int)> f = product;
    if (f(2, 3) != 6) {
        std::cerr << "Store a free function failed.\n";
        return false;
    }
    return true;
}

bool store_lambda_test() {
    // lambda with no parameters
    function<int()> f1 = []() { return 6; };
    if (f1() != 6) {
        std::cerr << "Store a lambda failed.\n";
        return false;
    }
    // lambda with two parameters
    function<int(int, int)> f2 = [](int i, int j) { return i * j; };
    if (f2(2, 3) != 6) {
        std::cerr << "Store a lambda failed.\n";
        return false;
    }
    // lambda with capture by reference
    int i = 2;
    function<void(int)> f3 = [&i](int j) { i *= j; };
    f3(3);
    if (i != 6) {
        std::cerr << "Store a lambda failed.\n";
        return false;
    }
    f3(2);
    if (i != 12) {
        std::cerr << "Store a lambda failed.\n";
        return false;
    }
    return true;
}

bool store_call_to_member_function_test() {
    function<int(const Number&, int)> f = &Number::add;
    const Number foo(1);
    if (f(foo, 3) != 4) {
        std::cerr << "Store a call to a member function failed.\n";
        return false;
    }
    if (f(1, 3) != 4) {
        std::cerr << "Store a call to a member function failed.\n";
        return false;
    }
    return true;
}

bool store_call_to_function_object_test() {
    function<int(int, int)> f = Product();
    if (f(2, 3) != 6) {
        std::cerr << "Store a call to a function object failed.\n";
        return false;
    }
    return true;
}

bool compose_test() {
    // free function
    const function<size_t(size_t, size_t)> f1 = product;
    // lambda
    std::string str = "hello";
    const function<std::string(size_t)> f2 = [str](size_t times) {
        std::string res = "";
        for (size_t i = 0; i < times; ++i) {
            res += str;
        }
        return res;
    };
    // member function
    const function<size_t(const String&)> f3 = &String::length;
    // function object
    const function<double(double)> f4 = Sqrt();
    auto comp = compose(f4, f3, f2, f1);
    if (std::abs(comp(2, 3) - std::sqrt(30)) > EPS) {
        std::cerr << "Composition failed.\n";
        return false;
    }
    return true;
}

int main() {
    if (!store_free_function_test())
        return 1;

    if (!store_lambda_test())
        return 1;

    if (!store_call_to_member_function_test())
        return 1;

    if (!store_call_to_function_object_test())
        return 1;

    if (!compose_test())
        return 1;

    return 0;
}