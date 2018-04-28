// name: inspector_common.hpp
// type: c++
// auth: is0urce
// desc: status ui panel class

#pragma once

#include "panel.hpp"
#include "immediate.hpp"

#include "../es/body_component.hpp"

#include <string>

namespace px::ui {

    class inspector_common
        : public panel
    {
    public:
        virtual ~inspector_common() noexcept override = default;

    protected:
        void combine_status(const char * label, ImVec2 const& pos, ImVec2 const& size, body_component const& body) {

            ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
            ImGui::SetNextWindowSize(size);
            ImGui::Begin(label, nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);

            combine_name(body);
            combine_attributes(body);

            ImGui::End();

            combine_buffs(label, pos, body);
        }

        void combine_attributes(body_component const& body) {
            immediate::bar(body.health(), "hp", { 0.5f, 0.0f, 0.0f, 1.0f });
            immediate::bar(body.energy(), "mp", { 0.0f, 0.0f, 0.5f, 1.0f });
            if (body.is_useable()) {
                ImGui::Text("useable [e]");
            }
        }

        void combine_buffs(const char * label, ImVec2 const& start, body_component const& body) {
            int i = 0;
            for (auto const& buff : body.get_buffs()) {
                if (buff.is_hidden()) continue;

                ImVec2 pen = { start.x , start.y - 64 - 32 * i };
                combine_buff((label + std::to_string(i)).c_str(), pen, buff);
                ++i;
            }
        }

        void combine_buff(const char * label, ImVec2 const& pos, body_component::buff_type const& buff) {
            const float window_width = 300.0f;
            const float window_height = 32.0f;

            ImVec2 size{ window_width, window_height };

            ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
            ImGui::SetNextWindowSize(size);
            ImGui::Begin(label, nullptr, 
                ImGuiWindowFlags_NoTitleBar | 
                ImGuiWindowFlags_NoBringToFrontOnFocus | 
                ImGuiWindowFlags_NoResize | 
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);

            ImGui::Text(format_buff_name(buff).c_str());
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text(buff.name().c_str());
                ImGui::NewLine();
                combine_effect(buff, rl::effect::dot, "Damage");
                ImGui::EndTooltip();
            }

            ImGui::End();
        }

        // entity label in status
        void combine_name(body_component const& body) {
            auto const level = body.level();
            ImGui::Text(body.name().c_str());
  
            if (level != 0) {
                ImGui::SameLine();
                ImGui::Text(std::string(" (lvl " + std::to_string(level) + ")").c_str());
            }
        }

        void combine_effect(body_component::buff_type const& buff, rl::effect efx, std::string name) {
            if (buff.has_effect(efx)) {
                float const value = static_cast<float>(buff.accumulate({ efx }).magnitude0);
                ImGui::Text(format_effect_name(name, value).c_str());
            }
        }

        std::string format_buff_name(body_component::buff_type const& buff) const {
            return buff.name() + " (" + std::to_string(buff.timer()) + ")";
        }

        std::string format_effect_name(std::string name, float value) const {
            int const floor = static_cast<int>(value);
            return name + ": " + std::to_string(floor);
        }
    };
}