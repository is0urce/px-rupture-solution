// name: inventory.hpp
// type: c++

#pragma once

#include "panel.hpp"

#include "rupture/environment.hpp"

#include <imgui/imgui.h>

#include <string>
#include <vector>

namespace px::ui {

	inline bool name_getter(void * data, int n, const char** result) {
		auto & vector = *static_cast<std::vector<std::string>*>(data);
		if (n >= 0 && n < static_cast<int>(vector.size())) {
			*result = vector[n].c_str();
			return true;
		}
		return false;
	}

	class inventory final
		: public panel
	{
	public:
		virtual ~inventory()
		{
		}
		inventory(environment * env, bool * p_open)
			: game(env)
			, opened(p_open)
		{
		}

	protected:
		virtual void combine_panel() override
		{
			if (!game) return;

			if (!*opened) return;

			const float screen_width = ImGui::GetIO().DisplaySize.x;
			const float screen_height = ImGui::GetIO().DisplaySize.y;
			const float window_width = 330.0f;
			const float window_height = 330.0f;
			ImGui::SetNextWindowPos({ screen_width / 2 - window_width / 2, screen_height / 2 - window_height / 2 }, ImGuiCond_Always);
			ImGui::SetNextWindowSize({ window_width, window_height });
			ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);

			transform_component * target = game->possessed();
			if (target) {
				auto body = target->linked<body_component>();
				if (body) {
					auto cont = body->linked<container_component>();

					if (cont) {
						ImGui::Begin((body->name() + "##inventory").c_str()
							, opened
							, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );

						//ImGui::Text("%s", body->name().c_str());
						//ImGui::NewLine();

						format_names(*cont);
						int current_unselected = -1;
						ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
						ImGui::ListBox("##inventory_list", &current_unselected, name_getter, static_cast<void*>(&names), static_cast<int>(names.size()), 10);
						ImGui::PopItemWidth();

						if (ImGui::Button("close##close_inventory")) {
							*opened = false;
						}

						ImGui::End();
					}
				}
			}
		}

	private:
		void format_names(container_component & container)
		{
			names.clear();
			container.enumerate([&](rl::item const& item) {
				names.push_back(item.name());
			});
		}

	private:
		environment *				game;
		std::vector<std::string>	names;
		bool *						opened;
	};
}