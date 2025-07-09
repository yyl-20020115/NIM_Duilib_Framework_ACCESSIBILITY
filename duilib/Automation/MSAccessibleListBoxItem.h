#pragma once

#include <Windows.h>
#include <oleacc.h>
#include "MSAccessibleCheckBox.h"


namespace ui
{
	class MSAccessibleListBoxItem
		: public MSAccessibleCheckBox
	{
	public:
		MSAccessibleListBoxItem(Control* pControl);
		virtual ~MSAccessibleListBoxItem();
	public:

	};

}
