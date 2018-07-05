// name: field_description.hpp
// type: c++
// auth: is0urce
// type: class-aggregated static functions

#pragma once

#include <imgui/imgui.h>

#include <px/rl/effect.hpp>
#include <px/rl/craft_activity.hpp>

#include <iomanip>
#include <string>
#include <sstream>

namespace px {

    class field_description final {
    public:

        template <typename Buff>
        static void display_buff(Buff const& affect) {
            display_entity(item);
            display_collection(item);
        }

        template <typename Item>
        static void display_item(Item const& item) {
            display_entity(item);
            display_collection(item);
            display_stack(item);
        }

        template <typename Entity>
        static void display_entity(Entity const& record) {
            ImGui::Text(record.name().c_str());
            ImGui::Text(record.description().c_str());
        }

        template <typename Collection>
        static void display_collection(Collection const& collection) {

            print_bool<rl::effect::equipment>(collection, "Equipment");
            print_bool<rl::effect::useable>(collection, "Useable");

            if (collection.has_effect<rl::effect::ingredient_power>()) {
                switch (static_cast<rl::craft_activity>(collection.find_subtype(rl::effect::ingredient_power, 0))) {
                case rl::craft_activity::blacksmith:
                    ImGui::Text("Reagent: Blacksmith");
                    break;
                case rl::craft_activity::alchemy:
                    ImGui::Text("Reagent: Alchemy");
                    break;
                default:
                    ImGui::Text("Reagent: Generic");
                    break;
                }
            }

            // equipment props

            print_real<rl::effect::damage>(collection, "Damage: %.0f");
            print_real<rl::effect::armor>(collection, "Armor: %.0f");
            print_real<rl::effect::accuracy>(collection, "Accuracy: %.2f");
            print_real<rl::effect::critical>(collection, "Critical: %.2f");
            print_real<rl::effect::dodge>(collection, "Dodge: %.2f");

            // potion props

            print_real<rl::effect::hp_heal>(collection, "Heal: %.2f");
            print_real<rl::effect::mp_heal>(collection, "Innervate: %.2f");
            print_real<rl::effect::hp_regen>(collection, "Regenerate: %.2f");
            print_real<rl::effect::mp_regen>(collection, "Invigorate: %.2f");

            print_bool<rl::effect::intoxication>(collection, "Toxic");
            print_bool<rl::effect::intoxication_power>(collection, "Intoxication");
        }

        template <typename Stack>
        static void display_stack(Stack const& pack) {
            if (pack.maximum() > 1) {
                ImGui::Text("Quantity: %d/%d", pack.count(), pack.maximum());
            }
        }

        template <rl::effect Efx, typename Collection>
        static void print_real(Collection const& collection, std::string const& format_with) {
            if (collection.has_effect<Efx>()) {
                ImGui::Text(format_with.c_str(), collection.accumulate<Efx>().magnitude0);
            }
        }

        template <rl::effect Efx, typename Collection>
        static void print_bool(Collection const& collection, std::string const& format_with) {
            if (collection.has_effect<Efx>()) {
                ImGui::Text(format_with.c_str());
            }
        }
    };
}