// name: pool.hpp
// type: c++ header
// desc: template class
// auth: is0urce

#pragma once

// object memory pool
// cashe coherent (or at least sequental) enumeration of living objects with On = elements currently allocated
// constant request() and release() methods
// for general performanse there is no internal support for concurrent requests and/or releases

#include <array>

#include <px/memory/uq_ptr.hpp>
#include <px/memory/op_release_block.hpp>

namespace px {

	template <typename T, size_t Size>
	class pool
	{
	public:
		typedef T element_type;
		typedef T* pointer;
		typedef uq_ptr<T> uq_ptr;
		
	public:
		struct links;
		class ptr_deleter;

	public:
		// returns nullptr if all object in pool were requested, full() returns true
		template <typename... Args>
		T * request(Args... args)
		{
			T * result = nullptr;
			links * rec = m_free;
			if (rec) {
				m_free = m_free->next_free; // pop free stack
				m_live = rec->prev_live == nullptr ? rec : m_live; // set this as root?

				// update links
				if (rec->next_live)	{
					rec->next_live->prev_live = rec;
				}
				if (rec->prev_live)	{
					rec->prev_live->next_live = rec;
				}

				// modify aux fields
				rec->live = true;
				++m_current;

				result = reinterpret_cast<T*>(&m_pool[(rec - &m_links[0]) * sizeof(T)]);
				create(*result, std::forward<Args>(args)...);
			}
			return result;
		}

		// pointer must be in correct range and alignment of pool
		// it's safe to release already released objects - it's an nop
		bool release(T * ptr)
		{
			bool flag = false;
			auto index = to_index(ptr);
			if (index >= 0 && index < Size)	{
				links * rec = &m_links[index];
				if (rec->live) { // ensure it's not double release
					flag = true;

					rec->next_free = m_free;
					m_free = rec; // push free stack
					m_live = m_live == rec ? rec->next_live : m_live; // not root anymore?

					// update links
					if (rec->prev_live) {
						rec->prev_live->next_live = rec->next_live;
					}
					if (rec->next_live) {
						rec->next_live->prev_live = rec->prev_live;
					}

					// modify aux fields
					rec->live = false;
					--m_current;

					destroy(*ptr);
				}
			}
			return flag;
		}

		template <typename... Args>
		uq_ptr make_uq(Args ... args)
		{
			T * ptr = request(std::forward<Args>(args)...);
			return { ptr, &m_links[to_index(ptr)].ctrl };
		}

		size_t size() const noexcept
		{
			return m_current;
		}
		bool full() const noexcept
		{
			return m_current == Size;
		}
		bool empty() const noexcept
		{
			return m_current == 0;
		}
		constexpr static size_t max_size() noexcept
		{
			return Size;
		}

		// checks only range of pointer, not correctness (i.e alignment etc)
		bool can_contain(T const* ptr) const noexcept
		{
			return ptr >= reinterpret_cast<T const*>(&m_pool[0])
				&& ptr <= reinterpret_cast<T const*>(&m_pool[(Size - 1) * sizeof(T)]);
		}
		bool contains(T const* ptr) const noexcept
		{
			return can_contain(ptr) && m_links[to_index(ptr)].live;
		}

		template <typename UnaryFunction>
		void enumerate(UnaryFunction && func)
		{
			for (links * i = m_live; i != nullptr; i = i->next_live)	{
				func(reinterpret_cast<T&>(m_pool[(i - &m_links[0]) * sizeof(T)]));
			}
		}
		template <typename UnaryFunction >
		void enumerate(UnaryFunction && func) const
		{
			for (links const* i = m_live; i != nullptr; i = i->next_live) {
				func(reinterpret_cast<T const&>(m_pool[(i - &m_links[0]) * sizeof(T)]));
			}
		}

		void clear()
		{
			destroy_existing();
			startup();
		}

	public:
		~pool()
		{
			destroy_existing();
		}
		pool()
		{
			startup();
		}
		pool(pool const&) = delete;
		pool & operator=(pool const&) = delete;

	private:
		void destroy_existing()
		{
			for (links * i = m_live; i != nullptr; i = i->next_live)	{
				destroy(reinterpret_cast<T&>(m_pool[(i - &m_links[0]) * sizeof(T)]));
			}
		}
		void startup() noexcept
		{
			m_current = 0;
			m_free = &m_links[0];
			m_live = nullptr;

			for (size_t i = 0; i != Size; ++i) {
				m_links[i].next_free = i == Size - 1 ? nullptr : &m_links[i + 1];
				m_links[i].prev_live = i == 0 ? nullptr : &m_links[i - 1];
				m_links[i].next_live = nullptr;
				m_links[i].live = false;
				m_links[i].ctrl = op_release_block<T, ptr_deleter>(from_index(i), ptr_deleter(this));
			}
		}
		template <typename... Args>
		void create(T & item, Args... args)
		{
			new (&item) T(std::forward<Args>(args)...);
		}
		void destroy(T & item)
		{
			item.~T();
			//item; // 'item': unreferenced formal parameter if there is no destructor -> referencing
		}
		T * from_index(size_t index) noexcept
		{
			return reinterpret_cast<T*>(&m_pool[index * sizeof(T)]);
		}
		size_t to_index(T const* ptr) const noexcept
		{
			return ptr - reinterpret_cast<T const*>(&m_pool[0]);
		}

	public:
		class ptr_deleter final
		{
		public:
			void operator()(T * ptr) // for lambda
			{
				source->release(ptr);
			}
		public:
			ptr_deleter(pool * current) noexcept
				: source(current)
			{
			}
			ptr_deleter() noexcept
				: ptr_deleter(nullptr)
			{
			}
		private:
			pool * source;
		};

		struct links
		{
			links * next_free;
			links * next_live;
			links * prev_live;
			bool live; // auxiliary state to avoid double released
			op_release_block<T, ptr_deleter> ctrl; // for smart ptrs
		};

	private:
		alignas(T) std::array<char, sizeof(T) * Size> m_pool;
		std::array<links, Size>	m_links;
		size_t					m_current;	// cashed number of living objest for fast size queries
		links *					m_free;		// first free node (root)
		links *					m_live;		// first living node (root)
	};
}