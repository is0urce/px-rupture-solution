// name: environment.cpp
// type: c++
// auth: is0urce
// desc: class methods implementation

#include "environment.hpp"

#include "app/settings.hpp"
#include "draw/message.hpp"
#include "draw/visual.hpp"
#include "io/repository.hpp"

#include "es/builder.hpp"

#include "es/animator_component.hpp"
#include "es/body_component.hpp"
#include "es/character_component.hpp"
#include "es/composite_component.hpp"
#include "es/container_component.hpp"
#include "es/light_component.hpp"
#include "es/player_component.hpp"
#include "es/sprite_component.hpp"
#include "es/transform_component.hpp"

#include <px/dev/assert.hpp>
#include <px/memory/memory.hpp>
#include <px/algorithm/fov.hpp>
#include <px/algorithm/bresenham.hpp>

#include <algorithm>

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

        std::mt19937::result_type noize[rng.state_size];
        std::random_device device;
        std::generate(std::begin(noize), std::end(noize), std::ref(device));
        std::seed_seq seq(std::begin(noize), std::end(noize));
        rng = std::mt19937(seq);
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
            else {
                if (auto versus = stage.anybody(destination)) {
                    target_unit = versus->linked<transform_component>();
                    action(0);
                }
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
        light->tint = color(0.7, 0.7, 0.7);
        light->elevation = 0.5;
        light->is_on = true;

        // stats
        auto body = unit_builder.add_body();
        body->movement().make_traversable(rl::traverse::floor);
        body->blocking().make_transparent();
        body->health().emplace(50);
        body->energy().emplace(100);
        body->set_name("Gnome");
        body->join_faction(1);
        auto character = unit_builder.add_character();
        character->learn("sk_v_melee", "sk_s_smite", "sk_s_rend", "sk_s_flurry", "sk_v_sound", "sk_o_teleport");

        // inventory
        auto container = unit_builder.add_container();
        auto weapon = make_uq<rl::item>();
        weapon->add(body_component::enhancement_type::real(rl::effect::damage, 0, 6));
        weapon->add(body_component::enhancement_type::zero(rl::effect::equipment, static_cast<body_component::enhancement_type::integer_type>(rl::equipment::hand)));
        weapon->set_name("Sword");
        container->add(std::move(weapon));

        // intrinsic effect item
        auto hide = make_uq<rl::item>();
        hide->setup_entity("intrinsic", "i_hide");
        hide->add(body_component::enhancement_type::real(rl::effect::accuracy, 0, 0.85));
        hide->add(body_component::enhancement_type::real(rl::effect::critical, 0, 0.05));
        hide->add(body_component::enhancement_type::real(rl::effect::dodge, 0, 0.05));
        body->get_mannequin().equip(rl::equipment::hide, std::move(hide));

        for (int i = 0; i != 10; ++i) {
            auto item = make_uq<rl::item>();
            item->add(body_component::enhancement_type::real(rl::effect::ingredient_power, static_cast<body_component::enhancement_type::integer_type>(rl::craft_activity::alchemy), 1));
            item->add(body_component::enhancement_type::integral(rl::effect::essence, 0, 3));
            item->set_name("petal");
            item->make_stacking();
            container->acquire(std::move(item));
            auto ore = make_uq<rl::item>();
            ore->add(body_component::enhancement_type::real(rl::effect::ingredient_power, static_cast<body_component::enhancement_type::integer_type>(rl::craft_activity::blacksmith), 1));
            ore->add(body_component::enhancement_type::integral(rl::effect::essence, 0, 3));
            ore->make_stacking();
            ore->set_name("ore");
            container->acquire(std::move(ore));
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
            auto[pawn, body, loot] = composite.unwind<transform_component, body_component, container_component>();
            if (body && loot && loot->item_count() != 0) {
                auto & bag = spawn("bag", pawn->position());
                if (auto drop = bag->qlink<container_component, body_component, transform_component>()) {
                    drop->give(*loot);
                }
            }
        });

        // update visual effect unit tracking
        for (auto & fx : vfx) {
            fx.pawn->place(fx.track ? fx.track->position() : fx.finish);
            fx.pawn->store(fx.start);
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

                    auto lvl = body->level();

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

    rl::hit_result environment::hit(body_component const& source, body_component const& target) {
        rl::hit_result result = rl::hit_result::miss;

        double accuracy = source.accumulate({ rl::effect::accuracy }).magnitude0;
        double dodge = target.accumulate({ rl::effect::dodge }).magnitude0;

        auto score = accuracy - dodge;

        // level difference attentuation
        if (source.linked<transform_component>() == player) {
            auto spread = target.level() - source.level();
            score -= spread > 0 ? spread * 0.05 : 0;
        }

        if (std::uniform_real_distribution<double>{}(rng) < score) {
            result = rl::hit_result::connects;
            double critical = source.accumulate({ rl::effect::critical }).magnitude0;
            if (std::uniform_real_distribution<double>{}(rng) < critical) {
                result = rl::hit_result::critical;
            }
        }
        else {
            if (std::uniform_real_distribution<double>{}(rng) < 0.05) {
                result = rl::hit_result::failure;
            }
        }

        return result;
    }

    std::tuple<float, rl::damage_type> environment::dps(body_component const& source) const {
        auto dps = source.accumulate({ rl::effect::damage });
        return { static_cast<float>(dps.magnitude0), rl::damage_type::pure };
    }

    void environment::damage(body_component & target, int damage, rl::damage_type /*damage_type*/) {
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
    }

    void environment::emit_visual(std::string const& name, point2 start, point2 finish, transform_component const* track) {
        if (auto sprite = sprites.make(name)) {
            auto pawn = make_uq<transform_component>();

            // setup

            pawn->store(start);
            pawn->place(finish);

            sprite->connect(pawn.get());

            // activate

            pawn->activate();
            sprite->activate();

            vfx.push_back({ start, finish, std::move(pawn), std::move(sprite), nullptr, track });
        }
        else {
            debug("environment::evit_visual(..) - '" + name + "' do not exists");
        }
    }

    void environment::emit_animation(std::string const& name, unsigned int clip_id, point2 start, point2 finish, transform_component const* track) {
        auto sprite = sprites.make("e_empty");
        auto animation = animators.make(name);
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

            vfx.push_back({ start, finish, std::move(pawn), std::move(sprite), std::move(animation), track });
        }
        else {
            debug("environment::evit_visual(..) - sprite or animation do not exists, name='" + name + "'");
        }
    }

    bool environment::in_sight(body_component const& body, point2 const& location) const {
        auto pawn = body.linked<transform_component>();
        if (!pawn) return false;

        fov sight;
        sight.calculate(pawn->position(), 25, [&](int x, int y) { return stage.is_transparent({ x, y }); });
        return sight.contains(location);
    }

    bool environment::in_line(body_component const& body, point2 const& destination) const {
        auto pawn = body.linked<transform_component>();
        if (!pawn) return false;

        bool traverse = true;
        bresenham::line(pawn->position(), destination, [&](int x, int y) {
            point2 position(x, y);
            if (position != pawn->position()) traverse &= stage.is_traversable(position, body.movement());
        });
        return traverse;
    }

    int environment::roll(int min, int max) {
        return std::uniform_int_distribution<>{ min, max }(rng);
    }
}