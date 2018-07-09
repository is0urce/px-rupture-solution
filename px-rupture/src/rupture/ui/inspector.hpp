// name: inspector.hpp
// type: c++
// auth: is0urce
// desc: ui inspectior panel class

#pragma once

#include "inspector_common.hpp"

#include "../environment.hpp"
#include "../es/transform_component.hpp"
#include "../es/body_component.hpp"

namespace px::ui {

    class inspector final
        : public inspector_common
    {
    public:
        virtual ~inspector() override = default;

        inspector(environment * env)
            : game(env) {
        }

    protected:
        virtual void combine_panel() override {
            if (auto target = game ? game->target() : nullptr) {
                if (auto body = target->linked<body_component>()) {
                    ImVec2 const& screen = ImGui::GetIO().DisplaySize;
                    ImVec2 const window_size(300.0f, 125.0f);
                    ImVec2 const window_padding(16, 65);
                    ImVec2 const position(screen.x - window_size.x - window_padding.x, screen.y - window_size.y - window_padding.y);

                    combine_status("##inspector", position, window_size, *body);
                }
            }
        }

    private:
        environment * game;
    };
}