// name: constitution.hpp

#pragma once

#include "bar.hpp"

#include <optional>

namespace px::rl {

    template <typename I>
    class constitution {
    public:
        using resource_type = std::optional<bar<I>>;

    public:
        bool is_dead() const {
            return hp && hp->is_empty();
        }

        bool is_alive() const {
            return hp && !hp->is_empty();
        }

        bool has_health() const noexcept {
            return hp.has_value();
        }

        bool has_energy() const noexcept {
            return mp.has_value();
        }

        resource_type & health() noexcept {
            return hp;
        }

        resource_type & energy() noexcept {
            return mp;
        }

        resource_type const& health() const noexcept {
            return hp;
        }

        resource_type const& energy() const noexcept {
            return mp;
        }

        template <typename Archive>
        void serialize(Archive & archive) {
            bool has_hp = hp.has_value();
            bool has_mp = mp.has_value();
            archive(has_hp, has_mp);
            if (has_hp) {
                if (!hp.has_value()) hp.emplace();
                archive(*hp);
            }
            if (has_mp) {
                if (!mp.has_value()) mp.emplace();
                archive(*mp);
            }
        }

    public:
        constitution() = default;

    private:
        resource_type hp;
        resource_type mp;
    };
}