#pragma once

#include <Windows.h>
#include <oleacc.h>
#include "MSAccessible.h"
#include "MSAccessibleLabel.h"

namespace ui
{
	class MSAccessibleButton
		: public MSAccessibleLabel
	{
	public:
		MSAccessibleButton(Control* pControl, MSAccessible* pParent);
		virtual ~MSAccessibleButton();
	public:
		HRESULT STDMETHODCALLTYPE accDoDefaultAction(VARIANT varChild) override;

	public:
		void TryReloadChildren() override;
	};

}
