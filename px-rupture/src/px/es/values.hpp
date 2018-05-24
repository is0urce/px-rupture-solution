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

        void set_float(std::string const& key, F value) {
            floats[key] = value;
        }

        F float_value(std::string const& key) {
            return floats[key];
        }

        void set_string(std::string const& key, std::string const& value) {
            strings[key] = value;
        }

        std::string string_value(std::string const& key) {
            return strings[key];
        }

        template <typename Archive>
        void serialize(Archive & archive) {
            archive(ints, floats, strings);
        }

    public:
        values() = default;

    private:
        std::map<std::string, I>            ints;
        std::map<std::string, F>            floats;
        std::map<std::string, std::string>  strings;
    };
}