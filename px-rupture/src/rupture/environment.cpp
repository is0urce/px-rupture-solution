// name: environment.cpp
// type: c++
// auth: is0urce
// desc: class methods implementation

#include "environment.hpp"

#include "app/settings.hpp"
#include "draw/message.hpp"
#include "draw/visual.hpp"
#include "io/repository.hpp"

#include "es/animator_component.hpp"
#include "es/body_component.hpp"
#include "es/character_component.hpp"
#include "es/composite_component.hpp"
#include "es/container_component.hpp"
#include "es/light_component.hpp"
#include "es/player_component.hpp"
#include "es/sprite_component.hpp"
#include "es/transform_component.hpp"

#include "es/builder.hpp"

#include <px/memory/memory.hpp>

namespace px {

	// ctor & dtor

	environment::~environment() {
		clear();
	}

	environment::environment() {
		parent = make_uq<repository>(std::string(settings::save_path) + "base");
		current = make_uq<repository>(std::string(settings::save_path) + "current", parent.get());

		stage.set_leave_event([&](point2 const& cell) { save_scene(cell); });
		stage.set_enter_event([&](point2 const& cell) { load_scene(cell); });

		clear();
	}

	// methods

	bool environment::is_running() const noexcept {
		return run;
	}

	void environment::shutdown() {
		run = false;
	}

	bool environment::has_control() const {
		if (turn_pass) return false;
		if (!player) return false;
		auto body = player->linked<body_component>();
		if (!body) return false;
		auto hp = body->health();
		if (!hp) return false;
		return !hp->empty();
	}

	void environment::incarnate(transform_component * camera) {

		if (camera) {
			stage.focus(camera->position());
		}

		player = camera;
		sprites.target(camera);
		messages.target(camera);
		lights.target(camera);
		lights.recalculate();
	}

	// player move action
	void environment::step(point2 const& movement) {
		if (!has_control()) return;

		if (auto body = player->linked<body_component>()) {
			point2 destination = player->position() + movement;
			stage.focus(destination);
			if (stage.is_traversable(destination, body->movement())) {
				start_turn();
				player->place(destination);
				end_turn(1);
			}
		}
	}

	// player ability use action
	void environment::action(unsigned int action_idx) {
		if (!has_control()) return;

		if (auto body = player->linked<body_component>()) {
			if (auto character = body->linked<character_component>()) {
				if (skill * ability = character->get(action_idx)) {
					bool success = false;
					start_turn();
					if (ability->is_targeted()) {
						success = ability->try_use(body, target_unit ? target_unit->linked<body_component>() : nullptr);
					}
					else {
						success = ability->try_use(body, target_area);
					}
					if (success) {
						end_turn(1);
					}
				}
			}
		}
	}

	void environment::advance() {
		if (!has_control()) return;

		start_turn();
		end_turn(1);
	}

	void environment::use(unsigned int /*mods*/) {
		if (!has_control()) return;

		if (target_unit) {
			auto useable = target_unit->linked<body_component>();
			auto user = player->linked<body_component>();
			if (user && useable) {
				start_turn();
				useable->try_use(user, this);
			}
		}
	}

	unsigned int environment::current_turn() const noexcept {
		return turn_number;
	}
	bool environment::turn_passed() const noexcept {
		return turn_pass;
	}

	void environment::start() {
		// clear repository and reset environment variables
		clear();

		incarnate(create_player());
		prewarm();
	}

	transform_component * environment::create_player() {
		builder unit_builder(this);

		// position
		auto pawn = unit_builder.add_transform({ 1966, 860 });

		// appearance
		unit_builder.add_sprite("m_gnome_warrior");
		auto light = unit_builder.add_light();
		light->tint = color(0.3, 0.3, 0.3);
		light->elevation = 0.5;
		light->is_on = true;

		// stats
		auto body = unit_builder.add_body();
		body->movement().make_traversable(rl::traverse::floor);
		body->blocking().make_transparent();
		body->health().emplace(100);
		body->energy().emplace(50);
		body->set_name("Gnome");
		body->join_faction(1);
		auto character = unit_builder.add_character();
		character->learn("sk_v_melee", "sk_s_smite", "sk_s_rend", "sk_s_flurry");

		// inventory
		auto container = unit_builder.add_container();
		auto weapon = make_uq<rl::item>();
		weapon->add(body_component::enhancement_type::real(rl::effect::damage, 0, 6));
		weapon->add(body_component::enhancement_type::zero(rl::effect::equipment, static_cast<body_component::enhancement_type::integer_type>(rl::equipment::hand)));
		weapon->set_name("Sword");
		container->add(std::move(weapon));
		for (int i = 0; i != 10; ++i) {
			auto item = make_uq<rl::item>();
			item->add(body_component::enhancement_type::real(rl::effect::ingredient_power, static_cast<body_component::enhancement_type::integer_type>(rl::craft_activity::alchemy), 1));
			item->add(body_component::enhancement_type::integral(rl::effect::essence, 0, 3));
			item->set_name("petal");
			container->add(std::move(item));
			auto ore = make_uq<rl::item>();
			ore->add(body_component::enhancement_type::real(rl::effect::ingredient_power, static_cast<body_component::enhancement_type::integer_type>(rl::craft_activity::blacksmith), 1));
			ore->add(body_component::enhancement_type::integral(rl::effect::essence, 0, 3));
			ore->set_name("ore");
			container->add(std::move(ore));
		}

		// as player
		unit_builder.add_player();

		// compose unit
		auto composite = unit_builder.request();
		composite->set_persistency(persistency::permanent);

		body->equip(0); // equip after linking equipment and body
		stage.spawn(std::move(composite));

		return pawn;
	}

	void environment::prewarm() {
		spawn("anvil", { 1966, 861 });
		spawn("alchemy", { 1967, 861 });
	}

	void environment::end() {
		clear();
	}

	void environment::clear() {
		stage.unload();
		stage.clear_units();
		current->clear();

		player = nullptr;
		target_unit = nullptr;
		target_area = { 0, 0 };
		target_hover = { 0, 0 };
		opened_workshop = rl::craft_activity::none;
		run = true;
		turn_number = 0;
		turn_pass = true;
	}

	uq_ptr<composite_component>	& environment::spawn(uq_ptr<composite_component> unit) {
		return stage.spawn(std::move(unit));
	}

	transform_component	* environment::possessed() noexcept {
		return player;
	}
	transform_component	* environment::target() noexcept {
		return target_unit;
	}
	point2 environment::area() const noexcept {
		return target_area;
	}
	void environment::target(point2 offset) {
		target_hover = offset;
		lock_target();
	}

	void environment::lock_target() {
		target_area = target_hover + (player ? player->position() : point2(0, 0));
		body_component * body = stage.anybody(target_area);
		target_unit = body ? body->linked<transform_component>() : nullptr;
	}

	void environment::start_turn() {
		transforms.store();
		animators.finish_animations();
		opened_workshop = rl::craft_activity::none;
		vfx.clear();
	}

	// execute after npc turn
	void environment::return_turn() {

		// death of units
		stage.discard([&](composite_component & composite) {
			if (auto pawn = composite.linked<transform_component>()) {
				if (auto body = pawn->linked<body_component>()) {
					if (auto loot = body->linked<container_component>()) {

						// loot drop
						if (loot->size() != 0) {
							auto & bag = spawn("bag", pawn->position());
							if (auto drop = bag->qlink<container_component, body_component, transform_component>()) {
								drop->take(*loot);
							}
						}
					}
				}
			}
		});

		// select hovered unit
		lock_target();

		turn_pass = false;
	}

	void environment::end_turn(unsigned int turns) {
		turn_number += turns;
		turn_pass = true;
	}

	void environment::popup(std::string text, color tint, point2 location) {
		messages.send({ text, tint, 1.0 }, location);
	}
	void environment::popup(std::string text, color tint) {
		if (player) {
			messages.send({ text, tint, 1.0 }, player->position());
		}
	}

	int	environment::distance(point2 const& a, point2 const& b) const {
		return a.king_distance(b);
	}

	// give experience to current player
	void environment::give_experience(int exp, int source_level) {
		if (player && exp != 0) {
			if (auto body = player->linked<body_component>()) {
				if (!body->is_dead()) {

					int lvl = body->level();

					// apply softcap
					if (lvl == source_level + 1) exp /= 2;
					else if (lvl == source_level + 2) exp /= 10;
					else if (lvl >= source_level + 3) exp = 1;

					body->set_experience(exp + body->experience());
					popup("+ " + std::to_string(exp) + " exp", color(0.8, 0, 0.9), player->position());
				}
			}
		}
	}

	rl::hit_result environment::hit(body_component const& /*source*/, body_component const& /*target*/) const {
		return rl::hit_result::connects;
	}
	std::tuple<int, rl::damage_type> environment::dps(body_component const& source) const {
		auto dps = source.accumulate(body_component::enhancement_type::zero(rl::effect::damage));
		int damage = static_cast<int>(dps.magnitude0);
		return { damage, rl::damage_type::pure };
	}
	void environment::damage(body_component & target, int damage, rl::damage_type /*discard*/) {
		if (auto & hp = target.health()) {
			hp->damage(damage);

			// send popup notification
			if (transform_component * pawn = target.linked<transform_component>()) {
				popup(std::to_string(damage), pawn == player ? 0xff0000 : 0xffff00, pawn->position());
			}
		}
	}

	// check if workshop activity available
	bool environment::has_access(rl::craft_activity station) const noexcept {
		return opened_workshop == station;
	}

	// start workshop activity
	void environment::open_workshop(rl::craft_activity station) {
		opened_workshop = station;
	}

	// finish any workshop activity
	void environment::close_workshop() {
		opened_workshop = rl::craft_activity::none;
	}

	void environment::function_edit(std::uint32_t /*idx*/) {
		stage.pset(3, area());
	}

	void environment::emit_visual(std::string const& name, point2 start, point2 finish, transform_component * track) {
		auto sprite = sprites.make(name);
		auto tr = make_uq<transform_component>();
		tr->store(start);
		tr->place(finish);

		sprite->connect<transform_component>(track ? track : tr.get());
		sprite->activate();

		vfx.push_back({ start, finish, std::move(tr), std::move(sprite), nullptr });
	}

	void environment::emit_animation(std::string const& name, unsigned int clip_id, point2 start, point2 finish, transform_component * track) {
		auto sprite = sprites.make("e_empty");
		auto animation = animators.make(name);
		auto tr = make_uq<transform_component>();
		tr->store(start);
		tr->place(finish);

		sprite->connect<transform_component>(track ? track : tr.get());
		sprite->activate();

		animation->connect<sprite_component>(sprite.get());
		animation->activate();
		animation->play(clip_id);

		vfx.push_back({ start, finish, std::move(tr), std::move(sprite), std::move(animation) });
	}
}