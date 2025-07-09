#pragma once
#include "app_db.h"

enum ThreadId
{
	kThreadUI
};

class MoveControlForm : public ui::WindowImplBase
{
public:
	MoveControlForm(const std::wstring& class_name, const std::wstring& theme_directory, const std::wstring& layout_xml);
	~MoveControlForm();

	/**
	 * һ�������ӿ��Ǳ���Ҫ��д�Ľӿڣ����������������ӿ�����������
	 * GetSkinFolder		�ӿ�������Ҫ���ƵĴ���Ƥ����Դ·��
	 * GetSkinFile			�ӿ�������Ҫ���ƵĴ��ڵ� xml �����ļ�
	 * GetWindowClassName	�ӿ����ô���Ψһ��������
	 */
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;

	/**
	 * �յ� WM_CREATE ��Ϣʱ�ú����ᱻ���ã�ͨ����һЩ�ؼ���ʼ���Ĳ���
	 */
	virtual void InitWindow() override;

	/**
	 * �յ� WM_CLOSE ��Ϣʱ�ú����ᱻ����
	 */
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	/**
	* @brief ���յ�������������Ϣʱ������
	* @param[in] uMsg ��Ϣ����
	* @param[in] wParam ��Ϣ���Ӳ���
	* @param[in] lParam ��Ϣ���Ӳ���
	* @param[out] bHandled ���� false ������ɷ�����Ϣ���������ɷ�����Ϣ
	* @return ������Ϣ������
	*/
	virtual LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


public:
	static void ShowCustomWindow(const std::wstring& class_name, const std::wstring& theme_directory, const std::wstring& layout_xml);

private:
	//drag-drop���
	bool OnProcessAppItemDrag(ui::EventArgs* param);
	void DoDrag(ui::Control* pAppItem, POINT pt_offset);
	void DoBeforeDrag();
	void DoDraging(POINT pt_offset);
	bool DoAfterDrag(ui::Box* check);

private:
	std::wstring class_name_;
	std::wstring theme_directory_;
	std::wstring layout_xml_;

	ui::Box* frequent_app_=nullptr;
	ui::Box* my_app_ = nullptr;

	bool is_drag_state_=false;
	POINT old_drag_point_;
	AppItemUi* current_item_ = nullptr;
	
};

