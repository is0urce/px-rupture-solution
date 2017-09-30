// name: main.cpp

#define GLEW_STATIC
#include <gl/glew.h>

#include "application/configuration.hpp"
#include "application/document.hpp"
#include "application/settings.hpp"
#include "key.hpp"
#include "shell.hpp"

#include <px/common/bindings.hpp>
#include <px/common/timer.hpp>
#include <px/dev/logger.hpp>
#include <px/dev/test.hpp>
#include <px/rglfw/rglfw.hpp>

#include <stdexcept>

namespace px {

	// create main window from configuration
	glfw_window create_window(configuration const& config, char const* name, GLFWmonitor * monitor)
	{
		if (monitor && config.fullscreen) {
			auto *const mode = glfwGetVideoMode(monitor);
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		}
		return glfwCreateWindow(config.width, config.height, name, monitor, nullptr);
	}

	// enable context and load opengl extensions
	void create_context(glfw_window const& win, int swap_interval)
	{
		glfwMakeContextCurrent(win); // context
		glfwSwapInterval(swap_interval);
		glewInit();	// OpenGL extensions, after context
	}

	void process()
	{
		glfw_instance glfw;

		// load settings

		auto config = configuration::from_document(document::load_document(settings::configuration_path));
		auto binds = bindings<int, key>::from_document(document::load_document(settings::bindings_path));

		auto monitor = glfwGetPrimaryMonitor();
		auto *const mode = glfwGetVideoMode(monitor);
		if (config.fullscreen) {
			config.width = mode->width;
			config.height = mode->height;
		}

		// create windows

		glfw_window win = create_window(config, settings::application_name.c_str(), config.fullscreen ? monitor : nullptr);
		create_context(win, config.vsync);
		shell game(config.width, config.height);

		// events

		glfw_callback event_callback(win);
		event_callback.on_resize([&](int widht, int height) {
			game.resize(widht, height);
		});
		event_callback.on_click([&](int mouse_button, int action, int /* mods */) {
			if (action == GLFW_PRESS) {
				game.click(mouse_button);
			}
		});
		event_callback.on_hover([&](double x, double y) {
			game.hover(static_cast<int>(x), static_cast<int>(y));
		});
		event_callback.on_scroll([&](double horisontal, double vertical) {
			game.scroll(horisontal, vertical);
		});
		event_callback.on_text([&](unsigned int codepoint) {
			game.text(codepoint);
		});
		event_callback.on_key([&](int os_code, int /* scancode */, int action, int /* mods */) {
			if (action == GLFW_PRESS || action == GLFW_REPEAT) {
				game.press(binds.get_or(os_code, key::not_valid));
			}
		});

		// main loop
		timer<glfw_time> time;
		while (win.process()) {
			game.frame(time);
		}
	}

	int run_application()
	{
		logger log(settings::log_path);

		try {
			process();
		}
		catch (std::exception const& exc) {
			log.message(exc.what());
			return -1;
		}
		catch (...) {
			log.message("unhandled exception");
			return -1;
		}

		return 0;
	}
}

void run_tests();

int main()
{
	run_tests();
	return px::run_application();
}