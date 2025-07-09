#pragma once

#include <Windows.h>
#include <oleacc.h>
#include "MSAccessible.h"

class Window;
namespace ui
{
	class MSAccessibleWindow
		: public MSAccessible
	{
	public:
		MSAccessibleWindow(Window* pWindow);
		virtual ~MSAccessibleWindow();
	public:

	protected:
		Window* m_pWindow;
	};
}
