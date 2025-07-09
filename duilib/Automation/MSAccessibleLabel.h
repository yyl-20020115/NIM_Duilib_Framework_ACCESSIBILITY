#pragma once

#include <Windows.h>
#include <oleacc.h>
#include "MSAccessibleControl.h"


namespace ui
{
	class MSAccessibleLabel
		: public MSAccessibleControl
	{
	public:
		MSAccessibleLabel(Control* pControl);
		virtual ~MSAccessibleLabel();
	public:

	};

}
