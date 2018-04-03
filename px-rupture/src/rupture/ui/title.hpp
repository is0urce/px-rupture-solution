// name: title.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "panel.hpp"
#include "immediate.hpp"

#include "../environment.hpp"
#include "../es/transform_component.hpp"

namespace px::ui {

    class title final
        : public panel
    {
    public:
        virtual ~title() override = default;
        title(environment * game)
            : context(game)
        {
        }

    protected:
        virtual void combine_panel() override {
            if (!context || context->possessed()) return;

            const float screen_width = ImGui::GetIO().DisplaySize.x;
            const float screen_height = ImGui::GetIO().DisplaySize.y;
            const float options_length = 200.0f;

            ImGui::SetNextWindowPos({ screen_width / 2 - options_length / 2, screen_height / 3 }, ImGuiCond_Always);
            ImGui::SetNextWindowSize({ screen_width, screen_height });
            ImGui::Begin("##title_screen", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);

            // title
            immediate::print("Gnomi##title_name", options_length);
            ImGui::NewLine();

            // options
            if (immediate::button("Start", options_length)) press_start();
            if (immediate::button("Exit##title_exit", options_length))	press_exit();

            ImGui::End();
        }

    private:
        void press_start() {
            context->start();
        }
        void press_exit() {
            context->shutdown();
        }

    private:
        environment * context;
    };
}