#pragma once

#include <Windows.h>
#include <oleacc.h>
#include "MSAccessible.h"
#include <UIlib.h>


namespace ui
{
	class MSAccessibleControl
		: public MSAccessible
	{
	public:
		MSAccessibleControl(Control* pControl,MSAccessible* pParent = nullptr);
		virtual ~MSAccessibleControl();

		HRESULT STDMETHODCALLTYPE accLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight, VARIANT varChild) override;

		HRESULT STDMETHODCALLTYPE accHitTest(long xLeft, long yTop, VARIANT* pvarChild) override;

		HRESULT STDMETHODCALLTYPE get_accName(
			/* [optional][in] */ VARIANT varChild,
			/* [retval][out] */ __RPC__deref_out_opt BSTR* pszName) override;

	public:	
		void TryReloadChildren() override;

	protected:
		Control* m_pControl;
	};


}
