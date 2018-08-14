// name: shell.cpp
// type: c++
// auth: is0urce
// type: class methods implementation

#include "shell.hpp"

#include "app/document.hpp"
#include "app/settings.hpp"

#include <px/common/vector.hpp>

#include <lodepng.h>

#include <stdexcept>
#include <string>

namespace px {

    // ctor & dtor

    shell::~shell() = default;

    shell::shell(unsigned int start_width, unsigned int start_height, cfg * settings)
        : renderer(start_width, start_height)
        , config(settings)
        , ui(start_width, start_height, this, settings)
        , width(start_width)
        , height(start_height) {
        connect_managers();
        load_data();
        register_systems();
    }

    // methods

    void shell::resize(unsigned int screen_width, unsigned int screen_height) {
        width = screen_width;
        height = screen_height;
        renderer.resize(width, height);
        ui.resize(width, height);
    }

    void shell::connect_managers() {
        renderer.assign_sprite_data(get_factory<sprite_system>().data());
        renderer.assign_lightmap_data(get_factory<light_system>().current_data(), get_factory<light_system>().last_data());
        renderer.assigm_message_data(messages.data());
        get_factory<light_system>().assign_scene(&stage);
        stage.assign_sprites(&get_factory<sprite_system>());
        get_factory<npc_system>().assign_scene(&stage);
        get_factory<script_system>().assign_environment(this);
        get_factory<body_system>().assign_environment(this);
    }

    void shell::load_data() {

        // images
        auto const document = document::load_document(settings::texture_path);
        auto const& textures = document["textures"];
        unsigned int texture_index = 0;
        for (auto const& texture : textures) {
            std::string const atlas = texture["atlas"];   // conversion constructors
            std::string const bitmap = texture["texture"];

            add_texture(bitmap.c_str());
            add_atlas(atlas.c_str(), texture_index);
            ++texture_index;
        }
        ui.assign_logo(add_texture("data/img/extras/fmod.png"));

        get_factory<character_system>().load_skills(&get_factory<script_system>());
        get_factory<animator_system>().load(&get_factory<sprite_system>());
    }

    void shell::register_systems() {
        engine.add(&get_factory<animator_system>());
        engine.add(&get_factory<sprite_system>());
        engine.add(&renderer);
        engine.add(&get_factory<sound_system>());
        engine.add(&ui);

        // order is important
        engine.add(&get_factory<transform_system>());
        engine.add(&get_factory<character_system>());    // skill cooldowns
        engine.add(&get_factory<npc_system>());          // ai
        engine.add(&get_factory<body_system>());        // effects ticks, deaths, exp gains
        engine.add(&get_factory<light_system>());        // make shadowmap
        engine.add(&messages);      // popups

        engine.add(&get_factory<script_system>());
    }

    void shell::frame(double timer) {
        if (!is_running()) return;

        time.advance(timer);
        if (turn_passed()) {
            time.advance_turn(current_turn());
            engine.turn_update(time);
            return_turn();
        }
        engine.update(time);
    }

    void shell::text(unsigned int codepoint) {
        if (!is_running() || turn_passed()) return;
        ui.text(codepoint);
    }

    void shell::click(int mouse_button, bool is_down) {
        if (!is_running() || turn_passed()) return;
        if (ui.click(mouse_button, is_down)) return;
        if (mouse_button == 0 && is_down) {
            advance();
        }
    }

    void shell::hover(double x, double y) {
        if (!is_running() || turn_passed()) return;
        if (ui.hover(static_cast<unsigned int>(x), static_cast<unsigned int>(y))) return;

        vector2 position(x, y);
        position /= vector2(width, height);         // (0, 1) screen space
        position += { -0.5, -0.5 };                 // to screen center
        position *= { 2.0, -2.0 * height / width }; // account aspect
        position /= renderer.get_scale();
        position += { 0.5, 0.5 };                   // tile center offset
        target(position.floor());
    }

    void shell::scroll(double horisontal, double vertical) {
        if (!is_running() || turn_passed()) return;
        if (ui.scroll(horisontal, vertical)) return;
        renderer.zoom(horisontal + vertical > 0);
    }

    void shell::press(key action_index) {
        if (!is_running() || turn_passed()) return;
        if (ui.takes_input() && action_index != key::escape) return;    // escape is special

        switch (action_index) {
        case key::quick_save: {
            ui.rollback();
            //popup("quicksave...", { 1, 1, 1 });
            save("quicksave");
            break;
        }
        case key::quick_load: {
            ui.rollback();
            load("quicksave");
            //popup("quickload", { 1, 1, 1 });
            time.advance_turn(current_turn());
            break;
        }

        case key::move_east:
            ui.rollback();
            step({ 1, 0 });
            break;
        case key::move_west:
            ui.rollback();
            step({ -1, 0 });
            break;
        case key::move_north:
            ui.rollback();
            step({ 0, 1 });
            break;
        case key::move_south:
            ui.rollback();
            step({ 0, -1 });
            break;
        case key::move_northeast:
            ui.rollback();
            step({ 1, 1 });
            break;
        case key::move_northwest:
            ui.rollback();
            step({ -1, 1 });
            break;
        case key::move_southeast:
            ui.rollback();
            step({ 1, -1 });
            break;
        case key::move_southwest:
            ui.rollback();
            step({ -1, -1 });
            break;

        case key::action_use:
            ui.rollback();
            use(0);
            break;

        case key::action0:
            ui.rollback();
            action(0);
            break;
        case key::action1:
            ui.rollback();
            action(1);
            break;
        case key::action2:
            ui.rollback();
            action(2);
            break;
        case key::action3:
            ui.rollback();
            action(3);
            break;
        case key::action4:
            ui.rollback();
            action(4);
            break;
        case key::action5:
            ui.rollback();
            action(5);
            break;

        case key::panel_inventory:
            ui.toggle_inventory();
            break;
        case key::escape:
            ui.escape();
            break;

        case key::function_edit:
            function_edit(0);
            break;
        default:
            break;
        }
    }

    unsigned int shell::add_texture(const char * name) {
        std::vector<unsigned char> bits;
        unsigned int texture_width;
        unsigned int texture_height;
        auto const error = lodepng::decode(bits, texture_width, texture_height, name);
        if (error) throw std::runtime_error("add_texture(name) error while loading image, path=" + std::string(name));
        return renderer.add_texture(texture_width, texture_height, bits.data());
    }

    void shell::add_atlas(const char * name, unsigned int texture_index) {
        auto const document = document::load_document(name);
        get_factory<sprite_system>().load_atlas(document, texture_index, true);
    }
}