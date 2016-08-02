#if !defined(AFX_ARACRYPT_H__66035FB9_8319_11D2_85E6_004005FFF9AF__INCLUDED_)
#define AFX_ARACRYPT_H__66035FB9_8319_11D2_85E6_004005FFF9AF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ARACrypt.h : header file
//
// Note: A Special Thanks to Mr. Warren Ward for his Sept. 1998 CUJ article:
// "Stream Encryption" Copyright (c) 1998 by Warren Ward
/////////////////////////////////////////////////////////////////////////////
// ARACrypt window

class ARACrypt
{
// Construction
public:
	ARACrypt();

// Attributes
public:

// Operations
public:
	virtual void TransformString(LPCTSTR csKey, CString& csTarget);

protected:
	virtual void SetKey(LPCTSTR csKey);
	virtual void GetKey(CString& csKey);
	virtual void TransformChar(unsigned char& csTarget);

// Implementation
public:
	virtual ~ARACrypt();

// Attributes
protected:

	CString m_csKey;

    unsigned long           m_LFSR_A;
    unsigned long           m_LFSR_B;
    unsigned long           m_LFSR_C;
    const unsigned long     m_Mask_A;
    const unsigned long     m_Mask_B;
    const unsigned long     m_Mask_C;
    const unsigned long     m_Rot0_A;
    const unsigned long     m_Rot0_B;
    const unsigned long     m_Rot0_C;
    const unsigned long     m_Rot1_A;
    const unsigned long     m_Rot1_B;
    const unsigned long     m_Rot1_C;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ARACRYPT_H__66035FB9_8319_11D2_85E6_004005FFF9AF__INCLUDED_)


