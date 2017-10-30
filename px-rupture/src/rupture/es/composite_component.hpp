// name: composite_component.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#pragma once

#include <px/es/component.hpp>
#include <px/es/composition.hpp>
#include <px/es/persistency.hpp>
#include <px/es/link_dispatcher.hpp>

#include <string>

namespace px {

	class composite_component
		: public component
		, public composition
		, public link<transform_component>
		, public link_dispatcher<composite_component>
	{
	public:
		persistency lifetime() const noexcept
		{
			return endurance;
		}
		void set_persistency(persistency lifetime) noexcept
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
		void destroy() noexcept
		{
			endurance = persistency::destroying;
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

	protected:
		virtual void activate_component() override
		{
			enable();
		}
		virtual void deactivate_component() override
		{
			disable();
		}

	private:
		persistency endurance;
		std::string source;
	};
}