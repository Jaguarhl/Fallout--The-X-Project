/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//			���� ����� �������� �� ������ ����� � �����
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SAFE_H
#define __SAFE_H

class CSafe : public CObject
{
	public:

		NDX_Timer Timer; // ������ �����

		double timer; // timer
		int tryes_s; // ���-�� �������

		int l_success, l_fail; // ������ �� ������ � ����� ����� ��� �������

		int c1;
		int c2;
		int c3;
		int c4;
		int c5;
		int c6;
		int c7;

		// ����� �� ������� ��������� �����
		bool cn1;
		bool cn2;
		bool cn3;
		bool cn4;
		bool cn5;
		bool cn6;
		bool cn7;

		bool cracked; // �������

		int tryes;
		int period;

		double ReserveTime; // ���� ����� �����, �� ������� ���������� ������ ����� (�� ������ ������ ������)

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

