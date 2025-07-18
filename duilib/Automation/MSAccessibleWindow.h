#pragma once

#include <Windows.h>
#include <oleacc.h>
#include "MSAccessible.h"

namespace ui
{
	class Window;
	class MSAccessibleWindow
		: public MSAccessible
	{
	public:
		MSAccessibleWindow(Window* pWindow);
		virtual ~MSAccessibleWindow();
	public:
		void TryReloadChildren() override;

	protected:
		Window* m_pWindow;
	};
}
