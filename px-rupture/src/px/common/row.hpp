#pragma once

#include <stdexcept>

namespace px {

	template <typename T, size_t N>
	class row {
	public:
		using element_type = T;

	public:
		T * data() noexcept {
			return &raw;
		}

		T * begin() noexcept {
			return &reinterpret()[0];
		}
		T * end() noexcept {
			return &reinterpret()[cursor];
		}
		T const* begin() const noexcept {
			return &reinterpret()[0];
		}
		T const* end() const noexcept {
			return &reinterpret()[cursor];
		}
		T const* cbegin() const noexcept {
			return &reinterpret()[0];
		}
		T const* cend() const noexcept {
			return &reinterpret()[cursor];
		}
		void erase(T * ptr) {
			*ptr = std::move(back());
			back().~T();
			--cursor;
		}

		size_t size() const noexcept {
			return cursor;
		}

		T & back() {
			if (cursor == 0) throw std::runtime_error("px::row::back() - no elements");
			return reinterpret()[cursor - 1];
		}
		T const& back() const {
			if (cursor == 0) throw std::runtime_error("px::row::back() const - no elements");
			return reinterpret()[cursor - 1];
		}

		T & push_back(T const& value) {
			if (cursor == N) throw std::runtime_error("px::row::push_back(value) - out of memory");
			new (&(reinterpret()[++cursor])) T(value);
			return back();
		}

		T & push_back(T && value) {
			if (cursor == N) throw std::runtime_error("px::row::push_back(value) - out of memory");
			new (&(reinterpret()[++cursor])) T(std::forward<T>(value));
			return back();
		}

		template <typename ...Args>
		T & emplace_back(Args && ...args) {
			if (cursor == N) throw std::runtime_error("px::row::emplace_back(value) - out of memory");
			new (&(reinterpret()[++cursor])) T(std::forward<Args>(args)...);
			return back();
		}

		void pop() {
			if (cursor == 0) throw std::runtime_error("px::row::pop(value) - no elements");
			reinterpret()[cursor--].~T();
		}

		bool empty() const noexcept {
			return cursor == 0;
		}

		void clear() {
			for (auto i = begin(), l = end(); i != l; ++i) {
				i->~T();
			}
			cursor = 0;
		}

	public:
		~row() {
			clear();
		}
		row()
			: cursor(0)
		{
		}
		row(row const& that)
			: cursor(0)
		{
			T * arr = reinterpret();
			for (auto i = that.begin(), l = that.end(); i != l; ++i) {
				new (&arr[++cursor]) T(*i);
			}
		}
		row(row && that)
			: cursor(0)
		{
			T * arr = reinterpret();
			for (auto i = that.begin(), l = that.end(); i != l; ++i) {
				new (&arr[++cursor]) T(std::move(*i));
			}
		}
		row & operator=(row const& that)
		{
			clear();
			T * arr = reinterpret();
			for (auto i = that.begin(), l = that.end(); i != l; ++i) {
				new (&arr[++cursor]) T(*i);
			}
			return *this;
		}
		row & operator=(row && that)
		{
			clear();
			T * arr = reinterpret();
			for (auto i = that.begin(), last = that.end(); i != last; ++i) {
				new (&arr[++cursor]) T(std::move(*i));
			}
			return *this;
		}

	private:
		T * reinterpret() noexcept {
			return reinterpret_cast<T*>(&raw[0]);
		}
		T const* reinterpret() const noexcept {
			return reinterpret_cast<T const*>(&raw[0]);
		}

	private:
		alignas(T) char			raw[N * sizeof T];
		size_t					cursor;
	};
}