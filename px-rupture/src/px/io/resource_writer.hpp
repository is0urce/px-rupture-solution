// name: resource_writer.hpp

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
            size_t      l_index;
            size_t      r_index;
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
            sort_binary(0, resources.size());
        }

        void sort_binary(size_t start, size_t end) {
            if (start == end) return;

            std::sort(resources.begin() + start, resources.begin() + end, [&](auto const& lh, auto const rh) {
                return compare(lh.k.name.c_str(), rh.k.name.c_str()) < 0;
            });

            size_t mid = (start + end) / 2;

            std::swap(resources[start], resources[mid]);

            size_t l = start + 1 > end ? end : start + 1;
            size_t l_end = mid + 1 > end ? end : mid + 1;

            sort_binary(l, l_end);
            sort_binary(l_end, end);

            resources[start].k.l_index = l < end ? l : -1;
            resources[start].k.r_index = l_end < end ? l_end : -1;
        }

        void generate() {
            size_t const size = resources.size();

            pos_t pen = 0;

            pen += sizeof keys_header;
            pos_t const keys_begin = pen;
            for (size_t i = 0; i != size; ++i) {
                resources[i].k.absoulte = pen;
                pen += static_cast<pos_t>(resources[i].k.name.size()) + 1;
                pen += sizeof(pos_t) * 3;
            }
            keys_header.size = pen - keys_begin;

            pen += sizeof data_header;
            pos_t const data_begin = pen;
            for (size_t i = 0; i != size; ++i) {
                resources[i].v.absolute = pen;
                resources[i].k.current = resources[i].v.absolute;
                pen += static_cast<pos_t>(resources[i].v.raw.size());
            }
            data_header.size = pen - data_begin;

            for (size_t i = 0; i != size; ++i) {
                resources[i].k.l_absolute = resources[i].k.l_index < size ? resources[resources[i].k.l_index].k.absoulte : 0;
                resources[i].k.r_absolute = resources[i].k.r_index < size ? resources[resources[i].k.r_index].k.absoulte : 0;
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