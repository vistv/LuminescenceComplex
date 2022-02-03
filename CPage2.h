#pragma once


// CPage2 dialog

class CPage2 : public CDialog
{
	DECLARE_DYNAMIC(CPage2)

public:
	CPage2(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPage2();

// Dialog Data

	enum { IDD = IDD_DIALOG2 };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL isSoftwareHistogram;
	afx_msg void OnBnClickedRadiohardhist();
	afx_msg void OnBnClickedRadiosofthist();
	//BOOL isYScaleLog;
	afx_msg void OnBnClickedCheck1();
	CComboBox m_MaxCount;
	CString m_selectedMaxInt;
	virtual BOOL OnInitDialog();
	CComboBox m_comboChanNum;
	
	BOOL isMaxTime;
	UINT maxTime;
	afx_msg void OnBnClickedRadioIntens();
	afx_msg void OnBnClickedRadioTime();
	CEdit m_MaxTime;
	virtual void OnOK();
	
	afx_msg void OnCbnSelchangeCombonumchann();
	int m_numChan;
	CFloatNumberEdit m_BeginEditCtrl;
	CFloatNumberEdit m_EndEditCtrl;
	afx_msg void OnBnClickedButton1();
	double m_beginCalc;
	double m_endCalc;
	CDialog* pParentDialog;
	double m_calcRes;
};
