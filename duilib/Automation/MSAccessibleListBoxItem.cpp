#include "MSAccessibleListBoxItem.h"

namespace ui
{
	MSAccessibleListBoxItem::MSAccessibleListBoxItem(Control* pControl, MSAccessible* pParent)
		:MSAccessibleCheckBox(pControl, pParent)
	{
	}
	MSAccessibleListBoxItem::~MSAccessibleListBoxItem()
	{
	}
	void MSAccessibleListBoxItem::TryReloadChildren()
	{
		__super::TryReloadChildren();
	}
}
