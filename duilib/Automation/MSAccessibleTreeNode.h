#pragma once

#include <Windows.h>
#include <oleacc.h>
#include "MSAccessibleListBoxItem.h"


namespace ui
{
	class MSAccessibleTreeNode
		: public MSAccessibleListBoxItem
	{
	public:
		MSAccessibleTreeNode(Control* pControl, MSAccessible* pParent);
		virtual ~MSAccessibleTreeNode();
	public:
		void TryReloadChildren() override;
	};

}
