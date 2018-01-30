// name: inventory.hpp
// type: c++

#pragma once

//#include "cfcommon.hpp"
#include "panel.hpp"

#include "rupture/environment.hpp"
#include "rupture/es/transform_component.hpp"
#include "rupture/es/body_component.hpp"
#include "rupture/es/container_component.hpp"

#include <imgui/imgui.h>

#include <string>
#include <vector>

namespace px {

	class inventory final
		: public panel
	{
	public:
		virtual ~inventory() = default;
		inventory(environment * env, bool * p_open)
			: game(env)
			, opened(p_open)
			, selected_slot(nullptr)
			, selected(-1)
		{
		}

	protected:
		virtual void combine_panel() override {
			if (!game) return;
			if (!*opened) return;
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
			ImVec2 inventory_position{ screen_width / 2 - window_width / 2, screen_height / 2 - window_height / 2 };
			ImVec2 inventory_size{ window_width, window_height };
			ImGui::SetNextWindowPos(inventory_position, ImGuiCond_Always);
			ImGui::SetNextWindowSize(inventory_size);

			ImGui::Begin((body->name() + " inventory##inventory_panel").c_str()
				, nullptr
				, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

			format_names(*container, names);
			selected = -1;
			ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
			if (ImGui::ListBox("##inventory_list", &selected, name_getter, static_cast<void*>(&names), static_cast<int>(names.size()), 15)) {
				body->equip(selected);
				sort(*container);
				selected = -1;
			}
			ImGui::PopItemWidth();

			if (ImGui::Button("close##close_inventory", { 334, 32 })) {
				*opened = false;
			}

			ImGui::End();

			ImVec2 slot_position = inventory_position;
			ImVec2 slot_size{ 200, 32 };
			slot_position.x -= slot_size.x + 32;
			slot_position.y += 64;

			selected_slot = nullptr;

			combine_slot("hands", slot_position, slot_size, *body, rl::equipment::hand);
			slot_position.y += slot_size.y;

			slot_position.y += 8;
			combine_slot("head", slot_position, slot_size, *body, rl::equipment::head);
			slot_position.y += slot_size.y;

			ImVec2 inspector_position = inventory_position;
			inspector_position.x += inventory_size.x + 32;
			inspector_position.y += 64;

			rl::item * inspect = nullptr;
			if (selected != -1) {
				inspect = container->get(selected);
			}
			if (selected_slot) inspect = selected_slot;
			combine_inspector(inspect, inspector_position);
		}

	private:
		void combine_slot(std::string const& name, ImVec2 const& position, ImVec2 const& size, body_component & body, rl::equipment slot) {
			ImGui::SetNextWindowPos(position, ImGuiCond_Always);
			ImGui::SetNextWindowSize(size);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
			ImGui::Begin((name + "title").c_str(),
				nullptr,
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

			rl::item * ptr = body.equipment(slot);
			if (ptr) {
				ImGui::PushID(ptr);
				if (ImGui::Button(ptr->name().c_str(), size)) {
					body.unequip(slot);
					sort(*body.linked<container_component>());
				}
				if (ImGui::IsItemHovered()) {
					selected_slot = ptr;
					selected = -1;
				}
				ImGui::PopID();
			}
			else {
				ImGui::Button(name.c_str(), size);
			}

			ImGui::End();
			ImGui::PopStyleVar(2);
		}
		void combine_inspector(rl::item const* ptr, ImVec2 const& position) {
			if (!ptr) return;

			ImGui::SetNextWindowPos(position, ImGuiCond_Always);
			ImGui::Begin((ptr->name() + "##item_inspector_title").c_str(),
				nullptr,
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

			ImGui::Text(ptr->name().c_str());
			ImGui::Text(ptr->description().c_str());

			body_component::enhancement_type enhancement;
			enhancement = ptr->accumulate(body_component::enhancement_type::zero(rl::effect::damage));
			if (enhancement.magnitude0 > 0) {
				int dmg = static_cast<int>(enhancement.magnitude0);
				ImGui::Text("Damage: %d", dmg);
			}

			ImGui::Text("Quantity: %d", ptr->count());

			ImGui::End();
		}

	private:
		environment *				game;
		std::vector<std::string>	names;
		bool *						opened;
		int							selected;		// hovered item in container list
		rl::item *					selected_slot;	// hovered item is equipment slot
	};
}