// name: design.hpp

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

    void clear_flag(bool * flag) noexcept {
        if (flag) {
            *flag = false;
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
                | ImGuiWindowFlags_NoResize
                | ImGuiWindowFlags_NoMove
                | ImGuiWindowFlags_NoScrollbar
                | ImGuiWindowFlags_NoScrollWithMouse
                | ImGuiWindowFlags_NoCollapse
                | ImGuiWindowFlags_NoTitleBar;
        }

        static ImVec2 menu_position() {
            float const screen_width = ImGui::GetIO().DisplaySize.x;
            float const screen_height = ImGui::GetIO().DisplaySize.y;
            float constexpr menu_width = options_width();
            return { screen_width / 2 - menu_width / 2, screen_height / 3 };
        }

        static constexpr ImVec4 panel_background() {
            return { 0.02f, 0.23f, 0.38f, 0.25f };
        }
        static constexpr ImVec4 panel_title_color() {
            return { 0.03f, 0.39f, 0.64f, 1.0f };
        }
        static constexpr ImVec4 button_idle_color() {
            return { 0.61f, 0.34f, 0.00f, 1.0f };
        }
        static constexpr ImVec4 button_hover_color() {
            return { 0.78f, 0.44f, 0.00f, 1.0f };
        }
        static constexpr ImVec4 button_active_color() {
            return { 0.99f, 0.56f, 0.00f, 1.0f };
        }
        static constexpr ImVec4 button_disabled_color() {
            return { 0.30f, 0.30f, 0.30f, 1.0f };
        }
    };
}