// name: link_test.hpp

#pragma once

#include <px/dev/test.hpp>
#include <px/es/link_dispatcher.hpp>

using namespace px;

class component_a;
class component_b;

class component_a
	: public link_dispatcher<component_a>
	, public link<component_b>
{
};

class component_b
	: public link_dispatcher<component_b>
	, public link<component_a>
{
};

void test_link()
{
	test::section("link_test");
	{
		component_a a;
		component_b b;

		a.connect<component_b>(&b);
		b.connect<component_a>(&a);

		test::require(a.linked<component_b>() == &b);
		test::require(a.linked<component_b>()->linked<component_a>() == &a);

		test::require(a.qlink<component_a, component_b>() == &a);
		test::require(a.qlink<component_b>() == &b);
	}
}