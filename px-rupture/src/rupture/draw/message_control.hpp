#pragma once

#include "message_data.hpp"

namespace px {

    class message_control {
    public:
        message_data const*     data;
        unsigned int            last_version;

    public:
        bool is_dirty() {
            return data && data->version != last_version;
        }

        void notify_cashing() {
            last_version = data ? data->version : 0;
        }

    public:
        message_control()
            : last_version()
            , data(nullptr) {
        }
    };
}