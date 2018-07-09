// name: status.hpp
// type: c++
// auth: is0urce
// desc: status ui panel class

#pragma once

#include "inspector_common.hpp"

#include "../environment.hpp"
#include "../es/transform_component.hpp"
#include "../es/body_component.hpp"

namespace px::ui {

    class status final
        : public inspector_common
    {
    public:
        virtual ~status() override = default;

        status(environment * game)
            : context(game) {
        }

    protected:
        virtual void combine_panel() override {
            if (auto pawn = context ? context->controlled() : nullptr) {
                if (auto body = pawn->linked<body_component>()) {
                    ImVec2 const& screen = ImGui::GetIO().DisplaySize;
                    ImVec2 const window_size(300, 125);
                    ImVec2 const window_padding(16, 65);
                    ImVec2 const position(window_padding.x, screen.y - window_size.y - window_padding.y);
                    combine_status("##player_status", position, window_size, *body);
                }
            }
        }

    private:
        environment * context;
    };
}