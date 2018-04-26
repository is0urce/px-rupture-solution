// name: glfw_time.cpp
// type: c++
// auth: is0urce
// desc: glfw lambda

#pragma once

// glfw time querry operator

#include "glfw_options.hpp"

namespace px {

    struct glfw_time final {
        double operator()() const {
            return glfwGetTime();
        }
    };
}