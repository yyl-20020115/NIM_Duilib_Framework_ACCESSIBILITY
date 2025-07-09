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
		MSAccessibleControl(Control* pControl);
		virtual ~MSAccessibleControl();
	public:


	protected:
		Control* m_pControl;
	};

}
