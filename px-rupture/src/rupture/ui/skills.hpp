// name: status.hpp
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

					const float screen_width = ImGui::GetIO().DisplaySize.x;
					const float screen_height = ImGui::GetIO().DisplaySize.y;
					//const float window_width = 250.0f;
					//const float window_height = 100.0f;
					ImGui::SetNextWindowPos({ 22, screen_height - 50 }, ImGuiCond_Always);
					//ImGui::SetNextWindowSize({ window_width, window_height });

					ImGui::Begin("##skillset", nullptr
						, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse
						| ImGuiWindowFlags_AlwaysAutoResize);

					ImGui::NewLine();
					for (size_t i = 0, size = skillset->size(); i != size; ++i) {
						auto skill = skillset->get(i);
						if (skill) {
							auto const& state = skill->state();
							ImGui::SameLine();
							ImGui::Button((std::to_string(i) + ": " + state.name()).c_str());
							if (ImGui::IsItemHovered()) {
								ImGui::BeginTooltip();
								ImGui::Text(state.name().c_str());
								ImGui::Separator();
								ImGui::Text("%s", state.description().c_str());
								ImGui::Text("cost %d", state.cost());
								ImGui::Text("cd %d/%d", state.cooldown_remaining(), state.cooldown_duration());
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