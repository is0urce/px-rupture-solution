// name: inspector.hpp
// type: c++
// auth: is0urce
// desc: ui inspectior panel class

#pragma once

#include "inspector_common.hpp"

#include "../environment.hpp"
#include "../es/transform_component.hpp"
#include "../es/body_component.hpp"

namespace px::ui {

    class inspector final
        : public inspector_common
    {
    public:
        virtual ~inspector() override = default;

        inspector(environment * env)
            : game(env) {
        }

    protected:
        virtual void combine_panel() override {
            if (auto target = game ? game->target() : nullptr) {
                if (auto body = target->linked<body_component>()) {

                    float const screen_width = ImGui::GetIO().DisplaySize.x;
                    float const screen_height = ImGui::GetIO().DisplaySize.y;
                    float const window_width = 300.0f;
                    float const window_height = 125.0f;
                    float const window_padding_x = 16;
                    float const window_padding_y = 65;

                    ImVec2 const position{ screen_width - window_width - window_padding_x, screen_height - window_height - window_padding_y };
                    ImVec2 const size{ window_width, window_height };

                    combine_status("##inspector", position, size, *body);
                }
            }
        }

    private:
        environment * game;
    };
}