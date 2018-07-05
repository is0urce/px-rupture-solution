// name: escape_screen.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "panel.hpp"

#include "immediate.hpp"
#include "design.hpp"

#include "../environment.hpp"

namespace px::ui {

    class escape_screen final
        : public panel {
    public:
        virtual ~escape_screen() noexcept override = default;

        escape_screen(environment * ctx, bool * flag, bool * options_flag)
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
            immediate::dim();

            // main options
            ImGui::SetNextWindowPos(position, ImGuiCond_Always);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
            ImGui::Begin("##escape_screen", nullptr, design::window_flat() | ImGuiWindowFlags_AlwaysAutoResize);
            immediate::print("Gnomi##escape_name_txt", width);
            ImGui::NewLine();

            ImGui::PushStyleColor(ImGuiCol_Text, { 0.8f, 0.8f, 0.8f, 1.0f });
            if (immediate::button("Continue##escape_continue_btn", width)) {
                press_contunue();
            }
            if (immediate::button("Options##escape_options_btn", width)) {
                press_options();
            }
            //if (immediate::button("Credits##escape_credits_btn", width)) {
            //    press_credits();
            //}

            if (immediate::button("Title##escape_exit_title_btn", width)) {
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

        bool is_open() const {
            return context && is_true(open_flag) && !is_hidden_by_options();
        }

        bool is_hidden_by_options() const {
            return is_true(open_options);
        }

        void press_contunue() {
            set_flag(open_flag, false);
        }

        void press_options() {
            set_flag(open_options, true);
        }

        void press_title() {
            if (context) {
                context->end();
            }
            set_flag(open_flag, false);
        }

        void press_desctop() {
            if (context) {
                context->shutdown();
            }
            set_flag(open_flag, false);
        }

    private:
        environment *   context;
        bool *          open_flag;
        bool *          open_options;
    };
}