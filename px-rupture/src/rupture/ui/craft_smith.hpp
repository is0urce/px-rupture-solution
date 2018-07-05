// name: craft_smith.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "craft_station.hpp"

#include "immediate.hpp"
#include "design.hpp"

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
            if (container = is_open() ? acquire_container() : nullptr) {
                float const screen_width = ImGui::GetIO().DisplaySize.x;
                float const screen_height = ImGui::GetIO().DisplaySize.y;
                float const window_width = 350.0f;
                float const window_height = 538.0f;
                ImVec2 const craft_position(screen_width / 2 - window_width / 2, screen_height / 2 - window_height / 2);
                ImVec2 const recipes_position(craft_position.x - 64 - window_width, craft_position.y);
                ImVec2 const inventory_position(craft_position.x + 64 + window_width, craft_position.y);
                combine_recipes(recipes_position, { window_width, window_height });
                combine_slots(craft_position, { window_width, window_height });
                combine_inventory(inventory_position, { window_width, window_height });
            }
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
            immediate::style_color bg_transparent(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
            ImGui::SetNextWindowPos(window_position, ImGuiCond_Always);
            ImGui::SetNextWindowSize(window_size);
            ImGui::Begin("##blacksmith_recipes_panel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            immediate::line("recipes##blacksmith_recipes_title", window_size.x, design::panel_title_color());

            ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
            int recipe_select = -1;
            if (ImGui::ListBox("##recipes_list", &recipe_select, &recipe_name_getter, static_cast<void*>(&recipes), static_cast<int>(recipes.size()), 16)) {
                if (recipe_select >= 0) {
                    select_recipe(recipe_select);
                }
            }
            ImGui::PopItemWidth();

            ImGui::End();
        }

        void combine_slots(ImVec2 const& window_position, ImVec2 const& window_size) {
            immediate::style_color bg_color(ImGuiCol_WindowBg, design::panel_background());
            ImGui::SetNextWindowPos(window_position, ImGuiCond_Always);
            ImGui::SetNextWindowSize(window_size);
            ImGui::Begin("##blacksmith_craft_panel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            immediate::line("blacksmith##blacksmith_title", window_size.x, design::panel_title_color());

            ImGui::BeginGroup();
            ImGui::BeginChild("blacksmith_slots", { 0.0f, -2.0f * ImGui::GetItemsLineHeightWithSpacing() }); // Leave room for 1 line below us
            if (recipe_current) {
                immediate::line("recipe: " + recipe_current->name, window_size.x, { 0.0f, 0.0f, 0.0f, 0.0f });
                combine_reagents(window_size.x);
                ImGui::NewLine();
                ImGui::TextWrapped("fill all the slots with preferred reagents from the inventory on the right and press 'smith'");
            }
            else {
                ImGui::TextWrapped("select a recipe from the left panel, than fill slots with reagents from the inventory on the right");
            }
            ImGui::EndChild();
            ImGui::BeginChild("blacksmith_buttons");
            if (can_execute()) {
                if (immediate::line("smith##blacksmith_execute", window_size.x, design::button_idle_color(), design::button_hover_color(), design::button_active_color())) {
                    execute_smith();
                }
            }
            else {
                immediate::line("smith##blacksmith_execute", window_size.x, design::button_disabled_color(), design::button_disabled_color(), design::button_disabled_color());
            }
            if (immediate::line("cancel##blacksmith_close", window_size.x, design::button_idle_color(), design::button_hover_color(), design::button_active_color())) {
                cancel_smith();
            }

            ImGui::EndChild();
            ImGui::EndGroup();

            ImGui::End();
        }

        static bool recipe_name_getter(void * data, int n, char const* * result) {
            auto const& vector = *static_cast<std::vector<rl::craft_recipe> const*>(data);
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