#pragma once

#include <Windows.h>
#include <oleacc.h>
#include "MSAccessibleLabel.h"


namespace ui
{
	class MSAccessibleProgress
		: public MSAccessibleLabel
	{
	public:
		MSAccessibleProgress(Control* pControl);
		virtual ~MSAccessibleProgress();
	public:

	};

}
