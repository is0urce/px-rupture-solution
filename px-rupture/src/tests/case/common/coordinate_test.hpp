// name: coordinate_test.hpp

#pragma once

#include <px/dev/test.hpp>
#include <px/common/point.hpp>
#include <px/common/vector.hpp>

void test_coordinate()
{
	using namespace px;

	test::section("coordinate - point2");
	{
		test::require(point2(1, 2).x() == 1);
		test::require(point2(3, 4).y() == 4);

		test::require(point2(1, 1) == point2(1, 1));
		test::require(point2(1, 2) != point2(2, 1)); // require_compile

		test::require(point2(1, 2).moved({ 3, 6 }) == point2(4, 8));
		test::require(point2(1, 2).moved_axis<0>(3) == point2(4, 2));
		test::require(point2(1, 2).negated() == point2(-1, -2));
		test::require(point2(-3, 15).clamped({ 0, 0 }, { 10, 10 }) == point2(0, 10));
	}

	test::section("coordinate - vector2");
	{
		test::require(vector2(0.0, 2.0).divided({ 1.0, 2.0 }).y() == 1.0);
	}

	test::section("coordinate - i/o");
	{
		std::array<int, 2> mem{ 1, 2 };
		point2 a;

		a.read(mem.data());
		test::require(a == point2{ 1, 2 });

		point2(3, 4).write(mem.data());
		test::require(mem[0] == 3 && mem[1] == 4);
	} 

	test::section("coordinate - point2 operators");
	{
		point2 a(1, 2);
		point2 b(3, 4);

		point2 c = a + b;
		test::require(c == point2(4, 6));
		c = a - b;
		test::require(c == point2(-2, -2));
		c = a * b;
		test::require(c == point2(3, 8));
		c = a / b;
		test::require(c == point2(0, 0)); // integral division

		a += b;
		test::require(a == point2(4, 6));
		a -= b;
		a *= b;
		a /= b;

		a *= 2;
		test::require(a == point2(2, 4));
		a /= 2;
	}

	test::section("coordinate - vector2 operators");
	{
		vector2 a(1.0f, 2.0f);
		vector2 b(3, 4);

		auto c = a + b;
		c = a - b;
		c = a * b;
		c = a / b;
		test::require(c == vector2(1.0 / 3.0f, 2.0f / 4.0f));
		a += b;
		a -= b;
		a *= b;
		a /= b;

		a *= 2.0f;
		a /= 2.0f;
	}
}