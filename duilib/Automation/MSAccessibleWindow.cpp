#include "MSAccessibleWindow.h"
#include "UIlib.h"

namespace ui
{
	MSAccessibleWindow::MSAccessibleWindow(Window* pWindow, MSAccessible* pParent)
		:MSAccessible(pParent)
		,m_pWindow(pWindow)
	{
		if (pWindow != nullptr) {
			this->m_hWnd = pWindow->GetHWND();
		}
		else {
			this->m_hWnd = nullptr;
			this->m_name = nullptr;
		}
	}
	MSAccessibleWindow::~MSAccessibleWindow()
	{
	}
	void MSAccessibleWindow::TryReloadChildren()
	{
		__super::TryReloadChildren();
		if (this->m_pWindow != nullptr) {
			auto pc = this->m_pWindow->GetRoot();
			if (pc != nullptr) {
				auto ms =pc->GetAccessible();
				if (ms != nullptr) {
					ms->AddRef();
					ms->TryReloadChildren();
					this->m_children.push_back(ms);
				}
			}
		}
	}
}
