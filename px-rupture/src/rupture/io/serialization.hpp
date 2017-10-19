// name: serialization.hpp
// type: c++
// auth: is0urce
// desc: serialization macros and defines

#define SAVE_READABLE 0
#define SAVE_BINARY 1

#include <ios>

#if SAVE_READABLE
#define SAVE_INCLUDE_ARCHIVE <cereal/archives/xml.hpp>
#define SAVE_OUTPUT_ARCHIVE cereal::XMLOutputArchive
#define SAVE_INPUT_ARCHIVE cereal::XMLInputArchive
#define SAVE_OUTPUT_MODE std::ios_base::out
#define SAVE_INPUT_MODE std::ios_base::in
#endif

#if SAVE_BINARY
#define SAVE_INCLUDE_ARCHIVE <cereal/archives/binary.hpp>
#define SAVE_OUTPUT_ARCHIVE cereal::BinaryOutputArchive
#define SAVE_INPUT_ARCHIVE cereal::BinaryInputArchive
#define SAVE_OUTPUT_MODE std::ios_base::out | std::ios_base::binary
#define SAVE_INPUT_MODE std::ios_base::in | std::ios_base::binary
#endif

#include SAVE_INCLUDE_ARCHIVE
#include <cereal/types/bitset.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>

#include <fstream>

namespace px {

	inline std::ifstream input_stream(std::string const& name) {
		std::ifstream input(name, SAVE_INPUT_MODE);
		if (!input.is_open()) throw std::runtime_error("px::input_stream(name) - file error, name=" + name);
		return input;
	}
	inline std::ofstream output_stream(std::string const& name) {
		std::ofstream output(name, SAVE_OUTPUT_MODE);
		if (!output.is_open()) throw std::runtime_error("px::output_stream(name) - file error, name=" + name);
		return output;
	}
}