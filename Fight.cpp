/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// All player stats and manipulations here
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <NukeDX.h>
#include <NukeFile.h>
#include "Fight.h"
#include "INI.h"

CFight::CFight(int mode)
{
	f_mode = mode;
	immortal=false;
	srand((unsigned)time(0));
}

CFight::~CFight()
{	
}

void CFight::InitFight()
{
	// освещение, берется от текущего времени
	CTime t = CTime::GetCurrentTime();
	int r=t.GetHour();
	if(r<=6)
		lightning = 8;
	else if(r<=12)
		lightning = 4;
	else if(r<=18)
		lightning = 0;
	else if(r<=23)
		lightning = 4;

	// melee settings
	Targeted[0][0] = 20;
	Targeted[1][0] = 30;
	Targeted[2][0] = 0;
	Targeted[3][0] = 15;
	Targeted[4][0] = 15;
	Targeted[5][0] = 15;
	Targeted[6][0] = 10;
	Targeted[7][0] = 10;

	// ranged settings
	Targeted[0][1] = 40;
	Targeted[1][1] = 60;
	Targeted[2][1] = 0;
	Targeted[3][1] = 30;
	Targeted[4][1] = 30;
	Targeted[5][1] = 30;
	Targeted[6][1] = 20;
	Targeted[7][1] = 20;

	// ap costs
	Targeted[0][2] = 5;
	Targeted[1][2] = 6;
	Targeted[2][2] = 4;
	Targeted[3][2] = 5;
	Targeted[4][2] = 5;
	Targeted[5][2] = 6;
	Targeted[6][2] = 4;
	Targeted[7][2] = 4;

	// critical strike bonuses
	Targeted[0][3] = 15;
	Targeted[1][3] = 20;
	Targeted[2][3] = 0;
	Targeted[3][3] = 5;
	Targeted[4][3] = 5;
	Targeted[5][3] = 10;
	Targeted[6][3] = 10;
	Targeted[7][3] = 10;

	for(int i=0; i<targets; ++i)
	{
		Enemy[i].part_n[0][0].name = "голове";
		Enemy[i].part_n[1][0].name = "глазам";
		Enemy[i].part_n[2][0].name = "туловищу";
		Enemy[i].part_n[3][0].name = "правой руке";
		Enemy[i].part_n[4][0].name = "левой руке";
		Enemy[i].part_n[5][0].name = "паху";
		Enemy[i].part_n[6][0].name = "правой ноге";
		Enemy[i].part_n[7][0].name = "левой ноге";
		
		Enemy[i].part_n[0][1].name = "твоей";
		Enemy[i].part_n[1][1].name = "твоим";
		Enemy[i].part_n[2][1].name = "твоему";
		Enemy[i].part_n[3][1].name = "твоей";
		Enemy[i].part_n[4][1].name = "твоей";
		Enemy[i].part_n[5][1].name = "твоему";
		Enemy[i].part_n[6][1].name = "твоей";
		Enemy[i].part_n[7][1].name = "твоей";
	}

	for(i=0; i<8; i++)
	{
		Mtarget[i][0] = HitChanceForM(i);
	}
}

void CFight::LoadWeapon(CString w, NDX_Screen * ParentScreen)
{
	if(w!="")
	{
		CString tmp = w;
		
		tmp+=".bmp";
		LPSTR fn;
		fn = tmp.GetBuffer(tmp.GetLength());
		tmp.ReleaseBuffer();
		
		CD.SetFileDescription("Fallout Data File");
		CD.Open("CRITTERS.DAT");

		if(CD.IsFileOpen())
		{
			Weapon[0].wpn_image.LoadBMP(ParentScreen,NDXST_SYSMEM,CD.GetFile(fn),true);
			Weapon[0].wpn_image.SetColorKey(ParentScreen->PixelFormat->Rgb(255,0,255));
			Weapon[0].wpn_image.SetTransP(true);
			
			tmp = w;
			tmp+=".des";
			fn = tmp.GetBuffer(tmp.GetLength());
			tmp.ReleaseBuffer();
			
			CD.ExtractFile_ToFile(fn,"data\\tempt");
			
			CIni ini;

			ini.SetPathName("data\\tempt");

			Weapon[0].player_weapon = w;
			Weapon[0].weapon_ammo = ini.GetInt("Main", "Ammo", 0);
			Weapon[0].weapon_cmagazine = ini.GetInt("Main", "Magazine", 0);
			Weapon[0].weapon_distance = ini.GetInt("Main", "Distance", 1);
			Weapon[0].weapon_DMGfrom = ini.GetInt("Main", "MinEffect", 0);
			Weapon[0].weapon_DMGplus = ini.GetInt("Main", "Bonus", 0);
			Weapon[0].weapon_DMGto = ini.GetInt("Main", "MaxEffect", 0);
			Weapon[0].weapon_magazine = ini.GetInt("Main", "Magazine", 0);
			Weapon[0].weapon_type = ini.GetInt("Main", "DmgType", 1);
			
			DeleteFile("data\\tempt");

			if(Weapon[0].weapon_type==1)
			{
				player_DMGfrom = player_SDMGfrom + Weapon[0].weapon_DMGfrom;
				player_DMGto = player_SDMGto + Weapon[0].weapon_DMGto;
				player_DMGplus = player_SDMGplus + Weapon[0].weapon_DMGplus;
			}
		}

		CD.Close();
	}
	else
	{
		Weapon[0].player_weapon=w;
		player_DMGfrom = player_SDMGfrom;
		player_DMGto = player_SDMGto;
		player_DMGplus = player_SDMGplus;
	}
}

void CFight::InitMeleeFight(int stats[12], CString t_name)
{
//	dmg_mode = dmg;

	f_mode = 0;

	InitFight();

	missp = misst = 0;
	targets = 1;

	if(player_SKILL < stats[2])
		player_SKILL = stats[2];
	player_HP = stats[0];
	player_mHP = stats[0];
	player_AC = stats[4];
	player_AP = stats[6];
	player_cAP = stats[6];
	/*player_DMGto = stats[8];
	player_DMGplus = stats[10];*/
	player_APPenalty=0;
	player_requed_distance = 1;

	Enemy[0].target_NAME = t_name;
	Enemy[0].target_weapon = Enemy[0].target_type;
	Enemy[0].target_SKILL = stats[3];
	Enemy[0].target_HP = stats[1];
	Enemy[0].target_mHP = stats[1];
	Enemy[0].target_AC = stats[5];
	Enemy[0].target_AP = stats[7];
	Enemy[0].target_cAP = stats[7];
	Enemy[0].target_DMGfrom = 1;
	Enemy[0].target_DMGto = stats[9];
	Enemy[0].target_DMGplus = stats[11];
	Enemy[0].target_APPenalty=0;
	Enemy[0].target_requed_dist=1;
	Enemy[0].target_xpos=0;
	Enemy[0].target_ypos=1;
	Enemy[0].target_distance=1;
	Enemy[0].dead=false;
}

int CFight::HitChance(int part, int t)
{
	int chance = player_SKILL - lightning - Enemy[t].target_AC/10 - Targeted[part][f_mode];

	if(chance < 0)
		chance=0;
	else if(chance > 99)
		chance=99;

	return chance;
}

int CFight::HitChanceForM(int part, int t)
{
	int chance = Enemy[t].target_SKILL - lightning - player_AC/10 - Targeted[part][f_mode];

	if(chance < 0)
		chance=0;
	else if(chance > 99)
		chance=99;

	return chance;
}

CString CFight::HitTarget(int part, int t)
{
	CString result;
	move=false;

	if((player_cAP >= Targeted[part][2])&&(player_requed_distance>=Enemy[t].target_distance))
	{
		player_cAP = player_cAP - Targeted[part][2]; // вычли ап на удар

		// узнаем, попали ли мы
		int hit1 = Rand(0,9);
		int hit2 = Rand(0,9);
		if((hit1*10+hit2 >= HitChance(part))&&(missp!=0))
		{
			int hit1 = Rand(0,9);
			int hit2 = Rand(0,9);
		}
		if(hit1*10+hit2 <= HitChance(part))
		{
			missp=0;
			// узнаем, парировал ли удар
			int parry = Rand(0,100);
			int k;
			if(player_SKILL - Enemy[t].target_SKILL < 0)
				k = 0;
			else
				k = player_SKILL / Enemy[t].target_SKILL * 3;
			k+=Rand(1,7);

			if((parry > k + 10)&&(f_mode==0))
			{
			int dmg = Rand(player_DMGfrom,player_DMGto); // посчитаем силу удара
			dmg += player_DMGplus;

			// узнаем, критический ли удар
			int crit = Rand(0,100);

			CString s;

			if(crit <= (((Enemy[t].target_SKILL/300)*Targeted[part][3]+Targeted[part][3])-((player_SKILL/300)*Targeted[part][3]+Targeted[part][3])+Targeted[part][3]))
			{
				if(part==0)
				{
					// проверим, не сняли мы этим ударом АП противнику
					k=Rand(0,10);
					if(k<2)
						Enemy[t].target_APPenalty=1;
					dmg=dmg + dmg*1.5;
				}
				else if(part==5)
				{
					// проверим, не сняли мы этим ударом АП противнику
					k=Rand(0,10);
					if(k<2)
						Enemy[t].target_APPenalty=3;
					dmg=dmg +dmg*2;
				}
				else if(part==1)
				{
					// проверим, не сняли мы этим ударом АП противнику
					k=Rand(0,10);
					if(k<2)
						Enemy[t].target_APPenalty=2;
					dmg=dmg +dmg*2;
				}
				else
					dmg=dmg*2;

				char num[10];
				itoa(dmg, num, 10);
				s=num;

				k = Rand(0,2);
				
				result=Enemy[t].e_comm.GetAt(14)+Enemy[t].target_NAME + Enemy[t].e_comm.GetAt(15) + Enemy[t].part_n[part][2].name + Enemy[t].e_comm.GetAt(16) + s + Enemy[t].e_comm.GetAt(17);
				
				if((k==1)&&(part==5))
					result=Enemy[t].target_NAME+Enemy[t].e_comm.GetAt(18)+s+Enemy[t].e_comm.GetAt(19);
				else if((k==2)&&(part==5))
					result=Enemy[t].e_comm.GetAt(20)+Enemy[t].target_NAME+Enemy[t].e_comm.GetAt(21)+s+Enemy[t].e_comm.GetAt(22);
				else if((k==1)&&(part==0))
					result=Enemy[t].target_NAME+Enemy[t].e_comm.GetAt(23)+Enemy[t].part_n[part][2].name+Enemy[t].e_comm.GetAt(24)+s+Enemy[t].e_comm.GetAt(25);
				else if((k==2)&&(part==0))
					result=Enemy[t].e_comm.GetAt(26)+Enemy[t].target_NAME+Enemy[t].e_comm.GetAt(27)+s+Enemy[t].e_comm.GetAt(28)+Enemy[t].target_NAME+Enemy[t].e_comm.GetAt(29);
				else if((k==1)&&(part==1))
					result=Enemy[t].target_NAME+Enemy[t].e_comm.GetAt(30)+s+Enemy[t].e_comm.GetAt(31);
				else if((k==2)&&(part==1))
					result=Enemy[t].e_comm.GetAt(32)+s+Enemy[t].e_comm.GetAt(33)+Enemy[t].target_NAME+Enemy[t].e_comm.GetAt(34);
				else if((k==1)&&(part==3))
					result=Enemy[t].e_comm.GetAt(35)+Enemy[t].target_NAME+Enemy[t].e_comm.GetAt(36)+s+Enemy[t].e_comm.GetAt(37);
				else if((k==1)&&(part==7))
					result=Enemy[t].e_comm.GetAt(38)+Enemy[t].target_NAME+Enemy[t].e_comm.GetAt(39)+s+Enemy[t].e_comm.GetAt(40);

				// сообщим, что ап в следующем ходу у противника будет меньше
				if((part==5)&&(Enemy[t].target_APPenalty==3))
					result=Enemy[t].target_NAME+Enemy[t].e_comm.GetAt(41)+s+Enemy[t].e_comm.GetAt(42);
				if((part==0)&&(Enemy[t].target_APPenalty==1))
					result=Enemy[t].e_comm.GetAt(43)+Enemy[t].target_NAME+Enemy[t].e_comm.GetAt(44)+s+Enemy[t].e_comm.GetAt(45);
				if((part==1)&&(Enemy[t].target_APPenalty==2))
					result=Enemy[t].target_NAME+Enemy[t].e_comm.GetAt(46)+s+".";


			}
			else
			{
				char num[10];
				itoa(dmg, num, 10);
				s=num;
				
				result=Enemy[t].e_comm.GetAt(47)+Enemy[t].target_NAME + Enemy[t].e_comm.GetAt(48) + Enemy[t].part_n[part][2].name + Enemy[t].e_comm.GetAt(49) + s+Enemy[t].e_comm.GetAt(50);			
			}

			Enemy[t].target_HP = Enemy[t].target_HP - dmg;
			}
			else
			{
				missp++;
				if(Rand(0,1)==0)
					result=Enemy[t].target_NAME + Enemy[t].e_comm.GetAt(51);
				else
					result=Enemy[t].target_NAME + Enemy[t].e_comm.GetAt(52)+ Enemy[t].part_n[part][2].name+Enemy[t].e_comm.GetAt(53);
			}
		}
		else
		{
			missp++;
			// узнаем, имел ли место критический промах
			int k = Enemy[t].target_SKILL / player_SKILL * 2;
			k += Rand(0,5);
			if(Rand(0,100)<k)
			{
				// имеет место быть критический промах
				if(Rand(0,2)==0)
				{
					// снимаем только ап
					player_cAP -=2;
					result =Enemy[t].e_comm.GetAt(54) + Enemy[t].part_n[part][2].name + Enemy[t].e_comm.GetAt(55) + Enemy[t].target_NAME + Enemy[t].e_comm.GetAt(56);
				}
				else if(Rand(0,2)==1)
				{
					// снимаем только жизнь
					int dmg = Rand(1,4);
					if(!immortal)
						player_HP -= dmg;
					CString s;
					char num[10];
					itoa(dmg, num, 10);
					s=num;
					result =Enemy[t].e_comm.GetAt(57) + Enemy[t].part_n[part][2].name + Enemy[t].e_comm.GetAt(58) + Enemy[t].target_NAME +  Enemy[t].e_comm.GetAt(59)+s+Enemy[t].e_comm.GetAt(60);
				}
				else if(Rand(0,2)==2)
				{
					// снимаем жизнь и ап
					player_cAP -=2;
					int dmg = Rand(1,4);
					if(!immortal)
						player_HP -= dmg;
					CString s;
					char num[10];
					itoa(dmg, num, 10);
					s=num;
					result =Enemy[t].e_comm.GetAt(61) + Enemy[t].part_n[part][2].name + Enemy[t].e_comm.GetAt(62) + Enemy[t].target_NAME + Enemy[t].e_comm.GetAt(63)+s+Enemy[t].e_comm.GetAt(64);
				}
			}
			else
				result = Enemy[t].e_comm.GetAt(65) + Enemy[t].part_n[part][2].name + Enemy[t].e_comm.GetAt(66) + Enemy[t].target_NAME+Enemy[t].e_comm.GetAt(67);
		}
	}
	else if((player_cAP >= Targeted[part][2])&&(player_requed_distance<Enemy[t].target_distance))
	{
		move=true;
		result = "Цель вне радиуса атаки.";
	}
	else
		result="Недостаточно очков действия для нанесения этого удара.";

	Enemy[t].target_cAP -= Enemy[t].target_APPenalty;
	Enemy[t].target_APPenalty=0;

	if(Enemy[t].target_HP<0)
		Enemy[t].target_HP=0;

	if(Enemy[t].target_HP==0)
	{
		result += " ";
		result += Enemy[t].target_NAME;
		result += Enemy[t].e_comm.GetAt(120);
	}

	return result;
}

CString CFight::HitPlayer(int part, int t)
{
	CString result="";

	if(Enemy[t].target_cAP >= Targeted[part][2])
	{
		Enemy[t].target_cAP = Enemy[t].target_cAP - Targeted[part][2]; // вычли ап на удар

		// узнаем, попали ли мы
		int hit1 = Rand(0,9);
		int hit2 = Rand(0,9);
		if((hit1+hit2 >= Mtarget[part][0])&&(misst!=0))
		{
			int hit1 = Rand(0,9);
			int hit2 = Rand(0,9);
		}
		if(hit1*10+hit2 <= /*Mtarget[part][0]*/ HitChanceForM(part, t))
		{
			misst=0;
			// узнаем, парировал ли удар
			int parry = Rand(0,100);
			int k;
			if(player_SKILL - Enemy[t].target_SKILL < 0)
				k = 0;
			else
				k = player_SKILL / Enemy[t].target_SKILL * 3;
			k += Rand(1,7);

			if((parry > k)&&(f_mode==0))
			{
			int dmg = Rand(Enemy[t].target_DMGfrom, Enemy[t].target_DMGto); // посчитаем силу удара
			dmg += Enemy[t].target_DMGplus;

			// узнаем, критический ли удар
			int crit = Rand(0,100);

			CString s;

			if(crit <= (((player_SKILL/300)*Targeted[part][3]+Targeted[part][3])-((Enemy[t].target_SKILL/300)*Targeted[part][3]+Targeted[part][3])+Targeted[part][3]))
			{
				if(part==0)
				{
					// проверим, не сняли мы этим ударом АП игроку
					k=Rand(0,10);
					if(k<2)
						player_APPenalty=1;
					dmg=dmg + dmg*1.5;
				}
				else if(part==5)
				{
					// проверим, не сняли мы этим ударом АП игроку
					k=Rand(0,10);
					if(k<2)
						player_APPenalty=3;
					dmg=dmg +dmg*2;
				}
				else if(part==1)
				{
					// проверим, не сняли мы этим ударом АП игроку
					k=Rand(0,10);
					if(k<2)
						player_APPenalty=2;
					dmg=dmg +dmg*2;
				}
				else
					dmg=dmg*2;

				char num[10];
				itoa(dmg, num, 10);
				s=num;

				k = Rand(0,2);

				result=Enemy[t].target_NAME + Enemy[t].e_comm.GetAt(68) + Enemy[t].part_n[part][1].name + Enemy[t].e_comm.GetAt(69) + Enemy[t].part_n[part][0].name + Enemy[t].e_comm.GetAt(70) + s+Enemy[t].e_comm.GetAt(71);

				if((k==1)&&(part==5))
					result=Enemy[t].target_NAME+Enemy[t].e_comm.GetAt(72)+s+Enemy[t].e_comm.GetAt(73);
				else if((k==2)&&(part==5))
					result=Enemy[t].e_comm.GetAt(74)+Enemy[t].target_NAME+Enemy[t].e_comm.GetAt(75)+s+Enemy[t].e_comm.GetAt(76);
//				else if((k==1)&&(part==0))
//					result=target_NAME+" пропускает критический удар по "+part_n[part][0].name+" на "+s+" и некоторое время пытается сообразить где верх, а где низ.";
				else if((k==2)&&(part==0))
					result=Enemy[t].target_NAME+Enemy[t].e_comm.GetAt(77)+s+Enemy[t].e_comm.GetAt(78);
//				else if((k==1)&&(part==1))
//					result=target_NAME+" кричит: \"Мои глаза! Я ничего не вижу, что ты сделал с моими глазами?\" Да, видимо зрение не скоро вернется к нему после твоего критического удара на "+s+".";
				else if((k==2)&&(part==1))
					result=Enemy[t].target_NAME+Enemy[t].e_comm.GetAt(79)+s+Enemy[t].e_comm.GetAt(80);
				else if((k==1)&&(part==4))
					result=Enemy[t].e_comm.GetAt(81)+Enemy[t].target_NAME+Enemy[t].e_comm.GetAt(82)+s+Enemy[t].e_comm.GetAt(83);
				else if((k==1)&&(part==6))
					result=Enemy[t].e_comm.GetAt(84)+Enemy[t].target_NAME+Enemy[t].e_comm.GetAt(85)+s+Enemy[t].e_comm.GetAt(86);

				// сообщим, что ап в следующем ходу у противника будет меньше
				if((part==5)&&(player_APPenalty==3))
					result=Enemy[t].e_comm.GetAt(87)+Enemy[t].target_NAME+Enemy[t].e_comm.GetAt(88)+s+Enemy[t].e_comm.GetAt(89);
				if((part==0)&&(player_APPenalty==1))
					result=Enemy[t].e_comm.GetAt(90)+Enemy[t].target_NAME+Enemy[t].e_comm.GetAt(91)+s+Enemy[t].e_comm.GetAt(92);
				if((part==1)&&(player_APPenalty==2))
					result=Enemy[t].e_comm.GetAt(93)+Enemy[t].target_NAME+Enemy[t].e_comm.GetAt(94)+s+Enemy[t].e_comm.GetAt(95);
			}
			else
			{
				char num[10];
				itoa(dmg, num, 10);
				s=num;
				
				result=Enemy[t].target_NAME + Enemy[t].e_comm.GetAt(96) + Enemy[t].part_n[part][1].name + Enemy[t].e_comm.GetAt(97) + Enemy[t].part_n[part][0].name + Enemy[t].e_comm.GetAt(98) + s+Enemy[t].e_comm.GetAt(99);	
			}

			if(!immortal)
				player_HP = player_HP - dmg;
			}
			else
			{
				misst++;
				if(Rand(0,1)==0)
					result=Enemy[t].e_comm.GetAt(100)+Enemy[t].target_NAME+Enemy[t].e_comm.GetAt(101);
				else
					result=Enemy[t].e_comm.GetAt(102)+Enemy[t].target_NAME+Enemy[t].e_comm.GetAt(103)+Enemy[t].part_n[part][1].name+Enemy[t].e_comm.GetAt(104)+ Enemy[t].part_n[part][0].name+Enemy[t].e_comm.GetAt(105);
			}
		}
		else
		{
			misst++;
			// узнаем, имел ли место критический промах
			int k = player_SKILL / Enemy[t].target_SKILL * 2;
			k += Rand(0,5);
			if(Rand(0,100)<k)
			{
				// имеет место быть критический промах
				if(Rand(0,2)==0)
				{
					// снимаем только ап
					Enemy[t].target_cAP -=2;
					result =Enemy[t].target_NAME + Enemy[t].e_comm.GetAt(106) + Enemy[t].part_n[part][1].name + Enemy[t].e_comm.GetAt(107) + Enemy[t].part_n[part][0].name + Enemy[t].e_comm.GetAt(108);
				}
				else if(Rand(0,2)==1)
				{
					// снимаем только жизнь
					int dmg = Rand(1,4);
					Enemy[t].target_HP -= dmg;
					CString s;
					char num[10];
					itoa(dmg, num, 10);
					s=num;
					result =Enemy[t].target_NAME + Enemy[t].e_comm.GetAt(109) + Enemy[t].part_n[part][1].name + Enemy[t].e_comm.GetAt(110) + Enemy[t].part_n[part][0].name + Enemy[t].e_comm.GetAt(111)+s+Enemy[t].e_comm.GetAt(112);
				}
				else if(Rand(0,2)==2)
				{
					// снимаем жизнь и ап
					Enemy[t].target_cAP -=2;
					int dmg = Rand(1,4);
					Enemy[t].target_HP -= dmg;
					CString s;
					char num[10];
					itoa(dmg, num, 10);
					s=num;
					result =Enemy[t].target_NAME + Enemy[t].e_comm.GetAt(113) + Enemy[t].part_n[part][1].name + Enemy[t].e_comm.GetAt(114) + Enemy[t].part_n[part][0].name + Enemy[t].e_comm.GetAt(115)+s+Enemy[t].e_comm.GetAt(116);
				}
			}
			else
				result =Enemy[t].target_NAME + Enemy[t].e_comm.GetAt(117) + Enemy[t].part_n[part][1].name + Enemy[t].e_comm.GetAt(118) + Enemy[t].part_n[part][0].name + Enemy[t].e_comm.GetAt(119);
		}
	}

	if(player_HP<0)
		player_HP=0;
	return result;
}

CString CFight::AttackPlayer(int t)
{
	move=false;

	if(Enemy[t].target_distance<=Enemy[t].target_requed_dist)
	{
		// выставим оценки ударов по шансу попадания
		Mtarget[0][1] = 2;
		Mtarget[1][1] = 1;
		Mtarget[2][1] = 5;
		Mtarget[3][1] = 3;
		Mtarget[4][1] = 3;
		Mtarget[5][1] = 3;
		Mtarget[6][1] = 4;
		Mtarget[7][1] = 4;

		// выставим оценки ударам с учетом максимальных критов по доступному количеству АП
		if(Enemy[t].target_cAP == 4)
		{
			Mtarget[0][1] += 0;
			Mtarget[1][1] += 0;
			Mtarget[2][1] += 4;
			Mtarget[3][1] += 0;
			Mtarget[4][1] += 0;
			Mtarget[5][1] += 0;
			Mtarget[6][1] += 5;
			Mtarget[7][1] += 5;
		}
		else if(Enemy[t].target_cAP >= 5)
		{
			Mtarget[0][1] += 5;
			Mtarget[1][1] += 0;
			Mtarget[2][1] += 2;
			Mtarget[3][1] += 3;
			Mtarget[4][1] += 3;
			Mtarget[5][1] += 0;
			Mtarget[6][1] += 4;
			Mtarget[7][1] += 4;
		}
		else if(Enemy[t].target_cAP >= 6)
		{
			Mtarget[0][1] += 4;
			Mtarget[1][1] += 5;
			Mtarget[2][1] += 1;
			Mtarget[3][1] += 2;
			Mtarget[4][1] += 2;
			Mtarget[5][1] += 3;
			Mtarget[6][1] += 3;
			Mtarget[7][1] += 3;		
		}

		int most[3]; // загоним сюда 3 самых выверенных удара
		for(int i=0; i<3; ++i)
			most[i] = 0;

		// но предварительно выберем тактику в зависимости от количества ап, которым располагаем
		if(Enemy[t].target_AP < 10)
		{
			if(Enemy[t].target_cAP==8)
			{
				int s1 = Rand(0,6);
				int s2 = Rand(0,6);

				if(s1+s2>4)
				{
					Mtarget[0][1] += 0;
					Mtarget[1][1] += 0;
					Mtarget[2][1] += 4;
					Mtarget[3][1] += 0;
					Mtarget[4][1] += 0;
					Mtarget[5][1] += 0;
					Mtarget[6][1] += 5;
					Mtarget[7][1] += 5;
				}
				else if(s1+s2 == 4)
					Mtarget[0][1] += 5;
				else if(s1+s2 == 3)
					Mtarget[5][1] += 5;
				else if(s1+s2 < 3)
					Mtarget[1][1] += 5;
			}
			else if(Enemy[t].target_cAP==9)
			{
				int s1 = Rand(0,6);
				int s2 = Rand(0,6);

				if((s1+s2>3)&&(s1+s2<6))
				{
					Mtarget[0][1] += 5;
					Mtarget[1][1] += 0;
					Mtarget[2][1] += 4;
					Mtarget[3][1] += 3;
					Mtarget[4][1] += 4;
					Mtarget[5][1] += 0;
					Mtarget[6][1] += 5;
					Mtarget[7][1] += 5;
				}
				else if(s1+s2 == 4)
					Mtarget[0][1] += 5;
				else if(s1+s2 == 3)
					Mtarget[5][1] += 5;
				else if(s1+s2 < 3)
					Mtarget[1][1] += 5;
			}
		}

		for(i=0; i<8; ++i)
		{
			if((Mtarget[i][0] > Mtarget[most[0]][0])&&(Enemy[t].target_cAP >= Targeted[i][2]))
				most[0] = i;
		}
		for(i=0; i<8; ++i)
		{
			if((Mtarget[i][0] < Mtarget[most[0]][0])&&(Mtarget[i][0] > Mtarget[most[1]][0])&&(Enemy[t].target_cAP >= Targeted[i][2]))
				most[1] = i;
		}
		for(i=0; i<8; ++i)
		{
			if((Mtarget[i][0] < Mtarget[most[0]][0])&&(Mtarget[i][0] < Mtarget[most[1]][0])&&(Mtarget[i][1] > Mtarget[most[1]][0])&&(Enemy[t].target_cAP >= Targeted[i][2]))
				most[2] = i;
		}
		// узнаем, превышает ли вероятность попадания 75%, 50% или 40%
		for(i=0; i<8; ++i)
		{
			if(Mtarget[i][0] >= 75)
				Mtarget[i][1] += 4;
			else if(Mtarget[i][0] >= 50)
				Mtarget[i][1] += 3;
			else if(Mtarget[i][0] >= 40)
				Mtarget[i][1] += 2;
			else if(Mtarget[i][0] >= 30)
				Mtarget[i][1] += 1;
		}

		// теперь выберем 3 наиболее удачных варианта
		for(i=0; i<3; ++i)
			most[i] = 0;

		for(i=0; i<8; ++i)
		{
			if((Mtarget[i][1] > Mtarget[most[0]][1])&&(Enemy[t].target_cAP >= Targeted[i][2]))
				most[0] = i;
		}
		for(i=0; i<8; ++i)
		{
			if((Mtarget[i][1] <= Mtarget[most[0]][1])&&(Mtarget[i][1] > Mtarget[most[1]][1])&&(Enemy[t].target_cAP >= Targeted[i][2]))
				most[1] = i;
		}
		for(i=0; i<8; ++i)
		{
			if((Mtarget[i][1] <= Mtarget[most[0]][1])&&(Mtarget[i][1] <= Mtarget[most[1]][1])&&(Mtarget[i][1] > Mtarget[most[2]][1])&&(Enemy[t].target_cAP >= Targeted[i][2]))
				most[2] = i;
		}
			

		// собственно, выберем случайным образом удар из этих 3х
		//	int r = rand() % 2;
		
		return HitPlayer(most[Rand(0,2)], t);
	}
	else // если игрок вне досягаемости оружия, надо к нему приблизиться
	{
		int XPos; // сюда пишем новые координаты
		int YPos; 
		int APneeded; // сюда необходимое ап для передвижения в эту точку
		int distance=100; // сюда дистанцию до игрока

		for(int yp=Enemy[t].target_ypos; yp>0; --yp)
		{
			for(int xp=0-XSize/2; xp<=XSize/2; ++xp)
			{
				int xp2 = xp*xp;
				int yp2 = yp*yp;
				int dst = sqrt(xp2 + yp2);
				xp2 = (Enemy[t].target_xpos-xp)*(Enemy[t].target_xpos-xp);
				yp2 = (Enemy[t].target_ypos-yp)*(Enemy[t].target_ypos-yp);

				int rAP = sqrt(xp2 + yp2);
				rAP = rAP*2;

				if(rAP<=Enemy[t].target_cAP)
				{
					if(dst==Enemy[t].target_requed_dist)
					{
						if((distance>=dst)&&(IsEmpty(xp, yp)))
						{
							distance=dst;
							APneeded=rAP;
							XPos=xp;
							YPos=yp;
						}
						
					}
				}

/*				CString s="Dst: ";
			char num[10];
			itoa(dst, num, 10);
			s+=num;
			s+=" rAP: ";
			itoa(rAP, num, 10);
			s+=num;
			s+=" X: ";
			itoa(xp, num, 10);
			s+=num;
			s+=" Y: ";
			itoa(yp, num, 10);
			s+=num;
			s+=" Target: ";
			itoa(t, num, 10);
			s+=num;
			s+=" TAP: ";
			itoa(Enemy[t].target_cAP, num, 10);
			s+=num;
			s+=" Trdst: ";
			itoa(Enemy[t].target_requed_dist, num, 10);
			s+=num;
			AfxMessageBox(s);*/
			}
		}

		if(distance <100)
		{
			move = true;
			Enemy[t].target_xpos=XPos;
			Enemy[t].target_ypos=YPos;
			Enemy[t].target_distance=distance;
			Enemy[t].target_cAP-=APneeded;

			CString result=Enemy[t].target_NAME;
			result+=Enemy[t].e_comm.GetAt(121);

/*			CString s="Dst: ";
			char num[10];
			itoa(distance, num, 10);
			s+=num;
			s+=" rAP: ";
			itoa(APneeded, num, 10);
			s+=num;
			s+=" X: ";
			itoa(XPos, num, 10);
			s+=num;
			s+=" Y: ";
			itoa(YPos, num, 10);
			s+=num;
			s+=" Target: ";
			itoa(t, num, 10);
			s+=num;
			AfxMessageBox(s);
			//		result+=".";*/

			return result;
		}
		else
			return "";
	}	
}

bool CFight::IsEmpty(int x, int y)
{
	bool res = true;
	
	for(int i=0; i<targets; ++i)
	{
		if((Enemy[i].target_xpos==x)&&(Enemy[i].target_ypos==y))
		{
			res = false;
			break;
		}
	}

	return res;
}

void CFight::ReinitFight()
{
	player_cAP = player_AP-player_APPenalty;
	player_APPenalty=0;

	for(int i=0; i<targets; ++i)
	{
		Enemy[i].target_cAP = Enemy[i].target_AP - Enemy[i].target_APPenalty;
	}
}

int CFight::Rand(int min, int max)
{
	return (double)rand() / (RAND_MAX + 1) * (max - min) + min;
}

CString CFight::TargetStatus(int t)
{
	// расскажем о внешнем виде цели

	CString r = "здоров";

	double hp = (double)Enemy[t].target_HP / (double)Enemy[t].target_mHP;
	if((hp<1)&&(hp>=0.75))
		r = "легко ранен";
	else if((hp<0.75)&&(hp>=0.5))
		r = "ранен";
	else if((hp<0.5)&&(hp>=0.25))
		r = "серьезно ранен";
	else if((hp<0.25)&&(hp>0))
		r = "смертельно ранен";
	else if(hp==0)
		r = "мертв";

	return r;
}

void CFight::LoadText()
{
	CD.SetFileDescription("Fallout Data File");
	CD.Open("CRITTERS.DAT");

	if(CD.IsFileOpen())
	{		
		for(int i=0; i<targets; ++i)
		{
			CString name = "TXT/";
			name += Enemy[i].target_type;
			name += ".txt";
			
			LPSTR fn = name.GetBuffer(name.GetLength());
			name.ReleaseBuffer();

			CD.ExtractFile_ToFile(fn,"data\\tempt");
			
			Enemy[i].e_comm.RemoveAll();

			CStdioFile file;
			
			BOOL opened = file.Open("data\\tempt", CFile::modeRead);
			if (opened)
			{
				CString string;
				while(file.ReadString(string))
				{
					Enemy[i].e_comm.Add(string);
				}
				
				Enemy[i].head = Enemy[i].e_comm.GetAt(0);
				Enemy[i].eyes = Enemy[i].e_comm.GetAt(1);
				Enemy[i].hand = Enemy[i].e_comm.GetAt(2);
				Enemy[i].torso = Enemy[i].e_comm.GetAt(3);
				Enemy[i].leg = Enemy[i].e_comm.GetAt(4);
				Enemy[i].grain = Enemy[i].e_comm.GetAt(5);
				
				Enemy[i].part_n[0][2].name = Enemy[i].e_comm.GetAt(6);
				Enemy[i].part_n[1][2].name = Enemy[i].e_comm.GetAt(7);
				Enemy[i].part_n[2][2].name = Enemy[i].e_comm.GetAt(8);
				Enemy[i].part_n[3][2].name = Enemy[i].e_comm.GetAt(9);
				Enemy[i].part_n[4][2].name = Enemy[i].e_comm.GetAt(10);
				Enemy[i].part_n[5][2].name = Enemy[i].e_comm.GetAt(11);
				Enemy[i].part_n[6][2].name = Enemy[i].e_comm.GetAt(12);
				Enemy[i].part_n[7][2].name = Enemy[i].e_comm.GetAt(13);
			}
			else
				AfxMessageBox("Can't find data file!");

//			DeleteFile("data\\tempt");

		}

		DeleteFile("data\\tempt");
	}

	CD.Close();
}

bool CFight::CanDo(int ap)
{
	if(player_cAP - ap >= 0)
		return true;
	else 
		return false;
}

bool CFight::AllDead()
{
	bool res=true;

	for(int i=0; i<targets; ++i)
	{
		if(Enemy[i].dead==false)
			res=false;
	}

	return res;
}

void CFight::LoadScene(CString name)
{
	CString tmp = "SCENE/";
	tmp+=name;
	tmp+=".des";
	LPSTR fn;
	fn = tmp.GetBuffer(tmp.GetLength());
	tmp.ReleaseBuffer();

	targets=0;
	p_hpl = t_hpl =0;

	CD.SetFileDescription("Fallout Data File");
	CD.Open("CRITTERS.DAT");

	if(CD.IsFileOpen())
	{
		CD.ExtractFile_ToFile(fn,"data\\temps");
		
		CIni ini;
		ini.SetPathName("data\\temps");

		// загрузим основные переменные сцены
		lightning = ini.GetInt("General", "Light", 0);
		XSize = ini.GetInt("General", "XSize", 0);
		YSize = ini.GetInt("General", "YSize", 0);

		// загрузим противников
		if(ini.IsSectionExist("Enemy1"))
		{
			targets++;
			tmp = ini.GetString("Enemy1", "Type");
			LoadDesc(tmp, 0);
			Enemy[0].target_xpos=ini.GetInt("Enemy1", "X", 0);
			Enemy[0].target_ypos=ini.GetInt("Enemy1", "Y", 0);
			Enemy[0].target_distance=sqrt((Enemy[0].target_xpos^2) + (Enemy[0].target_ypos^2));
			Enemy[0].dead=false;
		}
		else
			Enemy[0].dead=true;

		if(ini.IsSectionExist("Enemy2"))
		{
			targets++;
			tmp = ini.GetString("Enemy2", "Type");
			LoadDesc(tmp, 1);
			Enemy[1].target_xpos=ini.GetInt("Enemy2", "X", 0);
			Enemy[1].target_ypos=ini.GetInt("Enemy2", "Y", 0);
			Enemy[1].target_distance=sqrt((Enemy[1].target_xpos^2) + (Enemy[1].target_ypos^2));
			Enemy[1].dead=false;
		}
		else
			Enemy[1].dead=true;

		if(ini.IsSectionExist("Enemy3"))
		{
			targets++;
			tmp = ini.GetString("Enemy3", "Type");
			LoadDesc(tmp, 2);
			Enemy[2].target_xpos=ini.GetInt("Enemy3", "X", 0);
			Enemy[2].target_ypos=ini.GetInt("Enemy3", "Y", 0);
			Enemy[2].target_distance=sqrt((Enemy[2].target_xpos^2) + (Enemy[2].target_ypos^2));
			Enemy[2].dead=false;
		}
		else
			Enemy[2].dead=true;
				
		DeleteFile("data\\temps");
	}

	CD.Close();
	
	InitFight();
}

void CFight::LoadDesc(CString name, int targ)
{
	CString tmp = "CREATURES/";
	tmp+=name;
	tmp+=".des";
	LPSTR fn;
	fn = tmp.GetBuffer(tmp.GetLength());
	tmp.ReleaseBuffer();

//	CD.SetFileDescription("Fallout Data File");
//	CD.Open("CRITTERS.DAT");

	if(CD.IsFileOpen())
	{
//		AfxMessageBox(fn);
		CD.ExtractFile_ToFile(fn,"data\\tempc");

		CIni ini;
		ini.SetPathName("data\\tempc");

		// загрузим основные переменные
		Enemy[targ].target_NAME = ini.GetString("General", "Name");
		Enemy[targ].target_type = ini.GetString("General", "Type");
//		AfxMessageBox(ini.GetString("General", "Type"));
		Enemy[targ].target_SKILL = ini.GetInt("General", "Skill", 0);
		Enemy[targ].target_HP = ini.GetInt("General", "HP", 0);
		Enemy[targ].target_mHP = ini.GetInt("General", "HP", 0);
		Enemy[targ].target_AC = ini.GetInt("General", "AC", 0);
		Enemy[targ].target_AP = ini.GetInt("General", "AP", 0);
		Enemy[targ].target_cAP = ini.GetInt("General", "AP", 0);
		Enemy[targ].target_APPenalty = 0;
		Enemy[targ].target_weapon = ini.GetString("General", "Weapon");

		LoadWeapon(Enemy[targ].target_weapon, targ);
		
		DeleteFile("data\\tempc");
	}
}

void CFight::LoadWeapon(CString name, int targ)
{
	CString tmp = "WEAPON/";
	tmp+=name;
	tmp+=".des";
	LPSTR fn;
	fn = tmp.GetBuffer(tmp.GetLength());
	tmp.ReleaseBuffer();

//	CD.SetFileDescription("Fallout Data File");
//	CD.Open("CRITTERS.DAT");

	if(CD.IsFileOpen())
	{
		CD.ExtractFile_ToFile(fn,"data\\tempw");

		CIni ini;
		ini.SetPathName("data\\tempw");

		// загрузим основные переменные
		Enemy[targ].target_DMGfrom = ini.GetInt("General", "From", 0);
		Enemy[targ].target_DMGto = ini.GetInt("General", "To", 0);
		Enemy[targ].target_DMGplus = ini.GetInt("General", "Plus", 0);
		Enemy[targ].target_requed_dist = ini.GetInt("General", "Distance", 0);

		DeleteFile("data\\tempw");
	}
}