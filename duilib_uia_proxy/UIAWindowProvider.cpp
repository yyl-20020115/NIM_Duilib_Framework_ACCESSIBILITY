#include "StdAfx.h"

#if defined(ENABLE_UIAUTOMATION)

namespace ui_automation
{
	UIAWindowProvider::UIAWindowProvider(IAccessible* pWindow, HWND hWnd) :
		m_refCount(1),
		m_pWnd(pWindow),
		m_hWnd(hWnd)
	{
	}

	UIAWindowProvider::~UIAWindowProvider()
	{
		if (m_pWnd)
		{
			m_pWnd->Release();
			m_pWnd = nullptr;
		}
	}

	// IUnknown implementation.
	//
	IFACEMETHODIMP_(ULONG) UIAWindowProvider::AddRef()
	{
		return InterlockedIncrement(&m_refCount);
	}

	IFACEMETHODIMP_(ULONG) UIAWindowProvider::Release()
	{
		long val = InterlockedDecrement(&m_refCount);
		if (val == 0)
		{
			delete this;
		}
		return val;
	}

	IFACEMETHODIMP UIAWindowProvider::QueryInterface(REFIID riid, void** ppInterface)
	{
		if (riid == __uuidof(IUnknown))
			*ppInterface = static_cast<IRawElementProviderSimple*>(this);
		else if (riid == __uuidof(IRawElementProviderSimple))
			*ppInterface = static_cast<IRawElementProviderSimple*>(this);
		else if (riid == __uuidof(IRawElementProviderFragment))
			*ppInterface = static_cast<IRawElementProviderFragment*>(this);
		else if (riid == __uuidof(IRawElementProviderFragmentRoot))
			*ppInterface = static_cast<IRawElementProviderFragmentRoot*>(this);
		else if (riid == __uuidof(IWindowProvider))
			*ppInterface = static_cast<IWindowProvider*>(this);
		else if (riid == __uuidof(ITransformProvider))
			*ppInterface = static_cast<ITransformProvider*>(this);
		else
		{
			*ppInterface = nullptr;
			return E_NOINTERFACE;
		}

		(static_cast<IUnknown*>(*ppInterface))->AddRef();

		return S_OK;
	}


	// IRawElementProviderSimple implementation
	//
	// Implementation of IRawElementProviderSimple::get_ProviderOptions.
	// Gets UI Automation provider options.
	//
	IFACEMETHODIMP UIAWindowProvider::get_ProviderOptions(ProviderOptions* pRetVal)
	{
		UIA_CHECK_ELEMENT(m_pWnd);

		*pRetVal = static_cast<ProviderOptions>(ProviderOptions_ServerSideProvider |
			ProviderOptions_UseComThreading |
			0x100);//ProviderOptions_UseClientCoordinates;

		return S_OK;
	}

	// Implementation of IRawElementProviderSimple::get_PatternProvider.
	//
	IFACEMETHODIMP UIAWindowProvider::GetPatternProvider(PATTERNID patternId, IUnknown** pRetVal)
	{
		UIA_CHECK_ELEMENT(m_pWnd);

		*pRetVal = nullptr;
		if (patternId == UIA_WindowPatternId)
			*pRetVal = static_cast<IWindowProvider*>(this);
		else if (patternId == UIA_TransformPatternId)
			*pRetVal = static_cast<ITransformProvider*>(this);

		if (*pRetVal)
			AddRef();

		return S_OK;
	}

	// Implementation of IRawElementProviderSimple::get_PropertyValue.
	// Gets custom properties.
	//
	IFACEMETHODIMP UIAWindowProvider::GetPropertyValue(PROPERTYID propertyId, VARIANT* pRetVal)
	{
		UIA_CHECK_ELEMENT(m_pWnd);

		// Although it is hard-coded for the purposes of this sample, localizable 
		// text should be stored in, and loaded from, the resource file (.rc). 
		pRetVal->vt = VT_EMPTY;

		switch (propertyId)
		{
		case UIA_ProviderDescriptionPropertyId:
			pRetVal->bstrVal = SysAllocString(L"UIAWindowProvider");
			if (pRetVal->bstrVal != nullptr)
			{
				pRetVal->vt = VT_BSTR;
			}
			break;

		case UIA_ControlTypePropertyId:
			pRetVal->vt = VT_I4;
			pRetVal->lVal = UIA_WindowControlTypeId;
			break;

		case UIA_IsKeyboardFocusablePropertyId:
			pRetVal->vt = VT_BOOL;
			pRetVal->lVal = VARIANT_TRUE;
			break;
		}

		return S_OK;
	}

	// Implementation of IRawElementProviderSimple::get_HostRawElementProvider.
	// Gets the default UI Automation provider for the host window. This provider 
	// supplies many properties.
	//
	IFACEMETHODIMP UIAWindowProvider::get_HostRawElementProvider(IRawElementProviderSimple** pRetVal)
	{
		UIA_CHECK_ELEMENT(m_pWnd);

		return UiaHostProviderFromHwnd(m_hWnd, pRetVal);
	}


	// IRawElementProviderFragment implementation
	//
	// Implementation of IRawElementProviderFragment::Navigate.
	// Enables UI Automation to locate the element in the tree.
	// Navigation to the parent is handled by the host window provider.
	//
	IFACEMETHODIMP UIAWindowProvider::Navigate(NavigateDirection direction, IRawElementProviderFragment** pRetVal)
	{
		UIA_CHECK_ELEMENT(m_pWnd);

		IRawElementProviderFragment* pFrag = nullptr;
		if (direction == NavigateDirection_FirstChild
			&& nullptr != m_pWnd) {

			pFrag = static_cast<IRawElementProviderFragment*>(
				m_pWnd->GetRoot()->GetUIAProvider()
				);
		}

		*pRetVal = pFrag;

		if (pFrag != nullptr) {
			pFrag->AddRef();
		}

		return S_OK;
	}

	// Implementation of IRawElementProviderFragment::GetRuntimeId.
	// UI Automation gets this value from the host window provider, so supply nullptr here.
	//
	IFACEMETHODIMP UIAWindowProvider::GetRuntimeId(SAFEARRAY** pRetVal)
	{
		UIA_CHECK_ELEMENT(m_pWnd);

		*pRetVal = nullptr;

		int runtimeId[2] = { UiaAppendRuntimeId, 0 /* this is always 0 */ };
		*pRetVal = SafeArrayCreateVector(VT_I4, 0, ARRAYSIZE(runtimeId));
		if (*pRetVal != nullptr)
		{
			for (long index = 0; index < ARRAYSIZE(runtimeId); ++index)
			{
				SafeArrayPutElement(*pRetVal, &index, &runtimeId[index]);
			}
		}
		else
			return E_OUTOFMEMORY;

		return S_OK;
	}

	// Implementation of IRawElementProviderFragment::get_BoundingRectangle.
	//
	// Retrieves the screen location and size of the control. Controls hosted in
	// Win32 windows can return an empty rectangle; UI Automation will
	// retrieve the rectangle from the HWND provider. 
	//
	// UI Spy uses the bounding rectangle to draw a red border around the element.
	//
	IFACEMETHODIMP UIAWindowProvider::get_BoundingRectangle(UiaRect* pRetVal)
	{
		UIA_CHECK_ELEMENT(m_pWnd);

		RECT rect;
		GetClientRect(m_hWnd, &rect);
		InflateRect(&rect, -2, -2);
		POINT upperLeft;
		upperLeft.x = rect.left;
		upperLeft.y = rect.top;
		ClientToScreen(m_hWnd, &upperLeft);

		pRetVal->left = upperLeft.x;
		pRetVal->top = upperLeft.y;
		pRetVal->width = rect.right - rect.left;
		pRetVal->height = rect.bottom - rect.top;

		return S_OK;
	}

	// Implementation of IRawElementProviderFragment::GetEmbeddedFragmentRoots.
	// Retrieves other fragment roots that may be hosted in this one.
	//
	IFACEMETHODIMP UIAWindowProvider::GetEmbeddedFragmentRoots(SAFEARRAY** pRetVal)
	{
		UIA_CHECK_ELEMENT(m_pWnd);

		*pRetVal = nullptr;

		return S_OK;
	}

	// Implementation of IRawElementProviderFragment::SetFocus.
	// Responds to the control receiving focus through a UI Automation request.
	// For HWND-based controls, this is handled by the host window provider.
	//
	IFACEMETHODIMP UIAWindowProvider::SetFocus()
	{
		UIA_CHECK_ELEMENT(m_pWnd);
		::SetFocus(m_hWnd);
		return S_OK;
	}

	// Implementation of IRawElementProviderFragment::get_FragmentRoot.
	// Retrieves the root element of this fragment.
	//
	IFACEMETHODIMP UIAWindowProvider::get_FragmentRoot(IRawElementProviderFragmentRoot** pRetVal)
	{
		UIA_CHECK_ELEMENT(m_pWnd);

		*pRetVal = static_cast<IRawElementProviderFragmentRoot*>(this);

		AddRef();

		return S_OK;
	}

	// IRawElementProviderFragmentRoot implementation
	//
	// Implementation of IRawElementProviderFragmentRoot::ElementProviderFromPoint.
	// Retrieves the IRawElementProviderFragment interface for the item at the specified 
	// point (in client coordinates).
	// UI Spy uses this to determine what element is under the cursor when Ctrl is pressed.
	//
	IFACEMETHODIMP UIAWindowProvider::ElementProviderFromPoint(double x, double y, IRawElementProviderFragment** pRetVal)
	{
		UIA_CHECK_ELEMENT(m_pWnd);

		*pRetVal = nullptr;

		//auto pControl = m_pWnd->FindControl(pt);
		VARIANT varChild;
		VariantInit(&varChild);
		HRESULT hr = m_pWnd->accHitTest(x, y, &varChild);
		//TODO:
		IAccessible* pControl = nullptr;
		if (pControl != nullptr)
		{
			*pRetVal = static_cast<IRawElementProviderFragment*>(
				pControl->GetUIAProvider());
			(*pRetVal)->AddRef();
		}

		return S_OK;
	}

	// Implementation of IRawElementProviderFragmentRoot::GetFocus.
	//
	IFACEMETHODIMP UIAWindowProvider::GetFocus(IRawElementProviderFragment** pRetVal)
	{
		UIA_CHECK_ELEMENT(m_pWnd);

		*pRetVal = nullptr;
		VARIANT varChild;
		VariantInit(&varChild);

		HRESULT hr = m_pWnd->get_accFocus(&varChild);

		if (S_OK == hr && varChild.vt == VT_I4 && varChild.lVal == CHILDID_SELF)
		{
			*pRetVal = this;
			(*pRetVal)->AddRef();
			return S_OK;
		}

		return E_FAIL;
	}


	// IWindowProvider
	//
	IFACEMETHODIMP UIAWindowProvider::SetVisualState(WindowVisualState state)
	{
		UIA_CHECK_ELEMENT(m_pWnd);

		if (state == WindowVisualState::WindowVisualState_Minimized)
			SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
		else if (state == WindowVisualState::WindowVisualState_Maximized)
			SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		else if (state == WindowVisualState::WindowVisualState_Normal)
			SendMessage(m_hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);

		return S_OK;
	}

	IFACEMETHODIMP UIAWindowProvider::Close(void)
	{
		UIA_CHECK_ELEMENT(m_pWnd);

		if (m_hWnd != NULL)
			::CloseWindow(m_hWnd);
		return S_OK;
	}

	IFACEMETHODIMP UIAWindowProvider::WaitForInputIdle(int milliseconds, BOOL* pRetVal)
	{
		UIA_CHECK_ELEMENT(m_pWnd);

		Sleep(min(milliseconds, 200));

		*pRetVal = TRUE;

		return S_OK;
	}

	IFACEMETHODIMP UIAWindowProvider::get_CanMaximize(BOOL* pRetVal)
	{
		UIA_CHECK_ELEMENT(m_pWnd);

		*pRetVal = TRUE;

		return S_OK;
	}

	IFACEMETHODIMP UIAWindowProvider::get_CanMinimize(BOOL* pRetVal)
	{
		UIA_CHECK_ELEMENT(m_pWnd);

		*pRetVal = TRUE;

		return S_OK;
	}

	IFACEMETHODIMP UIAWindowProvider::get_IsModal(BOOL* pRetVal)
	{
		UIA_CHECK_ELEMENT(m_pWnd);

		*pRetVal = FALSE;

		return S_OK;
	}

	IFACEMETHODIMP UIAWindowProvider::get_WindowInteractionState(WindowInteractionState* pRetVal)
	{
		UIA_CHECK_ELEMENT(m_pWnd);

		*pRetVal = WindowInteractionState::WindowInteractionState_Running;

		return S_OK;
	}

	IFACEMETHODIMP UIAWindowProvider::get_WindowVisualState(WindowVisualState* pRetVal)
	{
		UIA_CHECK_ELEMENT(m_pWnd);

		if (::IsIconic(m_hWnd))
			*pRetVal = WindowVisualState::WindowVisualState_Minimized;
		else if (::IsZoomed(m_hWnd))
			*pRetVal = WindowVisualState::WindowVisualState_Maximized;
		else *pRetVal = WindowVisualState::WindowVisualState_Normal;

		return S_OK;
	}

	IFACEMETHODIMP UIAWindowProvider::get_IsTopmost(BOOL* pRetVal)
	{
		UIA_CHECK_ELEMENT(m_pWnd);

		*pRetVal = ::GetWindowLong(m_hWnd, GWL_EXSTYLE) & WS_EX_TOPMOST;

		return S_OK;
	}


	// ITransformProvider
	//
	IFACEMETHODIMP UIAWindowProvider::Move(double x, double y)
	{
		UIA_CHECK_ELEMENT(m_pWnd);

		::SetWindowPos(m_hWnd, nullptr, (int)x, (int)y, 0, 0,
			SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREDRAW);

		return S_OK;
	}

	IFACEMETHODIMP UIAWindowProvider::Resize(double width, double height)
	{
		UIA_CHECK_ELEMENT(m_pWnd);

		::SetWindowPos(m_hWnd, nullptr, 0, 0, (int)width, (int)height,
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREDRAW);

		return S_OK;
	}

	IFACEMETHODIMP UIAWindowProvider::Rotate(double degrees)
	{
		UIA_CHECK_ELEMENT(m_pWnd);

		return S_OK;
	}

	IFACEMETHODIMP UIAWindowProvider::get_CanMove(BOOL* pRetVal)
	{
		*pRetVal = FALSE;

		UIA_CHECK_ELEMENT(m_pWnd);
		RECT windowRect, clientRect;
		if (GetWindowRect(m_hWnd, &windowRect) && GetClientRect(m_hWnd, &clientRect)) {
			if (windowRect.bottom - windowRect.top > clientRect.bottom - clientRect.top) {
				*pRetVal = TRUE; // ÓÐ±êÌâÀ¸
			}
		}

		return S_OK;
	}

	IFACEMETHODIMP UIAWindowProvider::get_CanResize(BOOL* pRetVal)
	{
		*pRetVal = FALSE;

		UIA_CHECK_ELEMENT(m_pWnd);
		long left, top, width, height;
		VARIANT varChild;
		VariantInit(&varChild);
		varChild.vt = VT_EMPTY;

		HRESULT hr = m_pWnd->accLocation(&left, &top, &width, &height, varChild);

		*pRetVal = width || height;

		return hr;
	}

	IFACEMETHODIMP UIAWindowProvider::get_CanRotate(BOOL* pRetVal)
	{
		// do not support roate window for now

		*pRetVal = FALSE;

		UIA_CHECK_ELEMENT(m_pWnd);

		return S_OK;
	}


	HWND UIAWindowProvider::GetHwnd() const
	{
		return m_hWnd;
	}

}

#endif