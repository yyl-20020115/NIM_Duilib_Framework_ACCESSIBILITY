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
	
	HRESULT STDMETHODCALLTYPE MSAccessibleControl::accLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight, VARIANT varChild)
	{
		if (!pxLeft || !pyTop || !pcxWidth || !pcyHeight)
			return E_INVALIDARG;

		*pxLeft = *pyTop = *pcxWidth = *pcyHeight = 0;

		// 只支持自身（CHILDID_SELF）
		if (varChild.vt != VT_I4 || varChild.lVal != CHILDID_SELF)
			return S_FALSE;

		if (!m_hWnd)
			return S_FALSE;

		// 获取窗口矩形（屏幕坐标）
		RECT rcWnd = { 0 };
		if (!::GetWindowRect(m_hWnd, &rcWnd))
			return S_FALSE;
		float ratio = GetScreenDisplayScale();
		UiRect r = this->m_pControl->GetPos();

		*pxLeft = (int)((rcWnd.left + r.left) * ratio);
		*pyTop = (int)((rcWnd.right + r.top) * ratio);
		*pcxWidth = (int)((r.right-r.left) * ratio);
		*pcyHeight = (int)((r.bottom-r.top) * ratio);

		return S_OK;
	}
	HRESULT STDMETHODCALLTYPE MSAccessibleControl::accHitTest(long xLeft, long yTop, VARIANT* pvarChild)
	{
		if (!pvarChild)
			return E_INVALIDARG;

		VariantInit(pvarChild);

		// 检查窗口句柄
		if (!m_hWnd)
			return S_FALSE;

		// 获取窗口矩形（屏幕坐标）
		RECT rcWnd = { 0 };
		if (!::GetWindowRect(m_hWnd, &rcWnd))
			return S_FALSE;
		float ratio = GetScreenDisplayScale();
		UiRect r = this->m_pControl->GetPos();

		rcWnd.left = (int)((rcWnd.left +r.left)* ratio);
		rcWnd.top = (int)((rcWnd.right +r.top) * ratio);
		rcWnd.right = (int)((rcWnd.left + r.right) * ratio);
		rcWnd.bottom = (int)((rcWnd.top + r.bottom) * ratio);

		// 判断点是否在窗口内
		if (xLeft < rcWnd.left || xLeft >= rcWnd.right ||
			yTop < rcWnd.top || yTop >= rcWnd.bottom)
		{
			return S_FALSE;
		}

		// 如果没有子元素，返回自身
		if (m_children.empty()) {
			pvarChild->vt = VT_I4;
			pvarChild->lVal = CHILDID_SELF;
			return S_OK;
		}

		// 遍历子元素，递归调用 accHitTest
		for (size_t i = 0; i < m_children.size(); ++i) {
			auto pChild = m_children[i];
			if (!pChild)
				continue;

			VARIANT varHit = {};
			HRESULT hr = pChild->accHitTest(xLeft, yTop, &varHit);
			if (SUCCEEDED(hr) && varHit.vt != VT_EMPTY && varHit.vt != VT_NULL && varHit.vt != VT_I4 /* not S_FALSE */) {
				// 命中子元素，返回子元素
				*pvarChild = varHit;
				return S_OK;
			}
			// 如果命中的是子对象本身
			if (SUCCEEDED(hr) && varHit.vt == VT_I4 && varHit.lVal == CHILDID_SELF) {
				pvarChild->vt = VT_DISPATCH;
				pChild->AddRef();
				pvarChild->pdispVal = pChild;
				return S_OK;
			}
			VariantClear(&varHit);
		}

		// 没有命中任何子元素，返回自身
		pvarChild->vt = VT_I4;
		pvarChild->lVal = CHILDID_SELF;
		return S_OK;
	}
	HRESULT __stdcall MSAccessibleControl::get_accName(VARIANT varChild, BSTR* pszName)
	{
		if (this->m_pControl == nullptr) return E_FAIL;
		if (!pszName) return E_INVALIDARG;
		*pszName = nullptr;

		// 只支持自身（CHILDID_SELF）
		if (varChild.vt != VT_I4 || varChild.lVal != CHILDID_SELF)
			return S_FALSE;

		if (!m_hWnd)
			return S_FALSE;
		if (this->m_name == nullptr) {
			this->m_name = ::SysAllocString(this->m_pControl->GetName().c_str());

		}
		if (this->m_name != nullptr) {
			*pszName = ::SysAllocString(this->m_name);
			return (*pszName) ? S_OK : E_OUTOFMEMORY;
		}

		return S_FALSE;


		return S_OK;
	}
	void MSAccessibleControl::TryReloadChildren()
	{
		__super::TryReloadChildren();
	}
}
