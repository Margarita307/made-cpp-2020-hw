#include <iostream>
#include "binary.h"
#include "value_list.h"

const size_t MOD = 1000000007;

template <size_t m, size_t row, size_t col, typename>
class CountCoverings {};

template <size_t m, size_t row, size_t col, Type Head, Type... Tail>
class CountCoverings<m, row, col, ValueList<Head, Tail...>> {
    using row_ = ValueList<Head, Tail...>;
    static constexpr bool is_covered_current = Get<col, row_>::value;
    static constexpr bool is_covered_right = Get<col - 1, row_>::value;
public:
    static constexpr size_t value =
        // move 1 cell right
        ((is_covered_current * CountCoverings<m, row, col - 1, typename Set<col, false, row_>::type>::value +
        // move 2 cells right with placing a horizontal tile
        !is_covered_current * !is_covered_right * CountCoverings<m, row, col - 2, row_>::value) % MOD +
        // move 1 cell right with placing a vertical tile
        !is_covered_current * CountCoverings<m, row, col - 1, typename Set<col, true, row_>::type>::value) % MOD;
};

template <size_t m, size_t row, Type Head, Type... Tail>
class CountCoverings<m, row, 1, ValueList<Head, Tail...>> {
    static constexpr size_t col = 1;
    using row_ = ValueList<Head, Tail...>;
    static constexpr bool is_covered_current = Get<col, row_>::value;
    static constexpr bool is_covered_right = Get<col - 1, row_>::value;
public:
    static constexpr size_t value =
        // move 1 cell right
        ((is_covered_current * CountCoverings<m, row, col - 1, typename Set<col, false, row_>::type>::value +
        // move to the row below with placing a horizontal tile
        !is_covered_current * !is_covered_right * CountCoverings<m, row - 1, m - 1, row_>::value) % MOD +
        // move 1 cell right with placing a vertical tile
        !is_covered_current * CountCoverings<m, row, col - 1, typename Set<col, true, row_>::type>::value) % MOD;
};

template <size_t m, size_t row, Type Head, Type... Tail>
class CountCoverings<m, row, 0, ValueList<Head, Tail...>> {
    static constexpr size_t col = 0;
    using row_ = ValueList<Head, Tail...>;
    static constexpr bool is_covered_current = Get<col, row_>::value;
public:
    static constexpr size_t value =
        // move to the row below
        (is_covered_current * CountCoverings<m, row - 1, m - 1, typename Set<col, false, row_>::type>::value +
        // move to the row below with placing a vertical tile
        !is_covered_current * CountCoverings<m, row - 1, m - 1, typename Set<col, true, row_>::type>::value) % MOD;
};

template <size_t m, size_t col, Type Head, Type... Tail>
class CountCoverings<m, 0, col, ValueList<Head, Tail...>> {
    static constexpr size_t row = 0;
    using row_ = ValueList<Head, Tail...>;
    static constexpr bool is_covered_current = Get<col, row_>::value;
    static constexpr bool is_covered_right = Get<col - 1, row_>::value;
public:
    static constexpr size_t value =
        // move 1 cell right
        (is_covered_current * CountCoverings<m, row, col - 1, typename Set<col, false, row_>::type>::value +
        // move 2 cells right with placing a horizontal tile
        !is_covered_current * !is_covered_right * CountCoverings<m, row, col - 2, row_>::value) % MOD;
};

template <size_t m, Type Head, Type... Tail>
class CountCoverings<m, 0, 1, ValueList<Head, Tail...>> {
    using row_ = ValueList<Head, Tail...>;
public:
    static constexpr size_t value = Get<0, row_>::value == Get<1, row_>::value;
};

template <size_t m, Type Head, Type... Tail>
class CountCoverings<m, 0, 0, ValueList<Head, Tail...>> {
    using row_ = ValueList<Head, Tail...>;
public:
    static constexpr size_t value = Get<0, row_>::value;
};

template <size_t width, size_t height>
class Floor {
    using empty_row = typename Construct<false, width>::type;
public:
    static constexpr size_t value = CountCoverings<width, height - 1, width - 1, empty_row>::value;
};

int main () {
    std::cout << "Possibilities count: " << Floor<6, 100>::value << std::endl;
    std::cout << std::endl;
    std::cout << "Possibilities count: " << Floor<1, 1>::value << std::endl; // 0
    std::cout << "Possibilities count: " << Floor<2, 1>::value << std::endl; // 1
    std::cout << "Possibilities count: " << Floor<3, 1>::value << std::endl; // 0
    std::cout << "Possibilities count: " << Floor<4, 1>::value << std::endl; // 1
    std::cout << "Possibilities count: " << Floor<5, 1>::value << std::endl; // 0
    std::cout << "Possibilities count: " << Floor<6, 1>::value << std::endl; // 1
    std::cout << std::endl;
    std::cout << "Possibilities count: " << Floor<1, 2>::value << std::endl; // 1
    std::cout << "Possibilities count: " << Floor<2, 2>::value << std::endl; // 2
    std::cout << "Possibilities count: " << Floor<3, 2>::value << std::endl; // 3
    std::cout << "Possibilities count: " << Floor<4, 2>::value << std::endl; // 5
    std::cout << "Possibilities count: " << Floor<5, 2>::value << std::endl; // 8
    std::cout << std::endl;
    std::cout << "Possibilities count: " << Floor<1, 3>::value << std::endl; // 0
    std::cout << "Possibilities count: " << Floor<2, 3>::value << std::endl; // = <3, 2> = 3
    std::cout << "Possibilities count: " << Floor<3, 3>::value << std::endl; // 0 - area is odd
    std::cout << "Possibilities count: " << Floor<5, 3>::value << std::endl; // 0 - area is odd
    return 0;
}
