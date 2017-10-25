// name: environment.cpp
// type: c++
// auth: is0urce
// desc: class methods implementation

#include "environment.hpp"

#include "draw/message.hpp"

#include "es/animator_component.hpp"
#include "es/body_component.hpp"
#include "es/character_component.hpp"
#include "es/composite_component.hpp"
#include "es/container_component.hpp"
#include "es/light_component.hpp"
#include "es/transform_component.hpp"

#include "es/builder.hpp"

#include <px/fn/ant_generator.hpp>
#include <random>

#include <px/memory/memory.hpp>

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
	}
	void environment::step(point2 const& movement)
	{
		if (player) {
			if (body_component * body = player->linked<body_component>()) {
				point2 destination = player->position() + movement;
				if (stage.is_traversable(destination, body->movement())) {
					start_turn();
					player->place(destination);
					end_turn();
				}
			}
		}
	}
	void environment::action(unsigned int action_idx)
	{
		if (player) {
			if (auto body = player->linked<body_component>()) {
				if (auto character = body->linked<character_component>()) {
					if (auto skill = character->get(action_idx)) {
						bool success = false;
						start_turn();
						if (skill->is_targeted()) {
							success = skill->try_use(body, target_unit ? target_unit->linked<body_component>() : nullptr);
						}
						else {
							success = skill->try_use(body, target_area);
						}
						if (success) {
							end_turn();
						}
					}
				}
			}
		}
	}
	void environment::advance()
	{
		start_turn();
		end_turn();
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
		container_component * cont = nullptr;

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
		ch->learn("sk_v_melee");
		ch->learn("sk_v_melee");
		ch->learn("sk_o_teleport");
		auto anim = b.add_animator("a_door");
		anim->play(0);
		cont = b.add_container();
		for (int i = 0; i != 5; ++i) {
			auto & ip = cont->add(make_uq<rl::item>());
			ip->set_name("orb of number " + std::to_string(i));
		}
		auto pc = b.request();
		pc->set_persistency(persistency::permanent);
		stage.spawn(std::move(pc), nullptr);

		b.add_sprite("i_cheese");
		b.add_transform({ 21, 24 });
		body = b.add_body();
		body->blocking().make_transparent();
		body->health().emplace(10);
		cont = b.add_container();
		for (int i = 0; i != 5; ++i) {
			auto & ip = cont->add(make_uq<rl::item>());
			ip->set_name("essence #" + std::to_string(i));
		}
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

	uq_ptr<composite_component>	& environment::spawn(uq_ptr<composite_component> unit, transform_component * hint)
	{
		return stage.spawn(std::move(unit), hint);
	}

	transform_component	* environment::possessed() noexcept
	{
		return player;
	}
	transform_component	* environment::target() noexcept
	{
		return target_unit;
	}
	point2 environment::area() const noexcept
	{
		return target_area;
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

	void environment::start_turn()
	{
		transforms.store();
		animators.finish_animations();
	}
	void environment::return_turn()
	{
		// rip
		stage.discard([&](composite_component & composite) -> bool {
			if (composite.lifetime() == persistency::permanent) return false;

			auto body = composite.query<body_component>();
			if (body && body->is_dead()) {
				composite.set_persistency(persistency::destroying);

				auto loot = body->linked<container_component>();
				auto pawn = body->linked<transform_component>();

				// spawn loot bag
				if (pawn && loot) {
					auto & bag = spawn("bag", pawn->position());
					if (auto drop = bag->query<container_component>()) {
						drop->take(*loot);
					}
				}
			}

			return composite.lifetime() == persistency::destroying;
		});

		++turn_number;
		turn_pass = false;

		lock_target();
	}
	void environment::end_turn()
	{
		turn_pass = true;
	}
}