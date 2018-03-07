// name: enhancement.hpp

#pragma once

namespace px::rl {

    template <typename Effect, typename Integral = int, typename Real = double>
    class enhancement {
    public:
        using real_type = Real;
        using integer_type = Integral;

    public:
        Effect          main;
        integer_type    sub;
        integer_type    value0;
        integer_type    value1;
        real_type       magnitude0;
        real_type       magnitude1;

    public:
        enhancement & operator +=(enhancement const& lh) {
            value0 += lh.value0;
            value1 += lh.value1;
            magnitude0 += lh.magnitude0;
            magnitude1 *= lh.magnitude1;
            return *this;
        }

        template <typename Archive>
        void serialize(Archive & archive) {
            archive(main, sub, value0, value1, magnitude0, magnitude1);
        }

    public:
        static enhancement mixed(Effect main, Integral sub, Integral i0, Integral i1, Real v0, Real v1) {
            return enhancement{ main, sub, i0, i1, v0, v1 };
        }
        static enhancement mixed(Effect main, Integral sub, Integral i, Real v) {
            return mixed(main, sub, i, i, v, v);
        }
        static enhancement zero(Effect main, Integral subtype_value) {
            return mixed(main, subtype_value, 0, 0, 0, 0);
        }
        static enhancement zero(Effect main) {
            return zero(main, 0);
        }
        static enhancement integral(Effect main, Integral sub, Integral i0, Integral i1) {
            return mixed(main, sub, i0, i1, 0, 0);
        }
        static enhancement integral(Effect main, Integral sub, Integral i) {
            return mixed(main, sub, i, i, 0, 0);
        }
        static enhancement real(Effect main, Integral sub, Real v0, Real v1) {
            return mixed(main, sub, 0, 0, v0, v1);
        }
        static enhancement real(Effect main, Integral sub, Real v) {
            return mixed(main, sub, 0, 0, v, v);
        }
    };
}