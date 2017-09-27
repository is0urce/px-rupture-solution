#pragma once

#include <px/common/pool_chain.hpp>

	void test_pool()
	{
		px::pool<int, 100> p;

		px::test::require(!p.contains(nullptr));
		px::test::require(p.empty());

		auto ip = p.make_uq(3);

		int * raw = ip.get();

		px::test::require(raw != nullptr);
		px::test::require(p.size() == 1);
		px::test::require(!p.empty());
		px::test::require(p.contains(raw));
		px::test::require(*raw == 3);

		ip.release();

		px::test::require(p.empty());
		px::test::require(p.size() == 0);
		px::test::require(!p.contains(raw));


		px::test::section("pool chain");

		px::pool_chain<int, 100> system;

		auto x1 = system.make_uq(6);
		auto x2 = system.make_uq(5);
		int summ = 0;
		system.enumerate([&](int e) { summ += e; });

		px::test::require(system.size() == 2);
		px::test::require(summ == 41);
	}