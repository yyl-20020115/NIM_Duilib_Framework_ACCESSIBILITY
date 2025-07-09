#pragma once

#include <Windows.h>
#include <oleacc.h>
#include "MSAccessible.h"
#include "MSAccessibleBox.h"

namespace ui
{
	class MSAccessibleScrollableBox
		: public MSAccessibleBox
	{
	public:
		MSAccessibleScrollableBox(Control* pControl);
		virtual ~MSAccessibleScrollableBox();
	public:

	};

}
