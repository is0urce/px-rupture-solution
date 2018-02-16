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

			bool dead = !body->is_alive();

			const float screen_width = ImGui::GetIO().DisplaySize.x;
			const float screen_height = ImGui::GetIO().DisplaySize.y;
			//const float window_width = 200.0f;
			const float window_height = screen_height / 2;
			const float padding_x = ImGui::GetStyle().FramePadding.x * 2; // multiplier from pixel size multiplication
			const float padding_y = ImGui::GetStyle().FramePadding.y * 2;

			ImGui::SetNextWindowPos({ screen_width / 2 - window_width / 2 - padding_x, screen_height / 4 - padding_y }, ImGuiCond_Always);
			ImGui::SetNextWindowSize({ window_width + padding_x * 2, window_height + padding_y * 2 });
			ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 1.0f });
			ImGui::Begin("##results_content", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);

			if (dead) {
				print("Game Over");
				ImGui::NewLine();
				print("You have died.");
				ImGui::NewLine();
				if (button("Load")) press_load();
				if (button("Restart")) press_restart();
			}
			if (!dead) {
				print("Victory!");
				ImGui::NewLine();
				print("You have won.");
				ImGui::NewLine();
				if (button("Continue")) press_resume(*character);
				if (button("Titles")) press_title();
			}

			ImGui::End();
			ImGui::PopStyleColor(1); // window style
		}

	private:
		void print(std::string const& text) {
			ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.0f, 0.0f, 0.0f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.0f, 0.0f, 0.0f, 1.0f });
			ImGui::Button(text.c_str(), { window_width, 0.0f });
			ImGui::PopStyleColor(3);
		}
		bool button(std::string const& name) {
			bool result;
			ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.5f, 0.1f, 0.0f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 1.0f, 0.3f, 0.0f, 1.0f });
			result = ImGui::Button(name.c_str(), { window_width, 0.0f });
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
		const float		window_width = 200.0f;
	};
}