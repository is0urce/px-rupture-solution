#pragma once

#include "resource_file.hpp"

#include <algorithm>
#include <ostream>
#include <fstream>

#include <stdexcept>
#include <string>

#include <vector>

namespace px {

    class resource_writer
        : public resource_file {

    public:
        struct r_key {
            std::string name;
            pos_t       current;
            pos_t       l_absolute;
            pos_t       r_absolute;
            pos_t       absoulte;
        };

        struct r_data {
            std::vector<char>   raw;
            pos_t               absolute;
        };

        struct kv {
            r_key     k;
            r_data    v;
        };

    public:
        void add(std::string const& name, char const* memory, size_t n) {
            std::vector<char> raw;
            std::copy(memory, memory + n, std::back_inserter(raw));

            resources.push_back(kv{ r_key{ name }, r_data{ std::move(raw) } });
        }

        void write(std::string const& filename) {
            std::ofstream out_stream;
            out_stream.open(filename, std::ofstream::binary | std::ofstream::out | std::ofstream::trunc);
            if (!out_stream.is_open()) {
                throw std::runtime_error("resource_file::seek(name) - file not opened, path=" + filename);
            }
            sort();
            generate();
            dump(out_stream);
        }

    public:
        resource_writer()
            : keys_header{ 'KEYS' }
            , data_header{ 'DATA' } {
        }

    private:
        void sort() {
            std::sort(resources.begin(), resources.end(), [&](auto const& lh, auto const rh) {
                return compare(lh.k.name.c_str(), rh.k.name.c_str()) < 0;
            });
        }

        void generate() {
            pos_t pen = 0;

            pen += sizeof keys_header;
            size_t const size = resources.size();
            for (size_t i = 0; i != size; ++i) {
                resources[i].k.absoulte = pen;
                pen += static_cast<pos_t>(resources[i].k.name.size()) + 1;
                pen += sizeof(pos_t) * 3;
            }
            //keys_header.size = 0;

            pen += sizeof data_header;
            for (size_t i = 0; i != size; ++i) {
                resources[i].v.absolute = pen;
                resources[i].k.current = resources[i].v.absolute;
                pen += static_cast<pos_t>(resources[i].v.raw.size());
            }
            //data_header.size = -1;

            // link keys
            for (size_t i = 1; i != size; ++i) {
                resources[i - 1].k.l_absolute = resources[i].k.absoulte;
                resources[i - 1].k.r_absolute = resources[i].k.absoulte;
            }
        }

        template <typename Out>
        void dump(Out && stream) {
            size_t const size = resources.size();

            dump_header(stream, keys_header);
            for (size_t i = 0; i != size; ++i) {
                stream.write(resources[i].k.name.c_str(), resources[i].k.name.size() + 1);

                stream.write(reinterpret_cast<char const*>(&resources[i].k.current), sizeof(pos_t));
                stream.write(reinterpret_cast<char const*>(&resources[i].k.l_absolute), sizeof(pos_t));
                stream.write(reinterpret_cast<char const*>(&resources[i].k.r_absolute), sizeof(pos_t));
            }

            dump_header(stream, data_header);
            for (size_t i = 0; i != size; ++i) {
                stream.write(resources[i].v.raw.data(), resources[i].v.raw.size());
            }
        }

        template <typename Out>
        void dump_header(Out && stream, header const& h) {
            stream.write(reinterpret_cast<char const*>(&h.name), sizeof h.name);
            stream.write(reinterpret_cast<char const*>(&h.size), sizeof h.size);
        }

    private:
        std::vector<kv> resources;
        header          keys_header;
        header          data_header;
    };
}