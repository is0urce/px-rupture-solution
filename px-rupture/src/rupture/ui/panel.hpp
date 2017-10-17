// name: panel.hpp

#pragma once

namespace px::ui {
	class panel
	{
	public:
		void combine()
		{
			combine_panel();
		}

	public:
		virtual ~panel()
		{
		}

	protected:
		virtual void combine_panel()
		{
		}
	};
}