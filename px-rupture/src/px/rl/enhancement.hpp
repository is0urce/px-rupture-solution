#pragma once

namespace px::rl {

	template <typename Effect, typename Integral = int, typename Real = double>
	class enhancement {
	public:
		Effect		main;
		Integral	sub;
		Integral	value0;
		Integral	value1;
		Real		magnitude0;
		Real		magnitude1;

	public:
		enhancement & operator +=(enhancement const& lh) {
			value0 += lh.value0;
			value1 += lh.value1;
			magnitude0 += lh.magnitude0;
			magnitude1 += lh.magnitude1;
			return *this;
		}

		template <typename Archive>
		void serialize(Archive & archive) {
			archive(main, sub, value0, value1, magnitude0, magnitude1);
		}

	public:
		static enhancement zero(Effect main) {
			return enhancement{ main, 0, 0, 0, 0, 0 };
		}
		static enhancement integral(Effect main, Integral sub, Integral v0, Integral v1) {
			return enhancement{ main, sub, v0, v1, 0, 0 };
		}
		static enhancement real(Effect main, Integral sub, Real v0, Real v1) {
			return enhancement{ main, sub, 0, 0, v0, v1 };
		}

	};
}