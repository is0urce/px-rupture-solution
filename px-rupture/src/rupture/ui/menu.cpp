// name: menu.cpp
// typc: c++

#include "menu.hpp"

#include "../app/bootstrap.hpp"

#include "director.hpp"
#include "panel.hpp"

#include "craft_smith.hpp"
#include "craft_alchemy.hpp"
#include "credits_screen.hpp"
#include "editor.hpp"
#include "escape_screen.hpp"
#include "inspector.hpp"
#include "inventory.hpp"
#include "performance.hpp"
#include "status.hpp"
#include "skills.hpp"
#include "title_screen.hpp"
#include "levelup_screen.hpp"
#include "options.hpp"
#include "screen/screen_results.hpp"

#include <px/memory/memory.hpp>

namespace px {

    // aux

    namespace {
        template <typename DesiredType, typename Container, typename ... Args>
        DesiredType * make_panel(Container & container, Args && ...args) {
            auto smart = make_uq<DesiredType>(std::forward<Args>(args)...);
            auto raw = smart.get();
            container.emplace_back(std::move(smart));
            return raw;
        }
    }

    // ctor & dtor

    menu::~menu() = default;

    menu::menu(unsigned int w, unsigned int h, environment * game, cfg * config)
        : context(game)
        , configuration(config)
        , nexus(make_uq<director>(w, h))
        , open_credits(false)
        , open_options(false)
        , open_inventory(false)
        , open_escape(false)
        , inventory_panel(nullptr)
        , smith_panel(nullptr)
        , alchemy_panel(nullptr) {
#if PX_INGAME_PERFORMANCE_TEST == 1
        make_panel<ui::performance>(stack);
#endif
#if PX_INGAME_EDITOR == 1
        make_panel<ui::editor>(stack, game);
#endif

        make_panel<ui::skills>(stack, game);
        make_panel<ui::status>(stack, game);
        make_panel<ui::inspector>(stack, game);

        inventory_panel = make_panel<inventory>(stack, game, &open_inventory);
        smith_panel = make_panel<craft_smith>(stack, game);
        alchemy_panel = make_panel<craft_alchemy>(stack, game);

        title_panel = make_panel<title_screen>(stack, game, &open_options, &open_credits);
        make_panel<ui::levelup_screen>(stack, game);
        make_panel<ui::screen_results>(stack, game);

        make_panel<ui::escape_screen>(stack, game, &open_escape, &open_options);
        make_panel<ui::options>(stack, game, config, &open_options);
        make_panel<credits_screen>(stack, &open_credits);
    }

    // methods

    void menu::assign_logo(unsigned int texture_id) noexcept {
        if (title_panel) {
            title_panel->assign_logo(texture_id);
        }
    }

    void menu::resize(unsigned int w, unsigned int h) {
        nexus->resize(w, h);
    }

    void menu::draw(double delta_time) {
        nexus->begin(delta_time);
        combine();
        nexus->end();
    }

    bool menu::click(unsigned int mouse_button, bool is_down) {
        return nexus->click(mouse_button, is_down);
    }

    bool menu::text(unsigned int codepoint) {
        return nexus->text(codepoint);
    }

    bool menu::hover(unsigned int x, unsigned int y) {
        return nexus->hover(x, y);
    }

    bool menu::scroll(double horisontal, double vertical) {
        return nexus->scroll(horisontal, vertical);
    }

    bool menu::takes_input() {
        return nexus->takes_input()
            || open_credits
            || open_escape
            || open_options;
    }

    bool menu::toggle_inventory() {
        smith_panel->cancel_smith();
        alchemy_panel->cancel_alchemy();
        return open_inventory = !open_inventory;
    }

    void menu::escape_command() {
        // close craft panels
        bool something_closed = smith_panel->cancel_smith() || alchemy_panel->cancel_alchemy();

        // close inventory panel
        if (!something_closed) {
            if (open_inventory) {
                toggle_inventory();
                something_closed = true;
            }
        }
        // close submenus
        if (!something_closed) {
            if (open_options || open_credits) {
                open_options = false;
                open_credits = false;
                something_closed = true;
            }
        }

        // toggle escape options
        if (!something_closed) {
            // require ingame
            if (context && context->is_ingame()) {
                open_escape = !open_escape;
                title_panel->update_saves();    // update save flag in case of returning to title
            }
        }
    }

    void menu::combine() {
        for (auto & ptr : stack) {
            if (ptr) {
                ptr->combine();
            }
        }
    }

    void menu::rollback() {
        smith_panel->cancel_smith();
        alchemy_panel->cancel_alchemy();
        open_inventory = false;
    }
}