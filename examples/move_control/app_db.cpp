#include "stdafx.h"
#include "app_db.h"

CAppDb::CAppDb()
{
	//������Դ�db��һЩ���� 
}


CAppDb::~CAppDb()
{
	//������Թر�db��һЩ����
}

bool CAppDb::LoadFromDb(std::vector<AppItem>& app_list)
{
	app_list.clear();
	//select from....
	//Ŀǰд��
	{
		AppItem item;
		item._id = "1";
		item._icon = L"1.png";
		item._isFrequent = false;
		item._name = L"���з���";
		app_list.emplace_back(item);
	}
	{
		AppItem item;
		item._id = "2";
		item._icon = L"2.png";
		item._isFrequent = false;
		item._name = L"����Ԥ��";
		app_list.emplace_back(item);
	}
	{
		AppItem item;
		item._id = "3";
		item._icon = L"3.png";
		item._isFrequent = true;
		item._name = L"����";
		app_list.emplace_back(item);
	}
	{
		AppItem item;
		item._id = "4";
		item._icon = L"4.png";
		item._isFrequent = true;
		item._name = L"���ʵ�";
		app_list.emplace_back(item);
	}
	{
		AppItem item;
		item._id = "5";
		item._icon = L"5.png";
		item._isFrequent = false;
		item._name = L"����5";
		app_list.emplace_back(item);
	}
	{
		AppItem item;
		item._id = "6";
		item._icon = L"5.png";
		item._isFrequent = false;
		item._name = L"����6";
		app_list.emplace_back(item);
	}
	{
		AppItem item;
		item._id = "7";
		item._icon = L"5.png";
		item._isFrequent = false;
		item._name = L"����7";
		app_list.emplace_back(item);
	}
	{
		AppItem item;
		item._id = "8";
		item._icon = L"5.png";
		item._isFrequent = false;
		item._name = L"����8";
		app_list.emplace_back(item);
	}
	{
		AppItem item;
		item._id = "9";
		item._icon = L"5.png";
		item._isFrequent = false;
		item._name = L"����9";
		app_list.emplace_back(item);
	}
	{
		AppItem item;
		item._id = "10";
		item._icon = L"5.png";
		item._isFrequent = false;
		item._name = L"����10";
		app_list.emplace_back(item);
	}
	{
		AppItem item;
		item._id = "11";
		item._icon = L"5.png";
		item._isFrequent = false;
		item._name = L"����11";
		app_list.emplace_back(item);
	}
	
	return true;
}

bool CAppDb::SaveToDb(const AppItem& item)
{
	// insert or replace into ....
	return true;
}
