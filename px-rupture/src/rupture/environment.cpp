// name: environment.cpp

#include "environment.hpp"

#include "es/transform_component.hpp"
#include "es/composite_component.hpp"
#include "es/light_component.hpp"
#include "es/builder.hpp"

#include <px/fn/ant_generator.hpp>
#include <random>

namespace px {

	environment::~environment()
	{
	}
	environment::environment()
		: turn_number(0)
		, turn_pass(true)
		, player(nullptr)
		, run(true)
	{
	}

	// methods

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
			if (stage.is_traversable(destination, rl::traverse_options<rl::traverse>{ 1 })) {
				player->place(destination);
				player->store();
				pass_turn();
			}
		}
	}
	void environment::pass_turn()
	{
		++turn_number;
		turn_pass = !turn_pass;
	}
	unsigned int environment::current_turn() const noexcept
	{
		return turn_number;
	}
	bool environment::turn_passed() const noexcept
	{
		return turn_pass;
	}

	void environment::start()
	{
		builder b(this);

		b.add_sprite("m_imp");
		b.add_transform({ 24, 25 });
		auto light = b.add_light();
		light->tint = color(1, 0, 0);
		light->elevation = 0.5;
		stage.spawn(b.request(), nullptr);

		b.add_sprite("m_gnome");
		incarnate(b.add_transform({ 25, 25 }));
		light = b.add_light();
		light->tint = color(1, 1, 1);
		light->elevation = 0.5;
		stage.spawn(b.request(), nullptr);

		b.add_sprite("m_succubus");
		b.add_transform({ 30, 25 });
		light = b.add_light();
		light->tint = color(0, 0, 1);
		light->elevation = 0.5;
		stage.spawn(b.request(), nullptr);

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
		auto transform = b.add_transform({ x, y });
		stage.spawn(b.request(), transform);
	}
}