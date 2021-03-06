// name: ui_system.hpp

#pragma once

#include <px/es/system.hpp>
#include <px/es/delta.hpp>

#include <px/memory/uq_ptr.hpp>

namespace px {

    class menu;
    class environment;
    class container_component;

    class cfg;

    class ui_system
        : public system<delta>
    {
    public:
        void            assign_logo(unsigned int texture_id) noexcept;
        bool            click(unsigned int mouse_button, bool is_down);
        bool            text(unsigned int codepoint);
        bool            hover(unsigned int x, unsigned int y);
        bool            scroll(double horisontal, double vertical);
        void            resize(unsigned int x, unsigned int y);
        bool            takes_input();
        void            toggle_inventory();
        void            escape();
        void            rollback();

    public:
        virtual         ~ui_system() override;
        ui_system(unsigned int width, unsigned int height, environment * game, cfg * config);

    protected:
        virtual void    update_system(delta_type const& delta_time) override;

    private:
        uq_ptr<menu>    works;
    };
}