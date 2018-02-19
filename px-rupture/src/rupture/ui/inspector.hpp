// name: inspector.hpp
// type: c++
// auth: is0urce
// desc: ui inspectior panel class

#pragma once

#include "panel.hpp"
#include "immediate.hpp"

#include "../environment.hpp"
#include "../es/transform_component.hpp"
#include "../es/body_component.hpp"

namespace px::ui {

	class inspector final
		: public panel
	{
	public:
		virtual ~inspector() = default;
		inspector(environment * env)
			: game(env)
		{
		}

	protected:
		virtual void combine_panel() override {
			if (!game) return;

			if (auto target = game->target()) {
				if (auto body = target->linked<body_component>()) {

					const float screen_width = ImGui::GetIO().DisplaySize.x;
					const float screen_height = ImGui::GetIO().DisplaySize.y;
					const float window_width = 300.0f;
					const float window_height = 125.0f;
					const float window_padding_x = 16;
					const float window_padding_y = 65;
					ImGui::SetNextWindowPos({ screen_width - window_width - window_padding_x, screen_height - window_height - window_padding_y }, ImGuiCond_Always);
					ImGui::SetNextWindowSize({ window_width, window_height });
					ImGui::Begin("##target_inspector", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);

					ImGui::Text(body->name().c_str());
					immediate::bar(body->health(), "hp", { 0.5f, 0.0f, 0.0f, 1.0f });
					immediate::bar(body->energy(), "mp", { 0.0f, 0.0f, 0.5f, 1.0f });

					if (body->is_useable()) {
						ImGui::Text("useable [e]");
					}

					ImGui::End();
				}
			}
		}

	private:
		environment * game;
	};
}