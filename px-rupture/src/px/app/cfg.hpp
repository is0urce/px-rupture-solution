// name: cfg.hpp
// type: c++
// auth: is0urce
// desc: configuration file

#pragma once

#include <fstream>
#include <map>
#include <string>

#include <nlohmann/json.hpp>

namespace px {

    class cfg {
    public:
        using int_type = int;
        using float_type = double;
        using string_type = std::string;

        template <typename T>
        class value_record {
        public:
            using value_type = T;
        private:
            value_type &    ref;
            bool &          dirt;

        public:
            value_record & operator=(value_type value) {
                if (ref != value) {
                    ref = value;
                    dirt = true;
                }
                return *this;
            }
            operator value_type() const {
                return ref;
            }
        public:
            value_record(value_type & r, bool & flag_ref)
                : ref(r)
                , dirt(flag_ref) {
            }
        };

        class option_record {
        private:
            std::string key;
            cfg &       config;
        public:
            int_type & operator=(int_type value) {
                return config.set_int(key, value);
            }
            float_type & operator=(float_type value) {
                return config.set_float(key, value);
            }
            string_type & operator=(string_type value) {
                return config.set_string(key, value);
            }
            bool & operator=(bool value) {
                return config.set_boolean(key, value);
            }
            operator int_type() {
                return config.get_int(key);
            }
            operator float_type() {
                return config.get_float(key);
            }
            operator string_type() {
                return config.get_string(key);
            }
            operator bool() {
                return config.get_boolean(key);
            }
        public:
            option_record(std::string const& k, cfg & c)
                : key(k)
                , config(c) {
            }
        };

    public:
        bool load(std::string const& path) {
            close();
            name = path;
            dirty = false;
            return read(name);
        }

        bool save() {
            dirty = false;
            return write(name);;
        }

        int_type get_int(std::string const& key) {
            return i[key];
        }

        float_type get_float(std::string const& key) {
            return f[key];
        }

        string_type get_string(std::string const& key) {
            return s[key];
        }

        bool get_boolean(std::string const& key) {
            return b[key];
        }

        int_type & set_int(std::string const& key, int_type value) {
            dirty = true;
            return i[key] = value;
        }

        float_type & set_float(std::string const& key, float_type value) {
            dirty = true;
            return f[key] = value;
        }

        string_type & set_string(std::string const& key, string_type value) {
            dirty = true;
            return s[key] = value;
        }

        bool & set_boolean(std::string const& key, bool value) {
            dirty = true;
            return b[key] = value;
        }

        value_record<int_type> number(std::string const& key) {
            return value_record<int_type>(i[key], dirty);
        }

        value_record<bool> boolean(std::string const& key) {
            return value_record<bool>(b[key], dirty);
        }

        value_record<float_type> real(std::string const& key) {
            return value_record<float_type>(f[key], dirty);
        }

        value_record<string_type> string(std::string key) {
            return value_record<string_type>(s[key], dirty);
        }

        bool value(std::string const& key, bool or_else) const {
            auto it = b.find(key);
            return it != b.end() ? it->second : or_else;
        }

        int_type value(std::string const& key, int_type or_else) const {
            auto it = i.find(key);
            return it != i.end() ? it->second : or_else;
        }

        float_type value(std::string const& key, float_type or_else) const {
            auto it = f.find(key);
            return it != f.end() ? it->second : or_else;
        }

        string_type value(std::string const& key, string_type or_else) const {
            auto it = s.find(key);
            return it != s.end() ? it->second : or_else;
        }

        option_record operator[](std::string const& key) {
            return option_record(key, *this);
        }

    public:
        ~cfg() {
            close();
        }

        cfg(std::string const& path)
            : dirty(false) {
            load(path);
        };

    private:
        void clear() {
            i.clear();
            f.clear();
            s.clear();
            b.clear();
        }

        void close() {
            if (dirty && !name.empty()) {
                save();
                dirty = false;
            }
        }

        bool read(std::string const& path) {
            clear();
            std::ifstream file(path);
            auto jsn = nlohmann::json::parse(file);
            parse("", jsn);
            return true;
        }

        bool write(std::string const& path) {
            nlohmann::json jsn;
            populate(jsn, i);
            populate(jsn, f);
            populate(jsn, s);
            populate(jsn, b);
            std::ofstream file(path);
            file << jsn;
            return true;
        }

        template <typename Document>
        void parse(std::string const& prefix, Document const& doc) {
            for (auto it = doc.begin(), last = doc.end(); it != last; ++it) {
                if (it->is_object()) {
                    parse(prefix + it.key() + ".", it.value());
                }
                else if (it->is_number_integer()) {
                    i[prefix + it.key()] = it.value();
                }
                else if (it->is_number_float()) {
                    f[prefix + it.key()] = it.value();
                }
                else if (it->is_string()) {
                    s[prefix + it.key()] = it.value();
                }
                else if (it->is_boolean()) {
                    b[prefix + it.key()] = it.value();
                }
            }
        }

        template <typename Document, typename Container>
        void populate(Document & doc, Container const& container) {
            for (auto const& kv : container) {
                doc[kv.first] = kv.second;
            }
        }

    private:
        std::map<std::string, int_type>     i;
        std::map<std::string, float_type>   f;
        std::map<std::string, string_type>  s;
        std::map<std::string, bool>         b;
        std::string                         name;
        bool                                dirty;
    };
}