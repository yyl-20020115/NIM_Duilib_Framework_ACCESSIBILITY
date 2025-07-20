#pragma once
#include <windows.h>
#include <UIAutomationCore.h>
#include <UIAutomationCoreApi.h>
#include <UIAutomationClient.h>

#include "UIABoxProvider.h"
#include "UIAButtonProvider.h"
#include "UIACheckBoxProvider.h"
#include "UIAComboBoxProvider.h"
#include "UIAControlProvider.h"
#include "UIALabelProvider.h"
#include "UIAListBoxItemProvider.h"
#include "UIAListBoxProvider.h"
#include "UIAProgressProvider.h"
#include "UIARichEditProvider.h"
#include "UIAScrollableBoxProvider.h"
#include "UIAScrollBarProvider.h"
#include "UIATreeNodeProvider.h"
#include "UIAWindowProvider.h"

#define UIA_CHECK_ELEMENT(element) if(!element) return UIA_E_ELEMENTNOTAVAILABLE

#define UIA_CHECK_ELEMENT_RETURN(element)          \
	if(!element) return UIA_E_ELEMENTNOTAVAILABLE; \
	else return S_OK 
