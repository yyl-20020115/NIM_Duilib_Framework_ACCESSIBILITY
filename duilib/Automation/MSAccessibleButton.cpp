#include "MSAccessibleButton.h"

namespace ui
{
	MSAccessibleButton::MSAccessibleButton(Control* pControl, MSAccessible* pParent)
		:MSAccessibleLabel(pControl, pParent)
	{
	}
	MSAccessibleButton::~MSAccessibleButton()
	{
	}
	HRESULT STDMETHODCALLTYPE MSAccessibleButton::accDoDefaultAction(VARIANT varChild)
	{
		if (varChild.vt != VT_I4 || varChild.lVal != CHILDID_SELF)
			return S_FALSE;
		if (!m_pControl) {
			Button* pButton = dynamic_cast<Button*>(m_pControl);
			if (pButton != nullptr) {
				pButton->Activate();
			}
			else {
				ButtonBox* pButtonBox = dynamic_cast<ButtonBox*>(m_pControl);
				if (pButtonBox != nullptr) {
					pButtonBox->Activate();
				}
			}
		}
		return E_NOTIMPL;
	}
	void MSAccessibleButton::TryReloadChildren()
	{
		__super::TryReloadChildren();
	}
}
