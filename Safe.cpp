#include <NukeDX.h>
#include <NukeFile.h>
#include "INI.h"
#include "Safe.h"

void CSafe::InitSafe()
{
	Timer.StartNow();

	ReserveTime = 0;
	
	period = 90; // период времени на попытку 90 секунд
	tryes_s = 3; // макимум попыток

	timer = period; // установим таймер
	tryes = tryes_s; // количество попыток

	c1=0;
	c2=0;
	c3=0;
	c4=0;
	c5=0;
	c6=0;
	c7=0;

	cn1=false;
	cn2=false;
	cn3=false;
	cn4=false;
	cn5=false;
	cn6=false;
	cn7=false;

	cracked = false;
}

int CSafe::CheckTimer()
{
	timer = period - Timer.GetTime();

	if((timer==0)&&(tryes>0))
	{
		tryes--;
		if(tryes>0)
			timer=-1;
		else
			timer=-3;
		
	}
	
	if((timer==0)&&(tryes==0))
		timer=-3;

	return timer;
}

void CSafe::Restart() // рестартим в случае провала попытки
{
	Timer.StartNow();

	period = 90;
	timer = period; // установим таймер

	c1=0;
	c2=0;
	c3=0;
	c4=0;
	c5=0;
	c6=0;
	c7=0;

	cn1=false;
	cn2=false;
	cn3=false;
	cn4=false;
	cn5=false;
	cn6=false;
	cn7=false;

	cracked = false;

}

bool CSafe::Code1()
{
	if(c1 != 7)
	{
		tryes--;
//		Restart();
	}
	else
	{
		cn1=true;
		cn2=true;
	}

	return cn2;
}

bool CSafe::Code2()
{
	if(cn2)
	{
		if(c2<=6)
		{
			tryes--;
//			Restart();
		}
		else
			cn3=true;
	}

	return cn3;
}

bool CSafe::Code3()
{
	if(cn3)
	{
	int r = c1 + c2;
	int r2=10;
	while (r2>=10)
	{
		if(r>=10)
		{
			r2=r/10;
			r=r%10;
			r2+=r;
			r=r2;
		}
		else
			r2=r;
	}

	if(c3!=r2)
	{
		tryes--;
//		Restart();
	}
	else
		cn4=true;
	}

	return cn4;
}

bool CSafe::Code4()
{
	if(cn4)
	{
	int r = c1 + c2 + c3;
	int r2=10;
	while (r2>=10)
	{
		if(r>=10)
		{
			r2=r/10;
			r=r%10;
			r2+=r;
			r=r2;
		}
		else
			r2=r;
	}

	if(c4!=r2)
	{
		tryes--;
//		Restart();
	}
	else
		cn5=true;
	}

	return cn5;
}

bool CSafe::Code5()
{
	if(cn5)
	{
	int r = c1 + c2 + c3 + c4;
	int r2=10;
	while (r2>=10)
	{
		if(r>=10)
		{
			r2=r/10;
			r=r%10;
			r2+=r;
			r=r2;
		}
		else
			r2=r;
	}

	if(c5!=r2)
	{
		tryes--;
//		Restart();
	}
	else
		cn6=true;
	}

	return cn6;
}

bool CSafe::Code6()
{
	if(cn6)
	{
	int r = c1 + c2 + c3 + c4 + c5;
	int r2=10;
	while (r2>=10)
	{
		if(r>=10)
		{
			r2=r/10;
			r=r%10;
			r2+=r;
			r=r2;
		}
		else
			r2=r;
	}

	if(c6!=r2)
	{
		tryes--;
//		Restart();
	}
	else
		cn7=true;
	}

	return cn7;
}

bool CSafe::Code7()
{
	if(cn7)
	{
	int r = c1 + c2 + c3 + c4 + c5 + c6;
	int r2=10;
	while (r2>=10)
	{
		if(r>=10)
		{
			r2=r/10;
			r=r%10;
			r2+=r;
			r=r2;
		}
		else
			r2=r;
	}

	if(c7!=r2)
	{
		tryes--;
//		Restart();
	}
	else
		cracked=true;
	}

	return cracked;
}