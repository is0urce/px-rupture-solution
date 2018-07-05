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
                    float const screen_width = ImGui::GetIO().DisplaySize.x;
                    float const screen_height = ImGui::GetIO().DisplaySize.y;
                    float const window_width = 300.0f;
                    float const window_height = 125.0f;
                    float const window_padding_x = 16;
                    float const window_padding_y = 65;
                    ImVec2 const position(window_padding_x, screen_height - window_height - window_padding_y);
                    ImVec2 const size(window_width, window_height);
                    combine_status("##player_status", position, size, *body);
                }
            }
        }

    private:
        environment * context;
    };
}