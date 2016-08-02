///////////////////////////////////////////////////////////////
// Глобальные переменные, описывающие игру и доступные из скрипта
//
// Будем хранить данные о них в связанном списке
///////////////////////////////////////////////////////////////


CPtrArray	QData;

struct CDatas
{
	float value; //значение переменной
	CString vname; //название переменной
};


float GetQDValue(int i)
{
	CDatas* pD = (CDatas*) QData.GetAt(i);
	
	return pD->value;
}

CString GetQDName(int i)
{
	CDatas* pD = (CDatas*) QData.GetAt(i);

	return pD->vname;
}

bool FindValue(CString name)
{
	bool res=false;

	if(QData.GetSize()>0)
	{
		for(int i=0; i <QData.GetSize(); ++i)
		{
			if(GetQDName(i)==name)
				res = true;
		}
	}

	return res;
}

void AddData(CString name, float value)
{
	CDatas* pD = new CDatas;
	pD->value = value;
	pD->vname = name;
		
	QData.Add(pD);
}

void ModifyData(CString name, float value)
{
	if(FindValue(name))
	{
		for(int i=0; i<QData.GetSize(); ++i)
		{
			if(GetQDName(i) == name)
			{
				CDatas* pD = new CDatas;
				pD->value = value;
				pD->vname = name;
				
				QData.SetAt(i, pD);
			}
		}
	}
	else
		AddData(name, value);
}

float GetData(CString name)
{
	float res = 0;

	if(FindValue(name))
	{
		for(int i=0; i<QData.GetSize(); ++i)
		{
			if(GetQDName(i) == name)
			{
				CDatas* pD = (CDatas*) QData.GetAt(i);
				res = pD->value;
			}
		}
	}

	return res;
}

void DeleteData(CString name)
{
	if(FindValue(name))
	{
		for(int i=0; i<QData.GetSize(); ++i)
		{
			if(GetQDName(i)==name)
				QData.RemoveAt(i);
		}
	}	
}

void RealiseQData()
{
	QData.RemoveAll();
}



///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////