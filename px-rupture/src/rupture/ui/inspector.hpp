// name: inspector.hpp
// type: c++

#pragma once

#include "panel.hpp"

#include "rupture/environment.hpp"
#include "rupture/es/transform_component.hpp"
#include "rupture/es/body_component.hpp"

#include <imgui/imgui.h>

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
		virtual void combine_panel() override
		{
			if (!game) return;

			transform_component * target = game->target();
			if (target) {
				auto body = target->linked<body_component>();

				if (body) {

					const float screen_width = ImGui::GetIO().DisplaySize.x;
					const float screen_height = ImGui::GetIO().DisplaySize.y;
					const float window_width = 300.0f;
					const float window_height = 125.0f;
					const float window_padding_x = 16;
					const float window_padding_y = 65;
					ImGui::SetNextWindowPos({ screen_width - window_width - window_padding_x, screen_height - window_height - window_padding_y }, ImGuiCond_Always);
					ImGui::SetNextWindowSize({ window_width, window_height });

					ImGui::Begin("##target_inspector", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);

					ImGui::Text("%s", body->name().c_str());

					if (auto const& hp = body->health()) {
						auto current = hp->current();
						auto max = hp->maximum();
						float fraction = (max == 0) ? 0.0f : current / static_cast<float>(max);
						ImGui::Text("hp");
						ImGui::SameLine();
						ImGui::PushStyleColor(ImGuiCol_PlotHistogram, { 0.5f, 0, 0, 1 });
						ImGui::ProgressBar(fraction, { -1, 0 }, (std::to_string(current) + "/" + std::to_string(max)).c_str());
						ImGui::PopStyleColor(1);
					}
					//else {
					//	ImGui::NewLine();
					//}

					if (auto const& mp = body->energy()) {
						auto current = mp->current();
						auto max = mp->maximum();
						float fraction = (max == 0) ? 0.0f : current / static_cast<float>(max);
						ImGui::Text("mp");
						ImGui::SameLine();
						ImGui::PushStyleColor(ImGuiCol_PlotHistogram, { 0.0f, 0.0f, 0.5f, 1.0f });
						ImGui::ProgressBar(fraction, { -1, 0 }, (std::to_string(current) + "/" + std::to_string(max)).c_str());
						ImGui::PopStyleColor(1);
					}
					//else {
					//	ImGui::NewLine();
					//}

					if (body->is_useable()) {
						ImGui::Text("Useable ('E')");
					}

					ImGui::End();
				}
			}
		}

	private:
		environment * game;
	};
}