// name: qtree_test.hpp

#pragma once

#include <px/dev/test.hpp>
#include <px/common/qtree.hpp>
#include <px/common/point.hpp>

#include <list>
#include <random>

void test_qtree() {
    using namespace px;

    test::section("qtree");
    {
        qtree<int> qt(64);
        bool found = false;

        test::require(qt.empty());

        qt.add(point2{ 1, 2 }, 3);

        test::require(!qt.empty());
        test::require(qt.exists(point2{ 1, 2 }));

        found = false;
        qt.find(point2{ 0, 0 }, 10000, [&](int x, int y, auto element) { if (element == 3 && x == 1 && y == 2) found = true; });
        test::require(found);

        found = false;
        qt.find(point2(1, 2), [&](auto element) { if (element == 3) found = true; });
        test::require(found);

        qt.move(point2{ 1, 2 }, 3, point2{ 2, 1 });
        test::require(!qt.exists(point2{ 1, 2 }));
        test::require(qt.exists(point2{ 2, 1 }));

        qt.remove(point2(2, 1), 3);
        test::require(qt.empty());
        test::require(!qt.exists(point2{ 1, 2 }));

        found = false;
        qt.find(point2{ 0, 0 }, 6, [&](int /*x*/, int /*y*/, auto element) { if (element == 3) found = true; });
        test::require(!found);

        std::list<point2> store;
        std::mt19937 rng;
        for (int i = 0; i != 10; ++i) {
            int x = rng() % 10000;
            int y = rng() % 10000;
            point2 e(x, y);
            store.push_back(e);
            qt.add(e, 0);
        }

        for (auto & e : store) {
            qt.remove(e, 0);
        }

        test::require(qt.empty());
    }

}