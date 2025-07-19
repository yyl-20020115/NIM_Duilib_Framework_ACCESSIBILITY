#pragma once

#include <Windows.h>
#include <oleacc.h>
#include "MSAccessible.h"
#include "MSAccessibleControl.h"

namespace ui
{
	class MSAccessibleBox
		: public MSAccessibleControl
	{
	public:
		MSAccessibleBox(Control* pControl,MSAccessible* pParent);
		virtual ~MSAccessibleBox();
	public:

		void TryReloadChildren() override;

	};
}