// name: entity.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#pragma once

// something with generic characteristics as name, tag, description

#include <string>

namespace px::rl {

    class entity {
    public:
        std::string const& name() const noexcept {
            return m_name;
        }

        void set_name(std::string name) {
            m_name = name;
        }

        std::string const& tag() const noexcept {
            return m_tag;
        }

        void set_tag(std::string resource_name) {
            m_tag = resource_name;
        }

        std::string const& description() const noexcept {
            return m_description;
        }

        void set_description(std::string text) {
            m_description = text;
        }

        bool same_tag(entity const& et) const {
            return m_tag == et.m_tag;
        }

        void setup_entity(std::string const& entity_name, std::string const& resource_name, std::string const& entity_description) {
            setup_entity(entity_name, resource_name);
            set_description(entity_description);
        }

        void setup_entity(std::string const& entity_name, std::string const& resource_name) {
            set_name(entity_name);
            set_tag(resource_name);
        }

        template <typename Archive>
        void serialize(Archive & archive) {
            archive(m_name, m_tag, m_description);
        }

    private:
        std::string m_tag;
        std::string m_name;
        std::string m_description;
    };

    namespace {
        bool operator==(entity const& lh, entity const& rh) noexcept {
            return lh.name() == rh.name() && lh.tag() == rh.tag();
        }
        bool operator!=(entity const& lh, entity const& rh) noexcept {
            return !operator==(lh, rh);
        }
    }
}