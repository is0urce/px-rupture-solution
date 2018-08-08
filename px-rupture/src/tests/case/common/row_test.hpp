// name: row_test.hpp

#pragma once

#include <px/dev/test.hpp>
#include <px/common/row.hpp>

void test_row() {
    using namespace px;

    test::section("row");
    {
        row<int, 3> x;
        x.push_back(1);
        test::require(x.size() == 1);
        int f = x[0];
        test::require(f == 1);

        test::require_throw([&]() { x.at(1) = 1; });

        x.push_back(2);
        x.emplace_back(3);

        test::require(x.size() == 3);
        test::require_throw([&]() { x.push_back(0); });

        x.pop();
        test::require(x.size() == 2);
        test::require(x.at(1) == 2);
        test::require_throw([&]() {x.at(2) = 3; });

        test::section("copy");
        {
            row<int, 3> y(std::move(x));
            test::require(y.size() == 2);
            test::require(y[0] == 1);
            test::require(y[1] == 2);
        }
    }
}