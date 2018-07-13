#pragma once

// Chunk format
// ID SIZE DATA

// File format

// RESF
// KEYS
// EXTR
// DATA

// Keys data:
// strz:key pos:CURR pos:LEFT pos:RIGHT

#include <istream>
#include <fstream>

#include <stdexcept>
#include <string>

#include <cstdint>

namespace px {

    class resource_file {
    public:
        using name_t = std::uint32_t;
        using pos_t = std::uint32_t;

    public:
        std::istream & get(std::string const& name) {
            seek_record(name);
            return stream;
        }

    public:
        resource_file(std::string const& path) {
            stream.open(path, std::ios::binary);
            if (!stream.is_open()) {
                throw std::runtime_error("resource_file::seek(name) - file not opened, path=" + path);
            }
        }

    private:
        bool seek_record(std::string const& name) {
            bool result = false;
            if (seek_keys()) {

                while (true) {
                    char cmp = compare(name);

                    pos_t stride[3];
                    stream.read(reinterpret_cast<char*>(stride), (sizeof stride[0]) * 3);

                    if (cmp == 0) {
                        stream.seekg(stride[0], std::ios::cur);
                        result = true;
                        break;
                    }
                }
            }
            return result;
        }

        bool seek_keys() {
            bool result = true;
            stream.seekg(0, std::ios::beg);

            while (true) {
                name_t name;
                pos_t size;
                stream.read(reinterpret_cast<char*>(&name), sizeof name);
                stream.read(reinterpret_cast<char*>(&name), sizeof size);

                if (name == 'KEYS') break;

                stream.seekg(size, std::ios::cur);

                if (stream.eof()) {
                    result = false;
                    break;
                }
            }

            return result;
        }

        char compare(std::string const& name) {
            char cmp = 0;
            char letter = 0;

            size_t len = name.length();
            for (size_t i = 0; i != len; ++i) {
                stream.read(&letter, sizeof letter);
                cmp = letter - name[0];
                if (cmp != 0) break;
            }

            while (letter != 0) {
                stream.read(&letter, sizeof letter);
            }

            return cmp;
        }

    private:
        std::ifstream   stream;
    };
}