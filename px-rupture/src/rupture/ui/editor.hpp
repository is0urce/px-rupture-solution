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

#define PX_BUILD(OP) { builder factory(game); factory.begin(std::move(current)); factory.OP; current = factory.request(); update(); }
#define PX_SWAP(A, B) { builder factory(game); factory.begin(std::move(current)); factory.A, factory.B; current = factory.request(); update(); }

namespace px::ui {

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
			, schemata_dirty(true)
			, blueprints_dirty(true)
		{
			schema_strref.fill(0);
		}

	protected:
		virtual void combine_panel() override
		{
			static bool metrics = false;
			static bool dbg = false;

			if (!game) return;

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
			const float window_width = 250.0f;
			const float screen_width = ImGui::GetIO().DisplaySize.x;
			const float screen_height = ImGui::GetIO().DisplaySize.y;
			ImGui::SetNextWindowPos({ screen_width - window_width, 0 }, ImGuiCond_Always);
			ImGui::SetNextWindowSize({ window_width, screen_height });
			ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

			// template selection

			refresh_template_items();
			if (ImGui::Combo("###schemata", &schema_selected, schemata_flat.data(), static_cast<int>(schemata_count))) {
				schema_strref.fill(0);
				auto const& name = schemata[schema_selected];
				std::copy(name.cbegin(), name.cbegin() + std::min(name.length(), schema_strref.size() - 2), schema_strref.begin());
			}
			if (ImGui::Combo("###blueprints", &blueprint_selected, blueprints_flat.data(), static_cast<int>(blueprints_count))) {
				//strref.fill(0);
				//auto const& name = blueprints[blueprint_selected];
				//std::copy(name.cbegin(), name.cbegin() + std::min(name.length(), strref.size() - 2), strref.begin());
			}

			ImGui::SameLine();
			if (ImGui::Button("Refresh###refresh_schemata")) {
				schemata_dirty = true;
				blueprints_dirty = true;
			}

			ImGui::InputText("###schemata_strref", schema_strref.data(), schema_strref.size());
			ImGui::SameLine();
			if (ImGui::Button("Load###load_schema")) {
				load_current_schema();
			}

			// blueprint selection



			//ImGui::InputText("###strref", &strref[0], strref.size());
			//ImGui::SameLine();
			//if (ImGui::Button("Load")) {
			//	load_current_schema();
			//}

			// object props

			if (current) {

				// composite

				ImGui::Separator();
				ImGui::Text("composite");

				ImGui::Text("Name: %s", current->name().c_str());
				ImGui::InputText("##composite_name", composite_name.data(), composite_name.size() - 1, ImGuiInputTextFlags_AutoSelectAll); ImGui::SameLine(); if (ImGui::Button("Name")) {
					current->set_name(composite_name.data());
				}

				// transform

				ImGui::Separator();
				transform_component * transform = current->query<transform_component>();
				if (transform) {
					ImGui::Text("transform");
					ImGui::SameLine();
					if (ImGui::Button("remove##remove_transform")) {
						PX_BUILD(remove_transform());
					}
					ImGui::Text("X:"); ImGui::SameLine(); ImGui::InputInt("##transform_x", &transform_x);
					ImGui::Text("Y:"); ImGui::SameLine(); ImGui::InputInt("##transform_y", &transform_y);
					if (ImGui::Button("place##transform_place")) {
						transform->place({ transform_x, transform_y });
					}
				}
				else if (ImGui::Button("+ transform")) {
					PX_BUILD(add_transform({ 0, 0 }));
				}

				// sprite

				ImGui::Separator();
				sprite_component * sprite = current->query<sprite_component>();
				if (sprite) {
					ImGui::Text("sprite: %s", sprite->name);
					ImGui::SameLine();
					if (ImGui::Button("remove##remove_sprite")) {
						PX_BUILD(remove_sprite());
					}
					ImGui::Text("texture_id: %d", sprite->texture_index);
					ImGui::Text("sx: %f, dx: %f", sprite->sx_texture, sprite->dx_texture);
					ImGui::Text("sy: %f, dy: %f", sprite->sy_texture, sprite->dy_texture);
					ImGui::InputText("##sprite_text", sprite_name.data(), sprite_name.size(), ImGuiInputTextFlags_AutoSelectAll);
					ImGui::SameLine();
					if (ImGui::Button("set##set_sprite")) {
						std::string str(sprite_name.data());
						PX_SWAP(remove_sprite(), add_sprite(str));
					}
				}
				else if (ImGui::Button("+ sprite")) {
					PX_BUILD(add_sprite("x_dummy"));
				}

				// spawn

				ImGui::Separator();

				if (transform) {
					ImGui::SameLine();
					if (ImGui::Button("Spawn")) {
						game->spawn(std::move(current), transform);
					}
					transform_component * player = game->possessed();
					if (player) {
						ImGui::SameLine();
						if (ImGui::Button("Here")) {
							transform->place(player->position());
							game->spawn(std::move(current), transform);
						}
					}
				}
				else {
					ImGui::Text("require transform to spawn mobiles");
				}
			}

			// export

			ImGui::Separator();
			if (ImGui::Button("Export")) {
				auto output = output_stream("data/blueprints/" + current->name() + ".dat");
				SAVE_OUTPUT_ARCHIVE archive(output);
				blueprint::save(archive, *current);
			}
			if (ImGui::Button("Import")) {
				game->spawn("dummy", { 20, 20 });
			}

			ImGui::End();
		}

	private:
		void load_schema(std::string const& name)
		{
			if (game) {
				builder factory(game);
				current = schema::load(document::load_document(name), factory);
			}
		}
		void refresh_template_items()
		{
			if (blueprints_dirty) {
				load_items(blueprint_selected, blueprints_count, blueprints, blueprints_flat, "data/blueprints/");
				blueprints_dirty = false;
			}
			if (schemata_dirty) {
				load_items(schema_selected, schemata_count, schemata, schemata_flat, "data/schemata/");
				schemata_dirty = false;
			}
		}
		static void load_items(int & selected, size_t & count, std::vector<std::string> & names, std::vector<char> & flat, std::string const& path)
		{
			namespace fs = std::experimental::filesystem;

			selected = -1;
			count = 0;
			names.clear();

			for (fs::directory_entry const& entry : fs::directory_iterator(path)) {
				if (fs::is_regular_file(entry.path())) {
					names.push_back(entry.path().filename().string());
					++count;
				}
			}

			size_t letters = 0;
			for (auto const& file : names) {
				letters += file.length() + 1;
			}

			size_t cursor = 0;
			flat.resize(letters + 1);
			for (auto const& file : names) {
				size_t len = file.length();
				std::copy(file.cbegin(), file.cbegin() + len, flat.begin() + cursor);
				flat[cursor + len] = 0;
				cursor += len + 1;
			}
			flat[cursor] = 0; // double 00 at end
		}
		void load_current_schema()
		{
			if (schema_strref[0] != 0) {
				load_schema("data/schemata/" + std::string(schema_strref.data()));
			}
			update();
		}
		void update()
		{
			if (current) {
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
			}
		}

	private:
		environment *				game;
		uq_ptr<composite_component>	current;

		int							schema_selected;
		std::array<char, 128>		schema_strref;
		bool						schemata_dirty;
		std::vector<std::string>	schemata;
		std::vector<char>			schemata_flat;
		size_t						schemata_count;

		int							blueprint_selected;
		bool						blueprints_dirty;
		std::vector<std::string>	blueprints;
		std::vector<char>			blueprints_flat;
		size_t						blueprints_count;

		std::array<char, 128>		composite_name;
		int							composite_lifetime;

		int							transform_x;
		int							transform_y;

		std::array<char, 128>		sprite_name;
	};
}

#undef PX_LEN