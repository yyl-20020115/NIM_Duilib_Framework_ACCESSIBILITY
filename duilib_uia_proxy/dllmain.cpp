#include <windows.h>

#include <Windows.h>
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
__declspec(dllexport) extern "C" void AttachWindow(HWND hwnd)
{
    if (hHook == NULL)
    {

        hHook = SetWindowsHookEx(WH_CALLWNDPROC, 
            [](int nCode, WPARAM wParam, LPARAM lParam) -> LRESULT {
                if (nCode >= 0) {
                    CWPSTRUCT* cwp = (CWPSTRUCT*)lParam;
                    if (cwp!=nullptr 
                        && cwp->hwnd == hwndTarget 
                        && cwp->message == WM_GETOBJECT) {

                        if (static_cast<long>(lParam) == static_cast<long>(UiaRootObjectId))
                        {
                            IRawElementProviderSimple* pRootProvider = nullptr;

                            return UiaReturnRawElementProvider(hwndTarget, wParam, lParam, pRootProvider);
                        }
                    }
                }
            return CallNextHookEx(hHook, nCode, wParam, lParam);
        }, NULL, GetCurrentThreadId());
        
        if(hHook!=NULL){
            hwndTarget = hwnd;
        }
        else
        {
            // ¥ÌŒÛ¥¶¿Ì
            hHook = NULL;
            hwndTarget = NULL;
		}

	}

}
__declspec(dllexport) extern "C" void DetachWindow(HWND hwnd)
{
    if(hHook != NULL && hwndTarget == hwnd)
    {
        UnhookWindowsHookEx(hHook);
        hHook = NULL;
		hwndTarget = NULL;
	}
}