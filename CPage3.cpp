// CPage3.cpp : implementation file
//

#include "pch.h"
#include "LuminescenceComplex.h"
#include "CPage3.h"
#include "afxdialogex.h"


// CPage3 dialog

IMPLEMENT_DYNAMIC(CPage3, CDialogEx)

CPage3::CPage3(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
	, m_FWstart(0)
	, m_FWstop(0)
	, m_SWstart(0)
	, m_SWstop(0)
{
	
}

CPage3::~CPage3()
{
}

void CPage3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_STATIC_toBeRealized, toBeRealized);
	DDX_Text(pDX, IDC_EDIT_FWstart, m_FWstart);
	DDX_Text(pDX, IDC_EDIT_FWstop, m_FWstop);
	DDX_Text(pDX, IDC_EDIT_SWstart, m_SWstart);
	DDX_Text(pDX, IDC_EDIT_SWstopt, m_SWstop);
	DDX_Control(pDX, IDC_EDIT_FWstart, c_FWstart);
	DDX_Control(pDX, IDC_EDIT_FWstop, c_FWstop);
	DDX_Control(pDX, IDC_EDIT_SWstart, c_SWstart);
	DDX_Control(pDX, IDC_EDIT_SWstopt, c_SWstop);
}


BEGIN_MESSAGE_MAP(CPage3, CDialogEx)
	
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON2, &CPage3::OnBnClickedButton2)
END_MESSAGE_MAP()


// CPage3 message handlers


BOOL CPage3::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	

	//CFont f;
	//f.CreatePointFont(-300/*tenths of a point*/, "Arial");
	//GetDlgItem(IDC_STATIC_toBeRealized)->SetFont(&f);
	//f.Detach();


	
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}





HBRUSH CPage3::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_toBeRealized)
	{
		pDC->SetTextColor(RGB(100, 0, 0)); //pDC->SetBkMode(TRANSPARENT);
		//return m_Brush;
	}
	

	return hbr;
}


void CPage3::OnBnClickedButton2()
{
	UpdateData(true);
}
