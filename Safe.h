/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//			Этот класс отвечает за подбор шифра к сейфу
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SAFE_H
#define __SAFE_H

class CSafe : public CObject
{
	public:

		NDX_Timer Timer; // таймер сейфа

		double timer; // timer
		int tryes_s; // кол-во попыток

		int l_success, l_fail; // ссылки на фреймы в лучае удачи или провала

		int c1;
		int c2;
		int c3;
		int c4;
		int c5;
		int c6;
		int c7;

		// можно ли вводить следующую цифру
		bool cn1;
		bool cn2;
		bool cn3;
		bool cn4;
		bool cn5;
		bool cn6;
		bool cn7;

		bool cracked; // взломан

		int tryes;
		int period;

		double ReserveTime; // сюда пишем время, на которое приложение теряет фокус (на момент потери фокуса)

	public:

		void InitSafe();

		int CheckTimer();
		void Restart();

		bool Code1();
		bool Code2();
		bool Code3();
		bool Code4();
		bool Code5();
		bool Code6();
		bool Code7();
		
};

#endif

