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

		test::require(point2(1, 2).moved(point2(3, 6)) == point2{ 4, 8 });
		test::require(point2(1, 2).moved_axis<0>(3) == point2{ 4, 2 });
		test::require(point2(1, 2).negated() == point2(-1, -2));
		test::require(point2(-3, 15).clamped(point2(0, 0), point2(10, 10)) == point2(0, 10));
	}

	test::section("coordinate - vector2");
	{
		test::require(vector2(0.0, 2.0).divided(vector2(1.0, 2.0)).y() == 1.0);
	}
}