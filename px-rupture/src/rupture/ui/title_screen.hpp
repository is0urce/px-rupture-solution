// name: title_screen.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "panel.hpp"
#include "immediate.hpp"

#include "../environment.hpp"
#include "../es/transform_component.hpp"

namespace px {

    class title_screen final
        : public panel
    {
    public:
        void assign_logo(unsigned int texture_id) noexcept {
            logo_texture_id = texture_id;
        }

        bool escape() noexcept {
            bool something_closed = false;
            if (open_options) {
                *open_options = false;
                something_closed = true;
            }
            if (open_credits) {
                *open_credits = false;
                something_closed = true;
            }
            return something_closed;
        }

    public:
        virtual ~title_screen() noexcept override = default;

        title_screen(environment * game, bool * options_flag, bool * credits_flag) noexcept
            : context(game)
            , open_options(options_flag)
            , open_credits(credits_flag)
            , logo_texture_id(0) {
        }

    protected:
        virtual void combine_panel() override {
            if (is_open()) {
                float const screen_width = ImGui::GetIO().DisplaySize.x;
                float const screen_height = ImGui::GetIO().DisplaySize.y;
                float const options_width = 200.0f;

                combine_title({ screen_width / 2 - options_width / 2, screen_height / 3 }, options_width);
                combine_logo({ 20, screen_height - 100 });
                combine_version({ 20, screen_height - 50 }, { screen_width, 100 });
            }
        }

    private:
        bool is_hidden_by_options() const noexcept {
            return open_options && *open_options;
        }

        bool is_hidden_by_credits() const noexcept {
            return open_credits && *open_credits;
        }

        bool is_hidden_by_ingame() const noexcept {
            return context && context->possessed();
        }

        bool is_open() const noexcept {
            return !is_hidden_by_options()
                && !is_hidden_by_credits()
                && !is_hidden_by_ingame();
        }

        void combine_credits() {

        }

        void combine_title(ImVec2 const& position, float width) {
            // main options
            ImGui::SetNextWindowPos(position, ImGuiCond_Always);
            ImGui::SetNextWindowSize({ width + 100, 300 });
            ImGui::Begin("##title_screen", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
            immediate::print("Gnomi##title_name_txt", width);
            ImGui::NewLine();

            //if (context->)
            //ImGui::PushStyleColor(ImGuiCol_Text, { 0.8f, 0.8f, 0.8f, 1.0f });
            //if (immediate::line("Continue...##title_continue_btn", width,)) {
                //press_continue();
            //}

            ImGui::PushStyleColor(ImGuiCol_Text, { 0.8f, 0.8f, 0.8f, 1.0f });
            if (immediate::button("New Game##titile_start_btn", width)) {
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
                float const logo_width = 72;
                float const logo_height = 19;
                ImGui::SetNextWindowPos(position, ImGuiCond_Always);
                ImGui::SetNextWindowSize({ logo_width + 10, logo_height + 10 });
                ImGui::Begin("##title_extras_logo", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
                ImGui::PushStyleColor(ImGuiCol_Text, { 1, 1, 1, 1 });
                ImGui::Image(reinterpret_cast<void*>(static_cast<size_t>(logo_texture_id)), { logo_width, logo_height });
                ImGui::PopStyleColor(1);
                ImGui::End();
            }
        }

        void combine_version(ImVec2 const& position, ImVec2 const& size) {
            ImGui::SetNextWindowPos(position, ImGuiCond_Always);
            ImGui::SetNextWindowSize(size);
            ImGui::Begin("##title_version", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
            ImGui::PushStyleColor(ImGuiCol_Text, { 0.6f, 0.6f, 0.6f, 1.0f });
            ImGui::Text("Gnomi: Caves of Nschryldamdshuncz, 2018, version 0.0.9.0 (x64)");
            ImGui::PopStyleColor(1);
            ImGui::End();
        }

        void press_continue() {
            if (context) {
                context->load("quicksave");
            }
        }

        void press_start() {
            if (context) {
                context->start();
            }
        }

        void press_exit() {
            if (context) {
                context->shutdown();
            }
        }

        void press_options() {
            if (open_options) {
                *open_options = true;
            }
        }

        void press_credits() {
            if (open_credits) {
                *open_credits = true;
            }
        }

    private:
        bool *          open_options;
        bool *          open_credits;
        environment *   context;
        unsigned int    logo_texture_id;
    };
}