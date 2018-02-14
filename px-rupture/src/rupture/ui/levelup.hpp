// name: trait.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "panel.hpp"

#include "rupture/environment.hpp"
#include "rupture/es/transform_component.hpp"
#include "rupture/es/body_component.hpp"
#include "rupture/es/character_component.hpp"

#include "rupture/rl/traits.hpp"

#include <imgui/imgui.h>

namespace px::ui {

	class levelup final
		: public panel
	{
	public:
		virtual ~levelup() = default;
		levelup(environment * game)
			: context(game)
		{
		}

	protected:
		virtual void combine_panel() override {
			if (!context) return;

			transform_component * player = context->possessed();
			if (!player) return;
			body_component * body = player->linked<body_component>();
			if (!body) return;

			if (!features.levelup_required(*body)) return; // require levelup

			const float screen_width = ImGui::GetIO().DisplaySize.x;
			const float screen_height = ImGui::GetIO().DisplaySize.y;

			const float window_width = 300.0f;
			const float window_height = 125.0f;
			const float padding_x = 16;
			const float padding_y = 65;

			combine_option({ screen_width - window_width - padding_x, screen_height - window_height - padding_y }, { window_width, window_height }, *body, 0, 1);
		}

	private:
		void combine_option(ImVec2 const& pos, ImVec2 const& size, body_component & body, unsigned int index, unsigned int trait_id) {
			ImGui::PushID(index);
			ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
			ImGui::SetNextWindowSize(size);
			ImGui::Begin("trait##trait", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);

			auto perk = features[trait_id];
			if (perk) {
				ImGui::Text("'%s'", perk->label.c_str());
				ImGui::Text("'%s'", perk->description.c_str());
				if (ImGui::Button("Take##take_trait")) {
					select_option(trait_id, body);
				}
			}

			ImGui::End();
			ImGui::PopID();
		}

		void select_option(unsigned int trait_id, body_component & body) {
			features.give_trait(trait_id, body);
			body.set_level(body.level() + 1);
		}

	private:
		environment *	context;
		traits			features;
	};
}