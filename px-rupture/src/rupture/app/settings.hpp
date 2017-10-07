// name: settings.hpp

#pragma once

#include <string>

namespace px {
	namespace {

		struct settings {
			static char *const application_name;
			static char *const bindings_path;
			static char *const configuration_path;
			static char *const texture_path;
			static char *const log_path;
		};

		char *const settings::application_name = "px-rupture";
		char *const settings::bindings_path = "data/controls.json";
		char *const settings::configuration_path = "data/config.json";
		char *const settings::texture_path = "data/textures.json";
		char *const settings::log_path = "log.txt";
	}
}