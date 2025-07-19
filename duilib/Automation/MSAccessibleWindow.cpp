#include "MSAccessibleWindow.h"
#include "UIlib.h"

namespace ui
{
	MSAccessibleWindow::MSAccessibleWindow(Window* pWindow)
		:m_pWindow(pWindow)
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
			Control* pc = this->m_pWindow->GetRoot();
			if (pc != nullptr) {
				MSAccessible* ms =pc->GetAccessible();
				if (ms != nullptr) {
					ms->TryReloadChildren();
					ms->AddRef();
					this->m_children.push_back(ms);
				}
			}
		}
	}
}
