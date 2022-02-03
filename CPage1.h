#pragma once


// CPage1 dialog

class CPage1 : public CDialog
{
	DECLARE_DYNAMIC(CPage1)

public:
	CPage1(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPage1();

// Dialog Data
//#ifdef AFX_DESIGN_TIME
//	enum { IDD = IDD_DIALOG1 };
//#endif
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnBnClickedButtonApplylumsettings();
	double m_stepNm;
	double m_endNm;
	float m_lumIntegrTime;
	CFloatNumberEdit m_StepEditCtrl;
	CFloatNumberEdit m_StopEditCtrl;
	CFloatNumberEdit m_TimeEditCtrl;
	afx_msg void OnIdok();
	virtual void OnOK();
};
