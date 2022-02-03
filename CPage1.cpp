// CPage1.cpp : implementation file
//

#include "pch.h"
#include "LuminescenceComplex.h"
#include "CPage1.h"
#include "afxdialogex.h"


// CPage1 dialog

IMPLEMENT_DYNAMIC(CPage1, CDialog)

CPage1::CPage1(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
	, m_stepNm(2)
	, m_endNm(800)
	, m_lumIntegrTime(0)
{

}

CPage1::~CPage1()
{
}

void CPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_step, m_stepNm);
	DDX_Text(pDX, IDC_EDIT_stopat, m_endNm);
	DDX_Text(pDX, IDC_EDIT_timeCount, m_lumIntegrTime);
	DDX_Control(pDX, IDC_EDIT_step, m_StepEditCtrl);
	DDX_Control(pDX, IDC_EDIT_stopat, m_StopEditCtrl);
	DDX_Control(pDX, IDC_EDIT_timeCount, m_TimeEditCtrl);
}


BEGIN_MESSAGE_MAP(CPage1, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ApplyLumSettings, &CPage1::OnBnClickedButtonApplylumsettings)
	ON_COMMAND(IDOK, &CPage1::OnIdok)
END_MESSAGE_MAP()


// CPage1 message handlers


void CPage1::OnBnClickedButtonApplylumsettings()
{
	UpdateData(true);
	
}


void CPage1::OnIdok()
{
	// TODO: Add your command handler code here
}


void CPage1::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	
}
