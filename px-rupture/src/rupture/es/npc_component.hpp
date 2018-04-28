// name: npc_component.hpp

#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>

#include <px/common/point.hpp>
#include <px/rl/ai_state.hpp>

#include <cstdint>
#include <tuple>

namespace px {

    class transform_component;

    class npc_component
        : public component
        , public link<transform_component>
        , public link_dispatcher<npc_component>
    {
    public:
        rl::ai_state get_state() const noexcept {
            return state;
        }

        std::uint32_t get_range() const noexcept {
            return state == rl::ai_state::idle ? range_idle : range_alert;
        }

        std::tuple<std::uint32_t, std::uint32_t> get_ranges() const noexcept {
            return { range_idle, range_alert };
        }

        void set_state(rl::ai_state current) noexcept {
            state = current;
        }

        void set_range(std::uint32_t idle, std::uint32_t alert) noexcept {
            range_idle = idle;
            range_alert = alert;
        }

        point2 & destination() {
            return waypoint;
        }

        bool is_idle() const noexcept {
            return state == rl::ai_state::idle;
        }

        void alert() {
            state = rl::ai_state::alert;
        }

        template <typename Archive>
        void serialize(Archive & archive) {
            archive(state, waypoint, range_idle, range_alert);
        }

    public:
        virtual ~npc_component() = default;

        npc_component()
            : state(rl::ai_state::idle)
            , range_idle(0)
            , range_alert(0)
            , waypoint{ 0, 0 } {
        }

        npc_component(npc_component const&) = delete;
        npc_component & operator=(npc_component const&) = delete;

    private:
        rl::ai_state    state;
        std::uint32_t   range_idle;
        std::uint32_t   range_alert;
        point2          waypoint;
    };
}