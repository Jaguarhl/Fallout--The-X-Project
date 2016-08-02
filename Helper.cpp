#include <NukeDX.h>
#include <NukeFile.h>
#include "INI.h"
#include "Helper.h"

//extern NDX_Screen Screen;

void CHelper::InitHelper(NDX_Screen * PScreen)
{
	ParentScreen = PScreen;
	maintext_width = 800;
	maintext_height = 6000;

/*	MD.SetFileDescription("Fallout Data File");
	MD.Open("MASTER.DAT");

	if(MD.IsFileOpen())
	{
		MenuFnt.Load(MD.GetFile("INGAME/a_mnu.btf"));
		Symb.LoadBMP(ParentScreen,NDXST_SYSMEM,MD.GetFile("MISC/symb.bmp"),true);
		MD.Close();
	}*/


	fnt=CreateFont(15, 0, 0, 0, FW_BOLD,
		FALSE, FALSE, TRUE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, VARIABLE_PITCH, "Arial Cyr");

}

void CHelper::PrepareFrame(/*NDX_Screen * ParentScreen*/)
{

}

void CHelper::PrepareMenu(/*NDX_Screen * ParentScreen,*/ int hover)
{
	HelperM.Create(ParentScreen, NDXST_SYSMEM, maintext_width, maintext_height);
	HelperM.Clear(RGB(0,0,0));
	HelperM.SetColorKey(RGB(0,0,0));
	HelperM.SetTransP(true);

/*	MenuFnt.Text(&HelperM, 13, 18, ParentScreen->PixelFormat->Rgb(189,189,0), "1. Статус");
	HelperM.FillRect(13, 33, 71, 34, ParentScreen->PixelFormat->Rgb(189,189,0));
	MenuFnt.Text(&HelperM, 13, 50, ParentScreen->PixelFormat->Rgb(189,189,0), "2. Заметки");
	HelperM.FillRect(13, 65, 84, 66, ParentScreen->PixelFormat->Rgb(189,189,0));
	MenuFnt.Text(&HelperM, 13, 84, ParentScreen->PixelFormat->Rgb(189,189,0), "3. Записи");
	HelperM.FillRect(13, 99, 76, 100, ParentScreen->PixelFormat->Rgb(189,189,0));
	MenuFnt.Text(&HelperM, 13, 118, ParentScreen->PixelFormat->Rgb(189,189,0), "4. Контакты");
	HelperM.FillRect(13, 133, 91, 134, ParentScreen->PixelFormat->Rgb(189,189,0));

	if(hover!=0)
	{
		if(hover==1)
		{
			HelperM.FillRect(13, 18, 145, 34, RGB(0,0,0));
			MenuFnt.Text(&HelperM, 13, 18, ParentScreen->PixelFormat->Rgb(219,219,0), "1. Статус");
			HelperM.FillRect(13, 33, 71, 34, ParentScreen->PixelFormat->Rgb(229,229,0));
		}
		else if(hover==2)
		{
			HelperM.FillRect(13, 50, 84, 66, RGB(0,0,0));
			MenuFnt.Text(&HelperM, 13, 50, ParentScreen->PixelFormat->Rgb(219,219,0), "2. Заметки");
			HelperM.FillRect(13, 65, 84, 66, ParentScreen->PixelFormat->Rgb(229,229,0));
		}
		else if(hover==3)
		{
			HelperM.FillRect(13, 84, 84, 100, RGB(0,0,0));
			MenuFnt.Text(&HelperM, 13, 84, ParentScreen->PixelFormat->Rgb(219,219,0), "3. Записи");
			HelperM.FillRect(13, 99, 76, 100, ParentScreen->PixelFormat->Rgb(229,229,0));
		}
		else if(hover==4)
		{
			HelperM.FillRect(13, 118, 91, 134, RGB(0,0,0));
			MenuFnt.Text(&HelperM, 13, 118, ParentScreen->PixelFormat->Rgb(219,219,0), "4. Контакты");
			HelperM.FillRect(13, 133, 91, 134, ParentScreen->PixelFormat->Rgb(229,229,0));
		}
	}*/

}

void CHelper::PrepareHelperFrm(/*NDX_Screen * ParentScreen*/)
{

}

void CHelper::PrepareNotes(/*NDX_Screen * ParentScreen*/)
{

}

void CHelper::PrepareHoloDisks(/*NDX_Screen * ParentScreen*/)
{

}

void CHelper::PrepareHoloDiskText()
{

}

void CHelper::PrepareContacts(/*NDX_Screen * ParentScreen*/)
{

}

void CHelper::Mouse(int mX, int mY, bool lbut, NDX_Screen * ParentScreen)
{ // обрабатываем координаты мыши и нажатия
/*	if(mode==0)
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

	PrepareFrame();*/
}


///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////