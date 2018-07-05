// name: craft_station.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "panel.hpp"

#include "immediate.hpp"

#include "inventory_common.hpp"

#include "../environment.hpp"
#include "../es/container_component.hpp"
#include "../rl/craft.hpp"

#include <px/rl/craft_activity.hpp>
#include <px/rl/craft_task.hpp>

#include <imgui/imgui.h>

#include <string>
#include <vector>

namespace px {

    namespace {
        bool name_getter(void * data, int n, char const** result) {
            auto const& vector = *static_cast<std::vector<std::string> const*>(data);
            if (n >= 0 && n < static_cast<int>(vector.size())) {
                *result = vector[n].c_str();
                return true;
            }
            return false;
        }
    }

    template <rl::craft_activity Activity>
    class craft_station
        : public panel
    {
    public:
        bool is_open() const noexcept {
            return game && game->is_ingame() && game->has_access(Activity);
        }

    public:
        virtual ~craft_station() override = default;

        craft_station(environment * context)
            : game(context)
            , container(nullptr) {
            generator.assign_environment(context);
        }

    protected:
        virtual uq_ptr<rl::item> execute() {
            return nullptr;
        }

    protected:
        container_component * acquire_container() {
            auto player = game ? game->controlled() : nullptr;
            return player ? player->qlink<container_component, body_component>() : nullptr;
        }

        void release_items() {
            if (container) {
                uq_ptr<rl::item> item;
                while (item = task.remove()) {
                    container->acquire(std::move(item));
                }
            }
        }

        void consume_items() {
            task.close();
        }

        void combine_reagents(float width) {
            for (size_t idx = 0, size = task.size(); idx != size; ++idx) {
                if (auto ptr = task[idx]) {
                    if (immediate::line((ptr->name() + "##craft_slot_n_" + std::to_string(idx)).c_str(), width, { 1.0f, 1.0f, 1.0f, 1.0f })) {
                        if (container) {
                            container->acquire(task.remove(idx));
                        }
                    }
                }
                else {
                    immediate::line("-- empty --", width, { 0.8f, 0.8f, 0.8f, 1.0f });
                }
            }
        }

        void combine_inventory(ImVec2 const& window_position, ImVec2 const& window_size) {
            static auto const filter = [&](rl::item const& item) {
                return item.has_effect(rl::effect::ingredient_power, static_cast<rl::item::enhancement_type::integer_type>(Activity));
            };

            if (container) {
                immediate::style_color(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
                ImGui::SetNextWindowPos(window_position, ImGuiCond_Always);
                ImGui::SetNextWindowSize(window_size);
                ImGui::Begin("inventory##inv_craft_resources", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
                ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());

                format_names(*container, inventory_names, filter);
                int selected = -1;
                if (ImGui::ListBox("##craft_inventory_list", &selected, name_getter, static_cast<void*>(&inventory_names), static_cast<int>(inventory_names.size()), 16)) {
                    size_t current_idx = 0;
                    size_t absolute_idx = 0;    // index in container
                    size_t relative_idx = 0;    // index on display
                    container->enumerate([&](auto const& item) {
                        if (filter(item)) {
                            if (relative_idx == selected) {
                                absolute_idx = current_idx;
                            }
                            ++relative_idx;
                        }
                        ++current_idx;
                    });

                    // add new reagent in mix if there is free slot
                    if (!task.is_complete()) {
                        task.add(container->unacquire(absolute_idx, 1));
                    }
                }

                ImGui::PopItemWidth();
                ImGui::End();
            }
        }

    protected:
        rl::craft_task<rl::item>        task;               // ingredient selected
        container_component *           container;          // user inventory
        environment *                   game;               // game context
        rl::craft                       generator;

    private:
        std::vector<std::string>        inventory_names;    // inventory items names cashe
    };
}