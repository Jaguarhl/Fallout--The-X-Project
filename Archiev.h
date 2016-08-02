/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//			����� ����� �������� � ����������
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __ARCHIEV_H
#define __ARCHIEV_H

class CArchiev : public CObject
{
	public:

		NDX_Screen*			ParentScreen;
		CNukeFile			MD; // master.dat
		CNukeFile			CD; // critters.dat
		NDX_Surface			ArchM; // � �� � ����� ����������
		NDX_Surface			Symb; // ����� ��������������� �������
		NDX_Surface			ArchMT; // �� ��� ����������� ����� �������� ����� ����������
		NDX_AAFont			MenuFnt; // ���� ����� ��� ����

		HFONT				fnt; // ����� ��� ����������� ��������
		HFONT				fnt1; // ����� ��� ������ �������� �������
		HFONT				fnt2; // ����� ��� ������ ������ �������
		HFONT				fnt3; // ����� ��� ������ ������ ����������

		int mode; // ����� ����� ������ �������
		int lastmode;
		int point_i; // ������� �� �����-�� ����������� �������
		// ����������� ������� �����������
		int maintext_width;
		int maintext_height;

		int scy; // ��������� �����������
		int scroll; // ������� ����� �������
		int scroll_c; // ������� ��������

		
		CPtrArray	Quests; // ������ �������

		// ��������� ������ �������
		typedef struct CQuest
		{
			CString qtitle; // ��������
			int qstatus; // ������

		};

		CPtrArray	Notes; // ������ �������

		// ��������� ������ �������
		struct CNote
		{
			CString nNoteName; // ��� �������
			CString nTitle; // ��������
			CString nText; // ���������� �������
			NDX_Surface nImage; // �������� �������
			bool nState; // �������� ������� ��� ��� ��� ���������
		};

		CPtrArray	Contacts; // ������ ���������

		// ��������� ������ ���������
		struct CContact
		{
			CString cContactName; // ��� ����� ��������
			CString cName; // ���
			CString cText; // ���������� ��������
			bool cState; // ������� ������� ��� ��� ��� ���������
		};


		CPtrArray	HoloDisks; // ������ ����������

		// ��������� ������ ����������
		struct CHoloDisk
		{
			CString hDiskName; // ��� ����� ���������
			CString hTitle; // �������� ���������
			CStringArray hData; // ���������� ���������
			bool hState; // ������� �������� ��� ��� ��� ���������
		};

	public:
		void InitArchiev(NDX_Screen * PScreen);
		void PrepareFrame(/*NDX_Screen * ParentScreen*/);

		void Mouse(int mX, int mY, bool lbut, NDX_Screen * ParentScreen);

	protected:
		void PrepareMenu(/*NDX_Screen * ParentScreen,*/ int hover=0);
		void PrepareArchievFrm(/*NDX_Screen * ParentScreen*/);
		void PrepareNotes(/*NDX_Screen * ParentScreen*/);
		void PrepareContacts(/*NDX_Screen * ParentScreen*/);
		void PrepareHoloDisks();
		void PrepareHoloDiskText();
		void PrepareHDView(int i);

		// ������ � �������� ///////////////////////
	public:
		void RealiseQuest();
		void AddQuest(CString title, int status);
		bool IsQuestPresent(CString title, int status);
	protected:
		int FindQuest(CString s);
		void ModifyQuest(CString title, int status);
		void DeleteQuest(CString title);
		int GetQStatus(int i);
		CString GetQTitle(int i);
		////////////////////////////////////////////

		// ������ � ��������� ///////////////////////
	public:
		void RealiseNotes();
		void AddNote(CString name);
	protected:
//		CString GetNTitle(int i);
//		CString GetNText(int i);
		void WriteNoteText(CString str, int x, int y);
		void DrawNoteString(CString desc_s, int lsp, int space, int rn, int x, int y);
		void CloseAllNotes();
		////////////////////////////////////////////

		// ������ � ���������� ///////////////////////
	public:
		void RealiseContacts();
		void AddContact(CString name);
	protected:
		void CloseAllContacts();
		bool FindContact(CString s);
		////////////////////////////////////////////

		// ������ � ����������� ///////////////////////
	public:
		void RealiseHoloDisks();
		void AddHoloDisk(CString name, bool read=false);
	protected:
//		void CloseAllContacts();
		bool FindHoloDisk(CString s);
		////////////////////////////////////////////

};

#endif