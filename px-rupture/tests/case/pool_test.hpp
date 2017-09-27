#pragma once

#include <px/common/pool.hpp>

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
	}