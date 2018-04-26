// name: effect.hpp

#pragma once

#include <cstdint>

namespace px::rl {

    enum class effect : std::uint32_t {
        invalid = 0,

        weight = 10,
        value = 11,
        duration = 12,

        equipment = 20,
        useable = 21,

        damage = 30,
        dot = 31,

        accuracy = 40,
        critical = 41,
        speed = 42,

        power = 50,
        strength = 51,
        dexterity = 52,
        intelligence = 53,
        vitality = 54,

        armor = 61,
        resistance = 62,
        dodge = 63,

        hp_bonus = 70,
        hp_regen = 71,
        mp_bonus = 72,
        mp_regen = 73,

        essence = 80,
        ingredient_power = 81,

        potion_intoxication = 90
    };
}