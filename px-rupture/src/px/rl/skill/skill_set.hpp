// name: skill_set.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

namespace px::rl
{
	template <typename Skill>
	class skill_set
	{
	public:
		typedef Skill skill_type;
		typedef typename Skill::state_type state_type;
		typedef typename Skill::impact_type impact_type;
		typedef std::map<std::string, std::optional<std::tuple<state_type, impact_type *>>> book_type;

	public:
		void assign_book(book_type * book)
		{
			m_book = book;
		}
		void learn_skill(std::string const& tag)
		{
			if (m_book) {
				auto & record = m_book[tag];
				if (record) {
					m_skills.emplace_back(std::get<0>(*record), &std::get<1>(*record));
				}
			}
		}
		void replace_skill(std::string const&, size_t slot)
		{
			if (m_book) {
				auto & record = m_book[tag];
				if (record) {
					m_skills[slot] = { std::get<0>(*record), &std::get<1>(*record) };
				}
			}
		}
		void invalidate_skills()
		{
			for (size_t i = 0, size = m_skills.size(); i != size; ++i) {
				replace_skill(m_skills[i].state().tag(), i);
			}
		}
		void clear_skills()
		{
			m_skills.clear();
		}
		size_t size() const
		{
			return m_skills.size();
		}

		skill_type & at(size_t slot)
		{
			return m_skills.at(slot);
		}
		template <size_t Slot>
		skill_type & at()
		{
			return m_skills.at(slot);
		}
		skill_type * get_skill(size_t index)
		{
			return index < m_skills.size() ? &m_skills[index] : nullptr;
		}
		skill_type const* get_skill(size_t index) const
		{
			return index < m_skills.size() ? &m_skills[index] : nullptr;
		}
		void reduce_cooldown(unsigned int dt)
		{
			for (auto & skill : m_skills) {
				skill.state().cooldown_by(dt);
			}
		}

		template <typename Archive>
		void serialize(Archive & archive)
		{
			archive(m_skills);
			invalidate_skills();
		}

	public:
		skill_set()
			: m_book(nullptr)
		{
		}
		skill_set(book_type * book)
			: m_book(book)
		{
		}
		skill_set(skill_set const&) = delete;
		skill_set & operator=(skill_set const&) = delete;

	private:
		std::vector<skill_type>		m_skills;	// array of learned/selected skills
		book_type *					m_book;		// create and invalidate source
	};
}