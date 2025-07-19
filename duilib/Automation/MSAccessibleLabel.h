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
		MSAccessibleLabel(Control* pControl, MSAccessible* pParent);
		virtual ~MSAccessibleLabel();
	public:
		void TryReloadChildren() override;
	};

}
