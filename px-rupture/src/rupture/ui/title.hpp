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
			if (!context || context->possessed()) return;

			const float screen_width = ImGui::GetIO().DisplaySize.x;
			const float screen_height = ImGui::GetIO().DisplaySize.y;
			const float options_length = 200.0f;

			ImGui::SetNextWindowPos({ screen_width / 2 - options_length / 2, screen_height / 3 }, ImGuiCond_Always);
			ImGui::SetNextWindowSize({ screen_width, screen_height });
			ImGui::Begin("##title_screen", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);

			// title
			print("Gnomi##title_name", options_length);
			ImGui::NewLine();

			// options
			if (button("Start", options_length)) press_start();
			if (button("Exit##title_exit", options_length))	press_exit();

			ImGui::End();
		}

	private:
		void print(std::string const& text, float width) {
			line(text, width, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f });
		}

		bool button(std::string const& text, float width) {
			return line(text, width, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.5f, 0.1f, 0.0f, 1.0f }, { 1.0f, 0.3f, 0.0f, 1.0f });
		}
		bool line(std::string const& name, float width, ImVec4 const& bg, ImVec4 const& hover, ImVec4 active) {
			bool result;
			ImGui::PushStyleColor(ImGuiCol_Button, bg);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hover);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, active);
			result = ImGui::Button(name.c_str(), { width, 0.0f });
			ImGui::PopStyleColor(3); // button style
			return result;
		}
		void press_start() {
			context->start();
		}
		void press_exit() {
			context->shutdown();
		}

	private:
		environment * context;
	};
}