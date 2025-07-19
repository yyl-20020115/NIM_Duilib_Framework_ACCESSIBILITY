#pragma once

#include <Windows.h>
#include <oleacc.h>
#include "MSAccessibleControl.h"


namespace ui
{
	class MSAccessibleScrollBar
		: public MSAccessibleControl
	{
	public:
		MSAccessibleScrollBar(Control* pControl, MSAccessible* pParent);
		virtual ~MSAccessibleScrollBar();
	public:

	};

}
