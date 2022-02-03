// CPage2.cpp : implementation file
//

#include "pch.h"
#include "LuminescenceComplex.h"
#include "CPage2.h"
#include "afxdialogex.h"
#include "LuminescenceComplexDlg.h"
// CPage2 dialog

IMPLEMENT_DYNAMIC(CPage2, CDialog)

CPage2::CPage2(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG2, pParent)
	, isSoftwareHistogram(FALSE)
	, m_selectedMaxInt(_T(""))
	, isMaxTime(FALSE)
	, maxTime(600)
	, m_numChan(0)
	, m_beginCalc(0)
	, m_endCalc(0)
	, m_calcRes(0)
{

}

CPage2::~CPage2()
{
}

void CPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_hardHist, isSoftwareHistogram);

	DDX_Control(pDX, IDC_COMBO_stopAt, m_MaxCount);
	DDX_CBString(pDX, IDC_COMBO_stopAt, m_selectedMaxInt);
	DDX_Control(pDX, IDC_COMBO_numChann, m_comboChanNum);

	DDX_Radio(pDX, IDC_RADIO1, isMaxTime);
	DDX_Text(pDX, IDC_EDIT1, maxTime);
	DDX_Control(pDX, IDC_EDIT1, m_MaxTime);
	DDX_CBIndex(pDX, IDC_COMBO_numChann, m_numChan);
	DDX_Control(pDX, IDC_EDIT_beginCalc, m_BeginEditCtrl);
	DDX_Control(pDX, IDC_EDIT_endCalc, m_EndEditCtrl);
	DDX_Text(pDX, IDC_EDIT_beginCalc, m_beginCalc);
	DDX_Text(pDX, IDC_EDIT_endCalc, m_endCalc);
	DDX_Text(pDX, IDC_EDIT_resCalc, m_calcRes);
}


BEGIN_MESSAGE_MAP(CPage2, CDialog)
	ON_BN_CLICKED(IDC_RADIO_hardHist, &CPage2::OnBnClickedRadiohardhist)
	ON_BN_CLICKED(IDC_RADIO_softHist, &CPage2::OnBnClickedRadiosofthist)
	
	ON_BN_CLICKED(IDC_RADIO1, &CPage2::OnBnClickedRadioIntens)
	ON_BN_CLICKED(IDC_RADIO2, &CPage2::OnBnClickedRadioTime)
	ON_BN_CLICKED(IDC_BUTTON_setMaxTime, &CPage2::OnOK)
	ON_CBN_SELCHANGE(IDC_COMBO_numChann, &CPage2::OnCbnSelchangeCombonumchann)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage2::OnBnClickedButton1)
END_MESSAGE_MAP()


// CPage2 message handlers


void CPage2::OnBnClickedRadiohardhist()
{
	isSoftwareHistogram = false;
	m_comboChanNum.EnableWindow(false);
}

void CPage2::OnBnClickedRadiosofthist()
{
	isSoftwareHistogram = true;
	m_comboChanNum.EnableWindow(true);
}

void CPage2::OnBnClickedCheck1()
{
	UpdateData(true);
}


BOOL CPage2::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_MaxCount.SetCurSel(4);
	m_comboChanNum.EnableWindow(false);
	
	m_MaxTime.EnableWindow(false);
	m_comboChanNum.SetCurSel(1);

	
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CPage2::OnBnClickedRadioIntens()
{
	isMaxTime = false;
	m_MaxTime.EnableWindow(false);
	m_MaxCount.EnableWindow(true);
}


void CPage2::OnBnClickedRadioTime()
{
	isMaxTime = true;
	m_MaxTime.EnableWindow(true);
	m_MaxCount.EnableWindow(false);
}


void CPage2::OnOK()
{
	UpdateData(true);
}


void CPage2::OnCbnSelchangeCombonumchann()
{
	UpdateData(true);
}


void CPage2::OnBnClickedButton1()
{
	UpdateData(true);
	CLuminescenceComplexDlg* pParentDialogReal = (CLuminescenceComplexDlg*) pParentDialog;
	pParentDialogReal->CalculateDecayConstant();
}
