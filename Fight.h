/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// All player stats and manipulations here
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __FIGHT_H
#define __FIGHT_H

class CFight : public CObject
{
	public:
		int f_mode; // ����� ��� (���� / �������)

		bool immortal; // ������� �� ��� ����������?

		int missp, misst; // ����������� ������
		bool move; // ��������� �� ����

//		CString target_NAME;
//		CString target_type; // ��� ���� ����

		BYTE targets; // ���-�� �����
		
		int p_hpl, t_hpl; // �������� ����� ��� ������
		int l_pw, l_tw; // ������ �� ������ � ������ ������

		int player_SKILL;
		int player_maxHP; // ���� ���� ������ �� � ��� ���� �� �����
		int player_HP;
		int player_mHP;
		int player_AC;
		int player_AP;
		int player_cAP;
		int player_DMGfrom;
		int player_SDMGfrom; // ����� ������ ����������� ������ ��� ����
		int player_DMGto;
		int player_SDMGto;
		int player_DMGplus;
		int player_SDMGplus;
		int player_APPenalty;
		int player_requed_distance;
		
		struct PWeapon
		{
			NDX_Surface wpn_image; // �������� ������
			CString player_weapon; // ��� �� ������ � ��� � �����?
			int weapon_type; // ��� ������
			int weapon_DMGfrom;
			int weapon_DMGto;
			int weapon_DMGplus;
			int weapon_ammo; // ��� ��������, ���� ���������
			int weapon_magazine; // ����� ��������, ���� ���������
			int	weapon_cmagazine; // ������� �������� ������
			int weapon_distance; // ��������� ���������

		} Weapon[1];


		struct Enemies
		{
			CString target_NAME;
			CString target_type;
			int target_SKILL;
			int target_HP;
			int target_mHP;
			int target_AC;
			int target_AP;
			int target_cAP;
			int target_DMGfrom;
			int	target_DMGto;
			int target_DMGplus;
			int target_requed_dist; // ��������� ��������� ��� ������
			int target_APPenalty;
			CString target_weapon;
			int target_distance;
			int target_xpos;
			int target_ypos;
			bool dead; // �� ����� �� ��� ���������?
			
			CStringArray e_comm; // ��� ����� ������� ������� �� ����� ����������
			
			struct part_names
			{
				CString name;
			} part_n[8][3]; // ������ �������� ������ ����

			// ��� ������� �������� ���� ��������� ��� ����
			CString head;
			CString eyes;
			CString hand;
			CString torso;
			CString leg;
			CString grain;
			
		} Enemy[3];

		int lightning; // ���������, ������� �� �������� �������

		int Targeted[8][4]; // ������� ������������� ����, ������� � ��������� ��	

		// ������� �����
		int XSize;
		int YSize;

		
	protected:

		int Mtarget[8][2]; // ������� ������ ��������� ��� ����������

		CNukeFile CD;
		
		
	public:
		CFight(int mode);
		~CFight();

		void InitFight();
		void InitMeleeFight(int stats[12], CString t_name); // ���������� ��� ����������� ����������
		int HitChance(int part, int t=0);
		int HitChanceForM(int part, int t=0);

		CString HitTarget(int part, int t=0); // ������� ���� ����
		CString HitPlayer(int part, int t=0); // ������� ���� ������

		CString AttackPlayer(int t=0); // ����� ����� �� ;)

		void ReinitFight();

		CString TargetStatus(int t=0);

		void LoadText();

		void LoadScene(CString name);

		void LoadDesc(CString name, int targ);

		void LoadWeapon(CString name, int targ);

		bool CanDo(int ap);

		bool AllDead();

		void LoadWeapon(CString w, NDX_Screen * ParentScreen);

	protected:
		int Rand(int min, int max);
		bool IsEmpty(int x, int y);
		

};

#endif