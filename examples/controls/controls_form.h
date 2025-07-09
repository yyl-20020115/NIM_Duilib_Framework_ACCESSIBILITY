#pragma once

enum ThreadId
{
	kThreadUI,
	kThreadGlobalMisc
};

class ControlForm : public ui::WindowImplBase
{
public:
	ControlForm();
	~ControlForm();

	/**
	 * һ�������ӿ��Ǳ���Ҫ��д�Ľӿڣ����������������ӿ�����������
	 * GetSkinFolder		�ӿ�������Ҫ���ƵĴ���Ƥ����Դ·��
	 * GetSkinFile			�ӿ�������Ҫ���ƵĴ��ڵ� xml �����ļ�
	 * GetWindowClassName	�ӿ����ô���Ψһ��������
	 */
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;
	virtual ui::Control* CreateControl(const std::wstring& pstrClass) override;

	/**
	 * �յ� WM_CREATE ��Ϣʱ�ú����ᱻ���ã�ͨ����һЩ�ؼ���ʼ���Ĳ���
	 */
	virtual void InitWindow() override;

	/**
	 * �յ� WM_CLOSE ��Ϣʱ�ú����ᱻ����
	 */
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	 * ��ʶ���� class name
	 */
	static const std::wstring kClassName;

private:
	/**
	 * ��Ͷ�ݵ������̶߳�ȡ xml ���ݵ�������
	 */
	void LoadRichEditData();

	/**
	 * �����������̶߳�ȡ xml ��ɺ���� UI ���ݵĽӿ�
	 */
	void OnResourceFileLoaded(const std::wstring& xml);

	/**
	 * ��̬���½������ӿ�
	 */
	void OnProgressValueChagned(float value);
};

