// name: environment.cpp
// type: c++
// auth: is0urce
// desc: class methods implementation

#include "environment.hpp"

#include "draw/message.hpp"

#include "es/transform_component.hpp"
#include "es/composite_component.hpp"
#include "es/body_component.hpp"
#include "es/light_component.hpp"

#include "es/builder.hpp"

#include <px/fn/ant_generator.hpp>
#include <random>

namespace px {

	// ctor & dtor

	environment::~environment()
	{
	}
	environment::environment()
		: turn_number(0)
		, turn_pass(true)
		, player(nullptr)
		, run(true)
		, directory("data/save/current", nullptr)
	{
	}

	// methods

	bool environment::is_running() const noexcept
	{
		return run;
	}
	void environment::incarnate(transform_component * camera)
	{
		player = camera;
		sprites.target(camera);
		lights.target(camera);
		messages.target(camera);
	}
	void environment::step(point2 const& movement)
	{
		if (player) {
			last_step = player->position();
			auto destination = last_step + movement;
			if (stage.is_traversable(destination, rl::traverse_options<rl::traverse>{ 1 })) {
				player->place(destination);
				messages.send({ u8"sfx: * ding! *", 0xffffff, 1.0 }, destination);
				messages.send({ "rubble", 0xffffff, 1.0 }, { 25, 25 });
				pass_turn();
			}
		}
	}
	void environment::pass_turn()
	{
		++turn_number;
		turn_pass = !turn_pass;

		if (player && !turn_pass) player->store(last_step);
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

		light_component * light = nullptr;

		b.add_player();
		b.add_sprite("m_gnome");
		incarnate(b.add_transform({ 25, 25 }));
		light = b.add_light();
		light->tint = color(1, 1, 1);
		light->elevation = 0.5;
		stage.spawn(b.request(), nullptr);

		//b.add_sprite("p_candelabra");
		//b.add_transform({ 26, 25 });
		//light = b.add_light();
		//light->tint = color(1, 0, 0);
		//light->elevation = 0.5;
		//stage.spawn(b.request(), nullptr);

		//b.add_sprite("p_candelabra");
		//b.add_transform({ 32, 30 });
		//light = b.add_light();
		//light->tint = color(0, 0, 1);
		//light->elevation = 0.5;
		//stage.spawn(b.request(), nullptr);

		//spawn("p_alchemy", 21, 24);

		spawn("p_chest", 31, 27);
		spawn("p_locker", 28, 32);
		spawn("p_vein", 26, 24);

		b.add_sprite("p_barrel");
		b.add_transform({ 21, 24 });
		//light = b.add_light();
		//light->tint = color(0, 1, 0);
		//light->elevation = 0.5;
		stage.spawn(b.request(), nullptr);

		// set terrain

		auto map = fn::ant_generator::generate(std::mt19937{}, 50, 50, 50 * 50 * 100 / 61);
		map.enumerate([this](size_t x, size_t y, unsigned char tile) {
			stage.pset(tile == 0 ? 1 : 2, point2(static_cast<int>(x), static_cast<int>(y)));
		});

		// set environment variables

		lights.clear_lightmap();
		turn_number = 0;
		turn_pass = true;
	}

	void environment::spawn(const char * name, int x, int y)
	{
		builder b(this);
		b.add_sprite(name);
		b.add_transform({ x, y });
		body_component * bd = b.add_body();
		bd->blocking().make_transparent();
		bd->blocking().make_blocking(rl::traverse::floor);
		b.add_animator("door");
		b.add_character();
		b.add_container();
		b.add_npc();
		b.add_door();
		b.add_deposite();
		b.add_storage();
		b.add_workshop();
		stage.spawn(b.request(), nullptr);
	}
}