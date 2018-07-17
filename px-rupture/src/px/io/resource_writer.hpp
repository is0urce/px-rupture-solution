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
        struct rec_key {
            std::string name;
            pos_t       current;
            pos_t       l_absolute;
            pos_t       r_absolute;
            pos_t       absoulte;
            size_t      l_index;
            size_t      r_index;
            size_t      sequence_index;
        };

        struct rec_data {
            std::vector<char>   storage;
            char const*         data;
            pos_t               size;
            pos_t               absolute;
        };

        struct sequence {
            size_t index;
            size_t l_index;
            size_t r_index;
        };

        struct kv {
            rec_key     k;
            rec_data    v;
        };

    public:
        void copy(std::string const& name, char const* source, size_t n) {
            std::vector<char> raw;
            raw.reserve(n);
            std::copy(source, source + n, std::back_inserter(raw));
            char * data_ptr = raw.data();

            resources.push_back(kv{ rec_key{ name }, rec_data{ std::move(raw), data_ptr, static_cast<pos_t>(n) } });
        }

        void link(std::string const& name, char const* memory, size_t n) {
            resources.push_back(kv{ rec_key{ name }, rec_data{ {}, memory, static_cast<pos_t>(n) } });
        }

        void write(std::string const& filename) {
            std::ofstream out_stream;
            out_stream.open(filename, std::ofstream::binary | std::ofstream::out | std::ofstream::trunc);
            if (!out_stream.is_open()) {
                throw std::runtime_error("resource_writer::write(name) - file not opened, path=" + filename);
            }
            sort_sequental();
            generate();
            dump(out_stream);
        }

    public:
        resource_writer()
            : keys_header{ keys_name() }
            , data_header{ data_name() } {
        }

    private:
        void sort_sequental() {
            size_t const size = resources.size();

            // sort
            std::sort(resources.begin(), resources.end(), compare_records);

            // place sequental
            keys_sequence.resize(size);
            arrange_sequence(0, size, 0);
            for (size_t i = 0; i != size; ++i) {
                resources[keys_sequence[i].index].k.sequence_index = i;
            }
            std::sort(resources.begin(), resources.end(), compare_sequence);

            // link keys
            for (size_t i = 0; i != size; ++i) {
                resources[i].k.l_index = keys_sequence[i].l_index;
                resources[i].k.r_index = keys_sequence[i].r_index;
            }
        }

        void arrange_sequence(size_t start, size_t end, size_t index) {
            if (start == end) return;

            size_t const size = end - start;
            size_t const average = size / 2 + index;

            size_t const l_start = start + 1;
            size_t const l_end = size / 2 + l_start;
            size_t const l_index = index;

            size_t const r_start = l_end;
            size_t const r_end = end;
            size_t const r_index = average + 1;

            arrange_sequence(l_start, l_end, l_index);
            arrange_sequence(r_start, r_end, r_index);

            keys_sequence[start].index = average;
            keys_sequence[start].l_index = l_start < l_end ? l_start : -1;
            keys_sequence[start].r_index = r_start < r_end ? r_start : -1;
        }

        // use sort sequental, this is for fallback
        void sort_binary(size_t start, size_t end) {
            if (start == end) return;

            size_t const l_start = start + 1;
            size_t const mid = (start + end) / 2;
            size_t const l_end = mid + 1 > end ? end : mid + 1;

            std::nth_element(resources.begin() + start, resources.begin() + mid, resources.begin() + end, compare_records);
            std::swap(resources[start], resources[mid]);

            sort_binary(l_start, l_end);
            sort_binary(l_end, end);

            resources[start].k.l_index = l_start < l_end ? l_start : -1;
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
                resources[i].k.current = resources[i].v.absolute = pen;
                pen += sizeof(data_header);
                pen += resources[i].v.size;
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

            // keys
            dump_header(stream, keys_header);
            for (size_t i = 0; i != size; ++i) {
                stream.write(resources[i].k.name.c_str(), resources[i].k.name.size() + 1);
                stream.write(reinterpret_cast<char const*>(&resources[i].k.current), sizeof(pos_t));
                stream.write(reinterpret_cast<char const*>(&resources[i].k.l_absolute), sizeof(pos_t));
                stream.write(reinterpret_cast<char const*>(&resources[i].k.r_absolute), sizeof(pos_t));
            }

            // data
            dump_header(stream, data_header);
            for (size_t i = 0; i != size; ++i) {
                dump_header(stream, { void_name(), static_cast<pos_t>(resources[i].v.size) });
                stream.write(resources[i].v.data, resources[i].v.size);
            }
        }

        template <typename Out>
        void dump_header(Out && stream, header const& h) {
            stream.write(reinterpret_cast<char const*>(&h.name), sizeof h.name);
            stream.write(reinterpret_cast<char const*>(&h.size), sizeof h.size);
        }

        static bool compare_records(kv const& lh, kv const& rh) {
            return compare(lh.k.name.c_str(), rh.k.name.c_str()) < 0;
        }

        static bool compare_sequence(kv const& lh, kv const& rh) {
            return lh.k.sequence_index < rh.k.sequence_index;
        }

    private:
        std::vector<kv>         resources;
        std::vector<sequence>   keys_sequence;
        header                  keys_header;
        header                  data_header;
    };
}