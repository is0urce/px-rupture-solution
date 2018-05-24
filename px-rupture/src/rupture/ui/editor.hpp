// name: editor.hpp
// type: c++
// auth: is0urce
// desc: editor window

#pragma once

#include "panel.hpp"

#include "../app/document.hpp"
#include "../app/settings.hpp"
#include "../es/builder.hpp"
#include "../es/animator_component.hpp"
#include "../es/composite_component.hpp"
#include "../es/character_component.hpp"
#include "../es/container_component.hpp"
#include "../es/deposite_component.hpp"
#include "../es/door_component.hpp"
#include "../es/light_component.hpp"
#include "../es/player_component.hpp"
#include "../es/npc_component.hpp"
#include "../es/sprite_component.hpp"
#include "../es/transform_component.hpp"
#include "../es/workshop_component.hpp"
#include "../io/schema.hpp"
#include "../io/blueprint.hpp"
#include "../io/serialization.hpp"
#include "../environment.hpp"

#include <px/memory/memory.hpp>

#include <imgui/imgui.h>

#include <algorithm>
#include <array>
#include <filesystem>
#include <string>
#include <vector>

#define PX_BUILD(OP)	{ \
                            builder factory(game); \
                            factory.begin(std::move(current)); \
                            factory.OP; \
                            current = factory.request(); \
                            update_props(); \
                        }

namespace px::ui {

    inline bool vector_getter(void * data, int n, const char** result) {
        auto & vector = *static_cast<std::vector<std::string>*>(data);
        if (n >= 0 && n < static_cast<int>(vector.size())) {
            *result = vector[n].c_str();
            return true;
        }
        return false;
    }

    class editor final
        : public panel
    {
    public:
        virtual ~editor() override = default;

        editor(environment * env)
            : game(env) {
            refresh_template_items();
        }

    protected:
        virtual void combine_panel() override {
            if (!game) return;

            transform_component * camera = game->possessed();

            const float window_width = 250.0f;
            const float screen_width = ImGui::GetIO().DisplaySize.x;
            const float screen_height = ImGui::GetIO().DisplaySize.y;
            ImGui::SetNextWindowPos({ screen_width - window_width, 0 }, ImGuiCond_Always);
            ImGui::SetNextWindowSize({ window_width, screen_height });
            ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
            ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

            // template selection

            ImGui::Combo("##shemata_list", &schema_selected, vector_getter, static_cast<void*>(&schemata), static_cast<int>(schemata.size()));
            ImGui::SameLine();
            if (ImGui::Button("load##load_schema")) {
                load_current_schema();
            }

            ImGui::Combo("##blueprints_list", &blueprint_selected, vector_getter, static_cast<void*>(&blueprints), static_cast<int>(blueprints.size()));
            ImGui::SameLine();
            if (ImGui::Button("load##load_blueprint")) {
                load_current_blueprint();
            }

            if (ImGui::Button("pick##pick_unit")) {
            }
            ImGui::SameLine();
            if (ImGui::Button("refresh##refresh_templates")) {
                refresh_template_items();
            }

            // object props

            if (current) {

                transform_component * transform = current->query<transform_component>();
                sprite_component * sprite = current->query<sprite_component>();
                body_component * body = current->query<body_component>();
                character_component * character = current->query<character_component>();
                container_component * container = current->query<container_component>();

                animator_component * animator = current->query<animator_component>();
                player_component * player = current->query<player_component>();
                npc_component * npc = current->query<npc_component>();
                light_component * light = current->query<light_component>();

                deposite_component * deposite = current->query<deposite_component>();
                door_component * door = current->query<door_component>();
                workshop_component * workshop = current->query<workshop_component>();

                // composite

                ImGui::Separator();
                ImGui::Text("composite");
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::Text("name: %s", current->name().c_str());
                    ImGui::Text("size: %d", current->size());
                    ImGui::Text("persistency: %d", static_cast<unsigned int>(current->lifetime()));
                    //ImGui::Separator();
                    //ImGui::Text("adress: %p", current.get());
                    ImGui::EndTooltip();
                }

                if (camera && transform) {
                    ImGui::SameLine();
                    if (ImGui::Button("at me##composite_place_at_me")) {
                        transform->place(camera->position());
                        current->enable();
                        update_props();
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("on##composite_on")) {
                    current->enable();
                }
                ImGui::SameLine();
                if (ImGui::Button("off##composite_off")) {
                    current->disable();
                }

                if (ImGui::InputText("##composite_name", composite_name.data(), composite_name.size() - 1, ImGuiInputTextFlags_AutoSelectAll)) {
                    current->set_name(composite_name.data());
                }

                // transform

                if (transform) {
                    ImGui::Separator();
                    ImGui::Text("transform");
                    if (ImGui::IsItemHovered()) {
                        ImGui::BeginTooltip();
                        ImGui::Text("space: %p", transform->world());
                        ImGui::Text("static: %s", transform->is_static() ? "yes" : "no");
                        ImGui::Text("current: (%d, %d)", transform->position().x(), transform->position().y());
                        ImGui::Text("last: (%d, %d)", transform->last().x(), transform->last().y());
                        ImGui::EndTooltip();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("x##remove_transform")) {
                        PX_BUILD(remove_transform());
                    }
                    else {
                        if (ImGui::Checkbox("static##tranform_flag", &transform_static)) {
                            transform->set_static(transform_static);
                        }

                        bool place = false;
                        ImGui::Text("x"); ImGui::SameLine(); place |= ImGui::InputInt("##transform_x", &transform_x);
                        ImGui::Text("y"); ImGui::SameLine(); place |= ImGui::InputInt("##transform_y", &transform_y);
                        if (place) {
                            transform->place({ transform_x, transform_y });
                        }
                    }
                }

                // visual
                combine_sprite(sprite);
                combine_animator(animator);
                combine_light(light);

                // rpg
                combine_body(body);
                combine_character(character);
                combine_container(container);

                // useables
                combine_deposite(deposite);
                combine_door(door);
                combine_workshop(workshop);

                // control
                combine_player(player);
                combine_npc(npc);

                ImGui::Separator();
                if (ImGui::Button("add...")) ImGui::OpenPopup("add##add_component");
                if (ImGui::BeginPopup("add##add_component")) {
                    if (!transform && ImGui::MenuItem("transform##add")) {
                        PX_BUILD(add_transform({ 0, 0 }));
                    }
                    if (!sprite && ImGui::MenuItem("sprite##add")) {
                        PX_BUILD(add_sprite("x_dummy"));
                    }
                    if (!animator && ImGui::MenuItem("animator##add")) {
                        PX_BUILD(add_animator("a_dummy"));
                    }
                    if (!light && ImGui::MenuItem("light##add")) {
                        PX_BUILD(add_light());
                    }
                    if (!body && ImGui::MenuItem("body##add")) {
                        PX_BUILD(add_body());
                    }
                    if (!character && ImGui::MenuItem("character##add")) {
                        PX_BUILD(add_character());
                    }
                    if (!container && ImGui::MenuItem("container##add")) {
                        PX_BUILD(add_container());
                    }
                    if (!deposite && !door && !workshop) {
                        if (ImGui::BeginMenu("useables..##add")) {
                            if (ImGui::MenuItem("deposit##add")) {
                                PX_BUILD(add_deposite());
                            }
                            if (ImGui::MenuItem("door##add")) {
                                PX_BUILD(add_door());
                            }
                            if (ImGui::MenuItem("workshop##add")) {
                                PX_BUILD(add_workshop());
                            }
                            ImGui::MenuItem("storage##add");
                            ImGui::EndMenu();
                        }
                    }
                    if (!player && !npc) {
                        if (ImGui::BeginMenu("controls..##add")) {
                            if (ImGui::MenuItem("player##add")) {
                                PX_BUILD(add_player());
                            }
                            if (ImGui::MenuItem("npc##add")) {
                                PX_BUILD(add_npc());
                            }
                            ImGui::EndMenu();
                        }
                    }
                    ImGui::EndPopup();
                }

                // spawn

                ImGui::Separator();
                if (transform) {

                    if (ImGui::Button("spawn##spawn_at_transform")) {
                        game->spawn(std::move(current));
                    }

                    if (camera) {
                        ImGui::SameLine();
                        if (ImGui::Button("here##spawn_here")) {
                            transform->place(camera->position());
                            game->spawn(std::move(current));
                        }
                    }
                }
                else {
                    ImGui::Text("require transform to spawn mobiles");
                }

                ImGui::Separator();
                if (ImGui::Button("export##export_composite")) {
                    export_composite();
                    refresh_template_items();
                }
                ImGui::SameLine();
                if (ImGui::Button("discard##discard_composite")) {
                    current = nullptr;
                }
            }

            ImGui::SetWindowFontScale(0.5f);
            ImGui::End();
        }

    private:
        void combine_animator(animator_component * animator) {
            if (!animator) return;

            ImGui::Separator();
            ImGui::Text("animator: %s", animator->get_id());
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("size: %d", animator->size());
                ImGui::Text("playing: %s", animator_playing ? "yes" : "no");
                if (animator_playing) {
                    ImGui::Text("current: %p", animator->current());
                }
                ImGui::EndTooltip();
            }

            ImGui::SameLine();
            if (ImGui::Button("x##remove_animator")) {
                PX_BUILD(remove_animator());
            }
            else {
                if (!animator->linked<sprite_component>()) {
                    ImGui::Text("WARNING: no associated sprite!");
                }
                ImGui::InputText("##animator_name", animator_name.data(), animator_name.size(), ImGuiInputTextFlags_AutoSelectAll);
                ImGui::SameLine();
                if (ImGui::Button("set##set_animator")) {
                    std::string last = animator->get_id();
                    std::string str(animator_name.data());
                    PX_BUILD(remove_animator());
                    PX_BUILD(add_animator(str));
                    animator = current->query<animator_component>();
                    if (!animator) {
                        PX_BUILD(add_animator(last)); // fallback
                    }
                }
            }
        }
        void combine_sprite(sprite_component * sprite) {
            if (!sprite) return;

            ImGui::Separator();
            ImGui::Text("sprite: %s", sprite->name);
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("texture_id: %d", sprite->texture_index);
                ImGui::Text("sx: %f, dx: %f", sprite->sx_texture, sprite->dx_texture);
                ImGui::Text("sy: %f, dy: %f", sprite->sy_texture, sprite->dy_texture);
                ImGui::EndTooltip();
            }

            ImGui::SameLine();
            if (ImGui::Button("x##remove_sprite")) {
                PX_BUILD(remove_sprite());
            }
            else {
                ImGui::InputText("##sprite_text", sprite_name.data(), sprite_name.size(), ImGuiInputTextFlags_AutoSelectAll);
                ImGui::SameLine();
                if (ImGui::Button("set##set_sprite")) {
                    std::string last(sprite->name);
                    std::string str(sprite_name.data());
                    PX_BUILD(remove_sprite());
                    PX_BUILD(add_sprite(str));
                    sprite = current->query<sprite_component>();
                    if (!sprite) {
                        PX_BUILD(add_sprite(last)); // fallback
                    }
                }
            }
        }

        void combine_body(body_component * body) {
            if (!body) return;

            ImGui::Separator();
            auto & mass = body->blocking();
            auto & movement = body->movement();
            auto & health = body->health();
            auto & energy = body->energy();
            ImGui::Text("body: %s", body->tag().c_str());
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                combine_entity(*body);
                ImGui::Text("level:       %d", body->level());
                ImGui::Text("exp:         %d", body->experience());
                ImGui::Text("faction_id:  %d", static_cast<unsigned int>(body->current_faction()));
                ImGui::Text("transparent: %s", mass.is_transparent() ? "true" : "false");
                ImGui::Text("blocking:    %s", mass.traverse_bitset().to_string().c_str());
                ImGui::Text("movement:    %s", movement.traverse_bitset().to_string().c_str());
                ImGui::Text("hp:          %s", (health ? (std::to_string(health->current()) + "/" + std::to_string(health->maximum())) : std::string{ "empty" }).c_str());
                ImGui::Text("mp:          %s", (energy ? (std::to_string(energy->current()) + "/" + std::to_string(energy->maximum())) : std::string{ "empty" }).c_str());
                ImGui::Text("useable:     %s", body->is_useable() ? "true" : "false");
                ImGui::EndTooltip();
            }
            ImGui::SameLine();
            if (ImGui::Button("x##remove_body")) {
                PX_BUILD(remove_body());
            }
            else {
                // entity
                ImGui::Text("tag: ");
                ImGui::SameLine();
                if (ImGui::InputText("##body_tag", body_tag.data(), body_tag.size() - 1, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank)) {
                    body->set_tag(body_tag.data());
                }
                ImGui::Text("name:");
                ImGui::SameLine();
                if (ImGui::InputText("##body_name", body_name.data(), body_name.size() - 1, ImGuiInputTextFlags_AutoSelectAll)) {
                    body->set_name(body_name.data());
                }
                ImGui::Text("desc:");
                ImGui::SameLine();
                if (ImGui::InputText("##body_description", body_description.data(), body_description.size() - 1, ImGuiInputTextFlags_AutoSelectAll)) {
                    body->set_description(body_description.data());
                }

                // props
                ImGui::Text("accuracy:     %f", body->accumulate({ rl::effect::accuracy }).magnitude0);
                ImGui::Text("dodge:        %f", body->accumulate({ rl::effect::dodge }).magnitude0);
                ImGui::Text("critical:     %f", body->accumulate({ rl::effect::accuracy }).magnitude0);

                // level
                if (ImGui::InputInt("level##body", &level)) {
                    body->set_level(level);
                }
                if (ImGui::InputInt("experience##body", &experience)) {
                    body->set_experience(experience);
                }

                // mass
                if (ImGui::Checkbox("transparent##body_transparent", &body_transparent)) {
                    mass.make_transparent(body_transparent);
                }
                if (ImGui::Button("set blocking")) {
                    mass.make_blocking();
                }
                ImGui::SameLine();
                if (ImGui::Button("set traversable")) {
                    mass.make_traversable();
                }

                // resource
                if (health) {
                    bool mod = false;
                    ImGui::Text("hp "); ImGui::SameLine(); mod |= ImGui::InputInt("##hp_current", &hp);
                    ImGui::Text("max"); ImGui::SameLine(); mod |= ImGui::InputInt("##hp_max", &hp_max);
                    if (mod) {
                        health->set(hp, hp_max);
                    }
                }
                else {
                    if (ImGui::Button("+ hp")) {
                        hp = 0;
                        hp_max = 0;
                        health.emplace(hp, hp_max);
                    }
                }
                if (energy) {
                    bool mod = false;
                    ImGui::Text("mp "); ImGui::SameLine(); mod |= ImGui::InputInt("##mp_current", &mp);
                    ImGui::Text("max"); ImGui::SameLine(); mod |= ImGui::InputInt("##mp_max", &mp_max);
                    if (mod) {
                        energy->set(mp, mp_max);
                    }
                }
                else {
                    if (ImGui::Button("+ mp")) {
                        mp = 0;
                        mp_max = 0;
                        energy.emplace(mp, mp_max);
                    }
                }

                combine_equipmentt(*body, rl::equipment::hand, "hand");
                combine_equipmentt(*body, rl::equipment::hide, "hide");

                if (ImGui::Button("equip##equip_first")) {
                    body->equip(0);
                }
            }
        }

        void combine_character(character_component * character) {
            if (!character) return;

            size_t skills = character->size();
            size_t traits = character->count_traits();

            ImGui::Separator();
            ImGui::Text("character");
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("book: %p", character->get_book());
                ImGui::Text("skills: %d", skills);
                ImGui::Text("traits: %d", traits);
                ImGui::EndTooltip();
            }
            ImGui::SameLine();
            if (ImGui::Button("x##remove_character")) {
                PX_BUILD(remove_character());
            }
            else {
                ImGui::Text("Traits:");
                for (size_t i = 0; i != traits; ++i) {
                    ImGui::Text("%d) %s", i, character->get_trait(i).c_str());
                    ImGui::SameLine();
                    ImGui::PushID(static_cast<int>(i));
                    if (ImGui::Button("x##remove_trait")) {
                        character->remove_trait(i);
                    }
                    ImGui::PopID();
                }

                ImGui::Text("Abilities:");
                for (size_t i = 0; i != skills; ++i) {
                    auto sk = character->get(i);
                    if (sk) {
                        auto & state = sk->state();
                        ImGui::Text("%d) %s", i, state.alias().c_str());
                        if (ImGui::IsItemHovered()) {
                            ImGui::BeginTooltip();
                            combine_entity(state);
                            ImGui::Text("cd: %d/%d", state.cooldown_remaining(), state.cooldown_duration());
                            ImGui::Text("cost: %d", state.cost());
                            ImGui::Text("hostile: %s", state.hostile() ? "true" : "false");
                            ImGui::Text("targeted: %s", sk->is_targeted() ? "true" : "false");
                            ImGui::EndTooltip();
                        }
                        ImGui::SameLine();
                        ImGui::PushID(static_cast<int>(i));
                        if (ImGui::Button("x##remove_skill")) {
                            character->remove(i);
                        }
                        ImGui::PopID();
                    }
                    else {
                        ImGui::Text("%d: null", i);
                    }
                }
                ImGui::InputText("##learn_skill_input", character_learn.data(), character_learn.size(), ImGuiInputTextFlags_AutoSelectAll);
                ImGui::SameLine();
                if (ImGui::Button("learn##skill")) {
                    character->learn(character_learn.data());
                    character_learn.fill(0);
                }
                if (skills == 0) {
                    ImGui::Text("no skills");
                }
                else {
                    if (ImGui::Button("clear##skills")) {
                        character->clear();
                    }
                }
            }
        }

        void combine_container(container_component * container) {
            if (!container) return;

            size_t size = container->item_count();
            ImGui::Separator();
            ImGui::Text("container");
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();

                ImGui::Text("items: %d", size);

                ImGui::EndTooltip();
            }
            ImGui::SameLine();
            if (ImGui::Button("x##remove_container")) {
                PX_BUILD(remove_container());
            }
            else {

                ImGui::InputText("tag##create_item", item_tag.data(), item_tag.size(), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank);
                ImGui::InputText("name##create_item", item_name.data(), item_name.size(), ImGuiInputTextFlags_AutoSelectAll);
                ImGui::InputText("description##create_item", item_description.data(), item_description.size());
                ImGui::InputInt("stack##item_stack_limits", &item_stack);
                ImGui::InputInt("maximum##item_stack_limit", &item_maximum);

                ImGui::InputFloat("damage##create_item", &item_damage);
                ImGui::InputFloat("ingredient power##create_item", &item_reagent_power);
                ImGui::InputInt("ingredient essence##create_item", &item_reagent_essence);

                if (ImGui::Button("+ item##create_item")) {
                    auto & item = container->add(make_uq<rl::item>());
                    setup_item(*item);
                }
                if (ImGui::Button("+ weapon##create_item")) {
                    auto & item = container->add(make_uq<rl::item>());
                    setup_item(*item);

                    item->add(rl::item::enhancement_type::real(rl::effect::damage, 0x00, item_damage));
                    item->add(rl::item::enhancement_type::zero(rl::effect::equipment, static_cast<rl::item::enhancement_type::integer_type>(rl::equipment::hand)));
                }
                if (ImGui::Button("+ ore##create_item")) {
                    auto & item = container->add(make_uq<rl::item>());
                    setup_item(*item);

                    item->add(rl::item::enhancement_type::real(rl::effect::ingredient_power, static_cast<int>(rl::craft_activity::blacksmith), item_reagent_power));
                    item->add(rl::item::enhancement_type::integral(rl::effect::essence, 0x00, item_reagent_essence));
                }

                // print contained items
                if (size != 0) {
                    for (size_t i = 0; i != size; ++i) {
                        rl::item * ptr = container->get(i);
                        if (ptr) {
                            ImGui::Text("%d) ", i);
                            ImGui::SameLine();
                            combine_item(*ptr);
                            ImGui::SameLine();
                            ImGui::PushID(static_cast<int>(i));
                            if (ImGui::Button("x")) {
                                container->remove(i);
                                i = 0;
                            }
                            ImGui::PopID();
                        }
                    }
                }

                // add item props to last item
                if (auto it = container->get<0>()) {
                    if (!it->has_effect(rl::effect::damage)) {
                        if (ImGui::Button("+ damage##add_item_prop")) {
                            it->add(rl::item::enhancement_type::real(rl::effect::damage, 0x00, item_damage));
                        }
                    }

                    ImGui::InputInt("effect##item_prop", &item_effect);
                    ImGui::InputInt("subtype##item_prop", &item_effect_subtype);
                    ImGui::InputInt("value##item_prop", &item_effect_value);
                    ImGui::InputFloat("magnitude##item_prop", &item_effect_magnitude);

                    if (ImGui::Button("+ prop##add_item_prop")) {
                        it->add(rl::item::enhancement_type::mixed(static_cast<rl::effect>(item_effect), item_effect_subtype, item_effect_value, item_effect_magnitude));
                    }
                }
            }
        }
        void combine_deposite(deposite_component * deposite) {
            if (!deposite) return;

            ImGui::Separator();
            ImGui::Text("deposite");
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("dissolve:  %s", deposite->is_dissolving() ? "true" : "false");
                ImGui::Text("use timer: %d", deposite->get_use_duration());
                ImGui::EndTooltip();
            }
            ImGui::SameLine();
            if (ImGui::Button("x##remove_deposit")) {
                PX_BUILD(remove_deposite());
            }
            else {
                if (ImGui::Checkbox("dissolve##deposit_dissolve", &deposit_dissolve)) {
                    deposite->set_dissolve(deposit_dissolve);
                }
            }
        }
        void combine_door(door_component * door) {
            if (!door) return;

            ImGui::Separator();
            ImGui::Text("door");
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("is opened:    %s", door->is_opened() ? "true" : "false");
                ImGui::Text("use duration: %d", door->use_duration());
                ImGui::Text("animator:     %p", door->linked<animator_component>());
                ImGui::EndTooltip();
            }
            ImGui::SameLine();
            if (ImGui::Button("x##remove_door")) {
                PX_BUILD(remove_door());
            }
            else {
                animator_component * animator = door->linked<animator_component>();
                if (!animator) {
                    ImGui::Text("WARNING: no animator!");
                }
                else if (animator->size() == 0) {
                    ImGui::Text("WARNING: animator has no animations!");
                }
                if (!door->linked<body_component>()) {
                    ImGui::Text("WARNING: no body!");
                }

                if (ImGui::Checkbox("opened##door_opened", &door_open)) {
                    door->set_open(door_open);
                }
            }
        }
        void combine_player(player_component * player) {
            if (!player) return;

            ImGui::Separator();
            ImGui::Text("player");
            ImGui::SameLine();
            if (ImGui::Button("x##remove_player")) {
                PX_BUILD(remove_player());
            }
        }
        void combine_npc(npc_component * npc) {
            if (!npc) return;

            ImGui::Separator();
            ImGui::Text("npc");
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("state:       %d", npc_state);
                ImGui::Text("range:       %d, %d", npc_range_idle, npc_range_alert);
                ImGui::Text("destination: (%d, %d)", npc_waypoint_x, npc_waypoint_y);
                ImGui::EndTooltip();
            }
            ImGui::SameLine();
            if (ImGui::Button("x##remove_npc")) {
                PX_BUILD(remove_npc());
            }
            else {
                if (ImGui::InputInt("idle##npc_range", &npc_range_idle)) npc->set_range(npc_range_idle, npc_range_alert);
                if (ImGui::InputInt("alert##npc_range", &npc_range_alert)) npc->set_range(npc_range_idle, npc_range_alert);
            }
        }
        void combine_light(light_component * light) {
            if (!light) return;
            ImGui::Separator();
            ImGui::Text("light");
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("is on:       %s", light->is_on ? "true" : "false");
                ImGui::Text("color:       rgba(%f, %f, %f, %f)", light->tint.R, light->tint.G, light->tint.B, light->tint.A);
                ImGui::Text("elevation:   %f", light->elevation);
                ImGui::Text("source type: %d", light->source);
                ImGui::EndTooltip();
            }
            ImGui::SameLine();
            if (ImGui::Button("x##remove_light")) {
                PX_BUILD(remove_light());
            }
            else {
                if (ImGui::ColorPicker4("color##picker", &light_tint[0], ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview)) {
                    light_tint[3] = 1; // alpha
                    light->tint.R = light_tint[0];
                    light->tint.G = light_tint[1];
                    light->tint.B = light_tint[2];
                    light->tint.A = 1;
                }
                if (ImGui::Checkbox("on##light", &light_on)) {
                    light->is_on = light_on;
                }
                if (ImGui::InputFloat("elevation##light", &light_elevation)) {
                    light->elevation = light_elevation;
                }
                if (ImGui::InputInt("source##light_type", &light_type)) {
                    light->source = static_cast<light_source>(light_type);
                }
            }
        }
        void combine_workshop(workshop_component * workshop) {
            if (!workshop) return;

            ImGui::Separator();
            ImGui::Text("workshop");
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("variant: %d", static_cast<std::underlying_type_t<rl::craft_activity>>(workshop->activity()));
                ImGui::EndTooltip();
            }
            ImGui::SameLine();
            if (ImGui::Button("x##remove_workshop")) {
                PX_BUILD(remove_workshop());
            }
            else {
                if (ImGui::InputInt("variant##workshop_variant", &workshop_variant)) {
                    workshop->set_activity(static_cast<rl::craft_activity>(workshop_variant));
                }
                if (ImGui::Button("blacksmith##set_workshop")) {
                    workshop_variant = static_cast<int>(rl::craft_activity::blacksmith);
                    workshop->set_activity(rl::craft_activity::blacksmith);
                }
                if (ImGui::Button("alchemy##set_workshop")) {
                    workshop_variant = static_cast<int>(rl::craft_activity::alchemy);
                    workshop->set_activity(rl::craft_activity::alchemy);
                }
            }
        }

        void combine_entity(rl::entity const& subject) {
            ImGui::Text("tag:         '%s'", subject.tag().c_str());
            ImGui::Text("name:        '%s'", subject.name().c_str());
            ImGui::Text("description: '%s'", subject.description().c_str());
        }

        void combine_equipmentt(body_component & body, rl::equipment slot, std::string const& slot_name) {
            if (auto i = body.equipment(slot)) {
                ImGui::Text((slot_name + ": ").c_str());
                ImGui::SameLine();
                combine_item(*i);
                ImGui::SameLine();
                if (ImGui::Button(("x##unequip_" + std::to_string(static_cast<int>(slot))).c_str())) {
                    body.unequip(slot);
                }
            }
        }

        void combine_item(rl::item const& item) {
            ImGui::Text("%s x%d", item.tag().c_str(), item.count());
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                combine_entity(item);
                ImGui::Text("stack:     [%d/%d]", item.count(), item.maximum());
                ImGui::Text("props size:      %d", item.size());
                if (item.has_effect(rl::effect::power)) {
                    ImGui::Text("power:     %f", item.accumulate(rl::item::enhancement_type::zero(rl::effect::power)).magnitude0);
                }
                if (item.has_effect(rl::effect::damage)) {
                    ImGui::Text("damage:    %f", item.accumulate(rl::item::enhancement_type::zero(rl::effect::damage)).magnitude0);
                }
                if (item.has_effect(rl::effect::hp_heal)) {
                    ImGui::Text("hp bonus:  %f", item.accumulate(rl::item::enhancement_type::zero(rl::effect::hp_heal)).magnitude0);
                }
                if (item.has_effect(rl::effect::ingredient_power)) {
                    ImGui::Text("ingredient power: %f", item.accumulate(rl::item::enhancement_type::zero(rl::effect::ingredient_power)).magnitude0);
                }
                if (item.has_effect(rl::effect::essence)) {
                    ImGui::Text("essence:   %d", item.accumulate(rl::item::enhancement_type::zero(rl::effect::essence)).value0);
                }
                ImGui::EndTooltip();
            }
        }

        void load_schema(std::string const& schema_name) {
            if (!game) return;

            builder factory(game);
            current = schema::load(document::load_document(settings::schemata_path + schema_name), factory);
            update_props();
        }
        void load_blueprint(std::string const& blueprint_name) {
            if (!game) return;

            builder factory(game);
            auto input = input_stream(settings::blueprints_path + blueprint_name);
            current = blueprint::assemble(SAVE_INPUT_ARCHIVE(input), factory);
            update_props();
        }
        void export_composite() {
            auto output = output_stream(settings::blueprints_path + current->name() + ".dat");
            blueprint::store(SAVE_OUTPUT_ARCHIVE(output), *current);
        }
        void refresh_template_items() {
            load_items(blueprint_selected, blueprints, settings::blueprints_path);
            load_items(schema_selected, schemata, settings::schemata_path);
        }
        static void load_items(int & selected, std::vector<std::string> & names, std::string const& path) {
            namespace fs = std::experimental::filesystem;

            selected = -1;
            names.clear();
            for (auto const& entry : fs::directory_iterator(path)) {
                if (fs::is_regular_file(entry.path())) {
                    names.push_back(entry.path().filename().string());
                }
            }
        }
        void load_current_schema() {
            if (schema_selected >= 0) {
                load_schema(schemata[schema_selected]);
            }
        }
        void load_current_blueprint() {
            if (blueprint_selected >= 0) {
                load_blueprint(blueprints[blueprint_selected]);
            }
        }
        void update_props() {
            if (current) {
                current->enable();

                composite_lifetime = static_cast<int>(current->lifetime());
                copy_str(current->name(), composite_name);

                if (auto transform = current->query<transform_component>()) {
                    auto pos = transform->position();
                    transform_x = pos.x();
                    transform_y = pos.y();
                    transform_static = transform->is_static();
                }
                if (auto sprite = current->query<sprite_component>()) {
                    copy_str(sprite->name, sprite_name);
                }
                if (auto animator = current->query<animator_component>()) {
                    copy_str(animator->get_id(), animator_name);
                    animator_playing = animator->is_playing();
                }
                if (auto light = current->query<light_component>()) {
                    light_tint[0] = static_cast<float>(light->tint.R);
                    light_tint[1] = static_cast<float>(light->tint.G);
                    light_tint[2] = static_cast<float>(light->tint.B);
                    light_tint[3] = static_cast<float>(light->tint.A);
                    light_elevation = static_cast<float>(light->elevation);
                    light_on = light->is_on;
                    light_type = static_cast<int>(light->source);
                }
                if (auto body = current->query<body_component>()) {
                    copy_str(body->name(), body_name);
                    copy_str(body->tag(), body_tag);
                    copy_str(body->description(), body_description);

                    body_transparent = body->blocking().is_transparent();
                    auto const& health = body->health();
                    auto const& energy = body->energy();
                    if (health) {
                        hp = health->current();
                        hp_max = health->maximum();
                    }
                    if (energy) {
                        mp = energy->current();
                        mp_max = energy->maximum();
                    }

                    level = body->level();
                    experience = body->experience();
                }
                if (auto deposit = current->query<deposite_component>()) {
                    deposit_dissolve = deposit->is_dissolving();
                }
                if (auto door = current->query<door_component>()) {
                    door_open = door->is_opened();
                }
                if (auto workshop = current->query<workshop_component>()) {
                    workshop_variant = static_cast<std::underlying_type<rl::craft_activity>::type>(workshop->activity());
                }

                // character
                character_learn.fill(0);

                if (auto npc = current->query<npc_component>()) {
                    npc_state = static_cast<int>(npc->get_state());
                    auto ranges = npc->get_ranges();
                    npc_range_idle = static_cast<int>(std::get<0>(ranges));
                    npc_range_alert = static_cast<int>(std::get<1>(ranges));
                    npc_waypoint_x = npc->destination().x();
                    npc_waypoint_y = npc->destination().y();
                }

                // item
                item_tag.fill(0);
                item_description.fill(0);
                item_name.fill(0);
                item_stack = 1;
                item_maximum = -1;
                item_effect = 0;
                item_effect_subtype = 0;
                item_effect_value = 0;
                item_effect_magnitude = 0;
                item_damage = 0;
                item_reagent_power = 0;
                item_reagent_essence = 0;
            }
        }

        // assign basic values of an item
        void setup_item(rl::item & it) {
            it.set_tag(item_tag.data());
            it.set_name(item_name.data());
            it.set_description(item_description.data());
            it.set_maximum_stack(item_maximum);
            it.set_current_stack(item_stack);
        }

        // copy string to char array
        template <size_t Max>
        void copy_str(std::string str, std::array<char, Max> & ar) {
            static_assert(Max > 1);
            ar.fill(0);
            std::copy(str.cbegin(), str.cbegin() + std::min(str.length(), ar.size() - 2), ar.begin()); // reserve extra zero for end of a string
        }

    private:
        uq_ptr<composite_component> current;
        environment *               game;

        int                         schema_selected;
        std::vector<std::string>    schemata;
        int                         blueprint_selected;
        std::vector<std::string>    blueprints;

        std::array<char, 128>       composite_name;
        int                         composite_lifetime;

        int                         transform_x;
        int                         transform_y;
        bool                        transform_static;

        std::array<char, 128>       sprite_name;

        std::array<char, 128>       body_tag;
        std::array<char, 128>       body_name;
        std::array<char, 1024>      body_description;
        bool                        body_transparent;
        int                         hp;
        int                         hp_max;
        int                         mp;
        int                         mp_max;
        int                         level;
        int                         experience;

        bool                        deposit_dissolve;
        bool                        door_open;

        std::array<char, 128>       animator_name;
        bool                        animator_playing;

        std::array<char, 128>       character_learn;

        int                         npc_state;
        int                         npc_range_idle;
        int                         npc_range_alert;
        int                         npc_waypoint_x;
        int                         npc_waypoint_y;

        std::array<char, 128>       item_tag;
        std::array<char, 128>       item_name;
        std::array<char, 1024>      item_description;
        int                         item_stack;
        int                         item_maximum;

        float                       item_damage;
        int                         item_effect;
        int                         item_effect_subtype;
        int                         item_effect_value;
        float                       item_effect_magnitude;
        float                       item_reagent_power;
        int                         item_reagent_essence;

        float                       light_tint[4];
        float                       light_elevation;
        bool                        light_on;
        int                         light_type;

        int                         workshop_variant;
    };
}

#undef PX_BUILD