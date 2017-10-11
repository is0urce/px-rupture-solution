// name: environment.cpp

#include "environment.hpp"

#include "es/transform_component.hpp"
#include "es/composite_component.hpp"
#include "es/builder.hpp"

#include <px/fn/ant_generator.hpp>
#include <px/memory/memory.hpp>

#include <random>

namespace px {

	environment::~environment()
	{
	}
	environment::environment()
		: turn_number(0)
		, player(nullptr)
		, run(true)
	{
	}

	void environment::incarnate(transform_component * target)
	{
		player = target;
		sprites.target(target);
	}
	void environment::step(point2 const& movement)
	{
		if (player) {
			auto destination = player->position() + movement;
			player->place(destination);
			player->store();
		}
	}
	void environment::turn()
	{
		++turn_number;
	}

	void environment::start()
	{
		builder b(this);

		b.add_sprite("m_imp");
		b.add_transform({ 4, 5 });
		auto unit = b.request();
		unit->enable();
		units.emplace_back(std::move(unit));

		b.add_sprite("m_darkness");
		incarnate(b.add_transform({ 5, 5 }));
		unit = b.request();
		unit->enable();
		units.emplace_back(std::move(unit));

		b.add_sprite("m_succubus");
		b.add_transform({ 6, 5 });
		unit = b.request();
		unit->enable();
		units.emplace_back(std::move(unit));

		auto map = fn::ant_generator::generate(std::mt19937{}, 10, 10, 10 * 10 * 100 / 61);
		map.enumerate([this](size_t x, size_t y, unsigned char tile) {
			this->terrain.pset(tile == 0 ? 1 : 3, point2(static_cast<int>(x), static_cast<int>(y)));
		});
	}
}