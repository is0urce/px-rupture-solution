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
        , ui(start_width, start_height, &context, settings)
        , width(start_width)
        , height(start_height)
        , context(&core) {
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
        renderer.assign_sprite_data(core.get_factory<sprite_system>().data());
        renderer.assign_lightmap_data(core.get_factory<light_system>().current_data(), core.get_factory<light_system>().last_data());
        renderer.assigm_message_data(context.get_notifications()->data());
        core.get_factory<light_system>().assign_scene(context.get_scene());
        context.get_scene()->assign_sprites(&core.get_factory<sprite_system>());
        core.get_factory<npc_system>().assign_scene(context.get_scene());
        core.get_factory<script_system>().assign_environment(&context);
        core.get_factory<body_system>().assign_environment(&context);
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

        core.get_factory<character_system>().load_skills(&core.get_factory<script_system>());
        core.get_factory<animator_system>().load(&core.get_factory<sprite_system>());
    }

    void shell::register_systems() {
        engine.add(&core.get_factory<animator_system>());
        engine.add(&core.get_factory<sprite_system>());
        engine.add(&renderer);
        engine.add(&core.get_factory<sound_system>());
        engine.add(&ui);

        // order is important
        engine.add(&core.get_factory<transform_system>());  // world positions
        engine.add(&core.get_factory<character_system>());  // skill cooldowns
        engine.add(&core.get_factory<npc_system>());        // ai
        engine.add(&core.get_factory<body_system>());       // effects ticks, deaths, exp gains
        engine.add(&core.get_factory<light_system>());      // make shadowmap
        engine.add(&core.get_factory<script_system>());     // scripted behaviour
        engine.add(context.get_notifications());            // popups
    }

    void shell::frame(double timer) {
        if (!context.is_running()) return;

        time.advance(timer);
        if (context.turn_passed()) {
            time.advance_turn(context.current_turn());
            engine.turn_update(time);
            context.return_turn();
        }
        engine.update(time);
    }

    void shell::text(unsigned int codepoint) {
        if (!context.is_running() || context.turn_passed()) return;
        ui.text(codepoint);
    }

    void shell::click(int mouse_button, bool is_down) {
        if (!context.is_running() || context.turn_passed()) return;
        if (ui.click(mouse_button, is_down)) return;
        if (mouse_button == 0 && is_down) {
            context.advance();
        }
    }

    void shell::hover(double x, double y) {
        if (!context.is_running() || context.turn_passed()) return;
        if (ui.hover(static_cast<unsigned int>(x), static_cast<unsigned int>(y))) return;

        vector2 position(x, y);
        position /= vector2(width, height);         // (0, 1) screen space
        position += { -0.5, -0.5 };                 // to screen center
        position *= { 2.0, -2.0 * height / width }; // account aspect
        position /= renderer.get_scale();
        position += { 0.5, 0.5 };                   // tile center offset
        context.target(position.floor());
    }

    void shell::scroll(double horisontal, double vertical) {
        if (!context.is_running() || context.turn_passed()) return;
        if (ui.scroll(horisontal, vertical)) return;
        renderer.zoom(horisontal + vertical > 0);
    }

    void shell::press(key action_index) {
        if (!context.is_running() || context.turn_passed()) return;
        if (ui.takes_input() && action_index != key::escape) return;    // escape is special

        switch (action_index) {
        case key::quick_save: {
            ui.rollback();
            //popup("quicksave...", { 1, 1, 1 });
            context.save("quicksave");
            break;
        }
        case key::quick_load: {
            ui.rollback();
            context.load("quicksave");
            //popup("quickload", { 1, 1, 1 });
            time.advance_turn(context.current_turn());
            break;
        }

        case key::move_east:
            ui.rollback();
            context.step({ 1, 0 });
            break;
        case key::move_west:
            ui.rollback();
            context.step({ -1, 0 });
            break;
        case key::move_north:
            ui.rollback();
            context.step({ 0, 1 });
            break;
        case key::move_south:
            ui.rollback();
            context.step({ 0, -1 });
            break;
        case key::move_northeast:
            ui.rollback();
            context.step({ 1, 1 });
            break;
        case key::move_northwest:
            ui.rollback();
            context.step({ -1, 1 });
            break;
        case key::move_southeast:
            ui.rollback();
            context.step({ 1, -1 });
            break;
        case key::move_southwest:
            ui.rollback();
            context.step({ -1, -1 });
            break;

        case key::action_use:
            ui.rollback();
            context.use(0);
            break;

        case key::action0:
            ui.rollback();
            context.action(0);
            break;
        case key::action1:
            ui.rollback();
            context.action(1);
            break;
        case key::action2:
            ui.rollback();
            context.action(2);
            break;
        case key::action3:
            ui.rollback();
            context.action(3);
            break;
        case key::action4:
            ui.rollback();
            context.action(4);
            break;
        case key::action5:
            ui.rollback();
            context.action(5);
            break;

        case key::panel_inventory:
            ui.toggle_inventory();
            break;
        case key::escape:
            ui.escape();
            break;

        case key::function_edit:
            context.function_edit(0);
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
        core.get_factory<sprite_system>().load_atlas(document, texture_index, true);
    }

    void shell::seed(unsigned int random_number) {
        context.seed(random_number);
    }

    bool shell::is_running() const {
        return context.is_running();
    }
}