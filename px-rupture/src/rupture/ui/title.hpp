// name: title.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "panel.hpp"

#include "rupture/environment.hpp"
#include "rupture/es/transform_component.hpp"

#include <imgui/imgui.h>

namespace px::ui {

	class title final
		: public panel
	{
	public:
		virtual ~title() = default;
		title(environment * game)
			: context(game)
		{
		}

	protected:
		virtual void combine_panel() override {
			if (!context) return;

			transform_component * player = context->possessed();
			if (!player) {

				const float screen_width = ImGui::GetIO().DisplaySize.x;
				const float screen_height = ImGui::GetIO().DisplaySize.y;

				ImGui::SetNextWindowPos({ screen_width / 3, screen_height / 3 }, ImGuiCond_Always);
				ImGui::SetNextWindowSize({ screen_width / 3, screen_width / 3 });
				ImGui::Begin("title##name"
					, nullptr
					, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
				ImGui::Text("Gnome Fury");
				ImGui::End();

				const float menu_width = 300.0f;
				const float menu_height = 125.0f;
				const float padding_x = 16;
				const float padding_y = 65;
				ImGui::SetNextWindowPos({ screen_width - menu_width - padding_x, screen_height - menu_height - padding_y }, ImGuiCond_Always);
				ImGui::SetNextWindowSize({ menu_width, menu_height });
				ImGui::Begin("start##title"
					, nullptr
					, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);

				if (ImGui::Button("Start##title_start")) {
					context->start();
				}

				ImGui::End();
			}
		}

	private:
		environment * context;
	};
}