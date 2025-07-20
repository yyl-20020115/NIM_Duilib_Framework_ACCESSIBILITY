#ifndef UI_AUTOMATION_UIA_CONTROL_PROVIDER_H_
#define UI_AUTOMATION_UIA_CONTROL_PROVIDER_H_
#include "StdAfx.h"

#pragma once

#if defined(ENABLE_UIAUTOMATION)

namespace ui
{

class UIAControlProvider : 
	public IRawElementProviderSimple,
	public IRawElementProviderFragment,
	public IValueProvider,
	public IInvokeProvider,
	public IRangeValueProvider,
	public IToggleProvider,
	public IItemContainerProvider,
	public IScrollProvider,
	public ISelectionProvider,
	public ISelectionItemProvider,
	public IExpandCollapseProvider
{
public:
	UIAControlProvider() = delete;
	UIAControlProvider(IAccessible* pControl, UIAWindowProvider* pWindow = nullptr);

	void ResetControl(IAccessible* pControl = nullptr);

	IAccessible* GetControl() const;

	// IUnknown methods
	virtual IFACEMETHODIMP_(ULONG) AddRef();
	virtual IFACEMETHODIMP_(ULONG) Release();
	virtual IFACEMETHODIMP QueryInterface(REFIID riid, void** ppInterface);

	// IRawElementProviderSimple methods
	virtual IFACEMETHODIMP get_ProviderOptions(_Out_ ProviderOptions* retVal);
	virtual IFACEMETHODIMP GetPatternProvider(_In_ PATTERNID iid, _Outptr_result_maybenull_ IUnknown** retVal);
	virtual IFACEMETHODIMP GetPropertyValue(_In_ PROPERTYID idProp, _Out_ VARIANT* retVal);
	virtual IFACEMETHODIMP get_HostRawElementProvider(_Outptr_result_maybenull_ IRawElementProviderSimple** retVal);

	// IRawElementProviderFragment methods
	virtual IFACEMETHODIMP Navigate(_In_ NavigateDirection direction, _Outptr_result_maybenull_ IRawElementProviderFragment** retVal);
	virtual IFACEMETHODIMP GetRuntimeId(_Outptr_result_maybenull_ SAFEARRAY** retVal);
	virtual IFACEMETHODIMP get_BoundingRectangle(_Out_ UiaRect* retVal);
	virtual IFACEMETHODIMP GetEmbeddedFragmentRoots(_Outptr_result_maybenull_ SAFEARRAY** retVal);
	virtual IFACEMETHODIMP SetFocus();
	virtual IFACEMETHODIMP get_FragmentRoot(_Outptr_result_maybenull_ IRawElementProviderFragmentRoot** retVal);

	// IValueProvider methods
	virtual IFACEMETHODIMP SetValue(_In_ LPCWSTR val);
	virtual IFACEMETHODIMP get_Value(_Out_ BSTR* retVal);
	virtual IFACEMETHODIMP get_IsReadOnly(_Out_ BOOL* retVal);

	// IInvokeProvider methods
	virtual IFACEMETHODIMP Invoke(void);

	// IRangeValueProvider methods
	virtual IFACEMETHODIMP SetValue(double val);
	virtual IFACEMETHODIMP get_Value(__RPC__out double* pRetVal);
	virtual IFACEMETHODIMP get_Maximum(__RPC__out double* pRetVal);
	virtual IFACEMETHODIMP get_Minimum(__RPC__out double* pRetVal);
	virtual IFACEMETHODIMP get_LargeChange(__RPC__out double* pRetVal);
	virtual IFACEMETHODIMP get_SmallChange(__RPC__out double* pRetVal);
	virtual IFACEMETHODIMP Toggle(void);
	virtual IFACEMETHODIMP get_ToggleState(__RPC__out enum ToggleState* pRetVal);

	// IItemContainerProvider methods
	virtual IFACEMETHODIMP FindItemByProperty(__RPC__in_opt IRawElementProviderSimple* pStartAfter,
		PROPERTYID propertyId, VARIANT value,
		__RPC__deref_out_opt IRawElementProviderSimple** pFound);

	// IScrollProvider methods
	virtual IFACEMETHODIMP Scroll(enum ScrollAmount horizontalAmount, enum ScrollAmount verticalAmount);
	virtual IFACEMETHODIMP SetScrollPercent(double horizontalPercent, double verticalPercent);
	virtual IFACEMETHODIMP get_HorizontalScrollPercent(__RPC__out double* pRetVal);
	virtual IFACEMETHODIMP get_VerticalScrollPercent(__RPC__out double* pRetVal);
	virtual IFACEMETHODIMP get_HorizontalViewSize(__RPC__out double* pRetVal);
	virtual IFACEMETHODIMP get_VerticalViewSize(__RPC__out double* pRetVal);
	virtual IFACEMETHODIMP get_HorizontallyScrollable(__RPC__out BOOL* pRetVal);
	virtual IFACEMETHODIMP get_VerticallyScrollable(__RPC__out BOOL* pRetVal);

	// ISelectionProvider methods
	virtual IFACEMETHODIMP GetSelection(__RPC__deref_out_opt SAFEARRAY** pRetVal);
	virtual IFACEMETHODIMP get_CanSelectMultiple(__RPC__out BOOL* pRetVal);
	virtual IFACEMETHODIMP get_IsSelectionRequired(__RPC__out BOOL* pRetVal);

	// ISelectionItemProvider methods
	virtual IFACEMETHODIMP Select(void);
	virtual IFACEMETHODIMP AddToSelection(void);
	virtual IFACEMETHODIMP RemoveFromSelection(void);
	virtual IFACEMETHODIMP get_IsSelected(__RPC__out BOOL* pRetVal);
	virtual IFACEMETHODIMP get_SelectionContainer(__RPC__deref_out_opt IRawElementProviderSimple** pRetVal);

	// IExpandCollapseProvider METHODS
	virtual IFACEMETHODIMP Expand(void);
	virtual IFACEMETHODIMP Collapse(void);
	virtual IFACEMETHODIMP get_ExpandCollapseState(__RPC__out enum ExpandCollapseState* pRetVal);

	virtual UIAWindowProvider* GetWindow();
	virtual HWND GetHwnd();

protected:
	virtual ~UIAControlProvider() {};

protected:
	// Ref counter for this COM object.
	ULONG m_refCount;
	IAccessible* m_pControl;
	UIAWindowProvider* m_pWindow;
};

}
#else
namespace ui
{
class UIAControlProvider
{
public:
  UIAControlProvider() = delete;

  UIAControlProvider(Control* pControl) :m_refCount(1)
  {
    m_pControl = pControl;
  }

  void ResetControl(Control* pControl)
  {
    m_pControl = pControl;
  }

  Control* GetControl() const
  {
    return m_pControl;
  }

protected:
  virtual ~UIAControlProvider() {};

protected:
  // Ref counter for this COM object.
  ULONG m_refCount;

  Control* m_pControl;
};
}

#endif


#endif