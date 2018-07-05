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
#include "../es/composite_component.hpp"
#include "../es/value_component.hpp"

#include <sstream>

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
            auto unit = body->linked<composite_component>();
            if (!show) return;

            auto const vault = unit ? unit->query<value_component>() : nullptr;

            ImVec2 const screen_size(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
            ImVec2 const window_size(200.0f, screen_size.y / 2);
            ImVec2 const padding(ImGui::GetStyle().FramePadding.x * ImGui::GetIO().FontGlobalScale, ImGui::GetStyle().FramePadding.y * ImGui::GetIO().FontGlobalScale);

            immediate::style_color bg_transparent(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 1.0f });
            ImGui::SetNextWindowPos({ screen_size.x / 2 - window_size.x / 2 - padding.x, screen_size.y / 4 - padding.y }, ImGuiCond_Always);
            ImGui::SetNextWindowSize({ window_size.x + padding.x * 2, window_size.y + padding.y * 2 });
            ImGui::Begin("##results_content", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);

            if (body->is_alive()) {
                immediate::print("Victory!", window_size.x);
                ImGui::NewLine();
                immediate::print("You have won.", window_size.x);
                ImGui::NewLine();
                immediate::print("Congratulations", window_size.x);
                if (vault) {
                    combine_metrics(*vault, *body, window_size.x);
                }
                if (immediate::button("Continue", window_size.x)) {
                    press_resume(*character);
                }
                if (immediate::button("Title", window_size.x)) {
                    press_title();
                }

            }
            else {
                immediate::print("Game Over", window_size.x);
                ImGui::NewLine();
                immediate::print("You have died.", window_size.x);
                ImGui::NewLine();
                if (vault) {
                    combine_metrics(*vault, *body, window_size.x);
                }
                if (immediate::button("Load", window_size.x)) {
                    press_load();
                }
            }

            ImGui::End();
        }

    private:
        void combine_metrics(value_component const& vault, body_component const& body, float window_width) {
            immediate::print(format_metrix("level", body.level()), window_width);
            immediate::print(format_metrix("experience", body.experience()), window_width);
            immediate::print(format_metrix("slayed", vault.int_value("slayed", 0)), window_width);
            immediate::print(format_metrix("mined", vault.int_value("mined", 0)), window_width);
            immediate::print(format_metrix("harvested", vault.int_value("harvested", 0)), window_width);
        }
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
                context->begin();
            }
        }

        void press_title() {
            if (context) {
                context->end();
            }
        }

        template <typename T>
        std::string format_metrix(std::string const& label, T value) {
            std::stringstream sstr;
            sstr << label << std::setw(20 - label.length()) << std::setfill('.') << value;
            return sstr.str();
        }

    private:
        environment * context;
    };
}