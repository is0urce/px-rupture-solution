// name: escape.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "panel.hpp"

#include "../environment.hpp"

#include <imgui/imgui.h>

namespace px::ui {

    class escape final
        : public panel
    {
    public:
        virtual ~escape() override = default;

        escape(environment * ctx)
            : context(ctx) {
        }

    protected:
        virtual void combine_panel() override {
            //if (!context) return;
            //ImGui::Begin("##escape");
            //ImGui::End();
        }

    private:
        environment * context;
    };
}