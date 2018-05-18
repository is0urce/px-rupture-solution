// name: title.hpp
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

    public:
        virtual ~title_screen() noexcept override = default;

        title_screen(environment * game) noexcept
            : context(game)
            , logo_texture_id(0) {
        }

    protected:
        virtual void combine_panel() override {
            if (!context || context->possessed()) return;

            float const screen_width = ImGui::GetIO().DisplaySize.x;
            float const screen_height = ImGui::GetIO().DisplaySize.y;
            float const options_length = 200.0f;
            float const options_height = 500.0f;

            // main options
            ImGui::SetNextWindowPos({ screen_width / 2 - options_length / 2, screen_height / 3 }, ImGuiCond_Always);
            ImGui::SetNextWindowSize({ options_length + 100, 300 });
            ImGui::Begin("##title_screen", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
            immediate::print("Gnomi##title_name", options_length);
            ImGui::NewLine();
            ImGui::PushStyleColor(ImGuiCol_Text, { 0.8f, 0.8f, 0.8f, 1.0f });
            if (immediate::button("Start##titile_start", options_length)) {
                press_start();
            }
            if (immediate::button("Options##title_options", options_length)) {
                press_options();
            }
            if (immediate::button("Exit##title_exit", options_length)) {
                press_exit();
            }
            ImGui::PopStyleColor(1);
            ImGui::End();

            // extras logo
            if (logo_texture_id != 0) {
                float const logo_width = 72;
                float const logo_height = 19;
                ImGui::SetNextWindowPos({ 20, screen_height - 100 }, ImGuiCond_Always);
                ImGui::SetNextWindowSize({ logo_width + 10, logo_height + 10 });
                ImGui::Begin("##title_extras_logo", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
                ImGui::PushStyleColor(ImGuiCol_Text, { 1, 1, 1, 1 });
                ImGui::Image(reinterpret_cast<void*>(static_cast<size_t>(logo_texture_id)), { logo_width, logo_height });
                ImGui::PopStyleColor(1);
                ImGui::End();
            }

            // version notice
            ImGui::SetNextWindowPos({ 20, screen_height - 50 }, ImGuiCond_Always);
            ImGui::SetNextWindowSize({ screen_width, 100 });
            ImGui::Begin("##title_version", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
            ImGui::PushStyleColor(ImGuiCol_Text, { 0.6f, 0.6f, 0.6f, 1.0f });
            ImGui::Text("Gnomi: Caves of Nschryldamdshuncz, 2018, version 0.0.9.0 (x64)");
            ImGui::PopStyleColor(1);
            ImGui::End();
        }

    private:
        void press_start() {
            context->start();
        }

        void press_exit() {
            context->shutdown();
        }

        void press_options() {

        }

    private:
        environment *   context;
        unsigned int    logo_texture_id;
    };
}