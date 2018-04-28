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
            if (!context) return;
            if (auto target = context->possessed()) {
                if (auto body = target->linked<body_component>()) {

                    const float screen_width = ImGui::GetIO().DisplaySize.x;
                    const float screen_height = ImGui::GetIO().DisplaySize.y;
                    const float window_width = 300.0f;
                    const float window_height = 125.0f;
                    const float window_padding_x = 16;
                    const float window_padding_y = 65;

                    ImVec2 position{ window_padding_x, screen_height - window_height - window_padding_y };
                    ImVec2 size{ window_width, window_height };

                    combine_status("##status", position, size, *body);
                }
            }
        }

    private:
        environment * context;
    };
}