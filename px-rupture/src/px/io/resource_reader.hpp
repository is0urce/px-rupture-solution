// name: resource_reader.hpp

#pragma once

#include "resource_file.hpp"

#include <istream>
#include <fstream>

#include <stdexcept>
#include <string>

namespace px {

    class resource_reader
        : public resource_file {
    public:
        std::istream & get(std::string const& name, header & info) {
            if (seek_record(name)) {
                read_header(info);
            }
            else {
                info = { noluck_name() };
            }
            return stream;
        }

        bool read(std::string const& name, char * dest) {
            if (seek_record(name)) {
                header info = read_header();
                stream.read(dest, info.size);
                return true;
            }
            return false;
        }

        bool read(std::string const& name, char * dest, size_t n_max) {
            if (seek_record(name)) {
                header info = read_header();
                stream.read(dest, n_max = n_max > info.size ? info.size : n_max);
                return true;
            }
            return false;
        }

        std::istream & get(std::string const& name) {
            header discard;
            return get(name, discard);
        }

        bool has_record(std::string const& name) {
            return seek_record(name);
        }

        std::istream & operator[](std::string const& name) {
            return get(name);
        }

    public:
        resource_reader(std::string const& path) {
            stream.open(path, std::ios::binary | std::ios::in);
            if (!stream.is_open()) {
                throw std::runtime_error("resource_reader::ctor(name) - file not opened, path=" + path);
            }
        }

    private:
        bool seek_record(std::string const& name) {
            if (!seek_keys()) return false;
            char lh_str[1024];
            pos_t left;
            pos_t right;

            while (true) {
                // read key string
                stream.get(lh_str, sizeof lh_str, 0x00);
                stream.get(); // skip trailing zero
                // read 'next' positions current data node, left tree branch, right tree branch
                stream.read(reinterpret_cast<char*>(&left), sizeof left);
                stream.read(reinterpret_cast<char*>(&right), sizeof right);
                auto cmp = compare(name.c_str(), lh_str);

                if (cmp == 0) {
                    pos_t current;
                    stream.read(reinterpret_cast<char*>(&current), sizeof current);
                    stream.seekg(current, std::ios::beg);
                    return true;
                }

                pos_t next = cmp < 0 ? left : right;
                if (!is_valid(next)) {
                    return false;
                }

                stream.seekg(next, std::ios::beg);
            }
        }

        bool seek_keys() {
            stream.seekg(0, std::ios::beg);

            while (true) {
                header info;
                read_header(info);

                if (info.name == keys_name()) return true;

                stream.seekg(info.size, std::ios::cur);

                // terminate
                if (stream.eof()) {
                    return false;
                }
            }
        }

        void read_header(header & info) {
            stream.read(reinterpret_cast<char*>(&info.name), sizeof info.name);
            stream.read(reinterpret_cast<char*>(&info.size), sizeof info.size);
        }

        header read_header() {
            header result;
            read_header(result);
            return result;
        }

    private:
        std::ifstream   stream;
    };
}