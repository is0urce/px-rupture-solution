// name: main.cpp
// type: c++
// auth: is0urce
// desc: programm entry point

#include <px/app/cfg.hpp>

#include "app/bootstrap.hpp"
#include "app/document.hpp"
#include "app/settings.hpp"
#include "draw/glew_options.hpp"
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
    glfw_window create_window(char const* name, bool is_fullscreen, unsigned int width, unsigned int height, GLFWmonitor * monitor) {
        if (monitor && is_fullscreen) {
            auto const* const mode = glfwGetVideoMode(monitor);
            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        }
        return glfwCreateWindow(width, height, name, monitor, nullptr);
    }

    // enable context and load opengl extensions
    void create_context(glfw_window const& win, int swap_interval) {
        glfwMakeContextCurrent(win);        // context
        glfwSwapInterval(swap_interval);
        glewInit();                         // OpenGL extensions, after context
    }

    // prepare and start main loop
    void process() {
        glfw_instance glfw;
        cfg config(settings::configuration_path);
        auto const binds = bindings<int, key>::from_document(document::load_document(settings::bindings_path));
        bool is_fullscreen = config["fullscreen"];
        int window_width = config["width"];
        int window_height = config["height"];
        int vsync = config["vsync"];

        // load settings

        auto const monitor = glfwGetPrimaryMonitor();
        auto const *const mode = glfwGetVideoMode(monitor);
        if (is_fullscreen) {
            window_width = mode->width;
            window_height = mode->height;
        }

        // create windows

        glfw_window win = create_window(settings::application_name, is_fullscreen, window_width, window_height, is_fullscreen ? monitor : nullptr);
        create_context(win, vsync);
        shell game(window_width, window_height, &config);

        // register events

        glfw_callback evt(win);
        evt.on_resize([&](int width, int height) {
            game.resize(width, height);
            // store size in configuration file
            config["width"] = width;
            config["height"] = height;
        });
        evt.on_click([&](int mouse_button, int action, int /* mods */) {
            game.click(mouse_button, action == GLFW_PRESS);
        });
        evt.on_hover([&](double x, double y) {
            game.hover(static_cast<int>(x), static_cast<int>(y));
        });
        evt.on_scroll([&](double horisontal, double vertical) {
            game.scroll(horisontal, vertical);
        });
        evt.on_text([&](unsigned int codepoint) {
            game.text(codepoint);
        });
        evt.on_key([&](int os_code, int /* scancode */, int action, int /* mods */) {
            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                game.press(binds.get_or(os_code, key::not_valid));
            }
        });

        // main loop

        timer<glfw_time> time;
        game.seed(static_cast<unsigned int>(time.raw()));
        while (game.is_running() && win.process()) {
            game.frame(time);
        }
    }

    int run_application() {
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

int main() {
    px::bootstrap();
    return px::run_application();
}