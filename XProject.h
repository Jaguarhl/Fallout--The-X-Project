#include <NukeDX.h>
#include <NukeFile.h>
#include <locale.h>
#include "INI.h"
#include "sound.h"
//#include "quest.h"
#include "qdata.h"
#include "resource.h"
#include "Fight.h"
#include "Inventory.h"
#include "Barter.h"
#include "Safe.h"
#include "ARACrypt.h"
#include "Archiev.h"
//#include "Script.h"

extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

lua_State* luaVM;

CFight* pFight;

CInventory plr_Inv; // ивентарь игрока
CInventory obj_Inv; // инвентарь объекта
CBarter plr_Bar; // экран бартера игрока
CBarter obj_Bar; // экран бартера объектра
CSafe safe_cr; // взлом сейфа
CArchiev archiev; // архив

////////////////////////////////////////////////////////////////////////////////////////////
// Version Info																			  //
const LPSTR vinfo="Version 0.9a";														  //
////////////////////////////////////////////////////////////////////////////////////////////
// Copyright Info																		  //
const LPSTR cinfo="Copyright (c) 2008 Dark Unicorn Entertainment. All rights reserved.";  //
////////////////////////////////////////////////////////////////////////////////////////////
// Name of Game Data File																  //
const CString datnf="SCENARIO.DAT";														  //
////////////////////////////////////////////////////////////////////////////////////////////
// Name of Game Update List File														  //
const CString datlst="DATA.LST";														  //
////////////////////////////////////////////////////////////////////////////////////////////
// Name of Game Main Resource File														  //
const LPSTR gfxnf="MASTER.DAT";															  //
////////////////////////////////////////////////////////////////////////////////////////////
// Crupt key for savegames																  //
const LPCTSTR cryptkey="Pd~pLjXrF10";													  //
////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////
//Параметры интерфейса пользователя// 
/////////////////////////////////////

int maintext_x = 260;
int maintext_y = 75;
int maintext_width = 460;
int maintext_height = 350;
int nexttext_x = 180;
int nexttext_y = 505;
int nexttext_width = 420;
int nexttext_height = 80;
int image_x = 33; //30
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
//Конец параметров интерфейса////////
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

} Level[1000];


// Prototype for window procedure.
LRESULT APIENTRY WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam);

// Global instance handle.
HINSTANCE hInstance;
HWND hWindow;

NDX_Window Window;
NDX_Screen Screen;
NDX_Video Video;

NDX_Surface			Back;
NDX_Surface			Splash;
NDX_Surface			Cursor;
NDX_Surface			ICursor;
NDX_Surface			Buttons;
NDX_Surface			Main;
NDX_Surface			Font;
NDX_Surface			Puncts;
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
NDX_Surface			Noise;
NDX_Surface			Saveload; // экран загрузки / сохранения
NDX_Surface			TargetFrm; // интерфейс
NDX_Surface			TargetMsc; // все остальное, что относится к бою
NDX_Surface			TargetRdr; // все, что касаемо радара
NDX_Surface			Temp;
NDX_Surface			FotoImg;
NDX_Surface			Inventory; // экран инвентаря
NDX_Surface			Barter; // экран бартера
NDX_Surface			InvEquip; // экран быстрой экипировки
NDX_Surface			MoveM; // экран выбора количества вещей
NDX_Surface			Stats; // статы
NDX_Surface			Messages; // сообщения
NDX_Surface			Safe; // кодовый замок

struct Targets
{
	NDX_Surface			Target; // сама цель
} TImages[3];

CNukeFile MD;

NDX_AAFont AAFont;
NDX_AAFont AFont;
//HFONT Fon;

HFONT fnt1;
HFONT fnt2;

RECT Scr={80,60,720,540}; // для видео
RECT Scr2={0,0,800,600}; // для видео

CString battlelog; // сюда пишем лог драки

CString music; // тут держим текущий трек

int MouseX=400;
int MouseY=300;
int MouseXold, MouseYold=0; // нужны при перетаскивании предметов в инвентаре

// What's the current coordinates of the mouse.
int MouseXPos;
int MouseYPos;

bool MouseLB, MouseMB, MouseRB, MouseWUp, MouseWDown, MouseLBup;

bool MouseLBD, MouseLBDClick;

// Was the mouse clicked.
bool MouseLBClick, MouseMBClick, MouseRBClick;

BOOL showcursor=false; // показывать курсор или нет. актуально для оконного режима
int mode=0;
int last_mode=0;
int cursor_mode=0; // для инвентаря
int inv_mode=0; // режим, в котором сейчас работает инвентарь
BOOL show_image=false;
BOOL prog_exit=false;
BOOL prog_error=false;
BOOL fade=false;
BOOL fight=false;
BOOL save=false;
BOOL codecrack=false;
BOOL table_mark=false;
int drag=0;  // тащим ли мышь
int amount[5]; // количество ля передачи (инвентарь)
int anim=0;
BOOL changes=false;
int z=0;

UINT fr=0;

int error_id = 0;
LPSTR error_msg;

int hp_interval=0; // интервал времени, через который здоровье игрока восстанавливается на единицу

BYTE cur_target=0; // текущая цель

BOOL sc_timer=false; // запущен ли скриптовый таймер
UINT sc_time=0; 
UINT sc_link;

BOOL think=false; // является ли текст далее мыслями игрока
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// options
//
BOOL mute=false; // mute sound on / off
BYTE tbright=75; // яркость текста
BYTE svolume=100; // громкость
int lightning_bar=0; // интервал подсветки
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// чит-коды
BOOL cheats = false; // включен ли ввод
CString cheat; // здесь храним введенные символы

//System metrics
int ResX=0;
int ResY=0;
BOOL fullscreen=false; //false only for debug
BOOL wActive=true;
CRect wrect;

// Стр-ра событий интерфейса
struct IEvents
{
	BYTE Object;		//идентификатор объекта
	int Conditional;	//состояние объекта 

} IEvent[5];

// Стр-ра описания текущей игровой локации для интерфейса
struct Interfaces
{
	int rows; //кол-во строк в тексте
	int cur_y; //сдвиг вывода текста
	BOOL scroll; //надо ли выводить кнопки скролинга?

	int ar1; //кол-во строк для ответа 1
	int ar2; 
	int ar3;
	int ar4;
	int ar5;
	int arheight; //высота всех строк ответов
	int c_y; //сдвиг вывода ответов
	int iFrame; //текущий кадр при анимации Image'а
	bool iAni; //анимировать или нет
	BOOL scr; 

} Interface[1];

// Стр-ра оповещения
struct INotify
{
	BYTE message; // what picture to show?
	bool on; // on / off
	BYTE loop; // 
	int period;
} Notify;

int s_PlayerName = 0;

int sc_array[10][10]; // массив, будем использовать в скриптах



void InitGame();
void InitGameValues();

// save / load
void LoadGame(CString fname);
void SaveGame(CString fname);
void PrepareSaveload(int e=0);

// file routines
bool OpenDataFile();

// optionts routines
void OptionsCFG(bool f_save);


void Game();
void OnKeyDown(WPARAM wParam);
//void MouseMove(LPARAM lParam);
void OnMouseWheel(WPARAM wParam, LPARAM lParam);
bool HandleMessages();
void CalcCursor();
void MainMenu();
void ManageMessages(); //обработка сообщений игры
void GameCircle();

void LoadScenario();
void WriteText(int frame);
void WriteText();
void DrawString(int lsp, int space, int rn, int frame);
void DrawString(int lsp, int space, int rn);
void DrawTextString(CString string, int y, int font);

void PrepareFrame(int frame);
void PrepareFightFrm(); // подготовим интерфейс боя
void PrepareArchiveFrm(); // подготовим экран архива

void PlayIntro();
void PlayNGIntro(); // проиграем заставку для новой игры
void Save_load(); // выводим экран загрузки / сохранения
void ShowInventory(); // отображаем экран инвентаря
void ShowBarter(); // отображаем экран бартера
void DrawOptions(); // выводим иконки настроек
void DrawStats(); // выводим статы игрока
void ShowSafeCode(); // отображаем экран взлома кода

void WriteNextText(int frame);
void WriteFLogText();
void DrawNextString1(int lsp, int space, int rn, int frame);
void DrawNextString2(int lsp, int space, int rn, int frame);
void DrawNextString3(int lsp, int space, int rn, int frame);
void DrawNextString4(int lsp, int space, int rn, int frame);
void DrawNextString5(int lsp, int space, int rn, int frame);
void DrawLogString(int lsp, int space, int rn);

void ChangeDate(int frame);
void DisplayNumber(char* s, UINT x, UINT y);
void DisplayAccNumber(char* s, UINT x, UINT y);
void DisplayMMoveNumber(char* s, UINT x, UINT y);
void DisplayBNumber(char* s, UINT x, UINT y); // рисуем большие цифры в панели вскрытия сейфа
void DisplaySNumber(char* s, UINT x, UINT y); // рисуем маленькие цифры в панели вскрытия сейфа

void HitTarget(int p);
void EndTurn();
bool CheckFight(); // проверяем, кто победил
void DrawRadar(); // нарисуем обстановку в бою
void DrawHit(); // нарисуем удар по игроку

// использование вещей из инвентаря
void UseItem();
void UseMedical();
void UseFood();
void UseBook();
void UseHaloDisk();
void UseFlare();

void Titles();
void ChangeLocation(int img);
void ChangeMusic(CString msc);

void OnDestroy();
void OnActivateApp(WPARAM wParam);

void CheckScript(CString script); // выполнить скрипт

void OnTimer();
int Rand(int min, int max);

void InitScriptSystem();
void RealiseScriptSystem();
void SetGlobals();
BOOL CheckCondition(CString condition);


///////////////////////////////////////////////////////////
//
//*************				Функции, доступные из скрипта
//
///////////////////////////////////////////////////////////

int s_FadeToBlack(lua_State* luaVM);
int s_PlayerDeath(lua_State* luaVM);
int s_FlashMemory(lua_State* luaVM);
int s_ChangeLink(lua_State* luaVM);
int s_StartFight(lua_State* luaVM);
int s_RulesFight(lua_State* luaVM);
int s_LoadFight(lua_State* luaVM);
int s_AddQuest(lua_State* luaVM);
int s_AddContact(lua_State* luaVM);
int s_Play3DSound(lua_State* luaVM);
int s_Play3DSoundLoop(lua_State* luaVM);
int s_Stop3DSoundLoop(lua_State* luaVM);
int s_PlaySound(lua_State* luaVM);
int s_ShowImage(lua_State* luaVM);
int s_StartSafeBreak(lua_State* luaVM);
int s_SetMVolume(lua_State* luaVM);

int s_AddItem(lua_State* luaVM);
int s_NewInventory(lua_State* luaVM);
int s_OpenInventory(lua_State* luaVM);
int s_InitPInventory(lua_State* luaVM);
int s_IsItemPresent(lua_State* luaVM);
int s_GetPItem(lua_State* luaVM);
int s_AddPItem(lua_State* luaVM);
int s_OpenBarter(lua_State* luaVM);

int s_Debug(lua_State* luaVM);

int s_SetTimer(lua_State* luaVM);

int s_SetGlobals(lua_State* luaVM);
int s_GetGlobals(lua_State* luaVM);
int s_AssociateGlobals(lua_State* luaVM);
int s_FreeGlobals(lua_State* luaVM);

int s_NewTable(lua_State* luaVM); //очищаем таблицу
int s_GetElement(lua_State* luaVM); //получаем значение по индексам
int s_SetElement(lua_State* luaVM); //устанавливаем значение по индексам
int s_SetTViewMarker(lua_State* luaVM); //устанавливаем отображение маски для таблицы
int s_DrawMarkers(lua_State* luaVM);
int s_DrawMarker(lua_State* luaVM);

///////////////////////////////////////////////////////////
//*********************************************************
///////////////////////////////////////////////////////////