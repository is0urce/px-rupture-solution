// name: levelup.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "panel.hpp"

#include "../environment.hpp"
#include "../es/transform_component.hpp"
#include "../es/body_component.hpp"
#include "../es/character_component.hpp"

#include "../rl/traits.hpp"

#include <imgui/imgui.h>

namespace px::ui {

    class levelup final
        : public panel
    {
    public:
        virtual ~levelup() override = default;

        levelup(environment * ctx)
            : context(ctx) {
        }

    protected:
        virtual void combine_panel() override {
            auto pawn = context ? context->possessed() : nullptr;
            auto body = pawn ? pawn->linked<body_component>() : nullptr;

            if (body && features.levelup_required(*body)) {

                const float screen_width = ImGui::GetIO().DisplaySize.x;
                const float screen_height = ImGui::GetIO().DisplaySize.y;
                const float padding_x = 16;
                const float padding_y = 65;

                const float window_width = screen_width / 3 - padding_x * 3;
                const float window_height = screen_height * 2 / 3 - padding_y;

                auto[trait_1, trait_2, trait_3] = features.select_traits(*body);

                combine_option({ padding_x * (1 + 2 * 0) + window_width * 0, screen_height / 4 }, { window_width, window_height }, *body, 0, trait_1);
                combine_option({ padding_x * (1 + 2 * 1) + window_width * 1, screen_height / 4 }, { window_width, window_height }, *body, 1, trait_2);
                combine_option({ padding_x * (1 + 2 * 2) + window_width * 2, screen_height / 4 }, { window_width, window_height }, *body, 2, trait_3);
            }
        }

    private:
        void combine_option(ImVec2 const& pos, ImVec2 const& window_size, body_component & body, unsigned int index, unsigned int trait_id) {
            if (auto perk = features[trait_id]) {
                ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
                ImGui::SetNextWindowSize(window_size);
                ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.25f, 0.25f, 0.25f, 1.0f });
                ImGui::Begin(("trait_card##idx" + std::to_string(index)).c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);

                ImGui::Text(perk->label.c_str());
                ImGui::Text(perk->description.c_str());
                if (ImGui::Button("Take##take_this_trait")) {
                    increment_level(trait_id, body);
                }

                ImGui::End();
                ImGui::PopStyleColor(1); // window style
            }
        }

        void increment_level(unsigned int trait_id, body_component & body) {
            features.give_trait(trait_id, body);
            body.mod_level(1);
        }

    private:
        traits          features;
        environment *   context;
    };
}