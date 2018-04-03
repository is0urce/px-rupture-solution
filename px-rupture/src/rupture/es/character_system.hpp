// name: character_system.hpp

#pragma once

#include <px/es/system.hpp>
#include <px/es/delta.hpp>

#include <px/memory/uq_ptr.hpp>

namespace px {

    class character_component;
    class character_works;
    class script;

    class character_system
        : public system<delta>
    {
    public:
        uq_ptr<character_component> make();
        void load(script * scripts);

    public:
        virtual ~character_system() override;
        character_system();

    protected:
        virtual void turn_update_system(delta_type const& delta_time) override;

    private:
        uq_ptr<character_works> works;
    };
}