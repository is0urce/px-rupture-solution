#pragma once

#include <imgui/imgui.h>
#include <string>

namespace px {

    bool is_true(bool * flag) noexcept {
        return flag && *flag;
    }

    void set_flag(bool * flag) noexcept {
        if (flag) {
            *flag = true;
        }
    }

    void set_flag(bool * flag, bool flag_value) noexcept {
        if (flag) {
            *flag = flag_value;
        }
    }

    class design {
    public:
        static constexpr float options_width() {
            return 200.0f;
        }
        static constexpr int window_caption() {
            return 0
                | ImGuiWindowFlags_NoResize
                | ImGuiWindowFlags_NoMove
                | ImGuiWindowFlags_NoScrollbar
                | ImGuiWindowFlags_NoScrollWithMouse
                | ImGuiWindowFlags_NoCollapse;
        }
        static constexpr int window_flat() {
            return 0
                | window_caption()
                | ImGuiWindowFlags_NoTitleBar;
        }
        static ImVec2 menu_position() {
            float const screen_width = ImGui::GetIO().DisplaySize.x;
            float const screen_height = ImGui::GetIO().DisplaySize.y;
            float constexpr menu_width = options_width();
            return ImVec2{ screen_width / 2 - menu_width / 2, screen_height / 3 };
        }
    };
}