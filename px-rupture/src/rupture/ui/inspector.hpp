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
		virtual ~inspector()
		{
		}
		inspector(environment * env)
			: game(env)
		{
		}

	protected:
		virtual void combine_panel() override
		{
			if (!game) return;

			const float screen_width = ImGui::GetIO().DisplaySize.x;
			const float screen_height = ImGui::GetIO().DisplaySize.y;
			const float window_width = 250.0f;
			const float window_height = 100.0f;
			ImGui::SetNextWindowPos({ screen_width - window_width - 22, screen_height - window_height - 22 }, ImGuiCond_Always);
			ImGui::SetNextWindowSize({ window_width, window_height });

			//ImGui::Begin("##target_inspector"
			//	, nullptr
			//	, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
			//point2 area = game->area();
			//ImGui::Text("area: %s", (std::to_string(area.x()) + "," + std::to_string(area.y())).c_str());
			//ImGui::End();

			transform_component * target = game->target();
			if (target) {
				auto body = target->linked<body_component>();

				if (body) {
					ImGui::Begin("##target_inspector"
						, nullptr
						, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);

					ImGui::Text("%s", body->name().c_str());
					ImGui::NewLine();

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

					ImGui::End();
				}
			}
		}

	private:
		environment * game;
	};
}