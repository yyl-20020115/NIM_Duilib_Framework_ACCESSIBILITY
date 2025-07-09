#pragma once

#include <Windows.h>
#include <oleacc.h>
#include "MSAccessibleButton.h"

namespace ui
{
	class MSAccessibleCheckBox
		: public MSAccessibleButton
	{
	public:
		MSAccessibleCheckBox(Control* pControl);
		virtual ~MSAccessibleCheckBox();
	public:

	};

}
