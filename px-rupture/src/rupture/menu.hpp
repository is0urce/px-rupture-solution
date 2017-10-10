// name: menu.hpp

#pragma once

namespace px {

	class menu final
	{
	public:
		void toggle(bool /*visible*/)
		{

		}
		void resize(unsigned int w, unsigned int h)
		{
			width = w;
			height = h;
		}
		void draw()
		{

		}

	public:
		~menu()
		{
		}
		menu(unsigned int w, unsigned int h)
			: width(w)
			, height(h)
		{

		}

	private:
		unsigned int width;
		unsigned int height;
	};
}