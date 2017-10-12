// name: recursive_shadow.hpp
// type: c++ header
// auth: is0urce
// desc: recursive shadowcasting

#pragma once

namespace px {

	class recursive_shadowcasting
	{
	public:
		template <typename Predicate, typename Light>
		static void light(int center_x, int center_y, unsigned int radius_size, Predicate && predicate_fn, Light && light_op)
		{
			// octants
			static int multipliers[4][8] = {
				{ 1, 0, 0, -1, -1, 0, 0, 1 },
				{ 0, 1, -1, 0, 0, -1, 1, 0 },
				{ 0, 1, 1, 0, 0, -1, -1, 0 },
				{ 1, 0, 0, 1, -1, 0, 0, -1 }
			};

			light_op(center_x, center_y);
			for (unsigned int i = 0; i != 8; ++i) {
				octant(center_x, center_y, radius_size, 1, 1.0, 0.0, multipliers[0][i], multipliers[1][i], multipliers[2][i], multipliers[3][i], predicate_fn, light_op); // calculate every octant
			}
		}

	private:
		template <typename Predicate, typename Light>
		static void octant(int start_x, int start_y, unsigned int radius_size, unsigned int row, float start, float end, int xx, int xy, int yx, int yy, Predicate && predicate, Light && light_op)
		{
			if (start < end) return;
			int radius2 = static_cast<int>(radius_size * radius_size);

			float next_start_slope = start;
			for (unsigned int i = row; i != radius_size; ++i) {
				int dx = 0 - i - 1;
				int dy = 0 - i;
				bool blocked = false;
				while (dx <= 0) {
					++dx;
					float l_slope = (dx - 0.5f) / (dy + 0.5f);
					float r_slope = (dx + 0.5f) / (dy - 0.5f);

					if (start < r_slope) {
						continue;
					}
					else if (end > l_slope) {
						break;
					}
					else {
						int current_x = start_x + dx * xx + dy * xy;
						int current_y = start_y + dx * yx + dy * yy;
						float slope = static_cast<float>(dx) / static_cast<float>(dy);

						// to avoid behind-collumn peek you have to see center of a tile to actualy see it (or it shoult be wall)
						bool wall = !predicate(current_x, current_y);
						bool center = start >= slope && slope >= end;
						if ((center || wall) && (dx * dx + dy * dy < radius2)) {
							light_op(current_x, current_y);
						}

						if (blocked) {
							if (wall) {
								next_start_slope = r_slope;
								continue;
							}
							else {
								blocked = false;
								start = next_start_slope;
							}
						}
						else {
							if (wall && i < radius_size) {
								blocked = true;
								octant(start_x, start_y, radius_size, i + 1, start, l_slope, xx, xy, yx, yy, predicate, light_op); // call recursive
								next_start_slope = r_slope;
							}
						}
					}
				}
				if (blocked) break;
			}
		}
	};
}