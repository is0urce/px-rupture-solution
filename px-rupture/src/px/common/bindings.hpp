// name: bindings.hpp
// type: c++ header
// desc: class to translate system virtual key messages to shell virtual key messages
// auth: is0urce

#pragma once

#include <stdexcept>
#include <unordered_map>
#include <optional>

namespace px {

    template <typename SK, typename VK>
    class bindings final {
    public:
        using sys_key = SK;
        using v_key = VK;

    public:
        ~bindings() = default;
        bindings() = default;

    public:
        template <typename Document>
        static bindings from_document(Document && document) {
            bindings result;
            result.load(std::forward<Document>(document));
            return result;
        }

    public:
        template <typename Document>
        void load(Document && document) {
            try {
                for (auto const& action_entry : document) {
                    long long action_index = action_entry["action"];
                    for (long long key : action_entry["keys"]) {
                        bind(static_cast<SK>(key), static_cast<VK>(action_index));
                    }
                }
            }
            catch (std::exception & exc) {
                throw std::runtime_error("px::bindings::load(document) - error while loading bindings, what=" + std::string(exc.what()));
            }
            catch (...) {
                throw std::runtime_error("px::bindings::load(document) - error while loading bindings, unhandled exception");
            }
        }

        void bind(SK key, VK vkey) {
            m_bindings[key] = vkey;
        }

        bool find(SK key, VK & vkey) const {
            auto it = m_bindings.find(key);
            if (it != m_bindings.end()) {
                vkey = it->second;
                return true;
            }
            return false;
        }

        std::optional<VK> find(SK key) const noexcept {
            auto it = m_bindings.find(key);
            return it == m_bindings.end() ? {} : it->second;
        }

        VK get_or(SK key, VK or_else) const {
            find(key, or_else);
            return or_else;
        }

        VK operator[](SK key) const noexcept {
            return m_bindings[key];
        }

        VK at(SK key) const {
            return m_bindings.at(key);
        }

    private:
        std::unordered_map<SK, VK> m_bindings;
    };
}