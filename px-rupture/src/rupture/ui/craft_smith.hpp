// name: craft_smith.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "craft_station.hpp"

#include "../es/transform_component.hpp"
#include "../es/body_component.hpp"
#include "../es/container_component.hpp"

namespace px {

    class craft_smith final
        : public craft_station<rl::craft_activity::blacksmith>
    {
    public:
        bool cancel_smith() {
            release_items();
            task.close();
            recipe_current = nullptr;
            return game->close_workshop();
        }

        bool can_execute() const {
            return game && container && task.is_complete() && recipe_current;
        }

    public:
        virtual ~craft_smith() override = default;

        craft_smith(environment * context)
            : craft_station(context)
            , recipe_current(nullptr) {
            fill_recipes();
        }

    protected:
        virtual void combine_panel() override {
            if (!game || !game->has_access(rl::craft_activity::blacksmith)) return;
            container = game->controlled()->qlink<container_component, body_component>();
            if (!container) return;

            const float screen_width = ImGui::GetIO().DisplaySize.x;
            const float screen_height = ImGui::GetIO().DisplaySize.y;
            const float window_width = 350.0f;
            const float window_height = 538.0f;

            ImVec2 craft_position{ screen_width / 2 - window_width / 2, screen_height / 2 - window_height / 2 };
            ImVec2 recipes_position{ craft_position.x - 64 - window_width, craft_position.y };
            ImVec2 inventory_position{ craft_position.x + 64 + window_width, craft_position.y };

            combine_recipes(recipes_position, { window_width, window_height });
            combine_slots(craft_position, { window_width, window_height });
            combine_inventory(inventory_position, { window_width, window_height });
        }

        virtual uq_ptr<rl::item> execute() override {
            uq_ptr<rl::item> result;
            if (can_execute()) {
                result = generator.create(*recipe_current, task);
                consume_items();
                recipe_current = nullptr;
            }
            return result;
        }

    private:
        void execute_smith() {
            if (auto item = execute()) {
                game->close_workshop();
                game->popup("+ " + item->name(), { 1, 1, 1 });
                container->acquire(std::move(item));
                game->end_turn(1);
            }
        }

        void fill_recipes() {
            recipes.push_back({ "axe", "i_axe", "", rl::item_category::weapon, rl::craft_activity::blacksmith, rl::equipment::hand, 4, 1.2, 0.8 });
            recipes.push_back({ "dagger", "i_sword", "", rl::item_category::weapon, rl::craft_activity::blacksmith, rl::equipment::hand, 2, 0.7, 1.1 });
            recipes.push_back({ "spear", "i_spear", "", rl::item_category::weapon, rl::craft_activity::blacksmith, rl::equipment::hand, 3, 1.0, 0.7 });
            recipes.push_back({ "sword", "i_sword", "", rl::item_category::weapon, rl::craft_activity::blacksmith, rl::equipment::hand, 4, 1.0, 1.0 });
            recipes.push_back({ "breastplate", "i_breastplate", "", rl::item_category::armor, rl::craft_activity::blacksmith, rl::equipment::chest, 6, 1.0, 1.0 });
            recipes.push_back({ "helmet", "i_helmet", "", rl::item_category::armor, rl::craft_activity::blacksmith, rl::equipment::head, 4, 1.0, 1.0 });
        }

        void select_recipe(size_t recipe_idx) {
            release_items();
            task.close();
            if (recipe_idx < recipes.size()) {
                recipe_current = &recipes[recipe_idx];
                task.reset(recipe_current->reagent_count);
            }
        }

        void combine_recipes(ImVec2 const& window_position, ImVec2 const& window_size) {
            ImGui::SetNextWindowPos(window_position, ImGuiCond_Always);
            ImGui::SetNextWindowSize(window_size);
            ImGui::Begin("recipes##recipes_panel", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

            ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
            static int recipe_select = -1;
            if (ImGui::ListBox("##recipes_list", &recipe_select, &recipe_name_getter, static_cast<void*>(&recipes), static_cast<int>(recipes.size()), 16)) {
                if (recipe_select >= 0) {
                    select_recipe(recipe_select);
                }
            }
            ImGui::PopItemWidth();

            ImGui::End();
        }

        void combine_slots(ImVec2 const& window_position, ImVec2 const& window_size) {
            ImGui::SetNextWindowPos(window_position, ImGuiCond_Always);
            ImGui::SetNextWindowSize(window_size);
            ImGui::Begin("blacksmith##craft_panel", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

            ImGui::BeginGroup();
            ImGui::BeginChild("slots view", ImVec2(0, -2 * ImGui::GetItemsLineHeightWithSpacing())); // Leave room for 1 line below us
            if (recipe_current) {
                combine_reagents();
            }
            else {
                ImGui::Text("select recipe");
            }
            ImGui::EndChild();
            ImGui::BeginChild("buttons");
            if (ImGui::Button("smith", { 334, 32 })) {
                execute_smith();
            }
            if (ImGui::Button("close", { 334, 32 })) {
                cancel_smith();
            }
            ImGui::EndChild();
            ImGui::EndGroup();

            ImGui::End();
        }

        static bool recipe_name_getter(void * data, int n, const char** result) {
            auto const& vector = *static_cast<std::vector<rl::craft_recipe>*>(data);
            if (n >= 0 && n < static_cast<int>(vector.size())) {
                *result = vector[n].name.c_str();
                return true;
            }
            return false;
        }

    private:
        std::vector<rl::craft_recipe>   recipes;            // recipe list
        rl::craft_recipe const*         recipe_current;     // selected recipe
    };
}