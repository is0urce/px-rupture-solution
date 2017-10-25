// name: editor.hpp
// type: c++

#pragma once

#include "panel.hpp"

#include "rupture/app/document.hpp"
#include "rupture/app/settings.hpp"
#include "rupture/es/builder.hpp"
#include "rupture/es/composite_component.hpp"
#include "rupture/es/character_component.hpp"
#include "rupture/es/container_component.hpp"
#include "rupture/es/sprite_component.hpp"
#include "rupture/es/transform_component.hpp"
#include "rupture/io/schema.hpp"
#include "rupture/io/blueprint.hpp"
#include "rupture/io/serialization.hpp"
#include "rupture/environment.hpp"

#include <imgui/imgui.h>

#include <algorithm>
#include <array>
#include <filesystem>
#include <string>
#include <vector>

#define PX_BUILD(OP) { builder factory(game); factory.begin(std::move(current)); factory.OP; current = factory.request(); update_props(); }
#define PX_SWAP(A, B) { builder factory(game); factory.begin(std::move(current)); factory.A, factory.B; current = factory.request(); update_props(); }

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

				// composite

				ImGui::Separator();
				ImGui::Text("composite");
				if (ImGui::IsItemHovered()) {
					ImGui::BeginTooltip();
					ImGui::Text("name: %s", current->name().c_str());
					ImGui::Text("size: %d", current->size());
					ImGui::Text("persistency: %d", static_cast<unsigned int>(current->lifetime()));
					ImGui::Separator();
					ImGui::Text("adress: %p", current.get());
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

				ImGui::Separator();
				if (transform) {
					ImGui::Text("transform");
					if (ImGui::IsItemHovered()) {
						ImGui::BeginTooltip();
						ImGui::Text("space: %p", transform->world());
						ImGui::Text("current: (%d, %d)", transform->position().x(), transform->position().y());
						ImGui::Text("last: (%d, %d)", transform->last().x(), transform->last().y());
						ImGui::Separator();
						ImGui::Text("adress: %p", transform);
						ImGui::EndTooltip();
					}
					ImGui::SameLine();
					if (ImGui::Button("remove##remove_transform")) {
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
				else if (ImGui::Button("+ transform")) {
					PX_BUILD(add_transform({ 0, 0 }));
				}

				combine_sprite();
				combine_body();
				combine_character();
				combine_container();

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
		void combine_sprite()
		{
			sprite_component * sprite = current->query<sprite_component>();
			ImGui::Separator();
			if (sprite) {
				ImGui::Text("sprite: %s", sprite->name);
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Text("texture_id: %d", sprite->texture_index);
					ImGui::Text("sx: %f, dx: %f", sprite->sx_texture, sprite->dx_texture);
					ImGui::Text("sy: %f, dy: %f", sprite->sy_texture, sprite->dy_texture);
					ImGui::EndTooltip();
				}

				ImGui::SameLine();
				if (ImGui::Button("remove##remove_sprite")) {
					PX_BUILD(remove_sprite());
				}
				else {
					ImGui::InputText("##sprite_text", sprite_name.data(), sprite_name.size(), ImGuiInputTextFlags_AutoSelectAll);
					ImGui::SameLine();
					if (ImGui::Button("set##set_sprite")) {
						std::string str(sprite_name.data());
						PX_SWAP(remove_sprite(), add_sprite(str));
					}
				}
			}
			else if (ImGui::Button("+ sprite")) {
				PX_BUILD(add_sprite("x_dummy"));
			}
		}
		void combine_body()
		{
			body_component * body = current->query<body_component>();
			ImGui::Separator();
			if (body) {
				auto & mass = body->blocking();
				auto & movement = body->movement();
				auto & health = body->health();
				auto & energy = body->energy();
				ImGui::Text("body: %s", body->tag().c_str());
				if (ImGui::IsItemHovered()) {
					ImGui::BeginTooltip();
					ImGui::Text("tag: %s", body->tag().c_str());
					ImGui::Text("name: %s", body->name().c_str());
					ImGui::Text("description: %s", body->description().c_str());
					ImGui::Text("faction_id: %d", static_cast<unsigned int>(body->current_faction()));
					ImGui::Text("transparent: %s", mass.is_transparent() ? "true" : "false");
					ImGui::Text("blocking: %s", mass.traverse_bitset().to_string().c_str());
					ImGui::Text("movement: %s", movement.traverse_bitset().to_string().c_str());
					ImGui::Text("hp: %s", (health ? (std::to_string(health->current()) + "/" + std::to_string(health->maximum())) : std::string{ "empty" }).c_str());
					ImGui::Text("mp: %s", (energy ? (std::to_string(energy->current()) + "/" + std::to_string(energy->maximum())) : std::string{ "empty" }).c_str());
					ImGui::EndTooltip();
				}
				ImGui::SameLine();
				if (ImGui::Button("remove##remove_body")) {
					PX_BUILD(remove_body());
				}
				else {

					ImGui::Text("tag: ");
					ImGui::SameLine();
					if (ImGui::InputText("##body_tag", body_tag.data(), body_tag.size() - 1, ImGuiInputTextFlags_AutoSelectAll)) {
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
				}
			}
			else if (ImGui::Button("+ body")) {
				PX_BUILD(add_body());
			}
		}
		void combine_character()
		{
			character_component * character = current->query<character_component>();
			ImGui::Separator();
			if (character) {
				ImGui::Text("character");
				if (ImGui::IsItemHovered()) {
					ImGui::BeginTooltip();
					ImGui::Text("adress: %p", character);
					ImGui::Separator();
					ImGui::EndTooltip();
				}
				ImGui::SameLine();
				if (ImGui::Button("remove##remove_character")) {
					PX_BUILD(remove_character());
				}
			}
			else if (ImGui::Button("+ character")) {
				PX_BUILD(add_character());
			}
		}
		void combine_container()
		{
			container_component * container = current->query<container_component>();
			ImGui::Separator();
			if (container) {
				ImGui::Text("container");
				if (ImGui::IsItemHovered()) {
					ImGui::BeginTooltip();
					ImGui::Text("adress: %p", container);
					ImGui::Separator();
					ImGui::EndTooltip();
				}
				ImGui::SameLine();
				if (ImGui::Button("remove##remove_container")) {
					PX_BUILD(remove_container());
				}
			}
			else if (ImGui::Button("+ container")) {
				PX_BUILD(add_container());
			}
		}
		void load_schema(std::string const& schema_name)
		{
			if (game) {
				builder factory(game);
				current = schema::load(document::load_document(settings::schemata_path + schema_name), factory);
				update_props();
			}
		}
		void load_blueprint(std::string const& blueprint_name)
		{
			if (game) {
				builder factory(game);
				auto input = input_stream(settings::blueprints_path + blueprint_name);
				current = blueprint::load(SAVE_INPUT_ARCHIVE(input), factory);
				update_props();
			}
		}
		void export_composite()
		{
			auto output = output_stream(settings::blueprints_path + current->name() + ".dat");
			blueprint::save(SAVE_OUTPUT_ARCHIVE(output), *current);
		}
		void refresh_template_items()
		{
			load_items(blueprint_selected, blueprints, settings::blueprints_path);
			load_items(schema_selected, schemata, settings::schemata_path);
		}
		static void load_items(int & selected, std::vector<std::string> & names, std::string const& path)
		{
			namespace fs = std::experimental::filesystem;

			names.clear();
			for (fs::directory_entry const& entry : fs::directory_iterator(path)) {
				if (fs::is_regular_file(entry.path())) {
					names.push_back(entry.path().filename().string());
				}
			}

			selected = -1;
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
				if (auto body = current->query<body_component>()) {
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
					copy_str(body->name(), body_name);
					copy_str(body->tag(), body_tag);
					copy_str(body->description(), body_description);
				}
			}
		}
		template <size_t max>
		void copy_str(std::string str, std::array<char, max> & ar)
		{
			static_assert(max > 2);
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
		int							hp;
		int							hp_max;
		int							mp;
		int							mp_max;
		bool						body_transparent;
	};
}