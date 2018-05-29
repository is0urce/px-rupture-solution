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

        options(environment * ctx, cfg * config, bool * flag)
            : context(ctx)
            , configuration(config)
            , open_flag(flag) {
        }

    protected:
        virtual void combine_panel() override {
            if (!context || !configuration || !is_open()) return;
            ImGui::Begin("##options");
            ImGui::End();
        }

    private:
        bool is_open() const noexcept {
            return open_flag && *open_flag;
        }

    private:
        environment *   context;
        cfg *           configuration;
        bool *          open_flag;
    };
}