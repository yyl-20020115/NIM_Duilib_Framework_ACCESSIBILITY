#pragma once

#include "app_item.h"

//��app��ص�Db����
//���������ǵ�ʵ����
class CAppDb
{
public:
	static CAppDb& GetInstance()
	{
		static CAppDb db;
		return db;
	}

	CAppDb();
	~CAppDb();

	bool LoadFromDb(std::vector<AppItem>& app_list);
	bool SaveToDb(const AppItem& item);

private:

};

