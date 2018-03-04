// name: label.hpp

#pragma once

#include <px/dev/test.hpp>
#include <px/common/label.hpp>

#include <string>

void test_label() {

    using namespace px;

    test::section("label");
    {
        label<8> empty;
        label<6> yes = "yes";

        test::require(empty.length() == 0);
        test::require(yes.length() == 3);

        label<6> b = yes;
        label<6> no = "nop";
        test::require(yes == b);
        test::require(yes != no);
        // test::require(empty != no); // size mismatch, compile error generated

        test::require(no < yes); // lexicographical comparison

        b = "trust"; // assignment
        test::require(std::string(b.c_str()) == "trust"); // correct c string

        auto y = empty + yes;
        test::require(y == "yes");

        label<256> bignono = "BIG" "NO" "NO";
        test::require(bignono == "BIGNONO");        // transparent comparison
        test::require(bignono != "anything-else");  // transparent comparison
    }
}