// name: traverse_test.hpp

#pragma once

#include <px/dev/test.hpp>

#include <px/rl/mass.hpp>
#include <px/rl/traverse.hpp>

using namespace px;

using namespace px::rl;

typedef traverse_options<traverse> opts;
typedef mass<traverse> ms;

void test_traverse()
{
	test::section("traverse test");
	{
		test::section("traverse creation");
		{
			ms tile;
			test::require(!tile.is_traversable<traverse::floor>());

			opts unit(opts::bitset_type{ "000001" });
			test::require(unit.is_traversable<traverse::floor>());
		}

		test::section("traverse set");
		{
			ms tile;

			tile.make_traversable<traverse::floor>();
			test::require(tile.is_traversable<traverse::floor>());

			tile.make_blocking<traverse::floor>();
			test::require(!tile.is_traversable<traverse::floor>());

			tile.make_traversable();
			test::require(tile.is_traversable<traverse::floor>());
			test::require(tile.is_traversable<traverse::fly>());

			tile.make_blocking();
			test::require(!tile.is_traversable<traverse::floor>());
			test::require(!tile.is_traversable<traverse::fly>());

			tile.make_traversable(traverse::floor);
			test::require(tile.is_traversable<traverse::floor>());

			tile.make_traversable(traverse::floor, false);
			test::require(!tile.is_traversable<traverse::floor>());
		}

		test::section("traverse move");
		{
			opts tile(opts::bitset_type{ "100000" });
			opts unit(opts::bitset_type{ "100000" });

			test::require(tile.is_traversable(unit));

			tile = opts::bitset_type{ "011111" };
			test::require(!tile.is_traversable(unit));

			test::require(!tile.is_traversable(unit));
			test::require(!tile.is_traversable(unit));
		}
	}
}