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
#include <map>
#include <string>       // for tags
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
                return equipment(recipe, task);
            case rl::item_category::potion:
                return potion(task);
            default:
                return nullptr;
            }
        }

        uq_ptr<rl::item> equipment(craft_recipe const& recipe, task_type const& task) {
            int rarity = roll_rarity();

            auto essence = calculate_essence(task);
            auto reagent_power = calculate_power(task).magnitude0;

            reagent_power *= 1.0 + std::pow(1.1, 1 + rarity);

            auto power_raw = reagent_power * recipe.power_raw;
            auto power_enh = reagent_power * recipe.power_enhancement;

            auto name = generate_name(recipe, essence, rarity);

            auto item = make_uq<rl::item>();

            item->setup_entity(name, recipe.tag, recipe.description);

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

        uq_ptr<rl::item> potion(task_type const& task) {
            auto essence = calculate_essence(task);
            auto reagent_power = calculate_power(task).magnitude0;

            return solution("potion", essence, reagent_power);
        }

        uq_ptr<rl::item> solution(std::string const& name, integer_type essence, real_type power) {
            auto item = make_uq<rl::item>();
            item->setup_entity(name_potion(name, essence), "i_potion#" + std::to_string(essence) + "#" + std::to_string(power));
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

        // biased in both hash and modulo, but we don't need much accuracy
        template <typename I>
        static auto hash(I x) {
            return std::hash<I>{}(x);
        }

        static unsigned int hash_mod(unsigned int x, unsigned int modulo) {
            return hash(x) % modulo;
        }

    public:
        craft()
            : context(nullptr) {
            name_prefix = {
                "boris",
                "ivan",
                "nestor",
                "executor"
            };
            name_postfix = {
                "slashworks",
                "intrigue",
                "equilibriub",
                "abyss" };
            name_substance = { 
                { 3, "numidium" }
            };
            name_color = {
                "azure",
                "beige",
                "black",
                "blue",
                "brown",
                "crimson",
                "cyan",
                "gray",
                "green",
                "indigo",
                "magenta",
                "orange",
                "pink",
                "purple",
                "red",
                "teal",
                "turquoise",
                "violet",
                "white",
                "yellow"
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

        std::string name_potion(std::string const& base, integer_type essence) const {
            return select_name(name_color, essence) + " " + base;
        }

        std::string generate_name(rl::craft_recipe const& recipe, integer_type essence, int rarity) const {
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
            return name + " of " + select_name(name_postfix, essence);
        }

        std::string name_rarity(std::string name, int rarity) const {
            if (rarity != 0) {
                size_t n = context->roll(0, static_cast<int>(name_prefix.size() - 1));
                return name_prefix[n] + " " + name;
            }
            return name;
        }

        static std::string select_name(std::vector<std::string> const& names, integer_type essence) {
            if (names.empty()) throw std::runtime_error("craft::select(names, essence) - names is empty");
            return names[hash_mod(essence, static_cast<unsigned int>(names.size() - 1))];
        }

    private:
        std::map<integer_type, std::string> name_substance;
        std::vector<std::string>            name_postfix;
        std::vector<std::string>            name_prefix;
        std::vector<std::string>            name_color;
        environment *                       context;
    };
}