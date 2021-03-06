// name: inspector_common.hpp
// type: c++
// auth: is0urce
// desc: status ui panel class

#pragma once

#include "panel.hpp"
#include "immediate.hpp"
#include "field_description.hpp"

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
            ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0, 0, 0, 0 });
            ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
            ImGui::SetNextWindowSize(size);
            ImGui::Begin(label, nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);

            combine_name(body);
            combine_attributes(body);

            ImGui::End();
            ImGui::PopStyleColor(1);

            combine_buffs(label, pos, body);
        }

        void combine_attributes(body_component const& body) {
            immediate::bar(body.health(), "hp", { 0.5f, 0.0f, 0.0f, 1.0f });
            immediate::bar(body.energy(), "mp", { 0.0f, 0.0f, 0.5f, 1.0f });
            if (body.is_useable()) {
                ImGui::Text("- useable [E]");
            }
        }

        void combine_buffs(const char * label, ImVec2 const& start, body_component const& body) {
            unsigned int i = 0;
            for (auto const& affect : body.get_buffs()) {
                if (!affect.is_hidden()) {
                    combine_buff((label + std::to_string(i)).c_str(), { start.x , start.y - 64 - 48 * i }, affect);
                    ++i;
                }
            }
        }

        void combine_buff(const char * label, ImVec2 const& pos, body_component::buff_type const& buff) {
            immediate::style_color bg_transparent(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
            ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
            ImGui::Begin(label, nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);

            ImGui::Text(format_buff_name(buff).c_str());
            if (ImGui::IsItemHovered()) {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 20, 20 });
                ImGui::BeginTooltip();
                ImGui::Text(buff.name().c_str());
                ImGui::Text(buff.description().c_str());
                ImGui::NewLine();
                field_description::display_collection(buff);
                ImGui::EndTooltip();
                ImGui::PopStyleVar(1);
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

        std::string format_buff_name(body_component::buff_type const& buff) const {
            return buff.name() + " (" + std::to_string(buff.timer()) + ")";
        }
    };
}