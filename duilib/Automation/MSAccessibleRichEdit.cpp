#include "MSAccessibleRichEdit.h"

namespace ui
{
	MSAccessibleRichEdit::MSAccessibleRichEdit(Control* pControl, MSAccessible* pParent)
		:MSAccessibleLabel(pControl,pParent)
	{
	}
	MSAccessibleRichEdit::~MSAccessibleRichEdit()
	{
	}
}
