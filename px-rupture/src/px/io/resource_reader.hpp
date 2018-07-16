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
        std::istream & get(std::string const& name) {
            seek_record(name);
            return stream;
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
                throw std::runtime_error("resource_file::seek(name) - file not opened, path=" + path);
            }
        }

    private:
        bool seek_record(std::string const& name) {
            if (!seek_keys()) return false;
            char lh_str[1024];
            pos_t stride[3];
            while (true) {
                // read key string
                stream.get(lh_str, sizeof lh_str, 0x00);
                stream.get(); // skip trailing zero
                // read 'next' positions current data node, left tree branch, right tree branch
                stream.read(reinterpret_cast<char*>(&stride[0]), sizeof stride[0]);
                stream.read(reinterpret_cast<char*>(&stride[1]), sizeof stride[1]);
                stream.read(reinterpret_cast<char*>(&stride[2]), sizeof stride[2]);

                auto cmp = compare(name.c_str(), lh_str);
                auto next = stride[cmp < 0 ? 1 : 2];

                if (cmp == 0) {
                    stream.seekg(stride[0], std::ios::beg);
                    return true;
                }
                if (!is_valid(next)) {
                    return false;
                }

                stream.seekg(next, std::ios::beg);
            }
        }

        bool seek_keys() {
            stream.seekg(0, std::ios::beg);

            while (true) {
                header h;
                read_header(h);

                if (h.name == 'KEYS') return true;

                stream.seekg(h.size, std::ios::cur);

                // terminate
                if (stream.eof()) {
                    return false;
                }
            }
        }

        void read_header(header & h) {
            stream.read(reinterpret_cast<char*>(&h.name), sizeof h.name);
            stream.read(reinterpret_cast<char*>(&h.size), sizeof h.size);
        }

    private:
        std::ifstream   stream;
    };
}