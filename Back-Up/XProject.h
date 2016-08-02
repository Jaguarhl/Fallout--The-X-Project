#include <NukeDX.h>
#include <NukeFile.h>
#include "sound.h"
//#include "Script.h"

extern "C"
{
	#include <lua.h>
}

lua_State* luaVM;

////////////////////////////////////////////////////////////////////////////////////////////
// Version Info																			  //
const LPSTR vinfo="Version 0.6";														  //
////////////////////////////////////////////////////////////////////////////////////////////
// Copyright Info																		  //
const LPSTR cinfo="Copyright (c) 2001-2003 Dark Unicorn Entertainment. All rights reserved.";			  //
////////////////////////////////////////////////////////////////////////////////////////////
// Name of Game Data File																  //
const CString datnf="test.dat";															  //
////////////////////////////////////////////////////////////////////////////////////////////
// Name of Game Update List File														  //
const CString datlst="DATA.LST";														  //
////////////////////////////////////////////////////////////////////////////////////////////
// Name of Game Main Resource File														  //
const LPSTR gfxnf="MASTER.DAT";															  //
////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////
//��������� ���������� ������������// 
/////////////////////////////////////

int maintext_x = 260;
int maintext_y = 75;
int maintext_width = 460;
int maintext_height = 350;
int nexttext_x = 180;
int nexttext_y = 505;
int nexttext_width = 420;
int nexttext_height = 80;
int image_x = 30;
int image_y = 63;
int pipmnu_x = 25;
int pipmnu_y = 330;
int pipsrl_x = 730;
int pipsrl_y = 100;
int nxtsrl_x = 615;
int nxtsrl_y = 510;
int mnupanel_x = 470;
int mnupanel_y = 145;
int mnubt_x = 510;
int mnubt_y = 170;
int location_x = 250;
int location_y = 45;

/////////////////////////////////////
//����� ���������� ����������////////
/////////////////////////////////////

//matrix of game data
struct Levels
{
	UINT image; //ID of image
	UINT year; //year
	UINT month; //month
	UINT day; //day
	CString location; //name of location
	CString sublocation; //subname of location
	UINT sublink; //link for sublocation info
	CString text_data; //text
	CString url1; //url 1 text
	UINT url1_id; //link 1
	CString condition1; //condition for this link
	CString script1; //script for this link
	CString url2;
	UINT url2_id;
	CString condition2; //condition for this link
	CString script2; //script for this link
	CString url3;
	UINT url3_id;
	CString condition3; //condition for this link
	CString script3; //script for this link
	CString url4;
	UINT url4_id;
	CString condition4; //condition for this link
	CString script4; //script for this link
	CString url5;
	UINT url5_id;
	CString condition5; //condition for this link
	CString script5; //script for this link
	CString mp3fn; //path to music file
	CString event; //event for this location

} Level[100];

// Prototype for window procedure.
LRESULT APIENTRY WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam);

// Global instance handle.
HINSTANCE hInstance;
HWND hWindow;

NDX_Window Window;
NDX_Screen Screen;

NDX_Surface			Back;
NDX_Surface			Splash;
NDX_Surface			Cursor;
NDX_Surface			Buttons;
NDX_Surface			Main;
NDX_Surface			Main_txt;
NDX_Surface			Image;
NDX_Surface			Pip_mnu;
NDX_Surface			Pip_srl;
NDX_Surface			Next_txt;
NDX_Surface			Next_txt_ins;
NDX_Surface			Location_txt;
NDX_Surface			Pip_date;
NDX_Surface			Misc;
NDX_Surface			Mnu_panel;
NDX_Surface			Error;
NDX_Surface			Misc_but;
NDX_Surface			Cpy_info;

CNukeFile MD;



NDX_AAFont AAFont;
NDX_AAFont AFont;
HFONT Fon;

int MouseX=400;
int MouseY=300;

// What's the current coordinates of the mouse.
int MouseXPos;
int MouseYPos;

bool MouseLB, MouseMB, MouseRB;

// Was the mouse clicked.
bool MouseLBClick, MouseMBClick, MouseRBClick;

int mode=0;
BOOL prog_exit=false;
BOOL fade=false;
BOOL anim=false;
int z=0;

UINT fr=0;


//System metrics
int ResX=0;
int ResY=0;

// ���-�� ������� ����������
struct IEvents
{
	BYTE Object;		//������������� �������
	int Conditional;	//��������� ������� 

} IEvent[5];

// ���-�� �������� ������� ������� ������� ��� ����������
struct Interfaces
{
	int rows; //���-�� ����� � ������
	int cur_y; //����� ������ ������
	BOOL scroll; //���� �� �������� ������ ���������?

	int ar1; //���-�� ����� ��� ������ 1
	int ar2; 
	int ar3;
	int ar4;
	int ar5;
	int arheight; //������ ���� ����� �������
	int c_y; //����� ������ �������
	int iFrame; //������� ���� ��� �������� Image'�
	int iAni; //����������� ��� ���
	BOOL scr; 

} Interface[1];




void InitGame();
void Game();
void OnKeyDown(WPARAM wParam);
//void MouseMove(LPARAM lParam);
bool HandleMessages();
void CalcCursor();
void MainMenu();
void ManageMessages(); //��������� ��������� ����
void GameCircle();
void LoadScenario();
void WriteText(int frame);
void DrawString(int lsp, int space, int rn, int frame);
void PrepareFrame(int frame);
void WriteNextText(int frame);
void DrawNextString1(int lsp, int space, int rn, int frame);
void DrawNextString2(int lsp, int space, int rn, int frame);
void DrawNextString3(int lsp, int space, int rn, int frame);
void DrawNextString4(int lsp, int space, int rn, int frame);
void DrawNextString5(int lsp, int space, int rn, int frame);
void ChangeDate(int frame);
void DisplayNumber(char* s, UINT x, UINT y);
void Titles();
void ChangeLocation(int img);
void OnDestroy();
void CheckScript(CString script);
void CheckPlayerS(CString script);
void CheckGraphicsS(CString script);
void OnTimer();

///////////////////////////////////////////////////////////
//
//*************				�������, ��������� �� �������
//
///////////////////////////////////////////////////////////

void s_FadeToBlack(lua_State* luaVM);
int s_PlayerDeath(lua_State* luaVM);

///////////////////////////////////////////////////////////
//*********************************************************
///////////////////////////////////////////////////////////