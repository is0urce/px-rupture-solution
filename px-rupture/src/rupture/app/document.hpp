// name: document.hpp
// type: c++
// auth: is0urce
// desc: load documents

#pragma once

#include <json.hpp>

#include <fstream>
#include <stdexcept>
#include <string>

namespace px {

	class document final
	{
	public:
		static auto load_document(std::string const& document_name)
		{
			std::ifstream file(document_name);
			if (!file.is_open()) throw std::runtime_error("px::depot::load_document() - error opening file, path=" + document_name);
			return nlohmann::json::parse(file);
		}
	};
}