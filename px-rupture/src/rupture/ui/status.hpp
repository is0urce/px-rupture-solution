// name: status.hpp
// type: c++

#pragma once

#include "panel.hpp"

#include "rupture/environment.hpp"
#include "rupture/es/transform_component.hpp"
#include "rupture/es/body_component.hpp"

#include <imgui/imgui.h>

namespace px::ui {

	class status final
		: public panel
	{
	public:
		virtual ~status() = default;
		status(environment * game)
			: context(game)
		{
		}

	protected:
		virtual void combine_panel() override {
			if (!context) return;
			if (auto target = context->possessed()) {
				if (auto body = target->linked<body_component>()) {

					const float screen_width = ImGui::GetIO().DisplaySize.x;
					const float screen_height = ImGui::GetIO().DisplaySize.y;
					const float window_width = 300.0f;
					const float window_height = 125.0f;
					const float window_padding_x = 16;
					const float window_padding_y = 65;

					ImGui::SetNextWindowPos({ window_padding_x, screen_height - window_height - window_padding_y }, ImGuiCond_Always);
					ImGui::SetNextWindowSize({ window_width, window_height });

					ImGui::Begin("##status_inspector", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);

					ImGui::Text(body->name().c_str());

					bar(body->health(), "hp", { 0.5f, 0.0f, 0.0f, 1.0f });
					bar(body->energy(), "mp", { 0.0f, 0.0f, 0.5f, 1.0f });

					ImGui::End();
				}
			}
		}

	private:
		// draw resource bar
		template <typename ResourceOption>
		bool bar(ResourceOption const& resource, std::string const& prefix, ImVec4 const& tint) {
			if (!resource) return false;
			auto current = resource->current();
			auto maximum = resource->maximum();
			float fraction = (maximum == 0) ? 0.0f : current / static_cast<float>(maximum);
			ImGui::Text(prefix.c_str());
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_PlotHistogram, tint);
			ImGui::ProgressBar(fraction, { -1, 0 }, (std::to_string(current) + "/" + std::to_string(maximum)).c_str());
			ImGui::PopStyleColor(1);
			return true;
		}

	private:
		environment * context;
	};
}