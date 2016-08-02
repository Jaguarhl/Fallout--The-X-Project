/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//			Здесь будем работать с подсказками
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __HELPER_H
#define __HELPER_H

class CHelper : public CObject
{
	public:

		NDX_Screen*			ParentScreen;
		CNukeFile			MD; // master.dat
		CNukeFile			CD; // critters.dat
		NDX_Surface			HelperM; // её мы и будем отображать
		
		HFONT				fnt; // шрифт для стандартных надписей
		
		// изначальные размеры поверхности
		int maintext_width;
		int maintext_height;
		
		

	public:
		void InitHelper(NDX_Screen * PScreen);
		void PrepareFrame(/*NDX_Screen * ParentScreen*/);

		void Mouse(int mX, int mY, bool lbut, NDX_Screen * ParentScreen);

	protected:
		void PrepareMenu(/*NDX_Screen * ParentScreen,*/ int hover=0);
		void PrepareHelperFrm(/*NDX_Screen * ParentScreen*/);
		void PrepareNotes(/*NDX_Screen * ParentScreen*/);
		void PrepareContacts(/*NDX_Screen * ParentScreen*/);
		void PrepareHoloDisks();
		void PrepareHoloDiskText();
		void PrepareHDView(int i);
		////////////////////////////////////////////

};

#endif