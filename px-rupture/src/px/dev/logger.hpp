// name: logger.hpp
// type: c++ class

#pragma once

// logs events

#include <ostream>
#include <fstream>
#include <mutex>
#include <string>

namespace px {

    class logger final {
    public:
        void message(std::string const& text)
        {
            log_message(text);
        }

    public:
        ~logger()
        {
            close();
        }
        logger(std::string file)
            : m_name(file)
        {
        }
        logger(logger const& other) = delete;
        logger & operator=(logger const& other) = delete;

    private:
        void open()
        {
            if (!m_stream.is_open()) {
                m_stream.open(m_name, std::ios_base::out | std::ios_base::app);
            }
        }
        void close()
        {
            m_stream.close();
        }
        void log_message(std::string const& text)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            open();
            m_stream << text.c_str() << std::endl;
            m_stream.flush();
        }

    private:
        std::string     m_name;
        std::ofstream   m_stream;
        std::mutex      m_mutex;
    };
}