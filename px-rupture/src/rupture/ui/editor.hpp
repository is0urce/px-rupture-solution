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
		{
			strref.fill(0);
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

			// schema selection

			refresh_schemata_items();
			if (ImGui::Combo("###schematas", &schema_selected, schemata_flat.data(), static_cast<int>(schemata_count))) {
				strref.fill(0);
				auto const& name = schemata[schema_selected];
				std::copy(name.cbegin(), name.cbegin() + std::min(name.length(), strref.size() - 1), strref.begin());
			} ImGui::SameLine(); if (ImGui::Button("Refresh")) {
				load_schemata_items();
			}

			ImGui::InputText("###strref", &strref[0], strref.size()); ImGui::SameLine(); if (ImGui::Button("Load")) {
				load_current_schema();
			}

			// object props

			if (current) {

				// composite

				ImGui::Separator();
				ImGui::Text("composite");

				ImGui::Text("Name: %s", current->name().c_str());
				ImGui::InputText("###composite_name", composite_name.data(), composite_name.size() - 1, ImGuiInputTextFlags_AutoSelectAll); ImGui::SameLine(); if (ImGui::Button("Name")) {
					current->set_name(composite_name.data());
				}

				// transform

				ImGui::Separator();
				transform_component * transform = current->query<transform_component>();
				if (transform) {
					ImGui::Text("transform");

					ImGui::Text("X:"); ImGui::SameLine(); ImGui::InputInt("###x", &transform_x);
					ImGui::Text("Y:"); ImGui::SameLine(); ImGui::InputInt("###y:", &transform_y);
					if (ImGui::Button("Place")) {
						transform->place({ transform_x, transform_y });
					}

					ImGui::SameLine();
					if (ImGui::Button("Remove transform")) {
						PX_BUILD(remove_transform());
					}			
				}
				else if (ImGui::Button("+ Transform")) {
					PX_BUILD(add_transform({ 0, 0 }));
				}

				// sprite

				ImGui::Separator();
				sprite_component * sprite = current->query<sprite_component>();
				if (sprite) {
					ImGui::Text("sprite: %s", sprite->name);
					ImGui::Text("texture_id: %d", sprite->texture_index);
					ImGui::Text("sx: %f, dx: %f", sprite->sx_texture, sprite->dx_texture);
					ImGui::Text("sy: %f, dy: %f", sprite->sy_texture, sprite->dy_texture);

					ImGui::InputText("###snit", sprite_name.data(), sprite_name.size(), ImGuiInputTextFlags_AutoSelectAll);
					ImGui::SameLine();
					if (ImGui::Button("Set")) {
						std::string str(sprite_name.data());
						PX_SWAP(remove_sprite(), add_sprite(str));
					}
				}
				else if (ImGui::Button("+ Sprite")) {
					PX_BUILD(add_sprite("x_dummy"));
				}

				// spawn & export

				ImGui::Separator();

				if (transform) {
					ImGui::SameLine();
					if (ImGui::Button("Spawn")) {
						game->spawn(std::move(current), transform);
						load_current_schema();
					}
					ImGui::SameLine();
					if (ImGui::Button("Here")) {

						auto player = game->possessed();
						transform->place(player->position());
						game->spawn(std::move(current), transform);
						load_current_schema();
					}
				}
			}

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
		void load_schemata_items()
		{
			namespace fs = std::experimental::filesystem;

			schema_selected = -1;
			schemata_count = 0;
			schemata.clear();

			for (fs::directory_entry const& entry : fs::directory_iterator("data/schemata/")) {
				if (fs::is_regular_file(entry.path())) {
					schemata.push_back(entry.path().filename().string());
					++schemata_count;
				}
			}

			size_t letters = 0;
			for (auto const& file : schemata) {
				letters += file.length() + 1;
			}

			size_t cursor = 0;
			schemata_flat.resize(letters + 1);
			for (auto const& file : schemata) {
				size_t len = file.length();
				std::copy(file.cbegin(), file.cbegin() + len, schemata_flat.begin() + cursor);
				schemata_flat[cursor + len] = 0;
				cursor += len + 1;
			}
			schemata_flat[cursor] = 0; // double 00 at end
		}
		void refresh_schemata_items()
		{
			if (schemata_dirty) {
				load_schemata_items();
				schemata_dirty = false;
			}
		}
		void load_current_schema()
		{
			if (strref[0] != 0) {
				load_schema("data/schemata/" + std::string(strref.data()));
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

		bool						schemata_dirty;
		std::vector<std::string>	schemata;
		std::vector<char>			schemata_flat;
		size_t						schemata_count;
		int							schema_selected;

		std::array<char, 128>		strref;
		std::array<char, 128>		composite_name;
		int							composite_lifetime;

		int							transform_x;
		int							transform_y;

		std::array<char, 128>		sprite_name;
	};
}

#undef PX_LEN