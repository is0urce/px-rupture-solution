// name: craft_station.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "panel.hpp"

#include "immediate.hpp"
#include "item_names.hpp"

#include "../environment.hpp"
#include "../es/container_component.hpp"
#include "../rl/craft.hpp"

#include <px/rl/craft_activity.hpp>
#include <px/rl/craft_task.hpp>

#include <string>
#include <vector>

namespace px {

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

        craft_station(environment * ctx)
            : game(ctx)
            , container(nullptr)
            , inventory_select(-1) {
            generator.assign_environment(ctx);
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
                    if (immediate::line((ptr->name() + "##craft_slot_n_" + std::to_string(idx)).c_str(), width, { 0.5f, 0.5f, 0.5f, 1.0f })) {
                        if (container) {
                            container->acquire(task.remove(idx));
                            if (game) {
                                game->play_sound(settings::sound_path + std::string("snd_ui_select.wav"), 1.0f);
                            }
                        }
                    }
                }
                else {
                    immediate::line("-- empty --", width, { 0.3f, 0.3f, 0.3f, 1.0f });
                }
            }
        }

        void combine_inventory(ImVec2 const& window_position, ImVec2 const& window_size) {
            static auto const filter = [&](rl::item const& item) {
                return item.has_effect(rl::effect::ingredient_power, static_cast<rl::item::enhancement_type::integer_type>(Activity));
            };

            if (container) {
                item_names::format(*container, inventory_names, filter);

                immediate::style_color bg_transparent(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
                ImGui::SetNextWindowPos(window_position, ImGuiCond_Always);
                ImGui::SetNextWindowSize(window_size);
                ImGui::Begin("##inventory_craft_station", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
                immediate::line("inventory##craft_inventory_title", window_size.x, design::panel_title_color());

                ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
                if (ImGui::ListBox("##craft_inventory_list", &inventory_select, item_names::getter, static_cast<void*>(&inventory_names), static_cast<int>(inventory_names.size()), 16)) {
                    size_t current_idx = 0;
                    size_t absolute_idx = 0;    // index in container
                    size_t relative_idx = 0;    // index on display
                    container->enumerate([&](auto const& item) {
                        if (filter(item)) {
                            if (relative_idx == inventory_select) {
                                absolute_idx = current_idx;
                            }
                            ++relative_idx;
                        }
                        ++current_idx;
                    });

                    // add new reagent in mix if there is free slot
                    if (!task.is_complete()) {
                        if (auto item = container->unacquire(absolute_idx, 1)) {
                            task.add(std::move(item));
                            if (game) {
                                game->play_sound(settings::sound_path + std::string("snd_ui_select.wav"), 1.0f);
                            }
                        }
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
        int                             inventory_select;
    };
}