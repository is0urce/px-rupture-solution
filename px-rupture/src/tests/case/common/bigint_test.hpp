// name: bigint_test.hpp

#pragma once

#include <px/dev/test.hpp>
#include <px/common/b_uint.hpp>

void test_bigint() {
    using namespace px;

    test::section("b_uint");
    {
        b_uint b0;
        b_uint b00(0);
        b_uint b1(1);

        test::require(b0.is_zero());
        test::require(b00.is_zero());
        test::require(!b1.is_zero());

        b0 = b1;
        test::require(!b0.is_zero());

        test::require(b_uint(100) == b_uint(100));
        test::require(b_uint(100) != b_uint(200));

        b00 *= 10;
        test::require(b00.is_zero());
    }

    test::section("b_uint operations");
    {
        test::require(b_uint(255) * 255 == 255 * 255);
        test::require(b_uint(255) + 255 == 255 + 255);
        test::require(b_uint(1000).less(1001));

        b_uint b0 = 0;
        b0.increment();
        test::require(b0 == 1);

        b_uint b1 = 1;
        b1.decrement();
        test::require(b1.is_zero());

        b_uint b255 = 255;
        b255.increment();
        test::require(b255 == 256);
        b255.decrement();
        test::require(b255 == 255);

        b_uint b2k = 2000;
        b2k.sub(1500);
        test::require(b2k == 500);
    }
}