///////////////////////////////////////////////////////////////
// InfoDocument.h
// InfoDocument - ��������, ���������� �������� ����������
///////////////////////////////////////////////////////////////


#pragma once

#include "inventory_item_object.h"
#include "InfoPortionDefs.h"
#include "script_export_space.h"

class CInfoDocument: public CInventoryItemObject {
private:
    typedef	CInventoryItemObject inherited;
public:
	CInfoDocument(void);
	virtual ~CInfoDocument(void);

	virtual BOOL net_Spawn			(CSE_Abstract* DC);
	virtual void Load				(LPCSTR section);
	virtual void net_Destroy		();
	virtual void shedule_Update		(u32 dt);
	virtual void UpdateCL			();
	virtual void renderable_Render	();

	virtual void OnH_A_Chield		();
	virtual void OnH_B_Independent	(bool just_before_destroy);

	DECLARE_SCRIPT_REGISTER_FUNCTION

protected:
	//������ ����������, ������������ � ���������
	std::vector<std::string> m_Info;
};

add_to_type_list(CInfoDocument)
#undef script_type_list
#define script_type_list save_type_list(CInfoDocument)
