//////////////////////////////////////////////////////////////////////////
// XProject.CPP: Main source code for Blade Text Adventure Technology.	//
//////////////////////////////////////////////////////////////////////////

#include "XProject.h"

///////////////////////////////////////////////////////////
// WinMain
///////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst, LPSTR lpszCmdLine, int nCmdShow)
{
	OptionsCFG(false); // загрузим опции

	ResX=GetSystemMetrics(SM_CXSCREEN);
	ResY=GetSystemMetrics(SM_CYSCREEN);

	if(fullscreen)
		Window.Create("Fallout: The X-Project",hCurrentInst,WndProc,WS_POPUP,0,0,GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	else
		Window.Create("Fallout: The X-Project",hCurrentInst,WndProc,WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX,(ResX - 806) / 2,(ResY - 632) / 2, 808, 638);

	Window.SetIcon(IDI_FXP);

	hWindow = Window.hWnd;

	Screen.Create();

	if(fullscreen)
	{
		if(Screen.CheckDisplayMode(800,600,32))
			Screen.SetFullScreen(NDXDF_SYSMEMBACK,Window.hWnd,800,600,32);
		else if(Screen.CheckDisplayMode(800,600,16))
			Screen.SetFullScreen(NDXDF_SYSMEMBACK,Window.hWnd,800,600,16);
//		ShowCursor(false);
	}
	else
	{
		Screen.SetWindowed(NDXDF_SYSMEMBACK,Window.hWnd,806,632);
		GetClientRect(Window.hWnd,&wrect);
	}

	ShowCursor(false);		

	// если нет поддержки ММХ, то завершение работы
	if(NDX_TestMMX()) NDX_UseMMX = true;
		/*else PostMessage(hWindow, WM_CLOSE, 0, 0L);*/

	InitGame();

	while(Window.HandleMessages())
	{
		if(wActive)
		{
			// функция игрового цикла
			Game();
		}
	}
	
/*	if(fullscreen)
		Screen.ChangeRes(ResX, ResY);*/

	PostQuitMessage(0);
	return 0;
}

///////////////////////////////////////////////////////////
// WndProc()
//
// This is the main window procedure, which is called by
// Windows.
///////////////////////////////////////////////////////////
LRESULT APIENTRY WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{
    // Handle the messages to which the application
    // must respond.
	Window.HandleMessage(message,wParam,lParam);
    switch(message)
    {
	    case WM_KEYDOWN:
            OnKeyDown(wParam);
            return 0;

		case WM_MOUSEMOVE:
			if(!fullscreen)
			{
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(TRACKMOUSEEVENT);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = Window.hWnd;
				::_TrackMouseEvent(&tme);

				if(showcursor)
				{
					showcursor=false;
					ShowCursor(false);
				}
			}
            MouseY=HIWORD(lParam);
			MouseX=LOWORD(lParam);
            return 0;
		case WM_MOUSELEAVE:
            ShowCursor(true);
			showcursor=true;
            return 0;
/*		case WM_MOUSEHOVER:
            ShowCursor(false);
            return 0;*/
		case WM_DESTROY:
            OnDestroy();
			PostQuitMessage(0);
            return 0;
		case WM_RBUTTONDOWN:
			MouseRB=true;
			MouseRBClick=true;
			return 0;
		case WM_RBUTTONUP:
			MouseRB=false;
			return 0;
		case WM_LBUTTONDOWN:
			MouseLB=true;
			MouseLBup=false;
			MouseLBClick=true;
			return 0;
		case WM_LBUTTONDBLCLK:
			MouseLB=true;
			MouseLBDClick=true;
			return 0;
		case WM_LBUTTONUP:
			MouseLB=false;
			MouseLBup=true;
//			drag=0;
			return 0;
		case WM_MOUSEWHEEL:
			OnMouseWheel(wParam, lParam);
			return 0;
		case WM_TIMER:
			OnTimer();
			return 0;
		case WM_ACTIVATEAPP:
			OnActivateApp(wParam);
			return 0;
/*		case WM_SETCURSOR:
			if(!fullscreen)
			{
				if (LOWORD(lParam) != HTCLIENT)				
					ShowCursor(true);
			}
			else
				ShowCursor(false);
			return 0;*/
    }

    // Make sure all unhandled messages get returned to Windows.
    return DefWindowProc(hWnd, message, wParam, lParam);

}

void InitGame()
{
	InitSound(hWindow);

	plr_Inv.ClearAll("сумка", "player_1", &Screen);

//	archiev.InitArchiev();

	srand((unsigned)time(0));
	
	OpenDataFile();

	CString st("SPLASH/splash_");
	char s[10];
    itoa(rand() % 8, s, 10);
	st+=s;
	st+=".bmp";

//	AfxMessageBox(st);

	LPSTR fn;
	fn = st.GetBuffer(st.GetLength());
	st.ReleaseBuffer();

	NDX_Surface temp;

	temp.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile(fn),true);
	temp.Draw(Screen.Back, 0, 0);

	Screen.Flip();

	Splash.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("MENU/sp5.bmp"),true);
	
	Cursor.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("CURSOR/CURSOR_1.bmp"),true);
	Cursor.SetColorKey(Screen.PixelFormat->Rgb(0,0,0));
	Cursor.SetTransP(true);

	ICursor.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("CURSOR/CURSOR_3.bmp"),true);
	ICursor.SetColorKey(Screen.PixelFormat->Rgb(255,0,255));
	ICursor.SetTransP(true);

	Buttons.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("MENU/b_but.bmp"),true);
	Buttons.SetColorKey(Screen.PixelFormat->Rgb(64,0,255));
	Buttons.SetTransP(true);

	Pip_mnu.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("INGAME/pip_but.bmp"),true);
	Pip_mnu.SetColorKey(Screen.PixelFormat->Rgb(0,0,255));
	Pip_mnu.SetTransP(true);

	Puncts.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("MISC/puncts.bmp"),true);
	Puncts.SetColorKey(Screen.PixelFormat->Rgb(0,0,255));
	Puncts.SetTransP(true);

	Pip_srl.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("INGAME/scroll.bmp"),true);
	Pip_srl.SetColorKey(Screen.PixelFormat->Rgb(0,0,0));
	Pip_srl.SetTransP(true);

	Misc.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("INGAME/p_numbers1.bmp"),true);
	Misc.SetColorKey(Screen.PixelFormat->Rgb(0,0,0));
	Misc.SetTransP(true);

	Misc_but.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("MISC/yn.bmp"),true);
	Misc_but.SetColorKey(Screen.PixelFormat->Rgb(0,0,255));
	Misc_but.SetTransP(true);

	Noise.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("MISC/noise.bmp"),true);
	Noise.SetColorKey(Screen.PixelFormat->Rgb(253,253,253));
	Noise.SetTransP(true);

	TargetMsc.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("INGAME/t_misc.bmp"),true);
	TargetMsc.SetColorKey(Screen.PixelFormat->Rgb(0,0,255));
	TargetMsc.SetTransP(true);

	TargetRdr.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("INGAME/t_radar.bmp"),true);
	TargetRdr.SetColorKey(Screen.PixelFormat->Rgb(255,0,255));
	TargetRdr.SetTransP(true);

	Saveload.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("INGAME/saveload.bmp"),true);
	Saveload.SetColorKey(Screen.PixelFormat->Rgb(255,0,0));
	Saveload.SetTransP(true);

	MoveM.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("INGAME/movemulty.bmp"),true);
	MoveM.SetColorKey(Screen.PixelFormat->Rgb(255,0,255));
	MoveM.SetTransP(true);

	Messages.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("INGAME/messages.bmp"),true);
	
	Inventory.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("INGAME/inv.bmp"),true);
	
	InvEquip.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("INGAME/inventory.bmp"),true);
	InvEquip.SetColorKey(Screen.PixelFormat->Rgb(255,0,0));
	InvEquip.SetTransP(true);

	Barter.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("INGAME/barter.bmp"),true);
	Barter.SetColorKey(Screen.PixelFormat->Rgb(255,0,255));
	Barter.SetTransP(true);

	Safe.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("INGAME/safe.bmp"),true);
	Safe.SetColorKey(Screen.PixelFormat->Rgb(255,0,255));
	Safe.SetTransP(true);

	Main.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("INGAME/pipboy.bmp"),true);
	Image.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("INGAME/image_1.bmp"),true);
	Mnu_panel.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("MENU/panel.bmp"),true);
	Error.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("MISC/error.bmp"),true);
	TargetFrm.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("INGAME/target.bmp"),true);
	
	Stats.Create(&Screen, NDXST_SYSMEM, 80, 99);
	Stats.Clear(Screen.PixelFormat->Rgb(0,0,0));
	Stats.SetColorKey(Screen.PixelFormat->Rgb(0,0,0));
	Stats.SetTransP(true);

	Cpy_info.Create(&Screen, NDXST_SYSMEM, 800, 20);
	Cpy_info.Clear(Screen.PixelFormat->Rgb(0,0,0));
	Cpy_info.SetColorKey(Screen.PixelFormat->Rgb(0,0,0));
	Cpy_info.SetTransP(true);

	HFONT fnt=CreateFont(14, 0, 0, 0, FW_NORMAL,
		FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, VARIABLE_PITCH, "Arial Cyr");

	Cpy_info.FormattedText(100,0,RGB(207, 191, 63),fnt, vinfo);
	Cpy_info.FormattedText(460,0,RGB(207, 191, 63),fnt, cinfo);

//	NDX_DeleteFont(fnt);

/*	AddFontResource("font1.ttf");
	
	// Create the AAFont object.
	AFont.Create("DS Army Cyr",8,15);
	AFont.Save("font_b.btf");

	RemoveFontResource("font1.ttf");*/

	AFont.Load(MD.GetFile("INGAME/font_b.btf"));
	AAFont.Load(MD.GetFile("INGAME/font.bft"));

	OpenMStream("data\\sound\\music\\menu.acm");
	OpenClickStream();
	OpenNextSStream();
	SetMVolume(1);
	
/*	CString str;
	int r = toascii('#');
	char num[10];
	itoa(r, num, 10);
	str=num;
	AfxMessageBox(str);*/

	//Переменные

	MouseLB=false;
	MouseRB=false;
	MouseLBup=false;
	MouseLBClick=false;
	MouseLBDClick=false;
	MouseRBClick=false;

	fade=true;

	SetTimer(hWindow, 1, 150, 0);

	InitScriptSystem();	

	pFight = new CFight(0);

	fnt1=CreateFont(14, 0, 0, 0, FW_BOLD,
		FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, VARIABLE_PITCH, "Arial Cyr");

	fnt2=CreateFont(12, 0, 0, 0, FW_NORMAL,
		FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, VARIABLE_PITCH, "Arial Cyr");

//	MD.ExtractFile_ToFile("VIDEO/intro.wmv","temp");
	MD.Close();

//##############################################################################
//##############################################################################
//##############################################################################
//
	Sleep(3400); //задержка для показа слэша UNCOMENT THIS IN RELEASE VERSION!!!
//
//##############################################################################
//##############################################################################
//##############################################################################


/*	Video.OpenMedia(&Screen,"temp");
	Screen.Back->Clear(RGB(0,0,0));
	mode=-3;
	Video.Start();*/
	mode=0;
}

void InitGameValues()
{
	// инициализация архивных записей
	RealiseQData();
	RealiseQuest();
	
	// инициализируем главного героя
	plr_Inv.ClearAll("сумка", "player_1", &Screen);

	pFight->player_maxHP=45;
	ModifyData("PHealth", pFight->player_maxHP);
	pFight->player_AC = 0;
	pFight->player_AP = 9;
	ModifyData("PAgility", pFight->player_AP);
	pFight->player_DMGfrom=1;
	pFight->player_SDMGfrom=1;
	ModifyData("PMDMGf", pFight->player_DMGfrom);
	pFight->player_DMGto=Rand(4,5);
	pFight->player_SDMGto=pFight->player_DMGto;
	ModifyData("PMDMGt", pFight->player_DMGto);
	pFight->player_DMGplus=Rand(2,3);
	pFight->player_SDMGplus=pFight->player_DMGplus;
	ModifyData("PMDMGp", pFight->player_DMGplus);
	pFight->player_SKILL=Rand(80,90);
	ModifyData("PMSkill", pFight->player_SKILL);
//	ModifyData("Inventory", 1);

	// инициализируем оружие. по умолчанию в руках ничего нет
	pFight->Weapon[0].player_weapon="";
	pFight->Weapon[0].weapon_ammo=0;
	pFight->Weapon[0].weapon_cmagazine=0;
	pFight->Weapon[0].weapon_distance=1;
	pFight->Weapon[0].weapon_DMGfrom=0;
	pFight->Weapon[0].weapon_DMGplus=0;
	pFight->Weapon[0].weapon_DMGto=0;
	pFight->Weapon[0].weapon_magazine=0;
	pFight->Weapon[0].weapon_type=1;

	for(int i=0; i<5; ++i)
		amount[i]=0;
}

void Game()
{
	CalcCursor();

	if(mode==0)
	{
		PlayMStream();
		mode=1;		
	}
	else if(mode==1) // меню
	{
		MainMenu();
	}
	else if(mode==2) // режим игры
	{
		GameCircle();
	}
	else if(mode==3) // режим архива
	{
		GameCircle();
	}
	else if(mode==4) // режим инвентаря
	{
		ShowInventory();
	}
	else if(mode==5) // режим бартера
	{
		ShowBarter();
	}
	else if(mode==-1)
	{
		// покажем титры
		Titles();
	}
	else if(mode==-2)
	{
		// загрузка / сохранение
		Save_load();
	}
	else if(mode==-3)
	{
		// проиграем-ка видео
		PlayIntro();
	}

//	Screen.WaitVBlank();
//	Screen.ShowFPS(0, 580);

	// переключение страниц
	Screen.Flip();
		
	ManageMessages();	
}

///////////////////////////////////////////////////////////
// OnKeyDown()
//
// This function handles key presses received by the
// program in the form of WM_KEYDOWN messages.
///////////////////////////////////////////////////////////
void OnKeyDown(WPARAM wParam)
{
    if (wParam == VK_ESCAPE)
    {
		if(mode==-1)
		{
			OpenDataFile();

			Splash.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("MENU/sp5.bmp"),true);
			StopMStream();
			NDX_Surface			Fon; //фон
			Fon.Create(&Screen,NDXST_SYSMEM,800, 600);
			
			Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));

			MD.Close();
			
			for(int i=17; i>0; --i)
			{
				Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
				Screen.Flip();
				Sleep(60);
			}
			OpenMStream("data\\sound\\music\\menu.acm");
			SetMVolume(1);
			PlayMStream();
			fade=true;
			mode=0;
		}
		else if(mode==2)
		{
			if((!fight)&&(!codecrack))
			{
				if(!save)
					save=true;
				else
					save=false;
			}
		}
		else if(mode==3)
		{
			if((!fight)&&(!codecrack))
			{
				if(!save)
					save=true;
				else
					save=false;
			}
		}
		else if(mode==4)
		{
			mode=last_mode;
			inv_mode=0;
			drag=0;
		}
		else if(mode==5)
		{
			mode=last_mode;
			inv_mode=0;
			drag=0;
		}
		else if(mode==-2)
		{
			NDX_Surface			Fon; //фон
			Fon.Create(&Screen,NDXST_SYSMEM,800, 600);
			
			Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
			
			for(int i=17; i>0; --i)
			{
				Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
				Screen.Flip();
				Sleep(30);
			}
			fade=true;

			save=false;
			mode=last_mode;
		}
		else if(mode==-3)
		{
			Video.Stop();
			Video.CloseMedia();
			DeleteFile("temp");
			mode=0;
		}
		else
		{
			Sleep(200);
			prog_exit=true;
			//PostMessage(hWindow, WM_CLOSE, 0, 0L);
		}		
    }
	else if(wParam == VK_F5)
	{
	}
	else if((wParam == 0xC0)&&(mode==3))
	{
		// включим режим читов
		if(!cheats)
		{
			cheats=true;
			cheat = "";
		}
		else
		{
			if(cheat="artmoney")
				plr_Inv.AddItem(1000, "money", &Screen);
			cheat="";
			cheats=false;
		}
	}
	else if(wParam == VK_F6)
	{
/*		InitGameValues();
		Sleep(200);
		NDX_Surface			Fon; //фон
		Fon.Create(&Screen,NDXST_SYSMEM,800, 600);
		
		Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
		
		for(int i=17; i>0; --i)
		{
			Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
			Screen.Flip();
			Sleep(30);
		}
		fade=true;
		StopMStream(); 
		OpenMStream("data\\sound\\music\\15shady.acm");
		LoadScenario();
		PrepareFrame(0);
		PlayMStream();
		mode=2;*/
	}
	else if(wParam == VK_DELETE)
	{
		if(mode==-2)
		{
			CString fname="data\\SAVEGAME\\save";
			char num[10];
			itoa(anim, num, 10);
			fname+=num;
			fname+=".sav";
			DeleteFile(fname);
			PrepareSaveload(anim);
		}
	}
	else if(wParam == VK_RETURN)
	{
		if((fight)&&(mode==2))
		{
			Sleep(200);
			OpenDataFile();
			Cursor.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("CURSOR/CURSOR_2.bmp"),true);
			Cursor.SetColorKey(Screen.PixelFormat->Rgb(255,255,0));
			Cursor.SetTransP(true);
			MD.Close();
			EndTurn();
		}

	}
	else if((wParam == 0x31)||(wParam == VK_NUMPAD1)) // 1
	{
		if((mode==2)&&(!fight)&&(!save))
		{
			if(CheckCondition(Level[fr].condition1))
			{
				CheckScript(Level[fr].script1);
				if(!changes)
					PrepareFrame(Level[fr].url1_id);
				Sleep(200);
			}
		}
		else if((mode==5)&&(inv_mode==1))
		{
			amount[0]=amount[1];
			amount[1]=amount[2];
			amount[2]=amount[3];
			amount[3]=amount[4];
			amount[4]=1;
		}
	}
	else if((wParam == 0x32)||(wParam == VK_NUMPAD2)) // 2
	{
		if((mode==2)&&(!fight)&&(!save))
		{
			if(CheckCondition(Level[fr].condition2))
			{
				CheckScript(Level[fr].script2);
				if(!changes)
					PrepareFrame(Level[fr].url2_id);
				Sleep(200);
			}
		}
		else if((mode==5)&&(inv_mode==1))
		{
			amount[0]=amount[1];
			amount[1]=amount[2];
			amount[2]=amount[3];
			amount[3]=amount[4];
			amount[4]=2;
		}
	}
	else if((wParam == 0x33)||(wParam == VK_NUMPAD3)) // 3
	{
		if((mode==2)&&(!fight)&&(!save))
		{
			if(CheckCondition(Level[fr].condition3))
			{
				CheckScript(Level[fr].script3);
				if(!changes)
					PrepareFrame(Level[fr].url3_id);
				Sleep(200);
			}
		}
		else if((mode==5)&&(inv_mode==1))
		{
			amount[0]=amount[1];
			amount[1]=amount[2];
			amount[2]=amount[3];
			amount[3]=amount[4];
			amount[4]=3;
		}
	}
	else if((wParam == 0x34)||(wParam == VK_NUMPAD4)) // 4
	{
		if((mode==2)&&(!fight)&&(!save))
		{
			if(CheckCondition(Level[fr].condition4))
			{
				CheckScript(Level[fr].script4);
				if(!changes)
					PrepareFrame(Level[fr].url4_id);
				Sleep(200);
			}
		}
		else if((mode==5)&&(inv_mode==1))
		{
			amount[0]=amount[1];
			amount[1]=amount[2];
			amount[2]=amount[3];
			amount[3]=amount[4];
			amount[4]=4;
		}
	}
	else if((wParam == 0x35)||(wParam == VK_NUMPAD5)) // 5
	{
		if((mode==2)&&(!fight)&&(!save))
		{
			if(CheckCondition(Level[fr].condition5))
			{
				CheckScript(Level[fr].script5);
				if(!changes)
					PrepareFrame(Level[fr].url5_id);
				Sleep(200);
			}
		}
		else if((mode==5)&&(inv_mode==1))
		{
			amount[0]=amount[1];
			amount[1]=amount[2];
			amount[2]=amount[3];
			amount[3]=amount[4];
			amount[4]=5;
		}
	}
	else if((wParam == 0x36)||(wParam == VK_NUMPAD6)) // 6
	{
		if((mode==5)&&(inv_mode==1))
		{
			amount[0]=amount[1];
			amount[1]=amount[2];
			amount[2]=amount[3];
			amount[3]=amount[4];
			amount[4]=6;
		}
	}
	else if((wParam == 0x37)||(wParam == VK_NUMPAD7)) // 7
	{
		if((mode==5)&&(inv_mode==1))
		{
			amount[0]=amount[1];
			amount[1]=amount[2];
			amount[2]=amount[3];
			amount[3]=amount[4];
			amount[4]=7;
		}
	}
	else if((wParam == 0x38)||(wParam == VK_NUMPAD8)) // 8
	{
		if((mode==5)&&(inv_mode==1))
		{
			amount[0]=amount[1];
			amount[1]=amount[2];
			amount[2]=amount[3];
			amount[3]=amount[4];
			amount[4]=8;
		}
	}
	else if((wParam == 0x39)||(wParam == VK_NUMPAD9)) // 9
	{
		if((mode==5)&&(inv_mode==1))
		{
			amount[0]=amount[1];
			amount[1]=amount[2];
			amount[2]=amount[3];
			amount[3]=amount[4];
			amount[4]=9;
		}
	}
	else if((wParam == 0x30)||(wParam == VK_NUMPAD0)) // 0
	{
		if((mode==5)&&(inv_mode==1))
		{
			amount[0]=amount[1];
			amount[1]=amount[2];
			amount[2]=amount[3];
			amount[3]=amount[4];
			amount[4]=0;
		}
	}
	else if(wParam == VK_BACK) // Backspace
	{
		if((mode==5)&&(inv_mode==1))
		{
			amount[4]=amount[3];
			amount[3]=amount[2];
			amount[2]=amount[1];
			amount[1]=amount[0];
			amount[0]=0;
		}
	}
	else if(wParam == 0x41) //A
	{
		if((mode==3)&&(cheats))
			cheat+="a";
		if((GetData("WASD") == 1)&&(mode==2)&&(!fight)&&(!save))
		{
			CheckScript(Level[fr].script3);
			if(!changes)
				PrepareFrame(Level[fr].url3_id);
			Sleep(200);
		}
	}
	else if(wParam == 0x57) // W
	{
		if((GetData("WASD") == 1)&&(mode==2)&&(!fight)&&(!save))
		{
			if(CheckCondition(Level[fr].condition2))
			{
				CheckScript(Level[fr].script2);
				if(!changes)
					PrepareFrame(Level[fr].url2_id);
				Sleep(200);
			}
		}
	}
	else if(wParam == 0x44) // D
	{
		if((GetData("WASD") == 1)&&(mode==2)&&(!fight)&&(!save))
		{
			if(CheckCondition(Level[fr].condition1))
			{
				CheckScript(Level[fr].script1);
				if(!changes)
					PrepareFrame(Level[fr].url1_id);
				Sleep(200);
			}
		}
	}
	else if(wParam == 0x53) // S
	{
		if((GetData("WASD") == 1)&&(mode==2)&&(!fight)&&(!save))
		{
			if(CheckCondition(Level[fr].condition4))
			{
				CheckScript(Level[fr].script4);
				if(!changes)
					PrepareFrame(Level[fr].url4_id);
				Sleep(200);
			}
		}
	}
	else if(wParam == 0x45) //E
	{
		if((mode==3)&&(cheats))
			cheat+="e";
	}
	else if(wParam == 0x4D) //M
	{
		if((mode==3)&&(cheats))
			cheat+="m";
	}
	else if(wParam == 0x4E) //N
	{
		if((mode==3)&&(cheats))
			cheat+="n";
	}
	else if(wParam == 0x4F) //O
	{
		if((mode==3)&&(cheats))
			cheat+="o";
	}
	else if(wParam == 0x52) //R
	{
		if((mode==3)&&(cheats))
			cheat+="r";
	}
	else if(wParam == 0x54) //T
	{
		if((mode==3)&&(cheats))
			cheat+="t";
	}
	else if(wParam == 0x59) //Y
	{
		if((mode==3)&&(cheats))
			cheat+="y";
	}
	else if(wParam == 0x49) // I, Inventory
	{
		if(GetData("Inventory") == 1)
		{
			if(mode==2)
			{
				Click();
				Sleep(200);
				if(fight)
				{
					if(pFight->CanDo(3))
					{
						pFight->player_cAP -= 3;
						plr_Inv.InitInventory();
						inv_mode=3;
						last_mode=mode;
						mode=4;
					}
					else
					{
						OpenSStream("deny.wav");
						battlelog.Insert(0, "Недостаточно очков действия. \n");
												
						WriteFLogText();
//						PrepareFightFrm();
					}					
				}
				else
				{
					plr_Inv.InitInventory();
					inv_mode=3;
					last_mode=mode;
					mode=4;
				}
			}
			else if(mode==4)
			{
				Click();
				Sleep(200);
				inv_mode=0;
				mode=last_mode;
				cursor_mode=0;
			}
		}
	}
	else if(wParam == VK_SPACE)
	{
		if((GetData("WASD") == 1)&&(mode==2)&&(!fight)&&(!save))
		{
			if(CheckCondition(Level[fr].condition5))
			{
				CheckScript(Level[fr].script5);
				if(!changes)
					PrepareFrame(Level[fr].url5_id);
				Sleep(200);
			}
		}
	}
}

void OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	MouseWUp = FALSE;
	MouseWDown = FALSE;

	if(mode==2)
	{
		int zDelta = (int)wParam;
		if(zDelta>0)
		{
			//скролируем текст вверх
			MouseWUp=true;
		}
		else if(zDelta<0)
		{
			//скролируем текст вниз
			MouseWDown=true;
		}
	}
}

bool HandleMessages()
{
	MSG msg;
	if(!IsWindow(hWindow)||hWindow==NULL)return false;
	if(PeekMessage(&msg,hWindow,0,0,PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return true;
}

void CalcCursor()
{
	IEvent[0].Object=0;
	IEvent[0].Conditional=0;
	IEvent[1].Object=0;
	IEvent[1].Conditional=0;
	if(mode==1)
	{
		if(prog_exit)
		{
			if(MouseLB==true)
			{
				if((MouseX>=258)&&(MouseX<=272)&&(MouseY>=364)&&(MouseY<=378))
				{
					IEvent[0].Object=10;
					IEvent[0].Conditional=1;
				}
				else if((MouseX>=433)&&(MouseX<=447)&&(MouseY>=364)&&(MouseY<=378))
				{
					IEvent[0].Object=11;
					IEvent[0].Conditional=1;
				}
			}
		}
		else
		{
			if(MouseLB==true)
			{
				if((MouseX>=528)&&(MouseX<=550)&&(MouseY>=243)&&(MouseY<=265))
				{
					IEvent[0].Object=2;
					IEvent[0].Conditional=1;
				}
				else if((MouseX>=528)&&(MouseX<=550)&&(MouseY>=308)&&(MouseY<=330))
				{
					IEvent[0].Object=3;
					IEvent[0].Conditional=1;
				}
				else if((MouseX>=528)&&(MouseX<=550)&&(MouseY>=373)&&(MouseY<=395))
				{
					IEvent[0].Object=4;
					IEvent[0].Conditional=1;
				}
				else if((MouseX>=528)&&(MouseX<=550)&&(MouseY>=438)&&(MouseY<=460))
				{
					IEvent[0].Object=5;
					IEvent[0].Conditional=1;
				}
			}			
		}
	}
	else if(mode==2)
	{
		if(prog_exit)
		{
			if(MouseLB==true)
			{
				if((MouseX>=258)&&(MouseX<=272)&&(MouseY>=364)&&(MouseY<=378))
				{
					IEvent[0].Object=10;
					IEvent[0].Conditional=1;
				}
				else if((MouseX>=433)&&(MouseX<=447)&&(MouseY>=364)&&(MouseY<=378))
				{
					IEvent[0].Object=11;
					IEvent[0].Conditional=1;
				}

				MouseLB=false;
			}
		}
		else if(prog_error)
		{
			if(MouseLB==true)
			{
				if((MouseX>=352)&&(MouseX<=366)&&(MouseY>=364)&&(MouseY<=378))
				{
					IEvent[0].Object=10;
					IEvent[0].Conditional=1;
				}

				MouseLB=false;
			}
		}
		else if(fight)
		{
			if((MouseX>=272)&&(MouseX<=347)&&(MouseY>=188)&&(MouseY<=202))
			{
				IEvent[0].Object=20;
				if(MouseLB==true)
				{
					IEvent[0].Conditional=2;
					MouseLB=false;
				}
				else
					IEvent[0].Conditional=1;
			}
			else if((MouseX>=272)&&(MouseX<=347)&&(MouseY>=252)&&(MouseY<=266))
			{
				IEvent[0].Object=23;
				if(MouseLB==true)
				{
					IEvent[0].Conditional=2;
					MouseLB=false;
				}
				else
					IEvent[0].Conditional=1;
			}
			else if((MouseX>=272)&&(MouseX<=347)&&(MouseY>=317)&&(MouseY<=331))
			{
				IEvent[0].Object=26;
				if(MouseLB==true)
				{
					IEvent[0].Conditional=2;
					MouseLB=false;
				}
				else
					IEvent[0].Conditional=1;
			}
			else if((MouseX>=272)&&(MouseX<=347)&&(MouseY>=380)&&(MouseY<=394))
			{
				IEvent[0].Object=25;
				if(MouseLB==true)
				{
					IEvent[0].Conditional=2;
					MouseLB=false;
				}
				else
					IEvent[0].Conditional=1;
			}
			else if((MouseX>=546)&&(MouseX<=621)&&(MouseY>=188)&&(MouseY<=202))
			{
				IEvent[0].Object=21;
				if(MouseLB==true)
				{
					IEvent[0].Conditional=2;
					MouseLB=false;
				}
				else
					IEvent[0].Conditional=1;
			}
			else if((MouseX>=546)&&(MouseX<=621)&&(MouseY>=252)&&(MouseY<=266))
			{
				IEvent[0].Object=22;
				if(MouseLB==true)
				{
					IEvent[0].Conditional=2;
					MouseLB=false;
				}
				else
					IEvent[0].Conditional=1;
			}
			else if((MouseX>=546)&&(MouseX<=621)&&(MouseY>=317)&&(MouseY<=331))
			{
				IEvent[0].Object=24;
				if(MouseLB==true)
				{
					IEvent[0].Conditional=2;
					MouseLB=false;
				}
				else
					IEvent[0].Conditional=1;
			}
			else if((MouseX>=546)&&(MouseX<=621)&&(MouseY>=380)&&(MouseY<=394))
			{
				IEvent[0].Object=27;
				if(MouseLB==true)
				{
					IEvent[0].Conditional=2;
					MouseLB=false;
				}
				else
					IEvent[0].Conditional=1;
			}
			else if((MouseX>=699)&&(MouseX<=731)&&(MouseY>=168)&&(MouseY<=250)&&(fight))
			{
				if((!pFight->Enemy[0].dead)&&(cur_target!=0))
				{
					if(MouseLB==true)
					{
						Click();
						cur_target=0;
						MouseLB=false;
					}
				}
			}
			else if((MouseX>=699)&&(MouseX<=731)&&(MouseY>=260)&&(MouseY<=344)&&(fight))
			{
				if((!pFight->Enemy[1].dead)&&(cur_target!=1))
				{
					if(MouseLB==true)
					{
						Click();
						cur_target=1;
						MouseLB=false;
					}
				}
			}
			else if((MouseX>=699)&&(MouseX<=731)&&(MouseY>=352)&&(MouseY<=434)&&(fight))
			{
				if((!pFight->Enemy[2].dead)&&(cur_target!=2))
				{
					if(MouseLB==true)
					{
						Click();
						cur_target=2;
						MouseLB=false;
					}
				}
			}
			else if((MouseX>=686)&&(MouseX<=773)&&(MouseY>=501)&&(MouseY<=536))
			{
				IEvent[0].Object=28; // конец хода
				if(MouseLB==true)
				{
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
				
			}
			else if((MouseX>=686)&&(MouseX<=773)&&(MouseY>=540)&&(MouseY<=571)&&(GetData("Inventory")==1))
			{
				// нажали кныпку инвентаря
				if(MouseLB==true)
				{
					IEvent[0].Object=30;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=615)&&(MouseX<=625)&&(MouseY>=505)&&(MouseY<=515))
			{
				//скролируем текст ответов вверх
				IEvent[0].Object=3;
				IEvent[0].Conditional=1;
			}
			else if((MouseX>=615)&&(MouseX<=625)&&(MouseY>=550)&&(MouseY<=560))
			{
				//скролируем текст ответов вниз
				IEvent[0].Object=4;
				IEvent[0].Conditional=1;
			}
			else if((MouseX>=nexttext_x)&&(MouseX<=nexttext_x+nexttext_width)&&(MouseY>=nexttext_y)&&(MouseY<=nexttext_y+nexttext_height))
			{
				if(MouseWUp==true)
				{
					//скролируем текст ответов вверх
					IEvent[0].Object=3;
					IEvent[0].Conditional=2;
				}
				else if(MouseWDown==true)
				{
					//скролируем текст ответов вниз
					IEvent[0].Object=4;
					IEvent[0].Conditional=2;
				}
			}
			
		}
		else if(save)
		{
			if(MouseLB==true)
			{
				if((MouseX>=298)&&(MouseX<=320)&&(MouseY>=190)&&(MouseY<=212))
				{
					IEvent[0].Object=1;
					IEvent[0].Conditional=1;
				}
				else if((MouseX>=298)&&(MouseX<=320)&&(MouseY>=255)&&(MouseY<=277))
				{
					IEvent[0].Object=2;
					IEvent[0].Conditional=1;
				}
				else if((MouseX>=298)&&(MouseX<=320)&&(MouseY>=320)&&(MouseY<=342))
				{
					IEvent[0].Object=3;
					IEvent[0].Conditional=1;
				}
				else if((MouseX>=298)&&(MouseX<=320)&&(MouseY>=385)&&(MouseY<=407))
				{
					IEvent[0].Object=4;
					IEvent[0].Conditional=1;
				}

				MouseLB=false;
			}
		}
		else if(codecrack)
		{
			if((MouseX>=759)&&(MouseX<=774)&&(MouseY>=9)&&(MouseY<=20))
			{
				// нажали кнопку mute
				if(MouseLB==true)
				{
					if(mute)
						mute=false;
					else
						mute=true;

					MuteSound(mute);
					MouseLB=false;
				}
			}
			else if((MouseX>=257)&&(MouseX<=357)&&(MouseY>=5)&&(MouseY<=24))
			{
				// изменили яркость
				lightning_bar=35;
				if(MouseLB==true)
				{
					int value = MouseX - 257;
					if(value<0)
						value=0;
					else if(value>100)
						value=100;
					tbright = 100 - value;
				}
			}
			else if((MouseX>=655)&&(MouseX<=755)&&(MouseY>=5)&&(MouseY<=24))
			{
				// изменили громкость
				lightning_bar=35;
				if(MouseLB==true)
				{
					int value = MouseX - 655;
					if(value<0)
						value=0;
					else if(value>100)
						value=100;
					svolume = value;
					SetVolume(svolume);
				}
			}
			else if((MouseX>=239)&&(MouseX<=253)&&(MouseY>=10)&&(MouseY<=19))
			{
				// яркость текста на максимум
				if(MouseLB==true)
				{
					tbright=100;
					MouseLB=false;
				}
			}
			else if((MouseX>=259)&&(MouseX<=272)&&(MouseY>=309)&&(MouseY<=317))
			{
				// подтверждение цифры 1
				if(MouseLB==true)
				{
					IEvent[0].Object=50;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=301)&&(MouseX<=314)&&(MouseY>=309)&&(MouseY<=317))
			{
				// подтверждение цифры 2
				if(MouseLB==true)
				{
					IEvent[0].Object=51;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=343)&&(MouseX<=356)&&(MouseY>=309)&&(MouseY<=317))
			{
				// подтверждение цифры 3
				if(MouseLB==true)
				{
					IEvent[0].Object=52;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=385)&&(MouseX<=398)&&(MouseY>=309)&&(MouseY<=317))
			{
				// подтверждение цифры 4
				if(MouseLB==true)
				{
					IEvent[0].Object=53;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=427)&&(MouseX<=440)&&(MouseY>=309)&&(MouseY<=317))
			{
				// подтверждение цифры 5
				if(MouseLB==true)
				{
					IEvent[0].Object=54;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=469)&&(MouseX<=492)&&(MouseY>=309)&&(MouseY<=317))
			{
				// подтверждение цифры 6
				if(MouseLB==true)
				{
					IEvent[0].Object=55;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=511)&&(MouseX<=524)&&(MouseY>=309)&&(MouseY<=317))
			{
				// подтверждение цифры 7
				if(MouseLB==true)
				{
					IEvent[0].Object=56;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=278)&&(MouseX<=292)&&(MouseY>=283)&&(MouseY<=293))
			{
				// + цифры 1
				if(MouseLB==true)
				{
					IEvent[0].Object=57;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=278)&&(MouseX<=292)&&(MouseY>=295)&&(MouseY<=305))
			{
				// - цифры 1
				if(MouseLB==true)
				{
					IEvent[0].Object=58;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=320)&&(MouseX<=334)&&(MouseY>=283)&&(MouseY<=293))
			{
				// + цифры 2
				if(MouseLB==true)
				{
					IEvent[0].Object=59;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=320)&&(MouseX<=334)&&(MouseY>=295)&&(MouseY<=305))
			{
				// - цифры 2
				if(MouseLB==true)
				{
					IEvent[0].Object=60;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=362)&&(MouseX<=376)&&(MouseY>=283)&&(MouseY<=293))
			{
				// + цифры 3
				if(MouseLB==true)
				{
					IEvent[0].Object=61;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=362)&&(MouseX<=376)&&(MouseY>=295)&&(MouseY<=305))
			{
				// + цифры 3
				if(MouseLB==true)
				{
					IEvent[0].Object=62;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=404)&&(MouseX<=418)&&(MouseY>=283)&&(MouseY<=293))
			{
				// + цифры 4
				if(MouseLB==true)
				{
					IEvent[0].Object=63;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=404)&&(MouseX<=418)&&(MouseY>=295)&&(MouseY<=305))
			{
				// - цифры 4
				if(MouseLB==true)
				{
					IEvent[0].Object=64;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=446)&&(MouseX<=460)&&(MouseY>=283)&&(MouseY<=293))
			{
				// + цифры 5
				if(MouseLB==true)
				{
					IEvent[0].Object=65;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=446)&&(MouseX<=460)&&(MouseY>=295)&&(MouseY<=305))
			{
				// - цифры 5
				if(MouseLB==true)
				{
					IEvent[0].Object=66;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=488)&&(MouseX<=502)&&(MouseY>=283)&&(MouseY<=293))
			{
				// + цифры 6
				if(MouseLB==true)
				{
					IEvent[0].Object=67;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=488)&&(MouseX<=502)&&(MouseY>=295)&&(MouseY<=305))
			{
				// - цифры 6
				if(MouseLB==true)
				{
					IEvent[0].Object=68;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=530)&&(MouseX<=544)&&(MouseY>=283)&&(MouseY<=293))
			{
				// + цифры 7
				if(MouseLB==true)
				{
					IEvent[0].Object=69;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=530)&&(MouseX<=544)&&(MouseY>=295)&&(MouseY<=305))
			{
				// - цифры 7
				if(MouseLB==true)
				{
					IEvent[0].Object=70;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else
			{
				if(MouseLB)
					MouseLB=false;
			}
		}
		else
		{
			if((MouseX>=730)&&(MouseX<=740)&&(MouseY>=100)&&(MouseY<=110))
			{
				//скролируем текст вверх
				IEvent[0].Object=1;
				IEvent[0].Conditional=1;
			}
			else if((MouseX>=730)&&(MouseX<=740)&&(MouseY>=140)&&(MouseY<=150))
			{
				//скролируем текст вниз
				IEvent[0].Object=2;
				IEvent[0].Conditional=1;
			}
			else if((MouseX>=615)&&(MouseX<=625)&&(MouseY>=505)&&(MouseY<=515))
			{
				//скролируем текст ответов вверх
				IEvent[0].Object=3;
				IEvent[0].Conditional=1;
			}
			else if((MouseX>=615)&&(MouseX<=625)&&(MouseY>=550)&&(MouseY<=560))
			{
				//скролируем текст ответов вниз
				IEvent[0].Object=4;
				IEvent[0].Conditional=1;
			}
			else if((MouseX>=37)&&(MouseX<=51)&&(MouseY>=436)&&(MouseY<=450))
			{
				// нажали кнопку logoff
				if(MouseLB==true)
				{
					IEvent[0].Object=18;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=37)&&(MouseX<=51)&&(MouseY>=402)&&(MouseY<=416))
			{
				// нажали кнопку гл. меню
				if(MouseLB==true)
				{
					IEvent[0].Object=17;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=37)&&(MouseX<=51)&&(MouseY>=368)&&(MouseY<=382))
			{
				// нажали кнопку терминал
				if(MouseLB==true)
				{
					IEvent[0].Object=16;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=37)&&(MouseX<=51)&&(MouseY>=334)&&(MouseY<=348))
			{
				// нажали кнопку архив
				if(MouseLB==true)
				{
					IEvent[0].Object=15;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=maintext_x)&&(MouseX<=maintext_width+maintext_x)&&(MouseY>=maintext_y)&&(MouseY<=maintext_height+maintext_y)&&(MouseWUp==true))
			{
				//скролируем текст вверх
				IEvent[0].Object=1;
				IEvent[0].Conditional=2;
			}
			else if((MouseX>=maintext_x)&&(MouseX<=maintext_width+maintext_x)&&(MouseY>=maintext_y)&&(MouseY<=maintext_height+maintext_y)&&(MouseWDown==true))
			{
				//скролируем текст вниз
				IEvent[0].Object=2;
				IEvent[0].Conditional=2;
			}
			else if((MouseX>=nexttext_x)&&(MouseX<=nexttext_x+nexttext_width)&&(MouseY>=nexttext_y)&&(MouseY<=nexttext_y+nexttext_height))
			{
				int ar1;
				if(Interface[0].ar1==0)
					ar1=0;
				else
					ar1=1;
				int ar2;
				if(Interface[0].ar2==0)
					ar2=0;
				else
					ar2=1;
				int ar3;
				if(Interface[0].ar3==0)
					ar3=0;
				else
					ar3=1;
				int ar4;
				if(Interface[0].ar4==0)
					ar4=0;
				else
					ar4=1;

				if((MouseY>=nexttext_y)&&(MouseY<=nexttext_y+Interface[0].ar1*12-Interface[0].c_y))
				{
					if(Level[fr].url1_id!=fr)
					{
						IEvent[0].Object=5;
						if(MouseLB==true)
						{
							IEvent[0].Conditional=2;
							MouseLB=false;
						}
						else
							IEvent[0].Conditional=1;
					}
				}
				else if((MouseY>=nexttext_y+Interface[0].ar1*12+16*ar1-Interface[0].c_y)&&(MouseY<=nexttext_y+Interface[0].ar1*12+16*ar1+Interface[0].ar2*12-Interface[0].c_y))
				{
					if(Level[fr].url2_id!=fr)
					{
						IEvent[0].Object=6;
						if(MouseLB==true)
						{
							IEvent[0].Conditional=2;
							MouseLB=false;
						}
						else
							IEvent[0].Conditional=1;
					}
				}
				else if((MouseY>=nexttext_y+Interface[0].ar1*12+16*ar1+Interface[0].ar2*12+16*ar2-Interface[0].c_y)&&(MouseY<=nexttext_y+Interface[0].ar1*12+16*ar1+Interface[0].ar2*12+16*ar2+Interface[0].ar3*12-Interface[0].c_y))
				{
					if(Level[fr].url3_id!=fr)
					{
						IEvent[0].Object=7;
						if(MouseLB==true)
						{
							IEvent[0].Conditional=2;
							MouseLB=false;
						}
						else
							IEvent[0].Conditional=1;
					}
				}
				else if((MouseY>=nexttext_y+Interface[0].ar1*12+16*ar1+Interface[0].ar2*12+16*ar2+Interface[0].ar3*12+16*ar3-Interface[0].c_y)&&(MouseY<=nexttext_y+Interface[0].ar1*12+16*ar1+Interface[0].ar2*12+16*ar2+Interface[0].ar3*12+16*ar3+Interface[0].ar4*12-Interface[0].c_y))
				{
					if(Level[fr].url4_id!=fr)
					{
						IEvent[0].Object=8;
						if(MouseLB==true)
						{
							IEvent[0].Conditional=2;
							MouseLB=false;							
						}
						else
							IEvent[0].Conditional=1;
					}
				}
				else if((MouseY>=nexttext_y+Interface[0].ar1*12+16*ar1+Interface[0].ar2*12+16*ar2+Interface[0].ar3*12+16*ar3+Interface[0].ar4*12+16*ar4-Interface[0].c_y)&&(MouseY<=nexttext_y+Interface[0].ar1*12+16*ar1+Interface[0].ar2*12+16*ar2+Interface[0].ar3*12+16*ar3+Interface[0].ar4*12+16*ar4+Interface[0].ar5*12-Interface[0].c_y))
				{
					if(Level[fr].url5_id!=fr)
					{
						IEvent[0].Object=9;
						if(MouseLB==true)
						{
							IEvent[0].Conditional=2;
							MouseLB=false;
						}
						else
							IEvent[0].Conditional=1;
					}
				}
				
				if(MouseWUp==true)
				{
					//скролируем текст ответов вверх
					IEvent[0].Object=3;
					IEvent[0].Conditional=2;
				}
				else if(MouseWDown==true)
				{
					//скролируем текст ответов вниз
					IEvent[0].Object=4;
					IEvent[0].Conditional=2;
				}
			}
			else if((MouseX>=686)&&(MouseX<=773)&&(MouseY>=540)&&(MouseY<=571)&&(GetData("Inventory")==1))
			{
				if(MouseLB==true)
				{
					IEvent[0].Object=30;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=118)&&(MouseX<=143)&&(MouseY>=12)&&(MouseY<=34))
			{
				// нажали кнопку отдыха
				if(MouseLB==true)
				{
					IEvent[0].Object=35;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
			else if((MouseX>=759)&&(MouseX<=774)&&(MouseY>=9)&&(MouseY<=20))
			{
				// нажали кнопку mute
				if(MouseLB==true)
				{
					if(mute)
						mute=false;
					else
						mute=true;

					MuteSound(mute);
					MouseLB=false;
				}
			}
			else if((MouseX>=257)&&(MouseX<=357)&&(MouseY>=5)&&(MouseY<=24))
			{
				// изменили яркость
				lightning_bar=35;
				if(MouseLB==true)
				{
					int value = MouseX - 257;
					if(value<0)
						value=0;
					else if(value>100)
						value=100;
					tbright = 100 - value;
				}
			}
			else if((MouseX>=655)&&(MouseX<=755)&&(MouseY>=5)&&(MouseY<=24))
			{
				// изменили громкость
				lightning_bar=35;
				if(MouseLB==true)
				{
					int value = MouseX - 655;
					if(value<0)
						value=0;
					else if(value>100)
						value=100;
					svolume = value;
					SetVolume(svolume);
				}
			}
			else if((MouseX>=239)&&(MouseX<=253)&&(MouseY>=10)&&(MouseY<=19))
			{
				// яркость текста на максимум
				if(MouseLB==true)
				{
					tbright=100;
					MouseLB=false;
				}
			}
			else
			{
				if(MouseLB)
					MouseLB=false;
			}
		}
	}
	else if(mode==3)
	{
	/*	if((MouseX>=maintext_x)&&(MouseX<=maintext_x+maintext_width)&&(MouseY>=maintext_y)&&(MouseY<=maintext_y+maintext_height))
		{ // передаём информацию в управляющую функцию 
			archiev.Mouse(MouseX-maintext_x, MouseY-maintext_y, MouseLB, &Screen);
		}*/
		if(save)
		{
			if(MouseLB==true)
			{
				if((MouseX>=298)&&(MouseX<=320)&&(MouseY>=190)&&(MouseY<=212))
				{
					IEvent[0].Object=1;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
				else if((MouseX>=298)&&(MouseX<=320)&&(MouseY>=255)&&(MouseY<=277))
				{
					IEvent[0].Object=2;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
				else if((MouseX>=298)&&(MouseX<=320)&&(MouseY>=320)&&(MouseY<=342))
				{
					IEvent[0].Object=3;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
				else if((MouseX>=298)&&(MouseX<=320)&&(MouseY>=385)&&(MouseY<=407))
				{
					IEvent[0].Object=4;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
		}
		else 
		{
		if(prog_exit)
		{
			if(MouseLB==true)
			{
				if((MouseX>=258)&&(MouseX<=272)&&(MouseY>=364)&&(MouseY<=378))
				{
					IEvent[0].Object=10;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
				else if((MouseX>=433)&&(MouseX<=447)&&(MouseY>=364)&&(MouseY<=378))
				{
					IEvent[0].Object=11;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
		}
		else if(prog_error)
		{
			if(MouseLB==true)
			{
				if((MouseX>=352)&&(MouseX<=366)&&(MouseY>=364)&&(MouseY<=378))
				{
					IEvent[0].Object=10;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
		}
		else if((MouseX>=37)&&(MouseX<=51)&&(MouseY>=436)&&(MouseY<=450))
		{
			// нажали кнопку logoff
			if(MouseLB==true)
			{
				IEvent[0].Object=18;
				IEvent[0].Conditional=1;
				MouseLB=false;
			}
		}
		else if((MouseX>=37)&&(MouseX<=51)&&(MouseY>=402)&&(MouseY<=416))
		{
			// нажали кнопку гл. меню
			if(MouseLB==true)
			{
				IEvent[0].Object=17;
				IEvent[0].Conditional=1;
				MouseLB=false;
			}
		}
		else if((MouseX>=37)&&(MouseX<=51)&&(MouseY>=368)&&(MouseY<=382))
		{
			// нажали кнопку терминал
			if(MouseLB==true)
			{
				IEvent[0].Object=16;
				IEvent[0].Conditional=1;
				MouseLB=false;
			}
		}
		else if((MouseX>=37)&&(MouseX<=51)&&(MouseY>=334)&&(MouseY<=348))
		{
			// нажали кнопку архив
			if(MouseLB==true)
			{
				IEvent[0].Object=15;
				IEvent[0].Conditional=1;
				MouseLB=false;
			}
		}
		else if((MouseX>=759)&&(MouseX<=774)&&(MouseY>=9)&&(MouseY<=20))
		{
			// нажали кнопку mute
			if(MouseLB==true)
			{
				if(mute)
					mute=false;
				else
					mute=true;

				MuteSound(mute);
				MouseLB=false;
			}
		}
		else if((MouseX>=257)&&(MouseX<=357)&&(MouseY>=5)&&(MouseY<=24))
		{
			// изменили яркость
			lightning_bar=35;
			if(MouseLB==true)
			{
				int value = MouseX - 257;
				if(value<0)
					value=0;
				else if(value>100)
					value=100;
				tbright = 100 - value;
			}
		}
		else if((MouseX>=655)&&(MouseX<=755)&&(MouseY>=5)&&(MouseY<=24))
		{
			// изменили громкость
			lightning_bar=35;
			if(MouseLB==true)
			{
				int value = MouseX - 655;
				if(value<0)
					value=0;
				else if(value>100)
					value=100;
				svolume = value;
					SetVolume(svolume);
			}
		}
		else if((MouseX>=239)&&(MouseX<=253)&&(MouseY>=10)&&(MouseY<=19))
		{
			// яркость текста на максимум
			if(MouseLB==true)
			{
				tbright=100;
				MouseLB=false;
			}
		}
		else
		{
			if(MouseLB)
				MouseLB=false;
		}
		}
	}
	else if(mode==4)
	{
		if((inv_mode==0)||(inv_mode==2)||(inv_mode==-2))
		{
			if((MouseX>=532)&&(MouseX<=662)&&(MouseY>=108)&&(MouseY<=492))
			{
				if((MouseRB)&&(obj_Inv.GetObjIndex(MouseX-128, MouseY-108)!=-1))
				{
					if(cursor_mode<1)
						cursor_mode++;
					else
						cursor_mode=0;

					inv_mode=0;
					MouseRB = false;
				}
				else if((MouseLBDClick)&&(obj_Inv.GetObjIndex(MouseX-128, MouseY-108)!=-1)&&(drag==0)&&(cursor_mode==0))
				{
					drag=0;
					MouseLBDClick=false;
					MouseLB=false;
					obj_Inv.cur_itm=obj_Inv.GetObjIndex(MouseX-128, MouseY-108);
//					OpenSStream("IPICKUP.wav");

					if(obj_Inv.GetAmount()>1)
					{
						inv_mode=1;
						for (int i=0; i<5; ++i)
							amount[i]=0;

						amount[4]=1;
					}
					else
					{
						drag=0;
						OpenSStream("IPUTDOWN.wav");
						plr_Inv.AddItem(obj_Inv.GetAmount(), obj_Inv.GetDescription(), &Screen);
						obj_Inv.DeleteItem(obj_Inv.GetDescription(), &Screen);
						obj_Inv.cur_itm=-1;
					}					
				}
				else if((MouseLB)&&(obj_Inv.GetObjIndex(MouseX-128, MouseY-108)!=-1)&&(drag==0)&&(cursor_mode==0))
				{
					obj_Inv.cur_itm=obj_Inv.GetObjIndex(MouseX-128, MouseY-108);
					drag=1;
					OpenSStream("IPICKUP.wav");
				}
				else if((MouseLB)&&(obj_Inv.GetObjIndex(MouseX-128, MouseY-108)!=-1)&&(drag==0)&&(cursor_mode==1))
				{
					obj_Inv.cur_itm=obj_Inv.GetObjIndex(MouseX-128, MouseY-108);
					inv_mode=2; // переключимся в режим отображения описания
					obj_Inv.DrawDescription(&Screen);
				}
				else if((MouseLBup)&&(drag==-1))
				{
					if(plr_Inv.GetAmount()>1)
					{
						inv_mode=1;
						for (int i=0; i<5; ++i)
							amount[i]=0;

						amount[4]=1;
					}
					else
					{
						drag=0;
						OpenSStream("IPUTDOWN.wav");
						obj_Inv.AddItem(plr_Inv.GetAmount(), plr_Inv.GetDescription(), &Screen);
						plr_Inv.DeleteItem(plr_Inv.GetDescription(), &Screen);
						plr_Inv.cur_itm=-1;
					}
				}
				else if((MouseLBup)&&(drag==1))
				{
					drag=0;
					OpenSStream("IPUTDOWN.wav");
				}
			}
			else if((MouseX>=134)&&(MouseX<=264)&&(MouseY>=108)&&(MouseY<=492))
			{
				if((MouseRB)&&(plr_Inv.GetObjIndex(MouseX-128, MouseY-108)!=-1))
				{
					if(cursor_mode<3)
						cursor_mode++;
					else
						cursor_mode=0;
	
					inv_mode=0;
					MouseRB = false;
				}
				else if((MouseLBDClick)&&(plr_Inv.GetObjIndex(MouseX-128, MouseY-108)!=-1)&&(drag==0)&&(cursor_mode==0))
				{
					drag=0;
					MouseLBDClick=false;
					MouseLB=false;
					plr_Inv.cur_itm=plr_Inv.GetObjIndex(MouseX-128, MouseY-108);
//					OpenSStream("IPICKUP.wav");

					if(plr_Inv.GetAmount()>1)
					{
						inv_mode=1;
						for (int i=0; i<5; ++i)
							amount[i]=0;

						amount[4]=1;
					}
					else
					{
						drag=0;
						OpenSStream("IPUTDOWN.wav");
						obj_Inv.AddItem(plr_Inv.GetAmount(), plr_Inv.GetDescription(), &Screen);
						plr_Inv.DeleteItem(plr_Inv.GetDescription(), &Screen);
						plr_Inv.cur_itm=-1;
					}					
				}
				else if((MouseLB)&&(plr_Inv.GetObjIndex(MouseX-128, MouseY-108)!=-1)&&(drag==0)&&(cursor_mode==0))
				{
					plr_Inv.cur_itm=plr_Inv.GetObjIndex(MouseX-128, MouseY-108);
					drag=-1;
					OpenSStream("IPICKUP.wav");
				}
				else if((MouseLB)&&(plr_Inv.GetObjIndex(MouseX-128, MouseY-108)!=-1)&&(drag==0)&&(cursor_mode==1))
				{
					plr_Inv.cur_itm=plr_Inv.GetObjIndex(MouseX-128, MouseY-108);
					inv_mode=-2;
					plr_Inv.DrawDescription(&Screen);
				}
				else if((MouseLB)&&(plr_Inv.GetObjIndex(MouseX-128, MouseY-108)!=-1)&&(drag==0)&&(cursor_mode==2))
				{
					OpenSStream("IPICKUP.wav");
					plr_Inv.cur_itm=plr_Inv.GetObjIndex(MouseX-128, MouseY-108);
					IEvent[0].Object=32;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
				else if((MouseLB)&&(plr_Inv.GetObjIndex(MouseX-128, MouseY-108)!=-1)&&(drag==0)&&(cursor_mode==3))
				{
					plr_Inv.cur_itm=plr_Inv.GetObjIndex(MouseX-128, MouseY-108);
					prog_exit=true;
					cursor_mode=0;
					MouseLB=false;
				}
				else if((MouseLBup)&&(drag==1))
				{
					if(obj_Inv.GetAmount()>1)
					{
						inv_mode=1;
						for (int i=0; i<5; ++i)
							amount[i]=0;

						amount[4]=1;
					}
					else
					{
						drag=0;
						OpenSStream("IPUTDOWN.wav");
						plr_Inv.AddItem(obj_Inv.GetAmount(), obj_Inv.GetDescription(), &Screen);
						obj_Inv.DeleteItem(obj_Inv.GetDescription(), &Screen);
						obj_Inv.cur_itm=-1;
					}
				}
				else if((MouseLBup)&&(drag==-1))
				{
					drag=0;
					OpenSStream("IPUTDOWN.wav");
				}
			}
			else
			{
				if((drag!=0)&&(MouseLBup))
				{
					drag=0;
					OpenSStream("IPUTDOWN.wav");
				}

				cursor_mode=0;

				if(MouseLB)
					inv_mode=0;
			}

			if((drag==0)&&(MouseLB))
			{
				if(obj_Inv.scrollable_i) // если нажали на кнопки прокрутки в инвентаре торговца
				{
					if((MouseX>=664)&&(MouseX<=672)&&(MouseY>=108)&&(MouseY<=115)) // вверх
					{
						IEvent[0].Object=80;
						IEvent[0].Conditional=1;
						MouseLB=false;
					}
					else if((MouseX>=664)&&(MouseX<=672)&&(MouseY>=137)&&(MouseY<=144)) // вниз
					{
						IEvent[0].Object=81;
						IEvent[0].Conditional=1;
						MouseLB=false;
					}
				}
				else if(plr_Inv.scrollable_i) // если нажали на кнопки прокрутки в инвентаре игрока
				{
					if((MouseX>=270)&&(MouseX<=278)&&(MouseY>=108)&&(MouseY<=115)) // вверх
					{
						IEvent[0].Object=82;
						IEvent[0].Conditional=1;
						MouseLB=false;
					}
					else if((MouseX>=270)&&(MouseX<=278)&&(MouseY>=137)&&(MouseY<=144)) // вниз
					{
						IEvent[0].Object=83;
						IEvent[0].Conditional=1;
						MouseLB=false;
					}
				}
			}
		}
		else if((inv_mode==1)||(inv_mode==5))
		{
			if((MouseLB)&&(MouseX>=472)&&(MouseX<=487)&&(MouseY>=265)&&(MouseY<=275))
			{
				// +
				IEvent[0].Object=1;
				IEvent[0].Conditional=1;
			}
			else if((MouseLB)&&(MouseX>=472)&&(MouseX<=487)&&(MouseY>=276)&&(MouseY<=286))
			{
				// -
				IEvent[0].Object=2;
				IEvent[0].Conditional=1;
			}
			else if((MouseLB)&&(MouseX>=396)&&(MouseX<=484)&&(MouseY>=229)&&(MouseY<=326))
			{
				// all
				IEvent[0].Object=3;
				IEvent[0].Conditional=1;
				MouseLB=false;
			}
			else if((MouseLB)&&(MouseX>=371)&&(MouseX<=382)&&(MouseY>=350)&&(MouseY<=361))
			{
				// ok
				IEvent[0].Object=4;
				IEvent[0].Conditional=1;
				MouseLB=false;
			}
			else if((MouseLB)&&(MouseX>=421)&&(MouseX<=432)&&(MouseY>=349)&&(MouseY<=360))
			{
				// cancel
				IEvent[0].Object=5;
				IEvent[0].Conditional=1;
				MouseLB=false;
			}

		}
		else if((inv_mode==3)||(inv_mode==-4)) // окно экипировки
		{
			if((MouseX>=134)&&(MouseX<=264)&&(MouseY>=108)&&(MouseY<=492))
			{
				if((MouseRB)&&(plr_Inv.GetObjIndex(MouseX-128, MouseY-108)!=-1))
				{
					if(cursor_mode<3)
						cursor_mode++;
					else
						cursor_mode=0;
	
					inv_mode=3;
					MouseRB = false;
				}
				else if((MouseLB)&&(plr_Inv.GetObjIndex(MouseX-128, MouseY-108)!=-1)&&(drag==0)&&(cursor_mode==0))
				{
					plr_Inv.cur_itm=plr_Inv.GetObjIndex(MouseX-128, MouseY-108);
					drag=-1;
					OpenSStream("IPICKUP.wav");
				}
				else if((MouseLB)&&(plr_Inv.GetObjIndex(MouseX-128, MouseY-108)!=-1)&&(drag==0)&&(cursor_mode==1))
				{
					plr_Inv.cur_itm=plr_Inv.GetObjIndex(MouseX-128, MouseY-108);
					inv_mode=-4;
					plr_Inv.DrawDescription(&Screen);
				}
				else if((MouseLB)&&(plr_Inv.GetObjIndex(MouseX-128, MouseY-108)!=-1)&&(drag==0)&&(cursor_mode==2))
				{
					OpenSStream("IPICKUP.wav");
					plr_Inv.cur_itm=plr_Inv.GetObjIndex(MouseX-128, MouseY-108);
					IEvent[0].Object=32;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
				else if((MouseLB)&&(plr_Inv.GetObjIndex(MouseX-128, MouseY-108)!=-1)&&(drag==0)&&(cursor_mode==3))
				{
					plr_Inv.cur_itm=plr_Inv.GetObjIndex(MouseX-128, MouseY-108);
					prog_exit=true;
					cursor_mode=0;
					MouseLB=false;
				}
				else if((MouseLBup)&&(drag==1))
				{
					plr_Inv.AddItem(1, pFight->Weapon[0].player_weapon, &Screen);
					pFight->LoadWeapon("", &Screen);
					drag=0;
					OpenSStream("IPUTDOWN.wav");
					plr_Inv.cur_itm=-1;
				}
				else if((MouseLBup)&&(drag==-1))
				{
					drag=0;
					OpenSStream("IPUTDOWN.wav");					
				}
			}
			else if((MouseX>=329)&&(MouseX<=467)&&(MouseY>=400)&&(MouseY<=461)) // слот оружия
			{
				if((MouseLB)&&(pFight->Weapon[0].player_weapon!="")&&(drag==0)&&(cursor_mode==0))
				{
					drag=1;
					OpenSStream("IPICKUP.wav");
				}
				else if((MouseLBup)&&(drag==-1)) // если перетаскиваем в слот оружия
				{
					if(plr_Inv.GetType(plr_Inv.cur_itm)==1)
					{
						if(pFight->Weapon[0].player_weapon=="") // если руки пустые
						{
							pFight->LoadWeapon(plr_Inv.GetDescription(plr_Inv.cur_itm), &Screen);
							plr_Inv.GetItem(1, plr_Inv.GetDescription(plr_Inv.cur_itm), &Screen);
							plr_Inv.cur_itm=-1;
						}
						else // если в руках уже что-то есть
						{
							plr_Inv.AddItem(1, pFight->Weapon[0].player_weapon, &Screen);
							pFight->LoadWeapon(plr_Inv.GetDescription(plr_Inv.cur_itm), &Screen);
							plr_Inv.GetItem(1, plr_Inv.GetDescription(plr_Inv.cur_itm), &Screen);
							plr_Inv.cur_itm=-1;
						}

						drag=0;
						OpenSStream("IPUTDOWN.wav");	
					}
					else
					{
						drag=0;
						OpenSStream("IPUTDOWN.wav");
						plr_Inv.cur_itm=-1;
					}
				}							
			}
			else
			{
				if((drag!=0)&&(MouseLBup))
				{
					drag=0;
					OpenSStream("IPUTDOWN.wav");
				}

				cursor_mode=0;

				if(MouseLB)
					inv_mode=3;
			}

			if((drag==0)&&(MouseLB))
			{
				if(plr_Inv.scrollable_i) // если нажали на кнопки прокрутки в инвентаре игрока
				{
					if((MouseX>=270)&&(MouseX<=278)&&(MouseY>=108)&&(MouseY<=115)) // вверх
					{
						IEvent[0].Object=82;
						IEvent[0].Conditional=1;
						MouseLB=false;
					}
					else if((MouseX>=270)&&(MouseX<=278)&&(MouseY>=137)&&(MouseY<=144)) // вниз
					{
						IEvent[0].Object=83;
						IEvent[0].Conditional=1;
						MouseLB=false;
					}
				}
			}
		}

		if((MouseX>=686)&&(MouseX<=773)&&(MouseY>=540)&&(MouseY<=571)&&(GetData("Inventory")==1))
		{
			if(MouseLB==true)
			{
				IEvent[0].Object=30;
				IEvent[0].Conditional=1;
				MouseLB=false;
			}
		}
		else if((MouseX>=351)&&(MouseX<=362)&&(MouseY>=501)&&(MouseY<=512))
		{
			if(MouseLB==true)
			{
				IEvent[0].Object=31;
				IEvent[0].Conditional=1;
				MouseLB=false;
			}
		}
		if(prog_exit)
		{
			if(MouseLB==true)
			{
				if((MouseX>=258)&&(MouseX<=272)&&(MouseY>=364)&&(MouseY<=378))
				{
					IEvent[0].Object=35;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
				else if((MouseX>=433)&&(MouseX<=447)&&(MouseY>=364)&&(MouseY<=378))
				{
					IEvent[0].Object=36;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
		}
	}
	else if(mode==5)
	{
		if((inv_mode==0)||(inv_mode==2)||(inv_mode==-2)||(inv_mode==4)||(inv_mode==-4))
		{
			if((MouseX>=605)&&(MouseX<=725)&&(MouseY>=140)&&(MouseY<=460)) //область инвентаря объекта
			{
				if((MouseRB)&&(obj_Inv.GetObjIndex(MouseX-128, MouseY-108)!=-1))
				{
					if(cursor_mode==0)
						cursor_mode=1;
					else
						cursor_mode=0;

					inv_mode=0;
					MouseRB = false;
				}
				else if((MouseLBDClick)&&(obj_Inv.GetObjIndex(MouseX-128, MouseY-140)!=-1)&&(drag==0)&&(cursor_mode==0))
				{
					drag=0;
					MouseLBDClick=false;
					MouseLB=false;
					obj_Inv.cur_itm=obj_Inv.GetObjIndex(MouseX-128, MouseY-140);

					if(obj_Inv.GetAmount()>1)
					{
						inv_mode=1;
						for (int i=0; i<5; ++i)
							amount[i]=0;

						amount[4]=1;
						drag=1;
					}
					else
					{
						drag=0;
						OpenSStream("IPUTDOWN.wav");
						obj_Bar.AddItem(obj_Inv.GetAmount(), obj_Inv.GetDescription(), &Screen);
						obj_Inv.DeleteItem(obj_Inv.GetDescription(), &Screen);
						obj_Inv.cur_itm=-1;
					}					
				}
				else if((MouseLB)&&(obj_Inv.GetObjIndex(MouseX-128, MouseY-140)!=-1)&&(drag==0)&&(cursor_mode==0))
				{
					obj_Inv.cur_itm=obj_Inv.GetObjIndex(MouseX-128, MouseY-140);
					drag=1;
					OpenSStream("IPICKUP.wav");
				}
				else if(/*(MouseLB)&&*/(obj_Inv.GetObjIndex(0, MouseY-140)!=-1)&&(drag==0)&&(cursor_mode==1))
				{
					obj_Inv.cur_itm=obj_Inv.GetObjIndex(MouseX-128, MouseY-140);
					inv_mode = 2; // переключиться в режим описания
					obj_Inv.DrawBDescription(&Screen);
				}
				else if((MouseLBup)&&(drag==-1))
				{
					if(obj_Bar.GetAmount()>1)
					{
						inv_mode=1;
						for (int i=0; i<5; ++i)
							amount[i]=0;

						amount[4]=1;
					}
					else
					{
						drag=0;
						OpenSStream("IPUTDOWN.wav");
						obj_Inv.AddItem(obj_Bar.GetAmount(), obj_Bar.GetDescription(), &Screen);
						obj_Bar.DeleteItem(obj_Bar.GetDescription(), &Screen);
						obj_Bar.cur_itm=-1;
					}
				}
				else if((MouseLBup)&&(drag==1))
				{
					drag=0;
					OpenSStream("IPUTDOWN.wav");
				}
			}
			else if((MouseX>=429)&&(MouseX<=509)&&(MouseY>=173)&&(MouseY<=403)) // область бартера объекта
			{
				if((MouseRB)&&(obj_Inv.GetObjIndex(MouseX-128, MouseY-108)!=-1))
				{
					if(cursor_mode==0)
						cursor_mode=1;
					else
						cursor_mode=0;

					inv_mode=0;
					MouseRB = false;
				}
				else if((MouseLBDClick)&&(obj_Bar.GetObjIndex(MouseX-128, MouseY-173)!=-1)&&(drag==0)&&(cursor_mode==0))
				{
					drag=0;
					MouseLBDClick=false;
					MouseLB=false;
					obj_Bar.cur_itm=obj_Bar.GetObjIndex(MouseX-128, MouseY-173);

					if(obj_Bar.GetAmount()>1)
					{
						inv_mode=1;
						for (int i=0; i<5; ++i)
							amount[i]=0;

						amount[4]=1;
						drag=-1;
					}
					else
					{
						drag=0;
						OpenSStream("IPUTDOWN.wav");
						obj_Inv.AddItem(obj_Bar.GetAmount(), obj_Bar.GetDescription(), &Screen);
						obj_Bar.DeleteItem(obj_Bar.GetDescription(), &Screen);
						obj_Bar.cur_itm=-1;
					}					
				}
				else if((MouseLB)&&(obj_Bar.GetObjIndex(MouseX-128, MouseY-173)!=-1)&&(drag==0)&&(cursor_mode==0))
				{
					obj_Bar.cur_itm=obj_Bar.GetObjIndex(MouseX-128, MouseY-173);
					drag=-1;
					OpenSStream("IPICKUP.wav");
				}
				else if(/*(MouseLB)&&*/(obj_Bar.GetObjIndex(MouseX-128, MouseY-173)!=-1)&&(drag==0)&&(cursor_mode==1))
				{
					obj_Bar.cur_itm=obj_Inv.GetObjIndex(MouseX-128, MouseY-173);
					inv_mode = 4; // переключиться в режим описания
					obj_Bar.DrawDescription(&Screen);
				}
				else if((MouseLBup)&&(drag==1))
				{
					if(obj_Inv.GetAmount()>1)
					{
						inv_mode=1;
						for (int i=0; i<5; ++i)
							amount[i]=0;

						amount[4]=1;
					}
					else
					{
						drag=0;
						OpenSStream("IPUTDOWN.wav");
						obj_Bar.AddItem(obj_Inv.GetAmount(), obj_Inv.GetDescription(), &Screen);
						obj_Inv.DeleteItem(obj_Inv.GetDescription(), &Screen);
						obj_Inv.cur_itm=-1;
					}
				}
				else if((MouseLBup)&&(drag==-1))
				{
					drag=0;
					OpenSStream("IPUTDOWN.wav");
				}
			}
			else if((MouseX>=75)&&(MouseX<=195)&&(MouseY>=140)&&(MouseY<=460)) // область инвентаря игрока
			{
				if((MouseRB)&&(obj_Inv.GetObjIndex(MouseX-128, MouseY-108)!=-1))
				{
					if(cursor_mode==0)
						cursor_mode=1;
					else
						cursor_mode=0;

					inv_mode=0;
					MouseRB = false;
				}
				else if((MouseLBDClick)&&(plr_Inv.GetObjIndex(MouseX-128, MouseY-140)!=-1)&&(drag==0)&&(cursor_mode==0))
				{
					drag=0;
					MouseLBDClick=false;
					MouseLB=false;
					plr_Inv.cur_itm=plr_Inv.GetObjIndex(MouseX-128, MouseY-140);

					if(plr_Inv.GetAmount()>1)
					{
						inv_mode=1;
						for (int i=0; i<5; ++i)
							amount[i]=0;

						amount[4]=1;
						drag=-2;
					}
					else
					{
						drag=0;
						OpenSStream("IPUTDOWN.wav");
						plr_Bar.AddItem(plr_Inv.GetAmount(), plr_Inv.GetDescription(), &Screen);
						plr_Inv.DeleteItem(plr_Inv.GetDescription(), &Screen);
						plr_Inv.cur_itm=-1;
					}					
				}
				else if((MouseLB)&&(plr_Inv.GetObjIndex(MouseX-128, MouseY-140)!=-1)&&(drag==0)&&(cursor_mode==0))
				{
					plr_Inv.cur_itm=plr_Inv.GetObjIndex(MouseX-128, MouseY-140);
					drag=-2;
					OpenSStream("IPICKUP.wav");
				}
				else if(/*(MouseLB)&&*/(plr_Inv.GetObjIndex(MouseX-128, MouseY-140)!=-1)&&(drag==0)&&(cursor_mode==1))
				{
					plr_Inv.cur_itm=obj_Inv.GetObjIndex(MouseX-128, MouseY-140);
					inv_mode = -2; // переключиться в режим описания
					plr_Inv.DrawBDescription(&Screen);					
				}
				else if((MouseLBup)&&(drag==2))
				{
					if(plr_Bar.GetAmount()>1)
					{
						inv_mode=1;
						for (int i=0; i<5; ++i)
							amount[i]=0;

						amount[4]=1;
					}
					else
					{
						drag=0;
						OpenSStream("IPUTDOWN.wav");
						plr_Inv.AddItem(plr_Bar.GetAmount(), plr_Bar.GetDescription(), &Screen);
						plr_Bar.DeleteItem(plr_Bar.GetDescription(), &Screen);
						plr_Bar.cur_itm=-1;
					}
				}
				else if((MouseLBup)&&(drag==-2))
				{
					drag=0;
					OpenSStream("IPUTDOWN.wav");
				}
			}
			else if((MouseX>=291)&&(MouseX<=371)&&(MouseY>=173)&&(MouseY<=403)) // область бартера игрока
			{
				if((MouseRB)&&(obj_Inv.GetObjIndex(MouseX-128, MouseY-108)!=-1))
				{
					if(cursor_mode==0)
						cursor_mode=1;
					else
						cursor_mode=0;

					inv_mode=0;
					MouseRB = false;
				}
				else if((MouseLBDClick)&&(plr_Bar.GetObjIndex(MouseX-128, MouseY-173)!=-1)&&(drag==0)&&(cursor_mode==0))
				{
					drag=0;
					MouseLBDClick=false;
					MouseLB=false;
					plr_Bar.cur_itm=plr_Bar.GetObjIndex(MouseX-128, MouseY-173);

					if(plr_Bar.GetAmount()>1)
					{
						inv_mode=1;
						for (int i=0; i<5; ++i)
							amount[i]=0;

						amount[4]=1;
						drag=2;
					}
					else
					{
						drag=0;
						OpenSStream("IPUTDOWN.wav");
						plr_Inv.AddItem(plr_Bar.GetAmount(), plr_Bar.GetDescription(), &Screen);
						plr_Bar.DeleteItem(plr_Bar.GetDescription(), &Screen);
						plr_Bar.cur_itm=-1;
					}					
				}
				else if((MouseLB)&&(plr_Bar.GetObjIndex(MouseX-128, MouseY-173)!=-1)&&(drag==0)&&(cursor_mode==0))
				{
					plr_Bar.cur_itm=plr_Bar.GetObjIndex(MouseX-128, MouseY-173);
					drag=2;
					OpenSStream("IPICKUP.wav");
				}
				else if(/*(MouseLB)&&*/(plr_Bar.GetObjIndex(MouseX-128, MouseY-173)!=-1)&&(drag==0)&&(cursor_mode==1))
				{
					plr_Bar.cur_itm=obj_Inv.GetObjIndex(MouseX-128, MouseY-173);
					inv_mode = -4; // переключиться в режим описания
					plr_Bar.DrawDescription(&Screen);
				}
				else if((MouseLBup)&&(drag==-2))
				{
					if(plr_Inv.GetAmount()>1)
					{
						inv_mode=1;
						for (int i=0; i<5; ++i)
							amount[i]=0;

						amount[4]=1;
					}
					else
					{
						drag=0;
						OpenSStream("IPUTDOWN.wav");
						plr_Bar.AddItem(plr_Inv.GetAmount(), plr_Inv.GetDescription(), &Screen);
						plr_Inv.DeleteItem(plr_Inv.GetDescription(), &Screen);
						plr_Inv.cur_itm=-1;
					}
				}
				else if((MouseLBup)&&(drag==2))
				{
					drag=0;
					OpenSStream("IPUTDOWN.wav");
				}
			}
			else
			{
				if((drag!=0)&&(MouseLBup))
				{
					drag=0;
					OpenSStream("IPUTDOWN.wav");
				}

				cursor_mode=0;
				if((inv_mode==2)||(inv_mode==-2)||(inv_mode==4)||(inv_mode==-4))
					inv_mode=0;

				if(MouseLB)
					inv_mode=0;
			}

			if((drag==0)&&(MouseLB))
			{
				if(obj_Inv.scrollable) // если нажали на кнопки прокрутки в инвентаре торговца
				{
					if((MouseX>=566)&&(MouseX<=581)&&(MouseY>=279)&&(MouseY<=295)) // вверх
					{
						IEvent[0].Object=80;
						IEvent[0].Conditional=1;
						MouseLB=false;
					}
					else if((MouseX>=566)&&(MouseX<=581)&&(MouseY>=298)&&(MouseY<=314)) // вниз
					{
						IEvent[0].Object=81;
						IEvent[0].Conditional=1;
						MouseLB=false;
					}
				}
				else if(plr_Inv.scrollable) // если нажали на кнопки прокрутки в инвентаре игрока
				{
					if((MouseX>=221)&&(MouseX<=236)&&(MouseY>=279)&&(MouseY<=295)) // вверх
					{
						IEvent[0].Object=82;
						IEvent[0].Conditional=1;
						MouseLB=false;
					}
					else if((MouseX>=221)&&(MouseX<=236)&&(MouseY>=298)&&(MouseY<=314)) // вниз
					{
						IEvent[0].Object=83;
						IEvent[0].Conditional=1;
						MouseLB=false;
					}
				}
				else if(obj_Bar.scrollable) // если нажали на кнопки прокрутки в столе обмена торговца
				{
					if((MouseX>=542)&&(MouseX<=555)&&(MouseY>=282)&&(MouseY<=295)) // вверх
					{
						IEvent[0].Object=84;
						IEvent[0].Conditional=1;
						MouseLB=false;
					}
					else if((MouseX>=542)&&(MouseX<=555)&&(MouseY>=298)&&(MouseY<=311)) // вниз
					{
						IEvent[0].Object=85;
						IEvent[0].Conditional=1;
						MouseLB=false;
					}
				}
				else if(plr_Bar.scrollable) // если нажали на кнопки прокрутки в столе обмена игрока
				{
					if((MouseX>=247)&&(MouseX<=260)&&(MouseY>=282)&&(MouseY<=295)) // вверх
					{
						IEvent[0].Object=86;
						IEvent[0].Conditional=1;
						MouseLB=false;
					}
					else if((MouseX>=247)&&(MouseX<=260)&&(MouseY>=298)&&(MouseY<=311)) // вниз
					{
						IEvent[0].Object=87;
						IEvent[0].Conditional=1;
						MouseLB=false;
					}
				}
			}
		}
		else if((inv_mode==1)||(inv_mode==5))
		{
			if((MouseLB)&&(MouseX>=472)&&(MouseX<=487)&&(MouseY>=265)&&(MouseY<=275))
			{
				// +
				IEvent[0].Object=1;
				IEvent[0].Conditional=1;
			}
			else if((MouseLB)&&(MouseX>=472)&&(MouseX<=487)&&(MouseY>=276)&&(MouseY<=286))
			{
				// -
				IEvent[0].Object=2;
				IEvent[0].Conditional=1;
			}
			else if((MouseLB)&&(MouseX>=396)&&(MouseX<=484)&&(MouseY>=229)&&(MouseY<=326))
			{
				// all
				IEvent[0].Object=3;
				IEvent[0].Conditional=1;
				MouseLB=false;
			}
			else if((MouseLB)&&(MouseX>=371)&&(MouseX<=382)&&(MouseY>=350)&&(MouseY<=361))
			{
				// ok
				IEvent[0].Object=4;
				IEvent[0].Conditional=1;
				MouseLB=false;
			}
			else if((MouseLB)&&(MouseX>=421)&&(MouseX<=432)&&(MouseY>=349)&&(MouseY<=360))
			{
				// cancel
				IEvent[0].Object=5;
				IEvent[0].Conditional=1;
				MouseLB=false;
			}

		}
		
		if((MouseX>=284)&&(MouseX<=295)&&(MouseY>=478)&&(MouseY<=489))
		{ // кнопка "Сделка"
			if(MouseLB==true)
			{
				IEvent[0].Object=30;
				IEvent[0].Conditional=1;
				MouseLB=false;
			}
		}
		else if((MouseX>=419)&&(MouseX<=430)&&(MouseY>=478)&&(MouseY<=489))
		{ // кнопка "Отмена"
			if(MouseLB==true)
			{
				IEvent[0].Object=31;
				IEvent[0].Conditional=1;
				MouseLB=false;
			}
		}
		if(prog_exit)
		{
			if(MouseLB==true)
			{
				if((MouseX>=258)&&(MouseX<=272)&&(MouseY>=364)&&(MouseY<=378))
				{
					IEvent[0].Object=35;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
				else if((MouseX>=433)&&(MouseX<=447)&&(MouseY>=364)&&(MouseY<=378))
				{
					IEvent[0].Object=36;
					IEvent[0].Conditional=1;
					MouseLB=false;
				}
			}
		}
	}
	else if(mode==-2)
	{
		if(MouseLB)
		{
			if((MouseX>=74)&&(MouseX<=364)&&(MouseY>=83)&&(MouseY<=128))
			{
				IEvent[0].Object=1;
				if(MouseLBDClick)
					IEvent[0].Conditional=2;
				else
					IEvent[0].Conditional=1;
			}
			else if((MouseX>=74)&&(MouseX<=364)&&(MouseY>=129)&&(MouseY<=174))
			{
				IEvent[0].Object=2;
				if(MouseLBDClick)
					IEvent[0].Conditional=2;
				else
					IEvent[0].Conditional=1;
			}
			else if((MouseX>=74)&&(MouseX<=364)&&(MouseY>=175)&&(MouseY<=220))
			{
				IEvent[0].Object=3;
				if(MouseLBDClick)
					IEvent[0].Conditional=2;
				else
					IEvent[0].Conditional=1;
			}
			else if((MouseX>=74)&&(MouseX<=364)&&(MouseY>=221)&&(MouseY<=266))
			{
				IEvent[0].Object=4;
				if(MouseLBDClick)
					IEvent[0].Conditional=2;
				else
					IEvent[0].Conditional=1;
			}
			else if((MouseX>=74)&&(MouseX<=364)&&(MouseY>=267)&&(MouseY<=312))
			{
				IEvent[0].Object=5;
				if(MouseLBDClick)
					IEvent[0].Conditional=2;
				else
					IEvent[0].Conditional=1;
			}
			else if((MouseX>=74)&&(MouseX<=364)&&(MouseY>=313)&&(MouseY<=358))
			{
				IEvent[0].Object=6;
				if(MouseLBDClick)
					IEvent[0].Conditional=2;
				else
					IEvent[0].Conditional=1;
			}
			else if((MouseX>=74)&&(MouseX<=364)&&(MouseY>=359)&&(MouseY<=404))
			{
				IEvent[0].Object=7;
				if(MouseLBDClick)
					IEvent[0].Conditional=2;
				else
					IEvent[0].Conditional=1;
			}
			else if((MouseX>=74)&&(MouseX<=364)&&(MouseY>=405)&&(MouseY<=450))
			{
				IEvent[0].Object=8;
				if(MouseLBDClick)
					IEvent[0].Conditional=2;
				else
					IEvent[0].Conditional=1;
			}
			else if((MouseX>=74)&&(MouseX<=364)&&(MouseY>=451)&&(MouseY<=496))
			{
				IEvent[0].Object=9;
				if(MouseLBDClick)
					IEvent[0].Conditional=2;
				else
					IEvent[0].Conditional=1;
			}
			else if((MouseX>=74)&&(MouseX<=364)&&(MouseY>=497)&&(MouseY<=543))
			{
				IEvent[0].Object=10;
				if(MouseLBDClick)
					IEvent[0].Conditional=2;
				else
					IEvent[0].Conditional=1;
			}
			else if((MouseX>=478)&&(MouseX<=490)&&(MouseY>=450)&&(MouseY<=462))
			{
				IEvent[0].Object=11;
				IEvent[0].Conditional=1;
			}
			else if((MouseX>=613)&&(MouseX<=625)&&(MouseY>=450)&&(MouseY<=462))
			{
				IEvent[0].Object=12;
				IEvent[0].Conditional=1;
			}
		}
	}

	if((mode!=4)&&(mode!=2)&&(mode!=5)&&(mode!=3))
	{
		MouseLB=false;
		
	}
	MouseLBDClick=false;
	MouseWUp=false;
	MouseWDown=false;
	MouseLBup=false;
}

void MainMenu()
{
	Splash.Draw(Screen.Back,0,0,NDX_RECT(0,0,800,600));
	Cpy_info.Draw(Screen.Back, 0, 580);

	Mnu_panel.Draw(Screen.Back, mnupanel_x, mnupanel_y);

	Buttons.Draw(Screen.Back,mnubt_x,mnubt_y,NDX_RECT(0,0,240,300));

	if(IEvent[0].Conditional!=0)
	{
		if(IEvent[0].Object==5)
		{
			if(IEvent[0].Conditional==1)
			{
				Click();
				Buttons.Draw(Screen.Back,510,430,NDX_RECT(240,260,480,300));
				IEvent[1].Object=5;
				IEvent[1].Conditional=1;
			}
		}
		else if(IEvent[0].Object==2)
		{
			if(IEvent[0].Conditional==1)
			{
				Click();
				Buttons.Draw(Screen.Back,510,235,NDX_RECT(240,65,480,105));
				IEvent[1].Object=2;
				IEvent[1].Conditional=1;
			}
		}
		else if(IEvent[0].Object==3)
		{
			if(IEvent[0].Conditional==1)
			{
				Click();
				Buttons.Draw(Screen.Back,510,300,NDX_RECT(240,130,480,170));
				IEvent[1].Object=3;
				IEvent[1].Conditional=1;
			}
		}
		else if(IEvent[0].Object==4)
		{
			if(IEvent[0].Conditional==1)
			{
				Click();
				Buttons.Draw(Screen.Back,510,365,NDX_RECT(240,195,480,235));
				IEvent[1].Object=4;
				IEvent[1].Conditional=1;
			}
		}
	}

	if(prog_exit)
	{
		Error.Draw(Screen.Back, 201, 200);
		Misc_but.Draw(Screen.Back, 246, 360, NDX_RECT(0, 0, 125, 24));
		Misc_but.Draw(Screen.Back, 421, 360, NDX_RECT(0, 24, 125, 48));
		Screen.Back->FormattedText(291,260,RGB(207, 191, 63),NULL,"Вы уверены, что хотите выйти?");


		if(IEvent[0].Conditional!=0)
		{
			if(IEvent[0].Object==10)
			{
				if(IEvent[0].Conditional==1)
				{
					Click();
					Misc_but.Draw(Screen.Back,246,360,NDX_RECT(125,0,250,24));
					IEvent[1].Object=10;
					IEvent[1].Conditional=1;
				}
			}
			else if(IEvent[0].Object==11)
			{
				if(IEvent[0].Conditional==1)
				{
					Click();
					Misc_but.Draw(Screen.Back,421,360,NDX_RECT(125,24,250,48));
					IEvent[1].Object=11;
					IEvent[1].Conditional=1;
				}
			}
		}
	}

	if(fade==TRUE)
	{
		NDX_Surface			Fon; //фон

		Fon.Create(&Screen,NDXST_SYSMEM,800, 600);

		Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
	
		for(int i=0; i<17; ++i)
		{
			Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
			Screen.Flip();
			Sleep(30);
		}
	
		fade=false;
	}

	// курсор мышки
	if(!showcursor)
		Cursor.Draw(Screen.Back,MouseX,MouseY);	
}

void ManageMessages()
{
	if(mode==1)
	{
		if(IEvent[1].Object==5)
		{
			if(IEvent[1].Conditional==1)
			{
				Sleep(200);
				prog_exit=true;
//				PostMessage(hWindow, WM_CLOSE, 0, 0L);
			}
		}
		else if(IEvent[1].Object==2)
		{
			if(IEvent[1].Conditional==1)
			{
				InitGameValues();
				Sleep(200);
				NDX_Surface			Fon; //фон
				Fon.Create(&Screen,NDXST_SYSMEM,800, 600);
					
				Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
					
				for(int i=17; i>0; --i)
				{
					Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
					Screen.Flip();
					Sleep(30);
				}
				fade=true;
				StopMStream();
				OpenMStream("data\\sound\\music\\15shady.acm");
				LoadScenario();
				PrepareFrame(0);
				PlayMStream();
				mode=2;
			}
		}
		else if(IEvent[1].Object==3)
		{
			if((IEvent[1].Conditional==1)&&(!table_mark))
			{
				Sleep(200);
				NDX_Surface			Fon; //фон
				Fon.Create(&Screen,NDXST_SYSMEM,800, 600);
					
				Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
					
				for(int i=17; i>0; --i)
				{
					Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
					Screen.Flip();
					Sleep(30);
				}
				PrepareSaveload();
				fade=TRUE;
				last_mode=mode;
				mode=-2;
				save=false;
			}
		}
		else if(IEvent[1].Object==4)
		{
			if(IEvent[1].Conditional==1)
			{
				Sleep(200);
				StopMStream();
				OpenMStream("data\\sound\\music\\credits.acm");
				PlayMStream();
				MD.SetFileDescription("Fallout Data File");
				MD.Open(gfxnf);
				Splash.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("CREDITS/credits.bmp"),true);
				
				CStdioFile file;
				
				battlelog="";
			
				MD.ExtractFile_ToFile("TXT/Russian/credits.txt","tempt");

				MD.Close();

				BOOL opened = file.Open("tempt", CFile::modeRead);
				if (opened)
				{
					CString string;
					while(file.ReadString(string))
					{
						battlelog+=string;
						battlelog+="\n\n";
					}
				}
				file.Close();

				DeleteFile("tempt");

				int i = battlelog.GetLength();
				int rows=1;
				int sn=0;
				int space=0;
				int lsp=0;
				
				//вычисляем размер текста	
				for(int z=0; z<i; ++z)
				{
					if(battlelog.GetAt(z)==' ')
					{
						//			lsp=space;
						space=z+1;
					}
					
					if(sn*12>342)
					{
						rows=rows+1;
						sn=0;
						z=space;
						lsp=space;
						
					}
					else if(battlelog.GetAt(z)=='\n')
					{
						rows=rows+1;
						sn=0;
						z=z+1;
						lsp=z;
					}
					sn=sn+1;
				}

				int height=32;

				if(rows*16<maintext_height)
					height+=maintext_height;
				else 
					height+=rows*16;

				Main_txt.Create(&Screen, NDXST_SYSMEM, maintext_width, height);
				Main_txt.Clear(Screen.PixelFormat->Rgb(0,0,0));
				Main_txt.SetColorKey(Screen.PixelFormat->Rgb(0,0,0));
				Main_txt.SetTransP(true);

				WriteText();
				fr=460;
//				Main_txt.FormattedText(0, 0, RGB(255, 255, 255), fnt1, fn);

				mode=-1;
			}
		}
		else if(IEvent[1].Object==10)
		{
			if(IEvent[1].Conditional==1)
			{
				Sleep(200);
				NDX_Surface			Fon; //фон
				Fon.Create(&Screen,NDXST_SYSMEM,800, 600);
				
				Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
				
				for(int i=17; i>0; --i)
				{
					Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
					Screen.Flip();
					Sleep(30);
				}
				fade=true;
				PostMessage(hWindow, WM_CLOSE, 0, 0L);
			}
		}
		else if(IEvent[1].Object==11)
		{
			if(IEvent[1].Conditional==1)
			{
				Sleep(200);
				prog_exit=false;
			}
		}

	}
	else if(mode==2)
	{
		if(save)
		{
			if(IEvent[1].Object==4)
			{
				if(IEvent[1].Conditional==1)
				{
					Sleep(200);
					save=false;
				}
			}
			else if(IEvent[1].Object==3)
			{
				if(IEvent[1].Conditional==1)
				{
					Sleep(200);
					StopMStream();
					NDX_Surface			Fon; //фон
					Fon.Create(&Screen,NDXST_SYSMEM,800, 600);
					
					Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
					
					for(int i=17; i>0; --i)
					{
						Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
						Screen.Flip();
						Sleep(60);
					}
					OpenMStream("data\\sound\\music\\menu.acm");
					SetMVolume(1);
					PlayMStream();
					fade=true;
					mode=0;
					save=false;
					
				}
			}
			else if(IEvent[1].Object==2)
			{
				if((IEvent[1].Conditional==1)&&(!table_mark))
				{
					Sleep(200);
					NDX_Surface			Fon; //фон
					Fon.Create(&Screen,NDXST_SYSMEM,800, 600);
					
					Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
					
					for(int i=17; i>0; --i)
					{
						Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
						Screen.Flip();
						Sleep(30);
					}
					PrepareSaveload();
					fade=TRUE;
					last_mode=mode;
					mode=-2;
					save=false;
					
				}
			}
			else if(IEvent[1].Object==1)
			{
				if((IEvent[1].Conditional==1)&&(!table_mark))
				{
					Sleep(200);
					NDX_Surface			Fon; //фон
					Fon.Create(&Screen,NDXST_SYSMEM,800, 600);
					
					Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
					
					for(int i=17; i>0; --i)
					{
						Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
						Screen.Flip();
						Sleep(30);
					}
					PrepareSaveload();
					fade=TRUE;
					last_mode=mode;
					mode=-2;
					save=true;
					
				}
			}
		}
		else if(codecrack)
		{
			if(prog_error)
			{
				if(IEvent[1].Conditional==1)
				{
					Sleep(200);
					prog_error=false;
					if(safe_cr.timer==-2)
						safe_cr.Restart();
					else if(safe_cr.timer==-3)
					{
						codecrack=false;
						PrepareFrame(safe_cr.l_fail);
					}
					else if(safe_cr.timer==-4)
					{
						codecrack=false;
						PrepareFrame(safe_cr.l_success);
					}
				}
			}

			if(IEvent[1].Object==57)
			{
				if(IEvent[1].Conditional==1)
				{
					Sleep(200);
					if(safe_cr.c1<=8)
						safe_cr.c1++;
					else
						safe_cr.c1=0;

					Click();
				}
			}
			else if(IEvent[1].Object==58)
			{
				if(IEvent[1].Conditional==1)
				{
					Sleep(200);
					if(safe_cr.c1>0)
						safe_cr.c1--;
					else
						safe_cr.c1=9;

					Click();
				}
			}
			else if(IEvent[1].Object==59)
			{
				if(IEvent[1].Conditional==1)
				{
					if(safe_cr.cn2)
					{
						if(safe_cr.c2<9)
							safe_cr.c2++;
						else
							safe_cr.c2=0;

						Click();
					}
					else
						OpenSStream("deny.wav");

					Sleep(200);
				}
			}
			else if(IEvent[1].Object==60)
			{
				if(IEvent[1].Conditional==1)
				{
					if(safe_cr.cn2)
					{
						if(safe_cr.c2>0)
							safe_cr.c2--;
						else
							safe_cr.c2=9;

						Click();
					}
					else
						OpenSStream("deny.wav");

					Sleep(200);
				}
			}
			else if(IEvent[1].Object==61)
			{
				if(IEvent[1].Conditional==1)
				{
					if(safe_cr.cn3)
					{
						if(safe_cr.c3<9)
							safe_cr.c3++;
						else
							safe_cr.c3=0;

						Click();
					}
					else
						OpenSStream("deny.wav");

					Sleep(200);
				}
			}
			else if(IEvent[1].Object==62)
			{
				if(IEvent[1].Conditional==1)
				{
					if(safe_cr.cn3)
					{
						if(safe_cr.c3>0)
							safe_cr.c3--;
						else
							safe_cr.c3=9;

						Click();
					}
					else
						OpenSStream("deny.wav");

					Sleep(200);
				}
			}
			else if(IEvent[1].Object==63)
			{
				if(IEvent[1].Conditional==1)
				{
					if(safe_cr.cn4)
					{
						if(safe_cr.c4<9)
							safe_cr.c4++;
						else
							safe_cr.c4=0;

						Click();
					}
					else
						OpenSStream("deny.wav");

					Sleep(200);
				}
			}
			else if(IEvent[1].Object==64)
			{
				if(IEvent[1].Conditional==1)
				{
					if(safe_cr.cn4)
					{
						if(safe_cr.c4>0)
							safe_cr.c4--;
						else
							safe_cr.c4=9;

						Click();
					}
					else
						OpenSStream("deny.wav");

					Sleep(200);
				}
			}
			else if(IEvent[1].Object==65)
			{
				if(IEvent[1].Conditional==1)
				{
					if(safe_cr.cn5)
					{
						if(safe_cr.c5<9)
							safe_cr.c5++;
						else
							safe_cr.c5=0;

						Click();
					}
					else
						OpenSStream("deny.wav");

					Sleep(200);
				}
			}
			else if(IEvent[1].Object==66)
			{
				if(IEvent[1].Conditional==1)
				{
					if(safe_cr.cn5)
					{
						if(safe_cr.c5>0)
							safe_cr.c5--;
						else
							safe_cr.c5=9;

						Click();
					}
					else
						OpenSStream("deny.wav");

					Sleep(200);
				}
			}
			else if(IEvent[1].Object==67)
			{
				if(IEvent[1].Conditional==1)
				{
					if(safe_cr.cn6)
					{
						if(safe_cr.c6<9)
							safe_cr.c6++;
						else
							safe_cr.c6=0;

						Click();
					}
					else
						OpenSStream("deny.wav");

					Sleep(200);
				}
			}
			else if(IEvent[1].Object==68)
			{
				if(IEvent[1].Conditional==1)
				{
					if(safe_cr.cn6)
					{
						if(safe_cr.c6>0)
							safe_cr.c6--;
						else
							safe_cr.c6=9;

						Click();
					}
					else
						OpenSStream("deny.wav");

					Sleep(200);
				}
			}
			else if(IEvent[1].Object==69)
			{
				if(IEvent[1].Conditional==1)
				{
					if(safe_cr.cn7)
					{
						if(safe_cr.c7<9)
							safe_cr.c7++;
						else
							safe_cr.c7=0;

						Click();
					}
					else
						OpenSStream("deny.wav");

					Sleep(200);
				}
			}
			else if(IEvent[1].Object==70)
			{
				if(IEvent[1].Conditional==1)
				{
					if(safe_cr.cn7)
					{
						if(safe_cr.c7>0)
							safe_cr.c7--;
						else
							safe_cr.c7=9;

						Click();
					}
					else
						OpenSStream("deny.wav");

					Sleep(200);
				}
			}
			else if(IEvent[1].Object==50)
			{
				if(IEvent[1].Conditional==1)
				{
					if(safe_cr.Code1())
					{
						NextS();
					}
					else
					{
						OpenSStream("deny.wav");
						if(safe_cr.tryes>0)
						{
							prog_error=true;
							error_msg="Попытка провалена! Попробуйте еще раз.";
							error_id=4;
							safe_cr.timer=-2;
						}
						else
						{
							prog_error=true;
							error_msg="Попытка взлома. Сейф заблокирован.";
							error_id=4;
							safe_cr.timer=-3;
						}
					}

					Sleep(200);
				}
			}
			else if(IEvent[1].Object==51)
			{
				if(IEvent[1].Conditional==1)
				{
					if(safe_cr.Code2())
					{
						NextS();
					}
					else
					{
						OpenSStream("deny.wav");
						if(safe_cr.cn2)
						{
							if(safe_cr.tryes>0)
							{
								prog_error=true;
								error_msg="Попытка провалена! Попробуйте еще раз.";
								error_id=4;
								safe_cr.timer=-2;
							}
							else
							{
								prog_error=true;
								error_msg="Сейф заблокирован. Вы не смогли ввести шифр.";
								error_id=4;
								safe_cr.timer=-3;
							}
						}
					}

					Sleep(200);
				}
			}
			else if(IEvent[1].Object==52)
			{
				if(IEvent[1].Conditional==1)
				{
					if(safe_cr.Code3())
					{
						NextS();
					}
					else
					{
						OpenSStream("deny.wav");
						if(safe_cr.cn3)
						{
							if(safe_cr.tryes>0)
							{
								prog_error=true;
								error_msg="Попытка провалена! Попробуйте еще раз.";
								error_id=4;
								safe_cr.timer=-2;
							}
							else
							{
								prog_error=true;
								error_msg="Сейф заблокирован. Вы не смогли ввести шифр.";
								error_id=4;
								safe_cr.timer=-3;
							}
						}						
					}

					Sleep(200);
				}
			}
			else if(IEvent[1].Object==53)
			{
				if(IEvent[1].Conditional==1)
				{
					if(safe_cr.Code4())
					{
						NextS();
					}
					else
					{
						OpenSStream("deny.wav");
						if(safe_cr.cn4)
						{
							if(safe_cr.tryes>0)
							{
								prog_error=true;
								error_msg="Попытка провалена! Попробуйте еще раз.";
								error_id=4;
								safe_cr.timer=-2;
							}
							else
							{
								prog_error=true;
								error_msg="Сейф заблокирован. Вы не смогли ввести шифр.";
								error_id=4;
								safe_cr.timer=-3;
							}
						}
					}

					Sleep(200);
				}
			}
			else if(IEvent[1].Object==54)
			{
				if(IEvent[1].Conditional==1)
				{
					if(safe_cr.Code5())
					{
						NextS();
					}
					else
					{
						OpenSStream("deny.wav");
						if(safe_cr.cn5)
						{
							if(safe_cr.tryes>0)
							{
								prog_error=true;
								error_msg="Попытка провалена! Попробуйте еще раз.";
								error_id=4;
								safe_cr.timer=-2;
							}
							else
							{
								prog_error=true;
								error_msg="Сейф заблокирован. Вы не смогли ввести шифр.";
								error_id=4;
								safe_cr.timer=-3;
							}
						}						
					}

					Sleep(200);
				}
			}
			else if(IEvent[1].Object==55)
			{
				if(IEvent[1].Conditional==1)
				{
					if(safe_cr.Code6())
					{
						NextS();
					}
					else
					{
						OpenSStream("deny.wav");
						if(safe_cr.cn6)
						{
							if(safe_cr.tryes>0)
							{
								prog_error=true;
								error_msg="Попытка провалена! Попробуйте еще раз.";
								error_id=4;
								safe_cr.timer=-2;
							}
							else
							{
								prog_error=true;
								error_msg="Сейф заблокирован. Вы не смогли ввести шифр.";
								error_id=4;
								safe_cr.timer=-3;
							}
						}					
					}

					Sleep(200);
				}
			}
			else if(IEvent[1].Object==56)
			{
				if(IEvent[1].Conditional==1)
				{
					if(safe_cr.Code7())
					{
						NextS();

						prog_error=true;
						error_msg="Введен верный шифр. Замок открыт.";
						error_id=4;
						safe_cr.timer=-4;
					}
					else
					{
						OpenSStream("deny.wav");
						if(safe_cr.cn7)
						{
							if(safe_cr.tryes>0)
							{
								prog_error=true;
								error_msg="Попытка провалена! Попробуйте еще раз.";
								error_id=4;
								safe_cr.timer=-2;
							}
							else
							{
								prog_error=true;
								error_msg="Сейф заблокирован. Вы не смогли ввести шифр.";
								error_id=4;
								safe_cr.timer=-3;
							}
						}						
					}

					Sleep(200);
				}
			}
		}
		else
		{
		if(IEvent[1].Object==1)
		{
			if(IEvent[1].Conditional==1)
			{
				if(Interface[0].cur_y>=8)
					Interface[0].cur_y=Interface[0].cur_y-8;
				Sleep(25);
			}
			if(IEvent[1].Conditional==2)
			{
				if(Interface[0].cur_y>=16)
					Interface[0].cur_y=Interface[0].cur_y-8*3;
				else
					Interface[0].cur_y=0;

				if(Interface[0].cur_y<0)
					Interface[0].cur_y=0;

//				Sleep(5);
			}
		}
		else if(IEvent[1].Object==2)
		{
			if(IEvent[1].Conditional==1)
			{
				if(Interface[0].cur_y<Interface[0].rows*16-maintext_height+8)
					Interface[0].cur_y=Interface[0].cur_y+8;
				Sleep(25);
			}
			else if(IEvent[1].Conditional==2)
			{
				if(Interface[0].cur_y<Interface[0].rows*16-maintext_height+8)
					Interface[0].cur_y=Interface[0].cur_y+8*3;
				else
					Interface[0].cur_y=Interface[0].rows*16-maintext_height+8;
				
				if(Interface[0].cur_y>Interface[0].rows*16-maintext_height+8)
					Interface[0].cur_y=Interface[0].rows*16-maintext_height+8;

//				Sleep(5);
			}
		}
		else if(IEvent[1].Object==3)
		{
			if(IEvent[1].Conditional==1)
			{
				if(Interface[0].c_y>=2)
					Interface[0].c_y=Interface[0].c_y-2;
				Sleep(25);
			}
			if(IEvent[1].Conditional==2)
			{
				if(Interface[0].c_y>=2)
					Interface[0].c_y=Interface[0].c_y-2*3;
				if(Interface[0].c_y<0)
					Interface[0].c_y=0;

//				Sleep(25);
			}
		}
		else if(IEvent[1].Object==4)
		{
			if(IEvent[1].Conditional==1)
			{
				if(Interface[0].c_y<Interface[0].arheight-nexttext_height)
					Interface[0].c_y=Interface[0].c_y+2;
				Sleep(25);
			}
			if(IEvent[1].Conditional==2)
			{
				if(Interface[0].c_y<Interface[0].arheight-nexttext_height)
					Interface[0].c_y=Interface[0].c_y+2*3;
				if(Interface[0].c_y>Interface[0].arheight-nexttext_height)
					Interface[0].c_y=Interface[0].arheight-nexttext_height;

//				Sleep(25);
			}
		}
		else if(IEvent[1].Object==5)
		{
			if((IEvent[1].Conditional==1)&&(CheckCondition(Level[fr].condition1)))
			{
				CheckScript(Level[fr].script1);
				if(!changes)
					PrepareFrame(Level[fr].url1_id);
				Sleep(200);
			}
		}
		else if(IEvent[1].Object==6)
		{
			if((IEvent[1].Conditional==1)&&(CheckCondition(Level[fr].condition2)))
			{
				CheckScript(Level[fr].script2);
				if(!changes)
					PrepareFrame(Level[fr].url2_id);
				Sleep(200);
			}
		}
		else if((IEvent[1].Object==7)&&(CheckCondition(Level[fr].condition3)))
		{
			if(IEvent[1].Conditional==1)
			{
				CheckScript(Level[fr].script3);
				if(!changes)
					PrepareFrame(Level[fr].url3_id);
				Sleep(200);
			}
		}
		else if((IEvent[1].Object==8)&&(CheckCondition(Level[fr].condition4)))
		{
			if(IEvent[1].Conditional==1)
			{
				CheckScript(Level[fr].script4);
				if(!changes)
					PrepareFrame(Level[fr].url4_id);
				Sleep(200);
			}
		}
		else if(IEvent[1].Object==9)
		{
			if((IEvent[1].Conditional==1)&&(CheckCondition(Level[fr].condition5)))
			{
				CheckScript(Level[fr].script5);
				if(!changes)
					PrepareFrame(Level[fr].url5_id);
				Sleep(200);
			}
		}
		else if(IEvent[1].Object==18)
		{
			if(IEvent[1].Conditional==1)
			{
				Sleep(200);
				prog_exit=true;
			}
		}
		else if(IEvent[1].Object==17)
		{
			if(IEvent[1].Conditional==1)
			{
				Sleep(200);
				save=true;
			}
		}
		else if(IEvent[1].Object==16)
		{
			if(IEvent[1].Conditional==1)
			{
				Sleep(200);
				if(mode==2)
				{
					OpenSStream("deny.wav");
					prog_error=true;
					error_msg="Вы уже находитесь в режиме терминала.";
					error_id=1;
				}
				else
				{
					mode=2;
				}
			}
		}
		else if(IEvent[1].Object==15)
		{
			if(IEvent[1].Conditional==1)
			{
				Sleep(200);
				if(mode==3)
				{
					OpenSStream("deny.wav");
					prog_error=true;
					error_msg="Вы уже находитесь в режиме архива.";
					error_id=1;
				}
				else
				{
					Location_txt.Create(&Screen, NDXST_SYSMEM, 300, 20);
					Location_txt.Clear(Screen.PixelFormat->Rgb(0,0,0));
					Location_txt.SetColorKey(Screen.PixelFormat->Rgb(0,0,0));
					Location_txt.SetTransP(true);

					Location_txt.FormattedText(0,0,RGB(181, 149, 89),NULL,"Архивные записи:");

					//archiev.mode = 0;
					//archiev.PrepareFrame(&Screen);
					mode=3;
				}
			}
		}
		else if(IEvent[1].Object==10)
		{
			if(prog_exit)
			{
				if(IEvent[1].Conditional==1)
				{
					Sleep(200);
/*					fade=true;
					prog_exit=false;
					StopMStream();
					OpenMStream("data\\sound\\music\\menu.acm");
					SetMPosition();
					SetMVolume(1);
					PlayMStream();
					mode=1;*/

					NDX_Surface			Fon; //фон
					Fon.Create(&Screen,NDXST_SYSMEM,800, 600);
					
					Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
					
					for(int i=17; i>0; --i)
					{
						Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
						Screen.Flip();
						Sleep(60);
					}
					PostMessage(hWindow, WM_CLOSE, 0, 0L);
				}
			}

			if(prog_error)
			{
				if(IEvent[1].Conditional==1)
				{
					Sleep(200);
					prog_error=false;

					if(codecrack)
						safe_cr.Restart();
				}
			}
		}
		else if(IEvent[1].Object==11)
		{
			if(IEvent[1].Conditional==1)
			{
				Sleep(200);
				prog_exit=false;
			}
		}
		else if(IEvent[1].Object==20)
		{
			if(IEvent[1].Conditional==2)
			{
				Sleep(200);
				HitTarget(0);
//				AfxMessageBox("!!");
			}
		}
		else if(IEvent[1].Object==21)
		{
			if(IEvent[1].Conditional==2)
			{
				Sleep(200);
				HitTarget(1);
			}
		}
		else if(IEvent[1].Object==22)
		{
			if(IEvent[1].Conditional==2)
			{
				Sleep(200);
				HitTarget(2);
			}
		}
		else if(IEvent[1].Object==23)
		{
			if(IEvent[1].Conditional==2)
			{
				Sleep(200);
				HitTarget(3);
			}
		}
		else if(IEvent[1].Object==24)
		{
			if(IEvent[1].Conditional==2)
			{
				Sleep(200);
				HitTarget(4);
			}
		}
		else if(IEvent[1].Object==25)
		{
			if(IEvent[1].Conditional==2)
			{
				Sleep(200);
				HitTarget(5);
			}
		}
		else if(IEvent[1].Object==26)
		{
			if(IEvent[1].Conditional==2)
			{
				Sleep(200);
				HitTarget(6);
			}
		}
		else if(IEvent[1].Object==27)
		{
			if(IEvent[1].Conditional==2)
			{
				Sleep(200);
				HitTarget(7);
			}
		}
		else if(IEvent[1].Object==28)
		{
			if(IEvent[1].Conditional==1)
			{
				Sleep(200);
				OpenDataFile();
				Cursor.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("CURSOR/CURSOR_2.bmp"),true);
				Cursor.SetColorKey(Screen.PixelFormat->Rgb(255,255,0));
				Cursor.SetTransP(true);
				MD.Close();
				EndTurn();
			}
		}
		else if(IEvent[1].Object==30)
		{
			if(IEvent[1].Conditional==1)
			{
				Click();
				Sleep(200);
				if(fight)
				{
					if(pFight->CanDo(3))
					{
						pFight->player_cAP -= 3;
						plr_Inv.InitInventory();
						inv_mode=3;
						last_mode=mode;
						mode=4;
					}
					else
					{
						OpenSStream("deny.wav");
						battlelog.Insert(0, "Недостаточно очков действия. \n");
												
						WriteFLogText();
//						PrepareFightFrm();
					}					
				}
				else
				{
					plr_Inv.InitInventory();
					inv_mode=3;
					last_mode=mode;
					mode=4;
				}				
			}
		}
		else if(IEvent[1].Object==35)
		{
			if(IEvent[1].Conditional==1)
			{
				Click();
				Sleep(200);
				if((GetData("Rest")==0)&&(!fight))
				{
					ModifyData("PHealth", GetData("PHealth")+30);
					if(GetData("PHealth")>pFight->player_maxHP)
						ModifyData("PHealth", pFight->player_maxHP);
					ModifyData("Rest", 12000); // установим таймер

					DrawStats();

					NDX_Surface			Fon; //фон
					Fon.Create(&Screen,NDXST_SYSMEM,800, 600);
					
					Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
					
					for(int i=17; i>0; --i)
					{
						Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
						Screen.Flip();
						Sleep(30);
					}

					fade = true;
					prog_error=true;
					error_msg="Вы отдохнули и восстановили 30 единиц здоровья.";
					error_id=2;
				}
				else
				{
					OpenSStream("deny.wav");
					prog_error=true;
					error_msg="Вы недавно уже отдыхали.";
					error_id=3;
				}
			}
		}
		}

		changes=false;
	}
	else if(mode==3)
	{
		if(save)
		{
			if(IEvent[1].Object==4)
			{
				if(IEvent[1].Conditional==1)
				{
					Sleep(200);
					save=false;
				}
			}
			else if(IEvent[1].Object==3)
			{
				if(IEvent[1].Conditional==1)
				{
					Sleep(200);
					StopMStream();
					NDX_Surface			Fon; //фон
					Fon.Create(&Screen,NDXST_SYSMEM,800, 600);
					
					Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
					
					for(int i=17; i>0; --i)
					{
						Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
						Screen.Flip();
						Sleep(60);
					}
					OpenMStream("data\\sound\\music\\menu.acm");
					SetMVolume(1);
					PlayMStream();
					fade=true;
					mode=0;
					save=false;
					
				}
			}
			else if(IEvent[1].Object==2)
			{
				if((IEvent[1].Conditional==1)&&(!table_mark))
				{
					Sleep(200);
					NDX_Surface			Fon; //фон
					Fon.Create(&Screen,NDXST_SYSMEM,800, 600);
					
					Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
					
					for(int i=17; i>0; --i)
					{
						Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
						Screen.Flip();
						Sleep(60);
					}
					PrepareSaveload();
					fade=TRUE;
					last_mode=mode;
					mode=-2;
					save=false;
					
				}
			}
			else if(IEvent[1].Object==1)
			{
				if((IEvent[1].Conditional==1)&&(!table_mark))
				{
					Sleep(200);
					NDX_Surface			Fon; //фон
					Fon.Create(&Screen,NDXST_SYSMEM,800, 600);
					
					Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
					
					for(int i=17; i>0; --i)
					{
						Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
						Screen.Flip();
						Sleep(60);
					}
					PrepareSaveload();
					fade=TRUE;
					last_mode=mode;
					mode=-2;
					save=true;
					
				}
			}
		}
		
		if(IEvent[1].Object==15)
		{
			if(IEvent[1].Conditional==1)
			{
				Sleep(200);
				if(mode==3)
				{
					prog_error=true;
					error_msg="Вы уже находитесь в режиме архива.";
					error_id=1;
				}
				else
				{
					save=false;
					//archiev.mode = 0;
					//archiev.PrepareFrame(&Screen);
					mode=3;
				}
			}
		}
		else if(IEvent[1].Object==16)
		{
			if(IEvent[1].Conditional==1)
			{
				Sleep(200);
				if(mode==2)
				{
					prog_error=true;
					error_msg="Вы уже находитесь в режиме терминала.";
					error_id=1;
				}
				else
				{
					mode=2;
					save=false;
					PrepareFrame(fr);
				}
			}
		}
		else if(IEvent[1].Object==18)
		{
			if(IEvent[1].Conditional==1)
			{
				Sleep(200);
				prog_exit=true;
			}
		}
		else if(IEvent[1].Object==17)
		{
			if(IEvent[1].Conditional==1)
			{
				Sleep(200);
				save=true;
			}
		}
		else if(IEvent[1].Object==10)
		{
			if(prog_exit)
			{
				if(IEvent[1].Conditional==1)
				{
					Sleep(200);
					NDX_Surface			Fon; //фон
					Fon.Create(&Screen,NDXST_SYSMEM,800, 600);
					
					Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
					
					for(int i=17; i>0; --i)
					{
						Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
						Screen.Flip();
						Sleep(60);
					}
					PostMessage(hWindow, WM_CLOSE, 0, 0L);
				}
			}

			if(prog_error)
			{
				if(IEvent[1].Conditional==1)
				{
					Sleep(200);
					prog_error=false;
				}
			}
		}
		else if(IEvent[1].Object==11)
		{
			if(IEvent[1].Conditional==1)
			{
				Sleep(200);
				prog_exit=false;
			}
		}
		
	}
	else if(mode==4)
	{
		if(inv_mode==1)
		{
			if(IEvent[1].Object==1)
			{
				if(IEvent[1].Conditional==1)
				{
					Click();
					Sleep(200);
					if(drag==1)
					{
						int r=amount[0]*10000+amount[1]*1000+amount[2]*100+amount[3]*10+amount[4];

						if(r<obj_Inv.GetAmount())
							r++;

						amount[0]=r/10000;
						amount[1]=r/1000-amount[0]*10;
						amount[2]=r/100-(r/1000)*10;
						amount[3]=r/10-(r/100)*10;
						amount[4]=r-(r/10)*10;
					}
					else if(drag==-1)
					{
						int r=amount[0]*10000+amount[1]*1000+amount[2]*100+amount[3]*10+amount[4];

						if(r<plr_Inv.GetAmount())
							r++;

						amount[0]=r/10000;
						amount[1]=r/1000-amount[0]*10;
						amount[2]=r/100-(r/1000)*10;
						amount[3]=r/10-(r/100)*10;
						amount[4]=r-(r/10)*10;
					}
				}
			}
			else if(IEvent[1].Object==2)
			{
				if(IEvent[1].Conditional==1)
				{
					Click();
					Sleep(200);
					int r=amount[0]*10000+amount[1]*1000+amount[2]*100+amount[3]*10+amount[4];
					if(r>0)
						r--;

					amount[0]=r/10000;
					amount[1]=r/1000-amount[0]*10;
					amount[2]=r/100-(r/1000)*10;
					amount[3]=r/10-(r/100)*10;
					amount[4]=r-(r/10)*10;
				}
			}
			else if(IEvent[1].Object==3)
			{
				if(IEvent[1].Conditional==1)
				{
					Click();
					Sleep(200);
					int r=0;
					if(drag==1)
						r=obj_Inv.GetAmount();
					else if(drag==-1)
						r=plr_Inv.GetAmount();

					amount[0]=r/10000;
					amount[1]=r/1000-amount[0]*10;
					amount[2]=r/100-(r/1000)*10;
					amount[3]=r/10-(r/100)*10;
					amount[4]=r-(r/10)*10;
				}
			}
			else if(IEvent[1].Object==4)
			{
				if(IEvent[1].Conditional==1)
				{
					Click();
					Sleep(200);
					int r=amount[0]*10000+amount[1]*1000+amount[2]*100+amount[3]*10+amount[4];
					
					if(drag==1)
					{
						drag=0;
						plr_Inv.AddItem(r, obj_Inv.GetDescription(), &Screen);
						obj_Inv.GetItem(r, obj_Inv.GetDescription(), &Screen);
						obj_Inv.cur_itm=-1;
						
					}
					else if(drag==-1)
					{
						drag=0;
						obj_Inv.AddItem(r, plr_Inv.GetDescription(), &Screen);
						plr_Inv.GetItem(r, plr_Inv.GetDescription(), &Screen);
						plr_Inv.cur_itm=-1;
					}
					OpenSStream("IPUTDOWN.wav");
					inv_mode=0;
				}
			}
			else if(IEvent[1].Object==5)
			{
				if(IEvent[1].Conditional==1)
				{
					Click();
					Sleep(200);
					drag=0;
					inv_mode=0;
					OpenSStream("IPUTDOWN.wav");
				}
			}
		}
		
		if(IEvent[1].Object==30)
		{
			if(IEvent[1].Conditional==1)
			{
				Click();
				Sleep(200);
				inv_mode=0;
				mode=last_mode;
				cursor_mode=0;
			}
		}
		else if(IEvent[1].Object==32)
		{
			if(IEvent[1].Conditional==1)
			{
				Sleep(200);
				UseItem();
			}
		}
		else if(IEvent[1].Object==35)
		{
			if(IEvent[1].Conditional==1)
			{
				Sleep(200);
				OpenSStream("IPUTDOWN.wav");
				plr_Inv.DeleteItem(plr_Inv.GetDescription(), &Screen);
				plr_Inv.cur_itm=-1;
				prog_exit=false;
			}
		}
		else if(IEvent[1].Object==36)
		{
			if(IEvent[1].Conditional==1)
			{
				Sleep(200);
				prog_exit=false;
			}
		}
		if((IEvent[1].Object==80)&&(IEvent[1].Conditional==1))
		{
			Click();
			Sleep(200);
			if(obj_Inv.sc_y>0)
				obj_Inv.sc_y--;
		}
		else if((IEvent[1].Object==81)&&(IEvent[1].Conditional==1))
		{
			Click();
			Sleep(200);
			if(obj_Inv.sc_y < obj_Inv.m_Inventory.GetSize() - 6)
				obj_Inv.sc_y++;
		}
		else if((IEvent[1].Object==82)&&(IEvent[1].Conditional==1))
		{
			Click();
			Sleep(200);
			if(plr_Inv.sc_y>0)
				plr_Inv.sc_y--;
		}
		else if((IEvent[1].Object==83)&&(IEvent[1].Conditional==1))
		{
			Click();
			Sleep(200);
			if(plr_Inv.sc_y < plr_Inv.m_Inventory.GetSize() - 6)
				plr_Inv.sc_y++;
		}
	}
	else if(mode==5)
	{
		if(inv_mode==1)
		{
			if(IEvent[1].Object==1)
			{
				if(IEvent[1].Conditional==1)
				{
					Click();
					Sleep(200);
					int r=amount[0]*10000+amount[1]*1000+amount[2]*100+amount[3]*10+amount[4];
					if(drag==1)
					{
						if(r<obj_Inv.GetAmount())
							r++;
					}
					else if(drag==-1)
					{
						if(r<obj_Bar.GetAmount())
							r++;
					}
					else if(drag==2)
					{
						if(r<plr_Bar.GetAmount())
							r++;
					}
					else if(drag==-2)
					{
						if(r<plr_Inv.GetAmount())
							r++;
					}
					amount[0]=r/10000;
					amount[1]=r/1000-amount[0]*10;
					amount[2]=r/100-(r/1000)*10;
					amount[3]=r/10-(r/100)*10;
					amount[4]=r-(r/10)*10;
				}
			}
			else if(IEvent[1].Object==2)
			{
				if(IEvent[1].Conditional==1)
				{
					Click();
					Sleep(200);
					int r=amount[0]*10000+amount[1]*1000+amount[2]*100+amount[3]*10+amount[4];
					if(r>0)
						r--;

					amount[0]=r/10000;
					amount[1]=r/1000-amount[0]*10;
					amount[2]=r/100-(r/1000)*10;
					amount[3]=r/10-(r/100)*10;
					amount[4]=r-(r/10)*10;
				}
			}
			else if(IEvent[1].Object==3)
			{
				if(IEvent[1].Conditional==1)
				{
					Click();
					Sleep(200);
					int r=0;
					if(drag==1)
						r=obj_Inv.GetAmount();
					else if(drag==-1)
						r=obj_Bar.GetAmount();
					else if(drag==2)
						r=plr_Bar.GetAmount();
					else if(drag==-2)
						r=plr_Inv.GetAmount();

					amount[0]=r/10000;
					amount[1]=r/1000-amount[0]*10;
					amount[2]=r/100-(r/1000)*10;
					amount[3]=r/10-(r/100)*10;
					amount[4]=r-(r/10)*10;
				}
			}
			else if(IEvent[1].Object==4)
			{
				if(IEvent[1].Conditional==1)
				{
					Click();
					Sleep(200);
					int r=amount[0]*10000+amount[1]*1000+amount[2]*100+amount[3]*10+amount[4];
					
					if(drag==1)
					{
						if((r<=obj_Inv.GetAmount())&&(r>0))
						{
							drag=0;
							obj_Bar.AddItem(r, obj_Inv.GetDescription(), &Screen);
							obj_Inv.GetItem(r, obj_Inv.GetDescription(), &Screen);
							obj_Inv.cur_itm=-1;
						}						
					}
					else if(drag==-1)
					{
						if((r<=obj_Bar.GetAmount())&&(r>0))
						{
							drag=0;
							obj_Inv.AddItem(r, obj_Bar.GetDescription(), &Screen);
							obj_Bar.GetItem(r, obj_Bar.GetDescription(), &Screen);
							obj_Bar.cur_itm=-1;
						}
					}
					else if(drag==2)
					{
						if((r<=plr_Bar.GetAmount())&&(r>0))
						{
							drag=0;
							plr_Inv.AddItem(r, plr_Bar.GetDescription(), &Screen);
							plr_Bar.GetItem(r, plr_Bar.GetDescription(), &Screen);
							plr_Bar.cur_itm=-1;
						}
					}
					else if(drag==-2)
					{
						if((r<=plr_Inv.GetAmount())&&(r>0))
						{
							drag=0;
							plr_Bar.AddItem(r, plr_Inv.GetDescription(), &Screen);
							plr_Inv.GetItem(r, plr_Inv.GetDescription(), &Screen);
							plr_Inv.cur_itm=-1;
						}
					}
					OpenSStream("IPUTDOWN.wav");
					inv_mode=0;
				}
			}
			else if(IEvent[1].Object==5)
			{
				if(IEvent[1].Conditional==1)
				{
					Click();
					Sleep(200);
					drag=0;
					inv_mode=0;
					OpenSStream("IPUTDOWN.wav");
				}
			}
		}
		
		if(IEvent[1].Object==31)
		{
			if(IEvent[1].Conditional==1)
			{
				Click();
				Sleep(200);
				inv_mode=0;
				mode=last_mode;
				cursor_mode=0;
				for(int i=plr_Bar.m_Inventory.GetSize()-1; i>=0; i--)
				{
					plr_Inv.AddItem(plr_Bar.GetAmount(i), plr_Bar.GetDescription(i), &Screen);
					plr_Bar.GetItem(plr_Bar.GetAmount(i), plr_Bar.GetDescription(i), &Screen);
				}
				for(i=obj_Bar.m_Inventory.GetSize()-1; i>=0; i--)
				{
					obj_Inv.AddItem(obj_Bar.GetAmount(i), obj_Bar.GetDescription(i), &Screen);
					obj_Bar.GetItem(obj_Bar.GetAmount(i), obj_Bar.GetDescription(i), &Screen);						
				}
				plr_Bar.ClearAll("","",&Screen);
				obj_Bar.ClearAll("","",&Screen);
			}
		}
		if(IEvent[1].Object==30)
		{
			if(IEvent[1].Conditional==1)
			{
				Click();
				Sleep(200);
				if(plr_Bar.cost>=obj_Bar.cost)
				{
					for(int i=plr_Bar.m_Inventory.GetSize()-1; i>=0; i--)
					{
						obj_Inv.AddItem(plr_Bar.GetAmount(i), plr_Bar.GetDescription(i), &Screen);
						plr_Bar.GetItem(plr_Bar.GetAmount(i), plr_Bar.GetDescription(i), &Screen);
					}
					for(i=obj_Bar.m_Inventory.GetSize()-1; i>=0; i--)
					{
						plr_Inv.AddItem(obj_Bar.GetAmount(i), obj_Bar.GetDescription(i), &Screen);
						obj_Bar.GetItem(obj_Bar.GetAmount(i), obj_Bar.GetDescription(i), &Screen);						
					}
					plr_Bar.cur_itm=-1;
					obj_Bar.cur_itm=-1;
					OpenSStream("IPUTDOWN.wav");
				}
				else
					OpenSStream("deny.wav");
			}
		}
		if((IEvent[1].Object==80)&&(IEvent[1].Conditional==1))
		{
			Click();
			Sleep(200);
			if(obj_Inv.sc_y>0)
				obj_Inv.sc_y--;
		}
		else if((IEvent[1].Object==81)&&(IEvent[1].Conditional==1))
		{
			Click();
			Sleep(200);
			if(obj_Inv.sc_y < obj_Inv.m_Inventory.GetSize() - 5)
				obj_Inv.sc_y++;
		}
		else if((IEvent[1].Object==82)&&(IEvent[1].Conditional==1))
		{
			Click();
			Sleep(200);
			if(plr_Inv.sc_y>0)
				plr_Inv.sc_y--;
		}
		else if((IEvent[1].Object==83)&&(IEvent[1].Conditional==1))
		{
			Click();
			Sleep(200);
			if(plr_Inv.sc_y < plr_Inv.m_Inventory.GetSize() - 5)
				plr_Inv.sc_y++;
		}
		else if((IEvent[1].Object==84)&&(IEvent[1].Conditional==1))
		{
			Click();
			Sleep(200);
			if(obj_Bar.sc_y>0)
				obj_Bar.sc_y--;
		}
		else if((IEvent[1].Object==85)&&(IEvent[1].Conditional==1))
		{
			Click();
			Sleep(200);
			if(obj_Bar.sc_y < obj_Bar.m_Inventory.GetSize() - 4)
				obj_Bar.sc_y++;
		}
		else if((IEvent[1].Object==86)&&(IEvent[1].Conditional==1))
		{
			Click();
			Sleep(200);
			if(plr_Bar.sc_y>0)
				plr_Bar.sc_y--;
		}
		else if((IEvent[1].Object==87)&&(IEvent[1].Conditional==1))
		{
			Click();
			Sleep(200);
			if(plr_Bar.sc_y < plr_Bar.m_Inventory.GetSize() - 4)
				plr_Bar.sc_y++;
		}
	}
	else if(mode==-2)
	{
		if(IEvent[0].Object==1)
		{
			if(IEvent[0].Conditional==1)
			{
				Sleep(200);
				anim=1;
				PrepareSaveload(anim);
			}
			else if(IEvent[0].Conditional==2)
			{
				Sleep(200);
				if(save)
					SaveGame("save1.sav");
				else
					LoadGame("save1.sav");
			}
		}
		else if(IEvent[0].Object==2)
		{
			if(IEvent[0].Conditional==1)
			{
				Sleep(200);
				anim=2;
				PrepareSaveload(anim);
			}
			else if(IEvent[0].Conditional==2)
			{
				Sleep(200);
				if(save)
					SaveGame("save2.sav");
				else
					LoadGame("save2.sav");
			}
		}
		else if(IEvent[0].Object==3)
		{
			if(IEvent[0].Conditional==1)
			{
				Sleep(200);
				anim=3;
				PrepareSaveload(anim);
			}
			else if(IEvent[0].Conditional==2)
			{
				Sleep(200);
				if(save)
					SaveGame("save3.sav");
				else
					LoadGame("save3.sav");
			}
		}
		else if(IEvent[0].Object==4)
		{
			if(IEvent[0].Conditional==1)
			{
				Sleep(200);
				anim=4;
				PrepareSaveload(anim);
			}
			else if(IEvent[0].Conditional==2)
			{
				Sleep(200);
				if(save)
					SaveGame("save4.sav");
				else
					LoadGame("save4.sav");
			}
		}
		else if(IEvent[0].Object==5)
		{
			if(IEvent[0].Conditional==1)
			{
				Sleep(200);
				anim=5;
				PrepareSaveload(anim);
			}
			else if(IEvent[0].Conditional==2)
			{
				Sleep(200);
				if(save)
					SaveGame("save5.sav");
				else
					LoadGame("save5.sav");
			}
		}
		else if(IEvent[0].Object==6)
		{
			if(IEvent[0].Conditional==1)
			{
				Sleep(200);
				anim=6;
				PrepareSaveload(anim);
			}
			else if(IEvent[0].Conditional==2)
			{
				Sleep(200);
				if(save)
					SaveGame("save6.sav");
				else
					LoadGame("save6.sav");
			}
		}
		else if(IEvent[0].Object==7)
		{
			if(IEvent[0].Conditional==1)
			{
				Sleep(200);
				anim=7;
				PrepareSaveload(anim);
			}
			else if(IEvent[0].Conditional==2)
			{
				Sleep(200);
				if(save)
					SaveGame("save7.sav");
				else
					LoadGame("save7.sav");
			}
		}
		else if(IEvent[0].Object==8)
		{
			if(IEvent[0].Conditional==1)
			{
				Sleep(200);
				anim=8;
				PrepareSaveload(anim);
			}
			else if(IEvent[0].Conditional==2)
			{
				Sleep(200);
				if(save)
					SaveGame("save8.sav");
				else
					LoadGame("save8.sav");
			}
		}
		else if(IEvent[0].Object==9)
		{
			if(IEvent[0].Conditional==1)
			{
				Sleep(200);
				anim=9;
				PrepareSaveload(anim);
			}
			else if(IEvent[0].Conditional==2)
			{
				Sleep(200);
				if(save)
					SaveGame("save9.sav");
				else
					LoadGame("save9.sav");
			}
		}
		else if(IEvent[0].Object==10)
		{
			if(IEvent[0].Conditional==1)
			{
				Sleep(200);
				anim=10;
				PrepareSaveload(anim);
			}
			else if(IEvent[0].Conditional==2)
			{
				Sleep(200);
				if(save)
					SaveGame("save10.sav");
				else
					LoadGame("save10.sav");
			}
		}
		
		if(IEvent[1].Object==11)
		{
			if(IEvent[1].Conditional==1)
			{
				CString r="save";
				char num[10];
				itoa(anim,num,10);
				r+=num;
				r+=".sav";

				Sleep(200);
				if(save)
					SaveGame(r);
				else
					LoadGame(r);
			}
		}
		else if(IEvent[1].Object==12)
		{
			if(IEvent[1].Conditional==1)
			{
				Sleep(200);

				if((last_mode==2)||(last_mode==3))
					PrepareFrame(fr);

				NDX_Surface			Fon; //фон
				Fon.Create(&Screen,NDXST_SYSMEM,800, 600);
					
				Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
					
				for(int i=17; i>0; --i)
				{
					Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
					Screen.Flip();
					Sleep(30);
				}

				fade=true;
				mode=last_mode;
			}
		}
	}
}

void GameCircle()
{
	Main.Draw(Screen.Back,0,0,NDX_RECT(0,0,800,600));

	DrawOptions();

	if(mode==2)
	{
		//Main_txt.Draw(Screen.Back, maintext_x, maintext_y, NDX_RECT(0, 0+Interface[0].cur_y, maintext_width, maintext_height+Interface[0].cur_y));
		Main_txt.DrawFadeToBlack(Screen.Back, maintext_x, maintext_y, NDX_RECT(0, 0+Interface[0].cur_y, maintext_width, maintext_height+Interface[0].cur_y), 156+tbright);
		Next_txt.Draw(Screen.Back, nexttext_x, nexttext_y, NDX_RECT(0, 0+Interface[0].c_y, nexttext_width, nexttext_height+Interface[0].c_y));
	}
	else if(mode==3)
		PrepareArchiveFrm();/*archiev.ArchM.DrawFadeToBlack(Screen.Back,maintext_x, maintext_y, NDX_RECT(0, 0, maintext_width, maintext_height), 156+tbright);*/
	
	if((anim==1)&&(Interface[0].iAni))
		Image.Draw(Screen.Back, image_x, image_y, NDX_RECT(0, Interface[0].iFrame*100, 140, Interface[0].iFrame*100+100));
	else if((anim==2)&&(Interface[0].iAni)&&(!fight))
	{
		UINT an;
		an = rand() % 18;

		if(an != 3)
		{
			Image.Draw(Screen.Back, image_x, image_y, NDX_RECT(0, 0, 140, 100));
			if(table_mark)
				Temp.Draw(Screen.Back, image_x, image_y, NDX_RECT(0, 0, 140, 100));
		}
		else
		{
			Image.DrawColorMix(Screen.Back, image_x, image_y, NDX_RECT(0, 0, 140, 100), Screen.PixelFormat->Rgb(0,0,0), 0);
			if(table_mark)
				Temp.DrawColorMix(Screen.Back, image_x, image_y, NDX_RECT(0, 0, 140, 100), Screen.PixelFormat->Rgb(0,0,0), 0);
		}
		
		Noise.DrawTranslucent(Screen.Back, image_x, image_y, NDX_RECT(0, Interface[0].iFrame*100, 140, Interface[0].iFrame*100+100), 16+an);
	}
	else
	{
		if((fight)&&(pFight->targets>1))
		{
			TargetRdr.Draw(Screen.Back, 15, 57, NDX_RECT(0,0,178,113));
			int yp = Interface[0].iFrame*8+1;
			if(yp>113)
			{
				yp = 256 - Interface[0].iFrame*6;
				if(yp<=0)
					yp=0;
				Image.DrawTranslucent(Screen.Back, 15, 57, NDX_RECT(0, 0, 178, 113), yp);
			}
			else
			{
				Image.DrawTranslucent(Screen.Back, 15, 57+(Image.Height-yp), NDX_RECT(0, Image.Height-yp, Image.Width, Image.Height), 168);
			}
		}
		else
		{
			Image.Draw(Screen.Back, image_x, image_y, NDX_RECT(0, 0, 140, 100));
			if(table_mark)
				Temp.Draw(Screen.Back, image_x, image_y, NDX_RECT(0, 0, 140, 100));
		}
	}
	

	Pip_mnu.Draw(Screen.Back, pipmnu_x, pipmnu_y, NDX_RECT(0, 0, 125, 126));
	if(Interface[0].scroll==TRUE)
		Pip_srl.Draw(Screen.Back, pipsrl_x, pipsrl_y, NDX_RECT(0, 0, 11, 50));
	else
		Pip_srl.Draw(Screen.Back, pipsrl_x, pipsrl_y, NDX_RECT(22, 0, 33, 50));

//	if(!fight)
//	{
		if(Interface[0].scr==TRUE)
			Pip_srl.Draw(Screen.Back, nxtsrl_x, nxtsrl_y, NDX_RECT(0, 0, 11, 50));
		else
			Pip_srl.Draw(Screen.Back, nxtsrl_x, nxtsrl_y, NDX_RECT(22, 0, 33, 50));
//	}
	Pip_date.Draw(Screen.Back, 0, 0);
	Location_txt.Draw(Screen.Back, location_x, location_y);

	if(GetData("Inventory")!=0)
		InvEquip.Draw(Screen.Back, 686, 540, NDX_RECT(1, 473, 89, 505));

	if(Notify.on)
	{
		int t = Notify.period;
		if(Notify.period>256)
			t=256;
		Messages.DrawTranslucent(Screen.Back, 608, 54, NDX_RECT(0,0,137,14), t);
	}
	
	//обработка событий
	if(IEvent[0].Conditional!=0)
	{
		if(!fight)
		{
			Next_txt_ins.Clear(Screen.PixelFormat->Rgb(0,0,0));
			
			int ar1;
			if(Interface[0].ar1==0)
				ar1=0;
			else
				ar1=1;
			int ar2;
			if(Interface[0].ar2==0)
				ar2=0;
			else
				ar2=1;
			int ar3;
			if(Interface[0].ar3==0)
				ar3=0;
			else
				ar3=1;
			int ar4;
			if(Interface[0].ar4==0)
				ar4=0;
			else
				ar4=1;

			if((IEvent[0].Object==1)&&(Interface[0].scroll==TRUE))
			{
				if(IEvent[0].Conditional==1)
				{
					Pip_srl.Draw(Screen.Back, pipsrl_x, pipsrl_y, NDX_RECT(11, 0, 22, 10));
					IEvent[1].Object=1;
					IEvent[1].Conditional=1;
				}
				else if(IEvent[0].Conditional==2)
				{
					Pip_srl.Draw(Screen.Back, pipsrl_x, pipsrl_y, NDX_RECT(11, 0, 22, 10));
					IEvent[1].Object=1;
					IEvent[1].Conditional=2;
				}
			}
			else if((IEvent[0].Object==2)&&(Interface[0].scroll==TRUE))
			{
				if(IEvent[0].Conditional==1)
				{
					Pip_srl.Draw(Screen.Back, pipsrl_x, pipsrl_y+40, NDX_RECT(11, 40, 22, 50));
					IEvent[1].Object=2;
					IEvent[1].Conditional=1;
				}
				else if(IEvent[0].Conditional==2)
				{
					Pip_srl.Draw(Screen.Back, pipsrl_x, pipsrl_y+40, NDX_RECT(11, 40, 22, 50));
					IEvent[1].Object=2;
					IEvent[1].Conditional=2;
				}
			}
			else if(IEvent[0].Object==5)
			{
				if(IEvent[0].Conditional==1)
				{
					Next_txt.DrawColorMix(&Next_txt_ins, 0, 0, NDX_RECT(0, 0, nexttext_width, Interface[0].ar1*12+8), Screen.PixelFormat->Rgb(156, 146, 99), 150);
				}
				else if(IEvent[0].Conditional==2)
				{
					Next_txt.DrawFadeToBlack(&Next_txt_ins, 0, 0, NDX_RECT(0, 0, nexttext_width, Interface[0].ar1*12+8), 100);
					IEvent[1].Object=5;
					IEvent[1].Conditional=1;
				}
			}
			else if(IEvent[0].Object==6)
			{
				if(IEvent[0].Conditional==1)
				{
					Next_txt.DrawColorMix(&Next_txt_ins, 0, 16*ar1+Interface[0].ar1*12, NDX_RECT(0, 16*ar1+Interface[0].ar1*12, nexttext_width, 16*ar1+Interface[0].ar1*12+Interface[0].ar2*12+8), Screen.PixelFormat->Rgb(156, 146, 99), 150);
				}
				else if(IEvent[0].Conditional==2)
				{
					Next_txt.DrawFadeToBlack(&Next_txt_ins, 0, 16*ar1+Interface[0].ar1*12, NDX_RECT(0, 16*ar1+Interface[0].ar1*12, nexttext_width, 16*ar1+Interface[0].ar1*12+Interface[0].ar2*12+8), 100);
					IEvent[1].Object=6;
					IEvent[1].Conditional=1;
				}
			}
			else if(IEvent[0].Object==7)
			{
				if(IEvent[0].Conditional==1)
				{
					Next_txt.DrawColorMix(&Next_txt_ins, 0, 16*ar1+Interface[0].ar1*12+16*ar2+Interface[0].ar2*12, NDX_RECT(0, 16*ar1+Interface[0].ar1*12+16*ar2+Interface[0].ar2*12, nexttext_width, 16*ar1+Interface[0].ar1*12+Interface[0].ar2*12+16*ar2+Interface[0].ar3*12+8), Screen.PixelFormat->Rgb(156, 146, 99), 150);
				}
				else if(IEvent[0].Conditional==2)
				{
					Next_txt.DrawFadeToBlack(&Next_txt_ins, 0, 16*ar1+Interface[0].ar1*12+16*ar2+Interface[0].ar2*12, NDX_RECT(0, 16*ar1+Interface[0].ar1*12+16*ar2+Interface[0].ar2*12, nexttext_width, 16*ar1+Interface[0].ar1*12+Interface[0].ar2*12+16*ar2+Interface[0].ar3*12+8), 100);
					IEvent[1].Object=7;
					IEvent[1].Conditional=1;
				}
			}
			else if(IEvent[0].Object==8)
			{
				if(IEvent[0].Conditional==1)
				{
					Next_txt.DrawColorMix(&Next_txt_ins, 0, 16*ar1+Interface[0].ar1*12+16*ar2+Interface[0].ar2*12+16*ar3+Interface[0].ar3*12, NDX_RECT(0, 16*ar1+Interface[0].ar1*12+16*ar2+Interface[0].ar2*12+16*ar3+Interface[0].ar3*12, nexttext_width, 16*ar1+Interface[0].ar1*12+Interface[0].ar2*12+16*ar2+Interface[0].ar3*12+16*ar3+Interface[0].ar4*12+8), Screen.PixelFormat->Rgb(156, 146, 99), 150);
				}
				else if(IEvent[0].Conditional==2)
				{
					Next_txt.DrawFadeToBlack(&Next_txt_ins, 0, 16*ar1+Interface[0].ar1*12+16*ar2+Interface[0].ar2*12+16*ar3+Interface[0].ar3*12, NDX_RECT(0, 16*ar1+Interface[0].ar1*12+16*ar2+Interface[0].ar2*12+16*ar3+Interface[0].ar3*12, nexttext_width, 16*ar1+Interface[0].ar1*12+Interface[0].ar2*12+16*ar2+Interface[0].ar3*12+16*ar3+Interface[0].ar4*12+8), 100);
					IEvent[1].Object=8;
					IEvent[1].Conditional=1;
				}
			}
			else if(IEvent[0].Object==9)
			{
				if(IEvent[0].Conditional==1)
				{
					Next_txt.DrawColorMix(&Next_txt_ins, 0, 16*ar1+Interface[0].ar1*12+16*ar2+Interface[0].ar2*12+16*ar3+Interface[0].ar3*12+16*ar4+Interface[0].ar4*12, NDX_RECT(0, 16*ar1+Interface[0].ar1*12+16*ar2+Interface[0].ar2*12+16*ar3+Interface[0].ar3*12+16*ar4+Interface[0].ar4*12, nexttext_width, 16*ar1+Interface[0].ar1*12+Interface[0].ar2*12+16*ar2+Interface[0].ar3*12+16*ar3+Interface[0].ar4*12+16*ar4+Interface[0].ar5*12+8), Screen.PixelFormat->Rgb(156, 146, 99), 150);
				}
				else if(IEvent[0].Conditional==2)
				{
					Next_txt.DrawFadeToBlack(&Next_txt_ins, 0, 16*ar1+Interface[0].ar1*12+16*ar2+Interface[0].ar2*12+16*ar3+Interface[0].ar3*12+16*ar4+Interface[0].ar4*12, NDX_RECT(0, 16*ar1+Interface[0].ar1*12+16*ar2+Interface[0].ar2*12+16*ar3+Interface[0].ar3*12+16*ar4+Interface[0].ar4*12, nexttext_width, 16*ar1+Interface[0].ar1*12+Interface[0].ar2*12+16*ar2+Interface[0].ar3*12+16*ar3+Interface[0].ar4*12+16*ar4+Interface[0].ar5*12+8), 100);
					IEvent[1].Object=9;
					IEvent[1].Conditional=1;
				}
			}
			else if(IEvent[0].Object==18)
			{
				if(IEvent[0].Conditional==1)
				{
					Pip_mnu.Draw(Screen.Back, pipmnu_x, pipmnu_y+102, NDX_RECT(125, 102, 250, 126));
					Click();
					IEvent[1].Object=18;
					IEvent[1].Conditional=1;
				}
			}
			else if(IEvent[0].Object==17)
			{
				if(IEvent[0].Conditional==1)
				{
					Pip_mnu.Draw(Screen.Back, pipmnu_x, pipmnu_y+68, NDX_RECT(125, 68, 250, 92));
					Click();
					IEvent[1].Object=17;
					IEvent[1].Conditional=1;
				}
			}
			else if(IEvent[0].Object==16)
			{
				if(IEvent[0].Conditional==1)
				{
					Pip_mnu.Draw(Screen.Back, pipmnu_x, pipmnu_y+34, NDX_RECT(125, 34, 250, 58));
					Click();
					IEvent[1].Object=16;
					IEvent[1].Conditional=1;
				}
			}
			else if(IEvent[0].Object==15)
			{
				if(IEvent[0].Conditional==1)
				{
					Pip_mnu.Draw(Screen.Back, pipmnu_x, pipmnu_y, NDX_RECT(125, 0, 250, 24));
					Click();
					IEvent[1].Object=15;
					IEvent[1].Conditional=1;
				}
			}
		}
		
		if((IEvent[0].Object==3)&&(Interface[0].scr==TRUE))
		{
			if(IEvent[0].Conditional==1)
			{
				Pip_srl.Draw(Screen.Back, nxtsrl_x, nxtsrl_y, NDX_RECT(11, 0, 22, 10));
				IEvent[1].Object=3;
				IEvent[1].Conditional=1;
			}
			else if(IEvent[0].Conditional==2)
			{
				Pip_srl.Draw(Screen.Back, nxtsrl_x, nxtsrl_y, NDX_RECT(11, 0, 22, 10));
				IEvent[1].Object=3;
				IEvent[1].Conditional=2;
			}
		}
		else if((IEvent[0].Object==4)&&(Interface[0].scr==TRUE))
		{
			if(IEvent[0].Conditional==1)
			{
				Pip_srl.Draw(Screen.Back, nxtsrl_x, nxtsrl_y+40, NDX_RECT(11, 40, 22, 50));
				IEvent[1].Object=4;
				IEvent[1].Conditional=1;
			}
			else if(IEvent[0].Conditional==2)
			{
				Pip_srl.Draw(Screen.Back, nxtsrl_x, nxtsrl_y+40, NDX_RECT(11, 40, 22, 50));
				IEvent[1].Object=4;
				IEvent[1].Conditional=2;
			}
		}
	
		if(!fight)
			Next_txt_ins.Draw(Screen.Back, nexttext_x, nexttext_y, NDX_RECT(0, 0+Interface[0].c_y, nexttext_width, nexttext_height+Interface[0].c_y));
	}

	if(((save)&&(mode==2))||((save)&&(mode==3)))
	{
		Mnu_panel.Draw(Screen.Back, 242, 120);

		Buttons.Draw(Screen.Back,280,117,NDX_RECT(0,300,240,600));

		if(IEvent[0].Object==1)
		{
			if(IEvent[0].Conditional==1)
			{
				Click();
				Buttons.Draw(Screen.Back,280,182,NDX_RECT(240,365,480,405));
				IEvent[1].Object=1;
				IEvent[1].Conditional=1;
			}
		}
		else if(IEvent[0].Object==2)
		{
			if(IEvent[0].Conditional==1)
			{
				Click();
				Buttons.Draw(Screen.Back,280,247,NDX_RECT(240,430,480,470));
				IEvent[1].Object=2;
				IEvent[1].Conditional=1;
			}
		}
		else if(IEvent[0].Object==3)
		{
			if(IEvent[0].Conditional==1)
			{
				Click();
				Buttons.Draw(Screen.Back,280,312,NDX_RECT(240,495,480,535));
				IEvent[1].Object=3;
				IEvent[1].Conditional=1;
			}
		}
		else if(IEvent[0].Object==4)
		{
			if(IEvent[0].Conditional==1)
			{
				Click();
				Buttons.Draw(Screen.Back,280,377,NDX_RECT(240,560,480,600));
				IEvent[1].Object=4;
				IEvent[1].Conditional=1;
			}
		}
	}

	if(fight)
	{
		PrepareFightFrm();
	}
	else
		Stats.Draw(Screen.Back, 31, 487);	// нарисуем наши статы

	if(codecrack)
		ShowSafeCode();

	// нажата ли кнопка инвентаря
	if(IEvent[0].Object==30)
	{
		if(IEvent[0].Conditional==1)
		{
			InvEquip.Draw(Screen.Back, 686, 540, NDX_RECT(89, 473, 177, 505));
			IEvent[1].Object=30;
			IEvent[1].Conditional=1;
		}
	}

	if(prog_exit)
	{
		Error.Draw(Screen.Back, 201, 200);
		Misc_but.Draw(Screen.Back, 246, 360, NDX_RECT(0, 0, 125, 24));
		Misc_but.Draw(Screen.Back, 421, 360, NDX_RECT(0, 24, 125, 48));
		Screen.Back->FormattedText(281,260,RGB(207, 191, 63),NULL,"Подтвердите операцию выхода...");

		if(IEvent[0].Conditional!=0)
		{
			if(IEvent[0].Object==10)
			{
				if(IEvent[0].Conditional==1)
				{
					Misc_but.Draw(Screen.Back,246,360,NDX_RECT(125,0,250,24));
					Click();
					IEvent[1].Object=10;
					IEvent[1].Conditional=1;
				}
			}
			else if(IEvent[0].Object==11)
			{
				if(IEvent[0].Conditional==1)
				{
					Misc_but.Draw(Screen.Back,421,360,NDX_RECT(125,24,250,48));
					Click();
					IEvent[1].Object=11;
					IEvent[1].Conditional=1;
				}
			}
		}
	}

	if(prog_error)
	{
		Error.Draw(Screen.Back, 201, 200);
		Misc_but.Draw(Screen.Back, 340, 360, NDX_RECT(0, 48, 125, 72));
		if(error_id==1)
			Screen.Back->FormattedText(260,260,RGB(207, 191, 63),NULL,error_msg);
		else if(error_id==2)
			Screen.Back->FormattedText(227,260,RGB(207, 191, 63),NULL,error_msg);
		else if(error_id==3)
		{
			Screen.Back->FormattedText(310,260,RGB(207, 191, 63),NULL,error_msg);
			int k = GetData("Rest") * 150 / 60000;
			Screen.Back->FormattedText(265,280,RGB(207, 191, 63),NULL,"Вы сможете передохнуть через %u минут.", k);
		}
		else if(error_id==4)
			Screen.Back->FormattedText(260,260,RGB(207, 191, 63),NULL,error_msg);

		if(IEvent[0].Conditional!=0)
		{
			if(IEvent[0].Object==10)
			{
				if(IEvent[0].Conditional==1)
				{
					Misc_but.Draw(Screen.Back,340,360,NDX_RECT(125,48,250,72));
					Click();
					IEvent[1].Object=10;
					IEvent[1].Conditional=1;
				}
			}			
		}
	}

	if(fade==TRUE)
	{
		NDX_Surface			Fon; //фон

		Fon.Create(&Screen,NDXST_SYSMEM,800, 600);

		Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
	
		for(int i=0; i<17; ++i)
		{
			Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
			Screen.Flip();
			Sleep(10);
		}
	
		fade=false;
	}

	// курсор мышки
	if(!showcursor)
		Cursor.Draw(Screen.Back,MouseX,MouseY);	
}

void LoadScenario()
{
	CFile file;
	

	BOOL opened = file.Open(datnf, CFile::modeRead);

	if (opened)
	{
		CArchive ar(&file, CArchive::load);

		DWORD ext=0;
		ar >> ext;

		if(ext!=0x4e4353)
		{
			ShowCursor(true);
			MessageBox(hWindow, "Can't open scenario file!\nGame data corrupted.\n\nPlease reinstall game.", "Error", MB_OK);
			PostMessage(hWindow, WM_CLOSE, 0, 0L);
		}
		else
		{
			for (UINT i=0; i<1000; ++i)
			{
				ar >> Level[i].image;
				ar >> Level[i].year;
				ar >> Level[i].month;
				ar >> Level[i].day;
				ar >> Level[i].location;
				ar >> Level[i].sublocation;
				ar >> Level[i].sublink;
				ar >> Level[i].text_data;
				ar >> Level[i].url1;
				ar >> Level[i].url1_id;
				ar >> Level[i].condition1;
				ar >> Level[i].script1;
				ar >> Level[i].url2;
				ar >> Level[i].url2_id;
				ar >> Level[i].condition2;
				ar >> Level[i].script2;
				ar >> Level[i].url3;
				ar >> Level[i].url3_id;
				ar >> Level[i].condition3;
				ar >> Level[i].script3;
				ar >> Level[i].url4;
				ar >> Level[i].url4_id;
				ar >> Level[i].condition4;
				ar >> Level[i].script4;
				ar >> Level[i].url5;
				ar >> Level[i].url5_id;
				ar >> Level[i].condition5;
				ar >> Level[i].script5;
				ar >> Level[i].mp3fn;
				ar >> Level[i].event;
			}
		}

		ar.Close();
		file.Close();
	}
}

void WriteText(int frame)
{
	int i = Level[frame].text_data.GetLength();
	int sn=0;
	int rn=1;
	int space=0;
	int lsp = 0;

	for(int z=0; z<i; ++z)
	{
		if(Level[frame].text_data.GetAt(z)==' ')
		{
			space=z+1;
		}
			
		if(sn*12>maintext_width)
		{
			sn=0;
			z=space;
			DrawString(lsp,space,rn, frame);
			rn=rn+1;
			lsp=space;

		}
		else if(Level[frame].text_data.GetAt(z)=='\n')
		{
			sn=0;
			DrawString(lsp, z-1, rn, frame);
			rn=rn+1;
			z=z+1;
			lsp=z;
		}
		else if(z==i-1)
			DrawString(lsp, z+1, rn, frame);

		sn=sn+1;
	}
}

void WriteText()
{
	int i = battlelog.GetLength();
	int sn=0;
	int rn=1;
	int space=0;
	int lsp = 0;

	for(int z=0; z<i; ++z)
	{
		if(battlelog.GetAt(z)==' ')
		{
			space=z+1;
		}
			
		if(sn*12>342)
		{
			sn=0;
			z=space;
			DrawString(lsp,space-1,rn);
			rn=rn+1;
			lsp=space;

		}
		else if(battlelog.GetAt(z)=='\n')
		{
			sn=0;
			if(lsp!=z-1)
				DrawString(lsp, z-1, rn);
			rn++;
			z++;
			lsp=z;
		}
		else if(z==i-1)
			DrawString(lsp, z+1, rn);

		sn=sn+1;
	}
}

void DrawString(int lsp, int space, int rn, int frame)
{
//	setlocale( LC_ALL, "Russian.1251");

	CString str;
	LPSTR dstr;

	Puncts.SetClip(&Main_txt);

	int curxpos = 0; // текущая позиция печатоемого символа
	int h; //
	int w; // параметры для получения размера символа

	for(int i=lsp; i<space; ++i)
	{
		if((isupper(toascii(Level[frame].text_data.GetAt(i))) 
			|| (Level[frame].text_data.GetAt(i) =='А') 
			|| (Level[frame].text_data.GetAt(i) =='Э')
			|| (Level[frame].text_data.GetAt(i) =='Ю')
			|| (Level[frame].text_data.GetAt(i) =='Ь')
			|| (Level[frame].text_data.GetAt(i) =='Ъ')
			|| (Level[frame].text_data.GetAt(i) =='Я')))
		{
			str = Level[frame].text_data.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			AFont.Text(&Main_txt, curxpos, rn*16-4, Screen.PixelFormat->Rgb(170, 154, 19), dstr);
			AFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
		else if((Level[frame].text_data.GetAt(i)=='1')
			|| (Level[frame].text_data.GetAt(i)=='2')
			|| (Level[frame].text_data.GetAt(i)=='3')
			|| (Level[frame].text_data.GetAt(i)=='4')
			|| (Level[frame].text_data.GetAt(i)=='5')
			|| (Level[frame].text_data.GetAt(i)=='6')
			|| (Level[frame].text_data.GetAt(i)=='7')
			|| (Level[frame].text_data.GetAt(i)=='8')
			|| (Level[frame].text_data.GetAt(i)=='9')
			|| (Level[frame].text_data.GetAt(i)=='0'))
		{
			str = Level[frame].text_data.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			AFont.Text(&Main_txt, curxpos, rn*16-4, Screen.PixelFormat->Rgb(170, 154, 19), dstr);			
			AAFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
		else if(Level[frame].text_data.GetAt(i)==',')
		{
			str = Level[frame].text_data.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			Puncts.Draw(&Main_txt, curxpos, rn*16, NDX_RECT(0, 0, 5, 16));
			
			AAFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
		else if(Level[frame].text_data.GetAt(i)==':')
		{
			str = Level[frame].text_data.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			Puncts.Draw(&Main_txt, curxpos, rn*16, NDX_RECT(5, 0, 10, 16));
			
			AAFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
		else if(Level[frame].text_data.GetAt(i)==';')
		{
			str = Level[frame].text_data.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			Puncts.Draw(&Main_txt, curxpos, rn*16, NDX_RECT(10, 0, 15, 16));
			
			AAFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
		else if(Level[frame].text_data.GetAt(i)=='!')
		{
			str = Level[frame].text_data.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			AFont.Text(&Main_txt, curxpos, rn*16-4, Screen.PixelFormat->Rgb(207, 191, 63), dstr);
			AFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
		else if(Level[frame].text_data.GetAt(i)=='"')
		{
			str = Level[frame].text_data.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			AFont.Text(&Main_txt, curxpos, rn*16-4, Screen.PixelFormat->Rgb(207, 191, 63), dstr);
			AFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
		else if(Level[frame].text_data.GetAt(i)=='?')
		{
			str = Level[frame].text_data.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			AFont.Text(&Main_txt, curxpos, rn*16-4, Screen.PixelFormat->Rgb(207, 191, 63), dstr);
			AFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
		else if(Level[frame].text_data.GetAt(i)=='(')
		{
			str = Level[frame].text_data.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			Puncts.Draw(&Main_txt, curxpos+2, rn*16-2, NDX_RECT(17, 0, 24, 16));
			
			AAFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
		else if(Level[frame].text_data.GetAt(i)==')')
		{
			str = Level[frame].text_data.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			Puncts.Draw(&Main_txt, curxpos, rn*16-2, NDX_RECT(25, 0, 32, 16));
			
			AAFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
		else if(Level[frame].text_data.GetAt(i)=='[')
		{
			think=true;
			str = Level[frame].text_data.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			Puncts.Draw(&Main_txt, curxpos+2, rn*16-2, NDX_RECT(17, 0, 24, 16));
			
			AAFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
		else if(Level[frame].text_data.GetAt(i)==']')
		{
			think=false;
			str = Level[frame].text_data.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			Puncts.Draw(&Main_txt, curxpos, rn*16-2, NDX_RECT(25, 0, 32, 16));
			
			AAFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
		else if(Level[frame].text_data.GetAt(i)=='й')
		{
			str = Level[frame].text_data.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			if(think)
				AAFont.Text(&Main_txt, curxpos, rn*16, Screen.PixelFormat->Rgb(181,177,86), "и");
			else
				AAFont.Text(&Main_txt, curxpos, rn*16, Screen.PixelFormat->Rgb(207, 191, 63), "и");
			Puncts.Draw(&Main_txt, curxpos+1, rn*16-2, NDX_RECT(32, 0, 41, 3));
			
			AAFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
		else
		{
			str = Level[frame].text_data.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			if(think)
				AAFont.Text(&Main_txt, curxpos, rn*16, Screen.PixelFormat->Rgb(/*233, 229, 147 233,220,89 231,217,83 222,194,21 170, 154, 19 171,166,72*/181,177,86), dstr);
			else
				AAFont.Text(&Main_txt, curxpos, rn*16, Screen.PixelFormat->Rgb(207, 191, 63), dstr);

			AAFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
//			str+=Level[frame].text_data.GetAt(i);
	}

/*	int l=str.GetLength();

	dstr = str.GetBuffer(l);
	str.ReleaseBuffer();


	int h;
	int w;

	AAFont.GetTextSize(&w, &h, dstr);

	AAFont.Text(&Main_txt, 0, rn*16, Screen.PixelFormat->Rgb(207, 191, 63), dstr);*/
	
//	DrawTextString(str, rn*16, 0);
}

void DrawString(int lsp, int space, int rn)
{
//	setlocale( LC_ALL, "Russian.1251");

	CString str;
	LPSTR dstr;

	Puncts.SetClip(&Main_txt);

	int curxpos = 0; // текущая позиция печатоемого символа
	int h; //
	int w; // параметры для получения размера символа

	for(int i=lsp; i<=space; ++i)
	{
		if((isupper(toascii(battlelog.GetAt(i))) 
			|| (battlelog.GetAt(i) =='А') 
			|| (battlelog.GetAt(i) =='Э')
			|| (battlelog.GetAt(i) =='Ю')
			|| (battlelog.GetAt(i) =='Ь')
			|| (battlelog.GetAt(i) =='Ъ')
			|| (battlelog.GetAt(i) =='Я')))
		{
			str = battlelog.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			AFont.Text(&Main_txt, curxpos, rn*16-4, Screen.PixelFormat->Rgb(170, 154, 19), dstr);
			AFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
		else if(battlelog.GetAt(i)==',')
		{
			str = battlelog.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			Puncts.Draw(&Main_txt, curxpos, rn*16, NDX_RECT(0, 0, 5, 16));
			
			AAFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
		else if(battlelog.GetAt(i)==':')
		{
			str = battlelog.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			Puncts.Draw(&Main_txt, curxpos, rn*16, NDX_RECT(5, 0, 10, 16));
			
			AAFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
		else if(battlelog.GetAt(i)==';')
		{
			str = battlelog.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			Puncts.Draw(&Main_txt, curxpos, rn*16, NDX_RECT(10, 0, 15, 16));
			
			AAFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
		else if(battlelog.GetAt(i)=='!')
		{
			str = battlelog.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			AFont.Text(&Main_txt, curxpos, rn*16-4, Screen.PixelFormat->Rgb(207, 191, 63), dstr);
			AFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
		else if(battlelog.GetAt(i)=='"')
		{
			str = battlelog.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			AFont.Text(&Main_txt, curxpos, rn*16-4, Screen.PixelFormat->Rgb(207, 191, 63), dstr);
			AFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
		else if(battlelog.GetAt(i)=='?')
		{
			str = battlelog.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			AFont.Text(&Main_txt, curxpos, rn*16-4, Screen.PixelFormat->Rgb(207, 191, 63), dstr);
			AFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
		else if(battlelog.GetAt(i)=='(')
		{
			str = battlelog.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			Puncts.Draw(&Main_txt, curxpos+2, rn*16-2, NDX_RECT(17, 0, 24, 16));
			
			AAFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
		else if(battlelog.GetAt(i)==')')
		{
			str = battlelog.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			Puncts.Draw(&Main_txt, curxpos, rn*16-2, NDX_RECT(25, 0, 32, 16));
			
			AAFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
		else if(battlelog.GetAt(i)=='[')
		{
			str = battlelog.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			Puncts.Draw(&Main_txt, curxpos+2, rn*16-2, NDX_RECT(17, 0, 24, 16));
			
			AAFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
		else if(battlelog.GetAt(i)==']')
		{
			str = battlelog.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			Puncts.Draw(&Main_txt, curxpos, rn*16-2, NDX_RECT(25, 0, 32, 16));
			
			AAFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
	/*	else if(battlelog.GetAt(i)=='(')
		{
			str = battlelog.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			AFont.Text(&Main_txt, curxpos, rn*16-4, Screen.PixelFormat->Rgb(170, 154, 19), dstr);
			AFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
		else if(battlelog.GetAt(i)==')')
		{
			str = battlelog.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			AFont.Text(&Main_txt, curxpos, rn*16-4, Screen.PixelFormat->Rgb(170, 154, 19), dstr);
			AFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
		else if(battlelog.GetAt(i)=='[')
		{
			str = battlelog.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			AFont.Text(&Main_txt, curxpos, rn*16-4, Screen.PixelFormat->Rgb(170, 154, 19), dstr);
			AFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
		else if(battlelog.GetAt(i)==']')
		{
			str = battlelog.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			AFont.Text(&Main_txt, curxpos, rn*16-4, Screen.PixelFormat->Rgb(170, 154, 19), dstr);
			AFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}*/
		else
		{
			str = battlelog.GetAt(i);
			int l=str.GetLength();
			dstr = str.GetBuffer(l);
			str.ReleaseBuffer();

			AAFont.Text(&Main_txt, curxpos, rn*16, Screen.PixelFormat->Rgb(207, 191, 63), dstr);

			AAFont.GetTextSize(&w, &h, dstr);
			curxpos+=w;
		}
//			str+=Level[frame].text_data.GetAt(i);
	}

/*	int l=str.GetLength();

	dstr = str.GetBuffer(l);
	str.ReleaseBuffer();


	int h;
	int w;

	AAFont.GetTextSize(&w, &h, dstr);

	AAFont.Text(&Main_txt, 0, rn*16, Screen.PixelFormat->Rgb(207, 191, 63), dstr);*/
	
//	DrawTextString(str, rn*16, 0);
}

void DrawTextString(CString string, int y, int font)
{
	
}

void PrepareFrame(int frame)
{
	if(frame!=0)
		NextS();

	fr=frame;

	Interface[0].scroll=false;
	Interface[0].scr=false;
	Interface[0].ar1=0;
	Interface[0].ar2=0;
	Interface[0].ar3=0;
	Interface[0].ar4=0;
	Interface[0].ar5=0;
	Interface[0].c_y=0;
	Interface[0].iFrame=0;
	Interface[0].iAni=0;
	Interface[0].cur_y=0; 

	anim=0;
	
	int i = Level[frame].text_data.GetLength();
	int rows=1;
	int sn=0;
	int space=0;
	int lsp=0;

	int height=32;

	if(!show_image)
	{
		//вычисляем размер текста	
		for(int z=0; z<i; ++z)
		{
			if(Level[frame].text_data.GetAt(z)==' ')
			{
//				lsp=space;
				space=z+1;
			}
			
			if(sn*12>maintext_width)
			{
				rows=rows+1;
				sn=0;
				z=space;
				lsp=space;
			}
			else if(Level[frame].text_data.GetAt(z)=='\n')
			{
				rows=rows+1;
				sn=0;
				z=z+1;
				lsp=z;
			}
			sn=sn+1;
		}
		
		height=32;
		
		if(rows*16<maintext_height)
			height+=maintext_height;
		else 
			height+=rows*16;
		
		Main_txt.Create(&Screen, NDXST_SYSMEM, maintext_width, height);
		Main_txt.Clear(Screen.PixelFormat->Rgb(0,0,0));
		Main_txt.SetColorKey(Screen.PixelFormat->Rgb(0,0,0));
		Main_txt.SetTransP(true);

		Interface[0].rows = rows;
		if(rows*16>maintext_height)
			Interface[0].scroll=TRUE;
		else
			Interface[0].scroll=FALSE;
		
		Interface[0].cur_y=0;

		WriteText(frame);
	}
	else
	{
		Main_txt.Create(&Screen, NDXST_SYSMEM, maintext_width, maintext_height);
		Main_txt.Clear(Screen.PixelFormat->Rgb(0,0,0));
		Main_txt.SetColorKey(Screen.PixelFormat->Rgb(0,0,0));
		Main_txt.SetTransP(true);

		Temp.DrawRotoZoom(&Main_txt, 230, 175, 0, 1);

		Interface[0].scroll=FALSE;

		show_image = false;
	}


	//делаем предварительные расчеты для вариантов ответа
	i = Level[frame].url1.GetLength();
	int ansnum=0;

	rows=0;
	sn=0;
	space=0;
	lsp=0;

	for(z=0; z<i; ++z)
	{
		if(Level[frame].url1.GetAt(z)==' ')
		{
//			lsp=space;
			space=z+1;
		}

		if(sn*8>nexttext_width)
		{
			rows=rows+1;
			sn=0;
			z=space;
			lsp=space;

		}
		else if(Level[frame].url1.GetAt(z)=='\n')
		{
			rows=rows+1;
			sn=0;
			z=z+1;
			lsp=z;
		}
		sn=sn+1;
	}
	if((i>1)&&(CheckCondition(Level[frame].condition1)))
	{
		Interface[0].ar1=rows+1;
		ansnum++;
	}
	else
		Interface[0].ar1=0;

	i = Level[frame].url2.GetLength();

	rows=0;
	sn=0;
	space=0;
	lsp=0;

	for(z=0; z<i; ++z)
	{
		if(Level[frame].url2.GetAt(z)==' ')
		{
//			lsp=space;
			space=z+1;
		}

		if(sn*8>nexttext_width)
		{
			rows=rows+1;
			sn=0;
			z=space;
			lsp=space;

		}
		else if(Level[frame].url2.GetAt(z)=='\n')
		{
			rows=rows+1;
			sn=0;
			z=z+1;
			lsp=z;
		}
		sn=sn+1;
	}
	if((i>1)&&(CheckCondition(Level[frame].condition2)))
	{
		Interface[0].ar2=rows+1;
		ansnum++;
	}
	else
		Interface[0].ar2=0;

	i = Level[frame].url3.GetLength();

	rows=0;
	sn=0;
	space=0;
	lsp=0;

	for(z=0; z<i; ++z)
	{
		if(Level[frame].url3.GetAt(z)==' ')
		{
//			lsp=space;
			space=z+1;
		}

		if(sn*8>nexttext_width)
		{
			rows=rows+1;
			sn=0;
			z=space;
			lsp=space;

		}
		else if(Level[frame].url3.GetAt(z)=='\n')
		{
			rows=rows+1;
			sn=0;
			z=z+1;
			lsp=z;
		}
		sn=sn+1;
	}
	if((i>1)&&(CheckCondition(Level[frame].condition3)))
	{
		Interface[0].ar3=rows+1;
		ansnum++;
	}
	else
		Interface[0].ar3=0;

	i = Level[frame].url4.GetLength();

	rows=0;
	sn=0;
	space=0;
	lsp=0;

	for(z=0; z<i; ++z)
	{
		if(Level[frame].url4.GetAt(z)==' ')
		{
//			lsp=space;
			space=z+1;
		}

		if(sn*8>nexttext_width)
		{
			rows=rows+1;
			sn=0;
			z=space;
			lsp=space;

		}
		else if(Level[frame].url4.GetAt(z)=='\n')
		{
			rows=rows+1;
			sn=0;
			z=z+1;
			lsp=z;
		}
		sn=sn+1;
	}
	if((i>1)&&(CheckCondition(Level[frame].condition4)))
	{
		Interface[0].ar4=rows+1;
		ansnum++;
	}
	else
		Interface[0].ar4=0;

	i = Level[frame].url5.GetLength();

	rows=0;
	sn=0;
	space=0;
	lsp=0;

	for(z=0; z<i; ++z)
	{
		if(Level[frame].url5.GetAt(z)==' ')
		{
//			lsp=space;
			space=z+1;
		}

		if(sn*8>nexttext_width)
		{
			rows=rows+1;
			sn=0;
			z=space;
			lsp=space;

		}
		else if(Level[frame].url5.GetAt(z)=='\n')
		{
			rows=rows+1;
			sn=0;
			z=z+1;
			lsp=z;
		}
		sn=sn+1;
	}
	if((i>1)&&(CheckCondition(Level[frame].condition5)))
	{
		Interface[0].ar5=rows+1;
		ansnum++;
	}
	else
		Interface[0].ar5=0;

	height=0;
	int allrows = Interface[0].ar1+Interface[0].ar2+Interface[0].ar3+Interface[0].ar4+Interface[0].ar5;
//	allrows+=32;

	if(allrows*12+(ansnum-1)*16<nexttext_height)
	{
		Interface[0].scr=FALSE;
		height+=nexttext_height;
	}
	else 
	{
		Interface[0].scr=TRUE;
		height+=allrows*12+(ansnum)*16;
	}

	Interface[0].arheight=height;

	Next_txt.Create(&Screen, NDXST_SYSMEM, nexttext_width, height);
	Next_txt.Clear(Screen.PixelFormat->Rgb(0,0,0));
	Next_txt.SetColorKey(Screen.PixelFormat->Rgb(0,0,0));
	Next_txt.SetTransP(true);
	Next_txt_ins.Create(&Screen, NDXST_SYSMEM, nexttext_width, height);
	Next_txt_ins.Clear(Screen.PixelFormat->Rgb(0,0,0));
	Next_txt_ins.SetColorKey(Screen.PixelFormat->Rgb(0,0,0));
	Next_txt_ins.SetTransP(true);

	if(!fight)
		WriteNextText(frame);

	ChangeDate(frame);

	Location_txt.Create(&Screen, NDXST_SYSMEM, 300, 20);
	Location_txt.Clear(Screen.PixelFormat->Rgb(0,0,0));
	Location_txt.SetColorKey(Screen.PixelFormat->Rgb(0,0,0));
	Location_txt.SetTransP(true);

	
	LPSTR dstr;
	CString lcn = Level[frame].location;
	if(Level[frame].sublocation!="")
	{
		lcn+="   ::   ";
		lcn+= Level[frame].sublocation;
	}
	dstr = lcn.GetBuffer(lcn.GetLength());
	lcn.ReleaseBuffer();
	Location_txt.FormattedText(0,0,RGB(181, 149, 89),NULL,dstr);

	ChangeLocation(Level[frame].image);

	if(Level[frame].mp3fn!=music)
		ChangeMusic(Level[frame].mp3fn);

	if(GetData("WASD") == 1)
		Sleep(300);

	DrawStats();
}

void WriteNextText(int frame)
{
	int i = 0;
	int sn=0;
	int rn=0;
	int space=0;
	int lsp = 0;

	if(CheckCondition(Level[frame].condition1))
	{
		i = Level[frame].url1.GetLength();

		for(int z=0; z<i; ++z)
		{
			if(Level[frame].url1.GetAt(z)==' ')
			{
				space=z+1;
			}
				
			if(sn*8>nexttext_width)
			{
				sn=0;
				z=space;
				DrawNextString1(lsp,space,rn, frame);
				lsp=space;
				rn=rn+1;
	
			}
			else if(Level[frame].url1.GetAt(z)=='\n')
			{
				rn=rn+1;
				sn=0;
				DrawNextString1(lsp, z-1, rn, frame);
				z=z+1;
				lsp=z;
			}
			else if(z==i-1)
				DrawNextString1(lsp, z+1, rn, frame);
			sn=sn+1;
		}
	}

	if(CheckCondition(Level[frame].condition2))
	{
		i = Level[frame].url2.GetLength();
		sn=0;
		rn=0;
		space=0;
		lsp = 0;

		for(z=0; z<i; ++z)
		{
			if(Level[frame].url2.GetAt(z)==' ')
			{
				space=z+1;
			}
				
			if(sn*8>nexttext_width)
			{
				sn=0;
				z=space;
				DrawNextString2(lsp,space,rn, frame);
				lsp=space;
				rn=rn+1;
	
			}
			else if(Level[frame].url2.GetAt(z)=='\n')
			{
				rn=rn+1;
				sn=0;
				DrawNextString2(lsp, z-1, rn, frame);
				z=z+1;
				lsp=z;
			}
			else if(z==i-1)
				DrawNextString2(lsp, z+1, rn, frame);
			sn=sn+1;
		}
	}

	if(CheckCondition(Level[frame].condition3))
	{
		i = Level[frame].url3.GetLength();
		sn=0;
		rn=0;
		space=0;
		lsp = 0;
	
		for(z=0; z<i; ++z)
		{
			if(Level[frame].url3.GetAt(z)==' ')
			{
				space=z+1;
			}
				
			if(sn*8>nexttext_width)
			{
				sn=0;
				z=space;
				DrawNextString3(lsp,space,rn, frame);
				lsp=space;
				rn=rn+1;
	
			}
			else if(Level[frame].url3.GetAt(z)=='\n')
			{
				rn=rn+1;
				sn=0;
				DrawNextString3(lsp, z-1, rn, frame);
				z=z+1;
				lsp=z;
			}
			else if(z==i-1)
				DrawNextString3(lsp, z+1, rn, frame);
			sn=sn+1;
		}
	}

	if(CheckCondition(Level[frame].condition4))
	{
		i = Level[frame].url4.GetLength();
		sn=0;
		rn=0;
		space=0;
		lsp = 0;
	
		for(z=0; z<i; ++z)
		{
			if(Level[frame].url4.GetAt(z)==' ')
			{
				space=z+1;
			}
				
			if(sn*8>nexttext_width)
			{
				sn=0;
				z=space;
				DrawNextString4(lsp,space,rn, frame);
				lsp=space;
				rn=rn+1;
	
			}
			else if(Level[frame].url4.GetAt(z)=='\n')
			{
				rn=rn+1;
				sn=0;
				DrawNextString4(lsp, z-1, rn, frame);
				z=z+1;
				lsp=z;
			}
			else if(z==i-1)
				DrawNextString4(lsp, z+1, rn, frame);
			sn=sn+1;
		}
	}
	
	if(CheckCondition(Level[frame].condition5))
	{
		i = Level[frame].url5.GetLength();
		sn=0;
		rn=0;
		space=0;
		lsp = 0;
	
		for(z=0; z<i; ++z)
		{
			if(Level[frame].url5.GetAt(z)==' ')
			{
				space=z+1;
			}
				
			if(sn*8>nexttext_width)
			{
				sn=0;
				z=space;
				DrawNextString5(lsp,space,rn, frame);
				lsp=space;
				rn=rn+1;
	
			}
			else if(Level[frame].url5.GetAt(z)=='\n')
			{
				rn=rn+1;
				sn=0;
				DrawNextString5(lsp, z-1, rn, frame);
				z=z+1;
				lsp=z;
			}
			else if(z==i-1)
				DrawNextString5(lsp, z+1, rn, frame);
			sn=sn+1;
		}
	}
}

void DrawNextString1(int lsp, int space, int rn, int frame)
{
	CString str;
	LPSTR dstr;
	for(int i=lsp; i<space; ++i)
	{
		str+=Level[frame].url1.GetAt(i);
	}

	int l=str.GetLength();

	dstr = str.GetBuffer(l);
	str.ReleaseBuffer();

	Next_txt.FormattedText(0,rn*12,RGB(207, 191, 63),NULL,dstr);
}

void DrawNextString2(int lsp, int space, int rn, int frame)
{
	CString str;
	LPSTR dstr;
	for(int i=lsp; i<space; ++i)
	{
		str+=Level[frame].url2.GetAt(i);
	}

	int l=str.GetLength();

	dstr = str.GetBuffer(l);
	str.ReleaseBuffer();

	int ar1;
	if(Interface[0].ar1==0)
		ar1=0;
	else
		ar1=1;
	Next_txt.FormattedText(0,Interface[0].ar1*12+16*ar1+rn*12,RGB(207, 191, 63),NULL,dstr);
}

void DrawNextString3(int lsp, int space, int rn, int frame)
{
	CString str;
	LPSTR dstr;
	for(int i=lsp; i<space; ++i)
	{
		str+=Level[frame].url3.GetAt(i);
	}

	int l=str.GetLength();

	dstr = str.GetBuffer(l);
	str.ReleaseBuffer();

	int ar1;
	if(Interface[0].ar1==0)
		ar1=0;
	else
		ar1=1;
	int ar2;
	if(Interface[0].ar2==0)
		ar2=0;
	else
		ar2=1;
	Next_txt.FormattedText(0,Interface[0].ar1*12+16*ar1+Interface[0].ar2*12+16*ar2+rn*12,RGB(207, 191, 63),NULL,dstr);
}

void DrawNextString4(int lsp, int space, int rn, int frame)
{
	CString str;
	LPSTR dstr;
	for(int i=lsp; i<space; ++i)
	{
		str+=Level[frame].url4.GetAt(i);
	}

	int l=str.GetLength();

	dstr = str.GetBuffer(l);
	str.ReleaseBuffer();

	int ar1;
	if(Interface[0].ar1==0)
		ar1=0;
	else
		ar1=1;
	int ar2;
	if(Interface[0].ar2==0)
		ar2=0;
	else
		ar2=1;
	int ar3;
	if(Interface[0].ar3==0)
		ar3=0;
	else
		ar3=1;
	Next_txt.FormattedText(0,Interface[0].ar1*12+16*ar1+Interface[0].ar2*12+16*ar2+Interface[0].ar3*12+16*ar3+rn*12,RGB(207, 191, 63),NULL,dstr);
}

void DrawNextString5(int lsp, int space, int rn, int frame)
{
	CString str;
	LPSTR dstr;
	for(int i=lsp; i<space; ++i)
	{
		str+=Level[frame].url5.GetAt(i);
	}

	int l=str.GetLength();

	dstr = str.GetBuffer(l);
	str.ReleaseBuffer();


	int ar1;
	if(Interface[0].ar1==0)
		ar1=0;
	else
		ar1=1;
	int ar2;
	if(Interface[0].ar2==0)
		ar2=0;
	else
		ar2=1;
	int ar3;
	if(Interface[0].ar3==0)
		ar3=0;
	else
		ar3=1;
	int ar4;
	if(Interface[0].ar4==0)
		ar4=0;
	else
		ar4=1;
	Next_txt.FormattedText(0,Interface[0].ar1*12+16*ar1+Interface[0].ar2*12+16*ar2+Interface[0].ar3*12+16*ar3+Interface[0].ar4*12+16*ar4+rn*12,RGB(207, 191, 63),NULL,dstr);
}

void ChangeDate(int frame)
{
	Pip_date.Create(&Screen, NDXST_SYSMEM, 200, 40);
	Pip_date.Clear(Screen.PixelFormat->Rgb(0,0,0));
	Pip_date.SetColorKey(Screen.PixelFormat->Rgb(0,0,0));
	Pip_date.SetTransP(true);

	//выставить день и год
	char s[10];
    wsprintf(s, "%d", Level[fr].day);
	DisplayNumber(s, 13, 12);
	wsprintf(s, "%d", Level[fr].year);
	DisplayNumber(s, 74, 12);

	//напечатать месяц
	Misc.Draw(&Pip_date, 38, 12, NDX_RECT(0, Level[fr].month*21, 32, Level[fr].month*21+21));

}

void DisplayNumber(char* s, UINT x, UINT y)
{
    UINT len = strlen(s);

    // Loop through the entire string.
    for (UINT i=0; i<len; ++i)
    {
        // Convert a digit to an integer.
        char ch = s[i];
        UINT digit = atoi(&ch);

        // Display the digit as a graphic text character.
		Misc.Draw(&Pip_date, x+i*9+2, y, NDX_RECT(digit*9, 0, digit*9+9, 21));
    }
}

void PlayIntro()
{
/*	Video.UpdateMedia();
			
	// Start video if stopped
	Video.DrawMedia(Screen.Back,Scr);			
	if (!Video.IsPlaying())
	{
		Video.Stop();
		Video.CloseMedia();
		DeleteFile("temp");
		mode=0;
	}*/mode=0;	
}

void Titles()
{
	//вывод титров
	Splash.Draw(Screen.Back,0,0,NDX_RECT(0,0,800,600));

	Main_txt.Draw(Screen.Back, 5, 130, NDX_RECT(0, anim, 342, fr+anim));

	if(anim+fr<Main_txt.Height)
		anim++;
	else
	{
		if(fr>=0)
			fr--;
	}
	Sleep(70);

	if(fr==0)
	{
		StopMStream();
		NDX_Surface			Fon; //фон

		Fon.Create(&Screen,NDXST_SYSMEM,800, 600);

		Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
	
		for(int i=17; i>0; --i)
		{
			Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
			Screen.Flip();
			Sleep(60);
		}
	
		fade=true;
		OpenDataFile();
		Splash.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("MENU/sp5.bmp"),true);
		MD.Close();
		
		OpenMStream("data\\sound\\music\\menu.acm");
		SetMVolume(1);
		PlayMStream();
		mode=0;
	}
}

void ChangeLocation(int img)
{
	anim=0;
	Interface[0].iAni=false;
	CString st("INGAME/image_");
	char s[10];
    itoa(img, s, 10);
	st+=s;
	st+=".bmp";

	LPSTR fn;
	fn = st.GetBuffer(st.GetLength());
	st.ReleaseBuffer();
	OpenDataFile();
	Image.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile(fn),true);
	MD.Close();
	
	if(Image.Height>100)
	{
		anim=1;
		Interface[0].iAni=true;
	}
	else
	{
		anim=2;
		Interface[0].iAni=true;
		Interface[0].iFrame=0;
	}
}

void ChangeMusic(CString msc)
{
	StopMStream();
	music=msc;
	
	CString st("data\\sound\\music\\");
	st+=music;
	
	OpenMStream(st);
	PlayMStream();
}

void PrepareFightFrm()
{
	LPSTR head = pFight->Enemy[cur_target].head.GetBuffer(pFight->Enemy[cur_target].head.GetLength());
	pFight->Enemy[cur_target].head.ReleaseBuffer();
	LPSTR eyes = pFight->Enemy[cur_target].eyes.GetBuffer(pFight->Enemy[cur_target].eyes.GetLength());
	pFight->Enemy[cur_target].eyes.ReleaseBuffer();
	LPSTR hand = pFight->Enemy[cur_target].hand.GetBuffer(pFight->Enemy[cur_target].hand.GetLength());
	pFight->Enemy[cur_target].hand.ReleaseBuffer();
	LPSTR torso = pFight->Enemy[cur_target].torso.GetBuffer(pFight->Enemy[cur_target].torso.GetLength());
	pFight->Enemy[cur_target].torso.ReleaseBuffer();
	LPSTR leg = pFight->Enemy[cur_target].leg.GetBuffer(pFight->Enemy[cur_target].leg.GetLength());
	pFight->Enemy[cur_target].leg.ReleaseBuffer();
	LPSTR grain = pFight->Enemy[cur_target].grain.GetBuffer(pFight->Enemy[cur_target].grain.GetLength());
	pFight->Enemy[cur_target].grain.ReleaseBuffer();

	TargetFrm.Draw(Screen.Back, 193, 149);
	TImages[cur_target].Target.Draw(Screen.Back, 261,190);
	TargetMsc.Draw(Screen.Back, 686, 501, NDX_RECT(0, 22, 88, 54));

	for(int i=0; i<pFight->player_AP; ++i)
	{
		TargetMsc.Draw(Screen.Back, 376+i*14, 418, NDX_RECT(176, 30, 184, 38));
	}
	for(i=0; i<pFight->player_cAP; ++i)
	{
		TargetMsc.Draw(Screen.Back, 376+i*14, 418, NDX_RECT(176, 22, 184, 30));
	}

	TargetMsc.Draw(Screen.Back, 193, 127, NDX_RECT(0, 0, 304, 22));

	CString r = pFight->Enemy[cur_target].target_NAME;
	r+=": ";
	r+= pFight->TargetStatus(cur_target);
	LPSTR fn;
	fn = r.GetBuffer(r.GetLength());
	r.ReleaseBuffer();

	Screen.Back->FormattedText(211,130,RGB(255, 0, 0),fnt1, fn);

	double hp = (double)pFight->player_HP / (double)pFight->player_maxHP;

//	TargetMsc.Draw(Screen.Back, 55, 507, NDX_RECT(192, 22, 224, 77));
	TargetMsc.Draw(Screen.Back, 55, 507+55*(1-hp), NDX_RECT(192, 22+55*(1-hp), 224, 77));
//	i = 508;

	if(hp>=0.75)
		i = 508;
	else if((hp<0.75)&&(hp>=0.5))
		i = 522;
	else if((hp<0.5)&&(hp>=0.25))
		i = 536;
	else if(hp<0.25)
		i = 550;

	Screen.Back->FormattedText(40,488,RGB(0, 255, 0),fnt1, "Здоровье:");

	char num[10];
	itoa(pFight->player_HP, num, 10);
//	_gcvt( hp, 10, num );
	r=num;
	r+=" HP";
	Screen.Back->FormattedText(57,i,RGB(255, 255, 255),fnt2, fn);

	Screen.Back->FormattedText(272, 188, RGB(207, 191, 63), fnt1, head);
	Screen.Back->FormattedText(546, 188, RGB(207, 191, 63), fnt1, eyes);
	Screen.Back->FormattedText(272, 252, RGB(207, 191, 63), fnt1, hand);
	Screen.Back->FormattedText(546, 252, RGB(207, 191, 63), fnt1, torso);
	Screen.Back->FormattedText(272, 317, RGB(207, 191, 63), fnt1, leg);
	Screen.Back->FormattedText(546, 317, RGB(207, 191, 63), fnt1, hand);
	Screen.Back->FormattedText(272, 380, RGB(207, 191, 63), fnt1, grain);
	Screen.Back->FormattedText(546, 380, RGB(207, 191, 63), fnt1, leg);

	if(pFight->targets > 1)
	{
		int xp = cur_target*36;
		TargetMsc.Draw(Screen.Back, 695, 149, NDX_RECT(xp,77,36+xp,379));

		if(pFight->Enemy[0].dead)
			TargetMsc.Draw(Screen.Back, 708, 192, NDX_RECT(108,77,120,111));
		
		if(pFight->Enemy[1].dead)
			TargetMsc.Draw(Screen.Back, 708, 286, NDX_RECT(108,112,120,147));

		if(pFight->Enemy[2].dead)
			TargetMsc.Draw(Screen.Back, 708, 377, NDX_RECT(108,148,120,183));

		DrawRadar();
	}
	
	if(IEvent[0].Object==20)
	{
		if(IEvent[0].Conditional==1)
			Screen.Back->FormattedText(272, 188, RGB(238, 231, 134), fnt1, head);
		else if(IEvent[0].Conditional==2)
		{
			IEvent[1].Object=20;
			IEvent[1].Conditional=2;
			Screen.Back->FormattedText(272, 188, RGB(255, 255, 255), fnt1, head);
		}
	}
	else if(IEvent[0].Object==21)
	{
		if(IEvent[0].Conditional==1)
			Screen.Back->FormattedText(546, 188, RGB(238, 231, 134), fnt1, eyes);
		else if(IEvent[0].Conditional==2)
		{
			IEvent[1].Object=21;
			IEvent[1].Conditional=2;
			Screen.Back->FormattedText(546, 188, RGB(255, 255, 255), fnt1, eyes);
		}
	}
	else if(IEvent[0].Object==22)
	{
		if(IEvent[0].Conditional==1)
			Screen.Back->FormattedText(546, 252, RGB(238, 231, 134), fnt1, torso);
		else if(IEvent[0].Conditional==2)
		{
			IEvent[1].Object=22;
			IEvent[1].Conditional=2;
			Screen.Back->FormattedText(546, 252, RGB(255, 255, 255), fnt1, torso);
		}
	}
	else if(IEvent[0].Object==23)
	{
		if(IEvent[0].Conditional==1)
			Screen.Back->FormattedText(272, 252, RGB(238, 231, 134), fnt1, hand);
		else if(IEvent[0].Conditional==2)
		{
			IEvent[1].Object=23;
			IEvent[1].Conditional=2;
			Screen.Back->FormattedText(272, 252, RGB(255, 255, 255), fnt1, hand);
		}
	}
	else if(IEvent[0].Object==24)
	{
		if(IEvent[0].Conditional==1)
			Screen.Back->FormattedText(546, 317, RGB(238, 231, 134), fnt1, hand);
		else if(IEvent[0].Conditional==2)
		{
			IEvent[1].Object=24;
			IEvent[1].Conditional=2;
			Screen.Back->FormattedText(546, 317, RGB(255, 255, 255), fnt1, hand);
		}
	}
	else if(IEvent[0].Object==25)
	{
		if(IEvent[0].Conditional==1)
			Screen.Back->FormattedText(272, 380, RGB(238, 231, 134), fnt1, grain);
		else if(IEvent[0].Conditional==2)
		{
			IEvent[1].Object=25;
			IEvent[1].Conditional=2;
			Screen.Back->FormattedText(272, 380, RGB(255, 255, 255), fnt1, grain);
		}
	}
	else if(IEvent[0].Object==26)
	{
		if(IEvent[0].Conditional==1)
			Screen.Back->FormattedText(272, 317, RGB(238, 231, 134), fnt1, leg);
		else if(IEvent[0].Conditional==2)
		{
			IEvent[1].Object=26;
			IEvent[1].Conditional=2;
			Screen.Back->FormattedText(272, 317, RGB(255, 255, 255), fnt1, leg);
		}
	}
	else if(IEvent[0].Object==27)
	{
		if(IEvent[0].Conditional==1)
			Screen.Back->FormattedText(546, 380, RGB(238, 231, 134), fnt1, leg);
		else if(IEvent[0].Conditional==2)
		{
			IEvent[1].Object=27;
			IEvent[1].Conditional=2;
			Screen.Back->FormattedText(546, 380, RGB(255, 255, 255), fnt1, leg);
		}
	}
	else if(IEvent[0].Object==28)
	{
		if(IEvent[0].Conditional==1)
		{
			TargetMsc.Draw(Screen.Back, 686, 501, NDX_RECT(88, 22, 176, 54));
			IEvent[1].Object=28;
			IEvent[1].Conditional=1;
		}
	}


	char s[10];
    wsprintf(s, "%d", pFight->HitChance(0, cur_target));
	DisplayAccNumber(s, 221, 181);
	wsprintf(s, "%d", pFight->HitChance(1, cur_target));
	DisplayAccNumber(s, 644, 181);
	wsprintf(s, "%d", pFight->HitChance(3, cur_target));
	DisplayAccNumber(s, 221, 245);
	wsprintf(s, "%d", pFight->HitChance(2, cur_target));
	DisplayAccNumber(s, 644, 245);
	wsprintf(s, "%d", pFight->HitChance(6, cur_target));
	DisplayAccNumber(s, 221, 309);
	wsprintf(s, "%d", pFight->HitChance(4, cur_target));
	DisplayAccNumber(s, 644, 309);
	wsprintf(s, "%d", pFight->HitChance(5, cur_target));
	DisplayAccNumber(s, 221, 373);
	wsprintf(s, "%d", pFight->HitChance(7, cur_target));
	DisplayAccNumber(s, 644, 373);

/*	CString s;
	char num[10];
	itoa(pFight->player_AP, num, 10);
	s=num;
	AfxMessageBox(s);*/
}

void DrawRadar()
{
	int celx = 178 / pFight->XSize; // размер 1й ячейки по X
	int cely = 113 / pFight->YSize; // по Y
	if(cely<celx)
		celx=cely;
	else
		cely=celx;
	int centx = 178 / 2; // 
	int centy = 113;

	Image.Create(&Screen, NDXST_SYSMEM, 178, 113);
	Image.Clear(RGB(255,0,255));
	Image.SetColorKey(RGB(255,0,255));
	Image.SetTransP(true);

	for(int i=0; i<pFight->targets; ++i)
	{
		if(!pFight->Enemy[i].dead)
		{
			/*if(cur_target==i)
				TargetRdr.Draw(&Image, pFight->Enemy[i].target_xpos*celx+15, pFight->Enemy[i].target_ypos*cely+57, NDX_RECT(10,114,9,122));
			else
				TargetRdr.Draw(&Image, pFight->Enemy[i].target_xpos*celx+15, pFight->Enemy[i].target_ypos*cely+57, NDX_RECT(0,114,21,125));*/
			int xpos = centx + pFight->Enemy[i].target_xpos*celx;
			int ypos = centy - pFight->Enemy[i].target_ypos*cely;

			if(cur_target==i)
				TargetRdr.DrawRotoZoom(&Image, xpos, ypos, 0, 1, NDX_RECT(9,114,21,125));
			else
				TargetRdr.DrawRotoZoom(&Image, xpos, ypos, 0, 1, NDX_RECT(0,113,9,122));
		}
	}
}

void HitTarget(int p)
{
	CString r = pFight->HitTarget(p, cur_target);
	r+="\n";
	battlelog.Insert(0, r);
/*	LPSTR fn;
	fn = battlelog.GetBuffer(battlelog.GetLength());
	battlelog.ReleaseBuffer();*/

	Next_txt.Create(&Screen, NDXST_SYSMEM, nexttext_width, 200);
	Next_txt.Clear(Screen.PixelFormat->Rgb(0,0,0));
	Next_txt.SetColorKey(Screen.PixelFormat->Rgb(0,0,0));
	Next_txt.SetTransP(true);

	WriteFLogText();

	if(r!="Недостаточно очков действия для нанесения этого удара.\n")
	{
		if(!pFight->move)
		{
			// озвучка
			r = "humanoidAttack";
			int i = rand() % 2;
			i++;
			char num[10];
			itoa(i, num, 10);
			r+=num;
			r+=".WAV";

			OpenSStream(r);
			Sleep(200);
			if(pFight->missp==0)
			{
				r = "";
				r += pFight->Enemy[cur_target].target_type;
				i = rand () % 4;
				i++;
				itoa(i, num, 10);
				r+=num;
				r+=".WAV";
				OpenSStream(r);
			}
		}
		else
		{
			Sleep(200);
			OpenSStream("deny.wav");
		}
	}

	CheckFight();
}

void EndTurn()
{
	GameCircle();
	Screen.Flip();

	for(int i=0; i<pFight->targets; ++i)
	{
		if(!pFight->Enemy[i].dead)
		{
			while(pFight->Enemy[i].target_cAP>=4)
			{
				CString s=pFight->AttackPlayer(i);

				if(s!="")
				{
					s+="\n";
					battlelog.Insert(0, s);

					WriteFLogText();

					if(pFight->move)
					{
						CString r = pFight->Enemy[i].target_type;
						r+="Walk.WAV";
						OpenSStream(r);
						Sleep(1000);
					}
					else
					{
						// озвучка
						CString r = pFight->Enemy[i].target_weapon;
						r+="Attack";
						int i = rand() % 2;
						i++;
						char num[10];
						itoa(i, num, 10);
						r+=num;
						r+=".WAV";
						OpenSStream(r);
						Sleep(500);
						
						if(pFight->misst==0)
						{
							r = "humanoid";
							i = rand () % 4;
							i++;
							itoa(i, num, 10);
							r+=num;
							r+=".WAV";
							OpenSStream(r);
							Sleep(300);
						}
					}

					GameCircle();
					Screen.Flip();

					if(CheckFight())
						break;
				}
			}
		}
	}

	pFight->ReinitFight();

	OpenDataFile();
	Cursor.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("CURSOR/CURSOR_1.bmp"),true);
	Cursor.SetColorKey(Screen.PixelFormat->Rgb(0,0,0));
	Cursor.SetTransP(true);
	MD.Close();

	OpenSStream("yourTurn.wav");
}

bool CheckFight()
{
	CString s;
	// если мы проиграли
	if(pFight->p_hpl>=pFight->player_HP)
	{
		if(pFight->player_HP==0)
		{
			CString r = "humanoidDeath";
			int i = rand () % 2;
			i++;
			char num[10];
			itoa(i, num, 10);
			r+=num;
			r+=".WAV";
			OpenSStream(r);
			Sleep(700);
		}

		OpenSStream("combatEnd.wav");
		fight=false;
		ModifyData("PHealth", pFight->player_HP);
		PrepareFrame(pFight->l_tw);

		return true;
	}
	// если проиграл противник
	else if(pFight->t_hpl>=pFight->Enemy[cur_target].target_HP)
	{
		if(pFight->Enemy[cur_target].target_HP==0)
		{
			pFight->Enemy[cur_target].dead=true;
			CString r = pFight->Enemy[cur_target].target_type;
			r+="Death";
			int i = rand () % 2;
			i++;
			char num[10];
			itoa(i, num, 10);
			r+=num;
			r+=".WAV";
			OpenSStream(r);
			Sleep(700);

			for(i=0; i<pFight->targets; ++i)
			{
				if(!pFight->Enemy[i].dead)
				{
					cur_target=i;
					break;
				}
			}
		}
		else if(pFight->Enemy[cur_target].target_HP<=pFight->t_hpl)
		{
			pFight->Enemy[cur_target].dead=true;
			CString r = pFight->Enemy[cur_target].target_type;
			r+="Death";
			int i = rand () % 2;
			i++;
			char num[10];
			itoa(i, num, 10);
			r+=num;
			r+=".WAV";
			OpenSStream(r);
			Sleep(700);
		}

		if(pFight->AllDead())
		{
			OpenSStream("combatEnd.wav");
			fight=false;
			ModifyData("PHealth", pFight->player_HP);
			PrepareFrame(pFight->l_pw);			
		}

		return true;
	}
	else
		return false;
}

void WriteFLogText()
{
	int i = 0;
	int sn=0;
	int rn=0;
	int space=0;
	int lsp = 0;

	Next_txt.Create(&Screen, NDXST_SYSMEM, nexttext_width, 600);
	Next_txt.Clear(Screen.PixelFormat->Rgb(0,0,0));
	Next_txt.SetColorKey(Screen.PixelFormat->Rgb(0,0,0));
	Next_txt.SetTransP(true);

	Interface[0].arheight=600;
	Interface[0].scr=TRUE;

	i = battlelog.GetLength();

	for(int z=0; z<i; ++z)
	{
		if(battlelog.GetAt(z)==' ')
		{
			space=z;
		}
				
		if(sn*7>nexttext_width)
		{
			sn=0;
			z=space+1;
			DrawLogString(lsp,space,rn);
			lsp=space;	
			++rn;
		}
		else if(battlelog.GetAt(z)=='\n')
		{
			sn=0;
			DrawLogString(lsp, z-1, rn);
			z=z+1;
			lsp=z;
			++rn;
		}
		else if(z==i-1)
			DrawLogString(lsp, z+1, rn);

		++sn;
	}
}

void DrawLogString(int lsp, int space, int rn)
{
	CString str;
	LPSTR dstr;
	for(int i=lsp; i<=space; ++i)
	{
		str+=battlelog.GetAt(i);
	}

	int l=str.GetLength();

	dstr = str.GetBuffer(l);
	str.ReleaseBuffer();

	Next_txt.FormattedText(0,rn*12,RGB(207, 191, 63),fnt1,dstr);
//	AfxMessageBox(str);
}

void DisplayAccNumber(char* s, UINT x, UINT y)
{
    UINT len = strlen(s);

    // Loop through the entire string.
    for (UINT i=0; i<len; ++i)
    {
        // Convert a digit to an integer.
        char ch = s[i];
        UINT digit = atoi(&ch);

        // Display the digit as a graphic text character.
		//TargetMsc.Draw(Screen.Back, x+i*9+3, y, NDX_RECT(digit*7, 61, digit*7+7, 77));
		Misc.Draw(Screen.Back, x+i*9+2, y, NDX_RECT(digit*9, 0, digit*9+9, 21));
    }
}

void PrepareArchiveFrm()
{
	int line=0;
	for(int i=0; i<Quests.GetSize(); ++i)
	{
		DWORD Color;

		CString r = GetQTitle(i);

		if(GetQStatus(i)==1)
		{
			Color = RGB(207, 191, 63);
			r+="  (в процессе)";
		}
		else if(GetQStatus(i)==2)
		{
			Color = RGB(216,114,52);
			r+="  (провалено)";
		}
		else if(GetQStatus(i)==3)
		{
			Color = RGB(253, 233, 77);
			r+="  (завершено)";
		}

		LPSTR fn;
		fn = r.GetBuffer(r.GetLength());
		r.ReleaseBuffer();

		Screen.Back->FormattedText(maintext_x, maintext_y+line*12, Color, fnt1, fn);

		++line;
	}
}

bool OpenDataFile()
{
	MD.SetFileDescription("Fallout Data File");
	MD.Open(gfxnf);

	return MD.IsFileOpen();
}

void ShowInventory()
{
	Main.Draw(Screen.Back,0,0,NDX_RECT(0,0,800,600));

	Main_txt.Draw(Screen.Back, maintext_x, maintext_y, NDX_RECT(0, 0+Interface[0].cur_y, maintext_width, maintext_height+Interface[0].cur_y));
	Next_txt.Draw(Screen.Back, nexttext_x, nexttext_y, NDX_RECT(0, 0+Interface[0].c_y, nexttext_width, nexttext_height+Interface[0].c_y));
		
	if((anim==1)&&(Interface[0].iAni))
		Image.Draw(Screen.Back, image_x, image_y, NDX_RECT(0, Interface[0].iFrame*100, 140, Interface[0].iFrame*100+100));
	else if((anim==2)&&(Interface[0].iAni)&&(!fight))
	{
		UINT an;
		an = rand() % 18;

		if(an != 3)
			Image.Draw(Screen.Back, image_x, image_y, NDX_RECT(0, 0, 140, 100));
		else
			Image.DrawColorMix(Screen.Back, image_x, image_y, NDX_RECT(0, 0, 140, 100), Screen.PixelFormat->Rgb(0,0,0), 0);
			
		Noise.DrawTranslucent(Screen.Back, image_x, image_y, NDX_RECT(0, Interface[0].iFrame*100, 140, Interface[0].iFrame*100+100), 16+an);
	}
	else
	{
		if((fight)&&(pFight->targets>1))
		{
			TargetRdr.Draw(Screen.Back, 15, 57, NDX_RECT(0,0,178,113));
			int yp = Interface[0].iFrame*8+1;
			if(yp>113)
			{
				yp = 256 - Interface[0].iFrame*6;
				if(yp<=0)
					yp=0;
				Image.DrawTranslucent(Screen.Back, 15, 57, NDX_RECT(0, 0, 178, 113), yp);
			}
			else
				Image.DrawTranslucent(Screen.Back, 15, 57+(113-yp), NDX_RECT(0, 113-yp, 178, 113), 168);
		}
		else
			Image.Draw(Screen.Back, image_x, image_y, NDX_RECT(0, 0, 140, 100));
	}
	

	Pip_mnu.Draw(Screen.Back, pipmnu_x, pipmnu_y, NDX_RECT(0, 0, 125, 126));
	if(Interface[0].scroll==TRUE)
		Pip_srl.Draw(Screen.Back, pipsrl_x, pipsrl_y, NDX_RECT(0, 0, 11, 50));
	else
		Pip_srl.Draw(Screen.Back, pipsrl_x, pipsrl_y, NDX_RECT(22, 0, 33, 50));

	if(!fight)
	{
		if(Interface[0].scr==TRUE)
			Pip_srl.Draw(Screen.Back, nxtsrl_x, nxtsrl_y, NDX_RECT(0, 0, 11, 50));
		else
			Pip_srl.Draw(Screen.Back, nxtsrl_x, nxtsrl_y, NDX_RECT(22, 0, 33, 50));
	}
	Pip_date.Draw(Screen.Back, 0, 0);
	Location_txt.Draw(Screen.Back, location_x, location_y);
	
	InvEquip.Draw(Screen.Back, 686, 540, NDX_RECT(1, 473, 89, 505));

	if(fight)
	{
		PrepareFightFrm();
	}
	else
		Stats.Draw(Screen.Back, 31, 487);	// нарисуем наши статы

	if((inv_mode==3)||(inv_mode==-4))
	{
		InvEquip.Draw(Screen.Back, 108, 64, NDX_RECT(0, 0, 401, 472));	// рисуем окно экипировки
		
		if(pFight->Weapon[0].player_weapon!="")
		{
			double scale=1;
			if(pFight->Weapon[0].wpn_image.Height>62)
			{
				scale = 62/pFight->Weapon[0].wpn_image.Height;
			}
			if(pFight->Weapon[0].wpn_image.Width>139)
			{
				double r = 139/pFight->Weapon[0].wpn_image.Width;
				if(r<scale)
					scale=r;
			}
			pFight->Weapon[0].wpn_image.DrawRotoZoom(Screen.Back, 399, 431, 0, scale);
		}
	}
	else
	{
		Inventory.Draw(Screen.Back, 103, 64); // рисуем окно лута

		LPSTR fn;
		fn = obj_Inv.s_name.GetBuffer(obj_Inv.s_name.GetLength());
		obj_Inv.s_name.ReleaseBuffer();
		
		Screen.Back->FormattedText(545,80,RGB(207, 191, 63),fnt1, fn);

		// выводим вещи, которые игрок лутает
		obj_Inv.inv_list.Draw(Screen.Back, 532, 108, NDX_RECT(0, 0+obj_Inv.sc_y*64, 130, 384+obj_Inv.sc_y*64));
		if(obj_Inv.scrollable_i)
			Pip_srl.Draw(Screen.Back, 664, 108, NDX_RECT(34, 0, 42, 36));
	}

	if((obj_Inv.s_image!="")&&(inv_mode==0))
		obj_Inv.inv_source.DrawRotoZoom(Screen.Back, 401, 218, 0, 1.5);
	else if((plr_Inv.s_image!="")&&(inv_mode==3))
	{
		plr_Inv.inv_source.DrawRotoZoom(Screen.Back, 401, 218, 0, 1.5);
		// напишем жизнь игрока
		int r = GetData("PHealth");
		Screen.Back->FillRect(333, 302, 461, 303, Screen.PixelFormat->Rgb(207, 191, 63));
		Screen.Back->FormattedText(333,304, RGB(207, 191, 63),fnt2, "Здоровье: %u / %u", r, pFight->player_maxHP);
		// напишем урон игрока
		Screen.Back->FormattedText(333,314, RGB(207, 191, 63),fnt2, "Урон: %u - %u", pFight->player_DMGfrom, pFight->player_DMGto);

	}

	if(inv_mode==2)
		obj_Inv.inv_desc.Draw(Screen.Back, 330, 115);
	else if((inv_mode==-2)||(inv_mode==-4))
		plr_Inv.inv_desc.Draw(Screen.Back, 330, 115);

	// выводим вещи игрока
	plr_Inv.inv_list.Draw(Screen.Back, 134, 108, NDX_RECT(0, 0+plr_Inv.sc_y*64, 130, 384+plr_Inv.sc_y*64));
	if(plr_Inv.scrollable_i)
			Pip_srl.Draw(Screen.Back, 270, 108, NDX_RECT(34, 0, 42, 36));

	Misc_but.Draw(Screen.Back, 337, 495, NDX_RECT(0, 48, 125, 72));

	if((inv_mode==0)||(inv_mode==3))
	{
		if((MouseLB)&&(drag==1)&&(inv_mode==0))
			obj_Inv.DrawMoveObj(Screen.Back, MouseX, MouseY);
		else if((MouseLB)&&(drag==1)&&(inv_mode==3))
			pFight->Weapon[0].wpn_image.DrawRotoZoom(Screen.Back, MouseX, MouseY, 0, 1);
		
		if((MouseLB)&&(drag==-1))
			plr_Inv.DrawMoveObj(Screen.Back, MouseX, MouseY);
	}
	else if((inv_mode==1)||(inv_mode==5))
	{
		MoveM.Draw(Screen.Back, 270, 219, NDX_RECT(0, 0, 259, 162));

		if(drag==1)
			obj_Inv.DrawItem(Screen.Back, 332, 294);
		else if(drag==-1)
			plr_Inv.DrawItem(Screen.Back, 332, 294);
		
		char s[10];
		int r=amount[0]*10000+amount[1]*1000+amount[2]*100+amount[3]*10+amount[4];
		wsprintf(s, "%d", r);

		DisplayMMoveNumber(s, 393, 265);	
		
		if((IEvent[0].Object==1)&&(IEvent[0].Conditional==1))
		{
			MoveM.Draw(Screen.Back, 471, 265, NDX_RECT(212,175,227,186));
			IEvent[1].Object=1;
			IEvent[1].Conditional=1;			
		}
		else if((IEvent[0].Object==2)&&(IEvent[0].Conditional==1))
		{
			MoveM.Draw(Screen.Back, 471, 276, NDX_RECT(229,175,244,186));
			IEvent[1].Object=2;
			IEvent[1].Conditional=1;			
		}
		else if((IEvent[0].Object==3)&&(IEvent[0].Conditional==1))
		{
			MoveM.Draw(Screen.Back, 391, 299, NDX_RECT(94,204,188,233));
			IEvent[1].Object=3;
			IEvent[1].Conditional=1;			
		}
		else if((IEvent[0].Object==4)&&(IEvent[0].Conditional==1))
		{
			MoveM.Draw(Screen.Back, 289, 346, NDX_RECT(0,163,105,183));
			IEvent[1].Object=4;
			IEvent[1].Conditional=1;			
		}
		else if((IEvent[0].Object==5)&&(IEvent[0].Conditional==1))
		{
			MoveM.Draw(Screen.Back, 408, 346, NDX_RECT(105,163,210,183));
			IEvent[1].Object=5;
			IEvent[1].Conditional=1;			
		}
	}

	if(IEvent[0].Object==30)
	{
		if(IEvent[0].Conditional==1)
		{
			InvEquip.Draw(Screen.Back, 686, 540, NDX_RECT(89, 473, 177, 505));
			IEvent[1].Object=30;
			IEvent[1].Conditional=1;
		}
	}
	else if(IEvent[0].Object==31)
	{
		if(IEvent[0].Conditional==1)
		{
			Misc_but.Draw(Screen.Back, 337, 495, NDX_RECT(125, 48, 250, 72));
			IEvent[1].Object=30; // 30 - правильное значение!!!
			IEvent[1].Conditional=1;
		}
	}
	else if(IEvent[0].Object==32)
	{
		if(IEvent[0].Conditional==1)
		{
			IEvent[1].Object=32;
			IEvent[1].Conditional=1;
		}
	}
	else if(IEvent[0].Object==80) // начинаем рисовать нажатые кнопки прокрутки
	{	if(IEvent[0].Conditional==1) {
			Pip_srl.Draw(Screen.Back, 664, 108, NDX_RECT(42, 0, 50, 6));
			IEvent[1].Object=80; 
			IEvent[1].Conditional=1; }
	}
	else if(IEvent[0].Object==81)
	{	if(IEvent[0].Conditional==1) {
			Pip_srl.Draw(Screen.Back, 664, 137, NDX_RECT(42, 29, 50, 36));
			IEvent[1].Object=81; 
			IEvent[1].Conditional=1; }
	}
	else if(IEvent[0].Object==82)
	{	if(IEvent[0].Conditional==1) {
			Pip_srl.Draw(Screen.Back, 270, 108, NDX_RECT(42, 0, 50, 6));
			IEvent[1].Object=82; 
			IEvent[1].Conditional=1; }
	}
	else if(IEvent[0].Object==83)
	{	if(IEvent[0].Conditional==1) {
			Pip_srl.Draw(Screen.Back, 270, 137, NDX_RECT(42, 29, 50, 36));
			IEvent[1].Object=83; 
			IEvent[1].Conditional=1; }
	}
	
	if(prog_exit)
	{
		Error.Draw(Screen.Back, 201, 200);
		Misc_but.Draw(Screen.Back, 246, 360, NDX_RECT(0, 0, 125, 24));
		Misc_but.Draw(Screen.Back, 421, 360, NDX_RECT(0, 24, 125, 48));
		Screen.Back->FormattedText(246,260,RGB(207, 191, 63),NULL,"Это действие приведет к уничтожению вещи...");

		if(IEvent[0].Conditional!=0)
		{
			if(IEvent[0].Object==35)
			{
				if(IEvent[0].Conditional==1)
				{
					Misc_but.Draw(Screen.Back,246,360,NDX_RECT(125,0,250,24));
					Click();
					IEvent[1].Object=35;
					IEvent[1].Conditional=1;
				}
			}
			else if(IEvent[0].Object==36)
			{
				if(IEvent[0].Conditional==1)
				{
					Misc_but.Draw(Screen.Back,421,360,NDX_RECT(125,24,250,48));
					Click();
					IEvent[1].Object=36;
					IEvent[1].Conditional=1;
				}
			}
		}
	}

	if(fade==TRUE)
	{
		NDX_Surface			Fon; //фон

		Fon.Create(&Screen,NDXST_SYSMEM,800, 600);

		Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
	
		for(int i=0; i<17; ++i)
		{
			Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
			Screen.Flip();
			Sleep(10);
		}
	
		fade=false;
	}

	// курсор мышки
	if(/*(fullscreen)&&*/(drag==0)&&(inv_mode==0))
			ICursor.Draw(Screen.Back,MouseX,MouseY, NDX_RECT(0, cursor_mode*40, 70, cursor_mode*40+40));
	else if(/*(fullscreen)&&*/(inv_mode!=0))
			ICursor.Draw(Screen.Back,MouseX,MouseY, NDX_RECT(0, cursor_mode*40, 70, cursor_mode*40+40));
}

void ShowBarter()
{
	Main.Draw(Screen.Back,0,0,NDX_RECT(0,0,800,600));

	Main_txt.Draw(Screen.Back, maintext_x, maintext_y, NDX_RECT(0, 0+Interface[0].cur_y, maintext_width, maintext_height+Interface[0].cur_y));
	Next_txt.Draw(Screen.Back, nexttext_x, nexttext_y, NDX_RECT(0, 0+Interface[0].c_y, nexttext_width, nexttext_height+Interface[0].c_y));
		
	if((anim==1)&&(Interface[0].iAni))
		Image.Draw(Screen.Back, image_x, image_y, NDX_RECT(0, Interface[0].iFrame*100, 140, Interface[0].iFrame*100+100));
	else if((anim==2)&&(Interface[0].iAni)&&(!fight))
	{
		UINT an;
		an = rand() % 18;

		if(an != 3)
			Image.Draw(Screen.Back, image_x, image_y, NDX_RECT(0, 0, 140, 100));
		else
			Image.DrawColorMix(Screen.Back, image_x, image_y, NDX_RECT(0, 0, 140, 100), Screen.PixelFormat->Rgb(0,0,0), 0);
			
		Noise.DrawTranslucent(Screen.Back, image_x, image_y, NDX_RECT(0, Interface[0].iFrame*100, 140, Interface[0].iFrame*100+100), 16+an);
	}	

	Pip_mnu.Draw(Screen.Back, pipmnu_x, pipmnu_y, NDX_RECT(0, 0, 125, 126));
	if(Interface[0].scroll==TRUE)
		Pip_srl.Draw(Screen.Back, pipsrl_x, pipsrl_y, NDX_RECT(0, 0, 11, 50));
	else
		Pip_srl.Draw(Screen.Back, pipsrl_x, pipsrl_y, NDX_RECT(22, 0, 33, 50));

	if(!fight)
	{
		if(Interface[0].scr==TRUE)
			Pip_srl.Draw(Screen.Back, nxtsrl_x, nxtsrl_y, NDX_RECT(0, 0, 11, 50));
		else
			Pip_srl.Draw(Screen.Back, nxtsrl_x, nxtsrl_y, NDX_RECT(22, 0, 33, 50));
	}
	Pip_date.Draw(Screen.Back, 0, 0);
	Location_txt.Draw(Screen.Back, location_x, location_y);
	
	InvEquip.Draw(Screen.Back, 686, 540, NDX_RECT(1, 473, 89, 505)); //кнопка инвентаря

	Stats.Draw(Screen.Back, 31, 487);	// нарисуем наши статы

	Barter.Draw(Screen.Back, 50, 93, NDX_RECT(0, 0, 700, 413)); // нарисуем экран бартера
	// кнопка отмены и кнопка сделки
	Misc_but.Draw(Screen.Back, 405, 472, NDX_RECT(0, 48, 125, 72));
	Barter.Draw(Screen.Back, 270, 472, NDX_RECT(0, 413, 125, 437));

	LPSTR fn; // отобразим названия инвентарей
	fn = obj_Inv.s_name.GetBuffer(obj_Inv.s_name.GetLength());
	obj_Inv.s_name.ReleaseBuffer();		
	Screen.Back->FormattedText(606,107,RGB(207, 191, 63),fnt1, fn);
	fn = plr_Inv.s_name.GetBuffer(plr_Inv.s_name.GetLength());
	plr_Inv.s_name.ReleaseBuffer();		
	Screen.Back->FormattedText(74,107,RGB(207, 191, 63),fnt1, fn);
	char s[10]; // отобразим стоимости
	wsprintf(s, "%d", plr_Bar.cost);
	fn = s;
	Screen.Back->FormattedText(300,428,RGB(255, 255, 255),fnt1, "$: ");
	Screen.Back->FormattedText(315,428,RGB(255, 255, 255),fnt1, fn);
	wsprintf(s, "%d", obj_Bar.cost);
	fn = s;
	Screen.Back->FormattedText(438,428,RGB(255, 255, 255),fnt1, "$: ");
	Screen.Back->FormattedText(453,428,RGB(255, 255, 255),fnt1, fn);

	
	obj_Inv.inv_blist.Draw(Screen.Back, 605, 140, NDX_RECT(0, obj_Inv.sc_y*64, 121, 320+obj_Inv.sc_y*64)); // выводим вещи объекта
	if(obj_Inv.scrollable){
		Barter.Draw(Screen.Back, 566, 279, NDX_RECT(251, 413, 266, 429)); // выводим скроллбар, если надо
		Barter.Draw(Screen.Back, 566, 298, NDX_RECT(283, 413, 298, 429)); }
	plr_Inv.inv_blist.Draw(Screen.Back, 75, 140, NDX_RECT(0, plr_Inv.sc_y*64, 121, 320+plr_Inv.sc_y*64)); // выводим вещи игрока
	if(plr_Inv.scrollable){
		Barter.Draw(Screen.Back, 221, 279, NDX_RECT(251, 413, 266, 429)); // выводим скроллбар, если надо
		Barter.Draw(Screen.Back, 221, 298, NDX_RECT(283, 413, 298, 429)); }
	obj_Bar.inv_blist.Draw(Screen.Back, 429, 175, NDX_RECT(0, obj_Bar.sc_y*55, 80, 220+obj_Bar.sc_y*55)); // выводим вещи, предлагаемые на обмен игроку
	if(obj_Bar.scrollable){
		Barter.Draw(Screen.Back, 542, 282, NDX_RECT(316, 413, 329, 426)); // выводим скроллбар, если надо
		Barter.Draw(Screen.Back, 542, 298, NDX_RECT(344, 413, 357, 426)); }
	plr_Bar.inv_blist.Draw(Screen.Back, 291, 175, NDX_RECT(0, plr_Bar.sc_y*55, 80, 220+plr_Bar.sc_y*55)); // выводим вещи, предлагаемые на обмен игроком
	if(plr_Bar.scrollable){
		Barter.Draw(Screen.Back, 247, 282, NDX_RECT(316, 413, 329, 426)); // выводим скроллбар, если надо
		Barter.Draw(Screen.Back, 247, 298, NDX_RECT(344, 413, 357, 426)); }

	if(inv_mode==2)
		obj_Inv.inv_desc.DrawTranslucent(Screen.Back, 457, 200, 166);
	else if(inv_mode==-2)
		plr_Inv.inv_desc.DrawTranslucent(Screen.Back, 188, 200, 166);
	else if(inv_mode==4)
		obj_Bar.inv_desc.DrawTranslucent(Screen.Back, 532, 184, 166);
	else if(inv_mode==-4)
		plr_Bar.inv_desc.DrawTranslucent(Screen.Back, 113, 184, 166);
	
	if(inv_mode==0)
	{
		if((MouseLB)&&(drag==1))
			obj_Inv.DrawMoveObj(Screen.Back, MouseX, MouseY);
		else if((MouseLB)&&(drag==-1))
			obj_Bar.DrawMoveObj(Screen.Back, MouseX, MouseY);
		else if((MouseLB)&&(drag==2))
			plr_Bar.DrawMoveObj(Screen.Back, MouseX, MouseY);
		else if((MouseLB)&&(drag==-2))
			plr_Inv.DrawMoveObj(Screen.Back, MouseX, MouseY);
	}
	else if((inv_mode==1)||(inv_mode==5))
	{
		MoveM.Draw(Screen.Back, 270, 219, NDX_RECT(0, 0, 259, 162));

		if(drag==1)
			obj_Inv.DrawItem(Screen.Back, 332, 294);
		else if(drag==-1)
			obj_Bar.DrawItem(Screen.Back, 332, 294);
		else if(drag==-2)
			plr_Inv.DrawItem(Screen.Back, 332, 294);
		else if(drag==2)
			plr_Bar.DrawItem(Screen.Back, 332, 294);

		int r=amount[0]*10000+amount[1]*1000+amount[2]*100+amount[3]*10+amount[4];
		wsprintf(s, "%d", r);

		DisplayMMoveNumber(s, 393, 265);	
		
		if((IEvent[0].Object==1)&&(IEvent[0].Conditional==1))
		{
			MoveM.Draw(Screen.Back, 471, 265, NDX_RECT(212,175,227,186));
			IEvent[1].Object=1;
			IEvent[1].Conditional=1;			
		}
		else if((IEvent[0].Object==2)&&(IEvent[0].Conditional==1))
		{
			MoveM.Draw(Screen.Back, 471, 276, NDX_RECT(229,175,244,186));
			IEvent[1].Object=2;
			IEvent[1].Conditional=1;			
		}
		else if((IEvent[0].Object==3)&&(IEvent[0].Conditional==1))
		{
			MoveM.Draw(Screen.Back, 391, 299, NDX_RECT(94,204,188,233));
			IEvent[1].Object=3;
			IEvent[1].Conditional=1;			
		}
		else if((IEvent[0].Object==4)&&(IEvent[0].Conditional==1))
		{
			MoveM.Draw(Screen.Back, 289, 346, NDX_RECT(0,163,105,183));
			IEvent[1].Object=4;
			IEvent[1].Conditional=1;			
		}
		else if((IEvent[0].Object==5)&&(IEvent[0].Conditional==1))
		{
			MoveM.Draw(Screen.Back, 408, 346, NDX_RECT(105,163,210,183));
			IEvent[1].Object=5;
			IEvent[1].Conditional=1;			
		}
	}

	if(IEvent[0].Object==31)
	{
		if(IEvent[0].Conditional==1)
		{
			Misc_but.Draw(Screen.Back, 405, 472, NDX_RECT(125, 48, 250, 72));
			IEvent[1].Object=31;
			IEvent[1].Conditional=1;
		}
	}
	else if(IEvent[0].Object==30)
	{
		if(IEvent[0].Conditional==1)
		{
			Barter.Draw(Screen.Back, 270, 472, NDX_RECT(125, 413, 250, 437));
			IEvent[1].Object=30; 
			IEvent[1].Conditional=1;
		}
	}
	else if(IEvent[0].Object==80) // начинаем рисовать нажатые кнопки прокрутки
	{	if(IEvent[0].Conditional==1) {
			Barter.Draw(Screen.Back, 566, 279, NDX_RECT(267, 413, 282, 429));
			IEvent[1].Object=80; 
			IEvent[1].Conditional=1; }
	}
	else if(IEvent[0].Object==81)
	{	if(IEvent[0].Conditional==1) {
			Barter.Draw(Screen.Back, 566, 298, NDX_RECT(299, 413, 314, 429));
			IEvent[1].Object=81; 
			IEvent[1].Conditional=1; }
	}
	else if(IEvent[0].Object==82)
	{	if(IEvent[0].Conditional==1) {
			Barter.Draw(Screen.Back, 221, 279, NDX_RECT(267, 413, 282, 429));
			IEvent[1].Object=82; 
			IEvent[1].Conditional=1; }
	}
	else if(IEvent[0].Object==83)
	{	if(IEvent[0].Conditional==1) {
			Barter.Draw(Screen.Back, 221, 298, NDX_RECT(299, 413, 314, 429));
			IEvent[1].Object=83; 
			IEvent[1].Conditional=1; }
	}
	else if(IEvent[0].Object==84)
	{	if(IEvent[0].Conditional==1) {
			Barter.Draw(Screen.Back, 542, 282, NDX_RECT(330, 413, 343, 426));
			IEvent[1].Object=84; 
			IEvent[1].Conditional=1; }
	}
	else if(IEvent[0].Object==85)
	{	if(IEvent[0].Conditional==1) {
			Barter.Draw(Screen.Back, 542, 298, NDX_RECT(358, 413, 371, 426));
			IEvent[1].Object=85; 
			IEvent[1].Conditional=1; }
	}
	else if(IEvent[0].Object==86)
	{	if(IEvent[0].Conditional==1) {
			Barter.Draw(Screen.Back, 247, 282, NDX_RECT(330, 413, 343, 426));
			IEvent[1].Object=86; 
			IEvent[1].Conditional=1; }
	}
	else if(IEvent[0].Object==87)
	{	if(IEvent[0].Conditional==1) {
			Barter.Draw(Screen.Back, 247, 298, NDX_RECT(358, 413, 371, 426));
			IEvent[1].Object=87; 
			IEvent[1].Conditional=1; }
	}

	if(fade==TRUE)
	{
		NDX_Surface			Fon; //фон

		Fon.Create(&Screen,NDXST_SYSMEM,800, 600);

		Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
	
		for(int i=0; i<17; ++i)
		{
			Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
			Screen.Flip();
			Sleep(10);
		}
	
		fade=false;
	}

	// курсор мышки
	if((drag==0)&&(inv_mode==0))
			ICursor.Draw(Screen.Back,MouseX,MouseY, NDX_RECT(0, cursor_mode*40, 70, cursor_mode*40+40));
	else if((inv_mode!=0))
			ICursor.Draw(Screen.Back,MouseX,MouseY, NDX_RECT(0, cursor_mode*40, 70, cursor_mode*40+40));
}

void ShowSafeCode()
{
	Safe.Draw(Screen.Back, 242, 231, NDX_RECT(0, 0, 316, 137));

	// отобразим цифры
	char s[10];
	wsprintf(s, "%d", safe_cr.c1);
	DisplayBNumber(s, 260, 283);
	wsprintf(s, "%d", safe_cr.c2);
	DisplayBNumber(s, 302, 283);
	wsprintf(s, "%d", safe_cr.c3);
	DisplayBNumber(s, 344, 283);
	wsprintf(s, "%d", safe_cr.c4);
	DisplayBNumber(s, 386, 283);
	wsprintf(s, "%d", safe_cr.c5);
	DisplayBNumber(s, 428, 283);
	wsprintf(s, "%d", safe_cr.c6);
	DisplayBNumber(s, 470, 283);
	wsprintf(s, "%d", safe_cr.c7);
	DisplayBNumber(s, 512, 283);

	// отобразим оставшееся время
	if(safe_cr.timer >=0)
	{
		wsprintf(s, "%d", safe_cr.CheckTimer());
		DisplaySNumber(s, 374, 255);
	}
	else
	{
		if(safe_cr.timer==-1)
		{
			OpenSStream("deny.wav");
			prog_error=true;
			error_msg="Попытка провалена! Попробуйте еще раз.";
			error_id=4;
			safe_cr.timer=-2;
		}
		else if(safe_cr.timer==-3)
		{
			OpenSStream("deny.wav");
			prog_error=true;
			error_msg="Попытка взлома. Сейф заблокирован.";
			error_id=4;
		}

		wsprintf(s, "%d", 0);
		DisplaySNumber(s, 374, 255);
	}

	// отрисуем кнопки, если они были нажаты, и передадим обработчику событий информацию об этом
	if(IEvent[0].Conditional!=0)
	{
		if((IEvent[0].Object==50)&&(IEvent[0].Conditional==1))
		{
			Safe.Draw(Screen.Back, 259, 309, NDX_RECT(14, 137, 27, 145));
			IEvent[1].Object=50;
			IEvent[1].Conditional=1;
		}
		else if((IEvent[0].Object==51)&&(IEvent[0].Conditional==1))
		{
			Safe.Draw(Screen.Back, 301, 309, NDX_RECT(14, 137, 27, 145));
			IEvent[1].Object=51;
			IEvent[1].Conditional=1;
		}
		else if((IEvent[0].Object==52)&&(IEvent[0].Conditional==1))
		{
			Safe.Draw(Screen.Back, 343, 309, NDX_RECT(14, 137, 27, 145));
			IEvent[1].Object=52;
			IEvent[1].Conditional=1;
		}
		else if((IEvent[0].Object==53)&&(IEvent[0].Conditional==1))
		{
			Safe.Draw(Screen.Back, 385, 309, NDX_RECT(14, 137, 27, 145));
			IEvent[1].Object=53;
			IEvent[1].Conditional=1;
		}
		else if((IEvent[0].Object==54)&&(IEvent[0].Conditional==1))
		{
			Safe.Draw(Screen.Back, 427, 309, NDX_RECT(14, 137, 27, 145));
			IEvent[1].Object=54;
			IEvent[1].Conditional=1;
		}
		else if((IEvent[0].Object==55)&&(IEvent[0].Conditional==1))
		{
			Safe.Draw(Screen.Back, 469, 309, NDX_RECT(14, 137, 27, 145));
			IEvent[1].Object=55;
			IEvent[1].Conditional=1;
		}
		else if((IEvent[0].Object==56)&&(IEvent[0].Conditional==1))
		{
			Safe.Draw(Screen.Back, 511, 309, NDX_RECT(14, 137, 27, 145));
			IEvent[1].Object=56;
			IEvent[1].Conditional=1;
		}
		else if((IEvent[0].Object==57)&&(IEvent[0].Conditional==1))
		{
			Safe.Draw(Screen.Back, 277, 284, NDX_RECT(46, 149, 61, 160));
			IEvent[1].Object=57;
			IEvent[1].Conditional=1;
		}
		else if((IEvent[0].Object==58)&&(IEvent[0].Conditional==1))
		{
			Safe.Draw(Screen.Back, 277, 295, NDX_RECT(62, 149, 77, 160));
			IEvent[1].Object=58;
			IEvent[1].Conditional=1;
		}
		else if((IEvent[0].Object==59)&&(IEvent[0].Conditional==1))
		{
			Safe.Draw(Screen.Back, 319, 284, NDX_RECT(46, 149, 61, 160));
			IEvent[1].Object=59;
			IEvent[1].Conditional=1;
		}
		else if((IEvent[0].Object==60)&&(IEvent[0].Conditional==1))
		{
			Safe.Draw(Screen.Back, 319, 295, NDX_RECT(62, 149, 77, 160));
			IEvent[1].Object=60;
			IEvent[1].Conditional=1;
		}
		else if((IEvent[0].Object==61)&&(IEvent[0].Conditional==1))
		{
			Safe.Draw(Screen.Back, 361, 284, NDX_RECT(46, 149, 61, 160));
			IEvent[1].Object=61;
			IEvent[1].Conditional=1;
		}
		else if((IEvent[0].Object==62)&&(IEvent[0].Conditional==1))
		{
			Safe.Draw(Screen.Back, 361, 295, NDX_RECT(62, 149, 77, 160));
			IEvent[1].Object=62;
			IEvent[1].Conditional=1;
		}
		else if((IEvent[0].Object==63)&&(IEvent[0].Conditional==1))
		{
			Safe.Draw(Screen.Back, 403, 284, NDX_RECT(46, 149, 61, 160));
			IEvent[1].Object=63;
			IEvent[1].Conditional=1;
		}
		else if((IEvent[0].Object==64)&&(IEvent[0].Conditional==1))
		{
			Safe.Draw(Screen.Back, 403, 295, NDX_RECT(62, 149, 77, 160));
			IEvent[1].Object=64;
			IEvent[1].Conditional=1;
		}
		else if((IEvent[0].Object==65)&&(IEvent[0].Conditional==1))
		{
			Safe.Draw(Screen.Back, 445, 284, NDX_RECT(46, 149, 61, 160));
			IEvent[1].Object=65;
			IEvent[1].Conditional=1;
		}
		else if((IEvent[0].Object==66)&&(IEvent[0].Conditional==1))
		{
			Safe.Draw(Screen.Back, 445, 295, NDX_RECT(62, 149, 77, 160));
			IEvent[1].Object=66;
			IEvent[1].Conditional=1;
		}
		else if((IEvent[0].Object==67)&&(IEvent[0].Conditional==1))
		{
			Safe.Draw(Screen.Back, 487, 284, NDX_RECT(46, 149, 61, 160));
			IEvent[1].Object=67;
			IEvent[1].Conditional=1;
		}
		else if((IEvent[0].Object==68)&&(IEvent[0].Conditional==1))
		{
			Safe.Draw(Screen.Back, 487, 295, NDX_RECT(62, 149, 77, 160));
			IEvent[1].Object=68;
			IEvent[1].Conditional=1;
		}
		else if((IEvent[0].Object==69)&&(IEvent[0].Conditional==1))
		{
			Safe.Draw(Screen.Back, 529, 284, NDX_RECT(46, 149, 61, 160));
			IEvent[1].Object=69;
			IEvent[1].Conditional=1;
		}
		else if((IEvent[0].Object==70)&&(IEvent[0].Conditional==1))
		{
			Safe.Draw(Screen.Back, 529, 295, NDX_RECT(62, 149, 77, 160));
			IEvent[1].Object=70;
			IEvent[1].Conditional=1;
		}
	}

	// отрисуем кнопки, подтверждающие корректный ввод
	if(safe_cr.cn2)
		Safe.Draw(Screen.Back, 259, 309, NDX_RECT(14, 137, 27, 145));
	if(safe_cr.cn3)
		Safe.Draw(Screen.Back, 301, 309, NDX_RECT(14, 137, 27, 145));
	if(safe_cr.cn4)
		Safe.Draw(Screen.Back, 343, 309, NDX_RECT(14, 137, 27, 145));
	if(safe_cr.cn5)
		Safe.Draw(Screen.Back, 385, 309, NDX_RECT(14, 137, 27, 145));
	if(safe_cr.cn6)
		Safe.Draw(Screen.Back, 427, 309, NDX_RECT(14, 137, 27, 145));
	if(safe_cr.cn7)
		Safe.Draw(Screen.Back, 469, 309, NDX_RECT(14, 137, 27, 145));
	if(safe_cr.cracked)
		Safe.Draw(Screen.Back, 511, 309, NDX_RECT(14, 137, 27, 145));
}

void DisplayMMoveNumber(char* s, UINT x, UINT y)
{
    UINT len = strlen(s);
	char ns[5];

	if(len<5)
	{
		int r=5-len;
		for(UINT t=0; t<5; ++t)
			ns[t] = 0;
	
		for(t=0; t<len; ++t)
			ns[t+r] = s[t];
	}

    // Loop through the entire string.
    for (UINT i=0; i<5; ++i)
    {
        // Convert a digit to an integer.
        char ch = ns[i];
        UINT digit = atoi(&ch);

        // Display the digit as a graphic text character.
		//TargetMsc.Draw(Screen.Back, x+i*9+3, y, NDX_RECT(digit*7, 61, digit*7+7, 77));
		Misc.Draw(Screen.Back, x+i*14+2, y, NDX_RECT(digit*14, 274, digit*14+14, 297));
    }
}

void DisplayBNumber(char* s, UINT x, UINT y)
{
    UINT len = strlen(s);

    // Loop through the entire string.
    for (UINT i=0; i<len; ++i)
    {
        // Convert a digit to an integer.
        char ch = s[i];
        UINT digit = atoi(&ch);

        // Display the digit as a graphic text character.
		Misc.Draw(Screen.Back, x+i*13, y, NDX_RECT(digit*13, 313, digit*13+13, 335));
    }
}

void DisplaySNumber(char* s, UINT x, UINT y)
{
    UINT len = strlen(s);
	char ns[2];

	if(len<2)
	{
		ns[0] = 0;
		ns[1] = s[0];
	}
	else
	{
		ns[0] = s[0];
		ns[1] = s[1];
	}

    // Loop through the entire string.
    for (UINT i=0; i<2; ++i)
    {
        // Convert a digit to an integer.
        char ch = ns[i];
        UINT digit = atoi(&ch);

        // Display the digit as a graphic text character.
		Misc.Draw(Screen.Back, x+i*8+1, y, NDX_RECT(digit*8, 299, digit*8+8, 312));
    }
}

// использовать предмет инвентаря
void UseItem()
{
	switch(plr_Inv.GetType())
	{
		case 3:
			UseMedical();
			break;
		case 2:
			plr_Inv.UseMiscItem(&Screen);
			PrepareFrame(fr);
			break;
		case 5:
			UseFood();
			break;
		case 7:
			UseBook();
			break;
	}
}

void UseMedical()
{
	int min;
	int max;
	int bonus;

	CNukeFile CD;
	CD.SetFileDescription("Fallout Data File");
	CD.Open("CRITTERS.DAT");
	
	if(CD.IsFileOpen())
	{
		CString tmp="";
		tmp = plr_Inv.GetDescription();
		tmp+=".des";
		LPSTR fn;
		fn = tmp.GetBuffer(tmp.GetLength());
		tmp.ReleaseBuffer();

		if(CD.ExtractFile_ToFile(fn,"data\\temp"))
		{
			CIni ini;
			ini.SetPathName(_T("data\\temp"));
			min = ini.GetInt("Main", "MinEffect", 0);
			max = ini.GetInt("Main", "MaxEffect", 0);
			bonus = ini.GetInt("Main", "Bonus", 0);
		}

		DeleteFile("data\\temp");

		CD.Close();

		tmp = plr_Inv.GetDescription();
		tmp+=".WAV";
		OpenSStream(tmp);
	}

	int result = Rand(min, max) + bonus;
	plr_Inv.GetItem(1, plr_Inv.GetDescription(), &Screen);

	pFight->player_HP += result;

	// если мы в бою, то добавим надпись о использовании медикаментов и эффекте
	if(fight)
	{
		CString r = "Ты получил ";
		char num[10];
		itoa(result, num, 10);
		r+=num;
		r+=" очков здоровья.";
		r+="\n";
		battlelog.Insert(0, r);

		WriteFLogText();
	}
	else
	{
		ModifyData("PHealth", GetData("PHealth")+result);
		if(GetData("PHealth")>pFight->player_maxHP)
			ModifyData("PHealth", pFight->player_maxHP);
		DrawStats();
	}
}

void UseFood()
{
	if(fight)
	{
		CString r = "Нельзя использовать во время боя.\n";
		battlelog.Insert(0, r);

		WriteFLogText();
	}
	else
	{
		int min;
		int max;
		int bonus;

		CNukeFile CD;
		CD.SetFileDescription("Fallout Data File");
		CD.Open("CRITTERS.DAT");
		
		if(CD.IsFileOpen())
		{
			CString tmp="";
			tmp = plr_Inv.GetDescription();
			tmp+=".des";
			LPSTR fn;
			fn = tmp.GetBuffer(tmp.GetLength());
			tmp.ReleaseBuffer();

			if(CD.ExtractFile_ToFile(fn,"data\\temp"))
			{
				CIni ini;
				ini.SetPathName(_T("data\\temp"));
				min = ini.GetInt("Main", "MinEffect", 0);
				max = ini.GetInt("Main", "MaxEffect", 0);
				bonus = ini.GetInt("Main", "Bonus", 0);
			}

			DeleteFile("data\\temp");

			CD.Close();

			tmp = plr_Inv.GetDescription();
			tmp+=".WAV";
//			OpenSStream(tmp);
		}

		int result = Rand(min, max) + bonus;
		plr_Inv.GetItem(1, plr_Inv.GetDescription(), &Screen);

		pFight->player_HP += result;

		ModifyData("PHealth", GetData("PHealth")+result);
		if(GetData("PHealth")>pFight->player_maxHP)
			ModifyData("PHealth", pFight->player_maxHP);
		DrawStats();
	}
}

void UseBook()
{
	if(fight)
	{
		CString r = "Нельзя использовать во время боя.\n";
		battlelog.Insert(0, r);

		WriteFLogText();
	}
	else
	{
		int min;
		int max;
		int bonus;

		CNukeFile CD;
		CD.SetFileDescription("Fallout Data File");
		CD.Open("CRITTERS.DAT");
		
		if(CD.IsFileOpen())
		{
			CString tmp="";
			tmp = plr_Inv.GetDescription();
			tmp+=".des";
			LPSTR fn;
			fn = tmp.GetBuffer(tmp.GetLength());
			tmp.ReleaseBuffer();

			if(CD.ExtractFile_ToFile(fn,"data\\temp"))
			{
				CIni ini;
				ini.SetPathName(_T("data\\temp"));
				min = ini.GetInt("Main", "MinEffect", 0);
				max = ini.GetInt("Main", "MaxEffect", 0);
				bonus = ini.GetInt("Main", "Bonus", 0);
			}

			DeleteFile("data\\temp");

			CD.Close();

			tmp = plr_Inv.GetDescription();
			tmp+=".WAV";
//			OpenSStream(tmp);
		}

		int result = Rand(min, max) + bonus;
		plr_Inv.GetItem(1, plr_Inv.GetDescription(), &Screen);

		pFight->player_SDMGfrom += min;
		pFight->player_DMGfrom = pFight->player_SDMGfrom;
		pFight->player_SDMGto += result;
		pFight->player_DMGto = pFight->player_SDMGto;

		prog_error=true;
		error_msg="Ваш уровень рукопашного боя повысился.";
		error_id=4;
	}
}

int Rand(int min, int max)
{
	return (double)rand() / (RAND_MAX + 1) * (max - min) + min;
}

void DrawOptions()
{
	if(!mute)
		Misc.Draw(Screen.Back, 759, 9, NDX_RECT(50,43,67,54));
	else
		Misc.Draw(Screen.Back, 759, 9, NDX_RECT(68,43,85,54));

	int k = lightning_bar*16.4;
	if(k>256)
		k=256;

	Misc.DrawTranslucent(Screen.Back, 357-tbright, 10, NDX_RECT(135-tbright,23,135,32), 92+k);
	Misc.DrawTranslucent(Screen.Back, 655, 10, NDX_RECT(35,33,35+(svolume),42), 92+k);

	Misc.Draw(Screen.Back, 239, 10, NDX_RECT(35,43,49,52));

	// нарисуем кнопку будильника
	Misc.Draw(Screen.Back, 118, 12, NDX_RECT(91,0,116,22));

	if((IEvent[0].Object==35)&&(IEvent[0].Conditional==1))
	{
		Misc.Draw(Screen.Back, 118, 12, NDX_RECT(117,0,142,22));
		IEvent[1].Object=35;
		IEvent[1].Conditional=1;
	}

	// нарисуем кол-во оставшихся ходов (используется в некоторых квестах)
	if(GetData("turns")>0)
	{
		float segment = 137 / GetData("turns_max");
		Misc.Draw(Screen.Back, 608, 54, NDX_RECT(35,55,segment*GetData("turns"),69));
	}
}

void DrawStats()
{
	CString r="HP: ";
	char num[10];
	itoa(GetData("PHealth"), num, 10);
	r+=num;
	r+="/";
	itoa(pFight->player_maxHP, num, 10);
	r+=num;

	LPSTR fn;
	fn = r.GetBuffer(r.GetLength());
	r.ReleaseBuffer();

	Stats.Clear(RGB(0,0,0));
	Stats.FormattedText(10,5,RGB(0, 255, 0),fnt2, fn);
}

///////////////////////////////////////////////////////////
// 
//
//				Загрузка / сохранение игры
// 
//
///////////////////////////////////////////////////////////

void Save_load()
{
	Saveload.Draw(Screen.Back, 0, 0);

	if(!save)
		Saveload.Draw(Screen.Back, 95, 39, NDX_RECT(0, 601, 164, 618));
	else
		Saveload.Draw(Screen.Back, 94, 39, NDX_RECT(0, 619, 164, 636));

	Saveload.Draw(Screen.Back, 465, 445, NDX_RECT(230, 600, 355, 624));
	Saveload.Draw(Screen.Back, 600, 445, NDX_RECT(230, 624, 355, 648));

	Temp.Draw(Screen.Back, 74, 83);

	if((anim!=0)&&/*(!save)&&*/(changes))
//		Image.DrawRotoZoom(Screen.Back,598,140,0,1.4,NDX_RECT(0,0,140,100));
		FotoImg.DrawTranslucent(Screen.Back, 493, 63, 200);

	if(IEvent[0].Object==11)
	{
		if(IEvent[0].Conditional==1)
		{
			Saveload.Draw(Screen.Back, 465, 445, NDX_RECT(355, 600, 480, 624));
			Click();
			IEvent[1].Object=11;
			IEvent[1].Conditional=1;
		}
	}
	else if(IEvent[0].Object==12)
	{
		if(IEvent[0].Conditional==1)
		{
			Saveload.Draw(Screen.Back, 600, 445, NDX_RECT(355, 624, 480, 648));
			Click();
			IEvent[1].Object=12;
			IEvent[1].Conditional=1;
		}
	}

	if(fade==TRUE)
	{
		NDX_Surface			Fon; //фон

		Fon.Create(&Screen,NDXST_SYSMEM,800, 600);

		Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
	
		for(int i=0; i<17; ++i)
		{
			Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
			Screen.Flip();
			Sleep(10);
		}
	
		fade=false;
	}

	// курсор мышки
	if(!showcursor)
		Cursor.Draw(Screen.Back,MouseX,MouseY);	
}

void PrepareSaveload(int e)
{
	Temp.Create(&Screen,NDXST_SYSMEM, 650, 460);
	Temp.SetColorKey(RGB(0,0,0));
	Temp.SetTransP(true);

	Temp.Clear(RGB(0,0,0));

	changes=false;

	CFileFind finder;
	CFile file;

	CString stime, gtime, location, txt;
	int image;
	
	for(int i=1; i<=10; ++i)
	{
		CString r="data\\SAVEGAME\\save";
		char num[10];
		itoa(i, num, 10);
		r+=num;
		r+=".sav";

		if(finder.FindFile(r))
		{
			if(i<10)
				Temp.FormattedText(25,7+(i-1)*46,RGB(207, 191, 63),fnt1,"[   ЗАГРУЗИТЬ 0%u:   ]", i);
			else
				Temp.FormattedText(25,7+(i-1)*46,RGB(207, 191, 63),fnt1,"[   ЗАГРУЗИТЬ %u:   ]", i);

			BOOL opened = file.Open(r, CFile::modeRead);

			if (opened)
			{
				CArchive ar(&file, CArchive::load);

				ar >> stime;
				ar >> image;
				ar >> location;
				ar >> gtime;
				ar >> txt;
				
				ar.Close();
				file.Close();

				LPSTR tmp;
				tmp = stime.GetBuffer(stime.GetLength());
				stime.ReleaseBuffer();

				Temp.FormattedText(30,25+(i-1)*46,RGB(207, 191, 63),fnt1, tmp);
			}
		}
		else
		{
			if(i<10)
				Temp.FormattedText(25,7+(i-1)*46,RGB(207, 191, 63),fnt1,"[   ЗАГРУЗИТЬ 0%u:   ]", i);
			else
				Temp.FormattedText(25,7+(i-1)*46,RGB(207, 191, 63),fnt1,"[   ЗАГРУЗИТЬ %u:   ]", i);
			Temp.FormattedText(30,25+(i-1)*46,RGB(207, 191, 63),fnt1,"пусто");
		}
	}


	if((e>0)&&(e<=10))
	{
		CString r="data\\SAVEGAME\\save";
		char num[10];
		itoa(e, num, 10);
		r+=num;
		r+=".sav";

		if(finder.FindFile(r))
		{
/*			if(i<10)
				Temp.FormattedText(25,7+(e-1)*46,RGB(226, 236, 162),fnt1,"[   ЗАГРУЗИТЬ 0%u:   ]", i);
			else
				Temp.FormattedText(25,7+(e-1)*46,RGB(226, 236, 162),fnt1,"[   ЗАГРУЗИТЬ %u:   ]", i);*/

			BOOL opened = file.Open(r, CFile::modeRead);

			if (opened)
			{
				CArchive ar(&file, CArchive::load);

				ar >> stime;
				ar >> image;
				ar >> location;
				ar >> gtime;
				ar >> txt;
				
				ar.Close();
				file.Close();

				ChangeLocation(image);

				FotoImg.Create(&Screen, NDXST_SYSMEM, 209, 155);
				NDX_Surface tmps;
				tmps.Create(&Screen, NDXST_SYSMEM, 209, 155);
				tmps.Clear(RGB(255,255,255));
				Image.DrawRotoZoom(&tmps,104,77,0,1.4,NDX_RECT(0,0,140,100));
				tmps.DrawColorMix(&FotoImg, 0, 0, RGB(192,192,192), 165);

				LPSTR tmp;
				tmp = stime.GetBuffer(stime.GetLength());
				stime.ReleaseBuffer();

				Temp.FormattedText(30,25+(e-1)*46,RGB(226, 236, 162),fnt1, tmp);

				tmp = location.GetBuffer(location.GetLength());
				location.ReleaseBuffer();

				Temp.FormattedText(416,222,RGB(226, 236, 162),fnt1, tmp);

				tmp = gtime.GetBuffer(gtime.GetLength());
				gtime.ReleaseBuffer();

				Temp.FormattedText(416,242,RGB(226, 236, 162),fnt1, tmp);

				tmp = txt.GetBuffer(txt.GetLength());
				txt.ReleaseBuffer();

				Temp.FormattedText(416,262,RGB(226, 236, 162),fnt2, tmp);
				changes=true;
			}			
		}
		else
		{
		/*	if(i<10)
				Temp.FormattedText(25,7+(e-1)*46,RGB(226, 236, 162),fnt1,"[   ЗАГРУЗИТЬ 0%u:   ]", i);
			else
				Temp.FormattedText(25,7+(e-1)*46,RGB(226, 236, 162),fnt1,"[   ЗАГРУЗИТЬ %u:   ]", i);*/
			Temp.FormattedText(30,25+(e-1)*46,RGB(226, 236, 162),fnt1,"пусто");
			changes=false;
		}
	}

	anim = e;

}

void LoadGame(CString fn)
{
	CFile file;
	CString fname="data\\SAVEGAME\\";
	fname+=fn;

	BOOL opened = file.Open(fname, CFile::modeRead);

	CString temp;
	int tmp;

	if (opened)
	{
		InitGameValues();

		CArchive ar(&file, CArchive::load);

		ar >> temp;
		ar >> tmp;
		ar >> temp;
		ar >> temp;
		ar >> temp;
		ar >> fr;
		ar >> tmp;

		
		for (UINT i=0; i<tmp; ++i)
		{
			CDatas* pD = new CDatas;

			ar >> pD->value;
			ar >> pD->vname;

			QData.Add(pD);
		}

		ar >> tmp;

		for (i=0; i<tmp; ++i)
		{
			CQuest* pQuest = new CQuest;

			ar >> pQuest->qtitle;
			ar >> pQuest->qstatus;

			Quests.Add(pQuest);
		}

		ar >> tmp;

		for (i=0; i<tmp; ++i)
		{
			int amount;
			CString desc;

			ar >> amount;
			ar >> desc;

			plr_Inv.AddItem(amount, desc, &Screen);
		}

		ar >> temp;
		pFight->LoadWeapon(temp, &Screen);

		ar.Close();
		file.Close();

		NDX_Surface			Fon; //фон
		Fon.Create(&Screen,NDXST_SYSMEM,800, 600);
		
		Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
		
		for(i=17; i>0; --i)
		{
			Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
			Screen.Flip();
			Sleep(30);
		}
		
		save=false;
		Sleep(200);
		StopMStream();
		OpenMStream("data\\sound\\music\\15shady.acm");
		LoadScenario();
		PrepareFrame(fr);
		PlayMStream();
		mode=2;
		fade=true;
	}
}

void SaveGame(CString fn)
{
	CFile file;
	CString fname="data\\SAVEGAME\\";
	fname+=fn;

	BOOL opened = file.Open(fname, CFile::modeCreate | CFile::modeWrite);

	CTime t = CTime::GetCurrentTime();
	CString time = t.Format("%d.%m.%y, %H : %M");
	
	if (opened)
	{
		CArchive ar(&file, CArchive::store);

		ar << time;
		ar << Level[fr].image;
		CString r = Level[fr].location;
		r+="  ::  ";
		r+=Level[fr].sublocation;
		ar << r;
		r="";
		char num[10];
		if(Level[fr].day<10)
			r+="0";
		itoa(Level[fr].day, num, 10);
		r+=num;
		r+=" . ";
		if(Level[fr].month<10)
			r+="0";
		itoa(Level[fr].month, num, 10);
		r+=num;
		r+=" . ";
		itoa(Level[fr].year, num, 10);
		r+=num;
		ar << r;
		r="";
		for(int i=0; i<=35;++i)
		{
			r+=Level[fr].text_data[i];
		}
		r+="...";

		ar << r;
		ar << fr;
		ar << QData.GetSize();
			
		for (i=0; i<QData.GetSize(); ++i)
		{
			CDatas* pD = (CDatas*) QData.GetAt(i);

			ar << pD->value;
			ar << pD->vname;
		}

		ar << Quests.GetSize();

		for (i=0; i<Quests.GetSize(); ++i)
		{
			CQuest* pQuest = (CQuest*) Quests.GetAt(i);
			ar << pQuest->qtitle;
			ar << pQuest->qstatus;
		}

		ar << plr_Inv.m_Inventory.GetSize();

		for(i=0; i<plr_Inv.m_Inventory.GetSize(); ++i)
		{
			ar << plr_Inv.GetAmount(i);
			ar << plr_Inv.GetDescription(i);
		}

		ar << pFight->Weapon[0].player_weapon;

		ar.Close();
		file.Close();
	}

	NDX_Surface			Fon; //фон
	Fon.Create(&Screen,NDXST_SYSMEM,800, 600);
					
	Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
					
	for(int i=17; i>0; --i)
	{
		Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
		Screen.Flip();
		Sleep(30);
	}
	fade=true;

	save=false;
	mode=last_mode;
}

///////////////////////////////////////////////////////////
// OnDestroy()
//
// This function cleans up after the program when the
// application receives a WM_DESTROY message from Windows.
///////////////////////////////////////////////////////////
void OnDestroy()
{
	OptionsCFG(true);
	DeleteSound();
	MD.Close();
	AAFont.Close();
	AFont.Close();
	RealiseScriptSystem();
	pFight->~CFight();

	NDX_DeleteFont(fnt1);
	NDX_DeleteFont(fnt2);
}

void OnTimer()
{
	if(Window.Active)
	{
		if((mode==1)||(mode==-2))
		{
			int v = GetMVolume();
			if(v < svolume)
				SetMVolume(v + 1);
		}
		if((mode==2)||(mode==4)||(mode==3))
		{
			if((anim==1)&&(Interface[0].iAni))
			{
				if(Image.Height/100-1>Interface[0].iFrame)
				{
					Interface[0].iFrame++;
				}
				else if(Image.Height/100-1==Interface[0].iFrame)
				{
					Interface[0].iFrame=0;
					Interface[0].iAni=false;
				}
			}
			else if((anim==1)&&(!Interface[0].iAni))
			{
				UINT an;
				an = rand() % 8;
	
				if(an == 3)
					Interface[0].iAni=true;
			}	
	
			if(!fight)
			{
				if((anim==2)&&(Interface[0].iAni))
				{
					if(2<Interface[0].iFrame)
						Interface[0].iFrame++;
					else
					{
						Interface[0].iFrame=0;
						Interface[0].iAni=false;
					}
				}
				else if((anim==2)&&(!Interface[0].iAni))
				{
					UINT an;
					an = rand() % 50;	

					if(an == 18)
						Interface[0].iAni=true;
				}
			}
			else
			{
				if(Interface[0].iFrame<38)
					Interface[0].iFrame++;
				else
					Interface[0].iFrame=0;
			}

			if(lightning_bar>0)
				lightning_bar--;

			if(GetData("Rest")>0)
				ModifyData("Rest", GetData("Rest")-1);

			if(GetData("PHealth")<pFight->player_maxHP)
			{
				if(hp_interval>0)
				{
					hp_interval--;
				}
				else
				{
					hp_interval = 400;
					ModifyData("PHealth",GetData("PHealth")+1);
					DrawStats();
				}
			}

			if(sc_timer)
			{
				if(sc_time > 0)
					sc_time--;
				else
				{
					sc_timer = false;
					PrepareFrame(sc_link);
				}
			}

			if(Notify.on)
			{
				if(Notify.loop>0)
				{
					switch(Notify.loop)
					{
						case 1:
							if(Notify.period>0)
								Notify.period -= 64;
							else
								Notify.loop=0;
							break;

						case 2:
							if(Notify.period<320)
								Notify.period += 64;
							else
								Notify.loop=1;
							break;

						case 3:
							if(Notify.period>0)
								Notify.period -= 64;
							else
								Notify.loop=2;
							break;

						case 4:
							if(Notify.period<320)
								Notify.period += 64;
							else
								Notify.loop=3;
							break;

						case 5:
							if(Notify.period>0)
								Notify.period -= 64;
							else
								Notify.loop=4;
							break;
						
						case 6:
							if(Notify.period<320)
								Notify.period += 64;
							else
								Notify.loop=5;
							break;
					}
				}
				else
					Notify.on = false;

			}
		}
	}
}

void OnActivateApp(WPARAM wParam)
{
	if(wParam)
	{
		Window.Active=true;
		wActive=true;

		if(codecrack)
		{
			safe_cr.period = safe_cr.ReserveTime;
			safe_cr.Timer.StartNow();
		}
	}
	else
	{
		Window.Active=false;
		wActive=false;

		if(codecrack)
			safe_cr.ReserveTime = safe_cr.timer;
	}

	PauseMStream(wActive);
}

void OptionsCFG(bool f_save)
{
	CIni ini;
	ini.SetPathName("data\\falloutxp.cfg");

	if(f_save)
	{
		ini.WriteBool("preferences", "fullscreen", fullscreen);
		ini.WriteInt("preferences", "brightness", tbright);
		ini.WriteInt("preferences", "volume", svolume);
	}
	else
	{
		fullscreen = ini.GetBool("preferences", "fullscreen", true);
		tbright = ini.GetInt("preferences", "brightness", 75);
		svolume = ini.GetInt("preferences", "volume", 100);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//
//				Обработка скриптов
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

void InitScriptSystem()
{
	luaVM = lua_open();
	
	if (NULL == luaVM)
		MessageBox(hWindow, "Failed to create Lua VM.", "Error", MB_OK);
	else
	{
		luaL_openlibs ( luaVM );

		luaL_dostring(luaVM, "math.randomseed (os.clock());");

		lua_register(luaVM, "FadeToBlack", s_FadeToBlack);
		lua_register(luaVM, "PlayerDeath", s_PlayerDeath);
		lua_register(luaVM, "FlashMemory", s_FlashMemory);
		lua_register(luaVM, "SetGlobals", s_SetGlobals);
		lua_register(luaVM, "GetGlobals", s_GetGlobals);
		lua_register(luaVM, "AssociateGameGlobals", s_AssociateGlobals);
		lua_register(luaVM, "FreeGlobals", s_FreeGlobals);
		lua_register(luaVM, "LinkTo", s_ChangeLink);
		lua_register(luaVM, "StartFight", s_StartFight);
		lua_register(luaVM, "LoadFight", s_LoadFight);
		lua_register(luaVM, "RulesFight", s_RulesFight);
		lua_register(luaVM, "AddQuest", s_AddQuest);
		lua_register(luaVM, "Play3DSound", s_Play3DSound);
		lua_register(luaVM, "SetMVolume", s_SetMVolume);
		lua_register(luaVM, "AddItem", s_AddItem);
		lua_register(luaVM, "NewInventory", s_NewInventory);
		lua_register(luaVM, "OpenInventory", s_OpenInventory);
		lua_register(luaVM, "OpenBarter", s_OpenBarter);
		lua_register(luaVM, "InitPInventory",s_InitPInventory);
		lua_register(luaVM, "ShowImage",s_ShowImage);
		lua_register(luaVM, "IsItemPresent", s_IsItemPresent);
		lua_register(luaVM, "GetPItem", s_GetPItem);
		lua_register(luaVM, "AddPItem", s_AddPItem);
		lua_register(luaVM, "StartSafeBreak", s_StartSafeBreak);

		lua_register(luaVM, "NewTable", s_NewTable);
		lua_register(luaVM, "SetElement", s_SetElement);
		lua_register(luaVM, "GetElement", s_GetElement);
		lua_register(luaVM, "SetTViewMarker", s_SetTViewMarker);
		lua_register(luaVM, "DrawMarkers", s_DrawMarkers);
		lua_register(luaVM, "DrawMarker", s_DrawMarker);

		lua_register(luaVM, "SetTimer", s_SetTimer);
		lua_register(luaVM, "Debug", s_Debug);
	}
}

int s_FadeToBlack(lua_State* luaVM)
{
	NDX_Surface			Fon; //фон

	Fon.Create(&Screen,NDXST_SYSMEM,800, 600);

	Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
	
	for(int i=17; i>0; --i)
	{
		Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
		Screen.Flip();
		Sleep(70);
	}

	fade=true;

	lua_pushnumber( luaVM, 0 );
	return 1;
}

int s_PlayerDeath(lua_State* luaVM)
{
	StopMStream();

	NDX_Surface			Fon; //фон

	Fon.Create(&Screen,NDXST_SYSMEM,800, 600);

	Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
	
	for(int i=17; i>0; --i)
	{
		Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(255,255,255), i*15);
		Screen.Flip();
		Sleep(70);
	}

	OpenDataFile();
	Fon.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("INGAME/death.bmp"),true);
	MD.Close();

	OpenMStream("data\\sound\\music\\death.acm");
	SetMVolume(1);
	PlayMStream();

	for(i=0; i<17; ++i)
	{
		Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(255,255,255), i*15);
		Screen.Flip();
		SetMVolume(GetMVolume()+15);
		Sleep(120);
	}


	Sleep(4000);

	for(i=17; i>0; --i)
	{
		Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
		Screen.Flip();
		SetMVolume(GetMVolume()-15);
		Sleep(120);
	}

	fade=true;

	prog_exit=false;
	StopMStream();
	OpenMStream("data\\sound\\music\\menu.acm");
	SetMPosition();
	SetMVolume(1);
	PlayMStream();
	mode=1;

	lua_pushnumber( luaVM, 0 );
	return 1;
}

int s_FlashMemory(lua_State* luaVM)
{
	CString st = "INGAME/";
	st += lua_tostring(luaVM, -1);
	st+= ".bmp";

	LPSTR fn;
	fn = st.GetBuffer(st.GetLength());
	st.ReleaseBuffer();

	NDX_Surface			Fon; //фон
	NDX_Surface			Last; //то, что было до этого

	Fon.Create(&Screen,NDXST_SYSMEM,800, 600);
	Last.Create(&Screen,NDXST_SYSMEM,800, 600);

	Screen.Back->Draw(&Last,0,0,NDX_RECT(0,0,800,600));

	OpenSStream("FLASH.WAV");

	OpenDataFile();
	Fon.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile(fn),true);
	MD.Close();

	Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(255,255,255), 0);
	Screen.Flip();
	Sleep(5);
	for(int i=0; i<5; ++i)
	{
		Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(255,255,255), i*51);
		Screen.Flip();
		SetMVolume(GetMVolume()+15);
		Sleep(20);
	}
	Sleep(40);
	for(i=5; i>0; --i)
	{
		Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(255,255,255), i*51);
		Screen.Flip();
		SetMVolume(GetMVolume()+15);
		Sleep(10);
	}
	Last.Draw(Screen.Back, NDX_RECT(0, 0, 800, 600));
	Screen.Flip();

	lua_pushnumber( luaVM, 0 );
	return 1;
}

int s_ChangeLink(lua_State* luaVM)
{
	PrepareFrame(lua_tonumber(luaVM, -1));

	changes=true;

	lua_pushnumber( luaVM, 0 );
	return 1;
}

void CheckScript(CString script)
{
	if(!script.IsEmpty())
	{
		LPSTR sc = script.GetBuffer(script.GetLength());
		script.ReleaseBuffer();

//		SetGlobals();
		
		int oldtop = lua_gettop(luaVM);

		luaL_dostring(luaVM, sc);

//		GetGlobals();
		lua_settop(luaVM, oldtop);
	}
}

void RealiseScriptSystem()
{
	lua_close(luaVM);
}

void SetGlobals()
{
	lua_newtable(luaVM);

	for(int i=0; i<QData.GetSize(); ++i)
	{
		lua_pushnumber(luaVM, i);
		lua_pushnumber(luaVM, GetQDValue(i));
		lua_rawset(luaVM, -3);
	}

	lua_setglobal(luaVM, "QData");


/*	lua_pushnumber(luaVM, 1);
	lua_pushnumber(luaVM, s_PlayerName);
	lua_rawset(luaVM, -3);

	lua_pushliteral(luaVM, "n");
	lua_pushnumber(luaVM, 1);
	
	lua_rawset(luaVM, -3);*/
}

int s_SetGlobals(lua_State* luaVM)
{
	if(lua_isstring(luaVM, -2)==1)
	{
		ModifyData(lua_tostring(luaVM, -2), lua_tonumber(luaVM, -1));		
	}

/*	CString s;
	char num[10];
	itoa(index, num, 10);
	s=num;
	itoa(QData[index].value, num, 10);
	s+=" ";
	s+=num;
	AfxMessageBox(s);*/

	lua_pushnumber( luaVM, 0 );
	return 1;
}

int s_GetGlobals(lua_State* luaVM)
{
	float result=0;

	if(lua_isstring(luaVM, -1)==1)
	{
		result = GetData(lua_tostring(luaVM, -1));
	}

	lua_pushnumber( luaVM, result );
	return 1;
}

int s_AssociateGlobals(lua_State* luaVM)
{
/*	int index=-1;
	if(lua_isnumber(luaVM, -1)==1)
		index = lua_tonumber(luaVM, -1);

	if(lua_isstring(luaVM, -2)==1)
	{
		if((index>=0)&&(lua_tostring(luaVM, -2)!=""))
			QData[index].vname=lua_tostring(luaVM, -2);
	}
	
	lua_pushnumber( luaVM, 0 );*/
	return 1;
}

int s_FreeGlobals(lua_State* luaVM)
{
	if(lua_isstring(luaVM, -1)==1)
		DeleteData(lua_tostring(luaVM, -1));

	lua_pushnumber( luaVM, 0 );
	return 1;
}

int s_NewTable(lua_State* luaVM) // очистим массив
{
	for(int i=0; i<10; ++i)
	{
		for(int j=0; j<10; ++j)
		{
			sc_array[i][j] = 0;
		}
	}

	lua_pushnumber( luaVM, 0 );
	return 1;
}

int s_SetElement(lua_State* luaVM) // установим значение элемента (индекс, индекс, значение)
{
	int i = lua_tonumber(luaVM, -3);
	int j = lua_tonumber(luaVM, -2);
	sc_array[i][j] = lua_tonumber(luaVM, -1);
	
	lua_pushnumber( luaVM, 0 );
	return 1;
}

int s_GetElement(lua_State* luaVM) // вернём значение элемента (индекс, индекс)
{
	int result=0;
	int i = lua_tonumber(luaVM, -2);
	int j = lua_tonumber(luaVM, -1);

	result = sc_array[i][j];
	
	lua_pushnumber( luaVM, result );
	return 1;
}

int s_SetTViewMarker(lua_State* luaVM) // установим отображение маркеров по значению таблицы
{
	Temp.Create(&Screen,NDXST_SYSMEM, 140, 100);
	Temp.SetColorKey(RGB(0,0,0));
	Temp.SetTransP(true);
	Temp.Clear(RGB(0,0,0));

	if(table_mark)
		table_mark=false;
	else
		table_mark=true;

	lua_pushnumber( luaVM, 0 );
	return 1;
}

int s_DrawMarkers(lua_State* luaVM) // установим отображение маркеров по значению таблицы
{
	if(table_mark)
	{
		Temp.Create(&Screen,NDXST_SYSMEM, 140, 100);
		Temp.SetColorKey(RGB(0,0,0));
		Temp.SetTransP(true);
		Temp.Clear(RGB(0,0,0));

		NDX_Surface marker;
		CString r = "INGAME/MARKERS/";
		r += lua_tostring(luaVM, -3);
		LPSTR fn;
		fn = r.GetBuffer(r.GetLength());
		r.ReleaseBuffer();
		OpenDataFile();
		marker.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile(fn),true);
		marker.SetColorKey(Screen.PixelFormat->Rgb(255,0,255));
		marker.SetTransP(true);

		int i = lua_tonumber(luaVM, -6);
		int j = lua_tonumber(luaVM, -5);

		for(int x=0; x<i; ++x)
		{
			for(int y=0; y<j; ++y)
			{
				if(sc_array[x][y] == lua_tonumber(luaVM, -4))
				{
					marker.DrawRotoZoom(&Temp, x*lua_tonumber(luaVM, -8) + lua_tonumber(luaVM, -8)/2 + lua_tonumber(luaVM, -2), y*lua_tonumber(luaVM, -7) + lua_tonumber(luaVM, -7)/2 + lua_tonumber(luaVM, -1), 0, 1);
				}
			}
		}
	}

	lua_pushnumber( luaVM, 0 );
	return 1;
}

int s_DrawMarker(lua_State* luaVM) // нарисуем маркер в заданном месте
{
	if(table_mark)
	{
		if(lua_tonumber(luaVM, -4) == 1)
		{
			Temp.Create(&Screen,NDXST_SYSMEM, 140, 100);
			Temp.SetColorKey(RGB(0,0,0));
			Temp.SetTransP(true);
			Temp.Clear(RGB(0,0,0));
		}

		NDX_Surface marker;
		CString r = "INGAME/MARKERS/";
		r += lua_tostring(luaVM, -1);
		LPSTR fn;
		fn = r.GetBuffer(r.GetLength());
		r.ReleaseBuffer();
		OpenDataFile();
		marker.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile(fn),true);
		marker.SetColorKey(Screen.PixelFormat->Rgb(255,0,255));
		marker.SetTransP(true);

		marker.DrawRotoZoom(&Temp, lua_tonumber(luaVM, -3), lua_tonumber(luaVM, -2), 0, 1);		
	}

	lua_pushnumber( luaVM, 0 );
	return 1;
}

int s_StartFight(lua_State* luaVM)
{
	int stats[11];

	int i = lua_gettop(luaVM);

	cur_target=0;

	CString name = "TARGET/";
	name += lua_tostring(luaVM, i);
	name += ".bmp";

	pFight->Enemy[cur_target].target_type = lua_tostring(luaVM, i);
	pFight->targets=1;

	LPSTR fn;
	fn = name.GetBuffer(name.GetLength());
	name.ReleaseBuffer();

	OpenDataFile();
	TImages[0].Target.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile(fn), true); // загрузим изображение цели

/*	name = "TXT/Russian/";
	name += pFight->Enemy[cur_target].target_type;
	name += ".txt";

	fn = name.GetBuffer(name.GetLength());
	name.ReleaseBuffer();

	MD.ExtractFile_ToFile(fn,"tempt");*/
	pFight->LoadText();

	MD.Close();

//	DeleteFile("tempt");

	name = lua_tostring(luaVM, i-1);
	
	for(int k=0; k<i-2; ++k)
		stats[k] = lua_tonumber(luaVM, k+1);

	pFight->InitMeleeFight(stats, name);

	fight=true;
	
	battlelog = "Бой с "+name+" начался. \n";

	PrepareFightFrm();

	OpenSStream("combatStart.wav");

//	CString s;
//	char num[10];
//	for(k=0; k<12; ++k)
//	{
//	itoa(index, num, 10);
//	s=num;
//	itoa(stats[k], num, 10);
//	s+=" ";
//	s+=num;
//	}
//	AfxMessageBox(s);

	lua_pushnumber( luaVM, 0 );
	return 1;
}

int s_RulesFight(lua_State* luaVM)
{
	int i = lua_gettop(luaVM);
	pFight->p_hpl = lua_tonumber(luaVM, i);
	pFight->t_hpl = lua_tonumber(luaVM, i-1);
	pFight->l_tw = lua_tonumber(luaVM, i-2);
	pFight->l_pw = lua_tonumber(luaVM, i-3);

	lua_pushnumber( luaVM, 0 );
	return 1;
}

int s_LoadFight(lua_State* luaVM)
{
	pFight->LoadScene(lua_tostring(luaVM, -1));
	pFight->LoadText();

	pFight->player_SKILL = GetData("PMSkill");
	pFight->player_HP = GetData("PHealth");
	pFight->player_mHP = GetData("PHealth");
	pFight->player_AP = GetData("PAgility");
	pFight->player_cAP = GetData("PAgility");
	pFight->player_DMGto = GetData("PMDMGt");
	pFight->player_DMGplus = GetData("PMDMGp");
	pFight->player_APPenalty=0;
	pFight->player_requed_distance=1;

	for(int i=0; i<pFight->targets; ++i)
	{
		CString name = "TARGET/";
		name += pFight->Enemy[i].target_type;
		name += ".bmp";
		
		LPSTR fn = name.GetBuffer(name.GetLength());
		name.ReleaseBuffer();

		OpenDataFile();
		TImages[i].Target.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile(fn), true); // загрузим изображение цели
		
		MD.Close();
	}

	cur_target=0;

	fight=true;
	
	battlelog = "Бой начался. \n";

	PrepareFightFrm();

	OpenSStream("combatStart.wav");

	lua_pushnumber( luaVM, 0 );
	return 1;
}

int s_AddQuest(lua_State* luaVM)
{
	int i = lua_gettop(luaVM);
	AddQuest(lua_tostring(luaVM, i), lua_tonumber(luaVM, i-1));

	Notify.on = true;
	Notify.loop = 6;
	Notify.period = 0;
	
	lua_pushnumber( luaVM, 0 );
	return 1;
}

int s_SetMVolume(lua_State* luaVM)
{
	SetMVolume(lua_tonumber(luaVM, -1));
	
	lua_pushnumber( luaVM, 0 );
	return 1;
}

int s_NewInventory(lua_State* luaVM)
{
	obj_Inv.ClearAll(lua_tostring(luaVM, -2), lua_tostring(luaVM, -1), &Screen);
		
	lua_pushnumber( luaVM, 0 );
	return 1;
}

int s_InitPInventory(lua_State* luaVM)
{
	plr_Inv.ClearAll(lua_tostring(luaVM, -1), "player_1", &Screen);
		
	lua_pushnumber( luaVM, 0 );
	return 1;
}

int s_AddItem(lua_State* luaVM)
{
	obj_Inv.AddItem(lua_tonumber(luaVM, -2), lua_tostring(luaVM, -1), &Screen);
			
	lua_pushnumber( luaVM, 0 );
	return 1;
}

int s_AddPItem(lua_State* luaVM)
{
	ModifyData("Inventory", 1);
	plr_Inv.AddItem(lua_tonumber(luaVM, -2), lua_tostring(luaVM, -1), &Screen);
			
	lua_pushnumber( luaVM, 0 );
	return 1;
}

int s_OpenInventory(lua_State* luaVM)
{
	last_mode = mode;
	mode = 4;
	inv_mode = 0;

	obj_Inv.InitInventory();
	plr_Inv.InitInventory();

	lua_pushnumber( luaVM, 0 );
	return 1;
}

int s_OpenBarter(lua_State* luaVM)
{
	last_mode = mode;
	mode = 5;
	inv_mode = 0;

	obj_Inv.m_Inventory.RemoveAll();

	MD.SetFileDescription("Fallout Data File");
	MD.Open("CRITTERS.DAT");

	if(MD.IsFileOpen())
	{
		CString s = "TRADERS/";
		s+=lua_tostring(luaVM, -1);
		LPSTR fn;
		fn = s.GetBuffer(s.GetLength());
		s.ReleaseBuffer();

		if(MD.ExtractFile_ToFile(fn,"data\\temp"))
		{
			CIni ini;
			ini.SetPathName(_T("data\\temp"));

			obj_Bar.koeff = ini.GetDouble("Main", "Skill", 1.75);

			int r = ini.GetInt("Main", "Goods", 1);

			char num[10];
			for(int i=1; i<=r; ++i)
			{
				s="Goods-";
				itoa(i, num, 10);
				s+=num;

				obj_Inv.AddItem(Rand(ini.GetInt(s, "MinAmount", 0), ini.GetInt(s, "MaxAmount", 1)), ini.GetString(s, "Name", ""), &Screen);
			}
		}

		DeleteFile("data\\temp");

		MD.Close();
	}


	plr_Inv.InitInventory();
	plr_Inv.DrawBInventory(&Screen);
	obj_Bar.InitInventory();
	obj_Bar.DrawBInventory(&Screen);
	plr_Bar.InitInventory();
	plr_Bar.koeff=1;
	plr_Bar.DrawBInventory(&Screen);

	lua_pushnumber( luaVM, 0 );
	return 1;
}

int s_StartSafeBreak(lua_State* luaVM)
{
	int i = lua_gettop(luaVM);
	safe_cr.l_success = lua_tonumber(luaVM, i);
	safe_cr.l_fail = lua_tonumber(luaVM, i-1);

	codecrack=true;
	safe_cr.InitSafe();
	
	lua_pushnumber( luaVM, 0 );
	return 1;
}

int s_ShowImage(lua_State* luaVM)
{
	CString name = "INGAME/IMG/";
	name += lua_tostring(luaVM, -1);

	LPSTR fn;
	fn = name.GetBuffer(name.GetLength());
	name.ReleaseBuffer();

	OpenDataFile();
	Temp.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile(fn), true); // загрузим изображение
	MD.Close();
	show_image = true;
	
	lua_pushnumber( luaVM, 0 );
	return 1;
}

int s_Debug(lua_State* luaVM)
{
	AfxMessageBox(lua_tostring(luaVM, -1));
	
	lua_pushnumber( luaVM, 0 );
	return 1;
}

int s_SetTimer(lua_State* luaVM)
{
	sc_timer = true;
	sc_time = lua_tonumber(luaVM, -2);
	sc_link = lua_tonumber(luaVM, -1);
	
	lua_pushnumber( luaVM, 0 );
	return 1;
}

int s_IsItemPresent(lua_State* luaVM)
{
	lua_pushnumber( luaVM, plr_Inv.Presence(lua_tostring(luaVM, -1)));
	return 1;
}

int s_GetPItem(lua_State* luaVM)
{

	plr_Inv.GetItem(lua_tonumber(luaVM, -2), lua_tostring(luaVM, -1), &Screen);
	lua_pushnumber( luaVM, 0 );
	return 1;
}

int s_Play3DSound(lua_State* luaVM)
{
	int i = lua_gettop(luaVM);

	float pos[3];

	pos[0] = lua_tonumber(luaVM, -3);
	pos[1] = lua_tonumber(luaVM, -2);
	pos[2] = lua_tonumber(luaVM, -1);

	Open3DSSound(lua_tostring(luaVM, -4), pos);

	lua_pushnumber( luaVM, 0 );
	return 1;
}

BOOL CheckCondition(CString condition)
{
	if(!condition.IsEmpty())
	{
		LPSTR sc = condition.GetBuffer(condition.GetLength());
		condition.ReleaseBuffer();

		SetGlobals();
		
		int oldtop = lua_gettop(luaVM);

		luaL_dostring(luaVM, sc);

		int res = lua_tonumber(luaVM, -1);

/*		CString s;
		char num[10];
		itoa(res, num, 10);
		s=num;
		AfxMessageBox(s);*/

		lua_settop(luaVM, oldtop);

		if(res==0)
			return FALSE;
		else
			return TRUE;
	}
	else
		return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//
//				Конец обработки скриптов
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
