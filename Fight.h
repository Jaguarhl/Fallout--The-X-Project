/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// All player stats and manipulations here
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __FIGHT_H
#define __FIGHT_H

class CFight : public CObject
{
	public:
		int f_mode; // режим боя (мили / рэнджед)

		bool immortal; // включён ли чит бессмертия?

		int missp, misst; // модификатор дамага
		bool move; // двигалась ли цель

//		CString target_NAME;
//		CString target_type; // кто наша цель

		BYTE targets; // кол-во целей
		
		int p_hpl, t_hpl; // значения жизни для победы
		int l_pw, l_tw; // ссылки на фреймы в случае победы

		int player_SKILL;
		int player_maxHP; // выше этой планки хп у нас быть не может
		int player_HP;
		int player_mHP;
		int player_AC;
		int player_AP;
		int player_cAP;
		int player_DMGfrom;
		int player_SDMGfrom; // здесь храним изначальные данные для мили
		int player_DMGto;
		int player_SDMGto;
		int player_DMGplus;
		int player_SDMGplus;
		int player_APPenalty;
		int player_requed_distance;
		
		struct PWeapon
		{
			NDX_Surface wpn_image; // картинка оружия
			CString player_weapon; // что за оружие у нас в руках?
			int weapon_type; // тип оружия
			int weapon_DMGfrom;
			int weapon_DMGto;
			int weapon_DMGplus;
			int weapon_ammo; // тип патронов, если требуется
			int weapon_magazine; // объём магазина, если требуется
			int	weapon_cmagazine; // сколько патронов сейчас
			int weapon_distance; // дистанция поражения

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
			int target_requed_dist; // требуемая дистанция для оружия
			int target_APPenalty;
			CString target_weapon;
			int target_distance;
			int target_xpos;
			int target_ypos;
			bool dead; // не мертв ли наш противник?
			
			CStringArray e_comm; // тут будем держать коменты на удары противника
			
			struct part_names
			{
				CString name;
			} part_n[8][3]; // список названий частей тела

			// тут текущие названия мест поражения для цели
			CString head;
			CString eyes;
			CString hand;
			CString torso;
			CString leg;
			CString grain;
			
		} Enemy[3];

		int lightning; // освещение, берется от текущего времени

		int Targeted[8][4]; // таблица модификаторов мили, рэнджед и стоимости ап	

		// размеры сцены
		int XSize;
		int YSize;

		
	protected:

		int Mtarget[8][2]; // таблица шансов попаданий для противника

		CNukeFile CD;
		
		
	public:
		CFight(int mode);
		~CFight();

		void InitFight();
		void InitMeleeFight(int stats[12], CString t_name); // инициируем все необходимые переменные
		int HitChance(int part, int t=0);
		int HitChanceForM(int part, int t=0);

		CString HitTarget(int part, int t=0); // нанести цели удар
		CString HitPlayer(int part, int t=0); // нанести удар игроку

		CString AttackPlayer(int t=0); // здесь живет ИИ ;)

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