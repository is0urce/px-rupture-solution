// name: inspector_common.hpp
// type: c++
// auth: is0urce
// desc: status ui panel class

#pragma once

#include "panel.hpp"
#include "immediate.hpp"

#include "../es/body_component.hpp"

namespace px::ui {

    class inspector_common
        : public panel
    {
    public:
        virtual ~inspector_common() = default;

    protected:
        void combine_status(body_component const& body) {
            ImGui::Text(body.name().c_str());
            immediate::bar(body.health(), "hp", { 0.5f, 0.0f, 0.0f, 1.0f });
            immediate::bar(body.energy(), "mp", { 0.0f, 0.0f, 0.5f, 1.0f });
            if (body.is_useable()) {
                ImGui::Text("useable [e]");
            }
        }
    };
}