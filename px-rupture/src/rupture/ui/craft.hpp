// name: craft.hpp
// type: c++

#pragma once

#include "common.hpp"
#include "panel.hpp"

#include "rupture/environment.hpp"
#include "rupture/es/transform_component.hpp"
#include "rupture/es/body_component.hpp"
#include "rupture/es/container_component.hpp"

#include <imgui/imgui.h>

#include <string>
#include <vector>

namespace px {

	class craft final
		: public panel
	{
	public:
		bool is_open() const {
			return game && game->has_access(rl::workshop::blacksmith);
		}
		void select() {
			rollback();

			switch (recipe) {
			case 0:
				subtype_filter = static_cast<int>(rl::workshop::blacksmith);
				break;
			default:
				subtype_filter = 0;
				break;
			}
		}
		void rollback() {
		}
		void commit() {
		}

	public:
		virtual ~craft() = default;
		craft(environment * env)
			: game(env)
		{
		}

	protected:
		virtual void combine_panel() override {

			if (!game) return;
			if (!game->has_access(rl::workshop::blacksmith)) return;

			transform_component * target = game->possessed();
			if (!target) return;
			body_component * body = target->linked<body_component>();
			if (!body) return;
			container_component * container = body->linked<container_component>();
			if (!container) return;

			const float screen_width = ImGui::GetIO().DisplaySize.x;
			const float screen_height = ImGui::GetIO().DisplaySize.y;
			const float window_width = 350.0f;
			const float window_height = 538.0f;
			ImVec2 craft_position{ screen_width / 2 - window_width / 2, screen_height / 2 - window_height / 2 };
			ImVec2 craft_size{ window_width, window_height };

			ImGui::SetNextWindowPos(craft_position, ImGuiCond_Always);
			ImGui::SetNextWindowSize(craft_size);
			ImGui::Begin("craft##craft_panel"
				, nullptr
				, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

			format_names(*container, names, [&](rl::item const& item) {
				return item.has_effect(rl::effect::ingredient_power, subtype_filter);
			});
			selected = -1;
			ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
			if (ImGui::ListBox("##craft_inventory_list", &selected, name_getter, static_cast<void*>(&names), static_cast<int>(names.size()), 15)) {
			}
			ImGui::PopItemWidth();

			if (ImGui::Button("CRAFT##execute_craft", { 334, 32 })) {
				commit();
				game->close_workshop();
			}
			if (ImGui::Button("close##close_craft", { 334, 32 })) {
				game->close_workshop();
			}
			ImGui::End();

			ImVec2 recipes_position{ screen_width / 2 - window_width / 2, screen_height / 2 - window_height / 2 };
			ImVec2 recipes_size{ window_width, window_height };
			ImGui::Begin("recipes##craft_panel"
				, nullptr
				, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

			ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
			if (ImGui::ListBox("##craft_inventory_list", &recipe, name_getter, static_cast<void*>(&recipes), static_cast<int>(recipes.size()), 15)) {
				select();
			}
			ImGui::PopItemWidth();
			ImGui::End();
		}

	private:
		environment *					game;
		std::vector<std::string>		recipes;
		std::vector<std::string>		names;
		int								selected;		// hovered item in container list
		rl::item *						selected_slot;	// hovered item is equipment slot
		std::vector<uq_ptr<rl::item>>	slots;
		int								recipe;
		int								subtype_filter;
	};
}