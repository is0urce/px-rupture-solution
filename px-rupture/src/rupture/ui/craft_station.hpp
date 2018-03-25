// name: craft_station.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "panel.hpp"

#include "craft_common.hpp"

#include "../environment.hpp"
#include "../es/container_component.hpp"

#include <px/rl/craft_activity.hpp>
#include <px/rl/craft_task.hpp>

#include <imgui/imgui.h>

#include <string>
#include <vector>

namespace px {

    template <rl::craft_activity Activity>
    class craft_station
        : public panel
    {
    public:
        bool is_open() const {
            return game && game->has_access(Activity) && game->possessed();
        }

    public:
        virtual ~craft_station() = default;
        craft_station(environment * context)
            : game(context)
            , container(nullptr)
        {
        }

    protected:
        void release_items() {
            if (!container) return;

            uq_ptr<rl::item> item;
            goto query;
            while (item) {
                container->aquire(std::move(item));
            query:
                item = task.remove();
            }
            sort(*container);
        }

        void consume_items() {
            task.close();
        }

        void combine_reagents() {
            for (size_t idx = 0, size = task.size(); idx != size; ++idx) {
                if (auto item_ptr = task[idx]) {
                    ImGui::Text(item_ptr->name().c_str());
                }
                else {
                    ImGui::TextDisabled("-- empty --");
                }
            }
        }

        void combine_inventory(ImVec2 const& window_position, ImVec2 const& window_size) {
            if (!container) return;

            const auto filter = [&](rl::item const& item) {
                return item.has_effect(rl::effect::ingredient_power, static_cast<rl::item::enhancement_type::integer_type>(Activity));
            };
            format_names(*container, inventory_names, filter);
            int selected = -1;

            ImGui::SetNextWindowPos(window_position, ImGuiCond_Always);
            ImGui::SetNextWindowSize(window_size);
            ImGui::Begin("inventory##inv_craft_resources", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
            if (ImGui::ListBox("##craft_inventory_list", &selected, name_getter, static_cast<void*>(&inventory_names), static_cast<int>(inventory_names.size()), 16)) {
                size_t current_idx = 0;
                size_t absolute_idx = 0;
                size_t relative_idx = 0;
                container->enumerate([&](auto & item) {
                    if (filter(item)) {
                        if (relative_idx == selected) {
                            absolute_idx = current_idx;
                        }
                        ++relative_idx;
                    }
                    ++current_idx;
                });
                if (!task.is_complete()) {
                    task.add(container->unaquire(absolute_idx, 1));
                }
            }
            ImGui::PopItemWidth();
            ImGui::End();
        }

    protected:
        rl::craft_task<rl::item>        task;               // ingredient selected
        container_component *           container;          // user inventory
        environment *                   game;               // game context

    private:
        std::vector<std::string>        inventory_names;    // inventory items names cashe
    };
}