// name: immediate.hpp
// type: c++
// auth: is0urce
// type: class-aggregated static functions

#pragma once

#include <imgui/imgui.h>
#include <string>

namespace px {

    class immediate final {
    public:
        // draw resource bar
        template <typename ResourceOption>
        static bool bar(ResourceOption const& resource, std::string const& prefix, ImVec4 const& tint) {
            if (!resource) return false;
            auto const current = resource->current();
            auto const maximum = resource->maximum();
            float const fraction = (maximum == 0) ? 0.0f : current / static_cast<float>(maximum);
            ImGui::Text(prefix.c_str());
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, tint);
            ImGui::ProgressBar(fraction, { -1, 0 }, (std::to_string(current) + "/" + std::to_string(maximum)).c_str());
            ImGui::PopStyleColor(1);
            return true;
        }

        static bool print(std::string const& text, float width) {
            return line(text, width, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f });
        }

        static bool button(std::string const& text, float width) {
            return line(text, width, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.5f, 0.1f, 0.0f, 1.0f }, { 1.0f, 0.3f, 0.0f, 1.0f });
        }

        static bool button(std::string const& text, float width, bool enabled) {
            return line(text, width, { 0.0f, 0.0f, 0.0f, 1.0f }, enabled ? ImVec4{ 0.1f, 0.1f, 0.0f, 1.0f } : ImVec4{ 0.1f, 0.1f, 0.0f, 1.0f }, { 0.1f, 0.1f, 0.0f, 1.0f });
        }

        static bool line(std::string const& name, float width, ImVec4 const& bg, ImVec4 const& hover, ImVec4 active) {
            bool result;
            ImGui::PushStyleColor(ImGuiCol_Button, bg);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hover);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, active);
            result = ImGui::Button(name.c_str(), { width, 0.0f });
            ImGui::PopStyleColor(3); // button style
            return result;
        }

        // dim
        static void dim() {
            ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.5f });
            ImGui::Begin("##dim_overlay", nullptr, 0
                | ImGuiWindowFlags_NoTitleBar
                | ImGuiWindowFlags_NoResize
                | ImGuiWindowFlags_NoMove
                | ImGuiWindowFlags_NoScrollbar
                | ImGuiWindowFlags_NoScrollWithMouse
                | ImGuiWindowFlags_NoCollapse
                | ImGuiWindowFlags_NoSavedSettings);
            ImGui::SetWindowFocus();
            ImGui::End();
            ImGui::PopStyleColor(1);
        }

        static void splash_version() {
            ImGui::SetNextWindowPos({ 20, ImGui::GetIO().DisplaySize.y - 50 }, ImGuiCond_Always);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
            ImGui::Begin("##splash_version", nullptr, 0
                | ImGuiWindowFlags_NoTitleBar
                | ImGuiWindowFlags_NoResize
                | ImGuiWindowFlags_NoMove
                | ImGuiWindowFlags_NoScrollbar
                | ImGuiWindowFlags_NoScrollWithMouse
                | ImGuiWindowFlags_NoCollapse
                | ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::SetWindowFocus();
            ImGui::PushStyleColor(ImGuiCol_Text, { 0.6f, 0.6f, 0.6f, 1.0f });
            ImGui::Text("Gnomi: Caves of Nschryldamdshuncz, 2018, version 0.0.9.0 (x64)");
            ImGui::PopStyleColor(1); // text color
            ImGui::End();
            ImGui::PopStyleColor(1); // window bg
        }
    };
}