#pragma once

#include <Windows.h>
#include <oleacc.h>
#include "MSAccessibleLabel.h"

namespace ui
{
	class MSAccessibleRichEdit
		: public MSAccessibleLabel
	{
	public:
		MSAccessibleRichEdit(Control* pControl, MSAccessible* pParent);
		virtual ~MSAccessibleRichEdit();
	public:


	};

}
