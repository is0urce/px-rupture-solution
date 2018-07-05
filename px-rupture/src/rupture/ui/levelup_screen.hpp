// name: levelup.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "panel.hpp"

#include "design.hpp"
#include "immediate.hpp"

#include "../environment.hpp"
#include "../es/transform_component.hpp"
#include "../es/body_component.hpp"
#include "../es/character_component.hpp"

#include "../rl/traits.hpp"

namespace px::ui {

    class levelup_screen final
        : public panel
    {
    public:
        virtual ~levelup_screen() noexcept override = default;

        levelup_screen(environment * ctx) noexcept
            : context(ctx) {
        }

    protected:
        virtual void combine_panel() override {
            auto pawn = context ? context->controlled() : nullptr;
            auto body = pawn ? pawn->linked<body_component>() : nullptr;

            if (body && features.is_levelup(*body)) {
                ImVec2 const screen_size = ImGui::GetIO().DisplaySize;
                ImVec2 const padding(16, 65);
                ImVec2 const window_size(screen_size.x / 3 - padding.x * 3, screen_size.y * 2 / 3 - padding.y);

                auto const traits = features.select_traits(*body);
                switch (traits.size()) {
                    case 3:
                        combine_option({ padding.x * (1 + 2 * 0) + window_size.x * 0, screen_size.y / 4 }, window_size, *body, 0, traits[0]);
                        combine_option({ padding.x * (1 + 2 * 1) + window_size.x * 1, screen_size.y / 4 }, window_size, *body, 1, traits[1]);
                        combine_option({ padding.x * (1 + 2 * 2) + window_size.x * 2, screen_size.y / 4 }, window_size, *body, 2, traits[2]);
                        break;
                    case 2:
                        combine_option({ padding.x * (1 + 2 * 0) + window_size.x * 0, screen_size.y / 4 }, window_size, *body, 0, traits[0]);
                        combine_option({ padding.x * (1 + 2 * 1) + window_size.x * 1, screen_size.y / 4 }, window_size, *body, 1, traits[1]);
                        break;
                    case 1:
                        combine_option({ padding.x * (1 + 2 * 0) + window_size.x * 0, screen_size.y / 4 }, window_size, *body, 0, traits[0]);
                        break;
                    default:
                        levelup(*body, 0);
                        break;
                }
            }
        }

    private:
        void combine_option(ImVec2 const& pos, ImVec2 const& window_size, body_component & body, unsigned int index, unsigned int trait_id) {
            if (auto perk = features[trait_id]) {
                immediate::style_color bg(ImGuiCol_WindowBg, { 0.25f, 0.25f, 0.25f, 1.0f });
                ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
                ImGui::SetNextWindowSize(window_size);
                ImGui::Begin(("trait_card##idx" + std::to_string(index)).c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);

                ImGui::Text(perk->label.c_str());
                ImGui::TextWrapped(perk->description.c_str());

                if (immediate::line("Take##take_trait" + std::to_string(trait_id), window_size.x, design::button_idle_color(), design::button_hover_color(), design::button_active_color())) {
                    levelup(body, trait_id);
                }

                ImGui::End();
            }
        }

        void levelup(body_component & body, unsigned int trait_id) {
            features.give_trait(trait_id, body);
            body.mod_level(1);
        }

    private:
        traits          features;
        environment *   context;
    };
}