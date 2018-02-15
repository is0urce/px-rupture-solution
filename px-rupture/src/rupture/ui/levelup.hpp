// name: levelup.hpp
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
			character_component * character = body->linked<character_component>();
			if (!character) return;

			if (!features.levelup_required(*body)) return; // require levelup

			const float screen_width = ImGui::GetIO().DisplaySize.x;
			const float screen_height = ImGui::GetIO().DisplaySize.y;
			const float padding_x = 16;
			const float padding_y = 65;

			const float window_width = screen_width / 3 - padding_x * 3;
			const float window_height = screen_height * 2 / 3 - padding_y;

			unsigned int trait_1, trait_2, trait_3;
			std::tie(trait_1, trait_2, trait_3) = features.select_traits(*body);

			combine_option({ padding_x * (1 + 2 * 0) + window_width * 0, screen_height / 4 }, { window_width, window_height }, *body, 0, trait_1);
			combine_option({ padding_x * (1 + 2 * 1) + window_width * 1, screen_height / 4 }, { window_width, window_height }, *body, 1, trait_2);
			combine_option({ padding_x * (1 + 2 * 2) + window_width * 2, screen_height / 4 }, { window_width, window_height }, *body, 2, trait_3);
		}

	private:
		void combine_option(ImVec2 const& pos, ImVec2 const& window_size, body_component & body, unsigned int index, unsigned int trait_id) {
			if (auto perk = features[trait_id]) {
				ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
				ImGui::SetNextWindowSize(window_size);
				ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.25f, 0.25f, 0.25f, 1.0f });
				ImGui::Begin(("trait_card##idx" + std::to_string(index)).c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);

				ImGui::Text("%s", perk->label.c_str());
				ImGui::Text("%s", perk->description.c_str());
				if (ImGui::Button("Take##take_this_trait")) {
					increment_level(trait_id, body);
				}

				ImGui::End();
				ImGui::PopStyleColor(1); // window style
			}
		}

		void increment_level(unsigned int trait_id, body_component & body) {
			features.give_trait(trait_id, body);
			body.set_level(body.level() + 1);
		}

	private:
		environment *	context;
		traits			features;
	};
}