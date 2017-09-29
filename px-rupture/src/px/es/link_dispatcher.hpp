// name: link_dispatcher
// type: c++ header
// auth: is0urce
// desc: template class

#pragma once

// makes call overrides to multiple link<T> correct and simple

#include <px/es/link.hpp>

namespace px {

	template <typename C>
	class link_dispatcher
	{
	public:
		template <typename T>
		void connect(T * x) noexcept
		{
			link<T> * l = static_cast<C*>(this);
			l->set_link(x);
		}
		template <typename T>
		void disconnect() noexcept
		{
			link<T> * l = static_cast<C*>(this);
			l->break_link();
		}

		template <typename T>
		T * linked() const noexcept
		{
			link<T> const* l = static_cast<C const*>(this);
			return l->get_link();
		}
	};
}