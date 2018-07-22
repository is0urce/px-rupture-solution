// name: values.hpp
// type: c++ header
// desc: class
// auth: is0urce

#pragma once

// template base for values component

#include <string>
#include <map>

namespace px {

    template <typename I, typename F>
    class values {
    public:
        void set_int(std::string const& key, I value) {
            ints[key] = value;
        }

        I int_value(std::string const& key) {
            return ints[key];
        }

        I int_value(std::string const& key, I fallback) const {
            auto it = ints.find(key);
            return it == ints.end() ? fallback : it->second;
        }

        I mod_int(std::string const& key, I mod) {
            return ints[key] += mod;
        }

        void set_float(std::string const& key, F value) {
            floats[key] = value;
        }

        F float_value(std::string const& key) {
            return floats[key];
        }

        F float_value(std::string const& key, F fallback) const {
            auto it = floats.find(key);
            return it == floats.end() ? fallback : it->second;
        }

        I mod_float(std::string const& key, F mod) {
            return floats[key] += mod;
        }

        void set_string(std::string const& key, std::string const& value) {
            strings[key] = value;
        }

        std::string string_value(std::string const& key) {
            return strings[key];
        }

        std::string string_value(std::string const& key, std::string const& fallback) const {
            auto it = strings.find(key);
            return it == strings.end() ? fallback : it->second;
        }

        template <typename Archive>
        void serialize(Archive & archive) {
            archive(ints, floats, strings);
        }

        bool is_empty() const {
            return ints.empty() && floats.empty() && strings.empty();
        }

        template <typename E>
        void enumerate_strings(E && fn) const {
            for (auto const& str_kv : strings) {
                fn(str_kv.first, str_kv.second);
            }
        }

        template <typename E>
        void enumerate_ints(E && fn) const {
            for (auto const& int_kv : ints) {
                fn(int_kv.first, int_kv.second);
            }
        }

        template <typename E>
        void enumerate_floats(E && fn) const {
            for (auto const& float_kv : floats) {
                fn(float_kv.first, float_kv.second);
            }
        }

    public:
        values() = default;
        ~values() = default;

    private:
        std::map<std::string, I>            ints;
        std::map<std::string, F>            floats;
        std::map<std::string, std::string>  strings;
    };
}