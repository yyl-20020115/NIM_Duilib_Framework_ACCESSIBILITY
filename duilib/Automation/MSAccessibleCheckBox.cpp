#include "MSAccessibleCheckBox.h"

namespace ui
{
	MSAccessibleCheckBox::MSAccessibleCheckBox(Control* pControl, MSAccessible* pParent)
		:MSAccessibleButton(pControl,pParent)
	{
	}
	MSAccessibleCheckBox::~MSAccessibleCheckBox()
	{
	}
	void MSAccessibleCheckBox::TryReloadChildren()
	{
		__super::TryReloadChildren();
	}

}
