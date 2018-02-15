// name: performance.hpp
// type: c++

#pragma once

#include "panel.hpp"

#include <imgui/imgui.h>

namespace px::ui {

	class performance final
		: public panel
	{
	public:
		virtual ~performance() = default;

		performance()
			: metrics(false)
			, dbg(false)
		{
		}

	protected:
		virtual void combine_panel() override {

			ImGui::SetNextWindowSize({ 150, 100 });
			ImGui::SetNextWindowPos({ 50, 50 });
			ImGui::Begin("performance", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
			if (ImGui::Button("metrics")) {
				metrics = !metrics;
			}
			if (ImGui::Button("test")) {
				dbg = !dbg;
			}
			ImGui::SetWindowFontScale(0.5f);
			ImGui::End();

			if (metrics) {
				ImGui::ShowMetricsWindow(&metrics);
			}
			if (dbg) {
				ImGui::ShowTestWindow();
			}
		}

	private:
		bool metrics;
		bool dbg;
	};
}