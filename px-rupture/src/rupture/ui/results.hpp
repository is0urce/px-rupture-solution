// name: results.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "panel.hpp"

#include "rupture/environment.hpp"
#include "rupture/es/transform_component.hpp"
#include "rupture/es/body_component.hpp"
#include "rupture/es/character_component.hpp"

#include <imgui/imgui.h>

namespace px::ui {

	class results final
		: public panel
	{
	public:
		virtual ~results() = default;

		results(environment * game)
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
			bool show = character->has_trait("game_over");
			if (!show) return;

			const float screen_width = ImGui::GetIO().DisplaySize.x;
			const float screen_height = ImGui::GetIO().DisplaySize.y;
			const float	window_width = 200.0f;
			const float window_height = screen_height / 2;
			const float padding_x = ImGui::GetStyle().FramePadding.x * 2; // multiplier from pixel size multiplication
			const float padding_y = ImGui::GetStyle().FramePadding.y * 2;

			ImGui::SetNextWindowPos({ screen_width / 2 - window_width / 2 - padding_x, screen_height / 4 - padding_y }, ImGuiCond_Always);
			ImGui::SetNextWindowSize({ window_width + padding_x * 2, window_height + padding_y * 2 });
			ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 1.0f });
			ImGui::Begin("##results_content", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);

			if (body->is_alive()) {
				print("Victory!", window_width);
				ImGui::NewLine();
				print("You have won.", window_width);
				ImGui::NewLine();
				if (button("Continue", window_width)) press_resume(*character);
				if (button("Titles", window_width)) press_title();

			}
			else {
				print("Game Over", window_width);
				ImGui::NewLine();
				print("You have died.", window_width);
				ImGui::NewLine();
				if (button("Load", window_width)) press_load();
				if (button("Restart", window_width)) press_restart();
			}

			ImGui::End();
			ImGui::PopStyleColor(1); // window style
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

		void press_load() {
			context->load("quicksave");
		}

		void press_resume(character_component & character) {
			character.remove_trait("game_over");
		}

		void press_restart() {
			context->end();
			context->start();
		}

		void press_title() {
			context->end();
		}

	private:
		environment *	context;
	};
}