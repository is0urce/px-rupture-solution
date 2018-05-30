// name: credits_screen.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "panel.hpp"
#include "immediate.hpp"

namespace px {

    class credits_screen final
        : public panel
    {
    public:
        virtual ~credits_screen() noexcept override = default;

        credits_screen(bool * flag) noexcept
            : open_flag(flag) {
        }

    protected:
        virtual void combine_panel() override {
            if (is_open()) {
                float const screen_width = ImGui::GetIO().DisplaySize.x;
                float const screen_height = ImGui::GetIO().DisplaySize.y;
                float const options_width = 600.0f;

                combine_credits({ screen_width / 2 - options_width / 2, screen_height / 3 }, options_width);
            }
        }

    private:
        bool is_open() const noexcept {
            return open_flag && *open_flag;
        }

        void close() noexcept {
            if (open_flag) {
                *open_flag = false;
            }
        }

        void combine_credits(ImVec2 const& position, float width) {
            // main options
            ImGui::SetNextWindowPos(position, ImGuiCond_Always);
            ImGui::SetNextWindowSize({ width + 100, 600 });
            ImGui::Begin("##credits_screen", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
            ImGui::NewLine();
            ImGui::PushStyleColor(ImGuiCol_Text, { 0.8f, 0.8f, 0.8f, 1.0f });
            immediate::print("-= Programming & Art =-", width);
            immediate::print("Lorem Ipsum", width);
            ImGui::NewLine();
            immediate::print("-= Music & Sound Design =-", width);
            immediate::print("Dolores Amet", width);
            ImGui::NewLine();
            if (immediate::button("Back##credits_back", width)) {
                press_exit();
            }
            ImGui::PopStyleColor(1);
            ImGui::End();
        }

        void press_exit() {
            close();
        }

    private:
        bool *          open_flag;
    };
}