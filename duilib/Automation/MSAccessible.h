#pragma once

#include <Windows.h>
#include <oleacc.h>
#include <vector>

namespace ui
{
	class MSAccessible 
        : public IAccessible
		, public IEnumVARIANT
	{
	public:
		MSAccessible();
		virtual ~MSAccessible();
    public:
		void SetRootAndParent(HWND hWnd, IAccessible* pRoot, IAccessible* pParent);
        IAccessible* GetRoot() const;
		IAccessible* GetParent() const;
    public:
        HWND GetWindowHandle() const;
    public:
        HRESULT STDMETHODCALLTYPE QueryInterface(
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

        ULONG STDMETHODCALLTYPE AddRef(void) override;

        ULONG STDMETHODCALLTYPE Release(void) override;

    public:
        /* [local] */ HRESULT STDMETHODCALLTYPE Next(
            /* [in] */ ULONG celt,
            /* [length_is][size_is][out] */ VARIANT* rgVar,
            /* [out] */ ULONG* pCeltFetched) override;

        HRESULT STDMETHODCALLTYPE Skip(
            /* [in] */ ULONG celt) override;

        HRESULT STDMETHODCALLTYPE Reset(void) override;

        HRESULT STDMETHODCALLTYPE Clone(
            /* [out] */ __RPC__deref_out_opt IEnumVARIANT** ppEnum) override;

	public:

        HRESULT STDMETHODCALLTYPE get_accParent(
            /* [retval][out] */ __RPC__deref_out_opt IDispatch** ppdispParent) override;

        HRESULT STDMETHODCALLTYPE get_accChildCount(
            /* [retval][out] */ __RPC__out long* pcountChildren) override;

        HRESULT STDMETHODCALLTYPE get_accChild(
            /* [in] */ VARIANT varChild,
            /* [retval][out] */ __RPC__deref_out_opt IDispatch** ppdispChild) override;

        HRESULT STDMETHODCALLTYPE get_accName(
            /* [optional][in] */ VARIANT varChild,
            /* [retval][out] */ __RPC__deref_out_opt BSTR* pszName) override;

        HRESULT STDMETHODCALLTYPE get_accValue(
            /* [optional][in] */ VARIANT varChild,
            /* [retval][out] */ __RPC__deref_out_opt BSTR* pszValue) override;

        HRESULT STDMETHODCALLTYPE get_accDescription(
            /* [optional][in] */ VARIANT varChild,
            /* [retval][out] */ __RPC__deref_out_opt BSTR* pszDescription) override;

        HRESULT STDMETHODCALLTYPE get_accRole(
            /* [optional][in] */ VARIANT varChild,
            /* [retval][out] */ __RPC__out VARIANT* pvarRole) override;

        HRESULT STDMETHODCALLTYPE get_accState(
            /* [optional][in] */ VARIANT varChild,
            /* [retval][out] */ __RPC__out VARIANT* pvarState) override;

        HRESULT STDMETHODCALLTYPE get_accHelp(
            /* [optional][in] */ VARIANT varChild,
            /* [retval][out] */ __RPC__deref_out_opt BSTR* pszHelp) override;

        HRESULT STDMETHODCALLTYPE get_accHelpTopic(
            /* [out] */ __RPC__deref_out_opt BSTR* pszHelpFile,
            /* [optional][in] */ VARIANT varChild,
            /* [retval][out] */ __RPC__out long* pidTopic) override;

        HRESULT STDMETHODCALLTYPE get_accKeyboardShortcut(
            /* [optional][in] */ VARIANT varChild,
            /* [retval][out] */ __RPC__deref_out_opt BSTR* pszKeyboardShortcut) override;

        HRESULT STDMETHODCALLTYPE get_accFocus(
            /* [retval][out] */ __RPC__out VARIANT* pvarChild) override;

        HRESULT STDMETHODCALLTYPE get_accSelection(
            /* [retval][out] */ __RPC__out VARIANT* pvarChildren) override;

        HRESULT STDMETHODCALLTYPE get_accDefaultAction(
            /* [optional][in] */ VARIANT varChild,
            /* [retval][out] */ __RPC__deref_out_opt BSTR* pszDefaultAction) override;

        HRESULT STDMETHODCALLTYPE accSelect(
            /* [in] */ long flagsSelect,
            /* [optional][in] */ VARIANT varChild) override;

        HRESULT STDMETHODCALLTYPE accLocation(
            /* [out] */ __RPC__out long* pxLeft,
            /* [out] */ __RPC__out long* pyTop,
            /* [out] */ __RPC__out long* pcxWidth,
            /* [out] */ __RPC__out long* pcyHeight,
            /* [optional][in] */ VARIANT varChild) override;

        HRESULT STDMETHODCALLTYPE accNavigate(
            /* [in] */ long navDir,
            /* [optional][in] */ VARIANT varStart,
            /* [retval][out] */ __RPC__out VARIANT* pvarEndUpAt) override;

        HRESULT STDMETHODCALLTYPE accHitTest(
            /* [in] */ long xLeft,
            /* [in] */ long yTop,
            /* [retval][out] */ __RPC__out VARIANT* pvarChild) override;

        HRESULT STDMETHODCALLTYPE accDoDefaultAction(
            /* [optional][in] */ VARIANT varChild) override;

        HRESULT STDMETHODCALLTYPE put_accName(
            /* [optional][in] */ VARIANT varChild,
            /* [in] */ __RPC__in BSTR szName) override;

        HRESULT STDMETHODCALLTYPE put_accValue(
            /* [optional][in] */ VARIANT varChild,
            /* [in] */ __RPC__in BSTR szValue) override;

        HRESULT STDMETHODCALLTYPE GetTypeInfoCount(
            /* [out] */ __RPC__out UINT* pctinfo) override;

        HRESULT STDMETHODCALLTYPE GetTypeInfo(
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ __RPC__deref_out_opt ITypeInfo** ppTInfo) override;

        HRESULT STDMETHODCALLTYPE GetIDsOfNames(
            /* [in] */ __RPC__in REFIID riid,
            /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR* rgszNames,
            /* [range][in] */ __RPC__in_range(0, 16384) UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID* rgDispId) override;

        /* [local] */ 
        HRESULT STDMETHODCALLTYPE Invoke(
            /* [annotation][in] */
            _In_  DISPID dispIdMember,
            /* [annotation][in] */
            _In_  REFIID riid,
            /* [annotation][in] */
            _In_  LCID lcid,
            /* [annotation][in] */
            _In_  WORD wFlags,
            /* [annotation][out][in] */
            _In_  DISPPARAMS* pDispParams,
            /* [annotation][out] */
            _Out_opt_  VARIANT* pVarResult,
            /* [annotation][out] */
            _Out_opt_  EXCEPINFO* pExcepInfo,
            /* [annotation][out] */
            _Out_opt_  UINT* puArgErr) override;
	protected:
		HWND m_hWnd;
		IAccessible* m_pRoot;
        IAccessible* m_pParent;
        ULONG m_refCount;
        size_t m_enumPos;
        BSTR m_name;
        BSTR m_value;
        std::vector<IAccessible*> m_children;
	};
}