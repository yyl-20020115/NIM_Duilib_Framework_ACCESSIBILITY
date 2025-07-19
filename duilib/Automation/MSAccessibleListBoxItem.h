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
		MSAccessibleListBoxItem(Control* pControl, MSAccessible* pParent);
		virtual ~MSAccessibleListBoxItem();
	public:
		void TryReloadChildren() override;

	};

}
