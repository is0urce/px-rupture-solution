// name: menu.hpp
// typc: c++ header

#pragma once

#include "panel.hpp"
#include <px/memory/uq_ptr.hpp>

#include <vector>

namespace px {

    class director;
    class environment;

    class cfg;
    class container_component;
    class craft_smith;
    class craft_alchemy;
    class inventory;
    class screen_title;

    class menu final {
    public:
        void                            assign_logo(unsigned int texture_id) noexcept;
        void                            resize(unsigned int w, unsigned int h);
        void                            draw(double delta_time);
        bool                            click(unsigned int mouse_button, bool is_down);
        bool                            text(unsigned int codepoint);
        bool                            hover(unsigned int x, unsigned int y);
        bool                            scroll(double horisontal, double vertical);
        bool                            takes_input();
        bool                            toggle_inventory();
        void                            rollback(); // close active inventory trasfers
        void                            escape_command();

    public:
        ~menu();
        menu(unsigned int w, unsigned int h, environment * game, cfg * config);

    private:
        void                            combine();

    private:
        uq_ptr<director>                nexus;
        std::vector<uq_ptr<panel>>      stack;
        environment *                   game;
        bool                            open_inventory;
        bool                            open_options;
        bool                            open_escape;
        bool                            open_credits;
        inventory *                     inventory_panel;
        craft_smith *                   smith_panel;
        craft_alchemy *                 alchemy_panel;
        screen_title *                  title_panel;
        environment *                   context;
        cfg *                           configuration;
    };
}