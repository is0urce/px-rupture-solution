// name: craft_alchemy.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "craft_station.hpp"

namespace px {

    class craft_alchemy final
        : public craft_station<rl::craft_activity::alchemy>
    {
    public:
        void cancel_alchemy() {
            release_items();
            game->close_workshop();
        }

        bool can_execute() const {
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
            if (!is_open()) return;
            container = game->possessed()->qlink<container_component, body_component>();
            if (!container) return;

            const float screen_width = ImGui::GetIO().DisplaySize.x;
            const float screen_height = ImGui::GetIO().DisplaySize.y;
            const float window_width = 350.0f;
            const float window_height = 538.0f;

            ImVec2 craft_position{ screen_width / 2 - window_width / 2, screen_height / 2 - window_height / 2 };
            ImVec2 inventory_position{ craft_position.x + 64 + window_width, craft_position.y };

            combine_slots(craft_position, { window_width, window_height });
            combine_inventory(inventory_position, { window_width, window_height });
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

        void combine_slots(ImVec2 const& window_position, ImVec2 const& window_size) {
            ImGui::SetNextWindowPos(window_position, ImGuiCond_Always);
            ImGui::SetNextWindowSize(window_size);
            ImGui::Begin("alchemy##craft_panel", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            ImGui::BeginGroup();

            ImGui::BeginChild("slots view", ImVec2(0, -2 * ImGui::GetItemsLineHeightWithSpacing())); // Leave room for 1 line below us
            combine_reagents();
            ImGui::EndChild();

            ImGui::BeginChild("buttons");
            if (ImGui::Button("brew", { 334, 32 })) {
                execute_alchemy();
            }
            if (ImGui::Button("close", { 334, 32 })) {
                cancel_alchemy();
            }
            ImGui::EndChild();
            ImGui::EndGroup();
            ImGui::End();
        }
    };
}