// name: settings.hpp

#pragma once

#include <string>

namespace px {

	struct settings {
		static std::string const application_name;
		static std::string const bindings_path;
		static std::string const configuration_path;
		static std::string const log_path;
	};

	std::string const settings::application_name = "px-rupture";
	std::string const settings::bindings_path = "data/controls.json";
	std::string const settings::configuration_path = "data/config.json";
	std::string const settings::log_path = "log.txt";
}