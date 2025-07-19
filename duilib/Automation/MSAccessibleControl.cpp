#include "MSAccessibleControl.h"

namespace ui
{
	MSAccessibleControl::MSAccessibleControl(Control* pControl, MSAccessible* pParent)
		:MSAccessible(pParent)
		,m_pControl(pControl)
	{
		if (pControl != nullptr) {
			this->m_hWnd = pParent!=nullptr? pParent->GetWindowHandle() 
				: pControl->GetWindow()->GetHWND();
			this->m_name = ::SysAllocString(pControl->GetType().c_str());

		}
	}
	MSAccessibleControl::~MSAccessibleControl()
	{
	}
	
	HRESULT MSAccessibleControl::accLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight, VARIANT varChild)
	{
		if (!m_pControl) return E_FAIL;
		__super::accLocation(pxLeft, pyTop, pcxWidth, pcyHeight, varChild);

		//UiRect rc = m_pControl->GetPos();
		//if (pxLeft) *pxLeft += rc.left;
		//if (pyTop) *pyTop += rc.top;
		//if (pcxWidth) *pcxWidth = rc.GetWidth();
		//if (pcyHeight) *pcyHeight = rc.GetHeight();
		return S_OK;
	}
	void MSAccessibleControl::TryReloadChildren()
	{
		__super::TryReloadChildren();
	}
}
