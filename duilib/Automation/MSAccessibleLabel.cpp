#include "MSAccessibleLabel.h"

namespace ui
{
	MSAccessibleLabel::MSAccessibleLabel(Control* pControl, MSAccessible* pParent)
		:MSAccessibleControl(pControl, pParent)
	{
	}
	MSAccessibleLabel::~MSAccessibleLabel()
	{
	}
	void MSAccessibleLabel::TryReloadChildren()
	{
		__super::TryReloadChildren();
	}
}
