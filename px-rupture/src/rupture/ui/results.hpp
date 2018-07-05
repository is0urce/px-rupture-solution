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
            auto pawn = context ? context->controlled() : nullptr;
            if (!pawn) return;
            auto[body, character] = pawn->unwind<body_component, character_component>();
            bool const show = body && character && character->has_trait("c_game_over");
            if (!show) return;

            float const screen_width = ImGui::GetIO().DisplaySize.x;
            float const screen_height = ImGui::GetIO().DisplaySize.y;
            float const window_width = 200.0f;
            float const window_height = screen_height / 2;
            float const padding_x = ImGui::GetStyle().FramePadding.x * 2; // multiplier from pixel size multiplication
            float const padding_y = ImGui::GetStyle().FramePadding.y * 2;

            immediate::style_color bg_transparent(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 1.0f });
            ImGui::SetNextWindowPos({ screen_width / 2 - window_width / 2 - padding_x, screen_height / 4 - padding_y }, ImGuiCond_Always);
            ImGui::SetNextWindowSize({ window_width + padding_x * 2, window_height + padding_y * 2 });
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
        }

    private:
        void press_load() {
            if (context) {
                context->load("quicksave");
            }
        }

        void press_resume(character_component & character) {
            character.remove_trait("game_over");
        }

        void press_restart() {
            if (context) {
                context->end();
                context->start();
            }
        }

        void press_title() {
            if (context) {
                context->end();
            }
        }

    private:
        environment * context;
    };
}