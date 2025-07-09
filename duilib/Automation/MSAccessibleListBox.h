#pragma once

#include <Windows.h>
#include <oleacc.h>
#include "MSAccessible.h"
#include "MSAccessibleScrollableBox.h"

namespace ui
{
	class MSAccessibleListBox
		: public MSAccessibleScrollableBox
	{
	public:
		MSAccessibleListBox(Control* pControl);
		~MSAccessibleListBox();

	};

}
