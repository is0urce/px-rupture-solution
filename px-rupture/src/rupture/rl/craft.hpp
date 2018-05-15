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

            // calculate values

            const auto essence = calculate_essence(task);
            const auto power = calculate_power(task).magnitude0;

            auto h = hash(essence);
            auto const variation = h % potion_variation.size();
            h /= potion_variation.size();
            auto const color = h % potion_color.size();
            h /= potion_color.size();
            auto const carrier = h % potion_carrier.size();

            // create item

            auto item = make_uq<rl::item>();
            switch (variation) {
            case 0:     // health
                item->add(enhancement_type::real(effect::hp_bonus, 0x00, power));
                break;
            case 1:     // regeneration
                item->add(enhancement_type::real(effect::hp_regen, 0x00, power));
                break;
            case 2:
                item->add(enhancement_type::real(effect::mp_bonus, 0x00, power));
                break;
            case 3:
                item->add(enhancement_type::real(effect::mp_regen, 0x00, power));
                break;
            case 4:     // rejuvenation
                item->add(enhancement_type::real(effect::hp_bonus, 0x00, power / 2));
                item->add(enhancement_type::real(effect::mp_bonus, 0x00, power / 2));
                break;
            case 5:     // stamina
                item->add(enhancement_type::real(effect::mp_bonus, 0x00, power / 2));
                item->add(enhancement_type::real(effect::mp_regen, 0x00, power / 2));
                break;
            case 6:     // endurance
                item->add(enhancement_type::real(effect::hp_regen, 0x00, power / 2));
                item->add(enhancement_type::real(effect::mp_regen, 0x00, power / 2));
                break;
            case 7:     // vivacity
                item->add(enhancement_type::real(effect::hp_bonus, 0x00, power / 2));
                item->add(enhancement_type::real(effect::hp_regen, 0x00, power / 2));
                break;
            case 8:     // lead skin
                item->add(enhancement_type::real(effect::armor, 0x00, power / 2));
                item->add(enhancement_type::real(effect::hp_bonus, 0x00, power / 2));
                break;
            case 9:     // true strike
                item->add(enhancement_type::real(effect::accuracy, 0x00, power / 2));
                item->add(enhancement_type::real(effect::critical, 0x00, power / 2));
                break;
            case 10:    // avoidance
                item->add(enhancement_type::real(effect::dodge, 0x00, power / 2));
                item->add(enhancement_type::real(effect::armor, 0x00, power / 2));
                break;
            case 11:    // bark skin
                item->add(enhancement_type::real(effect::armor, 0x00, power / 2));
                item->add(enhancement_type::real(effect::hp_regen, 0x00, power / 2));
                break;
            default:
                px::debug("potion variant not selected");
                break;
            }

            // other item props

            item->add(enhancement_type::integral(effect::essence, 0x00, essence));  // store essence origin
            item->add(enhancement_type::zero(effect::useable));

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
                "innervation",
                "invigoration",
                "rejuvenation",
                "stamina",
                "endurance",
                "vivacity",
                "lead skin",
                "true strike",
                "avoidance",
                "bark skin"
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