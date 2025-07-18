#include "MSAccessibleControl.h"

namespace ui
{
	MSAccessibleControl::MSAccessibleControl(Control* pControl)
		:m_pControl(pControl)
	{
		this->m_hWnd = pControl->GetWindow()->GetHWND();
	}
	MSAccessibleControl::~MSAccessibleControl()
	{
	}
	void MSAccessibleControl::TryReloadChildren()
	{
		__super::TryReloadChildren();
	}
}
