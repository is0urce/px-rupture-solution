// name: panel.hpp

#pragma once

namespace px {

    class panel {
    public:
        void combine() {
            combine_panel();
        }

    public:
        virtual ~panel() = default;

    protected:
        virtual void combine_panel() = 0;
    };
}