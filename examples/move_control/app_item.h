#pragma once

#define APP_HEIGHT 90
#define APP_WIDTH  90
#define EACH_LINE  6

#include <string>
//app�ľ�����Ϣ������ٶ���id��name��_icon,_isFrequent������չ
struct AppItem
{
	std::string _id;
	std::wstring _name;
	std::wstring _icon;
	bool _isFrequent=false;
};


//App UI��
class AppItemUi : public ui::VBox
{
public:
	static AppItemUi* Create(const AppItem& item);
	virtual void DoInit();
	void SetAppdata(const AppItem& item,bool refresh);
	void FixPos(int step,int index=-1);   //ǰ��/���˶��ٲ�  ĿǰӦ����-1 0 1    
	inline int getIndex() const { return index_; }
	inline const AppItem& getAppData() const { return app_data_; }
private:
	AppItem app_data_;
	int index_ = 0;    //�ڼ���
	ui::Control* app_icon_ = nullptr;
	ui::Label* app_name_ = nullptr;
};


//AppWindow �϶���ʾ������
//��ð�͸��
class AppWindow : public ui::WindowImplBase
{
public:
	AppWindow();
	~AppWindow();

	static AppWindow* CreateAppWindow(HWND hParent, POINT pt, const AppItem& Item)
	{
		AppWindow* ret = new AppWindow;
		ret->SetBeforeCreate(Item, pt);
		ret->Create(hParent, L"", WS_POPUP, WS_EX_TOOLWINDOW);
		pThis_ = ret;
		//��Ҫ�ı���pos,�Ӻ�initWindows
		return ret;
	}

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

	//�����Ĺ��ܺ���
	void SetBeforeCreate(const AppItem& Item, POINT pt){ item_ = Item; pt_ = pt; }
	void AdjustPos();
	void InstallHook();
	void UnInstallHook();
	static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);
private:
	AppItem item_;
	ui::Box* origin_owner = nullptr;  //
	POINT pt_;
	static HHOOK mouse_Hook_ ;
	static AppWindow* pThis_;
};

