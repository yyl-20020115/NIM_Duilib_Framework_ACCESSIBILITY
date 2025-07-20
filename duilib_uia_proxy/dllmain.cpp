#include "StdAfx.h"
#include <strsafe.h>
#include <tchar.h>
#include <UIAutomationCore.h>
#include <UIAutomationCoreApi.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}

static HHOOK hHook = NULL;
static HWND hwndTarget = NULL;
static IAccessible* acc = nullptr;
__declspec(dllexport) extern "C" BOOL AttachWindow(HWND hwnd)
{
    if (hHook == NULL)
    {
        HRESULT hr = AccessibleObjectFromWindow(hwndTarget, OBJID_CLIENT, IID_IAccessible, (void**)&acc);
        if(hr==S_OK)
        {
            hHook = SetWindowsHookEx(WH_CALLWNDPROC,
                [](int nCode, WPARAM wParam, LPARAM lParam) -> LRESULT {
                    if (nCode >= 0) {
                        CWPSTRUCT* cwp = (CWPSTRUCT*)lParam;
                        if (cwp != nullptr
                            && cwp->hwnd == hwndTarget
                            && cwp->message == WM_GETOBJECT) {

                            if (static_cast<long>(lParam) == static_cast<long>(UiaRootObjectId))
                            {
                                IRawElementProviderSimple* pRootProvider =
                                    new ui_automation::UIAWindowProvider(acc, hwndTarget);

                                return UiaReturnRawElementProvider(
                                    hwndTarget, wParam, lParam, pRootProvider);
                            }
                        }
                    }
                    return CallNextHookEx(hHook, nCode, wParam, lParam);
                }, NULL, GetCurrentThreadId());
        }
        else
        {
            // 错误处理
            acc = nullptr;
            return FALSE;
		}
        
        if(hHook!=NULL){
            hwndTarget = hwnd;
            return TRUE;
        }
        else
        {
            // 错误处理
            hHook = NULL;
            hwndTarget = NULL;
		}

	}
    return FALSE;

}
__declspec(dllexport) extern "C" BOOL DetachWindow(HWND hwnd)
{
    if(hHook != NULL && hwndTarget == hwnd)
    {
        BOOL done = UnhookWindowsHookEx(hHook);
        hHook = NULL;
		hwndTarget = NULL;
        if (acc) {
            acc->Release();
			acc = nullptr;
        }
        return done;
	}
    return FALSE;
}