// name: status.hpp
// type: c++
// auth: is0urce
// desc: status ui panel class

#pragma once

#include "inspector_common.hpp"

#include "../environment.hpp"
#include "../es/transform_component.hpp"
#include "../es/body_component.hpp"
#include "../rl/traits.hpp"

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
                ImVec2 const window_padding(16, 100);
                ImVec2 const position(window_padding.x, screen.y - window_size.y - window_padding.y);
                combine_status("##player_status", position, window_size, *body);

                // exp
                ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0, 0, 0, 0 });
                ImGui::SetNextWindowPos({ position.x, position.y + window_size.y }, ImGuiCond_Always);
                ImGui::SetNextWindowSize(window_size);
                ImGui::Begin("##status_exp", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
                ImGui::Text("Experience: %d/%d", body->experience(), traits::experience_to_level(body->level()));
                ImGui::End();
                ImGui::PopStyleColor(1);

                // health popup
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