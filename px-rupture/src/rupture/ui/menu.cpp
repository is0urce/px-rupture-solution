// name: menu.cpp
// typc: c++

#include "menu.hpp"

#include "../app/bootstrap.hpp"

#include "director.hpp"
#include "panel.hpp"

#include "craft_smith.hpp"
#include "craft_alchemy.hpp"
#include "editor.hpp"
#include "inspector.hpp"
#include "inventory.hpp"
#include "performance.hpp"
#include "status.hpp"
#include "skills.hpp"
#include "title.hpp"
#include "levelup.hpp"
#include "results.hpp"

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

    menu::menu(unsigned int w, unsigned int h, environment * game)
        : context(game)
        , nexus(make_uq<director>(w, h))
        , inventory_open(false)
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

        inventory_panel = make_panel<inventory>(stack, game, &inventory_open);
        smith_panel = make_panel<craft_smith>(stack, game);
        alchemy_panel = make_panel<craft_alchemy>(stack, game);

        make_panel<ui::title>(stack, game);
        make_panel<ui::levelup>(stack, game);
        make_panel<ui::results>(stack, game);
    }

    // methods

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
        return nexus->takes_input();
    }

    void menu::toggle_inventory() {
        smith_panel->cancel_smith();
        alchemy_panel->cancel_alchemy();
        inventory_open = !inventory_open;
    }

    void menu::combine() {
        for (auto & ptr : stack) {
            ptr->combine();
        }
    }

    void menu::rollback() {
        smith_panel->cancel_smith();
        alchemy_panel->cancel_alchemy();
        inventory_open = false;
    }
}