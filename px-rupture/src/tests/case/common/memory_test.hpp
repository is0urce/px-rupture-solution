// name: memory_test.hpp

#pragma once

#include <px/dev/test.hpp>
#include <px/memory/memory.hpp>

void test_memory()
{
	using namespace px;

	test::section("uq_ptr");
	{
		auto ptr = make_uq<int>(3);
		test::require(*ptr == 3);
	}

	test::section("uq_ptr move");
	{
		auto p1 = make_uq<int>(3);
		auto p2 = std::move(p1);
		test::require(p1.get() == nullptr);
		test::require(p2.get() != nullptr);
		test::require(*p2 == 3);
	}
}