// name: craft_alchemy.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "craft_station.hpp"

#include "design.hpp"
#include "immediate.hpp"

namespace px {

    class craft_alchemy final
        : public craft_station<rl::craft_activity::alchemy>
    {
    public:
        bool cancel_alchemy() {
            release_items();
            return game->close_workshop();
        }

        bool can_execute() const noexcept {
            return game && container && task.is_complete();
        }

    public:
        virtual ~craft_alchemy() override = default;

        craft_alchemy(environment * context)
            : craft_station(context) {
            reset_recipe();
        }

    protected:
        virtual void combine_panel() override {
            if (container = is_open() ? acquire_container() : nullptr) {
                float const screen_width = ImGui::GetIO().DisplaySize.x;
                float const screen_height = ImGui::GetIO().DisplaySize.y;
                float const window_width = 350.0f;
                float const window_height = 538.0f;

                ImVec2 const craft_position(screen_width / 2 - window_width / 2, screen_height / 2 - window_height / 2);
                ImVec2 const inventory_position(craft_position.x + 64 + window_width, craft_position.y);

                combine_slots(craft_position, { window_width, window_height });
                combine_inventory(inventory_position, { window_width, window_height });
            }
        }

        virtual uq_ptr<rl::item> execute() override {
            uq_ptr<rl::item> result;
            if (can_execute()) {
                result = generator.create_potion(task);
                consume_items();
                reset_recipe();
            }
            return result;
        }

    private:
        void reset_recipe() {
            task.reset(3); // three reagent recipes
        }

        void execute_alchemy() {
            if (auto item = execute()) {
                game->close_workshop();
                game->popup("+ " + item->name(), { 1, 1, 1 });
                container->acquire(std::move(item));
                game->end_turn(1);
            }
        }

        void combine_slots(ImVec2 const& window_position, ImVec2 const& size) {
            immediate::style_color bg_color(ImGuiCol_WindowBg, design::panel_background());
            ImGui::SetNextWindowPos(window_position, ImGuiCond_Always);
            ImGui::SetNextWindowSize(size);
            ImGui::Begin("alchemy##alchemy_craft_panel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            immediate::line("alchemy##alchemy_title", size.x, design::panel_title_color());

            ImGui::BeginGroup();

            ImGui::BeginChild("alchemy_slots", { 0.0f, -2.0f * ImGui::GetItemsLineHeightWithSpacing() }); // leave room for 1 line below
            immediate::line("recipe: potion", size.x, { 0.0f, 0.0f, 0.0f, 0.0f });
            combine_reagents(size.x);
            ImGui::NewLine();
            ImGui::TextWrapped("fill all the slots with preferred reagents from the inventory on the right and press 'brew'");
            ImGui::EndChild();

            ImGui::BeginChild("alchemy_buttons");
            if (can_execute()) {
                if (immediate::line("brew##alchemy_execute", size.x, design::button_idle_color(), design::button_hover_color(), design::button_active_color())) {
                    execute_alchemy();
                }
            }
            else {
                immediate::line("brew##alchemy_execute", size.x, design::button_disabled_color(), design::button_disabled_color(), design::button_disabled_color());
            }
            if (immediate::line("close##alchemy_close", size.x, design::button_idle_color(), design::button_hover_color(), design::button_active_color())) {
                cancel_alchemy();
            }
            ImGui::EndChild();
            ImGui::EndGroup();

            ImGui::End();
        }
    };
}