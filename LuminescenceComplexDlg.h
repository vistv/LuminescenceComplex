
// LuminescenceComplexDlg.h : header file
//

#pragma once

#include <CSLScope.h>
#include "CPage1.h"
#include "CPage2.h"
#include "CPage3.h"
#include "CPage4.h"


#define cm_measKinHard 2
#define cm_measKinSoft 3
#define cm_measLum 1
#define cm_measComb 4

#define IDT_timerForVisualisation 1
// CLuminescenceComplexDlg dialog
class CLuminescenceComplexDlg : public CDialogEx
{
// Construction
public:
	CLuminescenceComplexDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LUMINESCENCECOMPLEX_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	CTSLScope Scope;
	int Counter;
	CHAR   cfg_IniName[512];
	CWnd* pWnd;
	char tempSt64[64];
// Implementation
protected:
	image m_Image;
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	CWinThread* m_motorMovingThread;
	CWinThread* m_MeasuringThread;
	CWinThread* m_IdleMeasuringThread;

public:
	CStatic m_Scope;
	afx_msg void OnBnClickedOk();
	CTabCtrl m_ctrTab;
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	UINT StartsInt;
	UINT StopsInt;
	UINT StartsCorInt;
	UINT StopsCorInt;
	
	CEdit  lambda1ctrl;
	CEdit  lambda2ctrl;
	CEdit  lambda3ctrl;
	
	double lamb1;
	double lamb2;
	double lamb3;
	double old_lamb1;
	double old_lamb2;
	double old_lamb3;
	CPage1* pPage1;
	CPage2* pPage2;
	CPage3* pPage3;
	CPage4* pPage4;
	afx_msg void OnClose();
	CWinUsbPic devPic;
	CWinUsbNanoFluor devNanoFluor;
	bool isPicConnected;
	CString statusString;
	bool isNFConnected;
	UINT_PTR timerForVisualisation;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CComboBox timeWindCombo;
	byte currentRange;
	afx_msg void OnCbnSelchangeCombotimewindows();
	int whichMonoEnabled;
	
	afx_msg void OnBnClickedRadioMonochr1();
	afx_msg void OnBnClickedRadioMonochr2();
	afx_msg void OnBnClickedButtonSet();
	CString editBoxSetWave;
	CString editBoxGoWave;
	afx_msg void OnBnClickedButtonStepup();
	afx_msg void OnBnClickedButtonStepdown();
	CString statusString2;
	afx_msg void OnBnClickedButtonGo();
	static UINT MotorMovingThread(LPVOID pParam);
	static UINT IdlingIntensityMeasurementThread(LPVOID pParam);
	float lmb1;
	CString tempString;
	bool isMotorMoving;
	CButton m_GoButt;
	void MeasureKineticHard(bool isForCombo = false);
	void MeasureKineticSoft(bool isForCombo = false);
	void MeasureLuminescence();
	void MeasureCombo();
	bool m_StopThread;
	bool is_process_running;
	CTime startTime;
	CTime stopTime;
	DWORD CurrentMeasuredMax;
	DWORD CurrentMeasuredMaxForComboLum;
	
	long timeOfMeas;
	static UINT MeasureThread(LPVOID pParam);
	afx_msg void OnBnClickedButtonStart();
	void StartMeasurement();

	CButton m_buttonStart;
	afx_msg void OnBnClickedButtonstop();
	afx_msg void OnBnClickedButtoncontinue();
	bool isContinued;
	CButton m_ContButt;
	CButton m_StopButt;
	void DeactivateControls();
	void ActivateControls();
	BOOL isYScaleLog;
	afx_msg void OnBnClickedCheckLogyscale();
	afx_msg void OnBnClickedSave();
	CString FileName;
	CString cinaKanala;
	
	double MainXArray[1024];
	double MainYArray[1024];

	double MainXArray16[16384];
	double MainYArray16[16384];


	CArray <double,double> LumX_Array;
	CArray <double, double> LumY1_Array;
	CArray <double, double> LumY2_Array;
	CArray <double, double> LumY3_Array;

	//double ComboHardXArray[1024];
	CPtrArray ComboYArray; // Спільний і для хард і для софт

	//double ComboSoftXArray[16384];
	

	float ArrOfChaDiv[32];

	byte whatWeMeasure;

	void SaveKineticHard(bool isTempFile = false);
	void SaveKineticSoft(bool isTempFile = false);
	void SaveLuminescence(bool isTempFile = false);
	void SaveCombo(bool isTempFile = false);

	int m_numChan;

	afx_msg void OnBnClickedButton2();
	CButton m_SetButt;
	CButton m_Mono1Butt;
	CButton m_Mono2Butt;
	CButton m_Mono3Butt;
	CEdit m_statusString2;

	CDataExchange* pDXcopy;
	CMFCButton m_saveButt;
	
	void SaveTempFile();
	CFloatNumberEdit m_setEditCtrl;
	CFloatNumberEdit m_goEditCtrl;
	bool isIdleMeasuring;
	void StartIdleMeas();
	void StopIdleMeas();

	UINT zalyshTime;
	afx_msg void OnBnClickedRadioShowdec();
	BOOL isPlotLuminForCombo;
	afx_msg void OnBnClickedRadioShowlum();
	afx_msg void OnBnClickedButton1();
	CButton c_ComboDec;
	CButton c_ComboLum;

	void CalculateDecayConstant();
	bool isStopIdle;
	afx_msg void OnBnClickedAndigst();
	afx_msg void OnBnClickedDigst2();
	afx_msg void OnBnClickedRadioMonochr3();
	int m_isARCgr500;
	afx_msg void OnBnClickedArcSetup();
};
