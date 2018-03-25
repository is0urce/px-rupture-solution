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

#include <functional>   // for hash
#include <string>       // for tags

namespace px::rl {

    class craft {
    public:
        using item_type = rl::item;
        using task_type = craft_task<item_type>;
        using enhancement_type = item_type::enhancement_type;
        using integer_type = enhancement_type::integer_type;
        using real_type = enhancement_type::real_type;

    public:
        void assign_environment(environment * game) {
            context = game;
        }

        uq_ptr<rl::item> create(craft_recipe const& recipe, task_type const& task) {
            switch (recipe.category)
            {
            case rl::item_category::weapon: return weapon(recipe, task);
            case rl::item_category::armor: return armor(recipe, task);
            case rl::item_category::potion: return potion(task);
            default:
                break;
            }
            return nullptr;
        }

        uq_ptr<rl::item> weapon(craft_recipe const& recipe, task_type const& task) {
            auto item = make_uq<rl::item>();

            int quality = roll_quality();

            auto essence = calculate_essence(task);
            auto reagent_power = calculate_power(task).magnitude0;

            reagent_power *= 1.0 + std::pow(1.1, 1 + quality);

            auto power_raw = reagent_power * recipe.power_raw;
            auto power_enh = reagent_power * recipe.power_enhancement;

            enhance(*item, essence, power_enh);
            setup_equipment(*item, recipe);

            item->add(enhancement_type::real(rl::effect::damage, 0x00, power_raw));
            return item;
        }

        uq_ptr<rl::item> armor(craft_recipe const& recipe, task_type const& task) {
            auto item = make_uq<rl::item>();

            int quality = roll_quality();

            auto essence = calculate_essence(task);
            auto reagent_power = calculate_power(task).magnitude0;

            reagent_power *= 1.0 + std::pow(1.1, 1 + quality);

            auto power_raw = reagent_power * recipe.power_raw;
            auto power_enh = reagent_power * recipe.power_enhancement;

            enhance(*item, essence, power_enh);
            setup_equipment(*item, recipe);

            item->add(enhancement_type::real(rl::effect::armor, 0x00, power_raw));
            return item;
        }

        uq_ptr<rl::item> potion(task_type const& task) {
            auto essence = calculate_essence(task);
            auto reagent_power = calculate_power(task).magnitude0;

            return solution("potion", essence, reagent_power);
        }

        uq_ptr<rl::item> solution(std::string const& name, integer_type essence, real_type power) {
            auto item = make_uq<rl::item>();
            item->setup_entity(name, "i_potion#" + std::to_string(essence) + "#" + std::to_string(power));
            item->make_stacking();

            item->add(enhancement_type::zero(effect::useable));

            switch (hash_mod(essence, 4)) {
            case 0:
                item->add(enhancement_type::real(effect::hp_bonus, 0x00, power));
                break;
            case 1:
                item->add(enhancement_type::real(effect::hp_regen, 0x00, power));
                break;
            case 2:
                item->add(enhancement_type::real(effect::mp_bonus, 0x00, power));
                break;
            case 3:
                item->add(enhancement_type::real(effect::mp_regen, 0x00, power));
                break;
            }

            // store essence origin
            item->add(enhancement_type::integral(effect::essence, 0x00, essence));

            return item;
        }

    public:
        craft()
            : context(nullptr)
        {
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

        // setup general item attributes
        static void setup_equipment(rl::item & item, craft_recipe const& recipe) {
            item.setup_entity(generate_name(recipe), recipe.tag, recipe.description);
            item.make_single();

            if (recipe.equipment_slot != rl::equipment::not_valid) {
                item.add(enhancement_type::zero(effect::equipment, static_cast<integer_type>(recipe.equipment_slot)));
            }
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

        // biased in both hash and modulo, but we don't need much accuracy
        static unsigned int hash_mod(unsigned int x, unsigned int modulo) {
            return std::hash<unsigned int>{}(x) % modulo;
        }

        static std::string generate_name(craft_recipe const& recipe) {
            std::string prefix = "";
            std::string postfix = "";
            return prefix + " " + recipe.name + " " + postfix;
        }

        int roll_quality() {
            int quality = 0;
            if (context->roll(1, 5) == 5) {
                ++quality; // rare
                if (context->roll(1, 5) == 5) {
                    ++quality; // epic
                    if (context->roll(1, 5) == 5) {
                        ++quality; // legend
                    }
                }
            }
            return quality;
        }

    private:
        environment * context;
    };
}