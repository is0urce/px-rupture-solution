// name: inventory.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "panel.hpp"

#include "../environment.hpp"
#include "../es/transform_component.hpp"
#include "../es/body_component.hpp"
#include "../es/container_component.hpp"

#include <imgui/imgui.h>

#include <string>
#include <vector>

namespace px {

    class inventory final
        : public panel
    {
    public:
        virtual ~inventory() = default;
        inventory(environment * env, bool * p_open)
            : game(env)
            , opened(p_open)
            , selected_slot(nullptr)
            , selected(-1)
        {
        }

    protected:
        virtual void combine_panel() override {
            if (!game || !(*opened)) return;

            transform_component * target = game->possessed();
            if (!target) return;
            auto[body, container] = target->unwind<body_component, container_component>();
            if (!container) return;

            const float screen_width = ImGui::GetIO().DisplaySize.x;
            const float screen_height = ImGui::GetIO().DisplaySize.y;
            const float window_width = 350.0f;
            const float window_height = 538.0f;
            ImVec2 inventory_position{ screen_width / 2 - window_width / 2, screen_height / 2 - window_height / 2 };
            ImVec2 inventory_size{ window_width, window_height };

            ImVec2 slot_position = inventory_position;
            ImVec2 slot_size{ 200, 32 };
            slot_position.x -= slot_size.x + 32;
            slot_position.y += 64;

            ImVec2 inspector_position = inventory_position;
            inspector_position.x += inventory_size.x + 32;
            inspector_position.y += 64;

            combine_list(inventory_position, inventory_size, *body, *container);

            selected_slot = nullptr; // reset hovered equipment slot

            combine_slot("hands", slot_position, slot_size, *body, rl::equipment::hand);
            slot_position.y += slot_size.y;

            slot_position.y += 8;
            combine_slot("head", slot_position, slot_size, *body, rl::equipment::head);
            slot_position.y += slot_size.y;

            slot_position.y += 8;
            combine_slot("chest", slot_position, slot_size, *body, rl::equipment::chest);
            slot_position.y += slot_size.y;

            rl::item const* inspect = nullptr;
            if (selected >= 0) inspect = container->get(selected);
            if (selected_slot) inspect = selected_slot;

            if (inspect) {
                combine_inspector(*inspect, inspector_position);
            }
        }

    private:
        // inventory list window draw
        void combine_list(ImVec2 const& position, ImVec2 const& size, body_component & body, container_component & container) {
            ImGui::SetNextWindowPos(position, ImGuiCond_Always);
            ImGui::SetNextWindowSize(size);
            ImGui::Begin((body.name() + " inventory##inventory_panel").c_str()
                , nullptr
                , ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            format_names(container, names);
            selected = -1;
            ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
            if (ImGui::ListBox("##inventory_list", &selected, name_getter, static_cast<void*>(&names), static_cast<int>(names.size()), 15)) {
                if (selected >= 0) {
                    if (auto ptr = container.get(selected)) {

                        // equipment
                        if (ptr->has_effect(rl::effect::equipment)) {
                            game->start_turn();
                            body.equip(selected);
                            game->end_turn(1);
                        }

                        // useable
                        if (ptr->has_effect(rl::effect::useable)) {
                            auto useable = ptr->find({ rl::effect::useable });
                            if (useable.sub == 0x00) {
                                game->start_turn();
                                body.use_potion(*ptr, game);
                                container.unacquire(selected, 1);
                                game->end_turn(1);
                            }
                        }
                    }
                }
                selected = -1;
            }
            ImGui::PopItemWidth();

            if (ImGui::Button("close##close_inventory", { 334, 32 })) {
                *opened = false;
            }

            ImGui::End();
        }

        // equipment slots drawing
        void combine_slot(std::string const& name, ImVec2 const& position, ImVec2 const& size, body_component & body, rl::equipment slot) {
            ImGui::SetNextWindowPos(position, ImGuiCond_Always);
            ImGui::SetNextWindowSize(size);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
            ImGui::Begin((name + "title").c_str(),
                nullptr,
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

            if (rl::item * ptr = body.equipment(slot)) {
                ImGui::PushID(ptr);
                if (ImGui::Button(ptr->name().c_str(), size)) {
                    body.unequip(slot);
                }
                else if (ImGui::IsItemHovered()) {
                    selected_slot = ptr;
                }
                ImGui::PopID();
            }
            else {
                ImGui::Button(name.c_str(), size);
            }

            ImGui::End();
            ImGui::PopStyleVar(2);
        }

        // item inspector window draw
        void combine_inspector(rl::item const& item, ImVec2 const& position) {
            ImGui::SetNextWindowPos(position, ImGuiCond_Always);
            ImGui::Begin((item.name() + "##item_inspector_title").c_str(),
                nullptr,
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

            // main props
            ImGui::Text(item.name().c_str());
            ImGui::Text(item.description().c_str());

            // generic props
            if (item.has_effect(rl::effect::equipment)) {
                ImGui::Text("Equipment");
            }
            if (item.has_effect(rl::effect::useable)) {
                ImGui::Text("Useable");
            }

            // ingredient props
            if (item.has_effect(rl::effect::ingredient_power)) {
                switch (static_cast<rl::craft_activity>(item.find_subtype(rl::effect::ingredient_power, 0))) {
                case rl::craft_activity::blacksmith:
                    ImGui::Text("Reagent: Blacksmith");
                    break;
                case rl::craft_activity::alchemy:
                    ImGui::Text("Reagent: Alchemy");
                    break;
                default:
                    ImGui::Text("Reagent");
                    break;
                }
            }

            // equipment props
            if (item.has_effect(rl::effect::damage)) {
                ImGui::Text("Damage: %.0f", item.accumulate({ rl::effect::damage }).magnitude0);
            }
            if (item.has_effect(rl::effect::armor)) {
                ImGui::Text("Armor: %.0f", item.accumulate({ rl::effect::armor }).magnitude0);
            }

            // potion props
            if (item.has_effect(rl::effect::hp_bonus)) {
                ImGui::Text("Heal: %.0f", item.accumulate({ rl::effect::hp_bonus }).magnitude0);
            }
            if (item.has_effect(rl::effect::mp_bonus)) {
                ImGui::Text("Innervate: %.0f", item.accumulate({ rl::effect::mp_bonus }).magnitude0);
            }
            if (item.has_effect(rl::effect::hp_regen)) {
                auto effect = item.accumulate({ rl::effect::hp_regen });
                ImGui::Text("Regenerate: %.0f for %d", effect.magnitude0, effect.value0);
            }
            if (item.has_effect(rl::effect::mp_regen)) {
                auto effect = item.accumulate({ rl::effect::mp_regen });
                ImGui::Text("Invigorate: %.0f for %d", effect.magnitude0, effect.value0);
            }

            // item status
            ImGui::Text("Quantity: %d/%d", item.count(), item.maximum());

            ImGui::End();
        }

    private:
        std::vector<std::string>    names;
        bool *                      opened;
        int                         selected;       // hovered item in container list
        rl::item *                  selected_slot;  // hovered item is equipment slot
        environment *               game;           // context
    };
}