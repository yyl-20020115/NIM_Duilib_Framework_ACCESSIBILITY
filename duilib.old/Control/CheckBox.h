#ifndef UI_CONTROL_CHECKBOX_H_
#define UI_CONTROL_CHECKBOX_H_

#pragma once

namespace ui
{

template<typename InheritType = Control>
class UILIB_API CheckBoxTemplate : public ButtonTemplate<InheritType>
{
public:
	CheckBoxTemplate();

	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual void Activate() override;
	virtual Image* GetEstimateImage() override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	virtual void PaintStatusColor(IRenderContext* pRender) override;
	virtual void PaintStatusImage(IRenderContext* pRender) override;
	virtual void PaintText(IRenderContext* pRender) override;
	virtual bool HasHotState();

	/**
	 * @brief ѡ��״̬�£�û�����ñ���ɫ�򱳾�ͼʱ���Ƿ��÷�ѡ��״̬�Ķ�Ӧ����������
	 * @return ���� true Ϊѡ��״̬������Ϊ false
	 */
	bool IsPaintNormalFirst() const { return m_bPaintNormalFirst; };

	/**
	 * @brief ���ÿؼ�ѡ��״̬�£�û�����ñ���ɫ�򱳾�ͼʱ���÷�ѡ��״̬�Ķ�Ӧ����������
	 * @param[in] bFirst Ϊ true ���Ʒ�ѡ��״̬���ԣ�false ������
	 * @return ��
	 */
	void SetPaintNormalFirst(bool bFirst) { m_bPaintNormalFirst = bFirst; };

	/**
	 * @brief �жϵ�ǰ�Ƿ���ѡ��״̬
	 * @return ���� true Ϊѡ��״̬������Ϊ false
	 */
	bool IsSelected() const { return m_bSelected; }

	/**
	 * @brief ���ÿؼ��Ƿ�ѡ��״̬
	 * @param[in] bSelected Ϊ true ʱΪѡ��״̬��false ʱΪȡ��ѡ��״̬
	 * @param[in] bTriggerEvent �Ƿ���״̬�ı��¼���true Ϊ���ͣ�����Ϊ false��Ĭ��Ϊ false
	 * @return ��
	 */
	virtual void Selected(bool bSelected, bool bTriggerEvent = false);

	/**
	 * @brief ��ȡ��ѡ��ʱ��ͼƬ
	 * @param[in] stateType Ҫ��ȡ����״̬�µ�ͼƬ���ο� ControlStateType ö��
	 * @return ����ͼƬλ��
	 */
	std::wstring GetSelectedStateImage(ControlStateType stateType);

	/**
	 * @brief ���ñ�ѡ��ʱ��ͼƬ
	 * @param[in] stateType Ҫ��������״̬�µ�ͼƬ
	 * @param[in] strImage ͼƬ��ַ
	 * @return ��
	 */
	void SetSelectedStateImage(ControlStateType stateType, const std::wstring& strImage);

	/**
	 * @brief ��ȡ��ѡ��ʱ���ı���ɫ
	 * @return ���ر�ѡ��ʱ���ı���ɫ
	 */
	std::wstring GetSelectedTextColor();

	/**
	 * @brief ���ñ�ѡ��ʱ���ı���ɫ
	 * @param[in] dwTextColor Ҫ���õ���ɫ�ַ���������ɫ������ global.xml �д���
	 * @return ��
	 */
	void SetSelectedTextColor(const std::wstring& dwTextColor);

	/**
	 * @brief ��ȡ��ѡ��ʱָ��״̬�µ��ı���ɫ
	 * @param[in] stateType Ҫ��ȡ����״̬�µ���ɫ
	 * @return ������ɫ�ַ�������ֵ�� global.xml �ж���
	 */
	std::wstring GetSelectedStateTextColor(ControlStateType stateType);

	/**
	 * @brief ���ñ�ѡ��ʱָ��״̬�µ��ı���ɫ
	 * @param[in] stateType Ҫ���ú���״̬�µ���ɫ
	 * @param[in] stateColor Ҫ���õ���ɫ
	 * @return ��
	 */
	void SetSelectedStateTextColor(ControlStateType stateType, const std::wstring& dwTextColor);

	/**
	* @brief ��ȡ��ѡ��ʱָ��״̬�µ�ʵ�ʱ���Ⱦ�ı���ɫ
	* @param[in] buttonStateType Ҫ��ȡ����״̬�µ���ɫ
	* @param[out] stateType ʵ�ʱ���Ⱦ��״̬
	* @return ������ɫ�ַ�������ֵ�� global.xml �ж���
	*/
	std::wstring GetPaintSelectedStateTextColor(ControlStateType buttonStateType, ControlStateType& stateType);

	/**
	 * @brief ��ȡ��ѡ��ʱ�Ŀؼ���ɫ 
	 * @param[in] stateType Ҫ��ȡ����״̬�µ���ɫ
	 * @return ������ɫ�ַ�������ֵ�� global.xml �ж���
	 */
	std::wstring GetSelectStateColor(ControlStateType stateType);

	/**
	 * @brief ���ñ�ѡ��ʱ�Ŀؼ���ɫ
	 * @param[in] stateType Ҫ���ú���״̬�µ���ɫ
	 * @param[in] stateColor Ҫ���õ���ɫ
	 * @return ��
	 */
	void SetSelectedStateColor(ControlStateType stateType, const std::wstring& stateColor);

	/**
	 * @brief ��ȡ��ѡ��ʱ��ǰ��ͼƬ
	 * @param[in] stateType Ҫ��ȡ����״̬�µ�ǰ��ͼƬ
	 * @return ����ͼƬλ��
	 */
	std::wstring GetSelectedForeStateImage(ControlStateType stateType);

	/**
	 * @brief ���ñ�ѡ��ʱ��ǰ��ͼƬ
	 * @param[in] stateType Ҫ���ú���״̬�µ�ǰ��ͼƬ
	 * @param[in] pStrImage ͼƬλ��
	 * @return ��
	 */
	void SetSelectedForeStateImage(ControlStateType stateType, const std::wstring& pStrImage);

	/**
	 * @brief ������ѡ��ʱ���¼�
	 * @param[in] callback ��ѡ��ʱ�����Ļص�����
	 * @return ��
	 */
	void AttachSelect(const EventCallback& callback) { this->OnEvent[kEventSelect] += callback; }

	/**
	 * @brief ����ȡ��ѡ��ʱ���¼�
	 * @param[in] callback ȡ��ѡ��ʱ�����Ļص�����
	 * @return ��
	 */
	void AttachUnSelect(const EventCallback& callback) { this->OnEvent[kEventUnSelect] += callback; }

protected:
	bool			m_bSelected;
	bool			m_bPaintNormalFirst;
	std::wstring	m_dwSelectedTextColor;
	StateColorMap	m_selectedTextColorMap;
	StateColorMap	m_selectedColorMap;
};

template<typename InheritType>
CheckBoxTemplate<InheritType>::CheckBoxTemplate() : m_bSelected(false), m_bPaintNormalFirst(false), m_dwSelectedTextColor(), m_selectedColorMap()
{
    m_selectedColorMap.SetControl(this);
    m_selectedTextColorMap.SetControl(this);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::Activate()
{
    if (!IsActivatable())
        return;
    Selected(!m_bSelected, true);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::Selected(bool bSelected, bool bTriggerEvent)
{
    if (m_bSelected == bSelected) return;
    m_bSelected = bSelected;

    if (m_pWindow != NULL) {
        if (bTriggerEvent) {
            if (m_bSelected) {
                m_pWindow->SendNotify(this, kEventSelect);
            }
            else {
                m_pWindow->SendNotify(this, kEventUnSelect);
            }
        }
    }

    Invalidate();
}

template<typename InheritType>
bool CheckBoxTemplate<InheritType>::HasHotState()
{
    return m_selectedColorMap.HasHotColor() || m_selectedTextColorMap.HasHotColor() || __super::HasHotState();
}

template<typename InheritType>
Image* CheckBoxTemplate<InheritType>::GetEstimateImage()
{
    Image* estimateImage = __super::GetEstimateImage();
    if (!estimateImage) {
        estimateImage = m_imageMap.GetEstimateImage(kStateImageSelectedBk);
    }

    return estimateImage;
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
    if (strName == _T("selected")) Selected(strValue == _T("true"), true);
    else if (strName == _T("normalfirst")) SetPaintNormalFirst(strValue == _T("true"));
    else if (strName == _T("selectednormalimage")) SetSelectedStateImage(kControlStateNormal, strValue);
    else if (strName == _T("selectedhotimage")) SetSelectedStateImage(kControlStateHot, strValue);
    else if (strName == _T("selectedpushedimage")) SetSelectedStateImage(kControlStatePushed, strValue);
    else if (strName == _T("selecteddisabledimage")) SetSelectedStateImage(kControlStateDisabled, strValue);
    else if (strName == _T("selectedtextcolor")) SetSelectedTextColor(strValue);
    else if (strName == _T("selectednormaltextcolor")) SetSelectedStateTextColor(kControlStateNormal, strValue);
    else if (strName == _T("selectedhottextcolor"))	SetSelectedStateTextColor(kControlStateHot, strValue);
    else if (strName == _T("selectedpushedtextcolor")) SetSelectedStateTextColor(kControlStatePushed, strValue);
    else if (strName == _T("selecteddisabledtextcolor")) SetSelectedStateTextColor(kControlStateDisabled, strValue);
    else if (strName == _T("selectednormalcolor")) SetSelectedStateColor(kControlStateNormal, strValue);
    else if (strName == _T("selectedhotcolor")) SetSelectedStateColor(kControlStateHot, strValue);
    else if (strName == _T("selectedpushedcolor")) SetSelectedStateColor(kControlStatePushed, strValue);
    else if (strName == _T("selecteddisabledcolor")) SetSelectedStateColor(kControlStateDisabled, strValue);
    else if (strName == _T("selectedforenormalimage")) SetSelectedForeStateImage(kControlStateNormal, strValue);
    else if (strName == _T("selectedforehotimage")) SetSelectedForeStateImage(kControlStateHot, strValue);
    else if (strName == _T("selectedforepushedimage")) SetSelectedForeStateImage(kControlStatePushed, strValue);
    else if (strName == _T("selectedforedisabledimage")) SetSelectedForeStateImage(kControlStateDisabled, strValue);
    else if (strName == _T("switchselect")) Selected(!IsSelected());
    else __super::SetAttribute(strName, strValue);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::PaintStatusColor(IRenderContext* pRender)
{
    if (!IsSelected()) {
        __super::PaintStatusColor(pRender);
        return;
    }

    if (IsPaintNormalFirst() && !m_selectedColorMap.HasColor())
        m_colorMap.PaintStatusColor(pRender, m_rcPaint, m_uButtonState);
    else
        m_selectedColorMap.PaintStatusColor(pRender, m_rcPaint, m_uButtonState);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::PaintStatusImage(IRenderContext* pRender)
{
    if (!IsSelected()) {
        __super::PaintStatusImage(pRender);
        return;
    }

    if (IsPaintNormalFirst() && !m_imageMap.HasImageType(kStateImageSelectedBk))
        m_imageMap.PaintStatusImage(pRender, kStateImageBk, m_uButtonState);
    else
        m_imageMap.PaintStatusImage(pRender, kStateImageSelectedBk, m_uButtonState);

    if (IsPaintNormalFirst() && !m_imageMap.HasImageType(kStateImageSelectedFore))
        m_imageMap.PaintStatusImage(pRender, kStateImageFore, m_uButtonState);
    else
        m_imageMap.PaintStatusImage(pRender, kStateImageSelectedFore, m_uButtonState);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::PaintText(IRenderContext* pRender)
{
    if (!IsSelected()) {
        __super::PaintText(pRender);
        return;
    }

    if (GetText().empty()) return;
    UiRect rc = m_rcItem;
    rc.left += m_rcTextPadding.left;
    rc.right -= m_rcTextPadding.right;
    rc.top += m_rcTextPadding.top;
    rc.bottom -= m_rcTextPadding.bottom;

    auto stateType = m_uButtonState;
    std::wstring clrColor = GetPaintSelectedStateTextColor(m_uButtonState, stateType);
    if (clrColor.empty())
        clrColor = m_dwSelectedTextColor.empty() ? GetPaintStateTextColor(m_uButtonState, stateType) : m_dwSelectedTextColor;
    DWORD dwClrColor = this->GetWindowColor(clrColor);

    if (m_bSingleLine)
        m_uTextStyle |= DT_SINGLELINE;
    else
        m_uTextStyle &= ~DT_SINGLELINE;

    if (m_animationManager.GetAnimationPlayer(kAnimationHot)) {
        if ((stateType == kControlStateNormal || stateType == kControlStateHot)
            && !GetSelectedStateTextColor(kControlStateHot).empty()) {
            std::wstring clrColor = GetSelectedStateTextColor(kControlStateNormal);
            if (!clrColor.empty()) {
                DWORD dwClrColor = this->GetWindowColor(clrColor);
                pRender->DrawText(rc, GetText(), dwClrColor, m_sFontId, m_uTextStyle, 255, m_bLineLimit);
            }

            if (m_nHotAlpha > 0) {
                std::wstring clrColor = GetSelectedStateTextColor(kControlStateHot);
                if (!clrColor.empty()) {
                    DWORD dwClrColor = this->GetWindowColor(clrColor);
                    pRender->DrawText(rc, GetText(), dwClrColor, m_sFontId, m_uTextStyle, (BYTE)m_nHotAlpha, m_bLineLimit);
                }
            }

            return;
        }
    }

    pRender->DrawText(rc, GetText(), dwClrColor, m_sFontId, m_uTextStyle, 255, m_bLineLimit);
}

template<typename InheritType>
std::wstring CheckBoxTemplate<InheritType>::GetSelectedStateImage(ControlStateType stateType)
{
    return m_imageMap.GetImagePath(kStateImageSelectedBk, stateType);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::SetSelectedStateImage(ControlStateType stateType, const std::wstring& pStrImage)
{
    m_imageMap.SetImage(kStateImageSelectedBk, stateType, pStrImage);
    if (GetFixedWidth() == DUI_LENGTH_AUTO || GetFixedHeight() == DUI_LENGTH_AUTO) {
        ArrangeAncestor();
    }
    else {
        Invalidate();
    }
}

template<typename InheritType>
std::wstring CheckBoxTemplate<InheritType>::GetSelectedTextColor()
{
    return m_dwSelectedTextColor;
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::SetSelectedTextColor(const std::wstring& dwTextColor)
{
    m_dwSelectedTextColor = dwTextColor;
    Invalidate();
}

template<typename InheritType /*= Control*/>
std::wstring ui::CheckBoxTemplate<InheritType>::GetSelectedStateTextColor(ControlStateType stateType)
{
    return m_selectedTextColorMap[stateType];
}

template<typename InheritType /*= Control*/>
void ui::CheckBoxTemplate<InheritType>::SetSelectedStateTextColor(ControlStateType stateType, const std::wstring& dwTextColor)
{
    m_selectedTextColorMap[stateType] = dwTextColor;
    Invalidate();
}

template<typename InheritType /*= Control*/>
std::wstring ui::CheckBoxTemplate<InheritType>::GetPaintSelectedStateTextColor(ControlStateType buttonStateType, ControlStateType& stateType)
{
    stateType = buttonStateType;
    if (stateType == kControlStatePushed && GetSelectedStateTextColor(kControlStatePushed).empty()) {
        stateType = kControlStateHot;
    }
    if (stateType == kControlStateHot && GetSelectedStateTextColor(kControlStateHot).empty()) {
        stateType = kControlStateNormal;
    }
    if (stateType == kControlStateDisabled && GetSelectedStateTextColor(kControlStateDisabled).empty()) {
        stateType = kControlStateNormal;
    }
    return GetSelectedStateTextColor(stateType);
}

template<typename InheritType>
std::wstring CheckBoxTemplate<InheritType>::GetSelectStateColor(ControlStateType stateType)
{
    return m_selectedColorMap[stateType];
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::SetSelectedStateColor(ControlStateType stateType, const std::wstring& stateColor)
{
    m_selectedColorMap[stateType] = stateColor;
    Invalidate();
}

template<typename InheritType>
std::wstring CheckBoxTemplate<InheritType>::GetSelectedForeStateImage(ControlStateType stateType)
{
    return m_imageMap.GetImagePath(kStateImageSelectedFore, stateType);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::SetSelectedForeStateImage(ControlStateType stateType, const std::wstring& pStrImage)
{
    m_imageMap.SetImage(kStateImageSelectedFore, stateType, pStrImage);
    if (GetFixedWidth() == DUI_LENGTH_AUTO || GetFixedHeight() == DUI_LENGTH_AUTO) {
        ArrangeAncestor();
    }
    else {
        Invalidate();
    }
}

typedef CheckBoxTemplate<Control> CheckBox;
typedef CheckBoxTemplate<Box> CheckBoxBox;

} // namespace ui

#endif // UI_CONTROL_CHECKBOX_H_