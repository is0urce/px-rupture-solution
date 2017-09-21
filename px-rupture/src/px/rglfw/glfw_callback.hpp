// name: glfw_callback.hpp
// type: c++
// auth: is0urce
// desc: glfw state

#pragma once

// RAII glfw
// setter container for callback functions

#include <GLFW/glfw3.h>

#include <functional>

namespace px
{
	class glfw_callback final
	{
	public:
		template <typename Callable>
		void on_resize(Callable && fn)
		{
			m_resize = std::forward<Callable>(fn);
			glfwSetWindowSizeCallback(m_window, &glfw_on_resize);
		}
		template <typename Callable>
		void on_key(Callable && fn)
		{
			m_key = std::forward<Callable>(fn);
			glfwSetKeyCallback(m_window, &glfw_on_key);
		}
		template <typename Callable>
		void on_text(Callable && fn)
		{
			m_text = std::forward<Callable>(fn);
			glfwSetCharCallback(m_window, &glfw_on_text);
		}

		template <typename Callable>
		void on_click(Callable && fn)
		{
			m_click = std::forward<Callable>(fn);
			glfwSetMouseButtonCallback(m_window, &glfw_on_click);
		}
		template <typename Callable>
		void on_hover(Callable && fn)
		{
			m_hover = std::forward<Callable>(fn);
			glfwSetCursorPosCallback(m_window, &glfw_on_hover);
		}
		template <typename Callable>
		void on_scroll(Callable && fn)
		{
			m_scroll = std::forward<Callable>(fn);
			glfwSetScrollCallback(m_window, &glfw_on_scroll);
		}

	public:
		~glfw_callback()
		{
		}
		glfw_callback(GLFWwindow * window)
			: m_window(window)
		{
			glfwSetWindowUserPointer(window, this);
		}
		glfw_callback(glfw_callback const&) = delete;
		glfw_callback & operator=(glfw_callback const&) = delete;

	private:
		static glfw_callback * glfw_userdata(GLFWwindow * window)
		{
			return reinterpret_cast<px::glfw_callback*>(glfwGetWindowUserPointer(window));
		}
		static void glfw_on_resize(GLFWwindow * window, int width, int height)
		{
			glfw_userdata(window)->m_resize(width, height);
		}
		static void glfw_on_key(GLFWwindow * window, int key, int scancode, int action, int mods)
		{
			glfw_userdata(window)->m_key(key, scancode, action, mods);
		}
		static void glfw_on_text(GLFWwindow * window, unsigned int codepoint)
		{
			glfw_userdata(window)->m_text(codepoint);
		}
		static void glfw_on_click(GLFWwindow * window, int button, int action, int mods)
		{
			glfw_userdata(window)->m_click(button, action, mods);
		}
		static void glfw_on_hover(GLFWwindow * window, double x, double y)
		{
			glfw_userdata(window)->m_hover(x, y);
		}
		static void glfw_on_scroll(GLFWwindow * window, double horisontal, double vertical)
		{
			glfw_userdata(window)->m_scroll(horisontal, vertical);
		}

	private:
		GLFWwindow * m_window;
		std::function<void(int, int)> m_resize;			// width x height
		std::function<void(int, int, int, int)> m_key;	// key, scancode, action, mods
		std::function<void(unsigned int)> m_text;		// codepoint
		std::function<void(int, int, int)> m_click;		// 
		std::function<void(double, double)> m_hover;	// x, y
		std::function<void(double, double)> m_scroll;	// horisontal, vertical
	};
}