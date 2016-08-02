/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//			Здесь будем работать с инвентарем
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __INVENTORY_H
#define __INVENTORY_H

#define INV_WEAPON		1
#define INV_MISC		2
#define INV_MEDICAL		3
#define INV_AMMO		4
#define INV_FOOD		5
#define INV_TOOL		6
#define INV_BOOK		7
#define INV_MONEY		8
#define INV_HALO		9
#define INV_FLARE		10

const int y_size = 64; // размер каждой вещи в графическом эквиваленте
const int i_height = 384; // высота области прокрутки в инвентаре
const int i_weight = 130; // ширина области прокрутки в инвентаре
const int i_bheight = 320; // высота области прокрутки в бартере
const int i_bweight = 121; // ширина области прокрутки в бартере

class CInventory : public CObject
{
	public:

		int sc_y;			// на сколько элементов проскролено

		int cur_itm;		// что сейчас выделено / перетаскивается

		bool scrollable;	// прокрутка активна (для бартера)?
		bool scrollable_i;	// прокрутка активна (для инвентаря)?

		double koeff;		// коэффициент наценки у торговцев

		CString	s_name;		// собственно, чей инвентарь
		CString	s_image;	// какую картинку загрузить для источника

		NDX_Surface inv_list;		// список изображений для отображения в инвентаре
		NDX_Surface inv_blist;		// список изображений для отображения в экране бартера
		NDX_Surface inv_source;		// изображение открываемого источника
		NDX_Surface inv_desc;		// описание предмета будем выводить сюда

		CNukeFile CD;

		struct CItem
		{
			DWORD type;			// тип вещи
			DWORD stype;
			int amount;			// количество
//			CString image;		// изображение
			NDX_Surface img;	
			CString name;		// название
			CString i_desc;		// имя файла описания объекта
		};

		CPtrArray	m_Inventory;


	public:

		void InitInventory();

		///////////////////////////////////////////////////////////////////////////////////////////
		// Манипуляции с вещами в инвентаре
		///////////////////////////////////////////////////////////////////////////////////////////

		// добавить вещь в инвентарь
		void AddItem(int amount, CString desc, NDX_Screen * ParentScreen);
		// изменить
		void ModifyItem(int amount, CString desc, NDX_Screen * ParentScreen);
		// взять
		void GetItem(int amount, CString desc, NDX_Screen * ParentScreen);
		// заменить на другую
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

		bool Presence(CString desc);

		void ClearAll(CString s, CString r, NDX_Screen * ParentScreen);

		void UseMiscItem(NDX_Screen * ParentScreen);


		///////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////
		// Графическая часть
		///////////////////////////////////////////////////////////////////////////////////////////

		// подготовим поверхность с изображениями содержимого инвентаря
		void DrawInventory(NDX_Screen * ParentScreen);

		// подготовим поверхность с изображениями содержимого инвентаря
		void DrawBInventory(NDX_Screen * ParentScreen);

		// отобразим объект при переносе многих вещей
		void DrawItem(NDX_Surface* dest, int x, int y);

		// отобразим описание объекта
		void DrawDescription(NDX_Screen* ParentScreen);

		// отобразим описание объекта для бартера
		void DrawBDescription(NDX_Screen* ParentScreen);


		///////////////////////////////////////////////////////////////////////////////////////////
		// Работа с мышью
		///////////////////////////////////////////////////////////////////////////////////////////

		// вернем индекс объекта по координатам мыши
		int GetObjIndex(int x, int y);

		// отрисовать объект по координатам мыши (перенос объекта)
		void DrawMoveObj(NDX_Surface* dest, int x, int y);


	protected:

		int FindItem(CString name);
		int FindPareItem(DWORD st);
		CString GetIDesc(int i);

		void WriteDescText(CString str);
		void DrawDescString(CString desc_s, int lsp, int space, int rn, HFONT fnt);

		void WriteBDescText(CString str);
		void DrawBDescString(CString desc_s, int lsp, int space, int rn, HFONT fnt);
		
};

#endif

