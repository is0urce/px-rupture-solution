// name: stream.hpp
// type: c++
// auth: is0urce
// desc: template class

#pragma once

#include <stdexcept>
#include <thread>

namespace px {

	template <typename Data>
	class stream {
	public:
		typedef Data data_type;

	public:
		bool is_loaded() const noexcept {
			return loaded;
		}
		void discard() {
			finish_loading();
			loaded = false;
		}
		void wait() {
			finish_loading();
			exception_check();
		}
		template <typename Operator>
		void load(Operator && function) {
			discard();
			function(userdata);
			loaded = true;
		}
		template <typename Operator>
		void load_async(Operator && function) {
			discard();
			// function closure taken by value
			thread = std::thread([this, function]() {
				try {
					function(userdata);
					loaded = true;
				}
				catch (...) {
					exc_ptr = std::current_exception();
				}
			});
		}
		data_type const* operator->() const {
			return &m_data;
		}
		data_type * operator->() {
			return &m_data;
		}
		data_type const* get() const {
			return &m_data;
		}
		data_type * get() {
			return &m_data;
		}
		data_type const& data() const {
			return m_data;
		}
		data_type & data() {
			return m_data;
		}

	public:
		~stream() {
			finish_loading();
		}
		stream()
			: loaded(false)
		{
		}
		stream(stream const&) = delete;
		stream & operator=(stream const&) = delete;

	private:
		void finish_loading() {
			if (m_thread.joinable()) {
				m_thread.join();
			}
			m_loaded = true;
		}
		void exception_check() const {
			if (exc_ptr) {
				try {
					std::rethrow_exception(exc_ptr);
				}
				catch (std::exception const& exc) {
					throw std::runtime_error(std::string("px::stream<T>::exception_check() - std::exceptiong thrown in async load, what = ") + exc.what());
				}
				catch (...) {
					throw std::runtime_error(std::string("px::stream<T>::exception_check() - unhandled exception thrown in async load"));
				}
			}
		}

	private:
		data_type			userdata;
		volatile bool		loaded;		// flag set is data loaded
		std::thread			thread;
		std::exception_ptr	exc_ptr;
	};
}