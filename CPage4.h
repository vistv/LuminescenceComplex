#pragma once


// CPage4 dialog

class CPage4 : public CDialogEx
{
	DECLARE_DYNAMIC(CPage4)

public:
	CPage4(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPage4();

// Dialog Data

	enum { IDD = IDD_DIALOG4 };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonApplylumsettings();
	float stepMot1Delay;
	float stepMot2Delay;
	double stepMotnmPerStep1;
	double stepMotnmPerStep2;
	CHAR   cfg_IniName[256];
	CFloatNumberEdit m_nmPerStepEditCtrl;
	CFloatNumberEdit m_StopAtEditCtrl;

	CFloatNumberEdit m_LLIM1;
	CFloatNumberEdit m_LLIM2;
	CFloatNumberEdit m_ULIM1;
	CFloatNumberEdit m_ULIM2;

	float t_LLIM1;
	float t_LLIM2;
	float t_ULIM1;
	float t_ULIM2;
};
