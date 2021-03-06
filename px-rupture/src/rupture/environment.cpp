// name: environment.cpp
// type: c++
// auth: is0urce
// desc: class methods implementation

#include "environment.hpp"

#include "facility.hpp"

#include "app/settings.hpp"
#include "draw/message.hpp"
#include "draw/visual.hpp"
#include "io/repository.hpp"

#include "es/builder.hpp"
#include "rl/mechanic.hpp"

#include "es/animator_component.hpp"
#include "es/body_component.hpp"
#include "es/character_component.hpp"
#include "es/composite_component.hpp"
#include "es/container_component.hpp"
#include "es/light_component.hpp"
#include "es/player_component.hpp"
#include "es/sprite_component.hpp"
#include "es/transform_component.hpp"
#include "es/value_component.hpp"

#include <px/algorithm/bresenham.hpp>
#include <px/algorithm/fov.hpp>
#include <px/algorithm/random.hpp>
#include <px/dev/assert.hpp>
#include <px/memory/memory.hpp>

#include <algorithm>
#include <utility>

namespace px {

    // ctor & dtor

    environment::~environment() {
        end();
    }

    environment::environment(facility * core)
        : factory(core)
        , run(true)
        , ingame(false)
        , player(nullptr)
        , opened_workshop(rl::craft_activity::none)
        , turn_number(0)
        , turn_pass(true) {
        parent = make_uq<repository>(std::string(settings::save_path) + "base");
        current = make_uq<repository>(std::string(settings::save_path) + "current", parent.get());

        stage.set_leave_event([this](point2 const& cell) { save_scene(cell); });
        stage.set_enter_event([this](point2 const& cell) { load_scene(cell); });

        clear();
        seed(0);
    }

    // methods

    bool environment::is_ingame() const noexcept {
        return ingame;
    }

    bool environment::is_running() const noexcept {
        return run;
    }

    void environment::shutdown() {
        run = false;
    }

    void environment::begin() {
        // clear repository and reset environment variables
        end();
        ingame = true;
    }

    void environment::end() {
        clear();
        ingame = false;
    }

    void environment::clear() {
        stage.unload();
        stage.clear_units();
        vfx.clear();
        factory->get_factory<light_system>().clear_lightmap();
        current->clear();

        player = nullptr;

        target_unit = nullptr;
        target_area = { 0, 0 };
        target_hover = { 0, 0 };
        opened_workshop = rl::craft_activity::none;

        turn_number = 0;
        turn_pass = true;
    }

    bool environment::has_control() const {
        if (turn_pass) return false;

        auto body = player ? player->linked<body_component>() : nullptr;
        return body && body->is_alive();
    }

    void environment::incarnate(transform_component * camera) {
        player = camera;

        factory->get_factory<sprite_system>().target(camera);
        factory->get_factory<sound_system>().target(camera);
        factory->get_factory<light_system>().target(camera);

        messages.target(camera);

        if (camera) {
            stage.focus(camera->position());
            factory->get_factory<light_system>().recalculate();
        }
        else {
            factory->get_factory<light_system>().clear_lightmap();
        }
    }

    // player move action
    void environment::step(point2 const& movement) {
        if (!has_control()) return;

        target(movement);

        if (auto body = player->linked<body_component>()) {
            point2 destination = player->position() + movement;
            if (stage.is_traversable(destination, body->movement())) {
                start_turn();
                player->place(destination);

                // play step sound
                unsigned int sound_variant = std::uniform_int_distribution<unsigned int>{ 0, 2 }(rng)+((turn_number % 2 == 0) ? 0 : 3);
                std::string sound_name = settings::sound_path + std::string("snd_foot") + std::to_string(sound_variant) + ".wav";
                play_sound(sound_name, 0.2, destination);

                end_turn(1);
            }
            else {
                if (auto versus = stage.anybody(destination)) {
                    target_unit = versus->linked<transform_component>();
                    action(0);
                }
            }
        }
    }

    // player ability use action
    bool environment::action(unsigned int action_idx) {
        bool success = false;
        if (has_control()) {
            auto[user, person] = player->unwind<body_component, character_component>();
            if (auto ability = person ? person->get(action_idx) : nullptr) {
                start_turn();
                success = ability->is_targeted()
                    ? ability->try_use(user, target_unit ? target_unit->linked<body_component>() : nullptr)
                    : ability->try_use(user, target_area);
                if (success) {
                    end_turn(1);
                }
            }
        }
        return success;
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

    transform_component * environment::create_player(unsigned int specialization) {
        builder unit_builder(factory);

        // position
        auto pawn = unit_builder.add_transform({ 1966, 860 });

        auto light = unit_builder.add_light();
        if (light) {
            light->tint = { 0.2, 0.2, 0.2 };
            light->elevation = 0.5;
            light->is_on = true;
        }

        // stats
        auto body = unit_builder.add_body();
        body->set_name("Gnome");
        body->set_level(0);
        body->set_experience(0);
        body->movement().make_traversable(rl::traverse::floor);
        body->blocking().make_transparent();
        body->health().emplace(50);
        body->energy().emplace(100);

        body->join_faction(1);

        auto character = unit_builder.add_character();
        character->learn("sk_v_melee");                                             // basic attack

        // inventory
        auto container = unit_builder.add_container();

        // value vault
        unit_builder.add_value();

        // as player
        unit_builder.add_player();

        // specialization select

        if (specialization == 1) {
            unit_builder.add_sprite("m_gnome_warrior");
            unit_builder.add_animator("a_gnome_warrior");
            character->learn("sk_s_smite", "sk_s_rend", "sk_s_flurry", "sk_s_charge");          // class

            auto weapon = make_uq<rl::item>();
            weapon->add(body_component::enhancement_type::real(rl::effect::damage, 0x00, 6));
            weapon->add(body_component::enhancement_type::zero(rl::effect::equipment, static_cast<body_component::enhancement_type::integer_type>(rl::equipment::hand)));
            weapon->set_name("rusty shortsword");
            body->get_mannequin().equip(rl::equipment::hand, std::move(weapon));
        }
        else if (specialization == 2) {
            unit_builder.add_sprite("m_gnome_archer");
            unit_builder.add_animator("a_gnome_archer");
            character->learn("sk_i_lightning", "sk_v_drain", "sk_v_explosives", "sk_v_blink");  // test

            auto weapon = make_uq<rl::item>();
            weapon->add(body_component::enhancement_type::real(rl::effect::damage, 0x00, 6));
            weapon->add(body_component::enhancement_type::zero(rl::effect::equipment, static_cast<body_component::enhancement_type::integer_type>(rl::equipment::hand)));
            weapon->set_name("throwing knife");
            body->get_mannequin().equip(rl::equipment::hand, std::move(weapon));

            auto item = make_uq<rl::item>();
            item->add(body_component::enhancement_type::real(rl::effect::ingredient_power, static_cast<body_component::enhancement_type::integer_type>(rl::craft_activity::alchemy), 1));
            item->add(body_component::enhancement_type::integral(rl::effect::essence, 0x00, 3));
            item->set_name("salt");
            item->make_stacking();
            item->set_current_stack(5);
            container->acquire(std::move(item));

            auto ore = make_uq<rl::item>();
            ore->add(body_component::enhancement_type::real(rl::effect::ingredient_power, static_cast<body_component::enhancement_type::integer_type>(rl::craft_activity::blacksmith), 1));
            ore->add(body_component::enhancement_type::integral(rl::effect::essence, 0x00, 3));
            ore->make_stacking();
            ore->set_name("smuggled paperweight");
            ore->set_current_stack(6);
            container->acquire(std::move(ore));

            auto pot = make_uq<rl::item>();
            pot->add(body_component::enhancement_type::zero(rl::effect::useable, 0x00));
            pot->add(body_component::enhancement_type::real(rl::effect::hp_heal, 0x00, 100.0));
            pot->set_name("purified & certified potion");
            pot->set_tag("i_hp_potion_dev");
            pot->make_stacking();
            container->acquire(std::move(pot));
        }
        else if (specialization == 3) {
            unit_builder.add_sprite("m_gnome_mage");
            unit_builder.add_animator("a_gnome_mage");

            auto weapon = make_uq<rl::item>();
            weapon->add(body_component::enhancement_type::real(rl::effect::damage, 0x00, 6));
            weapon->add(body_component::enhancement_type::zero(rl::effect::equipment, static_cast<body_component::enhancement_type::integer_type>(rl::equipment::hand)));
            weapon->set_name("fireplace poker");
            body->get_mannequin().equip(rl::equipment::hand, std::move(weapon));
        }
        else {
            unit_builder.add_sprite("m_gnome_warrior");
            unit_builder.add_animator("a_gnome_warrior");
            px_debug("no specialization");
        }

        // intrinsic effect item
        auto hide = make_uq<rl::item>();
        hide->setup_entity("intrinsic", "i_hide");
        hide->add(body_component::enhancement_type::real(rl::effect::accuracy, 0x00, 0.85));
        hide->add(body_component::enhancement_type::real(rl::effect::critical, 0x00, 0.05));
        hide->add(body_component::enhancement_type::real(rl::effect::dodge, 0x00, 0.05));
        body->get_mannequin().equip(rl::equipment::hide, std::move(hide));

        // compose unit
        auto composite = unit_builder.request();
        composite->set_persistency(persistency::permanent);

        stage.spawn(std::move(composite));

        return pawn;
    }

    void environment::prewarm() {
        spawn("anvil", { 1966, 861 });
        spawn("alchemy", { 1967, 861 });
    }

    uq_ptr<composite_component>	& environment::spawn(uq_ptr<composite_component> unit) {
        return stage.spawn(std::move(unit));
    }

    transform_component	* environment::controlled() noexcept {
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
        factory->get_factory<transform_system>().store();
        factory->get_factory<animator_system>().finish_animations();
        opened_workshop = rl::craft_activity::none;
        vfx.clear();
    }

    // execute after npc turn
    void environment::return_turn() {

        auto player_body = player ? player->linked<body_component>() : nullptr;

        if (player) {
            // stream world
            stage.focus(player->position());

            // results screen
            auto person = player_body ? player_body->linked<character_component>() : nullptr;
            if (person && player_body->is_dead()) {
                person->add_trait("c_game_over");
            }
        }

        auto player_unit = player_body ? player_body->linked<composite_component>() : nullptr;
        auto player_vault = player_unit ? player_unit->query<value_component>() : nullptr;

        // death of units
        stage.discard([&](composite_component & composite) {
            auto[pawn, discarded_body, loot] = composite.unwind<transform_component, body_component, container_component>();
            if (discarded_body) {
                if (loot && loot->item_count() != 0) {
                    auto & bag = spawn("bag", pawn->position());
                    if (auto drop = bag->qlink<container_component, body_component, transform_component>()) {
                        drop->give(*loot);
                    }
                }
                if (discarded_body->has_health() && player_vault) {
                    player_vault->mod_int("slayed", 1);
                }
            }
        });

        // update visual effect unit tracking
        for (auto & fx : vfx) {
            if (auto & pawn = fx.pawn) {
                pawn->place(fx.track ? fx.track->position() : fx.finish);
                pawn->store(fx.start);
            }
        }

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
    void environment::give_experience(unsigned int exp, unsigned int source_level) {
        if (player && exp != 0) {
            if (auto body = player->linked<body_component>()) {
                if (!body->is_dead()) {

                    auto const lvl = body->level();

                    // apply softcap
                    if (lvl == source_level + 1) exp /= 2;
                    else if (lvl == source_level + 2) exp /= 10;
                    else if (lvl >= source_level + 3) exp = 1;

                    body->mod_experience(exp);
                    popup("+ " + std::to_string(exp) + " exp", color(0.8, 0, 0.9), player->position());
                }
            }
        }
    }

    rl::hit_result environment::hit(body_component const& source, body_component const& target) {
        bool const diminish = source.linked<transform_component>() == player;
        return mechanic::hit(source, target, diminish, rng);
    }

    std::tuple<float, rl::damage_type> environment::dps(body_component const& source) const {
        return mechanic::dps(source);
    }

    int environment::damage(body_component & target, int dmg, rl::damage_type source) {
        int const damage_done = mechanic::damage(target, dmg, source);
        // send popup notification
        if (auto pawn = target.linked<transform_component>()) {
            popup(std::to_string(damage_done), pawn == player ? 0xff0000 : 0xffff00, pawn->position());
        }
        return damage_done;
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
    bool environment::close_workshop() {
        bool const was_active = opened_workshop != rl::craft_activity::none;
        opened_workshop = rl::craft_activity::none;
        return was_active;
    }

    void environment::function_edit(std::uint32_t /*idx*/) {
    }

    void environment::play_sound(std::string const& sound, double volume, point2 const& location) {
        factory->get_factory<sound_system>().play_sound(sound, volume, location);
    }

    void environment::play_sound(std::string const& sound, double volume) {
        factory->get_factory<sound_system>().play_sound(sound, volume);
    }

    void environment::emit_visual(std::string const& name, point2 start, point2 finish, transform_component const* track) {
        if (auto sprite = factory->get_factory<sprite_system>().make(name)) {
            auto pawn = make_uq<transform_component>();

            // setup

            pawn->store(start);
            pawn->place(finish);

            sprite->connect(pawn.get());

            // activate

            pawn->activate();
            sprite->activate();

            vfx.push_back({ start, finish, std::move(pawn), std::move(sprite), nullptr, nullptr, track });
        }
        else {
            px_debug("environment::evit_visual(..) - '" + name + "' do not exists");
        }
    }

    void environment::emit_animation(std::string const& name, unsigned int clip_id, point2 start, point2 finish, transform_component const* track) {
        auto sprite = factory->get_factory<sprite_system>().make("e_empty");
        auto animation = factory->get_factory<animator_system>().make(name);
        if (sprite && animation) {
            auto pawn = make_uq<transform_component>();

            // setup

            pawn->store(start);
            pawn->place(finish);

            sprite->connect(pawn.get());

            animation->connect(sprite.get());
            animation->play(clip_id);

            // activate

            pawn->activate();
            animation->activate();
            sprite->activate();

            vfx.push_back({ start, finish, std::move(pawn), std::move(sprite), std::move(animation), nullptr, track });
        }
        else {
            px_debug("environment::evit_visual(..) - sprite or animation do not exists, name='" + name + "'");
        }
    }

    void environment::emit_light(point2 location, color const& light) {
        auto pawn = make_uq<transform_component>();
        auto lamp = factory->get_factory<light_system>().make();

        if (!pawn) return;
        if (!lamp) return;

        // setup
        lamp->tint = light;
        lamp->elevation = 0;
        lamp->is_on = true;
        lamp->source = light_source::point;
        lamp->connect(pawn.get());
        lamp->activate();

        pawn->store(location);
        pawn->place(location);
        pawn->activate();

        vfx.push_back({ location, location, std::move(pawn), nullptr, nullptr, std::move(lamp), nullptr });
    }

    bool environment::in_sight(body_component const& body, point2 const& location) const {
        auto pawn = body.linked<transform_component>();
        if (!pawn) return false;

        int max_distance = pawn->position().king_distance(location) + 1; // plus one to be included
        if (max_distance > 25) return false;

        fov sight;
        sight.calculate(pawn->position(), max_distance, [&](int x, int y) { return stage.is_transparent({ x, y }); });
        return sight.contains(location);
    }

    bool environment::in_line(body_component const& body, point2 const& destination) const {
        bool traverse = false;
        if (auto pawn = body.linked<transform_component>()) {
            traverse = true;
            bresenham::line(pawn->position(), destination, [&](int x, int y) {
                point2 position(x, y);
                traverse &= position == pawn->position() || is_traversable(position, body);
            });
        }
        return traverse;
    }

    int environment::roll(int min, int max) {
        return std::uniform_int_distribution<>{ min, max }(rng);
    }

    bool environment::is_traversable(point2 const location, body_component const& body) const {
        return stage.is_traversable(location, body.movement());
    }

    std::tuple<bool, point2> environment::neighbour(point2 const location, unsigned int direction) const {
        std::tuple<bool, point2> result{ false, {} };

        switch (direction) {
        case 0:
            result = { true, location.moved({ +1, +0 }) };
            break;
        case 1:
            result = { true, location.moved({ +1, +1 }) };
            break;
        case 2:
            result = { true, location.moved({ +0, +1 }) };
            break;
        case 3:
            result = { true, location.moved({ -1, +1 }) };
            break;
        case 4:
            result = { true, location.moved({ -1, +0 }) };
            break;
        case 5:
            result = { true, location.moved({ -1, -1 }) };
            break;
        case 6:
            result = { true, location.moved({ +0, -1 }) };
            break;
        case 7:
            result = { true, location.moved({ +1, -1 }) };
            break;
        default:
            break;
        }

        return result;
    }

    void environment::query_targets(point2 const& center, unsigned int radius, bool require_los, scene::area_query fn) const {
        if (require_los) {
            fov los;
            los.calculate(center, radius + 1, [&](int x, int y) { return stage.is_transparent({ x, y }); });
            stage.query_targets(center, radius, [&](point2 const& location, body_component * target) {
                if (distance(center, location) <= static_cast<int>(radius) && los.contains(location)) {
                    fn(location, target);
                }
            });
        }
        else {
            stage.query_targets(center, radius, [&](point2 const& location, body_component * target) {
                if (distance(center, location) <= static_cast<int>(radius)) {
                    fn(location, target);
                }
            });
        }
    }

    void environment::seed(unsigned int seed) {
        random::seed_noize(rng, seed);
    }

    void environment::set_volume(sound_channel group, double volume) {
        factory->get_factory<sound_system>().set_volume(group, volume);
    }

    void environment::play_music(std::string const& track_name) {
        factory->get_factory<sound_system>().play_music(track_name, 1.0f);
    }

    void environment::enqueue_music(std::string const &name) {
        factory->get_factory<sound_system>().enqueue_music(name);
    }

    facility * environment::get_facility() noexcept {
        return factory;
    }

    notification_system * environment::get_notifications() noexcept {
        return &messages;
    }

    scene * environment::get_scene() noexcept {
        return &stage;
    }
}