#pragma once

#include <px/dev/test.hpp>
#include <px/common/pool_chain.hpp>

void test_pool()
{
	using namespace px;

	test::section("pool");
	{
		pool<int, 100> p;

		test::require(!p.contains(nullptr));
		test::require(p.empty());

		auto ip = p.make_uq(3);

		int * raw = ip.get();

		test::require(raw != nullptr);
		test::require(p.size() == 1);
		test::require(!p.empty());
		test::require(p.contains(raw));
		test::require(*raw == 3);

		ip.reset();

		test::require(p.empty());
		test::require(p.size() == 0);
		test::require(!p.contains(raw));
	}

	test::section("pool chain");
	{
		pool_chain<int, 100> system;

		auto x1 = system.make_uq(6);
		auto x2 = system.make_uq(5);
		int summ = 0;
		system.enumerate([&](int e) { summ += e; });

		test::require(system.size() == 2);
		test::require(summ == 6 + 5);

		x1.reset();
		x2.reset();

		test::require(system.size() == 0);
		test::require(system.empty());
	}
}