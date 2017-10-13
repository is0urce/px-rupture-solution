// name: environment.cpp

#include "environment.hpp"

#include "es/transform_component.hpp"
#include "es/composite_component.hpp"
#include "es/light_component.hpp"
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

	void environment::incarnate(transform_component * focus)
	{
		player = focus;
		sprites.target(focus);
		lights.target(focus);
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
		b.add_transform({ 24, 25 });
		auto light = b.add_light();
		light->tint = color(1, 0, 0);
		light->elevation = 0.5;

		auto unit = b.request();
		unit->enable();
		stage.add(std::move(unit));

		b.add_sprite("m_gnome");
		incarnate(b.add_transform({ 25, 25 }));
		light = b.add_light();
		light->tint = color(1, 1, 1);
		light->elevation = 0.5;
		unit = b.request();
		unit->enable();
		stage.add(std::move(unit));

		b.add_sprite("m_succubus");
		b.add_transform({ 30, 25 });
		light = b.add_light();
		light->tint = color(0, 0, 1);
		light->elevation = 0.5;
		unit = b.request();
		unit->enable();
		stage.add(std::move(unit));

		auto map = fn::ant_generator::generate(std::mt19937{}, 50, 50, 50 * 50 * 100 / 61);
		map.enumerate([this](size_t x, size_t y, unsigned char tile) {
			stage.pset(tile == 0 ? 1 : 3, point2(static_cast<int>(x), static_cast<int>(y)));
		});

		spawn("m_doomling", 30, 26);
		spawn("m_nosferatu", 25, 30);

		spawn("m_darkness", 21, 24);
		spawn("m_demon", 31, 27);
		spawn("m_banshee", 28, 32);
		spawn("m_butcher", 23, 32);
	}

	void environment::spawn(const char * name, int x, int y)
	{
		builder b(this);

		b.add_sprite(name);
		b.add_transform({ x, y });
		auto unit = b.request();
		unit->enable();
		stage.add(std::move(unit));
	}
}