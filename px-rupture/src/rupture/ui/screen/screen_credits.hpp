// name: screen_credits.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "../panel.hpp"
#include "../immediate.hpp"
#include "../design.hpp"

namespace px {

    class screen_credits final
        : public panel
    {
    public:
        virtual ~screen_credits() noexcept override = default;

        screen_credits(bool * flag) noexcept
            : open_flag(flag) {
        }

    protected:
        virtual void combine_panel() override {
            if (is_open()) {
                ImVec2 const& screen = ImGui::GetIO().DisplaySize;
                float const options_width = 600.0f;

                combine_credits({ screen.x * 0.5f - options_width * 0.5f, screen.y * 0.25f }, options_width);
            }
        }

    private:
        bool is_open() const noexcept {
            return is_true(open_flag);
        }

        void close() noexcept {
            clear_flag(open_flag);
        }

        void combine_credits(ImVec2 const& position, float width) {
            // main options
            ImGui::SetNextWindowPos(position, ImGuiCond_Always);
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