// name: screen_escape.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

// ui class for ingame menu

#include "../panel.hpp"

#include "../immediate.hpp"
#include "../design.hpp"

#include "../../environment.hpp"

namespace px::ui {

    class screen_escape final
        : public panel {
    public:
        virtual ~screen_escape() noexcept override = default;

        screen_escape(environment * ctx, bool * flag, bool * options_flag)
            : context(ctx)
            , open_flag(flag)
            , open_options(options_flag) {
        }

    protected:
        virtual void combine_panel() override {
            if (is_open()) {
                combine_escape(design::menu_position(), design::options_width());
            }
        }

    private:
        void combine_escape(ImVec2 position, float width) {
            immediate::dimmed_background();

            // main options
            ImGui::SetNextWindowPos(position, ImGuiCond_Always);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
            ImGui::Begin("##escape_screen", nullptr, design::window_flat() | ImGuiWindowFlags_AlwaysAutoResize);
            immediate::print("Gnomi##escape_name_txt", width);
            ImGui::NewLine();

            ImGui::PushStyleColor(ImGuiCol_Text, { 0.8f, 0.8f, 0.8f, 1.0f });
            if (immediate::button("Resume##escape_continue_btn", width)) {
                press_contunue();
            }
            if (immediate::button("Options##escape_options_btn", width)) {
                press_options();
            }
            //if (immediate::button("Credits##escape_credits_btn", width)) {
            //    press_credits();
            //}

            if (immediate::button("To Title##escape_exit_title_btn", width)) {
                press_title();
            }

            if (immediate::button("Exit##escape_exit_desctop_btn", width)) {
                press_desctop();
            }
            ImGui::PopStyleColor(1);

            ImGui::SetWindowFocus();
            ImGui::End();
            ImGui::PopStyleColor(1);    // window background
        }

        bool is_open() const noexcept {
            return context && is_true(open_flag) && !is_hidden_by_options();
        }

        bool is_hidden_by_options() const noexcept {
            return is_true(open_options);
        }

        void press_contunue() {
            if (context) {
                context->play_sound(settings::sound_path + std::string("snd_ui_click.wav"), 1.0f);
                set_flag(open_flag, false);
            }
        }

        void press_options() {
            if (context) {
                context->play_sound(settings::sound_path + std::string("snd_ui_click.wav"), 1.0f);
                set_flag(open_options, true);
            }
        }

        void press_title() {
            if (context) {
                context->play_sound(settings::sound_path + std::string("snd_ui_click.wav"), 1.0f);
                context->end();
                set_flag(open_flag, false);
            }
        }

        void press_desctop() {
            if (context) {
                context->play_sound(settings::sound_path + std::string("snd_ui_click.wav"), 1.0f);
                context->shutdown();
                set_flag(open_flag, false);
            }
        }

    private:
        environment *   context;
        bool *          open_flag;
        bool *          open_options;
    };
}