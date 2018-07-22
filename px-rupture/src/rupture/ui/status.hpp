// name: status.hpp
// type: c++
// auth: is0urce
// desc: status ui panel class

#pragma once

#include "inspector_common.hpp"

#include "../environment.hpp"
#include "../es/transform_component.hpp"
#include "../es/body_component.hpp"

namespace px::ui {

    class status final
        : public inspector_common {
    public:
        virtual ~status() override = default;

        status(environment * game)
            : context(game)
            , lowhp_message(false) {
        }

    protected:
        virtual void combine_panel() override {
            auto pawn = context ? context->controlled() : nullptr;
            auto body = pawn ? pawn->linked<body_component>() : nullptr;
            if (body) {
                ImVec2 const& screen = ImGui::GetIO().DisplaySize;
                ImVec2 const window_size(300, 125);
                ImVec2 const window_padding(16, 65);
                ImVec2 const position(window_padding.x, screen.y - window_size.y - window_padding.y);
                combine_status("##player_status", position, window_size, *body);

                if (auto const& hp = body->health()) {
                    float const percent = static_cast<float>(hp->current()) / static_cast<float>(hp->maximum());

                    if (!lowhp_message) {
                        if (percent < 0.33f) {
                            context->popup("low health!", 0xff0000, pawn->position());
                            lowhp_message = true;
                        }
                    }
                    else {
                        if (percent > 0.50f) {
                            lowhp_message = false;
                        }
                    }
                }
            }
        }

    private:
        environment * context;
        bool          lowhp_message;
    };
}