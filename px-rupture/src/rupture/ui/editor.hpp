// name: editor.hpp
// type: c++

#pragma once

#include "panel.hpp"

#include <imgui/imgui.h>

namespace px::ui {
	class editor : public panel
	{
	protected:
		virtual void combine_panel() override
		{
			ImGui::SetNextWindowSize({ 150, 100 });
			ImGui::SetNextWindowPos({ 50, 50 });
			ImGui::Begin("performance", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

			static bool metrics = false;
			if (ImGui::Button("dear imgui metrics")) {
				metrics = !metrics;
			}
			ImGui::End();

			if (metrics) {
				ImGui::ShowMetricsWindow(&metrics);
			}
		}
	};
}