#include <NukeDX.h>
#include <NukeFile.h>
#include "INI.h"
#include "Barter.h"

//extern NDX_Screen Screen;

void CBarter::InitInventory()
{
	sc_y=0;
	cur_itm=-1;
	cost=0;
	koeff=1.75;
}

void CBarter::AddItem(int amount, CString desc, NDX_Screen * ParentScreen)
{
	if(FindItem(desc) == -1)
	{
		CItem* pItem = new CItem;
		
		CString tmp = desc;

		tmp+=".bmp";
		LPSTR fn;
		fn = tmp.GetBuffer(tmp.GetLength());
		tmp.ReleaseBuffer();

		CD.SetFileDescription("Fallout Data File");
		CD.Open("CRITTERS.DAT");

		if(CD.IsFileOpen())
		{
			pItem->img.LoadBMP(ParentScreen,NDXST_SYSMEM,CD.GetFile(fn),true);
			pItem->img.SetColorKey(ParentScreen->PixelFormat->Rgb(255,0,255));
			pItem->img.SetTransP(true);
			
			tmp = desc;
			
			tmp+=".des";
			fn = tmp.GetBuffer(tmp.GetLength());
			tmp.ReleaseBuffer();
			
			CD.ExtractFile_ToFile(fn,"data\\tempt");
			
			CIni ini;

			ini.SetPathName("data\\tempt");

			pItem->type = ini.GetInt("Main", "Type", 0);
			pItem->stype = ini.GetInt("Main", "SType", 0);
			pItem->amount = amount;
			pItem->i_cost = ini.GetInt("Main", "Cost", 0);
			pItem->name = ini.GetString("Main", "Title");
			pItem->i_desc = desc;

			DeleteFile("data\\tempt");
		}

		CD.Close();

		m_Inventory.Add(pItem);
	}
	else
		ModifyItem(amount, desc, ParentScreen);

	DrawBInventory(ParentScreen);
}

void CBarter::DeleteItem(CString desc, NDX_Screen * ParentScreen)
{
	if(FindItem(desc)!=-1)
		m_Inventory.RemoveAt(FindItem(desc));

	DrawBInventory(ParentScreen);
}

int CBarter::FindItem(CString desc)
{
	int res=-1;

	if(m_Inventory.GetSize()>0)
	{
		for(int i=0; i <m_Inventory.GetSize(); ++i)
		{
			if(GetIDesc(i) == desc)
			{
				res = i;
			}
		}
	}
	
	return res;
}

int CBarter::FindPareItem(DWORD st)
{
	int res=-1;

	if(m_Inventory.GetSize()>0)
	{
		for(int i=0; i <m_Inventory.GetSize(); ++i)
		{
			CItem* pItem = (CItem*) m_Inventory.GetAt(i);
			
			if((pItem->stype == st)&&(i != cur_itm))
			{
				res = i;
			}
		}
	}
	
	return res;
}

CString CBarter::GetIDesc(int i)
{
	CItem* pItem = (CItem*) m_Inventory.GetAt(i);

	return pItem->i_desc;
}

void CBarter::ModifyItem(int amount, CString desc, NDX_Screen * ParentScreen)
{
	CItem* pItem = (CItem*) m_Inventory.GetAt(FindItem(desc));
		
	pItem->amount += amount;
	m_Inventory.SetAt(FindItem(desc), pItem);
}

void CBarter::GetItem(int amount, CString desc, NDX_Screen * ParentScreen)
{
	CItem* pItem = (CItem*) m_Inventory.GetAt(FindItem(desc));
		
	int n_a = pItem->amount - amount;
	if(n_a <= 0)
		DeleteItem(desc, ParentScreen);
	else
	{
		pItem->amount = n_a;
		m_Inventory.SetAt(FindItem(desc), pItem);	
	}

	DrawBInventory(ParentScreen);
}

void CBarter::ReplaceItem(CString new_desc, NDX_Screen * ParentScreen, CString desc)
{
	if(FindItem(desc) != -1)
	{
		int index = FindItem(desc);
		CItem* pItem = (CItem*) m_Inventory.GetAt(index);
		
		CString tmp = new_desc;

		tmp+=".bmp";
		LPSTR fn;
		fn = tmp.GetBuffer(tmp.GetLength());
		tmp.ReleaseBuffer();

		CNukeFile CD;
		CD.SetFileDescription("Fallout Data File");
		CD.Open("CRITTERS.DAT");

		pItem->img.LoadBMP(ParentScreen,NDXST_SYSMEM,CD.GetFile(fn),true);
		pItem->img.SetColorKey(ParentScreen->PixelFormat->Rgb(255,0,255));
		pItem->img.SetTransP(true);

		tmp = new_desc;

		tmp+=".des";
		fn = tmp.GetBuffer(tmp.GetLength());
		tmp.ReleaseBuffer();

		CD.ExtractFile_ToFile(fn,"data\\tempt");

		CIni ini;

		ini.SetPathName("data\\tempt");

		pItem->type = ini.GetInt("Main", "Type", 0);
		pItem->name = ini.GetString("Main", "Title");
		pItem->i_desc = new_desc;

		CD.Close();

		DeleteFile("data\\tempt");

		m_Inventory.SetAt(index, pItem);
	}
}

void CBarter::ClearAll(CString s, CString r, NDX_Screen * ParentScreen)
{
	s_name = s;
	s_image = r;

	if(r!="")
	{
		r="SOURCE/";
		r+=s_image;
		r+=".bmp";
		LPSTR fn;
		fn = r.GetBuffer(r.GetLength());
		r.ReleaseBuffer();
		
		
		CD.SetFileDescription("Fallout Data File");
		CD.Open("CRITTERS.DAT");
		
		if(CD.IsFileOpen())
		{
			inv_source.LoadBMP(ParentScreen,NDXST_SYSMEM,CD.GetFile(fn),true);
			inv_source.SetColorKey(ParentScreen->PixelFormat->Rgb(255,0,255));
			inv_source.SetTransP(true);
		}
	}
	else
	{
		inv_source.Create(ParentScreen, NDXST_SYSMEM, 30, 30);
		inv_source.Clear(RGB(255, 0, 255));
		inv_source.SetColorKey(ParentScreen->PixelFormat->Rgb(255,0,255));
		inv_source.SetTransP(true);
	}

	m_Inventory.RemoveAll();

	DrawBInventory(ParentScreen);
}

void CBarter::DrawBInventory(NDX_Screen * ParentScreen)
{
	HFONT fnt=CreateFont(14, 0, 0, 0, FW_BOLD,
		FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, VARIABLE_PITCH, "Arial Cyr");

	inv_blist.Create(ParentScreen, NDXST_SYSMEM, i_brweight, i_brheight);
	inv_blist.Clear(RGB(0,0,0));
	inv_blist.SetColorKey(ParentScreen->PixelFormat->Rgb(0,0,0));
	inv_blist.SetTransP(true);

	if(m_Inventory.GetSize() > 0)
	{
		int size = m_Inventory.GetSize() * y_bsize;

		if(size<=i_brheight)
		{
			scrollable=false;
			size=i_brheight;
			sc_y=0;
		}
		else
			scrollable=true;

		inv_blist.Create(ParentScreen, NDXST_SYSMEM, i_brweight, size);
		inv_blist.Clear(RGB(0,0,0));
		inv_blist.SetColorKey(ParentScreen->PixelFormat->Rgb(0,0,0));
		inv_blist.SetTransP(true);

		for(int i=0; i<m_Inventory.GetSize(); ++i)
		{
			CItem* pItem = (CItem*) m_Inventory.GetAt(i);

			pItem->img.DrawRotoZoom(&inv_blist, i_brweight / 2, (i+1)*y_bsize-y_bsize/2, 0, 0.7);

			if(pItem->amount>1)
			{
				inv_blist.FormattedText(15, (i+1)*y_bsize-54, RGB(255,255,255), fnt, "x %u",pItem->amount);
			}
		}
	}

	GetCost();
}

CString CBarter::GetDescription()
{
	CItem* pItem = (CItem*) m_Inventory.GetAt(cur_itm);
	
	return pItem->i_desc;
}

CString CBarter::GetDescription(int i)
{
	CItem* pItem = (CItem*) m_Inventory.GetAt(i);
	
	return pItem->i_desc;
}

int CBarter::GetType()
{
	CItem* pItem = (CItem*) m_Inventory.GetAt(cur_itm);
	
	return pItem->type;
}

int CBarter::GetType(int i)
{
	CItem* pItem = (CItem*) m_Inventory.GetAt(i);
	
	return pItem->type;
}

void CBarter::GetCost()
{
	cost = 0;

	for(int i=0; i<m_Inventory.GetSize(); ++i)
	{
		CItem* pItem = (CItem*) m_Inventory.GetAt(i);
		if(pItem->type==INV_MONEY)
			cost += pItem->i_cost * pItem->amount;
		else
			cost += pItem->i_cost*pItem->amount*koeff;
	}
}

CString CBarter::GetFDescription()
{
	CItem* pItem = (CItem*) m_Inventory.GetAt(cur_itm);

	CString tmp="";

	CD.SetFileDescription("Fallout Data File");
	CD.Open("CRITTERS.DAT");
	
	if(CD.IsFileOpen())
	{
		tmp = pItem->i_desc;
		tmp+=".des";
		LPSTR fn;
		fn = tmp.GetBuffer(tmp.GetLength());
		tmp.ReleaseBuffer();

//		AfxMessageBox(fn);
		
		if(CD.ExtractFile_ToFile(fn,"data\\temp"))
		{
			CIni ini;
			ini.SetPathName(_T("data\\temp"));
//			AfxMessageBox(ini.GetPathName());
			tmp = ini.GetString("Main", "Title") + "\n";
			tmp+= " \n";
			tmp += ini.GetString("Main", "Description");
		}

		DeleteFile("data\\temp");

		CD.Close();
	}

	return tmp;
}

CString CBarter::GetSubstitute()
{
	CItem* pItem = (CItem*) m_Inventory.GetAt(cur_itm);

	CString tmp="";

	CD.SetFileDescription("Fallout Data File");
	CD.Open("CRITTERS.DAT");
	
	if(CD.IsFileOpen())
	{
		tmp = pItem->i_desc;
		tmp+=".des";
		LPSTR fn;
		fn = tmp.GetBuffer(tmp.GetLength());
		tmp.ReleaseBuffer();

//		AfxMessageBox(fn);
		
		if(CD.ExtractFile_ToFile(fn,"data\\temp"))
		{
			CIni ini;
			ini.SetPathName(_T("data\\temp"));
//			AfxMessageBox(ini.GetPathName());
			tmp = ini.GetString("Main", "Substitute");
		}

		DeleteFile("data\\temp");

		CD.Close();
	}

	return tmp;
}

int CBarter::GetAmount()
{
	CItem* pItem = (CItem*) m_Inventory.GetAt(cur_itm);
	
	return pItem->amount;
}

int CBarter::GetAmount(int i)
{
	CItem* pItem = (CItem*) m_Inventory.GetAt(i);
	
	return pItem->amount;
}

int CBarter::GetObjIndex(int x, int y)
{
	int res = -1;

	if(m_Inventory.GetSize()>0)
	{
		res = (y-1) / y_bsize + sc_y;

		if(res>=m_Inventory.GetSize())
			res=-1;
	}

	return res;
}

void CBarter::DrawMoveObj(NDX_Surface * dest, int x, int y)
{
	if(cur_itm!=-1)
	{
		CItem* pItem = (CItem*) m_Inventory.GetAt(cur_itm);

		pItem->img.DrawRotoZoom(dest, x, y, 0, 1);
	}
}

void CBarter::DrawItem(NDX_Surface * dest, int x, int y)
{
	if(cur_itm!=-1)
	{
		CItem* pItem = (CItem*) m_Inventory.GetAt(cur_itm);

		float zoom = 94 / pItem->img.Width;
		if(zoom> 64 / pItem->img.Height)
			zoom = 64 / pItem->img.Height;

		pItem->img.DrawRotoZoom(dest, x, y, 0, zoom-0.1);
	}
}

void CBarter::DrawDescription(NDX_Screen * ParentScreen)
{
	inv_desc.Create(ParentScreen, NDXST_SYSMEM, 155, 210);
	inv_desc.Clear(RGB(0,245,0));
	inv_desc.FillRect(1,1,154,209, RGB(0,0,0));
	inv_desc.FillRect(5, 17, 150, 18, ParentScreen->PixelFormat->Rgb(255, 255, 0));

	WriteDescText(GetFDescription());
}

void CBarter::WriteDescText(CString str)
{
	int i = 0;
	int sn=0;
	int rn=0;
	int space=0;
	int lsp = 0;

	HFONT fnt=CreateFont(14, 0, 0, 0, FW_BOLD,
		FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, VARIABLE_PITCH, "Arial Cyr");

	i = str.GetLength();

	for(int z=0; z<i; ++z)
	{
		if(str.GetAt(z)==' ')
		{
			space=z;
		}
				
		if(sn*7>150)
		{
			sn=0;
			z=space+1;
			DrawDescString(str, lsp,space,rn, fnt);
			lsp=space+1;	
			++rn;
		}
		else if(str.GetAt(z)=='\n')
		{
			sn=0;
			DrawDescString(str, lsp, z-1, rn, fnt);
			z=z+1;
			lsp=z;
			++rn;
		}
		else if(z==i-1)
			DrawDescString(str, lsp, z, rn, fnt);

		++sn;
	}
}

void CBarter::DrawDescString(CString desc_s, int lsp, int space, int rn, HFONT fnt)
{
	CString str;
	LPSTR dstr;
	for(int i=lsp; i<=space; ++i)
	{
		str+=desc_s.GetAt(i);
	}

	int l=str.GetLength();

	dstr = str.GetBuffer(l);
	str.ReleaseBuffer();

	inv_desc.FormattedText(5,3+rn*12,RGB(255, 255, 0),fnt,dstr);
//	AfxMessageBox(str);
}

bool CBarter::Presence(CString desc)
{
	if(FindItem(desc)!=-1)
		return true;
	else
		return false;
}

void CBarter::UseMiscItem(NDX_Screen * ParentScreen)
{
	CItem* pItem = (CItem*) m_Inventory.GetAt(cur_itm);

	if(pItem->stype!=0)
	{
		int pare = FindPareItem(pItem->stype);

		if(pare!=-1)
		{
			CString p_desc = GetDescription(pare);
			CString c_desc = GetDescription();
			CString new_i = p_desc;
			new_i += GetSubstitute();

			GetItem(1, c_desc, ParentScreen);
			GetItem(1, p_desc, ParentScreen);

			AddItem(1, new_i, ParentScreen);

//			DrawInventory(ParentScreen);
		}
	}
}
