// name: craft_alchemy.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "panel.hpp"
#include "craft_station.hpp"

#include "rupture/rl/craft_result.hpp"

namespace px {

	class craft_alchemy final
		: public craft_station<rl::craft_activity::alchemy>
	{
	public:
		void cancel_task() {
			release_items();
		}

	public:
		virtual ~craft_alchemy() = default;
		craft_alchemy(environment * context)
			: craft_station(context)
		{
			reset_recipe();
		}

	protected:
		virtual void combine_panel() override {
			if (!is_open()) return;
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
		void reset_recipe() {
			task.reset(3); // three reagent recipes
		}
		rl::item const* execute_craft() {
			rl::item const* result = nullptr;
			if (container && task.is_complete()) {
				auto item = rl::craft_result::create_potion(rl::craft_result::calculate_essence(task), rl::craft_result::calculate_power(task).magnitude0);
				result = item.get();
				container->add(std::move(item));
				sort(*container);
				consume_items();
				reset_recipe();
			}
			return result;
		}
		void combine_slots(ImVec2 const& window_position, ImVec2 const& window_size) {
			ImGui::SetNextWindowPos(window_position, ImGuiCond_Always);
			ImGui::SetNextWindowSize(window_size);
			ImGui::Begin("alchemy##craft_panel", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			ImGui::BeginGroup();

			ImGui::BeginChild("slots view", ImVec2(0, -2 * ImGui::GetItemsLineHeightWithSpacing())); // Leave room for 1 line below us
			combine_reagents();
			ImGui::EndChild();

			ImGui::BeginChild("buttons");
			if (ImGui::Button("brew", { 334, 32 })) {
				if (task.is_complete()) {
					auto item = execute_craft();
					game->close_workshop();
					game->popup("+ " + item->name(), { 1, 1, 1 });
					game->end_turn(1);
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
	};
}