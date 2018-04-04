// name: skill_set.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

namespace px::rl {

    template <typename Skill>
    class skill_set {
    public:
        using skill_type = Skill;
        using state_type = typename skill_type::state_type;
        using impact_type = typename skill_type::impact_type;
        using book_type = std::map<std::string, std::tuple<state_type, impact_type *>>;

    public:
        void assign_book(book_type const* book) {
            m_book = book;
        }

        book_type const* get_book() const noexcept {
            return m_book;
        }

        bool learn(std::string const& tag) {
            if (m_book) {
                auto const& it = m_book->find(tag);
                if (it != m_book->end()) {
                    auto const& record = it->second;
                    m_skills.emplace_back();
                    m_skills.back().state() = std::get<0>(record);
                    m_skills.back().assign_impact(std::get<1>(record));
                    return true;
                }
            }
            return false;
        }

        template <typename ...Args>
        bool learn(std::string const& tag1, std::string const& tag2, Args ...args) {
            return learn(tag1) && learn(tag2, args...);
        }

        void invalidate() {
            for (size_t i = 0, size = m_skills.size(); i != size; ++i) {
                invalidate(m_skills[i].state().tag(), i);
            }
        }

        bool remove(size_t idx) {
            if (idx < m_skills.size()) {
                m_skills[idx] = std::move(m_skills.back());
                m_skills.pop_back();
                return true;
            }
            return false;
        }

        void clear() {
            m_skills.clear();
        }

        size_t size() const {
            return m_skills.size();
        }

        skill_type & at(size_t slot) {
            return m_skills.at(slot);
        }

        template <size_t Slot>
        skill_type & at() {
            return m_skills.at(Slot);
        }

        skill_type * get(size_t index) {
            return index < m_skills.size() ? &m_skills[index] : nullptr;
        }

        skill_type const* get(size_t index) const {
            return index < m_skills.size() ? &m_skills[index] : nullptr;
        }

        void reduce_cooldown(int timespan) {
            for (auto & skill : m_skills) {
                skill.state().reduce_cooldown(timespan);
            }
        }

        template <typename Archive>
        void serialize(Archive & archive) {
            archive(m_skills);
            invalidate();
        }

    public:
        ~skill_set() = default;
        skill_set()
            : m_book(nullptr)
        {
        }
        skill_set(book_type * skillbook)
            : m_book(skillbook)
        {
        }
        skill_set(skill_set const&) = delete;
        skill_set & operator=(skill_set const&) = delete;

    private:
        void invalidate(std::string const& tag, size_t slot) {
            if (m_book) {
                auto const& it = m_book->find(tag);
                if (it != m_book->end()) {
                    auto const& record = it->second;
                    // do not modify state, so it's not affected by serialization
                    m_skills[slot].assign_impact(std::get<1>(record));
                }
            }
        }

    private:
        std::vector<skill_type>     m_skills;   // array of learned/selected skills
        book_type const*            m_book;     // create and invalidate source
    };
}