///////////////////////////////////////////////////////////////
//  вестлог
//
// Ѕудем хранить данные о квестах в св€занном списке
///////////////////////////////////////////////////////////////


//CPtrArray	Quests;

struct CQuest
{
	CString qtitle;
	int qstatus;

};


/*
int FindQuest(CString s)
{
	int res = -1;

	if(Quests.GetSize()>0)
	{
		for(int i=0; i<Quests.GetSize(); ++i)
		{
			CQuest* pQuest = (CQuest*) Quests.GetAt(i);
			if(pQuest->qtitle==s)
				res = i;
		}
	}
	
	return res;
}

void ModifyQuest(CString title, int status)
{
	int i =FindQuest(title);
	if(i<Quests.GetSize())
	{
		CQuest* pQuest = (CQuest*) Quests.GetAt(i);

		pQuest->qstatus=status;

		Quests.SetAt(i, pQuest);
	}
}

void AddQuest(CString title, int status)
{
	if(FindQuest(title)==-1)
	{
		CQuest* pQuest = new CQuest;
		pQuest->qtitle = title;
		pQuest->qstatus = status;
		
		Quests.Add(pQuest);
	}
	else
		ModifyQuest(title, status);
}

void DeleteQuest(CString title)
{
/*	int i=0;
	while(Quests[i].qtitle!=title)
	{
		++i;
	}
	Quests[i].qstatus=0;
	Quests[i].qtitle="";*/

/*	int i = Quests.GetSize();
	CQuest* pQuest = (CQuest*) Quests.GetAt(i);

	while((pQuest->qtitle!=title)&&(i>0))
	{
		--i;
	}

	Quests.RemoveAt(i);
}

void RealiseQuest()
{
	Quests.RemoveAll();
}

int GetQStatus(int i)
{
	CQuest* pQuest = (CQuest*) Quests.GetAt(i);

	return pQuest->qstatus;
}

CString GetQTitle(int i)
{
	CQuest* pQuest = (CQuest*) Quests.GetAt(i);

	return pQuest->qtitle;
}*/


///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////