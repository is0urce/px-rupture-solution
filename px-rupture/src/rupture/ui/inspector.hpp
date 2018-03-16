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
        virtual ~inspector() = default;
        inspector(environment * env)
            : game(env)
        {
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
                    ImGui::SetNextWindowPos({ screen_width - window_width - window_padding_x, screen_height - window_height - window_padding_y }, ImGuiCond_Always);
                    ImGui::SetNextWindowSize({ window_width, window_height });
                    ImGui::Begin("##target_inspector", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
                    combine_status(*body);
                    ImGui::End();
                }
            }
        }

    private:
        environment * game;
    };
}