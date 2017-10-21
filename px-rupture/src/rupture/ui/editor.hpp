// name: editor.hpp
// type: c++

#pragma once

#include "panel.hpp"

#include "rupture/app/document.hpp"
#include "rupture/es/builder.hpp"
#include "rupture/es/composite_component.hpp"
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

	inline bool vector_getter(void * data, int n, const char** result)
	{
		auto & vector = *static_cast<std::vector<std::string>*>(data);
		if (n >= 0 && n < static_cast<int>(vector.size())) {
			*result = vector[n].c_str();
			return true;
		}
		return false;
	}

	class editor : public panel
	{
	public:
		void assign_environment(environment * env) noexcept
		{
			game = env;
		}

	public:
		virtual ~editor()
		{
		}
		//editor() noexcept
		//	: editor(nullptr)
		//{
		//}
		editor(environment * e) noexcept
			: game(e)
		{
			refresh_template_items();
		}

	protected:
		virtual void combine_panel() override
		{
			static bool metrics = false;
			static bool dbg = false;

			ImGui::SetNextWindowSize({ 150, 100 });
			ImGui::SetNextWindowPos({ 50, 50 });
			ImGui::Begin("performance", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
			if (ImGui::Button("dear imgui metrics")) {
				metrics = !metrics;
			}
			if (ImGui::Button("test window")) {
				dbg = !dbg;
			}
			ImGui::End();

			if (dbg) {
				ImGui::ShowTestWindow();
			}
			if (metrics) {
				ImGui::ShowMetricsWindow(&metrics);
			}

			// editor
			if (!game) return;

			transform_component * camera = game->possessed();

			const float window_width = 250.0f;
			const float screen_width = ImGui::GetIO().DisplaySize.x;
			const float screen_height = ImGui::GetIO().DisplaySize.y;
			ImGui::SetNextWindowPos({ screen_width - window_width, 0 }, ImGuiCond_Always);
			ImGui::SetNextWindowSize({ window_width, screen_height });
			ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

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

				// composite

				ImGui::Separator();
				ImGui::Text("composite");
				if (ImGui::IsItemHovered()) {
					ImGui::BeginTooltip();
					ImGui::Text("name: %s", current->name().c_str());
					ImGui::Text("size: %d", current->size());
					ImGui::Text("persistency: %d", static_cast<unsigned int>(current->lifetime()));
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

				ImGui::InputText("##composite_name", composite_name.data(), composite_name.size() - 1, ImGuiInputTextFlags_AutoSelectAll);
				ImGui::SameLine();
				if (ImGui::Button("name##name_composite")) {
					current->set_name(composite_name.data());
				}

				// transform

				if (transform) {
					ImGui::Separator();
					ImGui::Text("transform");
					if (ImGui::IsItemHovered()) {
						ImGui::BeginTooltip();
						ImGui::Text("current: (%d, %d)", transform->position().x(), transform->position().y());
						ImGui::Text("last: (%d, %d)", transform->last().x(), transform->last().y());
						ImGui::Text("space: %p", transform->world());
						//ImGui::Text("adress: %p", transform);
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

				// sprite

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

				// body

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
						if (ImGui::Checkbox("transparent##body_transparent", &body_transparent)) {
							mass.make_transparent(body_transparent);
						}
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

				if (ImGui::Button("Export")) {
					auto output = output_stream("data/blueprints/" + current->name() + ".dat");
					SAVE_OUTPUT_ARCHIVE archive(output);
					blueprint::save(archive, *current);

					refresh_template_items();
				}
				ImGui::SameLine();
				if (ImGui::Button("Discard")) {
					current = nullptr;
				}
			}

			ImGui::End();
		}

	private:
		void load_schema(std::string const& schema_name)
		{
			if (game) {
				builder factory(game);
				current = schema::load(document::load_document("data/schemata/" + schema_name), factory);
				update_props();
			}
		}
		void load_blueprint(std::string const& blueprint_name)
		{
			if (game) {
				auto input = input_stream("data/blueprints/" + blueprint_name);
				SAVE_INPUT_ARCHIVE archive(input);
				builder factory(game);
				current = blueprint::load(archive, factory);
				update_props();
			}
		}
		void refresh_template_items()
		{
			load_items(blueprint_selected, blueprints, "data/blueprints/");
			load_items(schema_selected, schemata, "data/schemata/");
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

				composite_name.fill(0);
				std::string name = current->name();
				std::copy(name.cbegin(), name.cbegin() + std::min(name.length(), composite_name.size() - 2), composite_name.begin()); // reserve extra zero for end of a string

				if (auto transform = current->query<transform_component>()) {
					auto pos = transform->position();
					transform_x = pos.x();
					transform_y = pos.y();
				}
				if (auto sprite = current->query<sprite_component>()) {
					std::string str(sprite->name);

					sprite_name.fill(0);
					std::copy(str.cbegin(), str.cbegin() + std::min(str.length(), sprite_name.size() - 2), sprite_name.begin()); // reserve extra zero for end of a string
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
				}
			}
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

		bool						body_transparent;
		int							hp;
		int							hp_max;
		int							mp;
		int							mp_max;
	};
}