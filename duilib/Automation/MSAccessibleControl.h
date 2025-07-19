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

		HRESULT accLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight, VARIANT varChild) override;

	public:	
		void TryReloadChildren() override;

	protected:
		Control* m_pControl;
	};


}
