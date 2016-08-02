// Ѕудем хранить данные о квестах в св€занном списке

struct Quest
{
	CString qtitle;
	int qstatus;

} Quests[20];

int FindEmpty()
{
	int i=0;

	while((!Quests[i].qtitle.IsEmpty())&&(i<20))
		++i;

	return i;
}

void AddQuest(CString title, int status)
{
	int i=0;
	int z=-1;

	for(i=0; i<20; ++i)
	{
		if(Quests[i].qtitle==title)
			z=i;
	}

	if(z==-1)
	{
		z=FindEmpty();
	}

	Quests[z].qtitle=title;
	Quests[z].qstatus=status;
}

void DeleteQuest(CString title)
{
	int i=0;
	while(Quests[i].qtitle!=title)
	{
		++i;
	}
	Quests[i].qstatus=0;
	Quests[i].qtitle="";
}
