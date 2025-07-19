#include "MSAccessibleButton.h"

namespace ui
{
	MSAccessibleButton::MSAccessibleButton(Control* pControl, MSAccessible* pParent)
		:MSAccessibleLabel(pControl,pParent)
	{
	}
	MSAccessibleButton::~MSAccessibleButton()
	{
	}
	void MSAccessibleButton::TryReloadChildren()
	{
		__super::TryReloadChildren();
	}
}
