#include "pch.h"
#include "CFloatNumberEdit.h"
BEGIN_MESSAGE_MAP(CFloatNumberEdit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()


void CFloatNumberEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (((nChar >= '0') && (nChar <= '9')) || (nChar == '.') || (nChar == 8) || (nChar == '-'))
    {
        if (nChar == '.')
        {
            char sz[512];
            this->GetWindowTextA(sz, sizeof(sz) / sizeof(sz[0]));
            if (strstr(sz, ".")) return;
        }
        CEdit::OnChar(nChar, nRepCnt, nFlags);
    }
}


BOOL CFloatNumberEdit::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_CHAR)
    {
        if (pMsg->wParam == ',') pMsg->wParam = '.';
    }

    return CEdit::PreTranslateMessage(pMsg);
}
