// name: environment.cpp

#include "environment.hpp"

#include "es/transform_component.hpp"
#include "es/sprite_component.hpp"
#include "es/composite_component.hpp"

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
		auto tr = transforms.make();
		tr->place({ 4, 5 });
		tr->store();
		auto spr = sprites.make("m_imp");
		spr->connect<transform_component>(tr.get());
		auto unit = make_uq<composite_component>();
		unit->add(std::move(tr));
		unit->add(std::move(spr));
		unit->enable();
		units.emplace_back(std::move(unit));

		tr = transforms.make();
		incarnate(tr.get());
		tr->place({ 5, 5 });
		tr->store();
		spr = sprites.make("m_darkness");
		spr->connect<transform_component>(tr.get());
		unit = make_uq<composite_component>();
		unit->add(std::move(tr));
		unit->add(std::move(spr));
		unit->enable();
		units.emplace_back(std::move(unit));

		tr = transforms.make();
		tr->place({ 6, 5 });
		tr->store();
		spr = sprites.make("m_succubus");
		spr->connect<transform_component>(tr.get());
		unit = make_uq<composite_component>();
		unit->add(std::move(tr));
		unit->add(std::move(spr));
		unit->enable();
		units.emplace_back(std::move(unit));

		auto map = fn::ant_generator::generate(std::mt19937{}, 10, 10, 10 * 10 * 100 / 61);
		map.enumerate([this](size_t x, size_t y, unsigned char tile) {
			this->terrain.pset(tile == 0 ? 1 : 3, point2(static_cast<int>(x), static_cast<int>(y)));
		});
	}
}