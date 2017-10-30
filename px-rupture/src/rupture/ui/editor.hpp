// name: editor.hpp
// type: c++

#pragma once

#include "panel.hpp"

#include "rupture/app/document.hpp"
#include "rupture/app/settings.hpp"
#include "rupture/es/builder.hpp"
#include "rupture/es/animator_component.hpp"
#include "rupture/es/composite_component.hpp"
#include "rupture/es/character_component.hpp"
#include "rupture/es/container_component.hpp"
#include "rupture/es/deposit_component.hpp"
#include "rupture/es/player_component.hpp"
#include "rupture/es/npc_component.hpp"
#include "rupture/es/sprite_component.hpp"
#include "rupture/es/transform_component.hpp"
#include "rupture/io/schema.hpp"
#include "rupture/io/blueprint.hpp"
#include "rupture/io/serialization.hpp"
#include "rupture/environment.hpp"

#include <imgui/imgui.h>

#include <px/memory/memory.hpp>

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
		virtual ~editor()
		{
		}
		editor(environment * env)
			: game(env)
		{
			refresh_template_items();
		}

	protected:
		virtual void combine_panel() override
		{
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
			if (ImGui::Button("load###load_schema")) {
				load_current_schema();
			}

			ImGui::Combo("##blueprints_list", &blueprint_selected, vector_getter, static_cast<void*>(&blueprints), static_cast<int>(blueprints.size()));
			ImGui::SameLine();
			if (ImGui::Button("load###load_blueprint")) {
				load_current_blueprint();
			}

			if (ImGui::Button("pick###pick_unit")) {
			}
			ImGui::SameLine();
			if (ImGui::Button("refresh###refresh_templates")) {
				refresh_template_items();
			}

			// object props

			if (current) {

				transform_component * transform = current->query<transform_component>();
				sprite_component * sprite = current->query<sprite_component>();
				body_component * body = current->query<body_component>();
				character_component * character = current->query<character_component>();
				container_component * container = current->query<container_component>();

				deposite_component * deposit = current->query<deposite_component>();
				animator_component * animator = current->query<animator_component>();
				player_component * player = current->query<player_component>();
				npc_component * npc = current->query<npc_component>();

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
						ImGui::Text("current: (%d, %d)", transform->position().x(), transform->position().y());
						ImGui::Text("last: (%d, %d)", transform->last().x(), transform->last().y());
						//ImGui::Separator();
						//ImGui::Text("adress: %p", transform);
						ImGui::EndTooltip();
					}
					ImGui::SameLine();
					if (ImGui::Button("x##remove_transform")) {
						PX_BUILD(remove_transform());
					}
					else {
						bool place = false;
						ImGui::Text("x"); ImGui::SameLine(); place |= ImGui::InputInt("##transform_x", &transform_x);
						ImGui::Text("y"); ImGui::SameLine(); place |= ImGui::InputInt("##transform_y", &transform_y);
						if (place) {
							transform->place({ transform_x, transform_y });
						}
					}
				}

				combine_sprite(sprite);
				combine_animator(animator);
				combine_body(body);
				combine_character(character);
				combine_container(container);
				combine_deposit(deposit);
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
					if (!body && ImGui::MenuItem("body##add")) {
						PX_BUILD(add_body());
					}
					if (!character && ImGui::MenuItem("character##add")) {
						PX_BUILD(add_character());
					}
					if (!container && ImGui::MenuItem("container##add")) {
						PX_BUILD(add_container());
					}
					if (!deposit) {
						if (ImGui::BeginMenu("useables..##add")) {
							if (ImGui::MenuItem("deposit##add")) {
								PX_BUILD(add_deposite());
							}
							ImGui::MenuItem("door##add");
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
						game->spawn(std::move(current), transform);
					}

					if (camera) {
						ImGui::SameLine();
						if (ImGui::Button("here##spawn_here")) {
							transform->place(camera->position());
							game->spawn(std::move(current), transform);
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

			ImGui::End();
		}

	private:
		void combine_animator(animator_component * animator)
		{
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
		void combine_sprite(sprite_component * sprite)
		{
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
		void combine_body(body_component * body)
		{
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

				if (auto i = body->equipment(rl::equipment::hand)) {
					ImGui::Text("hand: ");
					ImGui::SameLine();
					combine_item(*i);
					ImGui::SameLine();
					if (ImGui::Button("x##unequip_hand")) {
						body->unequip(rl::equipment::hand);
					}
				}

				if (ImGui::Button("equip##equip_first")) {
					body->equip(0);
				}
			}
		}
		void combine_character(character_component * character)
		{
			if (!character) return;

			size_t size = character->size();

			ImGui::Separator();
			ImGui::Text("character");
			if (ImGui::IsItemHovered()) {
				ImGui::BeginTooltip();
				ImGui::Text("book: %p", character->get_book());
				ImGui::Text("size: %d", size);
				ImGui::EndTooltip();
			}
			ImGui::SameLine();
			if (ImGui::Button("x##remove_character")) {
				PX_BUILD(remove_character());
			}
			else {
				for (size_t i = 0; i != size; ++i) {
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
						if (ImGui::Button("x##remove_skill")) {
							character->remove(i);
						}
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
				if (size == 0) {
					ImGui::Text("no skills");
				}
				else {
					if (ImGui::Button("clear##skills")) {
						character->clear();
					}
				}
			}
		}
		void combine_container(container_component * container)
		{
			if (!container) return;

			size_t size = container->size();
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

				ImGui::InputText("tag##create_item", item_tag.data(), item_tag.size(), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank);
				ImGui::InputText("name##create_item", item_name.data(), item_name.size(), ImGuiInputTextFlags_AutoSelectAll);
				ImGui::InputText("description##create_item", item_description.data(), item_description.size());

				//ImGui::InputFloat("power##create_item", &item_power);
				ImGui::InputFloat("damage##create_item", &item_damage);
				//ImGui::InputFloat("hp##create_item", &item_hp_bonus);
				ImGui::InputFloat("ore power##create_item", &item_ore_power);
				ImGui::InputInt("essence##create_item", &item_essence);

				if (ImGui::Button("+ weapon##item")) {
					auto & item = container->add(make_uq<rl::item>());
					item->set_tag(item_tag.data());
					item->set_name(item_name.data());
					item->set_description(item_description.data());

					item->add(rl::item::enhancement_type::real(rl::effect::damage, 0, item_damage, item_damage));
				}
				if (ImGui::Button("+ ore##item")) {
					auto & item = container->add(make_uq<rl::item>());
					item->set_tag(item_tag.data());
					item->set_name(item_name.data());
					item->set_description(item_description.data());

					item->add(rl::item::enhancement_type::real(rl::effect::ingredient_power, 0, item_power, 0));
					item->add(rl::item::enhancement_type::integral(rl::effect::essence, 0, item_essence, 0));
				}
			}
		}
		void combine_deposit(deposite_component * deposit)
		{
			if (!deposit) return;

			ImGui::Separator();
			ImGui::Text("deposite");
			ImGui::SameLine();
			if (ImGui::Button("x##remove_deposit")) {
				PX_BUILD(remove_deposite());
			}
			else {
				if (ImGui::Checkbox("dissolve##deposit_dissolve", &deposit_dissolve)) {
					deposit->set_dissolve(deposit_dissolve);
				}
			}
		}
		void combine_player(player_component * player)
		{
			if (!player) return;

			ImGui::Separator();
			ImGui::Text("player");
			ImGui::SameLine();
			if (ImGui::Button("x##remove_player")) {
				PX_BUILD(remove_player());
			}
		}
		void combine_npc(npc_component * npc)
		{
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
		void combine_entity(rl::entity const& subject) {
			ImGui::Text("tag:         '%s'", subject.tag().c_str());
			ImGui::Text("name:        '%s'", subject.name().c_str());
			ImGui::Text("description: '%s'", subject.description().c_str());
		}
		void combine_item(rl::item const& item) {
			ImGui::Text("%s x%d", item.tag().c_str(), item.count());
			if (ImGui::IsItemHovered()) {
				ImGui::BeginTooltip();
				combine_entity(item);
				ImGui::Text("stack:     [%d/%d]", item.count(), item.maximum());
				ImGui::Text("size:      %d", item.size());
				ImGui::Text("power:     %f", item.accumulate(rl::item::enhancement_type::zero(rl::effect::power)).magnitude0);
				ImGui::Text("damage:    %f", item.accumulate(rl::item::enhancement_type::zero(rl::effect::damage)).magnitude0);
				ImGui::Text("hp bonus:  %f", item.accumulate(rl::item::enhancement_type::zero(rl::effect::hp_bonus)).magnitude0);
				ImGui::Text("ore power: %f", item.accumulate(rl::item::enhancement_type::zero(rl::effect::ingredient_power)).magnitude0);
				ImGui::Text("essence:   %d", item.accumulate(rl::item::enhancement_type::zero(rl::effect::essence)).value0);
				ImGui::EndTooltip();
			}
		}
		void load_schema(std::string const& schema_name)
		{
			if (!game) return;

			builder factory(game);
			current = schema::load(document::load_document(settings::schemata_path + schema_name), factory);
			update_props();
		}
		void load_blueprint(std::string const& blueprint_name)
		{
			if (!game) return;

			builder factory(game);
			auto input = input_stream(settings::blueprints_path + blueprint_name);
			current = blueprint::assemble(SAVE_INPUT_ARCHIVE(input), factory);
			update_props();
		}
		void export_composite()
		{
			auto output = output_stream(settings::blueprints_path + current->name() + ".dat");
			blueprint::store(SAVE_OUTPUT_ARCHIVE(output), *current);
		}
		void refresh_template_items()
		{
			load_items(blueprint_selected, blueprints, settings::blueprints_path);
			load_items(schema_selected, schemata, settings::schemata_path);
		}
		static void load_items(int & selected, std::vector<std::string> & names, std::string const& path)
		{
			namespace fs = std::experimental::filesystem;

			selected = -1;
			names.clear();
			for (fs::directory_entry const& entry : fs::directory_iterator(path)) {
				if (fs::is_regular_file(entry.path())) {
					names.push_back(entry.path().filename().string());
				}
			}
		}
		void load_current_schema()
		{
			if (schema_selected >= 0) {
				load_schema(schemata[schema_selected]);
			}
		}
		void load_current_blueprint()
		{
			if (blueprint_selected >= 0) {
				load_blueprint(blueprints[blueprint_selected]);
			}
		}
		void update_props()
		{
			if (current) {
				current->enable();

				composite_lifetime = static_cast<int>(current->lifetime());
				copy_str(current->name(), composite_name);

				if (auto transform = current->query<transform_component>()) {
					auto pos = transform->position();
					transform_x = pos.x();
					transform_y = pos.y();
				}
				if (auto sprite = current->query<sprite_component>()) {
					copy_str(sprite->name, sprite_name);
				}
				if (auto animator = current->query<animator_component>()) {
					copy_str(animator->get_id(), animator_name);
					animator_playing = animator->is_playing();
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
					deposit_dissolve = deposit->dissolving();
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
				item_power = 0;
				item_damage = 0;
				item_hp_bonus = 0;
				item_accuracy = 0;
				item_critical = 0;
				item_ore_power = 0;
				item_essence = 0;
			}
		}
		template <size_t max>
		void copy_str(std::string str, std::array<char, max> & ar)
		{
			static_assert(max > 1);
			ar.fill(0);
			std::copy(str.cbegin(), str.cbegin() + std::min(str.length(), ar.size() - 2), ar.begin()); // reserve extra zero for end of a string
		}

	private:
		environment *				game;
		uq_ptr<composite_component>	current;

		int							schema_selected;
		std::vector<std::string>	schemata;
		int							blueprint_selected;
		std::vector<std::string>	blueprints;

		std::array<char, 128>		composite_name;
		int							composite_lifetime;

		int							transform_x;
		int							transform_y;

		std::array<char, 128>		sprite_name;

		std::array<char, 128>		body_tag;
		std::array<char, 128>		body_name;
		std::array<char, 1024>		body_description;
		bool						body_transparent;
		int							hp;
		int							hp_max;
		int							mp;
		int							mp_max;
		int							level;
		int							experience;

		bool						deposit_dissolve;

		std::array<char, 128>		animator_name;
		bool						animator_playing;

		std::array<char, 128>		character_learn;

		int							npc_state;
		int							npc_range_idle;
		int							npc_range_alert;
		int							npc_waypoint_x;
		int							npc_waypoint_y;

		std::array<char, 128>		item_tag;
		std::array<char, 128>		item_name;
		std::array<char, 1024>		item_description;
		float						item_power;
		float						item_damage;
		float						item_hp_bonus;
		float						item_accuracy;
		float						item_critical;
		float						item_ore_power;
		int							item_essence;
	};
}