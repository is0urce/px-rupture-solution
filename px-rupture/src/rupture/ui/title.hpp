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
				const float options_length = 200.0f;

				ImGui::SetNextWindowPos({ screen_width / 2 - options_length / 2, screen_height / 3 }, ImGuiCond_Always);
				ImGui::SetNextWindowSize({ screen_width, screen_height});
				ImGui::Begin("##title_screen", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);

				// title
				ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.0f, 0.0f, 0.0f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.0f, 0.0f, 0.0f, 1.0f });
				ImGui::Button("Gnomi##title_name", { options_length, 0.0f });
				ImGui::PopStyleColor(3);
				ImGui::NewLine();

				//ImGui::End();

				//const float menu_width = 300.0f;
				//const float menu_height = 125.0f;
				//const float padding_x = 16;
				//const float padding_y = 65;
				//ImGui::SetNextWindowPos({ screen_width - menu_width - padding_x, screen_height - menu_height - padding_y }, ImGuiCond_Always);
				//ImGui::SetNextWindowSize({ menu_width, menu_height });
				//ImGui::Begin("start##title", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);

				ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.5f, 0.1f, 0.0f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 1.0f, 0.3f, 0.0f, 1.0f });
				if (ImGui::Button("Start##title_start", { options_length, 0.0f })) {
					start();
				}
				ImGui::PopStyleColor(3); // button style

				ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.5f, 0.1f, 0.0f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 1.0f, 0.3f, 0.0f, 1.0f });
				if (ImGui::Button("Exit##title_start", { options_length, 0.0f })) {
					exit();
				}
				ImGui::PopStyleColor(3); // button style

				ImGui::End();
			}
		}

	private:
		void start() {
			context->start();
		}
		void exit() {
			context->shutdown();
		}

	private:
		environment * context;
	};
}