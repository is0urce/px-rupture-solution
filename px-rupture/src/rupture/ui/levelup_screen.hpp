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

#include <cmath>

namespace px::ui {

    class levelup_screen final
        : public panel
    {
    public:
        virtual ~levelup_screen() noexcept override = default;

        levelup_screen(environment * ctx) noexcept
            : context(ctx)
            , body(nullptr) {
        }

    protected:
        virtual void combine_panel() override {
            if (context && context->is_ingame()) {
                auto pawn = context->controlled();
                body = pawn ? pawn->linked<body_component>() : nullptr;

                bool const starting = !pawn;
                bool const leveling = body && features.is_levelup(*body);
                if (starting || leveling) {

                    std::string txt_title;
                    std::string txt_info;
                    std::vector<unsigned int> traits;
                    if (starting) {
                        txt_title = "Pick your class!";
                        txt_info = "select one of the options below";
                        traits = { 1, 2, 3 };
                    }
                    if (leveling) {
                        txt_title = "Level Up!";
                        txt_info = "select one of te bonus perks";
                        traits = features.select_traits(*body);
                    }

                    ImVec2 const screen_size = ImGui::GetIO().DisplaySize;
                    ImVec2 const gz_position(0, 50);
                    ImVec2 const gz_size(screen_size.x, 75);
                    ImVec2 const cards_offset(gz_position.x, gz_position.y + gz_size.y);
                    ImVec2 const cards_size(screen_size.x - cards_offset.x, screen_size.y - cards_offset.y);

                    ImVec2 const card_padding(16, 16);
                    ImVec2 const card_size(cards_size.x * 0.67f - card_padding.x * 2, cards_size.y * 0.33f - card_padding.y * 2);
                    ImVec2 const card_position(cards_offset.x + cards_size.x * 0.17f + card_padding.x, cards_offset.y + card_padding.y);
                    ImVec2 const card_stride(0, card_size.y + card_padding.y); // vertical

                    ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
                    ImGui::SetNextWindowPos(gz_position, ImGuiCond_Always);
                    ImGui::SetNextWindowSize(gz_size);
                    ImGui::Begin("##levelup_message_panel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
                    immediate::line(txt_title, gz_size.x, { 0.0f, 0.0f, 0.0f, 0.0f });
                    immediate::line(txt_info, gz_size.x, { 0.0f, 0.0f, 0.0f, 0.0f });
                    ImGui::End();
                    ImGui::PopStyleColor(1);

                    switch (traits.size()) {
                    case 3:
                        combine_option({ card_position.x + card_stride.x * 0.0f, card_position.y + card_stride.y * 0.0f }, card_size, 0, traits[0]);
                        combine_option({ card_position.x + card_stride.x * 1.0f, card_position.y + card_stride.y * 1.0f }, card_size, 1, traits[1]);
                        combine_option({ card_position.x + card_stride.x * 2.0f, card_position.y + card_stride.y * 2.0f }, card_size, 2, traits[2]);
                        break;
                    case 2:
                        combine_option({ card_position.x + card_stride.x * 0.5f, card_position.y + card_stride.y * 0.5f }, card_size, 0, traits[0]);
                        combine_option({ card_position.x + card_stride.x * 1.5f, card_position.y + card_stride.y * 1.5f }, card_size, 1, traits[1]);
                        break;
                    case 1:
                        combine_option({ card_position.x + card_stride.x * 1.0f, card_position.y + card_stride.y * 1.0f }, card_size, 0, traits[0]);
                        break;
                    default:
                        levelup(0);
                        break;
                    }
                }
            }
        }

    private:
        void combine_option(ImVec2 const& pos, ImVec2 const& window_size, unsigned int display_index, unsigned int trait_id) {
            if (auto const perk = features[trait_id]) {
                immediate::style_color bg(ImGuiCol_WindowBg, design::panel_background());
                ImGui::SetNextWindowPos({ std::floor(pos.x), std::floor(pos.y) }, ImGuiCond_Always);
                ImGui::SetNextWindowSize({ std::floor(window_size.x), std::floor(window_size.y) });
                ImGui::Begin(("trait_card##idx" + std::to_string(display_index)).c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);

                ImGui::BeginGroup();
                ImGui::BeginChild("card_text", { 0.0f, -1.0f * ImGui::GetItemsLineHeightWithSpacing() }); // Leave room for 1 line below us
                immediate::line(perk->label, window_size.x, design::panel_title_color());
                ImGui::TextWrapped(perk->description.c_str());
                ImGui::EndChild();

                ImGui::BeginChild("card_buttons");
                if (immediate::line("Take##take_trait" + std::to_string(trait_id), window_size.x, design::button_idle_color(), design::button_hover_color(), design::button_active_color())) {
                    levelup(trait_id);
                }
                ImGui::EndChild();
                ImGui::EndGroup();

                ImGui::End();
            }
        }

        void levelup(unsigned int trait_id) {

            // mod stats
            switch (trait_id) {
            case 1: {
                if (context) {
                    auto pawn = context->create_player(1);
                    context->incarnate(pawn);
                    body = pawn ? pawn->linked<body_component>() : nullptr;
                }
                break;
            }
            case 2:
                if (context) {
                    auto pawn = context->create_player(2);
                    context->incarnate(pawn);
                    body = pawn ? pawn->linked<body_component>() : nullptr;
                }
                break;
            case 3:
                if (context) {
                    auto pawn = context->create_player(3);
                    context->incarnate(pawn);
                    body = pawn ? pawn->linked<body_component>() : nullptr;
                }
                break;
            default:
                break;
            }

            // add trait & mod level
            if (body) {
                features.give_trait(trait_id, *body);
                body->mod_level(1);
            }
        }

    private:
        traits              features;
        environment *       context;
        body_component *    body;
    };
}