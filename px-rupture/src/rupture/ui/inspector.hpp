// name: performance.hpp
// type: c++

#pragma once

#include "panel.hpp"

#include "rupture/environment.hpp"

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
			ImGui::SetNextWindowPos({ screen_width - window_width - 64, screen_height - window_height - 64 }, ImGuiCond_Always);
			ImGui::SetNextWindowSize({ window_width, window_height });

			ImGui::Begin("##target_inspector"
				, nullptr
				, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
			
			
			ImGui::Text("Goblin");

			ImGui::PushStyleColor(ImGuiCol_PlotHistogram, { 0.5, 0, 0, 1 });
			ImGui::ProgressBar(0.11f, { -1, 0 }, "10 / 1062%");
			ImGui::PopStyleColor(1);

			ImGui::PushStyleColor(ImGuiCol_PlotHistogram, { 0, 0.5, 0, 1 });
			ImGui::ProgressBar(0.95f, { -1, 0 }, "33 / 1062%");
			ImGui::PopStyleColor(1);

			ImGui::End();
		}

	private:
		environment * game;
	};
}