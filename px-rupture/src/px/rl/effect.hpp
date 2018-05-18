// name: effect.hpp
// type: c++
// auth: is0urce
// desc: effect id enum

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
        damage_periodic = 31,
        damage_thorns = 32,
        damage_bonus = 33,

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

        intoxication = 90,
        intoxication_power = 91,
        intoxication_duration = 92
    };
}