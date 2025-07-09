#pragma once

#include <Windows.h>
#include <oleacc.h>
#include "MSAccessible.h"
#include "MSAccessibleControl.h"

namespace ui
{
	class MSAccessibleComboBox
		: public MSAccessibleControl
	{
	public:
		MSAccessibleComboBox(Control* pControl);
		virtual ~MSAccessibleComboBox();
	public:

	};

}
