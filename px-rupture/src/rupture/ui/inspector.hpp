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
            if (!game) return;

            if (auto target = game->target()) {
                if (auto body = target->linked<body_component>()) {

                    const float screen_width = ImGui::GetIO().DisplaySize.x;
                    const float screen_height = ImGui::GetIO().DisplaySize.y;
                    const float window_width = 300.0f;
                    const float window_height = 125.0f;
                    const float window_padding_x = 16;
                    const float window_padding_y = 65;

                    ImVec2 position{ screen_width - window_width - window_padding_x, screen_height - window_height - window_padding_y };
                    ImVec2 size{ window_width, window_height };

                    combine_status("##inspector", position, size, *body);
                }
            }
        }

    private:
        environment * game;
    };
}