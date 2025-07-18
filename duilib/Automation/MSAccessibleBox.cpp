#include "MSAccessibleBox.h"

namespace ui
{
	MSAccessibleBox::MSAccessibleBox(Control* pControl)
		: MSAccessibleControl(pControl)
	{
	}
	MSAccessibleBox::~MSAccessibleBox()
	{
	}
	void MSAccessibleBox::TryReloadChildren()
	{
		__super::TryReloadChildren();
		Box* box = dynamic_cast<Box*>(this->m_pControl);
		if (box != nullptr) {
			for (int i = 0; i < box->GetCount(); i++) {
				MSAccessible* ms = box->GetItemAt(i)->GetAccessible();
				ms->TryReloadChildren();
				this->m_children.push_back(ms);
			}
		}
	}
}
