// name: results.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "panel.hpp"
#include "immediate.hpp"

#include "../environment.hpp"
#include "../es/transform_component.hpp"
#include "../es/body_component.hpp"
#include "../es/character_component.hpp"

namespace px::ui {

    class results final
        : public panel
    {
    public:
        virtual ~results() noexcept override = default;

        results(environment * game) noexcept
            : context(game) {
        }

    protected:
        virtual void combine_panel() override {
            if (!context) return;
            transform_component * player = context->possessed();
            if (!player) return;
            body_component * body = player->linked<body_component>();
            if (!body) return;
            character_component * character = body->linked<character_component>();
            bool show = character->has_trait("game_over");
            if (!show) return;

            const float screen_width = ImGui::GetIO().DisplaySize.x;
            const float screen_height = ImGui::GetIO().DisplaySize.y;
            const float	window_width = 200.0f;
            const float window_height = screen_height / 2;
            const float padding_x = ImGui::GetStyle().FramePadding.x * 2; // multiplier from pixel size multiplication
            const float padding_y = ImGui::GetStyle().FramePadding.y * 2;

            ImGui::SetNextWindowPos({ screen_width / 2 - window_width / 2 - padding_x, screen_height / 4 - padding_y }, ImGuiCond_Always);
            ImGui::SetNextWindowSize({ window_width + padding_x * 2, window_height + padding_y * 2 });
            ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 1.0f });
            ImGui::Begin("##results_content", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);

            if (body->is_alive()) {
                immediate::print("Victory!", window_width);
                ImGui::NewLine();
                immediate::print("You have won.", window_width);
                ImGui::NewLine();
                if (immediate::button("Continue", window_width)) press_resume(*character);
                if (immediate::button("Titles", window_width)) press_title();

            }
            else {
                immediate::print("Game Over", window_width);
                ImGui::NewLine();
                immediate::print("You have died.", window_width);
                ImGui::NewLine();
                if (immediate::button("Load", window_width)) press_load();
                if (immediate::button("Restart", window_width)) press_restart();
            }

            ImGui::End();
            ImGui::PopStyleColor(1); // window style
        }

    private:
        void press_load() {
            context->load("quicksave");
        }

        void press_resume(character_component & character) {
            character.remove_trait("game_over");
        }

        void press_restart() {
            context->end();
            context->start();
        }

        void press_title() {
            context->end();
        }

    private:
        environment * context;
    };
}