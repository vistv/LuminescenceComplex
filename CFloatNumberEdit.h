#pragma once
#include <afxwin.h>
class CFloatNumberEdit :
	public CEdit
{
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

