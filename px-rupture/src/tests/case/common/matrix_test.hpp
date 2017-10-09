// name: martix_test.hpp

#pragma once

#include <px/dev/test.hpp>

#include <px/common/matrix.hpp>

#include <px/common/point.hpp>

using namespace px;

void test_matrix()
{
	test::section("matrix");
	{
		test::section("matrix static");
		{
			test::section("matrix static construction");
			{
				matrix2<int, 2, 3> mx(8);

				test::require(mx.width() == 2);
				test::require(mx.height() == 3);
				test::require(mx.size() == 6);

				test::require(mx.at(0, 0) == 8);

				matrix2<int, 8, 8> my([](size_t x, size_t y) { return static_cast<int>(x + y); }, 0);
				test::require(my.at(1, 3) = 1 + 3);
			}

			test::section("matrix static access");
			{
				matrix2<int, 3, 3> mt;

				mt[point2(0, 0)] = 1;

				test::require(mt[point2(0, 0)] == 1);
				test::require(mt.at(0, 0) == 1);
				test::require(mt.at(point2(0, 0)) == 1);
				test::require(mt.get(0, 0) == 1);
				test::require(mt.get(point2(0, 0)) == 1);
				test::require(mt.get<0, 0>() == 1);
				test::require(mt.get_or(18, 100, 100) == 18);
				test::require(mt.get_or(18, point2(3, 6)) == 18);
			}

			test::section("matrix static mutate");
			{
				matrix2<int, 3, 3> mt(8);

				mt.fill(3);
				test::require(mt.get<0, 0>() == 3);

				mt.generate([](size_t x, size_t y) { return static_cast<int>(x + y); });
				test::require(mt.get<1, 1>() == 1 + 1);

				int confirm = 0;
				mt.enumerate([&](size_t x, size_t y, int element) { confirm += (x + y == element ? 1 : 0); });
				test::require(confirm == mt.size());
			}
		}

		test::section("matrix dynamic");
		{
			test::section("matrix construction");
			{
				int init = 8;
				auto lambda = [](size_t x, size_t y) { return static_cast<int>(x + y); };
				matrix2<int> m_zero;
				matrix2<int> ma(2, 3);
				matrix2<int> mb(4, 5, init);
				matrix2<int> m_lambda(8, 9, lambda, 0);

				test::require(ma.width() == 2);
				test::require(mb.height() == 5);

				test::require(mb[point2(0, 0)] == init);
				test::require(m_lambda.at(2, 3) == 2 + 3);

				test::require(m_zero.width() == 0);
				test::require(m_zero.height() == 0);
				test::require(m_zero.size() == 0);
				test::require(m_zero.empty());
			}

			test::section("matrix access");
			{
				matrix2<int> mt(4, 5, 8);

				test::require(mt[point2(0, 0)] == 8);
				test::require(mt.at(0, 0) == 8);
				test::require(mt.at(point2(0, 0)) == 8);
				test::require(mt.get(0, 0) == 8);
				test::require(mt.get(point2(0, 0)) == 8);
				test::require(mt.get<0, 0>() == 8);
				test::require(mt.get_or(18, 100, 100) == 18);
				test::require(mt.get_or(18, point2(3, 6)) == 18);
			}

			test::section("matrix mutate");
			{
				matrix2<int> mt(4, 5, 8);

				test::require(mt[point2(0, 0)] == 8);

				mt.fill(3);
				test::require(mt.at(0, 0) == 3);

				mt.generate([](size_t x, size_t y) { return static_cast<int>(x + y); });
				test::require(mt.at(1, 2) == 3);

				int confirm = 0;
				mt.enumerate([&](size_t x, size_t y, int element) { confirm += (x + y == element ? 1 : 0); });
				test::require(confirm == mt.size());
			}
		}
	}
}