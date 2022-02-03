#pragma once


// CPage3 dialog

class CPage3 : public CDialogEx
{
	DECLARE_DYNAMIC(CPage3)

public:
	CPage3(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPage3();

// Dialog Data

	enum { IDD = IDD_DIALOG3 };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic toBeRealized;
	virtual BOOL OnInitDialog();
//	afx_msg void OnStnClickedStatictoberealized();
	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	afx_msg void OnBnClickedButton2();
	double m_FWstart;
	double m_FWstop;
	double m_SWstart;
	double m_SWstop;
	CFloatNumberEdit c_FWstart;
	CFloatNumberEdit c_FWstop;
	CFloatNumberEdit c_SWstart;
	CFloatNumberEdit c_SWstop;
};
