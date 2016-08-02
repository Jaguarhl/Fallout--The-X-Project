#include <NukeDX.h>
#include <NukeFile.h>
#include "INI.h"
#include "Archiev.h"

//extern NDX_Screen Screen;

void CArchiev::InitArchiev(NDX_Screen * PScreen)
{
	ParentScreen = PScreen;
	maintext_width = 460;
	maintext_height = 350;

	scy = 0;
	mode = 0;
	lastmode=0;
	point_i = 0;

	MD.SetFileDescription("Fallout Data File");
	MD.Open("MASTER.DAT");

	if(MD.IsFileOpen())
	{
		MenuFnt.Load(MD.GetFile("INGAME/a_mnu.btf"));
		Symb.LoadBMP(ParentScreen,NDXST_SYSMEM,MD.GetFile("MISC/symb.bmp"),true);
		MD.Close();
	}

	RealiseQuest();
	RealiseNotes();
	RealiseContacts();
	RealiseHoloDisks();

	fnt=CreateFont(15, 0, 0, 0, FW_BOLD,
		FALSE, FALSE, TRUE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, VARIABLE_PITCH, "Arial Cyr");

	fnt1=CreateFont(15, 0, 0, 0, FW_BOLD,
		FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, VARIABLE_PITCH, "Arial Cyr");

	fnt2=CreateFont(13, 0, 0, 0, FW_REGULAR,
		FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, VARIABLE_PITCH, "Arial Cyr");

	fnt3=CreateFont(15, 0, 0, 0, FW_REGULAR,
		FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, VARIABLE_PITCH, "Arial Cyr");
}

void CArchiev::PrepareFrame(/*NDX_Screen * ParentScreen*/)
{
	if(mode==0)
		PrepareMenu(/*ParentScreen, */point_i);
	else if(mode==1)
		PrepareArchievFrm(/*ParentScreen*/);
	else if(mode==2)
		PrepareNotes(/*ParentScreen*/);
	else if(mode==3)
		PrepareHoloDisks();
	else if(mode==4)
		PrepareContacts(/*ParentScreen*/);
	else if(mode==5)
		PrepareHoloDiskText();
}

void CArchiev::PrepareMenu(/*NDX_Screen * ParentScreen,*/ int hover)
{
	ArchM.Create(ParentScreen, NDXST_SYSMEM, maintext_width, maintext_height);
	ArchM.Clear(RGB(0,0,0));
	ArchM.SetColorKey(RGB(0,0,0));
	ArchM.SetTransP(true);

	MenuFnt.Text(&ArchM, 13, 18, ParentScreen->PixelFormat->Rgb(189,189,0), "1. Статус");
	ArchM.FillRect(13, 33, 71, 34, ParentScreen->PixelFormat->Rgb(189,189,0));
	MenuFnt.Text(&ArchM, 13, 50, ParentScreen->PixelFormat->Rgb(189,189,0), "2. Заметки");
	ArchM.FillRect(13, 65, 84, 66, ParentScreen->PixelFormat->Rgb(189,189,0));
	MenuFnt.Text(&ArchM, 13, 84, ParentScreen->PixelFormat->Rgb(189,189,0), "3. Записи");
	ArchM.FillRect(13, 99, 76, 100, ParentScreen->PixelFormat->Rgb(189,189,0));
	MenuFnt.Text(&ArchM, 13, 118, ParentScreen->PixelFormat->Rgb(189,189,0), "4. Контакты");
	ArchM.FillRect(13, 133, 91, 134, ParentScreen->PixelFormat->Rgb(189,189,0));

	if(hover!=0)
	{
		if(hover==1)
		{
			ArchM.FillRect(13, 18, 145, 34, RGB(0,0,0));
			MenuFnt.Text(&ArchM, 13, 18, ParentScreen->PixelFormat->Rgb(219,219,0), "1. Статус");
			ArchM.FillRect(13, 33, 71, 34, ParentScreen->PixelFormat->Rgb(229,229,0));
		}
		else if(hover==2)
		{
			ArchM.FillRect(13, 50, 84, 66, RGB(0,0,0));
			MenuFnt.Text(&ArchM, 13, 50, ParentScreen->PixelFormat->Rgb(219,219,0), "2. Заметки");
			ArchM.FillRect(13, 65, 84, 66, ParentScreen->PixelFormat->Rgb(229,229,0));
		}
		else if(hover==3)
		{
			ArchM.FillRect(13, 84, 84, 100, RGB(0,0,0));
			MenuFnt.Text(&ArchM, 13, 84, ParentScreen->PixelFormat->Rgb(219,219,0), "3. Записи");
			ArchM.FillRect(13, 99, 76, 100, ParentScreen->PixelFormat->Rgb(229,229,0));
		}
		else if(hover==4)
		{
			ArchM.FillRect(13, 118, 91, 134, RGB(0,0,0));
			MenuFnt.Text(&ArchM, 13, 118, ParentScreen->PixelFormat->Rgb(219,219,0), "4. Контакты");
			ArchM.FillRect(13, 133, 91, 134, ParentScreen->PixelFormat->Rgb(229,229,0));
		}
	}

}

void CArchiev::PrepareArchievFrm(/*NDX_Screen * ParentScreen*/)
{
	ArchM.Create(ParentScreen, NDXST_SYSMEM, maintext_width, maintext_height);
	ArchM.Clear(RGB(0,0,0));
	ArchM.SetColorKey(RGB(0,0,0));
	ArchM.SetTransP(true);

	if(lastmode!=mode)
	{
		int line=0;
		for(int i=0; i<Quests.GetSize(); ++i)
		{
			DWORD Color;

			CString r = GetQTitle(i);

			if(GetQStatus(i)==1)
			{
				Color = RGB(/*207, 191, 63*/253, 233, 77);
				r+="  (в процессе)";
				Symb.Draw(&ArchM, 0, line*15+3, NDX_RECT(40, 0, 49, 9));

				LPSTR fn;
				fn = r.GetBuffer(r.GetLength());
				r.ReleaseBuffer();
				
				ArchM.FormattedText(20, line*15, Color, fnt1, fn);
			}
			else if(GetQStatus(i)==2)
			{
				Color = RGB(/*253, 176, 77*/216,114,52);
				r+="  (провалено)";
				Symb.Draw(&ArchM, 0, line*15+3, NDX_RECT(20, 0, 29, 9));

				LPSTR fn;
				fn = r.GetBuffer(r.GetLength());
				r.ReleaseBuffer();
				
				ArchM.FormattedText(20, line*15, Color, fnt1, fn);
			}
			else if(GetQStatus(i)==3)
			{
				Color = RGB(207, 191, 63);
				r+="  (завершено)";
				Symb.Draw(&ArchM, 0, line*15+3, NDX_RECT(30, 0, 39, 9));
				
				LPSTR fn;
				fn = r.GetBuffer(r.GetLength());
				r.ReleaseBuffer();
				
				ArchM.FormattedText(20, line*15, Color, fnt, fn);
			}

			++line;
		}
	}

	MenuFnt.Text(&ArchM, 230, 333, ParentScreen->PixelFormat->Rgb(189,189,0), "НАЗАД");
	ArchM.FillRect(230, 347, 272, 348, ParentScreen->PixelFormat->Rgb(189,189,0));

	if(point_i!=0)
	{
		if(point_i==1)
		{
			ArchM.FillRect(229, 332, 273, 349, RGB(0,0,0));
			MenuFnt.Text(&ArchM, 230, 333, ParentScreen->PixelFormat->Rgb(219,219,0), "НАЗАД");
			ArchM.FillRect(230, 347, 272, 348, ParentScreen->PixelFormat->Rgb(229,229,0));
		}
	}
}

void CArchiev::PrepareNotes(/*NDX_Screen * ParentScreen*/)
{
	ArchM.Create(ParentScreen, NDXST_SYSMEM, maintext_width, maintext_height);
	ArchM.Clear(RGB(0,0,0));
	ArchM.SetColorKey(RGB(0,0,0));
	ArchM.SetTransP(true);

	MenuFnt.Text(&ArchM, 230, 333, ParentScreen->PixelFormat->Rgb(189,189,0), "НАЗАД");
	ArchM.FillRect(230, 347, 272, 348, ParentScreen->PixelFormat->Rgb(189,189,0));

	if(lastmode!=mode)
	{
		int line=1;
		for(int i=0; i<Notes.GetSize(); ++i)
		{
			CNote* pNote = (CNote*) Notes.GetAt(i);
			DWORD Color = RGB(207, 191, 63);

			LPSTR fn;
			fn = pNote->nTitle.GetBuffer(pNote->nTitle.GetLength());
			pNote->nTitle.ReleaseBuffer();

			ArchM.FormattedText(20, line*15, Color, fnt1, fn);

			if(pNote->nState)
			{
				Symb.Draw(&ArchM, 0, line*15+3, NDX_RECT(10, 0, 19, 9));
				++line;

				ArchM.FillRect(25, line*15+2, 445, line*15+77, ParentScreen->PixelFormat->Rgb(15,15,15));
				WriteNoteText(pNote->nText, 145, line*15+5);
				pNote->nImage.Draw(&ArchM, 29, line*15+4);

				line+=6;
			}
			else
			{
				Symb.Draw(&ArchM, 0, line*15+3, NDX_RECT(0, 0, 9, 9));
				++line;
			}
		}
	}

	if(point_i!=0)
	{
		if(point_i==1)
		{
			ArchM.FillRect(229, 332, 273, 349, RGB(0,0,0));
			MenuFnt.Text(&ArchM, 230, 333, ParentScreen->PixelFormat->Rgb(219,219,0), "НАЗАД");
			ArchM.FillRect(230, 347, 272, 348, ParentScreen->PixelFormat->Rgb(229,229,0));
		}
	}
}

void CArchiev::PrepareHoloDisks(/*NDX_Screen * ParentScreen*/)
{
	ArchM.Create(ParentScreen, NDXST_SYSMEM, maintext_width, maintext_height);
	ArchM.Clear(RGB(0,0,0));
	ArchM.SetColorKey(RGB(0,0,0));
	ArchM.SetTransP(true);

	MenuFnt.Text(&ArchM, 230, 333, ParentScreen->PixelFormat->Rgb(189,189,0), "НАЗАД");
	ArchM.FillRect(230, 347, 272, 348, ParentScreen->PixelFormat->Rgb(189,189,0));

	if(lastmode!=mode)
	{
		int line=1;
		for(int i=0; i<HoloDisks.GetSize(); ++i)
		{
			CHoloDisk* pHoloDisk = (CHoloDisk*) HoloDisks.GetAt(i);
			DWORD Color = RGB(207, 191, 63);

			LPSTR fn;
			fn = pHoloDisk->hTitle.GetBuffer(pHoloDisk->hTitle.GetLength());
			pHoloDisk->hTitle.ReleaseBuffer();

//			ArchM.FillRect(0, line*15-1, 445, line*15+16, ParentScreen->PixelFormat->Rgb(25,25,25));

			ArchM.FormattedText(20, line*15, Color, fnt1, fn);

			if(pHoloDisk->hState)
				Symb.Draw(&ArchM, 0, line*15+3, NDX_RECT(30, 0, 39, 9));
			else
				Symb.Draw(&ArchM, 0, line*15+3, NDX_RECT(40, 0, 49, 9));
			++line;			
		}
	}

	if(point_i!=0)
	{
		if(point_i==1)
		{
			ArchM.FillRect(229, 332, 273, 349, RGB(0,0,0));
			MenuFnt.Text(&ArchM, 230, 333, ParentScreen->PixelFormat->Rgb(219,219,0), "НАЗАД");
			ArchM.FillRect(230, 347, 272, 348, ParentScreen->PixelFormat->Rgb(229,229,0));
		}
	}
}

void CArchiev::PrepareHoloDiskText()
{
	ArchM.Create(ParentScreen, NDXST_SYSMEM, maintext_width, maintext_height);
	ArchM.Clear(RGB(0,0,0));
	ArchM.SetColorKey(RGB(0,0,0));
	ArchM.SetTransP(true);

	MenuFnt.Text(&ArchM, 230, 333, ParentScreen->PixelFormat->Rgb(189,189,0), "НАЗАД");
	ArchM.FillRect(230, 347, 272, 348, ParentScreen->PixelFormat->Rgb(189,189,0));

	if(scroll_c != 0)
	{
		MenuFnt.Text(&ArchM, 10, 333, ParentScreen->PixelFormat->Rgb(189,189,0), "<<ПРЕДЫДУЩАЯ");
		ArchM.FillRect(10, 347, 122, 348, ParentScreen->PixelFormat->Rgb(189,189,0));
	}
	if(scroll_c < scroll)
	{
		MenuFnt.Text(&ArchM, 355, 333, ParentScreen->PixelFormat->Rgb(189,189,0), "СЛЕДУЮЩАЯ>>");
		ArchM.FillRect(355, 347, 457, 348, ParentScreen->PixelFormat->Rgb(189,189,0));
	}

	if(point_i!=0)
	{
		if(point_i==1)
		{
			ArchM.FillRect(229, 332, 273, 349, RGB(0,0,0));
			MenuFnt.Text(&ArchM, 230, 333, ParentScreen->PixelFormat->Rgb(219,219,0), "НАЗАД");
			ArchM.FillRect(230, 347, 272, 348, ParentScreen->PixelFormat->Rgb(229,229,0));
		}
		else if(point_i==2)
		{
			ArchM.FillRect(9, 332, 123, 349, RGB(0,0,0));
			MenuFnt.Text(&ArchM, 10, 333, ParentScreen->PixelFormat->Rgb(219,219,0), "<<ПРЕДЫДУЩАЯ");
			ArchM.FillRect(10, 347, 122, 348, ParentScreen->PixelFormat->Rgb(219,219,0));
		}
		else if(point_i==3)
		{
			ArchM.FillRect(354, 332, 458, 349, RGB(0,0,0));
			MenuFnt.Text(&ArchM, 355, 333, ParentScreen->PixelFormat->Rgb(219,219,0), "СЛЕДУЮЩАЯ>>");
			ArchM.FillRect(355, 347, 457, 348, ParentScreen->PixelFormat->Rgb(219,219,0));
		}
	}

	ArchMT.Draw(&ArchM, 0, 0, NDX_RECT(0, scroll_c*315, maintext_width, scroll_c*315+315));
}

void CArchiev::PrepareContacts(/*NDX_Screen * ParentScreen*/)
{
	ArchM.Create(ParentScreen, NDXST_SYSMEM, maintext_width, maintext_height);
	ArchM.Clear(RGB(0,0,0));
	ArchM.SetColorKey(RGB(0,0,0));
	ArchM.SetTransP(true);

	MenuFnt.Text(&ArchM, 230, 333, ParentScreen->PixelFormat->Rgb(189,189,0), "НАЗАД");
	ArchM.FillRect(230, 347, 272, 348, ParentScreen->PixelFormat->Rgb(189,189,0));

	if(lastmode!=mode)
	{
		int line=1;
		for(int i=0; i<Contacts.GetSize(); ++i)
		{
			CContact* pContact = (CContact*) Contacts.GetAt(i);

			LPSTR fn;
			fn = pContact->cName.GetBuffer(pContact->cName.GetLength());
			pContact->cName.ReleaseBuffer();

			ArchM.FormattedText(20, line*15, RGB(207, 191, 63), fnt1, fn);

			if(pContact->cState)
			{
				Symb.Draw(&ArchM, 0, line*15+3, NDX_RECT(10, 0, 19, 9));
				++line;

				ArchM.FillRect(25, line*15+2, 445, line*15+32, ParentScreen->PixelFormat->Rgb(15,15,15));
				WriteNoteText(pContact->cText, 30, line*15+3);

				line+=2;
			}
			else
			{
				Symb.Draw(&ArchM, 0, line*15+3, NDX_RECT(0, 0, 9, 9));
				++line;
			}
		}
	}

	if(point_i!=0)
	{
		if(point_i==1)
		{
			ArchM.FillRect(229, 332, 273, 349, RGB(0,0,0));
			MenuFnt.Text(&ArchM, 230, 333, ParentScreen->PixelFormat->Rgb(219,219,0), "НАЗАД");
			ArchM.FillRect(230, 347, 272, 348, ParentScreen->PixelFormat->Rgb(229,229,0));
		}
	}
}

void CArchiev::Mouse(int mX, int mY, bool lbut, NDX_Screen * ParentScreen)
{ // обрабатываем координаты мыши и нажатия
	if(mode==0)
	{
		if((mX>=13)&&(mX<=145)&&(mY>=18)&&(mY<=34))
		{
			point_i = 1;
			if(lbut)
			{
				lastmode=mode;
				mode=1;
			}
		}
		else if((mX>=13)&&(mX<=84)&&(mY>=50)&&(mY<=66))
		{
			point_i = 2;
			if(lbut)
			{
				lastmode=mode;
				mode=2;
			}
		}
		else if((mX>=13)&&(mX<=76)&&(mY>=84)&&(mY<=100))
		{
			point_i = 3;
			if(lbut)
			{
				lastmode=mode;
				mode=3;
			}
		}
		else if((mX>=13)&&(mX<=91)&&(mY>=118)&&(mY<=134))
		{
			point_i = 4;
			if(lbut)
			{
				lastmode=mode;
				mode=4;
			}
		}
		else
			point_i = 0;
	}
	else if(mode==1)
	{
		if((mX>=230)&&(mX<=272)&&(mY>=333)&&(mY<=348))
		{
			point_i = 1;
			if(lbut)
			{
				lastmode=mode;
				mode=0;
			}
		}
		else
			point_i = 0;
	}
	else if(mode==2)
	{
		if((mX>=0)&&(mX<=155)&&(mY>=15)&&(mY<=330)&&(lbut))
		{
			int line = 1;
			for(int i=0; i<Notes.GetSize(); ++i)
			{
				CNote* pNote = (CNote*) Notes.GetAt(i);

				if((mY>=line*15)&&(mY<=line*15+15)&&(lbut))
				{
					if(!pNote->nState)
					{
						CloseAllNotes();
						pNote->nState=true;
					}
					else
						pNote->nState=false;
				}
				if(pNote->nState)
					line+=6;
				
				++line;
			}
		}
		if((mX>=230)&&(mX<=272)&&(mY>=333)&&(mY<=348))
		{
			point_i = 1;
			if(lbut)
			{
				CloseAllNotes();

				lastmode=mode;
				mode=0;
			}
		}
		else
			point_i = 0;
	}
	else if(mode==3)
	{
		if((mX>=0)&&(mX<=155)&&(mY>=15)&&(mY<=330)&&(lbut))
		{
			int line = 1;
			for(int i=0; i<HoloDisks.GetSize(); ++i)
			{
				CHoloDisk* pHoloDisk = (CHoloDisk*) HoloDisks.GetAt(i);

				if((mY>=line*15)&&(mY<=line*15+15)&&(lbut)) // выбрали диск для просмотра
				{
					if(!pHoloDisk->hState) // если еще не смотрели - отметим, как прочитанный
					{
						pHoloDisk->hState=true;
					}

					// здесь проведём подготовку для просмотра
					PrepareHDView(i);

				}
								
				++line;
			}
		}
		if((mX>=230)&&(mX<=272)&&(mY>=333)&&(mY<=348))
		{
			point_i = 1;
			if(lbut)
			{
				lastmode=mode;
				mode=0;
			}
		}
		else
			point_i = 0;
	}
	else if(mode==4)
	{
		if((mX>=0)&&(mX<=155)&&(mY>=15)&&(mY<=330)&&(lbut))
		{
			int line = 1;
			for(int i=0; i<Contacts.GetSize(); ++i)
			{
				CContact* pContact = (CContact*) Contacts.GetAt(i);

				if((mY>=line*15)&&(mY<=line*15+15)&&(lbut))
				{
					if(!pContact->cState)
					{
						CloseAllContacts();
						pContact->cState=true;
					}
					else
						pContact->cState=false;
				}
				if(pContact->cState)
					line+=2;
				
				++line;
			}
		}
		if((mX>=230)&&(mX<=272)&&(mY>=333)&&(mY<=348))
		{
			point_i = 1;
			if(lbut)
			{
				CloseAllContacts();
				lastmode=mode;
				mode=0;
			}
		}
		else
			point_i = 0;
	}
	else if(mode==5)
	{
		if((mX>=230)&&(mX<=272)&&(mY>=333)&&(mY<=348))
		{
			point_i = 1;
			if(lbut)
			{
				mode=lastmode;
				lastmode=5;
			}
		}
		else if((mX>=10)&&(mX<=122)&&(mY>=333)&&(mY<=348)&&(scroll_c > 0))
		{
			point_i = 2;
			if(lbut)
			{
				scroll_c--;				
			}
		}
		else if((mX>=355)&&(mX<=457)&&(mY>=333)&&(mY<=348)&&(scroll_c < scroll))
		{
			point_i = 3;
			if(lbut)
			{
				scroll_c++;
			}
		}
		else
			point_i = 0;
	}

	PrepareFrame(/*ParentScreen*/);
}

///////////////////////////////////////////////////////////////
// Квестлог
//
// Будем хранить данные о квестах в связанном списке
///////////////////////////////////////////////////////////////
int CArchiev::FindQuest(CString s)
{
	int res = -1;

	if(Quests.GetSize()>0)
	{
		for(int i=0; i<Quests.GetSize(); ++i)
		{
			CQuest* pQuest = (CQuest*) Quests.GetAt(i);
			if(pQuest->qtitle==s)
				res = i;
		}
	}
	
	return res;
}

void CArchiev::ModifyQuest(CString title, int status)
{
	int i = FindQuest(title);
	if(i<Quests.GetSize())
	{
		CQuest* pQuest = (CQuest*) Quests.GetAt(i);

		if(pQuest->qstatus!=status)
		{
			pQuest->qstatus=status;
			Quests.SetAt(i, pQuest);
		}
	}
}

void CArchiev::AddQuest(CString title, int status)
{
	if(FindQuest(title)==-1)
	{
		CQuest* pQuest = new CQuest;
		pQuest->qtitle = title;
		pQuest->qstatus = status;
		
		Quests.Add(pQuest);
	}
	else
		ModifyQuest(title, status);
}

bool CArchiev::IsQuestPresent(CString title, int status)
{
	bool r=false;

	if((FindQuest(title)!=-1)&&(GetQStatus(FindQuest(title))))
		r=true;

	return r;
}

void CArchiev::DeleteQuest(CString title)
{
	int i = Quests.GetSize();
	CQuest* pQuest = (CQuest*) Quests.GetAt(i);

	while((pQuest->qtitle!=title)&&(i>0))
	{
		--i;
	}

	Quests.RemoveAt(i);
}

void CArchiev::RealiseQuest()
{
	Quests.RemoveAll();
}

int CArchiev::GetQStatus(int i)
{
	CQuest* pQuest = (CQuest*) Quests.GetAt(i);

	return pQuest->qstatus;
}

CString CArchiev::GetQTitle(int i)
{
	CQuest* pQuest = (CQuest*) Quests.GetAt(i);

	return pQuest->qtitle;
}


///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
// Заметки
//
// Будем хранить данные о заметках в связанном списке
///////////////////////////////////////////////////////////////

void CArchiev::AddNote(CString name)
{
	if(name!="")
	{
		CNote* pNote = new CNote;
		pNote->nNoteName = name;
		
		CString tmp = "TXT/";
		tmp+=name;	
		tmp+=".des";
		LPSTR fn;
		fn = tmp.GetBuffer(tmp.GetLength());
		tmp.ReleaseBuffer();

		CD.SetFileDescription("Fallout Data File");
		CD.Open("CRITTERS.DAT");

		if(CD.IsFileOpen())
		{
			CD.ExtractFile_ToFile(fn,"data\\tempt");
			
			CIni ini;
			ini.SetPathName("data\\tempt");
			
			pNote->nTitle = ini.GetString("Main", "Title", 0);
			pNote->nText = ini.GetString("Main", "Text", 0);
			pNote->nState = false;
			
			DeleteFile("data\\tempt");

			tmp = "IMG/";
			tmp += name;
			tmp += ".bmp";
			fn = tmp.GetBuffer(tmp.GetLength());
			tmp.ReleaseBuffer();
			pNote->nImage.LoadBMP(ParentScreen, NDXST_SYSMEM, CD.GetFile(fn), true);
		}

		CD.Close();

		Notes.Add(pNote);
	}
}

void CArchiev::RealiseNotes()
{
	Notes.RemoveAll();
}

void CArchiev::CloseAllNotes()
{
	for(int i=0; i<Notes.GetSize(); ++i)
	{
		CNote* pNote = (CNote*) Notes.GetAt(i);
		
		if(pNote->nState)
			pNote->nState=false;
	}
}

void CArchiev::WriteNoteText(CString str, int x, int y)
{
	int i = 0;
	int sn=0;
	int rn=0;
	int space=0;
	int lsp = 0;

	i = str.GetLength();

	for(int z=0; z<i; ++z)
	{
		if(str.GetAt(z)==' ')
		{
			space=z;
		}
				
		if((double)sn*5.5>280)
		{
			sn=0;
			z=space+1;
			DrawNoteString(str, lsp,space,rn, x, y);
			lsp=space+1;	
			++rn;
		}
		else if(str.GetAt(z)=='\n')
		{
			sn=0;
			DrawNoteString(str, lsp, z-1, rn, x, y);
			z=z+1;
			lsp=z;
			++rn;
		}
		else if(z==i-1)
			DrawNoteString(str, lsp, z, rn, x, y);

		++sn;
	}
}

void  CArchiev::DrawNoteString(CString desc_s, int lsp, int space, int rn, int x, int y)
{
	CString str;
	LPSTR dstr;
	for(int i=lsp; i<=space; ++i)
	{
		str+=desc_s.GetAt(i);
	}

	int l=str.GetLength();

	dstr = str.GetBuffer(l);
	str.ReleaseBuffer();

	ArchM.FormattedText(x,y+rn*11,RGB(0, 191, 243),fnt2,dstr);
//	AfxMessageBox(str);
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
// Контакты
//
// Будем хранить данные о контактах в связанном списке
///////////////////////////////////////////////////////////////

void CArchiev::AddContact(CString name)
{
	if(name!="")
	{
		if(!FindContact(name))
		{
			CContact* pContact = new CContact;
			pContact->cContactName = name;
			
			CString tmp = "TXT/";
			tmp+=name;	
			tmp+=".des";
			LPSTR fn;
			fn = tmp.GetBuffer(tmp.GetLength());
			tmp.ReleaseBuffer();

			CD.SetFileDescription("Fallout Data File");
			CD.Open("CRITTERS.DAT");

			if(CD.IsFileOpen())
			{
				CD.ExtractFile_ToFile(fn,"data\\tempt");
				
				CIni ini;
				ini.SetPathName("data\\tempt");
				
				pContact->cName = ini.GetString("Main", "Name", 0);
				pContact->cText = ini.GetString("Main", "Who", 0);
				pContact->cText += ". \n";
				pContact->cText += ini.GetString("Main", "Location", 0);
				pContact->cState = false;

				DeleteFile("data\\tempt");
			}

			CD.Close();

			Contacts.Add(pContact);
		}
	}
}

void CArchiev::RealiseContacts()
{
	Contacts.RemoveAll();
}

void CArchiev::CloseAllContacts()
{
	for(int i=0; i<Contacts.GetSize(); ++i)
	{
		CContact* pContact = (CContact*) Contacts.GetAt(i);
		
		if(pContact->cState)
			pContact->cState=false;
	}
}

bool CArchiev::FindContact(CString s)
{
	bool r=false;
	if(Contacts.GetSize()>0)
	{
		for(int i=0; i<Contacts.GetSize(); ++i)
		{
			CContact* pContact = (CContact*) Contacts.GetAt(i);
			if(pContact->cContactName==s)
				return true;
		}
	}
	
	return r;
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
// Голодиски
//
// Будем хранить данные о голодисках в связанном списке
///////////////////////////////////////////////////////////////

void CArchiev::AddHoloDisk(CString name, bool read)
{
	if(name!="")
	{
		if(!FindHoloDisk(name))
		{
			CHoloDisk* pHoloDisk = new CHoloDisk;
			pHoloDisk->hDiskName = name;
			
			CString tmp = "TXT/";
			tmp+=name;	
			tmp+=".des";
			LPSTR fn;
			fn = tmp.GetBuffer(tmp.GetLength());
			tmp.ReleaseBuffer();

			CD.SetFileDescription("Fallout Data File");
			CD.Open("CRITTERS.DAT");

			if(CD.IsFileOpen())
			{
				CD.ExtractFile_ToFile(fn,"data\\tempt");
				
				CIni ini;
				ini.SetPathName("data\\tempt");
				
				pHoloDisk->hTitle = ini.GetString("Main", "Title", 0);
				pHoloDisk->hState = read;

				DeleteFile("data\\tempt");
			}

			tmp = "TXT/";
			tmp+=name;	
			tmp+=".txt";
			fn = tmp.GetBuffer(tmp.GetLength());
			tmp.ReleaseBuffer();

			if(CD.IsFileOpen())
			{
				CD.ExtractFile_ToFile(fn,"data\\tempt");
				
				CStdioFile file;
				
				BOOL opened = file.Open("data\\tempt", CFile::modeRead);
				if (opened)
				{
					CString string;
					while(file.ReadString(string))
					{
						pHoloDisk->hData.Add(string);
					}					
				}
				else
					AfxMessageBox("Can't find data file!");

				DeleteFile("data\\tempt");
			}

			CD.Close();

			HoloDisks.Add(pHoloDisk);
		}
	}
}

void CArchiev::RealiseHoloDisks()
{
	HoloDisks.RemoveAll();
}

bool CArchiev::FindHoloDisk(CString s)
{
	bool r=false;
	if(HoloDisks.GetSize()>0)
	{
		for(int i=0; i<HoloDisks.GetSize(); ++i)
		{
			CHoloDisk* pHoloDisk = (CHoloDisk*) HoloDisks.GetAt(i);
			if(pHoloDisk->hDiskName==s)
				return true;
		}
	}
	
	return r;
}

void CArchiev::PrepareHDView(int i) // выведем текст голодиска и подготовим его для отображения
{
	CHoloDisk* pHoloDisk = (CHoloDisk*) HoloDisks.GetAt(i);
	int size = pHoloDisk->hData.GetSize(); // узнаем кол-во строк текста
	size = size * 15; // определим размер поверхности для текста по высоте
	if(size < maintext_height)
		size=maintext_height;

	scroll = size / 315; // сколько страниц текста получаем для прокрутки
	scroll_c = 0; // установим текущую страницу

	ArchMT.Create(ParentScreen, NDXST_SYSMEM, maintext_width, scroll*315+315);
	ArchMT.Clear(RGB(0,0,0));
	ArchMT.SetColorKey(RGB(0,0,0));
	ArchMT.SetTransP(true);

	DWORD Color = RGB(207, 191, 63);
	DWORD Color1 = RGB(253, 233, 77);
	LPSTR fn;
	CString r;
	bool date=false;

	for(int k=0; k<pHoloDisk->hData.GetSize(); ++k)
	{
		r = pHoloDisk->hData.GetAt(k);
		if ((r.GetLength() > 0) && (r.GetAt(0) == '¤'))
		{
			date=true;
			r.Delete(0);			
		}
		else
			date=false;

		fn = r.GetBuffer(r.GetLength());
		r.ReleaseBuffer();

		if (date)
		{
			ArchMT.FormattedText(10, k*15, Color1, fnt1, fn);
		}
		else
			ArchMT.FormattedText(25, k*15, Color, fnt3, fn);

	}

	// переключим в режим отображения
	lastmode = mode;
	mode = 5;
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////