//////////////////////////////////////////////////////////////////////////
// XProject.CPP: Main source code for Blade Text Adventure Technology.	//
//////////////////////////////////////////////////////////////////////////

#include "XProject.h"

///////////////////////////////////////////////////////////
// WinMain
///////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst, LPSTR lpszCmdLine, int nCmdShow)
{
/*    WNDCLASS wndClass;
    HWND hWnd;
    MSG msg;

    // Define and register the window class.
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hCurrentInst;
    wndClass.hIcon = LoadIcon(hCurrentInst,
        MAKEINTRESOURCE("IDI_ICON1"));
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = "XProject";

    RegisterClass(&wndClass);

    hInstance = hCurrentInst;

    // Create a window of the previously defined class.
    hWnd = CreateWindow(
               "XProject",						// Window class's name.
               "Fallout: The X-Project",		// Title bar text.
               WS_POPUP,						// The window's style.
               0,								// X position.
               0,								// Y position.
               GetSystemMetrics(SM_CXSCREEN),   // Width.
               GetSystemMetrics(SM_CYSCREEN),   // Height.
               NULL,							// Parent window's handle.
               NULL,							// Menu handle.
               hCurrentInst,					// Instance handle.
               NULL);							// No additional data.

    // Display the window on the screen.
    ShowWindow(hWnd, nCmdShow);

	hWindow = hWnd;

    // Force the window to repaint itself.
    UpdateWindow(hWnd);*/

	Window.Create("Fallout: The X-Project",hCurrentInst,WndProc,WS_POPUP,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));

	ResX=GetSystemMetrics(SM_CXSCREEN);
	ResY=GetSystemMetrics(SM_CYSCREEN);
	hWindow = Window.hWnd;

	Screen.Create();
	Screen.SetFullScreen(NDXDF_SYSMEMBACK,Window.hWnd,800,600,16);
	
	ShowCursor(false);

	// если нет поддержки ММХ, то завершение работы
	if(NDX_TestMMX()) NDX_UseMMX = true;
		else PostMessage(hWindow, WM_CLOSE, 0, 0L);

	InitGame();

/*    // Start the message loop.
    while (HandleMessages())
    {
		Game();
    }

    return msg.wParam;*/

	while(Window.HandleMessages())
	{
		if(Window.Active)
		{
			// функция игрового цикла
			Game();
		}
	}
	Screen.ChangeRes(ResX, ResY);
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
            MouseY=HIWORD(lParam);
			MouseX=LOWORD(lParam);
            return 0;
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
			MouseLBClick=true;
			return 0;
		case WM_LBUTTONUP:
			MouseLB=false;
			return 0;
		case WM_TIMER:
			OnTimer();
			return 0;
    }

    // Make sure all unhandled messages get returned to Windows.
    return DefWindowProc(hWnd, message, wParam, lParam);

}

void InitGame()
{
	InitSound(hWindow);
	
	MD.SetFileDescription("Fallout Data File");
	MD.Open(gfxnf);


	Splash.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("MENU/sp5.bmp"),true);
	Splash.Draw(Screen.Back, 0, 0);
	Screen.Flip();

	Cursor.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("CURSOR/CURSOR_1.bmp"),true);
	Cursor.SetColorKey(Screen.PixelFormat->Rgb(0,0,0));
	Cursor.SetTransP(true);

	Buttons.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("MENU/b_but.bmp"),true);
	Buttons.SetColorKey(Screen.PixelFormat->Rgb(64,0,255));
	Buttons.SetTransP(true);

	Pip_mnu.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("INGAME/pip_but.bmp"),true);
	Pip_mnu.SetColorKey(Screen.PixelFormat->Rgb(0,0,255));
	Pip_mnu.SetTransP(true);

	Pip_srl.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("INGAME/scroll.bmp"),true);
	Pip_srl.SetColorKey(Screen.PixelFormat->Rgb(0,0,0));
	Pip_srl.SetTransP(true);

	Misc.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("INGAME/p_numbers1.bmp"),true);
	Misc.SetColorKey(Screen.PixelFormat->Rgb(0,0,0));
	Misc.SetTransP(true);

	Misc_but.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("MISC/yn.bmp"),true);
	Misc_but.SetColorKey(Screen.PixelFormat->Rgb(0,0,255));
	Misc_but.SetTransP(true);

	Main.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("INGAME/pipboy.bmp"),true);
	Image.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("INGAME/image_1.bmp"),true);
	Mnu_panel.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("MENU/panel.bmp"),true);
	Error.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("MISC/error.bmp"),true);

	Cpy_info.Create(&Screen, NDXST_SYSMEM, 800, 20);
	Cpy_info.Clear(Screen.PixelFormat->Rgb(0,0,0));
	Cpy_info.SetColorKey(Screen.PixelFormat->Rgb(0,0,0));
	Cpy_info.SetTransP(true);

//	HFONT fnt=NDX_MakeFont("Arial Cyr",4,8);
	HFONT fnt=CreateFont(14, 0, 0, 0, FW_NORMAL,
		FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, VARIABLE_PITCH, "Arial Cyr");

	Cpy_info.FormattedText(100,0,RGB(207, 191, 63),fnt, vinfo);
	Cpy_info.FormattedText(400,0,RGB(207, 191, 63),fnt, cinfo);

	NDX_DeleteFont(fnt);

	
	// I case you don't have the font on your computer, we just add it for a while.
//	AddFontResource("font1.ttf");

	// Initialize AAFont object.
//	AFont.Create("Arial Cyr",10,12);
//	AFont.Load("font1.btf");
	AAFont.Load(MD.GetFile("INGAME/font.bft"));


	// Remove the FontResource again.
//	RemoveFontResource("font1.ttf");

	OpenMStream("01.WAV");
	OpenClickStream();
	OpenNextSStream();
	PlayMStream();



	//Переменные

	MouseLB=false;
	MouseRB=false;
	MouseLBClick=false;
	MouseRBClick=false;

	SetTimer(hWindow, 1, 150, 0);

	srand((unsigned)time(0));

	luaVM = lua_open();
	
	if (NULL == luaVM)
		MessageBox(hWindow, "Failed to create Lua VM.", "Error", MB_OK);
	else
	{
		lua_register(luaVM, "PlayerDeath", s_PlayerDeath);
	}
}

void Game()
{
	CalcCursor();
	if(mode==0)
	{
		mode=1;		
	}

	else if(mode==1)
	{
		MainMenu();
	}

	else if(mode==2)
	{
		GameCircle();
	}
	
	Screen.ShowFPS(0, 580);

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
        PostMessage(hWindow, WM_CLOSE, 0, 0L);
    }
	else if(wParam == VK_F5)
	{
		ShowCursor(true);
		Cursor.Clear(Screen.PixelFormat->Rgb(0,0,0));

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
				if(MouseLB==true)
				{
					IEvent[0].Object=18;
					IEvent[0].Conditional=1;
				}
			}
			else if((MouseX>=nexttext_x)&&(MouseX<=nexttext_x+nexttext_width)&&(MouseY>=nexttext_y)&&(MouseY<=nexttext_y+nexttext_height))
			{
				if((MouseY>=nexttext_y)&&(MouseY<=nexttext_y+Interface[0].ar1*12-Interface[0].c_y))
				{
					if(Level[fr].url1_id!=fr)
					{
						IEvent[0].Object=5;
						if(MouseLB==true)
							IEvent[0].Conditional=2;
						else
							IEvent[0].Conditional=1;
					}
				}
				else if((MouseY>=nexttext_y+Interface[0].ar1*12+16-Interface[0].c_y)&&(MouseY<=nexttext_y+Interface[0].ar1*12+16+Interface[0].ar2*12-Interface[0].c_y))
				{
					if(Level[fr].url2_id!=fr)
					{
						IEvent[0].Object=6;
						if(MouseLB==true)
							IEvent[0].Conditional=2;
						else
							IEvent[0].Conditional=1;
					}
				}
				else if((MouseY>=nexttext_y+Interface[0].ar1*12+16+Interface[0].ar2*12+16-Interface[0].c_y)&&(MouseY<=nexttext_y+Interface[0].ar1*12+16+Interface[0].ar2*12+16+Interface[0].ar3*12-Interface[0].c_y))
				{
					if(Level[fr].url3_id!=fr)
					{
						IEvent[0].Object=7;
						if(MouseLB==true)
							IEvent[0].Conditional=2;
						else
							IEvent[0].Conditional=1;
					}
				}
				else if((MouseY>=nexttext_y+Interface[0].ar1*12+16+Interface[0].ar2*12+16+Interface[0].ar3*12+16-Interface[0].c_y)&&(MouseY<=nexttext_y+Interface[0].ar1*12+16+Interface[0].ar2*12+16+Interface[0].ar3*12+16+Interface[0].ar4*12-Interface[0].c_y))
				{
					if(Level[fr].url4_id!=fr)
					{
						IEvent[0].Object=8;
						if(MouseLB==true)
							IEvent[0].Conditional=2;
						else
							IEvent[0].Conditional=1;
					}
				}
				else if((MouseY>=nexttext_y+Interface[0].ar1*12+16+Interface[0].ar2*12+16+Interface[0].ar3*12+16+Interface[0].ar4*12+16-Interface[0].c_y)&&(MouseY<=nexttext_y+Interface[0].ar1*12+16+Interface[0].ar2*12+16+Interface[0].ar3*12+16+Interface[0].ar4*12+16+Interface[0].ar5*12-Interface[0].c_y))
				{
					if(Level[fr].url5_id!=fr)
					{
						IEvent[0].Object=9;
						if(MouseLB==true)
							IEvent[0].Conditional=2;
						else
							IEvent[0].Conditional=1;
					}
				}
			}
		}
	}

	MouseLB=false;
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
				IEvent[1].Object=1;
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
		else if(IEvent[0].Object==4)
		{
			if(IEvent[0].Conditional==1)
			{
				Click();
				Buttons.Draw(Screen.Back,510,365,NDX_RECT(240,195,480,235));
				IEvent[1].Object=3;
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
					Misc_but.Draw(Screen.Back,246,360,NDX_RECT(125,0,250,24));
					IEvent[1].Object=10;
					IEvent[1].Conditional=1;
				}
			}
			else if(IEvent[0].Object==11)
			{
				if(IEvent[0].Conditional==1)
				{
					Misc_but.Draw(Screen.Back,421,360,NDX_RECT(125,24,250,48));
					IEvent[1].Object=11;
					IEvent[1].Conditional=1;
				}
			}
		}
	}



//	Screen.Back->Text(cinfo, 290, 460, Screen.PixelFormat->Rgb(207, 191, 63), Font);

	// курсор мышки
	Cursor.Draw(Screen.Back,MouseX,MouseY);	
}

void ManageMessages()
{
	if(mode==1)
	{
		if(IEvent[1].Object==1)
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
				Sleep(200);
				StopMStream();
				OpenMStream("15shady.mp3");
				LoadScenario();
				PrepareFrame(0);
				PlayMStream();
				mode=2;
			}
		}
		else if(IEvent[1].Object==3)
		{
			if(IEvent[1].Conditional==1)
			{
				Sleep(200);
				Titles();
			}
		}
		else if(IEvent[1].Object==10)
		{
			if(IEvent[1].Conditional==1)
			{
				Sleep(200);
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
		if(IEvent[1].Object==1)
		{
			if(IEvent[1].Conditional==1)
			{
				if(Interface[0].cur_y>=8)
					Interface[0].cur_y=Interface[0].cur_y-8;
				Sleep(25);
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
		}
		else if(IEvent[1].Object==3)
		{
			if(IEvent[1].Conditional==1)
			{
				if(Interface[0].c_y>=2)
					Interface[0].c_y=Interface[0].c_y-2;
				Sleep(25);
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
		}
		else if(IEvent[1].Object==5)
		{
			if(IEvent[1].Conditional==1)
			{
				CheckScript(Level[fr].script1);
				PrepareFrame(Level[fr].url1_id);
				Sleep(200);
			}
		}
		else if(IEvent[1].Object==6)
		{
			if(IEvent[1].Conditional==1)
			{
				CheckScript(Level[fr].script2);
				PrepareFrame(Level[fr].url2_id);
				Sleep(200);
			}
		}
		else if(IEvent[1].Object==7)
		{
			if(IEvent[1].Conditional==1)
			{
				CheckScript(Level[fr].script3);
				PrepareFrame(Level[fr].url3_id);
				Sleep(200);
			}
		}
		else if(IEvent[1].Object==8)
		{
			if(IEvent[1].Conditional==1)
			{
				CheckScript(Level[fr].script4);
				PrepareFrame(Level[fr].url4_id);
				Sleep(200);
			}
		}
		else if(IEvent[1].Object==9)
		{
			if(IEvent[1].Conditional==1)
			{
				CheckScript(Level[fr].script5);
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
		else if(IEvent[1].Object==10)
		{
			if(IEvent[1].Conditional==1)
			{
				Sleep(200);
				prog_exit=false;
				StopMStream();
				OpenMStream("01.WAV");
				PlayMStream();
				mode=1;
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

}

void GameCircle()
{
	Main.Draw(Screen.Back,0,0,NDX_RECT(0,0,800,600));
	Main_txt.Draw(Screen.Back, maintext_x, maintext_y, NDX_RECT(0, 0+Interface[0].cur_y, maintext_width, maintext_height+Interface[0].cur_y));
	Next_txt.Draw(Screen.Back, nexttext_x, nexttext_y, NDX_RECT(0, 0+Interface[0].c_y, nexttext_width, nexttext_height+Interface[0].c_y));
	Image.Draw(Screen.Back, image_x, image_y, NDX_RECT(0, Interface[0].iFrame*100, 140, Interface[0].iFrame*100+100));
	Pip_mnu.Draw(Screen.Back, pipmnu_x, pipmnu_y, NDX_RECT(0, 0, 125, 126));
	if(Interface[0].scroll==TRUE)
		Pip_srl.Draw(Screen.Back, pipsrl_x, pipsrl_y, NDX_RECT(0, 0, 11, 50));
	else
		Pip_srl.Draw(Screen.Back, pipsrl_x, pipsrl_y, NDX_RECT(22, 0, 33, 50));

	if(Interface[0].scr==TRUE)
		Pip_srl.Draw(Screen.Back, nxtsrl_x, nxtsrl_y, NDX_RECT(0, 0, 11, 50));
	else
		Pip_srl.Draw(Screen.Back, nxtsrl_x, nxtsrl_y, NDX_RECT(22, 0, 33, 50));
	Pip_date.Draw(Screen.Back, 0, 0);
	Location_txt.Draw(Screen.Back, location_x, location_y);
	
	//обработка событий
	if(IEvent[0].Conditional!=0)
	{
		Next_txt_ins.Clear(Screen.PixelFormat->Rgb(0,0,0));

		if((IEvent[0].Object==1)&&(Interface[0].scroll==TRUE))
		{
			if(IEvent[0].Conditional==1)
			{
				Pip_srl.Draw(Screen.Back, pipsrl_x, pipsrl_y, NDX_RECT(11, 0, 22, 10));
				IEvent[1].Object=1;
				IEvent[1].Conditional=1;
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
		}
		else if((IEvent[0].Object==3)&&(Interface[0].scr==TRUE))
		{
			if(IEvent[0].Conditional==1)
			{
				Pip_srl.Draw(Screen.Back, nxtsrl_x, nxtsrl_y, NDX_RECT(11, 0, 22, 10));
				IEvent[1].Object=3;
				IEvent[1].Conditional=1;
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
		}
		else if(IEvent[0].Object==5)
		{
			if(IEvent[0].Conditional==1)
			{
				Next_txt.DrawColorMix(&Next_txt_ins, 0, 0, NDX_RECT(0, 0, nexttext_width, Interface[0].ar1*12+8), Screen.PixelFormat->Rgb(0, 0, 255), 200);
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
				Next_txt.DrawColorMix(&Next_txt_ins, 0, 16+Interface[0].ar1*12, NDX_RECT(0, 16+Interface[0].ar1*12, nexttext_width, 16+Interface[0].ar1*12+Interface[0].ar2*12+8), Screen.PixelFormat->Rgb(0, 0, 255), 200);
			}
			else if(IEvent[0].Conditional==2)
			{
				Next_txt.DrawFadeToBlack(&Next_txt_ins, 0, 16+Interface[0].ar1*12, NDX_RECT(0, 16+Interface[0].ar1*12, nexttext_width, 16+Interface[0].ar1*12+Interface[0].ar2*12+8), 100);
				IEvent[1].Object=6;
				IEvent[1].Conditional=1;
				
			}
		}
		else if(IEvent[0].Object==7)
		{
			if(IEvent[0].Conditional==1)
			{
				Next_txt.DrawColorMix(&Next_txt_ins, 0, 16+Interface[0].ar1*12+16+Interface[0].ar2*12, NDX_RECT(0, 16+Interface[0].ar1*12+16+Interface[0].ar2*12, nexttext_width, 16+Interface[0].ar1*12+Interface[0].ar2*12+16+Interface[0].ar3*12+8), Screen.PixelFormat->Rgb(0, 0, 255), 200);
			}
			else if(IEvent[0].Conditional==2)
			{
				Next_txt.DrawFadeToBlack(&Next_txt_ins, 0, 16+Interface[0].ar1*12+16+Interface[0].ar2*12, NDX_RECT(0, 16+Interface[0].ar1*12+16+Interface[0].ar2*12, nexttext_width, 16+Interface[0].ar1*12+Interface[0].ar2*12+16+Interface[0].ar3*12+8), 100);
				IEvent[1].Object=7;
				IEvent[1].Conditional=1;
			}
		}
		else if(IEvent[0].Object==8)
		{
			if(IEvent[0].Conditional==1)
			{
				Next_txt.DrawColorMix(&Next_txt_ins, 0, 16+Interface[0].ar1*12+16+Interface[0].ar2*12+16+Interface[0].ar3*12, NDX_RECT(0, 16+Interface[0].ar1*12+16+Interface[0].ar2*12+16+Interface[0].ar3*12, nexttext_width, 16+Interface[0].ar1*12+Interface[0].ar2*12+16+Interface[0].ar3*12+16+Interface[0].ar4*12+8), Screen.PixelFormat->Rgb(0, 0, 255), 200);
			}
			else if(IEvent[0].Conditional==2)
			{
				Next_txt.DrawFadeToBlack(&Next_txt_ins, 0, 16+Interface[0].ar1*12+16+Interface[0].ar2*12+16+Interface[0].ar3*12, NDX_RECT(0, 16+Interface[0].ar1*12+16+Interface[0].ar2*12+16+Interface[0].ar3*12, nexttext_width, 16+Interface[0].ar1*12+Interface[0].ar2*12+16+Interface[0].ar3*12+16+Interface[0].ar4*12+8), 100);
				IEvent[1].Object=8;
				IEvent[1].Conditional=1;
			}
		}
		else if(IEvent[0].Object==9)
		{
			if(IEvent[0].Conditional==1)
			{
				Next_txt.DrawColorMix(&Next_txt_ins, 0, 16+Interface[0].ar1*12+16+Interface[0].ar2*12+16+Interface[0].ar3*12+16+Interface[0].ar4*12, NDX_RECT(0, 16+Interface[0].ar1*12+16+Interface[0].ar2*12+16+Interface[0].ar3*12+16+Interface[0].ar4*12, nexttext_width, 16+Interface[0].ar1*12+Interface[0].ar2*12+16+Interface[0].ar3*12+16+Interface[0].ar4*12+16+Interface[0].ar5*12+8), Screen.PixelFormat->Rgb(0, 0, 255), 200);
			}
			else if(IEvent[0].Conditional==2)
			{
				Next_txt.DrawFadeToBlack(&Next_txt_ins, 0, 16+Interface[0].ar1*12+16+Interface[0].ar2*12+16+Interface[0].ar3*12+16+Interface[0].ar4*12, NDX_RECT(0, 16+Interface[0].ar1*12+16+Interface[0].ar2*12+16+Interface[0].ar3*12+16+Interface[0].ar4*12, nexttext_width, 16+Interface[0].ar1*12+Interface[0].ar2*12+16+Interface[0].ar3*12+16+Interface[0].ar4*12+16+Interface[0].ar5*12+8), 100);
				IEvent[1].Object=9;
				IEvent[1].Conditional=1;
			}
		}
		else if(IEvent[0].Object==18)
		{
			if(IEvent[0].Conditional==1)
			{
				Pip_mnu.Draw(Screen.Back, pipmnu_x, pipmnu_y+102, NDX_RECT(125, 102, 250, 126));
				IEvent[1].Object=18;
				IEvent[1].Conditional=1;
			}
		}

		Next_txt_ins.Draw(Screen.Back, nexttext_x, nexttext_y, NDX_RECT(0, 0+Interface[0].c_y, nexttext_width, nexttext_height+Interface[0].c_y));
	}

	if(prog_exit)
	{
		Error.Draw(Screen.Back, 201, 200);
		Misc_but.Draw(Screen.Back, 246, 360, NDX_RECT(0, 0, 125, 24));
		Misc_but.Draw(Screen.Back, 421, 360, NDX_RECT(0, 24, 125, 48));
		Screen.Back->FormattedText(321,260,RGB(207, 191, 63),NULL,"Подтвердите операцию...");

		if(IEvent[0].Conditional!=0)
		{
			if(IEvent[0].Object==10)
			{
				if(IEvent[0].Conditional==1)
				{
					Misc_but.Draw(Screen.Back,246,360,NDX_RECT(125,0,250,24));
					IEvent[1].Object=10;
					IEvent[1].Conditional=1;
				}
			}
			else if(IEvent[0].Object==11)
			{
				if(IEvent[0].Conditional==1)
				{
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
			Sleep(10);
		}
	
		fade=false;
	}

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
			for (UINT i=0; i<100; ++i)
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
	int rn=0;
	int space=0;
	int lsp = 0;

//	AAFont.SetClip(NDX_RECT(maintext_x,maintext_y,maintext_x+maintext_width,maintext_y+maintext_height));

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

void DrawString(int lsp, int space, int rn, int frame)
{
	CString str;
	LPSTR dstr;
	for(int i=lsp; i<space; ++i)
	{
		str+=Level[frame].text_data.GetAt(i);
	}

	int l=str.GetLength();

	dstr = str.GetBuffer(l);
	str.ReleaseBuffer();

	AAFont.Text(&Main_txt, 0, rn*16, Screen.PixelFormat->Rgb(207, 191, 63), dstr);
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
	Interface[0].cur_y=0; 


	int i = Level[frame].text_data.GetLength();
	int rows=0;
	int sn=0;
	int space=0;
	int lsp=0;

//вычисляем размер текста	
	for(int z=0; z<i; ++z)
	{
		if(Level[frame].text_data.GetAt(z)==' ')
		{
//			lsp=space;
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

	int height=16;

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

	//делаем предварительные расчеты для вариантов ответа
	i = Level[frame].url1.GetLength();

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
	if(i>1)
		Interface[0].ar1=rows+1;

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
	if(i>1)
		Interface[0].ar2=rows+1;

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
	if(i>1)
		Interface[0].ar3=rows+1;

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
	if(i>1)
		Interface[0].ar4=rows+1;

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
	if(i>1)
		Interface[0].ar5=rows+1;

	height=0;
	int allrows = Interface[0].ar1+Interface[0].ar2+Interface[0].ar3+Interface[0].ar4+Interface[0].ar5;
//	allrows+=32;

	if(allrows*12+(allrows-1)*16<nexttext_height)
	{
		Interface[0].scr=FALSE;
		height+=nexttext_height;
	}
	else 
	{
		Interface[0].scr=TRUE;
		height+=allrows*12+(allrows)*16;
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

	WriteNextText(frame);

	ChangeDate(frame);

	Location_txt.Create(&Screen, NDXST_SYSMEM, 300, 20);
	Location_txt.Clear(Screen.PixelFormat->Rgb(0,0,0));
	Location_txt.SetColorKey(Screen.PixelFormat->Rgb(0,0,0));
	Location_txt.SetTransP(true);

	
	LPSTR dstr;
	int l=Level[frame].location.GetLength();
	dstr = Level[frame].location.GetBuffer(l);
	Level[frame].location.ReleaseBuffer();
	Location_txt.FormattedText(0,0,RGB(181, 149, 89),NULL,dstr);

	ChangeLocation(Level[frame].image);

}

void WriteNextText(int frame)
{
	int i = Level[frame].url1.GetLength();
	int sn=0;
	int rn=0;
	int space=0;
	int lsp = 0;

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
//	AFont.Text(&Next_txt, 0, rn*12, Screen.PixelFormat->Rgb(207, 191, 63), dstr);
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

	Next_txt.FormattedText(0,Interface[0].ar1*12+16+rn*12,RGB(207, 191, 63),NULL,dstr);
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

	Next_txt.FormattedText(0,Interface[0].ar1*12+16+Interface[0].ar2*12+16+rn*12,RGB(207, 191, 63),NULL,dstr);
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

	Next_txt.FormattedText(0,Interface[0].ar1*12+16+Interface[0].ar2*12+16+Interface[0].ar3*12+16+rn*12,RGB(207, 191, 63),NULL,dstr);
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

	Next_txt.FormattedText(0,Interface[0].ar1*12+16+Interface[0].ar2*12+16+Interface[0].ar3*12+16+Interface[0].ar4*12+16+rn*12,RGB(207, 191, 63),NULL,dstr);
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

void Titles()
{
	//вывод титров
	NDX_Surface			Fon; //фон
	NDX_Surface			Sprites;

	Fon.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("MENU/titles.bmp"),true);

/*	Sprites.LoadBMP(&Screen,NDXST_SYSMEM,"t_sprites.bmp",true);
	Sprites.SetColorKey(Screen.PixelFormat->Rgb(255,255,255));
	Sprites.SetTransP(true);*/

	for(int i=0; i<255; ++i)
	{
		Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i);
		Screen.Flip();
//		Sleep(10);
	}
}

void ChangeLocation(int img)
{
	anim=false;
	Interface[0].iAni=false;
	CString st("INGAME/image_");
	char s[10];
    itoa(img, s, 10);
	st+=s;
	st+=".bmp";

	LPSTR fn;
//	int l = st.GetLength();
	fn = st.GetBuffer(st.GetLength());
	st.ReleaseBuffer();
	Image.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile(fn),true);
	
	if(Image.Height>100)
	{
		anim=true;
		Interface[0].iAni=true;
	}
}

///////////////////////////////////////////////////////////
// OnDestroy()
//
// This function cleans up after the program when the
// application receives a WM_DESTROY message from Windows.
///////////////////////////////////////////////////////////
void OnDestroy()
{
	DeleteSound();
	MD.Close();
	AAFont.Close();
	AFont.Close();
	lua_close(luaVM);
//	Back.Clear(Screen.PixelFormat->Rgb(0,0,0));
/*	Back.Release();
	Splash.Release();
	Cursor.Release();
	Buttons.Release();
	Main.Release();
	Main_txt.Release();
	Image.Release();
	Pip_mnu.Release();
	Pip_srl.Release();
	Next_txt.Release();
	Location_txt.Release();
	Pip_date.Release();
	Misc.Release();
	Mnu_panel.Release();
	Error.Release();
	Misc_but.Release();
	Cpy_info.Release();*/
//  Screen.Close();
}

void OnTimer()
{
	if(mode==2)
	{
		if((anim)&&(Interface[0].iAni))
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
		else if((anim)&&(!Interface[0].iAni))
		{
			UINT an;
			an = rand() % 8;

			if(an == 3)
				Interface[0].iAni=true;
		}
	}
}

void s_FadeToBlack(lua_State* luaVM)
{
	NDX_Surface			Fon; //фон

	Fon.Create(&Screen,NDXST_SYSMEM,800, 600);

	Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
	
	for(int i=17; i>0; --i)
	{
		Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(0,0,0), i*15);
		Screen.Flip();
		Sleep(10);
	}

	fade=true;
}

int s_PlayerDeath(lua_State* luaVM)
{
	NDX_Surface			Fon; //фон

	Fon.Create(&Screen,NDXST_SYSMEM,800, 600);

	Screen.Back->Draw(&Fon,0,0,NDX_RECT(0,0,800,600));
	
	for(int i=17; i>0; --i)
	{
		Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(255,255,255), i*15);
		Screen.Flip();
		Sleep(10);
	}

	Fon.LoadBMP(&Screen,NDXST_SYSMEM,MD.GetFile("INGAME/death.bmp"),true);

	for(i=0; i<17; ++i)
	{
		Fon.DrawColorMix(Screen.Back, 0, 0, NDX_RECT(0, 0, 800, 600), Screen.PixelFormat->Rgb(255,255,255), i*15);
		Screen.Flip();
		Sleep(10);
	}

	Sleep(1000);
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//
//				Обработка скриптов
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

void CheckScript(CString script)
{
	if(!script.IsEmpty())
	{
		if(script.GetAt(1)=='G')
			CheckGraphicsS(script);
		else if(script.GetAt(1)=='P')
			CheckPlayerS(script);
	}

}

void CheckGraphicsS(CString script)
{
//	if(script=="G_FTB")
//		FadeToBlack();
}

void CheckPlayerS(CString script)
{
//	if(script=="P_PDIE")
//		PlayerDeath();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//
//				Конец обработки скриптов
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////