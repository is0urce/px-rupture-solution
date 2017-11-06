// name: skills.hpp
// type: c++

#pragma once

#include "panel.hpp"

#include "rupture/environment.hpp"
#include "rupture/es/transform_component.hpp"
#include "rupture/es/body_component.hpp"
#include "rupture/es/character_component.hpp"

#include <imgui/imgui.h>

namespace px::ui {

	class skills final
		: public panel
	{
	public:
		virtual ~skills()
		{
		}
		skills(environment * env)
			: game(env)
		{
		}

	protected:
		virtual void combine_panel() override
		{
			if (!game) return;

			transform_component * target = game->possessed();
			if (target) {
				if (auto skillset = target->qlink<character_component, body_component>()) {

					const float screen_height = ImGui::GetIO().DisplaySize.y;
					ImGui::SetNextWindowPos({ 16, screen_height - 50 }, ImGuiCond_Always);

					ImGui::Begin("##skillset", nullptr
						, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse
						| ImGuiWindowFlags_AlwaysAutoResize);

					//ImGui::PushStyleColor(ImGuiCol_Button, { 1, 0, 0, 1 });
					//ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 1, 0, 0, 1 });
					//ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1, 0, 0, 1 });

					ImGui::NewLine();

					transform_component * locked_pawn = game->target();
					body_component * locked = locked_pawn ? locked_pawn->linked<body_component>() : nullptr;
					body_component * user = target->linked<body_component>();
					for (size_t i = 1, size = skillset->size(); i != size; ++i) {
						skill * ability = skillset->get(i);
						if (ability) {
							auto const& state = ability->state();
							ImGui::SameLine();

							ImVec4 color = { 0.5, 0.5, 0.5, 1 };

							if (ability->is_targeted()) {
								if (ability->useable(user, locked)) {
									color = { 1.0, 0.5, 0.5, 1 };
								}
							}
							else {
								if (ability->useable(user, game->area())) {
									color = { 1.0, 0.5, 0.5, 1 };
								}
							}
							ImGui::PushStyleColor(ImGuiCol_Button, color);
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
							ImGui::Button(state.alias().c_str(), { 128, 32 });
							ImGui::PopStyleColor(3);

							if (ImGui::IsItemHovered()) {
								ImGui::BeginTooltip();
								ImGui::Text("%d) %s", i, state.name().c_str());
								ImGui::Separator();
								ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 500);
								ImGui::TextWrapped("%s", state.description().c_str());
								ImGui::PopTextWrapPos();
								auto cost = state.cost();
								auto cd = state.cooldown_duration();
								if (cost != 0) {
									ImGui::Text("cost: %d", cost);
								} 
								if (cd != 0) {
									ImGui::Text("cooldown: %d/%d", state.cooldown_remaining(), cd);
								}

								ImGui::EndTooltip();
							}
						}
					}

					ImGui::End();
				}
			}
		}

	private:
		environment * game;
	};
}