// name: roll_list.hpp

#pragma once

#include <random>
#include <vector>

namespace px {

    class roll_list_base {
    public:
        using chance_type = double;
        enum class roll_policy : unsigned char { all_of, one_of };

    public:
        roll_list_base()
            : roll_list_base(roll_policy::all_of) {
        }
        roll_list_base(roll_policy rule)
            : total_chance(0)
            , policy(rule) {
        }

    protected:
        roll_policy             policy;
        chance_type             total_chance;
    };

    template <typename Item>
    class roll_list : roll_list_base {
    public:
        using value_type = Item;

    private:
        struct item_entry {
            chance_type         chance;
            value_type          value;
        };

        struct list_entry {
            chance_type         chance;
            roll_list const*    list;
        };

    public:
        void set_policy(roll_policy rule) noexcept {
            if (policy != rule) {
                policy = rule;
                if (rule == roll_policy::one_of) {
                    calculate_total_chance();
                }
            }
        }
        roll_policy get_policy() const noexcept {
            return policy;
        }

        void add_item(value_type item, chance_type chance) {
            values.push_back(item_entry{ chance, item });
            if (policy == roll_policy::one_of) {
                total_chance += chance;
            }
        }

        void add_list(roll_list const* list, chance_type chance) {
            expand.push_back(list_entry{ chance, list });
            if (policy == roll_policy::one_of) {
                total_chance += chance;
            }
        }

        template <typename Unary, typename Rng>
        void evaluate(Rng && rng, Unary && callback_function) const {
            switch (policy) {
            case roll_policy::all_of: {
                for (auto const& i : values) {
                    if (std::uniform_real_distribution<chance_type>{}(rng) < i.chance) {
                        callback_function(i.value);
                    }
                }
                for (auto const& i : expand) {
                    if (std::uniform_real_distribution<chance_type>{}(rng) < i.chance) {
                        i.list->evaluate(rng, callback_function);
                    }
                }
                break;
            }
            case roll_policy::one_of: {
                chance_type roll = std::uniform_real_distribution<chance_type>{ 0, total_chance }(rng);
                chance_type current = 0;
                for (auto const& i : values) {
                    current += i.chance;
                    if (roll < current) {
                        callback_function(i.value);
                        goto end_roll;  // break to the end of case
                    }
                }
                for (auto const& i : expand) {
                    current += i.chance;
                    if (roll < current) {
                        i.list->evaluate(rng, callback_function);
                        goto end_roll;  // break to the end of case
                    }
                }
            end_roll:
                break;
            }
            default:
                break;
            }
        }

    public:
        roll_list() {
        }
        roll_list(roll_policy rule)
            : roll_list_base(rule) {
        }

    private:
        void calculate_total_chance() {
            total_chance = 0;
            for (auto const& i : values) {
                total_chance += i.chance;
            }
            for (auto const& i : expand) {
                total_chance += i.chance;
            }
        }

    private:
        std::vector<item_entry> values;
        std::vector<list_entry> expand;
    };
}