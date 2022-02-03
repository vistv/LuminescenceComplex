// CPage4.cpp : implementation file
//

#include "pch.h"
#include "LuminescenceComplex.h"
#include "CPage4.h"
#include "afxdialogex.h"


// CPage4 dialog

IMPLEMENT_DYNAMIC(CPage4, CDialogEx)

CPage4::CPage4(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
	, stepMotnmPerStep1(0)
	, stepMotnmPerStep2(0)
{

}

CPage4::~CPage4()
{
}

void CPage4::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_step, stepMot1Delay);
	DDX_Text(pDX, IDC_EDIT_stopat, stepMot2Delay);
	DDX_Text(pDX, IDC_EDIT_nmPerStep1, stepMotnmPerStep1);
	DDX_Text(pDX, IDC_EDIT_stopat2, stepMotnmPerStep2);
	DDX_Control(pDX, IDC_EDIT_nmPerStep1, m_nmPerStepEditCtrl);
	DDX_Control(pDX, IDC_EDIT_stopat2, m_StopAtEditCtrl);

	DDX_Control(pDX, IDC_EDIT_LLim1, m_LLIM1);
	DDX_Control(pDX, IDC_EDIT_LLim2, m_LLIM2);
	DDX_Control(pDX, IDC_EDIT_ULim1, m_ULIM1);
	DDX_Control(pDX, IDC_EDIT_ULim2, m_ULIM2);

	DDX_Text(pDX, IDC_EDIT_LLim1, t_LLIM1);
	DDX_Text(pDX, IDC_EDIT_LLim2, t_LLIM2);
	DDX_Text(pDX, IDC_EDIT_ULim1, t_ULIM1);
	DDX_Text(pDX, IDC_EDIT_ULim2, t_ULIM2);

}


BEGIN_MESSAGE_MAP(CPage4, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ApplyLumSettings, &CPage4::OnBnClickedButtonApplylumsettings)
END_MESSAGE_MAP()


void CPage4::OnBnClickedButtonApplylumsettings()
{
	CHAR temp[64] = "";
	UpdateData(true);

	GetCurrentDirectory(MAX_PATH, cfg_IniName);
	strcat_s(cfg_IniName, "\\LuminescenceComplex.ini");

	
	sprintf_s(temp, "%1.1f", stepMot1Delay);
	WritePrivateProfileStringA("lambdas", "stepMotorDelay1", temp, cfg_IniName);

	sprintf_s(temp, "%1.1f", stepMot2Delay);
	WritePrivateProfileStringA("lambdas", "stepMotorDelay2", temp, cfg_IniName);

	sprintf_s(temp, "%1.9f", stepMotnmPerStep1);
	WritePrivateProfileStringA("lambdas", "nmPerStep1", temp, cfg_IniName);

	sprintf_s(temp, "%1.9f", stepMotnmPerStep2);
	WritePrivateProfileStringA("lambdas", "nmPerStep2", temp, cfg_IniName);

	sprintf_s(temp, "%4.1f", t_LLIM1);
	WritePrivateProfileStringA("lambdas", "LowerLimit1", temp, cfg_IniName);

	sprintf_s(temp, "%4.1f", t_LLIM2);
	WritePrivateProfileStringA("lambdas", "LowerLimit2", temp, cfg_IniName);

	sprintf_s(temp, "%4.1f", t_ULIM1);
	WritePrivateProfileStringA("lambdas", "UpperLimit1", temp, cfg_IniName);

	sprintf_s(temp, "%4.1f", t_ULIM2);
	WritePrivateProfileStringA("lambdas", "UpperLimit2", temp, cfg_IniName);
		
}
