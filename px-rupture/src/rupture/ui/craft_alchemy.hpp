// name: craft_alchemy.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "panel.hpp"

#include "craft_common.hpp"

#include "rupture/environment.hpp"
#include "rupture/es/transform_component.hpp"
#include "rupture/es/body_component.hpp"
#include "rupture/es/container_component.hpp"

#include <px/rl/craft_activity.hpp>
#include "rupture/rl/craft_recipe.hpp"
#include "rupture/rl/craft_task.hpp"
#include "rupture/rl/craft_result.hpp"

#include <imgui/imgui.h>

#include <string>
#include <type_traits>
#include <vector>

namespace px {

	class craft_alchemy final
		: public panel
	{
	public:
		bool is_open() const {
			return game && game->has_access(rl::craft_activity::alchemy);
		}

		void release_items() {
			if (!container) return;

			uq_ptr<rl::item> item;
			goto query;
			while (item) {
				container->add(std::move(item));
			query:
				item = task.remove();
			}
		}

		bool execute_craft() {
			if (container && task.is_complete()) {
				container->add(rl::craft_result::create_potion(0, 0));
				return true;
			}
			return false;
		}

	public:
		virtual ~craft_alchemy() = default;
		craft_alchemy(environment * context)
			: game(context)
			, container(nullptr)
		{
			task.reset(3); // three reagent recipes
		}

	protected:
		virtual void combine_panel() override {
			if (!game || !game->has_access(rl::craft_activity::alchemy)) return;

			container = game->possessed()->qlink<container_component, body_component>();

			if (!container) return;

			const float screen_width = ImGui::GetIO().DisplaySize.x;
			const float screen_height = ImGui::GetIO().DisplaySize.y;
			const float window_width = 350.0f;
			const float window_height = 538.0f;

			ImVec2 craft_position{ screen_width / 2 - window_width / 2, screen_height / 2 - window_height / 2 };
			ImVec2 inventory_position{ craft_position.x + 64 + window_width, craft_position.y };

			combine_slots(craft_position, { window_width, window_height });
			combine_inventory(inventory_position, { window_width, window_height });
		}

	private:
		void combine_slots(ImVec2 const& window_position, ImVec2 const& window_size) {
			ImGui::SetNextWindowPos(window_position, ImGuiCond_Always);
			ImGui::SetNextWindowSize(window_size);
			ImGui::Begin("alchemy##craft_panel", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

			ImGui::BeginGroup();
			ImGui::BeginChild("slots view", ImVec2(0, -2 * ImGui::GetItemsLineHeightWithSpacing())); // Leave room for 1 line below us

			for (size_t size = task.reagent_count(), idx = 0; idx != size; ++idx) {
				auto item_ptr = task[idx];
				if (item_ptr) {
					ImGui::Text(item_ptr->name().c_str());
				}
				else {
					ImGui::TextDisabled("-- empty --");
				}
			}

			ImGui::EndChild();
			ImGui::BeginChild("buttons");
			if (ImGui::Button("brew", { 334, 32 })) {
				if (task.is_complete()) {
					execute_craft();
					release_items();
					game->close_workshop();
				}
			}
			if (ImGui::Button("close", { 334, 32 })) {
				release_items();
				game->close_workshop();
			}
			ImGui::EndChild();
			ImGui::EndGroup();

			ImGui::End();
		}
		void combine_inventory(ImVec2 const& window_position, ImVec2 const& window_size) {
			ImGui::SetNextWindowPos(window_position, ImGuiCond_Always);
			ImGui::SetNextWindowSize(window_size);
			ImGui::Begin("inventory##inv_craft_resources", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

			const auto filter = [&](rl::item const& item) {
				return item.has_effect(rl::effect::ingredient_power, static_cast<rl::item::enhancement_type::integer_type>(rl::craft_activity::alchemy));
			};

			format_names(*container, inventory_names, filter);

			int selected = -1;
			ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
			if (ImGui::ListBox("##craft_inventory_list", &selected, name_getter, static_cast<void*>(&inventory_names), static_cast<int>(inventory_names.size()), 16)) {
				size_t current_idx = 0;
				size_t absolute_idx = 0;
				size_t relative_idx = 0;
				container->enumerate([&](rl::item & item) {
					if (filter(item)) {
						if (relative_idx == selected) {
							absolute_idx = current_idx;
						}
						++relative_idx;
					}
					++current_idx;
				});
				task.add(container->remove(absolute_idx));
			}
			ImGui::PopItemWidth();

			ImGui::End();
		}

	private:
		environment *					game;				// game context
		container_component *			container;			// user inventory
		rl::craft_task					task;				// ingredient selected
		std::vector<std::string>		inventory_names;	// inventory items names cashe
	};
}