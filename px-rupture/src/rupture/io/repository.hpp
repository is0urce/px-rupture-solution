// name: repository.hpp
// type: c++
// auth: is0urce
// desc: save / load serialization folder management

#pragma once

#include <px/common/point.hpp>

#include <filesystem>
#include <stdexcept>
#include <string>

// save and persistent data management

namespace fs = std::experimental::filesystem;

namespace px {

	class repository final {
	public:
		bool exists() const {
			return fs::exists(name);
		}
		void remove() {
			fs::remove_all(name);
		}
		void create() {
			if (!exists()) {
				fs::create_directory(name);
			}
		}
		void clear() {
			// fs::create_directory sometime fails after fs::remove_all

			if (!exists()) {
				fs::create_directory(name);
			}
			else {
				for (auto const& entry : fs::directory_iterator(name)) {
					fs::remove_all(entry.path());
				}
			}
		}

		void save(repository & destination) const{
			copy(*this, destination);
		}
		void load(repository const& destination) {
			if (!destination.exists()) throw std::runtime_error("repository not exists");

			copy(destination, *this);
		}

		// data

		std::string depot_scene(point2 const& cell) const {
			return query("scene_" + to_string(cell));
		}
		std::string depot_main() const {
			return query("main");
		}
		std::string depot_meta() const {
			return query("meta");
		}

		bool has_scene(point2 const& cell) const {
			return record_exists("scene_" + to_string(cell));
		}
		bool has_main() const {
			return record_exists("main");
		}
		bool has_meta() const {
			return record_exists("meta");
		}

		bool clear_scene(std::string const& scene_name) {
			return clear(scene_name);
		}
		bool clear_main() {
			return clear("main");
		}
		bool clear_meta() {
			return clear("meta");
		}

	public:
		repository(std::string const& name, repository const* base)
			: name(name)
			, parent(base)
		{
			if (!exists()) {
				create();
			}
		}
		repository(std::string const& name)
			: repository(name, nullptr)
		{
		}
		repository(repository const&) = delete;
		repository & operator=(repository const&) = delete;

	private:
		std::string record_path(std::string const& record_name) const {
			fs::path filename(record_name);
			filename.replace_extension(".sav");
			return (name / filename).string();
		}
		std::string query(std::string const& record_name) const {
			auto current_path = record_path(record_name);

			// fallback to base
			if (!record_exists(record_name) && parent && parent->record_exists(record_name)) {
				pull(current_path, record_name, *parent);
			}

			return current_path;
		}
		bool pull(std::string const& dest_path, std::string const& record_name, repository const& base) const {
			if (base.record_exists(record_name)) {
				fs::copy(base.record_path(record_name), dest_path);
				return true;
			}
			return false;
		}
		bool record_exists(std::string const& record_name) const {
			return fs::exists(record_path(record_name));
		}
		bool clear(std::string const& record_name) const {
			return fs::remove(record_path(record_name));
		}
		static void move(repository & source, repository & destination) {
			destination.clear();

			for (auto const& entry : fs::directory_iterator(source.name)) {
				fs::path const& path = entry.path();
				fs::rename(path, destination.name / path.filename());
			}
		}
		static void copy(repository const& source, repository & destination) {
			destination.clear();

			for (auto const& entry : fs::directory_iterator(source.name)) {
				fs::path const& path = entry.path();
				fs::copy_file(path, destination.name / path.filename());
			}
		}
		static std::string to_string(point2 const& location) {
			return std::to_string(location.x() + coordinate_bias) + "_" + std::to_string(location.y() + coordinate_bias);
		}

	private:
		static const int	coordinate_bias = 0;
		fs::path			name;
		repository const*	parent;
	};
}