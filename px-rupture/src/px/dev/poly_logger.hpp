// name: i_logger.hpp
// type: c++ class

#pragma once

// logs events

#include <string>

namespace px {

    class abstract_logger {
    public:
        void message(std::string const& text) {
            log_message(text);
        }

    public:
        virtual ~abstract_logger() = default;

    protected:
        virtual void log_message(std::string const& text) {
        }
    };
}