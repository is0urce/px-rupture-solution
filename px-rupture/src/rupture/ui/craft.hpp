// name: craft.hpp
// type: c++

#pragma once

#include "common.hpp"
#include "panel.hpp"

#include "rupture/environment.hpp"
#include "rupture/es/transform_component.hpp"
#include "rupture/es/body_component.hpp"
#include "rupture/es/container_component.hpp"

#include <px/rl/craft_activity.hpp>
#include <px/rl/craft_recipe.hpp>
#include "rupture/rl/craft_task.hpp"

#include <imgui/imgui.h>

#include <string>
#include <type_traits>
#include <vector>

namespace px {

	namespace {
		bool recipe_name_getter(void * data, int n, const char** result) {
			auto & vector = *static_cast<std::vector<rl::craft_recipe>*>(data);
			if (n >= 0 && n < static_cast<int>(vector.size())) {
				*result = vector[n].name.c_str();
				return true;
			}
			return false;
		}
	}

	class craft final
		: public panel
	{
	public:
		bool is_open() const {
			return game && game->has_access(rl::craft_activity::blacksmith);
		}
		void select() {
			rollback();
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
			fill_recipes();
		}

	protected:
		virtual void combine_panel() override {
			if (!game) return;
			if (!game->has_access(rl::craft_activity::blacksmith)) return;

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
			ImVec2 recipes_position{ craft_position.x - 64 - window_width, craft_position.y };
			ImVec2 inventory_position{ craft_position.x + 64 + window_width, craft_position.y };

			combine_recipes(recipes_position, { window_width, window_height });
			combine_slots(craft_position, { window_width, window_height });
			combine_inventory(inventory_position, { window_width, window_height });
		}

	private:
		void combine_recipes(ImVec2 const& window_position, ImVec2 const& window_size) {
			ImGui::SetNextWindowPos(window_position, ImGuiCond_Always);
			ImGui::SetNextWindowSize(window_size);
			ImGui::Begin("recipes##recipes_panel", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

			ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
			if (ImGui::ListBox("##recipes_list", &current_recipe, recipe_name_getter, static_cast<void*>(&recipes), static_cast<int>(recipes.size()), 15)) {
				select();
			}
			ImGui::PopItemWidth();

			ImGui::End();
		}
		void combine_slots(ImVec2 const& window_position, ImVec2 const& window_size) {
			ImGui::SetNextWindowPos(window_position, ImGuiCond_Always);
			ImGui::SetNextWindowSize(window_size);
			ImGui::Begin("craft##craft_panel", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

			if (current_recipe >= 0) {

				if (ImGui::Button("CRAFT##execute_craft", { 334, 32 })) {
					commit();
					game->close_workshop();
				}
				if (ImGui::Button("close##close_craft", { 334, 32 })) {
					rollback();
					game->close_workshop();
				}
			}
			else {

			}
			ImGui::End();
		}
		void combine_inventory(ImVec2 const& window_position, ImVec2 const& window_size) {
			ImGui::SetNextWindowPos(window_position, ImGuiCond_Always);
			ImGui::SetNextWindowSize(window_size);
			ImGui::Begin("inventory##inv_craft_resources", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

			//format_names(*container, names, [&](rl::item const& item) {
			//	return item.has_effect(rl::effect::ingredient_power, subtype_filter);
			//});
			//selected = -1;
			//ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
			//if (ImGui::ListBox("##craft_inventory_list", &selected, name_getter, static_cast<void*>(&names), static_cast<int>(names.size()), 15)) {
			//}
			//ImGui::PopItemWidth();

			ImGui::End();
		}
		void fill_recipes() {
			recipes.push_back({ "sword", "", rl::craft_activity::blacksmith, 4, 1.0, 1.0 });
			recipes.push_back({ "armor", "", rl::craft_activity::blacksmith, 6, 1.0, 1.0 });
		}

	private:
		environment *					game;

		std::vector<uq_ptr<rl::item>>	slots;
		std::vector<rl::craft_recipe>	recipes;
		int								current_recipe;
	};
}