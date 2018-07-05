// name: title_screen.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

// store cashed save flags in attributes to avoid extra directory queries

#include "panel.hpp"
#include "immediate.hpp"
#include "design.hpp"

#include "../environment.hpp"
#include "../es/transform_component.hpp"

namespace px {

    class title_screen final
        : public panel {
    public:
        void assign_logo(unsigned int texture_id) noexcept {
            logo_texture_id = texture_id;
        }

    public:
        void update_saves() {
            quicksave = context && context->has_save("quicksave");
            autosave = context && context->has_save("autosave");
            exitsave = context && context->has_save("exitsave");
        }

    public:
        virtual ~title_screen() noexcept override = default;

        title_screen(environment * game, bool * options_flag, bool * credits_flag) noexcept
            : context(game)
            , open_options(options_flag)
            , open_credits(credits_flag)
            , logo_texture_id(0)
            , exitsave(false)
            , quicksave(false)
            , autosave(false)
            , greetings(true) {
            update_saves();
        }

    protected:
        virtual void combine_panel() override {
            if (is_open()) {
                float const screen_width = ImGui::GetIO().DisplaySize.x;
                float const screen_height = ImGui::GetIO().DisplaySize.y;
                combine_title({ screen_width / 2 - design::options_width() / 2, screen_height / 3 }, design::options_width());
                if (greetings) {
                    combine_logo({ 20, screen_height - 100 });
                    immediate::splash_version();
                }
            }
        }

    private:
        bool is_hidden_by_options() const noexcept {
            return is_true(open_options);
        }

        bool is_hidden_by_credits() const noexcept {
            return is_true(open_credits);
        }

        bool is_hidden_by_ingame() const noexcept {
            return context && context->is_ingame();
        }

        bool is_open() const noexcept {
            return !is_hidden_by_options()
                && !is_hidden_by_credits()
                && !is_hidden_by_ingame();
        }

        void combine_title(ImVec2 const& position, float width) {
            // main options
            ImGui::SetNextWindowPos(position, ImGuiCond_Always);
            ImGui::Begin("##title_screen", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
            immediate::print("Gnomi##title_name_txt", width);
            ImGui::NewLine();

            bool const show_continue = exitsave || autosave || quicksave;

            ImGui::PushStyleColor(ImGuiCol_Text, { 0.8f, 0.8f, 0.8f, 1.0f });
            if (show_continue) {
                if (immediate::button("Continue##title_continue_btn", width)) {
                    press_continue();
                }
            }
            if (immediate::button(show_continue ? "New Game##titile_start_btn" : "Start##title_start_btn", width)) {
                press_start();
            }
            if (immediate::button("Options##title_options_btn", width)) {
                press_options();
            }
            if (immediate::button("Credits##title_credits_btn", width)) {
                press_credits();
            }
            if (immediate::button("Exit##title_exit_btn", width)) {
                press_exit();
            }
            ImGui::PopStyleColor(1);
            ImGui::End();
        }

        void combine_logo(ImVec2 const& position) {
            if (logo_texture_id != 0) {
                float const logo_width = 72;    // logo bitmap is hardcoded
                float const logo_height = 19;
                ImGui::SetNextWindowPos(position, ImGuiCond_Always);
                ImGui::SetNextWindowSize({ logo_width + 10, logo_height + 10 });
                ImGui::Begin("##title_extras_logo", nullptr, design::window_flat());
                ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 1.0f, 1.0f, 1.0f });
                ImGui::Image(reinterpret_cast<void*>(static_cast<size_t>(logo_texture_id)), { logo_width, logo_height });
                ImGui::PopStyleColor(1);
                ImGui::End();
            }
        }

        void press_continue() {
            update_saves();
            if (context) {
                if (exitsave) {
                    context->load("exitsave");
                }
                else if (quicksave) {
                    context->load("quicksave");
                }
                else if (autosave) {
                    context->load("autosave");
                }
            }
            greetings = false;
        }

        void press_start() {
            if (context) {
                context->begin();
                greetings = false;
            }
        }

        void press_exit() {
            if (context) {
                context->shutdown();
                greetings = false;
            }
        }

        void press_options() {
            set_flag(open_options);
            greetings = false;
        }

        void press_credits() {
            set_flag(open_credits);
            greetings = false;
        }

    private:
        bool *          open_options;
        bool *          open_credits;
        environment *   context;
        unsigned int    logo_texture_id;
        bool            quicksave;
        bool            autosave;
        bool            exitsave;
        bool            greetings;  // title opened at start shows extra info
    };
}