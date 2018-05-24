// name: craft.hpp
// type: c++
// auth: is0urce
// desc: item creation

#pragma once

#include "../environment.hpp"
#include "item.hpp"
#include "craft_recipe.hpp"
#include <px/rl/craft_task.hpp>

#include <px/memory/memory.hpp>
#include <px/dev/assert.hpp>

#include <functional>   // for std::hash
#include <map>
#include <string>       // for std::string tags
#include <vector>

namespace px::rl {

    class craft {
    public:
        using item_type = rl::item;
        using task_type = craft_task<item_type>;
        using enhancement_type = item_type::enhancement_type;
        using integer_type = enhancement_type::integer_type;
        using real_type = enhancement_type::real_type;

    public:
        void assign_environment(environment * game) noexcept {
            context = game;
        }

        uq_ptr<rl::item> create(craft_recipe const& recipe, task_type const& task) {
            switch (recipe.category) {
            case rl::item_category::weapon:
            case rl::item_category::armor:
                return create_equipment(recipe, task);
            case rl::item_category::potion:
                return create_potion(task);
            default:
                return nullptr;
            }
        }

        uq_ptr<rl::item> create_equipment(craft_recipe const& recipe, task_type const& task) {
            const auto rarity = roll_rarity();
            const auto essence = calculate_essence(task);
            const auto reagent_power = calculate_power(task).magnitude0 * (1.0 + std::pow(1.1, 1 + rarity));
            const auto power_raw = reagent_power * recipe.power_raw;
            const auto power_enh = reagent_power * recipe.power_enhancement;

            auto item = make_uq<rl::item>();

            item->setup_entity(generate_equipment_name(recipe, essence, rarity), recipe.tag, recipe.description);

            enhance(*item, essence, power_enh);
            if (recipe.equipment_slot != rl::equipment::not_valid) item->add(enhancement_type::zero(effect::equipment, static_cast<integer_type>(recipe.equipment_slot)));

            switch (recipe.category) {
            case rl::item_category::weapon:
                item->add(enhancement_type::real(rl::effect::damage, 0x00, power_raw));
                break;
            case rl::item_category::armor:
                item->add(enhancement_type::real(rl::effect::armor, 0x00, power_raw));
                break;
            default:
                break;
            }

            return item;
        }

        uq_ptr<rl::item> create_potion(task_type const& task) {

            // 'settings'

            static unsigned int const potion_base_duration = 5;

            static double const hp_bonus_multiplier = 1.0;
            static double const hp_regen_multiplier = 1.0 / potion_base_duration;
            static double const armor_multiplier = 1.0;
            static double const damage_multiplier = 0.2;

            // calculate values

            const auto essence = calculate_essence(task);
            const auto power = calculate_power(task).magnitude0;

            auto h = hash(essence);
            auto const variation = h % potion_variation.size();
            h /= potion_variation.size();
            auto const color = h % potion_color.size();
            h /= potion_color.size();
            auto const carrier = h % potion_carrier.size();

            double const hp_bonus_value = power * hp_bonus_multiplier;
            double const hp_regen_value = power * hp_regen_multiplier;
            double const armor_value = power * armor_multiplier;
            double const damage_value = power * damage_multiplier;

            double const accuracy_value = 0.2;
            double const critical_value = 0.2;
            double const dodge_value = 0.2;
            double const damage_bonus_value = 0.2;
            double const mp_bonus_value = 20;
            double const mp_regen_value = mp_bonus_value / potion_base_duration;

            // create item

            auto item = make_uq<rl::item>();
            switch (variation) {

            case 0:
                // hp (health)
                add<effect::hp_heal>(*item, hp_bonus_value);
                break;
            case 1:
                // hp_regen (regeneration)
                add<effect::hp_regen>(*item, hp_regen_value);
                break;
            case 2:
                // armor (lead skin)
                add<effect::armor>(*item, armor_value);
                break;
            case 3:
                // damage (rage)
                add<effect::damage>(*item, damage_value);
                break;

            case 4:
                // hp + accuracy (clarity)
                add<effect::hp_heal>(*item, hp_bonus_value * 0.5);
                add<effect::accuracy>(*item, accuracy_value * 0.5);
                break;
            case 5:
                // hp + critical (reaction)
                add<effect::hp_heal>(*item, hp_bonus_value * 0.5);
                add<effect::critical>(*item, critical_value * 0.5);
                break;
            case 6:
                // hp + dodge (respite)
                add<effect::hp_heal>(*item, hp_bonus_value * 0.5);
                add<effect::dodge>(*item, dodge_value * 0.5);
                break;
            case 7:
                // hp + dmg_bonus (warrior)
                add<effect::hp_heal>(*item, hp_bonus_value * 0.5);
                add<effect::damage_bonus>(*item, damage_bonus_value * 0.5);
                break;
            case 8:
                // hp + mp (rejuvenation)
                add<effect::hp_heal>(*item, hp_bonus_value * 0.5);
                add<effect::mp_heal>(*item, mp_bonus_value * 0.5);
                break;
            case 9:
                // hp + mp_regen (soothing)
                add<effect::hp_heal>(*item, hp_bonus_value * 0.5);
                add<effect::mp_regen>(*item, mp_regen_value * 0.5);
                break;

            case 10:
                // hp_regen + accuracy (swirling)
                add<effect::hp_regen>(*item, hp_regen_value * 0.5);
                add<effect::accuracy>(*item, accuracy_value * 0.5);
                break;
            case 11:
                // hp_regen + critical (courage)
                add<effect::hp_regen>(*item, hp_regen_value * 0.5);
                add<effect::critical>(*item, critical_value * 0.5);
                break;
            case 12:
                // hp_regen + dodge (hope)
                add<effect::hp_regen>(*item, hp_regen_value * 0.5);
                add<effect::dodge>(*item, dodge_value * 0.5);
                break;
            case 13:
                // hp_regen + dmg_bonus (troll)
                add<effect::hp_regen>(*item, hp_regen_value * 0.5);
                add<effect::damage_bonus>(*item, damage_bonus_value * 0.5);
                break;
            case 14:
                // hp_regen + mp (vivacity)
                add<effect::hp_regen>(*item, hp_regen_value * 0.5);
                add<effect::mp_heal>(*item, mp_bonus_value * 0.5);
                break;
            case 15:
                // hp_regen + mp_regen (stamina)
                add<effect::hp_regen>(*item, hp_regen_value * 0.5);
                add<effect::mp_regen>(*item, mp_regen_value * 0.5);
                break;

            case 16:
                // hp + hp_regen (vigor)
                add<effect::hp_heal>(*item, hp_bonus_value * 0.5);
                add<effect::hp_regen>(*item, hp_regen_value * 0.5);
                break;

            case 17:
                // hp + armor (guardian)
                add<effect::hp_heal>(*item, hp_bonus_value * 0.5);
                add<effect::armor>(*item, armor_value * 0.5);
                break;
            case 18:
                // hp + damage (venegance)
                add<effect::hp_heal>(*item, hp_bonus_value * 0.5);
                add<effect::damage>(*item, damage_value * 0.5);
                break;
            case 19:
                // hp_regen + armor (bark skin)
                add<effect::hp_regen>(*item, hp_regen_value * 0.5);
                add<effect::armor>(*item, armor_value * 0.5);
                break;
            case 20:
                // hp_regen + damage (ogre)
                add<effect::hp_regen>(*item, hp_regen_value * 0.5);
                add<effect::damage>(*item, damage_value * 0.5);
                break;

            case 21:
                // armor + accuracy (knight)
                add<effect::armor>(*item, armor_value * 0.5);
                add<effect::accuracy>(*item, accuracy_value * 0.5);
                break;
            case 22:
                // armor + critical (crusader)
                add<effect::armor>(*item, armor_value * 0.5);
                add<effect::critical>(*item, critical_value * 0.5);
                break;
            case 23:
                // armor + dmg_bonus (soldier)
                add<effect::armor>(*item, armor_value * 0.5);
                add<effect::damage_bonus>(*item, damage_bonus_value * 0.5);
                break;
            case 24:
                // armor + mp (battle)
                add<effect::armor>(*item, armor_value * 0.5);
                add<effect::mp_heal>(*item, mp_bonus_value * 0.5);
                break;
            case 25:
                // armor + mp_regen (invoker)
                add<effect::armor>(*item, armor_value * 0.5);
                add<effect::mp_regen>(*item, mp_regen_value * 0.5);
                break;

            case 26:
                // damage + accuracy (slaughter)
                add<effect::damage>(*item, damage_value * 0.5);
                add<effect::accuracy>(*item, accuracy_value * 0.5);
                break;
            case 27:
                // damage + critical (sacrifices)
                add<effect::damage>(*item, damage_value * 0.5);
                add<effect::critical>(*item, critical_value * 0.5);
                break;
            case 28:
                // damage + dodge (tricks)
                add<effect::damage>(*item, damage_value * 0.5);
                add<effect::dodge>(*item, dodge_value * 0.5);
                break;
            case 29:
                // damage + dmg_bonus (violence)
                add<effect::damage>(*item, damage_value * 0.5);
                add<effect::damage_bonus>(*item, damage_bonus_value * 0.5);
                break;
            case 30:
                // damage + mp (wrath)
                add<effect::damage>(*item, damage_value * 0.5);
                add<effect::mp_heal>(*item, mp_bonus_value * 0.5);
                break;
            case 31:
                // damage + mp_regen (brawl)
                add<effect::damage>(*item, damage_value * 0.5);
                add<effect::mp_regen>(*item, mp_regen_value * 0.5);
                break;

            default:
                px_debug("potion variant not selected");
                break;
            }

            // other item props

            item->add(enhancement_type::integral(effect::essence, 0x00, essence));  // store essence origin
            item->add(enhancement_type::zero(effect::useable));

            item->add(enhancement_type::zero(effect::intoxication));
            //item->add(enhancement_type::real(effect::intoxication_power, 0x00, 1));
            item->add(enhancement_type::integral(effect::intoxication_duration, 0x00, 20));

            bool is_instant = true;
            item->enumerate([&](auto const& efx) { is_instant &= !has_duration(efx.main); });
            if (!is_instant) {
                item->add(enhancement_type::integral(effect::duration, 0x00, potion_base_duration));
            }

            item->set_name(potion_color[color] + " " + potion_carrier[carrier] + " of " + potion_variation[variation]);
            item->set_tag("i_potion_" + std::to_string(essence) + "_" + std::to_string(power));

            item->make_stacking();

            return item;
        }

        template <typename I>
        static auto hash(I x) {
            return std::hash<I>{}(x);
        }

        // biased in both hash and modulo, but we don't need much accuracy
        template <typename I>
        static auto hash_mod(I x, I modulo) {
            return hash(x) % modulo;
        }

    public:
        craft()
            : context(nullptr) {
            name_equipment_prefix = {
                "boris",
                "ivan",
                "nestor",
                "executor"
            };
            name_equipment_postfix = {
                "slashworks",
                "intrigue",
                "equilibriub",
                "abyss" };
            name_substance = {
                { 3, "numidium" }
            };
            potion_color = {
                "amber",
                "azure",
                "beige",
                "black",
                "blue",
                "brown",
                "crimson",
                "cyan",
                "grey",
                "green",
                "indigo",
                "magenta",
                "orange",
                "pink",
                "purple",
                "red",
                "teal",
                "turquoise",
                "vermillion"
                "violet",
                "white",
                "yellow"
            };
            potion_carrier = {
                "brew",
                "draught",
                "elixir",
                "eyedrops",
                "flack",
                "fluid",
                "juice",
                "lotion",
                "nostrum",
                "oil",
                "ointment",
                "pastil",
                "philter",
                "pill",
                "potion",
                "remedy",
                "serum",
                "tonic",
                "vial",
                "water"
            };
            potion_variation = {
                "health",
                "regeneration",
                "lead_skin",
                "rage",

                "clarity",
                "reaction",
                "respite",
                "warrior",
                "rejuvenation",
                "soothing",

                "swirling",
                "courage",
                "hope",
                "troll",
                "vivacity",
                "stamina",

                "vigor",

                "guardian",
                "venegance",
                "bark skin",
                "ogre",

                "knight",
                "crusader",
                "soldier",
                "battle",
                "invoker",

                "slaughter",
                "sacrifices",
                "tricks",
                "violence",
                "wrath",
                "brawl"
            };
        }

    private:
        // origin essence common denominator
        static integer_type calculate_essence(craft_task<item> const& task) {
            integer_type denominator = 1;
            task.enumerate([&](auto const& item) {
                auto enhancement = item.find({ effect::essence });
                auto essence = enhancement.value0;
                if (essence != 0 && denominator % essence != 0) {
                    denominator *= essence;
                }
            });
            return denominator;
        }

        // total power
        static enhancement_type calculate_power(craft_task<item> const& task) {
            auto power = enhancement_type::zero(effect::ingredient_power);
            task.enumerate([&](auto const& item) {
                power = item.accumulate(power);
            });
            return power;
        }

        template <rl::effect ID>
        static void add(rl::item & item, double value) {
            item.add(enhancement_type::real(ID, 0x00, value));
        }

        // add item properties
        static void enhance(rl::item & item, integer_type essence, real_type power) {
            switch (hash_mod(essence, 2)) {
            case 0:
                item.add(enhancement_type::real(effect::critical, 0x00, power));
                break;
            case 1:
                item.add(enhancement_type::real(effect::dodge, 0x00, power));
                break;
            }

            // store essence origin
            item.add(enhancement_type::integral(effect::essence, 0x00, essence));
        }

        int roll_rarity() {
            int quality = 0;
            if (context->roll(1, 5) == 5) {
                ++quality;          // rare
                if (context->roll(1, 5) == 5) {
                    ++quality;      // epic
                    if (context->roll(1, 5) == 5) {
                        ++quality;  // legend
                    }
                }
            }
            return quality;
        }

        std::string generate_equipment_name(rl::craft_recipe const& recipe, integer_type essence, int rarity) {
            std::string result = name_base(recipe);
            result = name_material(result, essence);
            result = name_attributes(result, essence);
            result = name_rarity(result, rarity);
            return result;
        }

        std::string name_base(rl::craft_recipe const& recipe) const {
            return recipe.name;
        }

        std::string name_material(std::string name, integer_type essence) const {
            auto found = name_substance.find(essence);
            if (found != name_substance.end()) return found->second + " " + name;

            return name;
        }

        std::string name_attributes(std::string name, integer_type essence) const {
            return name + " of " + select(name_equipment_postfix, essence);
        }

        std::string name_rarity(std::string name, int rarity) {
            if (rarity != 0) {
                return select_random(name_equipment_prefix) + " " + name;
            }
            return name;
        }

        // select one of names based on hashed essence value
        std::string select(std::vector<std::string> const& names, integer_type essence) const {
            if (names.empty()) throw std::runtime_error("craft::select(names, essence) - names is empty");
            return names[hash_mod(essence, static_cast<integer_type>(names.size() - 1))];
        }

        // select random item
        std::string select_random(std::vector<std::string> const& names) {
            if (names.empty()) throw std::runtime_error("craft::select(names, essence) - names is empty");
            size_t n = context->roll(0, static_cast<int>(names.size() - 1));
            return names[n];
        }

        bool constexpr has_duration(effect efx) {
            return efx == effect::damage_periodic
                || efx == effect::damage_thorns
                || efx == effect::damage_bonus
                || efx == effect::accuracy
                || efx == effect::critical
                || efx == effect::speed
                || efx == effect::armor
                || efx == effect::resistance
                || efx == effect::dodge
                || efx == effect::hp_regen
                || efx == effect::mp_regen;
        }

    private:
        std::map<integer_type, std::string> name_substance;
        std::vector<std::string>            name_equipment_postfix;
        std::vector<std::string>            name_equipment_prefix;
        std::vector<std::string>            potion_variation;
        std::vector<std::string>            potion_color;
        std::vector<std::string>            potion_carrier;
        environment *                       context;
    };
}