// name: row_list_test.hpp

#pragma once

#include <px/dev/test.hpp>
#include <px/fn/roll_list.hpp>
#include <string>

void test_roll_list() {
    using namespace px;

    test::section("roll_list");
    {
        test::section("roll_list create");
        {
            roll_list<std::string> loot_base;
            roll_list<std::string> loot_extra(roll_list_base::roll_policy::one_of);

            loot_base.add_item("a", 0.0);
            loot_base.add_item("b", 1.0);
            loot_base.add_list(&loot_extra, 1.0);

            test::require(loot_base.get_policy() == roll_list_base::roll_policy::all_of);
            test::require(loot_extra.get_policy() == roll_list_base::roll_policy::one_of);
        }

        test::section("roll_list 100%");
        {
            std::mt19937 rng;
            roll_list<int> loot;
            loot.add_item(42, 1.0);
            int result = 0;
            loot.evaluate(rng, [&](int roll) { result = roll; });
            test::require(result == 42);
        }
    }
}