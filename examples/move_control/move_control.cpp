#include "stdafx.h"
#include "move_control.h"

using namespace ui;
using namespace std;


MoveControlForm::MoveControlForm(const std::wstring& class_name, const std::wstring& theme_directory, const std::wstring& layout_xml)
	: class_name_(class_name)
	, theme_directory_(theme_directory)
	, layout_xml_(layout_xml)
{
}


MoveControlForm::~MoveControlForm()
{
}

std::wstring MoveControlForm::GetSkinFolder()
{
	return theme_directory_;
}

std::wstring MoveControlForm::GetSkinFile()
{
	return layout_xml_;
}

std::wstring MoveControlForm::GetWindowClassName() const
{
	return class_name_;
}

void MoveControlForm::InitWindow()
{
	//���Ӧ�á�Ӧ���п����Ƿ������·��ģ�һ�㱾��Ҳ�б����
	//loadFromDb
	//getFromServer---->��̨�����ȱ��浽db����post����Ϣ�������������´�db load��

	//��Ϊdemo����д��
	std::vector<AppItem> applist;
	CAppDb::GetInstance().LoadFromDb(applist);

	frequent_app_ = static_cast<ui::Box*>(FindControl(L"frequent_app"));
	my_app_ = static_cast<ui::Box*>(FindControl(L"my_app"));
	
	for (const auto& item: applist)
	{
		AppItemUi* pAppUi = AppItemUi::Create(item);
		pAppUi->AttachAllEvents(nbase::Bind(&MoveControlForm::OnProcessAppItemDrag, this, std::placeholders::_1));
		if (item._isFrequent)
		{
			pAppUi->FixPos(0, frequent_app_->GetCount());
			frequent_app_->Add(pAppUi);
		}
		else
		{
			pAppUi->FixPos(0, my_app_->GetCount());
			my_app_->Add(pAppUi);
		}
	}
}

LRESULT MoveControlForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}


LRESULT MoveControlForm::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (current_item_ == nullptr)
	{
		return __super::OnLButtonUp(uMsg,wParam,lParam,bHandled);
	}

	Box* pParent = current_item_->GetParent();
	pParent->SetAutoDestroyChild(true);

	if (!DoAfterDrag(frequent_app_) && !DoAfterDrag(my_app_))
	{
		//�ع�
		pParent->AddAt(current_item_, current_item_->getIndex());
		//��index����ʼ��ȱ��
		for (int index = current_item_->getIndex()+1; index < pParent->GetCount(); ++index)
		{
			AppItemUi* _pItem = dynamic_cast<AppItemUi*>(pParent->GetItemAt(index));
			if (_pItem)
			{
				_pItem->FixPos(+1);
			}
		}
	}

	//����App��Ϣ�����ݿ�
	CAppDb::GetInstance().SaveToDb(current_item_->getAppData());

	is_drag_state_ = false;
	current_item_ = nullptr;
	SetForegroundWindow(m_hWnd);
	SetActiveWindow(m_hWnd);
	return __super::OnLButtonUp(uMsg, wParam, lParam, bHandled);
}

void MoveControlForm::ShowCustomWindow(const std::wstring& class_name, const std::wstring& theme_directory, const std::wstring& layout_xml)
{
	MoveControlForm* window = new MoveControlForm(class_name, theme_directory, layout_xml);
	window->Create(NULL, class_name.c_str(), WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
	window->CenterWindow();
	window->ShowWindow();
}

//����취ץ����굯���һ��
bool MoveControlForm::OnProcessAppItemDrag(ui::EventArgs* param)
{
	switch (param->Type)
	{
	case kEventMouseMove:
	{
		if (::GetKeyState(VK_LBUTTON) >= 0)
			break;
		if (!is_drag_state_)
		{
			break;
		}
		//���λ��
		LONG cx = abs(param->ptMouse.x - old_drag_point_.x);
		LONG cy = abs(param->ptMouse.y - old_drag_point_.y);
		if (cx < 2 && cy < 2)
		{
			break;
		}
		//����קģʽ��
		//��ȡ������AppItem��λ��
		ui::UiRect rect = param->pSender->GetPos(); //���Ͻ���Ч
		POINT pt = { param->ptMouse.x - rect.left, param->ptMouse.y - rect.top };

		DoDrag(param->pSender, pt);
		is_drag_state_ = false;
	}
		break;
	case kEventMouseButtonDown:
	{
		is_drag_state_ = true;
		old_drag_point_ = param->ptMouse;
	}
		break;
	case kEventMouseButtonUp:
	{
		is_drag_state_ = false;
		//DoDrop

	}
		break;
	}
	return true;
}

void MoveControlForm::DoDrag(ui::Control* pAppItem, POINT pos)
{
	current_item_ = dynamic_cast<AppItemUi*>(pAppItem);
	if (nullptr==current_item_)
	{
		return;
	}
	DoBeforeDrag();
	DoDraging(pos);

}

void MoveControlForm::DoBeforeDrag()
{
	//�ٳ�����Ŀ���������Ŀȫ������
	ASSERT(current_item_);
	if (current_item_)
	{
		Box* pParent = current_item_->GetParent();
		ASSERT(pParent);
		pParent->SetAutoDestroyChild(false);  //�ӿؼ�������
		pParent->Remove(current_item_);

		//��index����ʼ��ȱ��
		for (int index = current_item_->getIndex(); index < pParent->GetCount(); ++index)
		{
			AppItemUi* _pItem = dynamic_cast<AppItemUi*>(pParent->GetItemAt(index));
			if (_pItem)
			{
				_pItem->FixPos(-1);
			}
		}
	}
}

void MoveControlForm::DoDraging(POINT pos)
{
	//����ע�⣬���ֻ�Ǹ��ؼ��ڲ��ƶ��Ļ�����򵥺ܶ�
	//������current_item_��setmargin������add��ȥ���ȱ����ڸ��ؼ������һ��
	//index_����֮ǰ��λ�ã���ȡ����������굯��ʱ���������º����ֵ�������ڸ��ؼ���λ��

	//������ƶ��Ļ�����Ҫ����drag-drop��Ҳ�ǿ���ʵ�ֵģ��������

	//��Demoʵ�ֵ��ǿ縸�ؼ��ƶ������ݸ��ؼ��ڲ��ƶ��������ҿ����ƶ������ڷ�Χ����˴�����ʱ����
	//�ǳ��ź�������ʱ���ڴ���ʱ����ʱ���ڲ�û�м�ʱ����ק�У������ȡHook��������mousemove��Ϣ�ƶ���


	//���ﴴ���´��� ��Ȼ��ȷ�������ظ��д��ڣ�����ʡ��
	AppWindow* pWindow = AppWindow::CreateAppWindow(GetHWND(), pos, current_item_->getAppData());
	ASSERT(pWindow);
}

bool MoveControlForm::DoAfterDrag(ui::Box* check)
{
	//��ȡ����λ��
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);
	int findIndex = 0;
	UiRect rectBox = check->GetPos();
	if (rectBox.IsPointIn(pt))
	{
		//������غ��������ģ�����������λ������
		for (findIndex = 0; findIndex < check->GetCount(); findIndex++)
		{
			auto control = check->GetItemAt(findIndex);
			UiRect rectCtrl = control->GetPos();
			if (rectCtrl.IsPointIn(pt))
			{
				//���뵽��index
				break;
			}
		}
		//����������
		if (findIndex < check->GetCount())
		{
			current_item_->FixPos(0, findIndex);
			check->AddAt(current_item_, findIndex);
			//��index����ʼ��ȱ��
			for (int index = findIndex + 1; index < check->GetCount(); ++index)
			{
				AppItemUi* _pItem = dynamic_cast<AppItemUi*>(check->GetItemAt(index));
				if (_pItem)
				{
					_pItem->FixPos(+1);
				}
			}
			return true;
		}
		else
		{
			//�ŵ������
			current_item_->FixPos(0, findIndex);
			check->Add(current_item_);
			return true;
		}
	}
	else
	{
		return false;
	}
	
}


