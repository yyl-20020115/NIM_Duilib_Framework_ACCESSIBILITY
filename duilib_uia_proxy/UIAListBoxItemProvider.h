#ifndef UI_AUTOMATION_UIA_LISTBOX_ITEM_PROVIDER_H_
#define UI_AUTOMATION_UIA_LISTBOX_ITEM_PROVIDER_H_
#include "StdAfx.h"
#pragma once

#if defined(ENABLE_UIAUTOMATION)

namespace ui_automation
{

class UIAListBoxItemProvider :public UIACheckBoxProvider
{
public:
	UIAListBoxItemProvider() = delete;
	UIAListBoxItemProvider(IAccessible* pControl);

	// IUnknown methods
	virtual IFACEMETHODIMP QueryInterface(REFIID riid, void** ppInterface);

	// IRawElementProviderSimple methods
	virtual IFACEMETHODIMP GetPatternProvider(_In_ PATTERNID iid, _Outptr_result_maybenull_ IUnknown** retVal);
	virtual IFACEMETHODIMP GetPropertyValue(_In_ PROPERTYID propertyId, _Out_ VARIANT* pRetVal);

	// ISelectionItemProvider methods
	virtual IFACEMETHODIMP Select(void);
	virtual IFACEMETHODIMP AddToSelection(void);
	virtual IFACEMETHODIMP RemoveFromSelection(void);
	virtual IFACEMETHODIMP get_IsSelected(__RPC__out BOOL* pRetVal);
	virtual IFACEMETHODIMP get_SelectionContainer(__RPC__deref_out_opt IRawElementProviderSimple** pRetVal);

protected:
	virtual ~UIAListBoxItemProvider() {}
};

}

#endif

#endif