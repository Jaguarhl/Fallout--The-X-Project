/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//			����� ����� �������� � ����������
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __BARTER_H
#define __BARTER_H

#define INV_WEAPON		1
#define INV_MISC		2
#define INV_MEDICAL		3
#define INV_AMMO		4
#define INV_FOOD		5
#define INV_TOOL		6
#define INV_BOOK		7
#define INV_MONEY		8

const int y_bsize = 55; // ������ ������ ���� � ����������� �����������
const int i_brheight = 220; // ������ ������� ��������� � �������
const int i_brweight = 80; // ������ ������� ��������� � �������

class CBarter : public CObject
{
	public:

		int sc_y;			// �� ������� ��������� �����������

		int cur_itm;		// ��� ������ �������� / ���������������

		bool scrollable;	// ��������� �������?

		int cost;			// ���� ������ �����
		double koeff;		// ����������� ������� � ���������

		CString	s_name;		// ����������, ��� ���������
		CString	s_image;	// ����� �������� ��������� ��� ���������

//		NDX_Surface inv_list;		// ������ ����������� ��� ����������� � ���������
		NDX_Surface inv_blist;		// ������ ����������� ��� ����������� � ������ �������
		NDX_Surface inv_source;		// ����������� ������������ ���������
		NDX_Surface inv_desc;		// �������� �������� ����� �������� ����

		CNukeFile CD;

		struct CItem
		{
			DWORD type;			// ��� ����
			DWORD stype;
			int amount;			// ����������
			int i_cost;			// ���������
//			CString image;		// �����������
			NDX_Surface img;	
			CString name;		// ��������
			CString i_desc;		// ��� ����� �������� �������
		};

		CPtrArray	m_Inventory;


	public:

		void InitInventory();

		///////////////////////////////////////////////////////////////////////////////////////////
		// ����������� � ������ � ���������
		///////////////////////////////////////////////////////////////////////////////////////////

		// �������� ���� � ���������
		void AddItem(int amount, CString desc, NDX_Screen * ParentScreen);
		// ��������
		void ModifyItem(int amount, CString desc, NDX_Screen * ParentScreen);
		// �����
		void GetItem(int amount, CString desc, NDX_Screen * ParentScreen);
		// �������� �� ������
		void ReplaceItem(CString new_desc, NDX_Screen * ParentScreen, CString desc);

		void DeleteItem(CString desc, NDX_Screen * ParentScreen);

		CString GetDescription(CString desc);

		CString GetDescription();

		CString GetDescription(int i);

		CString GetFDescription();

		CString GetSubstitute();

		int GetAmount();

		int GetAmount(int i);

		int GetType(int i);

		int GetType();

		void GetCost();

		bool Presence(CString desc);

		void ClearAll(CString s, CString r, NDX_Screen * ParentScreen);

		void UseMiscItem(NDX_Screen * ParentScreen);


		///////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////
		// ����������� �����
		///////////////////////////////////////////////////////////////////////////////////////////

		// ���������� ����������� � ������������� ����������� ���������
		void DrawBInventory(NDX_Screen * ParentScreen);

		// ��������� ������ ��� �������� ������ �����
		void DrawItem(NDX_Surface* dest, int x, int y);

		// ��������� �������� �������
		void DrawDescription(NDX_Screen* ParentScreen);


		///////////////////////////////////////////////////////////////////////////////////////////
		// ������ � �����
		///////////////////////////////////////////////////////////////////////////////////////////

		// ������ ������ ������� �� ����������� ����
		int GetObjIndex(int x, int y);

		// ���������� ������ �� ����������� ���� (������� �������)
		void DrawMoveObj(NDX_Surface* dest, int x, int y);


	protected:

		int FindItem(CString name);
		int FindPareItem(DWORD st);
		CString GetIDesc(int i);

		void WriteDescText(CString str);
		void DrawDescString(CString desc_s, int lsp, int space, int rn, HFONT fnt);
		
};

#endif

