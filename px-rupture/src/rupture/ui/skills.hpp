// name: skills.hpp
// type: c++

#pragma once

#include "panel.hpp"

#include "../environment.hpp"
#include "../es/transform_component.hpp"
#include "../es/body_component.hpp"
#include "../es/character_component.hpp"

#include <imgui/imgui.h>

namespace px::ui {

    class skills final
        : public panel
    {
    public:
        virtual ~skills() override = default;

        skills(environment * game)
            : context(game) {
        }

    protected:
        virtual void combine_panel() override {
            if (auto target = context ? context->controlled() : nullptr) {
                if (character_component const* const person = target->qlink<character_component, body_component>()) {

                    float const screen_height = ImGui::GetIO().DisplaySize.y;
                    ImGui::SetNextWindowPos({ 16, screen_height - 50 }, ImGuiCond_Always);
                    ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0, 0.0, 0.0, 0.0 });
                    ImGui::Begin("##skillset", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

                    ImGui::NewLine();

                    auto const locked_pawn = context->target();
                    auto const locked = locked_pawn ? locked_pawn->linked<body_component>() : nullptr;
                    auto const user = target->linked<body_component>();

                    for (size_t i = 1, total = person->size(); i != total; ++i) {
                        if (skill const* const ability = person->get(i)) {
                            bool const is_useable = context->has_control() && (ability->is_targeted()
                                ? ability->useable(user, locked)
                                : ability->useable(user, context->area()));
                            auto const& state = ability->state();

                            ImVec4 color = is_useable ? ImVec4(1.0f, 0.5f, 0.5f, 1.0f) : ImVec4(0.5f, 0.5f, 0.5f, 1.0f);

                            ImGui::SameLine();
                            ImGui::PushStyleColor(ImGuiCol_Button, color);
                            ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
                            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
                            ImGui::Button(state.alias().c_str(), { 128, 32 });
                            ImGui::PopStyleColor(3);
                            if (ImGui::IsItemHovered()) {
                                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 20, 20 });
                                ImGui::BeginTooltip();
                                ImGui::Text("%d. %s", i, state.name().c_str());
                                ImGui::Separator();
                                ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 500);
                                ImGui::TextWrapped("%s", state.description().c_str());
                                ImGui::PopTextWrapPos();
                                auto const cost = state.cost();
                                auto const cd = state.cooldown_duration();
                                if (cost != 0) {
                                    ImGui::Text("cost: %d", cost);
                                }
                                if (cd != 0) {
                                    ImGui::Text("cooldown: %d/%d", state.cooldown_remaining(), cd);
                                }
                                ImGui::EndTooltip();
                                ImGui::PopStyleVar(1);
                            }
                        }
                    }

                    ImGui::End();
                    ImGui::PopStyleColor(1);
                }
            }
        }

    private:
        environment * context;
    };
}