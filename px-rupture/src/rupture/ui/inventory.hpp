// name: inventory.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "panel.hpp"

#include "design.hpp"
#include "immediate.hpp"
#include "item_names.hpp"
#include "field_description.hpp"

#include "../environment.hpp"
#include "../es/transform_component.hpp"
#include "../es/body_component.hpp"
#include "../es/container_component.hpp"

#include <array>
#include <string>
#include <tuple>
#include <vector>

namespace px {

    class inventory final
        : public panel
    {
    public:
        virtual ~inventory() override = default;

        inventory(environment * env, bool * p_open)
            : game(env)
            , opened(p_open)
            , selected_slot(nullptr)
            , selected(-1) {
        }

    protected:
        virtual void combine_panel() override {
            if (!game || !(*opened)) return;

            transform_component * target = game->controlled();
            if (!target) return;
            auto[body, container] = target->unwind<body_component, container_component>();
            if (!container) return;

            float const screen_width = ImGui::GetIO().DisplaySize.x;
            float const screen_height = ImGui::GetIO().DisplaySize.y;
            float const window_width = 350.0f;
            float const window_height = 538.0f;
            ImVec2 const inventory_size(window_width, window_height);
            ImVec2 const slot_size(200, 32);
            ImVec2 const inventory_position(screen_width / 2 - window_width / 2, screen_height / 2 - window_height / 2);
            ImVec2 const slot_position(inventory_position.x - slot_size.x - 32, inventory_position.y + slot_size.y);
            ImVec2 const inspector_position(inventory_position.x + inventory_size.x + 32, slot_position.y);

            // inventory list

            combine_list(inventory_position, inventory_size, *body, *container);

            // equipment slots

            selected_slot = nullptr; // reset hovered equipment slot
            unsigned int i = 0;
            for (auto const& examined_slot : std::array<std::tuple<std::string, rl::equipment>, 3>{ {
                { "weapon", rl::equipment::hand },
                { "helmet", rl::equipment::head },
                { "armor", rl::equipment::chest } } }) {
                combine_slot(std::get<0>(examined_slot), { slot_position.x, slot_position.y + (slot_size.y + 8) * i }, slot_size, *body, std::get<1>(examined_slot));
                ++i;
            }

            // item inspector

            rl::item const* inspect_item = nullptr;
            if (selected >= 0) inspect_item = container->get(selected);
            if (selected_slot) inspect_item = selected_slot;
            if (inspect_item) {
                combine_inspector(*inspect_item, inspector_position);
            }
        }

    private:
        // inventory list window draw
        void combine_list(ImVec2 const& position, ImVec2 const& size, body_component & body, container_component & container) {
            ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
            ImGui::PushStyleColor(ImGuiCol_FrameBg, { 1.0f, 1.0f, 1.0f, 0.25f });
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, { 1.0f, 1.0f, 1.0f, 0.25f });
            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, { 1.0f, 1.0f, 1.0f, 0.25f });
            ImGui::SetNextWindowPos(position, ImGuiCond_Always);
            ImGui::SetNextWindowSize(size);
            ImGui::Begin("##inventory_panel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            immediate::line(body.name() + " inventory", size.x, design::panel_title_color());

            ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
            item_names::format(container, names);
            selected = -1;
            if (ImGui::ListBox("##inventory_list", &selected, item_names::getter, static_cast<void*>(&names), static_cast<int>(names.size()), 15)) {
                if (selected >= 0) {
                    if (auto ptr = container.get(selected)) {

                        // equipment
                        if (ptr->has_effect<rl::effect::equipment>()) {
                            if (game->has_control()) {
                                game->start_turn();
                                body.equip(selected);
                                game->end_turn(1);
                            }
                        }

                        // useable
                        if (ptr->has_effect<rl::effect::useable>()) {
                            auto useable = ptr->find({ rl::effect::useable });
                            if (game->has_control()) {
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

            if (immediate::line("close##close_inventory", size.x, design::button_idle_color(), design::button_hover_color(), design::button_active_color())) {
                *opened = false;
            }

            ImGui::End();
            ImGui::PopStyleColor(4);
        }

        // equipment slots drawing
        void combine_slot(std::string const& name, ImVec2 const& position, ImVec2 const& size, body_component & body, rl::equipment slot) {
            ImGui::SetNextWindowPos(position, ImGuiCond_Always);
            ImGui::SetNextWindowSize(size);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
            ImGui::Begin((name + "##equipment_slot_" + std::to_string(static_cast<long long>(slot))).c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

            if (rl::item * ptr = body.equipment(slot)) {
                ImGui::PushID(ptr);
                ImGui::PushStyleColor(ImGuiCol_Button, { 0.0, 0.0, 0.0, 1 });
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.0, 0.0, 0.0, 1 });
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.0, 0.0, 0.0, 1 });
                if (ImGui::Button(ptr->name().c_str(), size)) {
                    body.unequip(slot);
                }
                else if (ImGui::IsItemHovered()) {
                    selected_slot = ptr;
                }
                ImGui::PopStyleColor(3);
                ImGui::PopID();
            }
            else {
                ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 1 });
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.0, 0.0, 0.0, 1 });
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.0, 0.0, 0.0, 1 });
                ImGui::Button((name + ": no").c_str(), size);
                ImGui::PopStyleColor(3);
            }

            ImGui::End();
            ImGui::PopStyleVar(2);
        }

        // item inspector window draw
        void combine_inspector(rl::item const& item, ImVec2 const& position) {
            immediate::style_color bg_transparent(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
            ImGui::SetNextWindowPos(position, ImGuiCond_Always);
            ImGui::Begin((item.name() + "##item_inspector_title").c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

            field_description::display_item(item);

            ImGui::End();
        }

    private:
        std::vector<std::string>    names;
        bool *                      opened;
        rl::item *                  selected_slot;  // hovered item is equipment slot
        int                         selected;       // hovered item in container list
        environment *               game;           // context
    };
}