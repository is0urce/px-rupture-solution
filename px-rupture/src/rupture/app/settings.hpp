// name: settings.hpp

#pragma once

#include <string>

namespace px {
	namespace {

		struct settings {
			const static char *const application_name;
			const static char *const bindings_path;
			const static char *const configuration_path;
			const static char *const texture_path;
			const static char *const tiles_path;
			const static char *const schemata_path;
			const static char *const blueprints_path;
			const static char *const scripts_path;
			const static char *const skills_path;
			const static char *const log_path;
		};

		const char *const settings::application_name = "px-rupture";
		const char *const settings::bindings_path = "data/controls.json";
		const char *const settings::configuration_path = "data/config.json";
		const char *const settings::texture_path = "data/textures.json";
		const char *const settings::tiles_path = "data/tiles.json";
		const char *const settings::skills_path = "data/skills.json";
		const char *const settings::log_path = "log.txt";
		const char *const settings::schemata_path = "data/schemata/";
		const char *const settings::blueprints_path = "data/blueprints/";
		const char *const settings::scripts_path = "data/scripts/";
	}
}