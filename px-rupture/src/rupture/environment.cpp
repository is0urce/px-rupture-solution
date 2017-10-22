// name: environment.cpp
// type: c++
// auth: is0urce
// desc: class methods implementation

#include "environment.hpp"

#include "draw/message.hpp"

#include "es/body_component.hpp"
#include "es/character_component.hpp"
#include "es/composite_component.hpp"
#include "es/light_component.hpp"
#include "es/transform_component.hpp"

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
		, target_unit(nullptr)
		, target_area(0, 0)
		, target_hover(0, 0)
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

		if (camera) {
			last_step = camera->position();
		}
	}
	void environment::step(point2 const& movement)
	{
		if (player) {
			body_component * body = player->linked<body_component>();
			if (body) {
				last_step = player->position();
				point2 destination = last_step + movement;
				if (stage.is_traversable(destination, body->movement())) {
					player->place(destination);
					pass_turn(last_step);
				}
			}
		}
	}
	void environment::action(unsigned int idx)
	{
		if (player) {
			body_component * body = player->linked<body_component>();
			if (body) {
				character_component * character = body->linked<character_component>();
				if (character) {
					auto skill = character->get(idx);
					if (skill) {
						if (skill->is_targeted()) {
							skill->use(body, { 0, 0 });
							pass_turn();
						}
						else {
							skill->use(body, body);
							pass_turn();
						}
					}
				}
			}
		}
	}
	void environment::pass_turn()
	{
		pass_turn(player ? player->position() : last_step);
	}
	void environment::pass_turn(point2 const& last)
	{
		++turn_number;
		turn_pass = !turn_pass;

		if (player && !turn_pass) {
			player->store(last_step);
		}
		lock_target();

		last_step = last;
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
		body_component * body = nullptr;

		body = b.add_body();
		body->movement().make_traversable(rl::traverse::floor);
		body->blocking().make_transparent();
		body->health().emplace(100);
		body->energy().emplace(50);
		body->set_name("Gnome");
		b.add_player();
		b.add_sprite("m_gnome_mage");
		incarnate(b.add_transform({ 25, 25 }));
		light = b.add_light();
		light->tint = color(1, 1, 1);
		light->elevation = 0.5;
		auto ch = b.add_character();
		ch->learn("hit");
		ch->learn("hit");
		stage.spawn(b.request(), nullptr);

		b.add_sprite("i_cheese");
		b.add_transform({ 21, 24 });
		light = b.add_light();
		light->tint = color(1.0, 1.0, 1.0);
		light->elevation = 0.5;
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

	void environment::spawn(uq_ptr<composite_component> unit, transform_component * hint)
	{
		stage.spawn(std::move(unit), hint);
	}

	transform_component	* environment::possessed() noexcept
	{
		return player;
	}
	transform_component	* environment::target() noexcept
	{
		return target_unit;
	}
	void environment::focus(point2 offset)
	{
		target_hover = offset;
		lock_target();
	}
	void environment::lock_target()
	{
		target_area = target_hover + (player ? player->position() : point2(0, 0));
		body_component * body = stage.anybody(target_area);
		target_unit = body ? body->linked<transform_component>() : nullptr;
	}
}