// name: screen_results.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

// ui for game results and death screen

#include "../panel.hpp"
#include "../immediate.hpp"

#include "../../app/settings.hpp"
#include "../../environment.hpp"
#include "../../es/transform_component.hpp"
#include "../../es/body_component.hpp"
#include "../../es/character_component.hpp"
#include "../../es/composite_component.hpp"
#include "../../es/value_component.hpp"

#include <sstream>

namespace px::ui {

    class screen_results final
        : public panel
    {
    public:
        virtual ~screen_results() noexcept override = default;

        screen_results(environment * game) noexcept
            : context(game) {
        }

        bool is_open() const {
            auto pawn = (context && context->is_ingame()) ? context->controlled() : nullptr;
            auto body = pawn ? pawn->linked<body_component>() : nullptr;
            auto person = body ? body->linked<character_component>() : nullptr;

            bool const victory = person && person->has_trait("t_game_over");
            bool const defeat = body && body->is_dead();

            return victory || defeat;
        }

    protected:
        virtual void combine_panel() override {
            auto pawn = (context && context->is_ingame()) ? context->controlled() : nullptr;
            auto body = pawn ? pawn->linked<body_component>() : nullptr;
            auto person = body ? body->linked<character_component>() : nullptr;
            auto unit = body ? body->linked<composite_component>() : nullptr;
            auto vault = unit ? unit->query<value_component>() : nullptr;

            bool const victory = person && person->has_trait("t_game_over");
            bool const defeat = body && body->is_dead();

            if (victory || defeat) {
                ImVec2 const& screen_size = ImGui::GetIO().DisplaySize;
                ImVec2 const message_size(screen_size.x * 0.5f, screen_size.y * 75.0f);
                ImVec2 const message_position(screen_size.x * 0.25f, screen_size.y * 0.25f);

                immediate::style_color bg_transparent(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
                ImGui::SetNextWindowPos(message_position, ImGuiCond_Always);
                ImGui::SetNextWindowSize(message_size);
                ImGui::Begin("##results_panel", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);

                if (victory) {
                    immediate::print("Victory!", message_size.x);
                    ImGui::NewLine();
                    immediate::print("Congratulations, you have won", message_size.x);
                    ImGui::NewLine();
                    ImGui::TextWrapped("Thanks for playing this game! Hope you had a great time and enjoyed it as much as time, effort, and hard work i put into making it for you to play. Now allow me to kiss your tushie.");

                    // stats
                    if (vault) {
                        ImGui::NewLine();
                        combine_metrics(*vault, *body, message_size.x);
                    }

                    // buttons
                    ImGui::NewLine();
                    if (immediate::button("Continue...", message_size.x)) {
                        press_resume(*person);
                    }
                    if (immediate::button("Return to Title", message_size.x)) {
                        press_title();
                    }

                }
                else {
                    immediate::print("Game Over", message_size.x);
                    ImGui::NewLine();
                    immediate::print("You have died", message_size.x);

                    // stats
                    if (vault) {
                        ImGui::NewLine();
                        combine_metrics(*vault, *body, message_size.x);
                    }

                    // buttons
                    ImGui::NewLine();
                    if (immediate::button("Load", message_size.x)) {
                        press_load();
                    }
                }

                ImGui::End();
            }
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
                context->play_sound(settings::sound_path + std::string("snd_ui_click.wav"), 1.0f);
                context->load("quicksave");
            }
        }

        void press_resume(character_component & character) {
            if (context) {
                context->play_sound(settings::sound_path + std::string("snd_ui_click.wav"), 1.0f);
                character.remove_trait("t_game_over");
            }
        }

        void press_restart() {
            if (context) {
                context->play_sound(settings::sound_path + std::string("snd_ui_click.wav"), 1.0f);
                context->end();
                context->begin();
            }
        }

        void press_title() {
            if (context) {
                context->play_sound(settings::sound_path + std::string("snd_ui_click.wav"), 1.0f);
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