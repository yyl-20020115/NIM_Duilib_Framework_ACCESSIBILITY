#include "Stdafx.h"
#include "MSAccessible.h"
#include <comdef.h>

namespace ui
{
    typedef UINT(*Ptr_GetDpiForWindow)(HWND hwnd);

    UINT GetDpiForWindowWrapper(
        HWND hwnd
    ) {
        HMODULE h = LoadLibraryEx(_T("user32.dll"), nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
        if (h != NULL) {
            Ptr_GetDpiForWindow p = (Ptr_GetDpiForWindow)GetProcAddress(h, "GetDpiForWindow");
            if (p != NULL) {
                return p(hwnd);
            }
        }
        return 0;
    }

    float GetScreenDisplayScale()
    {
        const UINT zoom_table[] = { 96,120,144,192 };
        const float dpi_table[] = { 1,1.25,1.5,2 };

        UINT zoom = GetDpiForWindowWrapper(GetDesktopWindow());

        float ret = 1.0f;
        for (size_t i = 0; i < sizeof(zoom_table) / sizeof(zoom_table[0]); i++)
        {
            if (zoom == zoom_table[i])
            {
                ret = dpi_table[i];
                break;
            }
        }
        return ret;
    }

    MSAccessible::MSAccessible()
        : m_hWnd(nullptr)
        , m_pRoot(nullptr)
        , m_pParent(nullptr)
        , m_refCount(1) 
        , m_children()
        , m_enumPos(0)
        , m_name()
        , m_value()
    {
    }

    void MSAccessible::SetRootAndParent(HWND hWnd, IAccessible* pRoot, IAccessible* pParent)
    {
        m_hWnd = hWnd;
        if (m_pRoot) m_pRoot->Release();
        m_pRoot = pRoot;
        if (m_pRoot) m_pRoot->AddRef();
		m_pParent = pParent;
		if (pParent) pParent->AddRef();
    }

    IAccessible* MSAccessible::GetRoot() const
    {
        if (m_pRoot) m_pRoot->AddRef();
        return m_pRoot;
    }

    
    IAccessible* MSAccessible::GetParent() const
    {
        return m_pParent;
    }

    HWND MSAccessible::GetWindowHandle() const
    {
        return m_hWnd;
    }

    HRESULT STDMETHODCALLTYPE MSAccessible::QueryInterface(REFIID riid, void** ppvObject)
    {
        if (!ppvObject) return E_POINTER;
        *ppvObject = nullptr;
        if (riid == IID_IUnknown || riid == IID_IDispatch || riid == IID_IAccessible)
        {
            *ppvObject = static_cast<IAccessible*>(this);
        }
        else if (riid == IID_IEnumVARIANT)
        {
            *ppvObject = static_cast<IEnumVARIANT*>(this);
        }
        else
        {
            return E_NOINTERFACE;
        }
        AddRef();
        return S_OK;
    }

    ULONG STDMETHODCALLTYPE MSAccessible::AddRef(void)
    {
        return InterlockedIncrement(&m_refCount);
    }

    ULONG STDMETHODCALLTYPE MSAccessible::Release(void)
    {
        ULONG ref = InterlockedDecrement(&m_refCount);   
        if (ref == 0)
            delete this;
        return ref;
    }

    // IEnumVARIANT
    HRESULT STDMETHODCALLTYPE MSAccessible::Next(ULONG celt, VARIANT* rgVar, ULONG* pCeltFetched)
    {
        if (!rgVar) return E_INVALIDARG;
        if (pCeltFetched) *pCeltFetched = 0;

        ULONG fetched = 0;
        if (fetched < celt && m_enumPos < m_children.size()) {
            IAccessible* pChild = m_children[m_enumPos];
            if (pChild) {
                VariantInit(&rgVar[fetched]);
                rgVar[fetched].vt = VT_DISPATCH;
                pChild->AddRef();
                rgVar[fetched].pdispVal = pChild;
                ++fetched;
            }
            ++m_enumPos;
        }
        if (pCeltFetched) *pCeltFetched = fetched;
        return (fetched == celt) ? S_OK : S_FALSE;
    }

    HRESULT STDMETHODCALLTYPE MSAccessible::Skip(ULONG celt)
    {
        size_t remain = m_children.size() - m_enumPos;
        if (celt > remain) {
            m_enumPos = static_cast<ULONG>(m_children.size());
            return S_FALSE;
        }
        m_enumPos += celt;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE MSAccessible::Reset(void)
    {
        this->TryReloadChildren();
        m_enumPos = 0;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE MSAccessible::Clone(IEnumVARIANT** ppEnum)
    {
        if (!ppEnum) return E_INVALIDARG;
        *ppEnum = nullptr;

        MSAccessible* pClone = new MSAccessible();
        // 复制基本信息
        pClone->m_hWnd = m_hWnd;
        pClone->m_pRoot = m_pRoot;
        if (pClone->m_pRoot) pClone->m_pRoot->AddRef();
        pClone->m_pParent = m_pParent;
        if (pClone->m_pParent) pClone->m_pParent->AddRef();
        pClone->m_children = m_children;
        pClone->m_enumPos = m_enumPos;

        *ppEnum = static_cast<IEnumVARIANT*>(pClone);
        pClone->AddRef();
        return S_OK;
    }

    // IAccessible
    HRESULT STDMETHODCALLTYPE MSAccessible::get_accParent(IDispatch** pp)
    {
        if (!pp) return E_INVALIDARG;
        *pp = nullptr;
        if (m_pParent) {
            m_pParent->AddRef();
            *pp = m_pParent;
            return S_OK;
        }
        return S_FALSE;
    }
    HRESULT STDMETHODCALLTYPE MSAccessible::get_accChildCount(long* pcountChildren)
    {
        if (!pcountChildren) return E_INVALIDARG;
        this->TryReloadChildren();
        *pcountChildren = static_cast<long>(m_children.size());
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE MSAccessible::get_accChild(VARIANT varChild, IDispatch** ppdispChild)
    {
        if (!ppdispChild) return E_INVALIDARG;
        *ppdispChild = nullptr;

        // 只支持整数类型的 child id
        if (varChild.vt != VT_I4)
            return E_INVALIDARG;

        // CHILDID_SELF 表示自身，不是子对象
        if (varChild.lVal == CHILDID_SELF)
            return S_FALSE;

        // MSAA child id 从 1 开始
        LONG index = varChild.lVal - 1;
        if (index < 0 || static_cast<size_t>(index) >= m_children.size())
            return S_FALSE;

        IAccessible* pChild = m_children[index];
        if (!pChild)
            return S_FALSE;

        pChild->AddRef();
        *ppdispChild = pChild;
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE MSAccessible::get_accName(VARIANT varChild, BSTR* pszName)
    {
        if (!pszName) return E_INVALIDARG;
        *pszName = nullptr;

        // 只支持自身（CHILDID_SELF）
        if (varChild.vt != VT_I4 || varChild.lVal != CHILDID_SELF)
            return S_FALSE;

        if (!m_hWnd)
            return S_FALSE;
        if (this->m_name == nullptr) {
            // 获取窗口标题
            wchar_t szText[1024] = { 0 };
            int len = ::GetClassNameW(m_hWnd, szText, _countof(szText));
            if (len > 0) {
                this->m_name = ::SysAllocString(szText);
            }

        }
        if (this->m_name!=nullptr) {
            *pszName = ::SysAllocString(this->m_name);
            return (*pszName) ? S_OK : E_OUTOFMEMORY;
		}

        return S_FALSE;
    }
    HRESULT STDMETHODCALLTYPE MSAccessible::put_accValue(VARIANT varChild, BSTR szValue)
    {
        // 只支持自身（CHILDID_SELF）
        if (varChild.vt != VT_I4 || varChild.lVal != CHILDID_SELF)
            return S_FALSE;

        // 释放旧的 m_value
        if (m_value) {
            SysFreeString(m_value);
            m_value = nullptr;
        }

        // 赋值新 value
        if (szValue) {
            m_value = SysAllocString(szValue);
            if (!m_value)
                return E_OUTOFMEMORY;
        }

        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE MSAccessible::get_accValue(VARIANT varChild, BSTR* pszValue)
    {
        if (!pszValue) return E_INVALIDARG;
        *pszValue = nullptr;

        // 只支持自身（CHILDID_SELF）
        if (varChild.vt != VT_I4 || varChild.lVal != CHILDID_SELF)
            return S_FALSE;

        if (m_value) {
            *pszValue = SysAllocString(m_value);
            return (*pszValue) ? S_OK : E_OUTOFMEMORY;
        }

        return S_FALSE;
    }
    HRESULT STDMETHODCALLTYPE MSAccessible::get_accDescription(VARIANT varChild, BSTR* pszDescription)
    {
        return get_accName(varChild, pszDescription);
    }
    HRESULT STDMETHODCALLTYPE MSAccessible::get_accRole(VARIANT varChild, VARIANT* pvarRole)
    {
        if (!pvarRole) return E_INVALIDARG;
        VariantInit(pvarRole);

        // 只支持自身（CHILDID_SELF）
        if (varChild.vt != VT_I4 || varChild.lVal != CHILDID_SELF)
            return S_FALSE;

        // 默认角色
        LONG role = ROLE_SYSTEM_WINDOW;

        if (m_hWnd) {
            wchar_t className[256] = {0};
            ::GetClassNameW(m_hWnd, className, _countof(className));

            // 常见标准控件
            if (wcscmp(className, L"Button") == 0) {
                // 进一步区分按钮类型
                LONG style = static_cast<LONG>(::GetWindowLongPtrW(m_hWnd, GWL_STYLE));
                if (style & BS_CHECKBOX)
                    role = ROLE_SYSTEM_CHECKBUTTON;
                else if (style & BS_RADIOBUTTON)
                    role = ROLE_SYSTEM_RADIOBUTTON;
                else if (style & BS_GROUPBOX)
                    role = ROLE_SYSTEM_GROUPING;
                else
                    role = ROLE_SYSTEM_PUSHBUTTON;
            } else if (wcscmp(className, L"Edit") == 0) {
                role = ROLE_SYSTEM_TEXT;
            } else if (wcscmp(className, L"Static") == 0) {
                role = ROLE_SYSTEM_STATICTEXT;
            } else if (wcscmp(className, L"ComboBox") == 0) {
                role = ROLE_SYSTEM_COMBOBOX;
            } else if (wcscmp(className, L"ListBox") == 0) {
                role = ROLE_SYSTEM_LIST;
            } else if (wcscmp(className, L"ListView") == 0) {
                role = ROLE_SYSTEM_LIST;
            } else if (wcscmp(className, L"SysListView32") == 0) {
                role = ROLE_SYSTEM_LIST;
            } else if (wcscmp(className, L"TreeView") == 0) {
                role = ROLE_SYSTEM_OUTLINE;
            } else if (wcscmp(className, L"SysTreeView32") == 0) {
                role = ROLE_SYSTEM_OUTLINE;
            } else if (wcscmp(className, L"TabControl") == 0) {
                role = ROLE_SYSTEM_PAGETABLIST;
            } else if (wcscmp(className, L"SysTabControl32") == 0) {
                role = ROLE_SYSTEM_PAGETABLIST;
            } else if (wcscmp(className, L"ScrollBar") == 0) {
                role = ROLE_SYSTEM_SCROLLBAR;
            } else if (wcscmp(className, L"msctls_progress32") == 0) {
                role = ROLE_SYSTEM_PROGRESSBAR;
            } else if (wcscmp(className, L"msctls_trackbar32") == 0) {
                role = ROLE_SYSTEM_SLIDER;
            } else if (wcscmp(className, L"msctls_updown32") == 0) {
                role = ROLE_SYSTEM_SPINBUTTON;
            } else if (wcscmp(className, L"msctls_hotkey32") == 0) {
                role = ROLE_SYSTEM_TEXT;
            } else if (wcscmp(className, L"msctls_statusbar32") == 0) {
                role = ROLE_SYSTEM_STATUSBAR;
            } else if (wcscmp(className, L"ToolbarWindow32") == 0) {
                role = ROLE_SYSTEM_TOOLBAR;
            } else if (wcscmp(className, L"ReBarWindow32") == 0) {
                role = ROLE_SYSTEM_TOOLBAR;
            } else if (wcscmp(className, L"MDIClient") == 0) {
                role = ROLE_SYSTEM_CLIENT;
            } else if (wcscmp(className, L"RichEdit20A") == 0 || wcscmp(className, L"RichEdit20W") == 0) {
                role = ROLE_SYSTEM_TEXT;
            } else if (wcscmp(className, L"SysHeader32") == 0) {
                role = ROLE_SYSTEM_COLUMNHEADER;
            //} else if (wcscmp(className, L"SysDateTimePick32") == 0) {
            //    role = ROLE_SYSTEM_DROPLIST;
            } else if (wcscmp(className, L"ComboBoxEx32") == 0) {
                role = ROLE_SYSTEM_COMBOBOX;
            } else if (wcscmp(className, L"ScrollBar") == 0) {
                role = ROLE_SYSTEM_SCROLLBAR;
            } else if (wcscmp(className, L"SysIPAddress32") == 0) {
                role = ROLE_SYSTEM_TEXT;
            } else if (wcscmp(className, L"SysLink") == 0) {
                role = ROLE_SYSTEM_LINK;
            } else if (wcscmp(className, L"SysPager") == 0) {
                role = ROLE_SYSTEM_PAGETAB;
            } else if (wcscmp(className, L"SysAnimate32") == 0) {
                role = ROLE_SYSTEM_ANIMATION;
            } else if (wcscmp(className, L"SysTabControl32") == 0) {
                role = ROLE_SYSTEM_PAGETABLIST;
            } else if (wcscmp(className, L"SysListView32") == 0) {
                role = ROLE_SYSTEM_LIST;
            } else if (wcscmp(className, L"SysTreeView32") == 0) {
                role = ROLE_SYSTEM_OUTLINE;
            } else if (wcscmp(className, L"SysHeader32") == 0) {
                role = ROLE_SYSTEM_COLUMNHEADER;
            } else if (wcscmp(className, L"SysLink") == 0) {
                role = ROLE_SYSTEM_LINK;
            } else if (wcscmp(className, L"SysIPAddress32") == 0) {
                role = ROLE_SYSTEM_TEXT;
            }
            // 你可以根据需要继续扩展更多控件类型
        }

        pvarRole->vt = VT_I4;
        pvarRole->lVal = role;
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE MSAccessible::get_accState(VARIANT varChild, VARIANT* pvarState)
    {
        if (!pvarState) return E_INVALIDARG;
        VariantInit(pvarState);

        // 只支持自身（CHILDID_SELF）
        if (varChild.vt != VT_I4 || varChild.lVal != CHILDID_SELF)
            return S_FALSE;

        LONG state = 0;

        if (m_hWnd) {
            // 可见性
            if (!::IsWindowVisible(m_hWnd))
                state |= STATE_SYSTEM_INVISIBLE;

            // 启用状态
            if (!::IsWindowEnabled(m_hWnd))
                state |= STATE_SYSTEM_UNAVAILABLE;
            else
                state |= STATE_SYSTEM_FOCUSABLE;

            // 获取焦点
            HWND hFocus = ::GetFocus();
            if (hFocus == m_hWnd)
                state |= STATE_SYSTEM_FOCUSED;

            // 是否可选中
            // 可根据控件类型扩展
            wchar_t className[256] = {0};
            ::GetClassNameW(m_hWnd, className, _countof(className));
            if (wcscmp(className, L"Button") == 0) {
                LONG style = static_cast<LONG>(::GetWindowLongPtrW(m_hWnd, GWL_STYLE));
                if (style & BS_CHECKBOX || style & BS_RADIOBUTTON) {
                    // 选中状态
                    LRESULT checked = ::SendMessageW(m_hWnd, BM_GETCHECK, 0, 0);
                    if (checked == BST_CHECKED || checked == BST_INDETERMINATE)
                        state |= STATE_SYSTEM_CHECKED;
                }
            }
        } else {
            // 没有窗口句柄，标记为不可用和不可见
            state |= STATE_SYSTEM_UNAVAILABLE | STATE_SYSTEM_INVISIBLE;
        }

        pvarState->vt = VT_I4;
        pvarState->lVal = state;
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE MSAccessible::get_accHelp(VARIANT varChild, BSTR* pszDescription)
    {
        return get_accName(varChild, pszDescription);
    }
    HRESULT STDMETHODCALLTYPE MSAccessible::get_accHelpTopic(BSTR* pszHelpFile, VARIANT varChild, long* pidTopic)
    {
        if (!pszHelpFile || !pidTopic)
            return E_INVALIDARG;
        *pszHelpFile = nullptr;
        *pidTopic = -1;

        // 只支持自身（CHILDID_SELF）
        if (varChild.vt != VT_I4 || varChild.lVal != CHILDID_SELF)
            return S_FALSE;

        // 示例：假设帮助文件为 "help.chm"，主题ID为 0
        // 实际项目可根据窗口类名或控件类型返回不同的帮助文件和主题
        if (m_hWnd) {
            wchar_t className[256] = {0};
            ::GetClassNameW(m_hWnd, className, _countof(className));
            // 可根据 className 设置不同帮助
            *pszHelpFile = ::SysAllocString(L"help.chm");
            *pidTopic = 0;
            return (*pszHelpFile) ? S_OK : E_OUTOFMEMORY;
        }

        return S_FALSE;
    }
    HRESULT STDMETHODCALLTYPE MSAccessible::get_accKeyboardShortcut(VARIANT varChild, BSTR* pszKeyboardShortcut)
    {
        if (!pszKeyboardShortcut) return E_INVALIDARG;
        *pszKeyboardShortcut = nullptr;

        // 只支持自身（CHILDID_SELF）
        if (varChild.vt != VT_I4 || varChild.lVal != CHILDID_SELF)
            return S_FALSE;

        if (!m_hWnd)
            return S_FALSE;

        // 获取窗口类名
        wchar_t className[256] = {0};
        ::GetClassNameW(m_hWnd, className, _countof(className));

        // 获取窗口文本
        wchar_t szText[1024] = {0};
        ::GetWindowTextW(m_hWnd, szText, _countof(szText));

        // 查找助记符（&后跟字符）
        for (int i = 0; szText[i]; ++i) {
            if (szText[i] == L'&' && szText[i + 1] && szText[i + 1] != L'&') {
                wchar_t shortcut[3] = { L'\0' };
                shortcut[0] = szText[i + 1];
                *pszKeyboardShortcut = ::SysAllocString(shortcut);
                return (*pszKeyboardShortcut) ? S_OK : E_OUTOFMEMORY;
            }
        }

        // 其它控件可根据需要扩展（如热键控件、组合框等）
        // 这里仅处理常见助记符

        return S_FALSE;
    }
    HRESULT STDMETHODCALLTYPE MSAccessible::get_accFocus(VARIANT* pvarChild)
    {
        if (!pvarChild) return E_INVALIDARG;
        VariantInit(pvarChild);

        // 检查窗口句柄
        if (!m_hWnd)
            return S_FALSE;

        HWND hFocus = ::GetFocus();
        if (hFocus == m_hWnd) {
            // 本窗口有焦点
            pvarChild->vt = VT_I4;
            pvarChild->lVal = CHILDID_SELF;
            return S_OK;
        }

        // 检查子元素是否有焦点
        for (size_t i = 0; i < m_children.size(); ++i) {
            IAccessible* pChild = m_children[i];
            if (!pChild)
                continue;

            VARIANT varFocus;
            VariantInit(&varFocus);
            HRESULT hr = pChild->get_accFocus(&varFocus);
            if (SUCCEEDED(hr) && varFocus.vt != VT_EMPTY && varFocus.vt != VT_NULL) {
                *pvarChild = varFocus;
                return S_OK;
            }
            VariantClear(&varFocus);
        }

        // 没有焦点
        pvarChild->vt = VT_EMPTY;
        return S_FALSE;
    }
    HRESULT STDMETHODCALLTYPE MSAccessible::get_accSelection(VARIANT* pvarChildren)
    {
        if (!pvarChildren) return E_INVALIDARG;
        VariantInit(pvarChildren);

        // 检查窗口句柄
        if (!m_hWnd)
            return S_FALSE;

        // 遍历子元素，查找选中项
        for (size_t i = 0; i < m_children.size(); ++i) {
            IAccessible* pChild = m_children[i];
            if (!pChild)
                continue;

            VARIANT varState;
            VariantInit(&varState);
            // 查询子项状态
            HRESULT hr = pChild->get_accState({ VT_I4, CHILDID_SELF }, &varState);
            if (SUCCEEDED(hr) && varState.vt == VT_I4 && (varState.lVal & STATE_SYSTEM_SELECTED)) {
                // 返回第一个选中项
                pvarChildren->vt = VT_DISPATCH;
                pChild->AddRef();
                pvarChildren->pdispVal = pChild;
                VariantClear(&varState);
                return S_OK;
            }
            VariantClear(&varState);
        }

        // 没有选中项
        pvarChildren->vt = VT_EMPTY;
        return S_FALSE;
    }
    HRESULT STDMETHODCALLTYPE MSAccessible::get_accDefaultAction(VARIANT varChild, BSTR* pszDefaultAction)
    {
        if (!pszDefaultAction) return E_INVALIDARG;
        *pszDefaultAction = nullptr;

        // 只支持自身（CHILDID_SELF）
        if (varChild.vt != VT_I4 || varChild.lVal != CHILDID_SELF)
            return S_FALSE;

        if (!m_hWnd)
            return S_FALSE;

        wchar_t className[256] = {0};
        ::GetClassNameW(m_hWnd, className, _countof(className));

        // Button/CheckBox/RadioButton
        if (wcscmp(className, L"Button") == 0) {
            LONG style = static_cast<LONG>(::GetWindowLongPtrW(m_hWnd, GWL_STYLE));
            if (style & BS_CHECKBOX)
                *pszDefaultAction = ::SysAllocString(L"切换");
            else if (style & BS_RADIOBUTTON)
                *pszDefaultAction = ::SysAllocString(L"选择");
            else
                *pszDefaultAction = ::SysAllocString(L"单击");
            return (*pszDefaultAction) ? S_OK : E_OUTOFMEMORY;
        }

        // ComboBox
        if (wcscmp(className, L"ComboBox") == 0) {
            *pszDefaultAction = ::SysAllocString(L"展开");
            return (*pszDefaultAction) ? S_OK : E_OUTOFMEMORY;
        }

        // ListBox
        if (wcscmp(className, L"ListBox") == 0) {
            *pszDefaultAction = ::SysAllocString(L"选择");
            return (*pszDefaultAction) ? S_OK : E_OUTOFMEMORY;
        }

        // Edit
        if (wcscmp(className, L"Edit") == 0) {
            *pszDefaultAction = ::SysAllocString(L"编辑");
            return (*pszDefaultAction) ? S_OK : E_OUTOFMEMORY;
        }

        // Static
        if (wcscmp(className, L"Static") == 0) {
            *pszDefaultAction = nullptr;
            return S_FALSE;
        }

        // 其它控件默认返回“激活”
        *pszDefaultAction = ::SysAllocString(L"激活");
        return (*pszDefaultAction) ? S_OK : E_OUTOFMEMORY;
    }
    HRESULT STDMETHODCALLTYPE MSAccessible::accSelect(long flagsSelect, VARIANT varChild)
    {
        // 只支持自身和子项的选择，且仅处理 STATE_SYSTEM_SELECTED
        // flagsSelect: SELFLAG_TAKEFOCUS, SELFLAG_TAKESELECTION, SELFLAG_ADDSELECTION, SELFLAG_REMOVESELECTION, SELFLAG_EXTENDSELECTION
        // 这里只实现 SELFLAG_TAKESELECTION（单选），如需多选可扩展

        // 只支持整数类型 child id
        if (varChild.vt != VT_I4)
            return E_INVALIDARG;

        // 只支持子项选择（不支持自身）
        if (varChild.lVal == CHILDID_SELF)
            return S_FALSE;

        // MSAA child id 从 1 开始
        LONG index = varChild.lVal - 1;
        if (index < 0 || static_cast<size_t>(index) >= m_children.size())
            return S_FALSE;

        IAccessible* pChild = m_children[index];
        if (!pChild)
            return S_FALSE;

        // 只处理 SELFLAG_TAKESELECTION（单选）
        if (flagsSelect & SELFLAG_TAKESELECTION)
        {
            // 先清除所有子项的选中状态
            for (size_t i = 0; i < m_children.size(); ++i) {
                IAccessible* pItem = m_children[i];
                if (pItem) {
                    // 尝试调用 put_accState 或自定义接口（如有）
                    // 这里只能调用 put_accValue/put_accName 作为示例，实际应有自定义接口
                    // 真实项目中应有专门的“选中”接口
                    // 这里假设 put_accValue(VARIANT, BSTR) 可用于设置选中状态
                    VARIANT vSelf;
                    vSelf.vt = VT_I4;
                    vSelf.lVal = CHILDID_SELF;
                    pItem->put_accValue(vSelf, SysAllocString(L"0"));
                }
            }
            // 设置目标子项为选中
            VARIANT vSelf;
            vSelf.vt = VT_I4;
            vSelf.lVal = CHILDID_SELF;
            pChild->put_accValue(vSelf, SysAllocString(L"1"));
            return S_OK;
        }

        // 其它选择标志未实现
        return E_NOTIMPL;
    }


    HRESULT STDMETHODCALLTYPE MSAccessible::accLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight, VARIANT varChild)
    {
        if (!pxLeft || !pyTop || !pcxWidth || !pcyHeight)
            return E_INVALIDARG;

        *pxLeft = *pyTop = *pcxWidth = *pcyHeight = 0;

        // 只支持自身（CHILDID_SELF）
        if (varChild.vt != VT_I4 || varChild.lVal != CHILDID_SELF)
            return S_FALSE;

        if (!m_hWnd)
            return S_FALSE;

        RECT rc = { 0 };
        if (!::GetWindowRect(m_hWnd, &rc))
            return S_FALSE;

		float ratio = GetScreenDisplayScale();
        *pxLeft = (long)(rc.left * ratio);
        *pyTop = (long)(rc.top * ratio);
        *pcxWidth = (long)((rc.right - rc.left)*ratio);
        *pcyHeight = (long)((rc.bottom - rc.top)*ratio);

        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE MSAccessible::accNavigate(long navDir, VARIANT varStart, VARIANT* pvarEndUpAt)
    {
        if (!pvarEndUpAt) return E_INVALIDARG;
        VariantInit(pvarEndUpAt);

        // 只支持自身（CHILDID_SELF）
        if (varStart.vt != VT_I4 || varStart.lVal != CHILDID_SELF)
            return S_FALSE;

        // 导航到父对象:TODO: maybe wrong
        if (navDir == NAVDIR_MIN) {
            if (m_pParent) {
                pvarEndUpAt->vt = VT_DISPATCH;
                m_pParent->AddRef();
                pvarEndUpAt->pdispVal = m_pParent;
                return S_OK;
            }
            return S_FALSE;
        }

        // 导航到第一个子对象
        if (navDir == NAVDIR_FIRSTCHILD) {
            if (!m_children.empty() && m_children[0]) {
                pvarEndUpAt->vt = VT_DISPATCH;
                m_children[0]->AddRef();
                pvarEndUpAt->pdispVal = m_children[0];
                return S_OK;
            }
            return S_FALSE;
        }

        // 导航到最后一个子对象
        if (navDir == NAVDIR_LASTCHILD) {
            if (!m_children.empty() && m_children.back()) {
                pvarEndUpAt->vt = VT_DISPATCH;
                m_children.back()->AddRef();
                pvarEndUpAt->pdispVal = m_children.back();
                return S_OK;
            }
            return S_FALSE;
        }

        // 导航到兄弟节点（前一个/后一个）
        if (navDir == NAVDIR_NEXT || navDir == NAVDIR_PREVIOUS) {
            if (!m_pParent)
                return S_FALSE;

            // 在父对象的 children 中查找自身
            MSAccessible* pParentAcc = dynamic_cast<MSAccessible*>(m_pParent);
            if (!pParentAcc)
                return S_FALSE;

            auto& siblings = pParentAcc->m_children;
            auto it = std::find(siblings.begin(), siblings.end(), static_cast<IAccessible*>(this));
            if (it == siblings.end())
                return S_FALSE;

            if (navDir == NAVDIR_NEXT) {
                ++it;
                if (it != siblings.end() && *it) {
                    pvarEndUpAt->vt = VT_DISPATCH;
                    (*it)->AddRef();
                    pvarEndUpAt->pdispVal = *it;
                    return S_OK;
                }
            } else if (navDir == NAVDIR_PREVIOUS) {
                if (it != siblings.begin()) {
                    --it;
                    if (*it) {
                        pvarEndUpAt->vt = VT_DISPATCH;
                        (*it)->AddRef();
                        pvarEndUpAt->pdispVal = *it;
                        return S_OK;
                    }
                }
            }
            return S_FALSE;
        }

        // 其它导航类型未实现
        return S_FALSE;
    }
    HRESULT STDMETHODCALLTYPE MSAccessible::accHitTest(long xLeft, long yTop, VARIANT* pvarChild)
    {
        if (!pvarChild)
            return E_INVALIDARG;

        VariantInit(pvarChild);

        // 检查窗口句柄
        if (!m_hWnd)
            return S_FALSE;

        // 获取窗口矩形（屏幕坐标）
        RECT rcWnd = { 0 };
        if (!::GetWindowRect(m_hWnd, &rcWnd))
            return S_FALSE;

        // 判断点是否在窗口内
        if (xLeft < rcWnd.left || xLeft >= rcWnd.right ||
            yTop < rcWnd.top || yTop >= rcWnd.bottom)
        {
            return S_FALSE;
        }

        // 如果没有子元素，返回自身
        if (m_children.empty()) {
            pvarChild->vt = VT_I4;
            pvarChild->lVal = CHILDID_SELF;
            return S_OK;
        }

        // 遍历子元素，递归调用 accHitTest
        for (size_t i = 0; i < m_children.size(); ++i) {
            IAccessible* pChild = m_children[i];
            if (!pChild)
                continue;

            VARIANT varHit = {};
            HRESULT hr = pChild->accHitTest(xLeft, yTop, &varHit);
            if (SUCCEEDED(hr) && varHit.vt != VT_EMPTY && varHit.vt != VT_NULL && varHit.vt != VT_I4 /* not S_FALSE */) {
                // 命中子元素，返回子元素
                *pvarChild = varHit;
                return S_OK;
            }
            // 如果命中的是子对象本身
            if (SUCCEEDED(hr) && varHit.vt == VT_I4 && varHit.lVal == CHILDID_SELF) {
                pvarChild->vt = VT_DISPATCH;
                pChild->AddRef();
                pvarChild->pdispVal = pChild;
                return S_OK;
            }
            VariantClear(&varHit);
        }

        // 没有命中任何子元素，返回自身
        pvarChild->vt = VT_I4;
        pvarChild->lVal = CHILDID_SELF;
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE MSAccessible::accDoDefaultAction(VARIANT varChild)
    {
        // 只支持自身（CHILDID_SELF）
        if (varChild.vt != VT_I4 || varChild.lVal != CHILDID_SELF)
            return S_FALSE;

        if (!m_hWnd)
            return S_FALSE;

        wchar_t className[256] = {0};
        ::GetClassNameW(m_hWnd, className, _countof(className));

        // 复选框/单选框
        if (wcscmp(className, L"Button") == 0) {
            LONG style = static_cast<LONG>(::GetWindowLongPtrW(m_hWnd, GWL_STYLE));
            if (style & (BS_CHECKBOX | BS_RADIOBUTTON)) {
                ::SendMessageW(m_hWnd, BM_CLICK, 0, 0);
                return S_OK;
            }
        }
        // 按钮类控件默认执行点击
        if (wcscmp(className, L"Button") == 0) {
            ::SendMessageW(m_hWnd, BM_CLICK, 0, 0);
            return S_OK;
        }


        // ComboBox 下拉
        if (wcscmp(className, L"ComboBox") == 0) {
            ::SendMessageW(m_hWnd, CB_SHOWDROPDOWN, TRUE, 0);
            return S_OK;
        }

        // ListBox/ListView/TreeView等可选中控件，默认选中第一个
        if (wcscmp(className, L"ListBox") == 0) {
            ::SendMessageW(m_hWnd, LB_SETCURSEL, 0, 0);
            return S_OK;
        }
        if (wcscmp(className, L"SysListView32") == 0) {
            ::SendMessageW(m_hWnd, LVM_ENSUREVISIBLE, 0, TRUE);
            return S_OK;
        }
        if (wcscmp(className, L"SysTreeView32") == 0) {
            ::SendMessageW(m_hWnd, TVM_SELECTITEM, TVGN_CARET, (LPARAM)TreeView_GetRoot(m_hWnd));
            return S_OK;
        }

        // Edit控件默认设置焦点
        if (wcscmp(className, L"Edit") == 0) {
            ::SetFocus(m_hWnd);
            return S_OK;
        }

        // 其它控件默认设置焦点
        ::SetFocus(m_hWnd);
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE MSAccessible::put_accName(VARIANT varChild, BSTR szName)
    {
        // 只支持自身（CHILDID_SELF）
        if (varChild.vt != VT_I4 || varChild.lVal != CHILDID_SELF)
            return S_FALSE;

        // 释放旧的 m_name
        if (m_name) {
            SysFreeString(m_name);
            m_name = nullptr;
        }

        // 赋值新名称
        if (szName) {
            m_name = SysAllocString(szName);
            if (!m_name)
                return E_OUTOFMEMORY;
        }

        return S_OK;
    }

    // IDispatch (最基本的实现)
    HRESULT STDMETHODCALLTYPE MSAccessible::GetTypeInfoCount(UINT* pctinfo)
    {
        if (pctinfo) *pctinfo = 0;
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE MSAccessible::GetTypeInfo(UINT, LCID, ITypeInfo**)
    {
        return E_NOTIMPL;
    }
    HRESULT STDMETHODCALLTYPE MSAccessible::GetIDsOfNames(REFIID, LPOLESTR*, UINT, LCID, DISPID*)
    {
        return E_NOTIMPL;
    }
    HRESULT STDMETHODCALLTYPE MSAccessible::Invoke(DISPID, REFIID, LCID, WORD, DISPPARAMS*, VARIANT*, EXCEPINFO*, UINT*)
    {
        return E_NOTIMPL;
    }
    void MSAccessible::TryReloadChildren()
    {
        for (auto mc : m_children) mc->Release();
        this->m_children.clear();

    }
    // 析构函数
    MSAccessible::~MSAccessible()
    {
        if (m_pRoot) m_pRoot->Release();
        m_pRoot = nullptr;
		if (m_name) SysFreeString(m_name);
        m_name = nullptr;
        if (m_value) SysFreeString(m_value);
        m_value = nullptr;
    }
}