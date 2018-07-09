// name: screen_performance.hpp
// type: c++
// auth: is0urce
// type: class

#pragma once

// ui class for performance measure (fps) and testing

#include "../panel.hpp"

#include <imgui/imgui.h>

namespace px {

    class performance final
        : public panel
    {
    public:
        virtual ~performance() noexcept override = default;

        performance()
            : metrics(false)
            , dbg(false) {
        }

    protected:
        virtual void combine_panel() override {
            ImGui::SetNextWindowSize({ 150, 100 });
            ImGui::SetNextWindowPos({ 50, 50 });
            ImGui::Begin("performance", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
            if (ImGui::Button("metrics##performance_metrics_btn")) {
                metrics = !metrics;
            }
            if (ImGui::Button("test##performance_test_btn")) {
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