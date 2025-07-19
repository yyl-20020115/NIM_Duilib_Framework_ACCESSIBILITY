#include "MSAccessibleTreeNode.h"

namespace ui
{
	MSAccessibleTreeNode::MSAccessibleTreeNode(Control* pControl, MSAccessible* pParent)
		:MSAccessibleListBoxItem(pControl,pParent)
	{
	}
	MSAccessibleTreeNode::~MSAccessibleTreeNode()
	{
	}
	void MSAccessibleTreeNode::TryReloadChildren()
	{
		__super::TryReloadChildren();
	}
}
