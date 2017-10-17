// name: composite_component.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#pragma once

#include <px/es/component.hpp>
#include <px/es/composition.hpp>
#include <px/es/persistency.hpp>

#include <string>

namespace px {

	class composite_component
		: public component
		, public composition
	{
	public:
		persistency lifetime() const noexcept
		{
			return endurance;
		}
		void set_persistency(persistency lifetime)
		{
			endurance = lifetime;
		}
		std::string name()
		{
			return source;
		}
		void set_name(std::string resource_name)
		{
			source = resource_name;
		}

	public:
		virtual ~composite_component()
		{
		}
		composite_component()
			: endurance(persistency::serialized)
		{
		}
		composite_component(composite_component const&) = delete;
		composite_component& operator=(composite_component const&) = delete;

	private:
		persistency endurance;
		std::string source;
	};
}