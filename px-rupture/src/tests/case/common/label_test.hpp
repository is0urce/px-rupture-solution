// name: label.hpp

#pragma once

#include <px/dev/test.hpp>
#include <px/common/label.hpp>

#include <string>

void test_label() {

    using namespace px;

    test::section("label");
    {
        // constructors
        label<8> empty;
        label<6> yes = "yes";

        // length
        test::require(empty.length() == 0);
        test::require(yes.length() == 3);

        label<6> b = yes;                       // assignment

        label<6> no = "nop";
        test::require(yes == b);
        test::require(yes != no);
        test::require(no < yes);                // lexicographical comparison
        test::require_nocompile("empty != no"); // size mismatch, compile error generated


        b = "trust";                            // assignment

        // correct c string
        test::require(std::string(b.c_str()) == "trust");
        test::require(std::string(empty.c_str()) == "");
        test::require(b.c_str() == std::string("trust"));
        test::require(empty.c_str() == std::string(""));

        // concatenation
        auto y = empty + yes;
        test::require(y == "yes");

        // transparent comparison
        label<256> bignono = "BIG" "NO" "NO";
        test::require(bignono == "BIGNONO");        
        test::require(bignono != "anything-else");
    }
}