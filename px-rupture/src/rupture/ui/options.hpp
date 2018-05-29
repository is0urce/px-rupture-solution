// name: options.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "panel.hpp"

#include "../environment.hpp"

#include <px/app/cfg.hpp>

#include <imgui/imgui.h>

namespace px::ui {

    class options final
        : public panel
    {
    public:
        virtual ~options() override = default;

        options(environment * ctx, cfg * config)
            : context(ctx)
            , configuration(config) {
        }

    protected:
        virtual void combine_panel() override {
            //if (!context || !configuration) return;
            //ImGui::Begin("##options");
            //ImGui::End();
        }

    private:
        environment *   context;
        cfg *           configuration;
    };
}