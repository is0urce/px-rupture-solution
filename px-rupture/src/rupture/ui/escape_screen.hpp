// name: escape_screen.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "panel.hpp"

#include "../environment.hpp"

#include <imgui/imgui.h>

namespace px::ui {

    class escape_screen final
        : public panel
    {
    public:
        virtual ~escape_screen() override = default;

        escape_screen(environment * ctx, bool * flag, bool * options_flag)
            : context(ctx)
            , open_flag(flag)
            , open_options(options_flag) {
        }

    protected:
        virtual void combine_panel() override {
            if (!context || !is_open()) return;
            ImGui::Begin("esc##escape");
            ImGui::End();
        }

    private:
        bool is_open() const noexcept {
            return (open_flag && *open_flag) && (open_options && !*open_options);
        }

    private:
        environment *   context;
        bool *          open_flag;
        bool *          open_options;
    };
}