/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//			«десь будем работать с инвентарем
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
		NDX_Surface			ArchM; // еЄ мы и будем отображать
		NDX_Surface			Symb; // здесь вспомогательна€ графика
		NDX_Surface			ArchMT; // на эту поверхность будем выводить текст голодисков
		NDX_AAFont			MenuFnt; // этот шрифт дл€ меню

		HFONT				fnt; // шрифт дл€ стандартных надписей
		HFONT				fnt1; // шрифт дл€ вывода названий заметок
		HFONT				fnt2; // шрифт дл€ вывода текста заметок
		HFONT				fnt3; // шрифт дл€ вывода текста голодисков

		int mode; // какой режим сейчас активен
		int lastmode;
		int point_i; // выделен ли какой-то управл€ющий элемент
		// изначальные размеры поверхности
		int maintext_width;
		int maintext_height;

		int scy; // насколько проскролено
		int scroll; // сколько всего страниц
		int scroll_c; // текуща€ страница

		
		CPtrArray	Quests; // массив квестов

		// структура данных квестов
		typedef struct CQuest
		{
			CString qtitle; // название
			int qstatus; // статус

		};

		CPtrArray	Notes; // массив заметок

		// структура данных заметок
		struct CNote
		{
			CString nNoteName; // им€ заметки
			CString nTitle; // название
			CString nText; // содержание заметки
			NDX_Surface nImage; // картинка заметки
			bool nState; // раскрыта заметка или нет при просмотре
		};

		CPtrArray	Contacts; // массив контактов

		// структура данных контактов
		struct CContact
		{
			CString cContactName; // им€ файла контакта
			CString cName; // им€
			CString cText; // содержание контакта
			bool cState; // раскрыт контакт или нет при просмотре
		};


		CPtrArray	HoloDisks; // массив голодисков

		// структура данных голодисков
		struct CHoloDisk
		{
			CString hDiskName; // им€ файла голодиска
			CString hTitle; // название голодиска
			CStringArray hData; // содержание голодиска
			bool hState; // раскрыт голодиск или нет при просмотре
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

		// работа с квестами ///////////////////////
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

		// работа с заметками ///////////////////////
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

		// работа с контактами ///////////////////////
	public:
		void RealiseContacts();
		void AddContact(CString name);
	protected:
		void CloseAllContacts();
		bool FindContact(CString s);
		////////////////////////////////////////////

		// работа с голодисками ///////////////////////
	public:
		void RealiseHoloDisks();
		void AddHoloDisk(CString name, bool read=false);
	protected:
//		void CloseAllContacts();
		bool FindHoloDisk(CString s);
		////////////////////////////////////////////

};

#endif