
// LuminescenceComplexDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "LuminescenceComplex.h"
#include "LuminescenceComplexDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLuminescenceComplexDlg dialog



CLuminescenceComplexDlg::CLuminescenceComplexDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LUMINESCENCECOMPLEX_DIALOG, pParent)
	, StartsInt(0)
	, StopsInt(0)
	, StartsCorInt(0)
	, StopsCorInt(0)
	, statusString(_T(""))
	, whichMonoEnabled(0)
	, editBoxSetWave(_T(""))
	, editBoxGoWave(_T(""))
	, statusString2(_T(""))
	, lmb1(0)
	, isMotorMoving(false)
	, isContinued(false)
	, isYScaleLog(FALSE)
	, FileName(_T(""))
	, cinaKanala(_T(""))
	, isIdleMeasuring(false)
	, old_lamb1(0)
	, old_lamb2(0)
	, isPlotLuminForCombo(FALSE)
	, isStopIdle(FALSE)
	, m_isARCgr500(0)
	, isARCGr300(true)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLuminescenceComplexDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_SCOPE, m_Scope);
	DDX_Control(pDX, IDC_TAB, m_ctrTab);
	DDX_Text(pDX, IDC_EDIT_StartInt, StartsInt);
	DDX_Text(pDX, IDC_EDIT_StopInt, StopsInt);
	DDX_Text(pDX, IDC_EDIT_CorrStartsInt, StartsCorInt);
	DDX_Text(pDX, IDC_EDIT_CorrStopsInt, StopsCorInt);
	//	DDX_Text(pDX, IDC_EDIT_Wavelength1, lamb1);
	//	DDX_Text(pDX, IDC_EDIT_Wavelength2, lamb2);
	DDX_Control(pDX, IDC_EDIT_Wavelength1, lambda1ctrl);
	DDX_Control(pDX, IDC_EDIT_Wavelength2, lambda2ctrl);
	DDX_Control(pDX, IDC_EDIT_Wavelength3, lambda3ctrl);
	DDX_Text(pDX, IDC_EDIT2, statusString);
	DDX_Control(pDX, IDC_COMBO_timeWindows, timeWindCombo);
	DDX_Radio(pDX, IDC_RADIO_Monochr1, whichMonoEnabled);
	//	DDX_Radio(pDX, IDC_RADIO_Monochr3, isMono3Enabled);
	DDX_Text(pDX, IDC_EDIT_SetWavelength, editBoxSetWave);
	DDX_Text(pDX, IDC_EDIT_GoWavelength, editBoxGoWave);
	DDX_Text(pDX, IDC_EDITstatus2, statusString2);

	DDX_Control(pDX, IDC_BUTTON_Go, m_GoButt);

	DDX_Control(pDX, IDC_BUTTON_Start, m_buttonStart);
	DDX_Control(pDX, IDC_BUTTON_continue, m_ContButt);
	DDX_Control(pDX, IDC_BUTTON_stop, m_StopButt);

	DDX_Text(pDX, IDC_EDIT1, FileName);
	DDX_Control(pDX, IDC_BUTTON_Set, m_SetButt);
	DDX_Control(pDX, IDC_RADIO_Monochr1, m_Mono1Butt);
	DDX_Control(pDX, IDC_RADIO_Monochr2, m_Mono2Butt);
	DDX_Control(pDX, IDC_RADIO_Monochr3, m_Mono3Butt);
	DDX_Control(pDX, IDC_RADIO_GR300, m_Gr300Butt);
	DDX_Control(pDX, IDC_RADIO_GR500, m_Gr500Butt);
	DDX_Control(pDX, IDC_EDITstatus2, m_statusString2);
	DDX_Control(pDX, IDSave, m_saveButt);
	DDX_Control(pDX, IDC_EDIT_SetWavelength, m_setEditCtrl);
	DDX_Control(pDX, IDC_EDIT_GoWavelength, m_goEditCtrl);
	DDX_Radio(pDX, IDC_RADIO_ShowDec, isPlotLuminForCombo);
	DDX_Control(pDX, IDC_RADIO_ShowDec, c_ComboDec);
	DDX_Control(pDX, IDC_RADIO_ShowLum, c_ComboLum);
	DDX_Radio(pDX, IDC_RADIO_GR300, m_isARCgr500);

}

BEGIN_MESSAGE_MAP(CLuminescenceComplexDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CLuminescenceComplexDlg::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CLuminescenceComplexDlg::OnSelchangeTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, &CLuminescenceComplexDlg::OnSelchangingTab)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_timeWindows, &CLuminescenceComplexDlg::OnCbnSelchangeCombotimewindows)
	ON_BN_CLICKED(IDC_RADIO_Monochr1, &CLuminescenceComplexDlg::OnBnClickedRadioMonochr1)
	ON_BN_CLICKED(IDC_RADIO_Monochr2, &CLuminescenceComplexDlg::OnBnClickedRadioMonochr2)
	ON_BN_CLICKED(IDC_BUTTON_Set, &CLuminescenceComplexDlg::OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_StepUp, &CLuminescenceComplexDlg::OnBnClickedButtonStepup)
	ON_BN_CLICKED(IDC_BUTTON_StepDown, &CLuminescenceComplexDlg::OnBnClickedButtonStepdown)
	ON_BN_CLICKED(IDC_BUTTON_Go, &CLuminescenceComplexDlg::OnBnClickedButtonGo)
	
	ON_BN_CLICKED(IDC_BUTTON_Start, &CLuminescenceComplexDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_stop, &CLuminescenceComplexDlg::OnBnClickedButtonstop)
	ON_BN_CLICKED(IDC_BUTTON_continue, &CLuminescenceComplexDlg::OnBnClickedButtoncontinue)
	
	ON_BN_CLICKED(IDC_CHECK_LogYScale, &CLuminescenceComplexDlg::OnBnClickedCheckLogyscale)
	ON_BN_CLICKED(IDSave, &CLuminescenceComplexDlg::OnBnClickedSave)
	
		
	ON_BN_CLICKED(IDC_RADIO_ShowDec, &CLuminescenceComplexDlg::OnBnClickedRadioShowdec)
	ON_BN_CLICKED(IDC_RADIO_ShowLum, &CLuminescenceComplexDlg::OnBnClickedRadioShowlum)
	ON_BN_CLICKED(IDC_BUTTON1, &CLuminescenceComplexDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_AnDigSt, &CLuminescenceComplexDlg::OnBnClickedAndigst)
	ON_BN_CLICKED(IDC_DigSt2, &CLuminescenceComplexDlg::OnBnClickedDigst2)
	ON_BN_CLICKED(IDC_RADIO_Monochr3, &CLuminescenceComplexDlg::OnBnClickedRadioMonochr3)
	ON_BN_CLICKED(IDC_ARC_SETUP, &CLuminescenceComplexDlg::OnBnClickedArcSetup)
	ON_BN_CLICKED(IDC_RADIO_GR500, &CLuminescenceComplexDlg::OnBnClickedRadioGr500)
	ON_BN_CLICKED(IDC_RADIO_GR300, &CLuminescenceComplexDlg::OnBnClickedRadioGr300)
END_MESSAGE_MAP()

BOOL CLuminescenceComplexDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();



	CurrentMeasuredMaxForComboLum = 1;



	CRect lprect(718, 88, 43, 290);

	MapDialogRect(lprect);

	UINT a = lprect.top;

	m_Image.init("ic.bmp");
	if (m_Image.error())
	{
		// Щось пішло не так...
	}

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	//SCOPE
	{
		Scope.Open(m_Scope.m_hWnd);
		Scope.Channels.Add();
		Scope.Channels.Add();
		Scope.Channels[0].Name = "Integral";
		Scope.Channels[1].Name = "Fast";
		Scope.Channels[1].Name = "Slow";
		Scope.Legend.Visible = false;
		//Scope.ToolBar.Position.CX = 100;
		Scope.ToolBar.Position.CY = 45;
		Scope.YAxis.AxisLabel.Text = "Intensity arb. un.";
		Scope.XAxis.AxisLabel.Text = "Wavelength, nm";
		VCL_Loaded();
		Counter = 0;
	}
	//TAB_ITEMS
	{
		TC_ITEM TabItem;
		TabItem.mask = TCIF_TEXT;
		TabItem.pszText = "Spectra";
		m_ctrTab.InsertItem(0, &TabItem);
		TabItem.pszText = "Decay";
		m_ctrTab.InsertItem(1, &TabItem);
		TabItem.pszText = "Combo";
		m_ctrTab.InsertItem(2, &TabItem);
		TabItem.pszText = "Settings";
		m_ctrTab.InsertItem(3, &TabItem);

		pPage1 = new CPage1;
		TabItem.mask = TCIF_PARAM; 	TabItem.lParam = (LPARAM)pPage1;
		m_ctrTab.SetItem(0, &TabItem);
		pPage1->Create(CPage1::IDD, &m_ctrTab);
		pPage1->SetWindowPos(NULL, 7, 25, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		pPage1->ShowWindow(SW_SHOW);

		pPage2 = new CPage2;
		TabItem.mask = TCIF_PARAM; 	TabItem.lParam = (LPARAM)pPage2;
		m_ctrTab.SetItem(1, &TabItem);
		pPage2->Create(CPage2::IDD, &m_ctrTab);
		pPage2->SetWindowPos(NULL, 7, 25, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		pPage2->ShowWindow(SW_HIDE);
		pPage2->pParentDialog = this;

		pPage3 = new CPage3;
		TabItem.mask = TCIF_PARAM; TabItem.lParam = (LPARAM)pPage3;
		m_ctrTab.SetItem(2, &TabItem);
		pPage3->Create(CPage3::IDD, &m_ctrTab);
		pPage3->SetWindowPos(NULL, 7, 25, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		pPage3->ShowWindow(SW_HIDE);

		pPage4 = new CPage4;
		TabItem.mask = TCIF_PARAM; TabItem.lParam = (LPARAM)pPage4;
		m_ctrTab.SetItem(3, &TabItem);
		pPage4->Create(CPage4::IDD, &m_ctrTab);
		pPage4->SetWindowPos(NULL, 7, 25, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		pPage4->ShowWindow(SW_HIDE);
	}

//	bool b = m_saveButt.SubclassDlgItem(IDSave , this);
	
	m_saveButt.EnableWindowsTheming(FALSE);   
	//m_saveButt.SetTextColor(RGB(0, 0, 255));
	
	

	GetCurrentDirectory(MAX_PATH, cfg_IniName); 
	
	CString tempFolder = cfg_IniName; tempFolder += "\\temp";
	CreateDirectory(tempFolder, NULL);
	
	strcat_s(cfg_IniName, "\\LuminescenceComplex.ini");

	timerForVisualisation = SetTimer(IDT_timerForVisualisation, 500, NULL);

	

	CHAR temp[128] = "";
	//Init Lambdas
	{
	GetPrivateProfileStringA("ARC", "ComportNo", "COM3", temp, 32, cfg_IniName); ARCcomport = CString(temp);
	GetPrivateProfileStringA("lambdas", "Lambda1", "300.0", temp, 32, cfg_IniName); lamb1 = atof(temp);
	GetPrivateProfileStringA("lambdas", "Lambda2", "300.0", temp, 32, cfg_IniName); lamb2 = atof(temp);
	GetPrivateProfileStringA("lambdas", "Lambda3", "300.0", temp, 32, cfg_IniName); lamb3 = atof(temp);
	CFont f;
	f.CreatePointFont(-120/*tenths of a point*/, "Arial");
	GetDlgItem(IDC_EDIT_Wavelength1)->SetFont(&f);
	GetDlgItem(IDC_EDIT_Wavelength2)->SetFont(&f);
	GetDlgItem(IDC_EDIT_Wavelength3)->SetFont(&f);
	GetDlgItem(IDC_EDIT_StartInt)->SetFont(&f);
	GetDlgItem(IDC_EDIT_StopInt)->SetFont(&f);
	GetDlgItem(IDC_EDIT_CorrStartsInt)->SetFont(&f);
	GetDlgItem(IDC_EDIT_CorrStopsInt)->SetFont(&f);
	GetDlgItem(IDC_mdr2)->SetFont(&f);
	f.Detach();
	//lambda1str.Format("%.1f", lamb1); //(_T("%f"), myFloat);
	//lambda2str.Format("%.1f", lamb2); //(_T("%f"), myFloat);
	GetPrivateProfileStringA("lambdas", "stepMotorDelay1", "3", temp, 32, cfg_IniName);
	pPage4->stepMot1Delay = atof(temp);
	GetPrivateProfileStringA("lambdas", "stepMotorDelay2", "4", temp, 32, cfg_IniName);
	pPage4->stepMot2Delay = atof(temp);
	GetPrivateProfileStringA("lambdas", "nmPerStep1", "0.002884895", temp, 32, cfg_IniName);
	pPage4->stepMotnmPerStep1 = atof(temp);
	GetPrivateProfileStringA("lambdas", "nmPerStep2", "0.002884895", temp, 32, cfg_IniName);
	pPage4->stepMotnmPerStep2 = atof(temp);
	// ARC
	GetPrivateProfileStringA("lambdas", "ARCnmPerStep1", "0.031386430", temp, 32, cfg_IniName);
	arc_nmperstep1 = atof(temp);
	GetPrivateProfileStringA("lambdas", "ARCnmPerStep2", "0.031572700", temp, 32, cfg_IniName);
	arc_nmperstep2 = atof(temp);
	GetPrivateProfileStringA("lambdas", "ARC_a1", "-7320", temp, 32, cfg_IniName);
	arc_a1 = long(atof(temp));
	GetPrivateProfileStringA("lambdas", "ARC_a2", "280880", temp, 32, cfg_IniName);
	arc_a2 = long(atof(temp));




	GetPrivateProfileStringA("lambdas", "LowerLimit1", "200", temp, 32, cfg_IniName);
	pPage4->t_LLIM1 = atof(temp);

	GetPrivateProfileStringA("lambdas", "LowerLimit2", "200", temp, 32, cfg_IniName);
	pPage4->t_LLIM2 = atof(temp);

	GetPrivateProfileStringA("lambdas", "UpperLimit1", "800", temp, 32, cfg_IniName);
	pPage4->t_ULIM1 = atof(temp);

	GetPrivateProfileStringA("lambdas", "UpperLimit2", "800", temp, 32, cfg_IniName);
	pPage4->t_ULIM2 = atof(temp);

	pPage4->UpdateData(false);
}


	// Open devices
	{
		HRESULT hr = devPic.OpenDevice();
		if (hr == S_OK) isPicConnected = true; else isPicConnected = false;


		hr = devNanoFluor.OpenDevice();
		if (hr == S_OK) isNFConnected = true; else isNFConnected = false;

		devARC.SetComportAndConnect(ARCcomport);
		isARCConnected = devARC.IsConnected();

		CString whatsConnected = "Nothing connected";
		if (isPicConnected || isNFConnected || isARCConnected) whatsConnected = "Connected: ";
		if (isPicConnected) whatsConnected += "Pic ";
		if (isNFConnected) whatsConnected += "NanoFluor ";
		if (isARCConnected) whatsConnected += "ARC ";

		statusString = whatsConnected;
	}
	
	if (isNFConnected)
	{
		CStringArray rArr;
		devNanoFluor.GetRanges(&rArr);
		for (byte i = 0; i < rArr.GetSize(); i++)
		{
			GetPrivateProfileStringA("TimeSettings", rArr[i], "0.0", temp, 32, cfg_IniName);
			ArrOfChaDiv[i] =(float) atof(temp);
			if (rArr[i].GetLength() == 2) rArr[i] += "  "; if (rArr[i].GetLength() == 3) rArr[i] += " "; rArr[i] += "\t";
			rArr[i] += temp;
			timeWindCombo.AddString(rArr[i]);
		}

		if (rArr.GetSize() > 4)		{timeWindCombo.SetCurSel(4); devNanoFluor.SetRangeNumber(4); currentRange = 0;}
		else { timeWindCombo.SetCurSel(0); devNanoFluor.SetRangeNumber(0); currentRange = 0; }

		double cinkan = ArrOfChaDiv[timeWindCombo.GetCurSel()];
		pPage3->m_FWstop = 511 * cinkan; pPage3->m_SWstart = 512 * cinkan; pPage3->m_SWstop = 1023 * cinkan;
		pPage3->UpdateData(false);

	}
	else 
	{ 
		timeWindCombo.EnableWindow(0); 
		pPage2->EnableWindow(0); 
		pPage3->EnableWindow(0);
	}

	if (isARCConnected)
	{
		devARC.b1 = arc_nmperstep1;
		devARC.b2 = arc_nmperstep2;
		devARC.a1 = arc_a1;
		devARC.a2 = arc_a2;
	}
	else
	{
		m_Mono3Butt.EnableWindow(false);
		m_Gr300Butt.EnableWindow(false);
		m_Gr500Butt.EnableWindow(false);
	}

	if (isPicConnected) 
	{	
		byte delay, delayMks; 

		delay = (BYTE) truncf(pPage4->stepMot1Delay);
		delayMks = (BYTE)truncf((pPage4->stepMot1Delay - delay)*10);

		devPic.SetDelayForMotor1(delay, delayMks);

		delay = (BYTE)truncf(pPage4->stepMot2Delay);
		delayMks = (BYTE)truncf((pPage4->stepMot2Delay - delay) * 10);

		devPic.SetDelayForMotor2(delay, delayMks);
	}
	else 
	{
		if (!isARCConnected)
		{
			m_GoButt.EnableWindow(false);
			m_SetButt.EnableWindow(false);
			pPage3->EnableWindow(0);
			pPage1->EnableWindow(0);
		}
	}

	m_ContButt.EnableWindow(false);

	if (!isPicConnected && !isNFConnected && !isARCConnected)
	{
//		statusString = "No device is connected";
		pPage1->EnableWindow(0);
		pPage2->EnableWindow(0);
		pPage3->EnableWindow(0);
		DeactivateControls();
	}

	// Init ARC
	



//	int test = timeWindCombo.GetCount();
	GetPrivateProfileString("initial", "FileName", "01_Sample_Exc=400nm_Em=450nm_T=300K_I=10mkA_Fon=200", temp, 128, cfg_IniName);
	FileName = temp;
	m_StopButt.EnableWindow(false);

	GetPrivateProfileString("initial", "LumIntegrTime", "1", temp, 128, cfg_IniName);
	pPage1->m_lumIntegrTime = (float) atof(temp);
	pPage1->UpdateData(false);

	whatWeMeasure = 1;

	lambda2ctrl.EnableWindow(false);
	lambda3ctrl.EnableWindow(false);

	c_ComboDec.EnableWindow(false);
	c_ComboLum.EnableWindow(false);

	pWnd = GetDlgItem(IDC_mdr2);
	tempString.Format("%.1f", lamb1 *0.547+197.45);
	pWnd->SetWindowText(tempString);

	UpdateData(false);
	return TRUE;  
}

void CLuminescenceComplexDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		for (int i = 0; i < 20; i++)
		{
			m_Image.put(dc, 1436, 242+28*i, 85, 30, SRCCOPY);
		}
		CDialogEx::OnPaint();
	}
}
HCURSOR CLuminescenceComplexDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLuminescenceComplexDlg::OnBnClickedOk()
{
	//float SampleDataArray[1000];
	//int i;
	//for (i = 0; i < 1000; i++)
	//	SampleDataArray[i] = rand() - 16000;

	//Scope.Channels[0].Data.SetYData(SampleDataArray, 1000);

	//for (i = 0; i < 1000; i++)
	//	SampleDataArray[i] = (float)(Counter++ % 120) * 300 - 16000;

	//Scope.Channels[1].Data.SetYData(SampleDataArray, 1000);
}

void CLuminescenceComplexDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nTab = m_ctrTab.GetCurSel();
	
	if (nTab == 0) whatWeMeasure = cm_measLum;
	if (nTab == 1)
	{
		if (pPage2->isSoftwareHistogram) whatWeMeasure = cm_measKinSoft;
		else whatWeMeasure = cm_measKinHard;
	}
	if (nTab == 2)
	{
		whatWeMeasure = cm_measComb;
		c_ComboDec.EnableWindow(true);
		c_ComboLum.EnableWindow(true);
	}
	else
	{
		c_ComboDec.EnableWindow(false);
		c_ComboLum.EnableWindow(false);
	}



	TC_ITEM tci;
	tci.mask = TCIF_PARAM;
	m_ctrTab.GetItem(nTab, &tci);
	ASSERT(tci.lParam);
	CWnd* pWnd = (CWnd*)tci.lParam;
	pWnd->ShowWindow(SW_SHOW);
	*pResult = 0;
}
void CLuminescenceComplexDlg::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nTab = m_ctrTab.GetCurSel();
	TC_ITEM tci;
	tci.mask = TCIF_PARAM;
	m_ctrTab.GetItem(nTab, &tci);
	ASSERT(tci.lParam);
	CWnd* pWnd = (CWnd*)tci.lParam;
	pWnd->ShowWindow(SW_HIDE);
	*pResult = 0;
}


void CLuminescenceComplexDlg::OnClose()
{
	CString tst; 
	tst.Format("%.1f", lamb1);
	WritePrivateProfileString(_T("lambdas"), _T("Lambda1"), tst, cfg_IniName);
	tst.Format("%.1f", lamb2);
	WritePrivateProfileString(_T("lambdas"), _T("Lambda2"), tst, cfg_IniName);
	
	tst.Format("%.1f", pPage1->m_lumIntegrTime);
	WritePrivateProfileString(_T("initial"), _T("LumIntegrTime"), tst, cfg_IniName);
		
	WritePrivateProfileString(_T("initial"), _T("FileName"), FileName, cfg_IniName);


	
	WritePrivateProfileString(_T("ARC"), _T("ComportNo"), ARCcomport, cfg_IniName);
	//tst.Format("%.9f", arc_nmperstep1);
	//WritePrivateProfileString(_T("lambdas"), _T("ARCnmPerStep1"), tst, cfg_IniName);
	//tst.Format("%.9f", arc_nmperstep2);
	//WritePrivateProfileString(_T("lambdas"), _T("ARCnmPerStep2"), tst, cfg_IniName);
	//tst.Format("%.0f", arc_a1);
	//WritePrivateProfileString(_T("lambdas"), _T("ARC_a1"), tst, cfg_IniName);
	//tst.Format("%.0f", arc_a2);
	//WritePrivateProfileString(_T("lambdas"), _T("ARC_a2"), tst, cfg_IniName);





	//TCHAR protocolChar[32] = _T("drivers");
	//GetPrivateProfileStringW(_T("drivers"), _T("wave"), _T(""), protocolChar, 32, cfg_IniName);
	//WritePrivateProfileString(_T("drivers1"), _T("wave2"), protocolChar, cfg_IniName);
	KillTimer(IDT_timerForVisualisation);
	if (isPicConnected) devPic.CloseDevice();
	if (isNFConnected)  devNanoFluor.CloseDevice();
	CDialogEx::OnClose();
}


void CLuminescenceComplexDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == IDT_timerForVisualisation)
	{
		CDataExchange dx(this, false);
		DDX_Text(&dx, IDC_EDITstatus2, statusString2);

		if (!isMotorMoving)
		{
			if ((!is_process_running) && (m_IdleMeasuringThread == NULL) && (!isIdleMeasuring))
			{
				isIdleMeasuring = true;
				isStopIdle = false;
		//		AfxMessageBox("started");
				StartIdleMeas();
			}
		}
		
		
		
		pWnd = GetDlgItem(IDC_EDIT_StartInt);
		_itoa_s(StartsInt, tempSt64, 10); pWnd->SetWindowText(tempSt64);
		pWnd = GetDlgItem(IDC_EDIT_CorrStartsInt);
		_itoa_s(StartsCorInt, tempSt64, 10); pWnd->SetWindowText(tempSt64);
		pWnd = GetDlgItem(IDC_EDIT_StopInt);
		_itoa_s(StopsInt, tempSt64, 10); pWnd->SetWindowText(tempSt64);
		pWnd = GetDlgItem(IDC_EDIT_CorrStopsInt);
		_itoa_s(StopsCorInt, tempSt64, 10); pWnd->SetWindowText(tempSt64);
		


		if (old_lamb2 != lamb2)
		{
			pWnd = GetDlgItem(IDC_EDIT_Wavelength2);
			tempString.Format("%.2f", lamb2);
			pWnd->SetWindowText(tempString);
			old_lamb2 = lamb2;

			pWnd = GetDlgItem(IDC_mdr2);
			tempString.Format("%.1f", lamb2 *0.547+197.45);
			pWnd->SetWindowText(tempString);
		}
		if (old_lamb1 != lamb1)
		{
			pWnd = GetDlgItem(IDC_EDIT_Wavelength1);
			tempString.Format("%.2f", lamb1);
			pWnd->SetWindowText(tempString);
			old_lamb1 = lamb1;

			pWnd = GetDlgItem(IDC_mdr2);
			tempString.Format("%.1f", lamb1 *0.547+197.45);
			pWnd->SetWindowText(tempString);
		}
		if (old_lamb3 != lamb3)
		{
			pWnd = GetDlgItem(IDC_EDIT_Wavelength3);
			tempString.Format("%.2f", lamb3);
			pWnd->SetWindowText(tempString);
			old_lamb3 = lamb3;

		}

		//if (pPage2->isYScaleLog) Scope.YAxis.ScaleMode = smLogDecascale; // smLogarithmic;
		//else Scope.YAxis.ScaleMode = smLinear; // smLogarithmic;

		if (CurrentMeasuredMax && is_process_running)
		{
			
			switch (whatWeMeasure)
			{
			case cm_measKinHard:
				Scope.ZoomTo(0, MainXArray[1023], 0, CurrentMeasuredMax * 1.2, true);
				Scope.Hold = false;
				Scope.Channels[0].Data.SetXYData(MainXArray, MainYArray, 1024);
				break;
			case cm_measKinSoft:
				Scope.ZoomTo(0, MainXArray16[m_numChan-1], 0, CurrentMeasuredMax * 1.2, true);
				Scope.Hold = false;
				Scope.Channels[0].Data.SetXYData(MainXArray16, MainYArray16, m_numChan);
				break;
			case cm_measLum:
				if (LumX_Array.GetSize() > 1) Scope.ZoomTo(LumX_Array[0], LumX_Array[LumX_Array.GetSize()-1], 0, CurrentMeasuredMax * 1.2, true);
				Scope.Hold = false;
				Scope.Channels[0].Data.SetXYData(LumX_Array.GetData(), LumY1_Array.GetData(), LumX_Array.GetSize());
				Scope.Channels[1].Data.SetXYData(LumX_Array.GetData(), LumY2_Array.GetData(), LumX_Array.GetSize());
				break;
			case cm_measComb:
				if (!isPlotLuminForCombo)
				{
					Scope.Channels[1].Data.Clear();
					Scope.Channels[2].Data.Clear();
					if (!pPage2->isSoftwareHistogram)
					{
						Scope.ZoomTo(0, MainXArray[1023], 0, CurrentMeasuredMax * 1.2, true);
						Scope.Hold = false;
						Scope.Channels[0].Data.SetXYData(MainXArray, MainYArray, 1024);
					}
					else
					{
						Scope.ZoomTo(0, MainXArray16[m_numChan - 1], 0, CurrentMeasuredMax * 1.2, true);
						Scope.Hold = false;
						Scope.Channels[0].Data.SetXYData(MainXArray16, MainYArray16, m_numChan);
					}
				}
				else
				{
					if (LumX_Array.GetSize() > 1) 
						Scope.ZoomTo(LumX_Array[0], LumX_Array[LumX_Array.GetSize() - 1], 0, CurrentMeasuredMaxForComboLum * 1.2, true);
					Scope.Hold = false;
					if (LumX_Array.GetSize() > 1)
					{
						Scope.Channels[0].Data.SetXYData(LumX_Array.GetData(), LumY1_Array.GetData(), LumX_Array.GetSize());
						Scope.Channels[1].Data.SetXYData(LumX_Array.GetData(), LumY2_Array.GetData(), LumX_Array.GetSize());
						Scope.Channels[2].Data.SetXYData(LumX_Array.GetData(), LumY3_Array.GetData(), LumX_Array.GetSize());
					}
					
				}
				
				break;

				
			}
		}
		
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CLuminescenceComplexDlg::OnCbnSelchangeCombotimewindows()
{
	devNanoFluor.SetRangeNumber(timeWindCombo.GetCurSel());
}
void CLuminescenceComplexDlg::OnBnClickedRadioMonochr1()
{
 	whichMonoEnabled = 0;
	lambda3ctrl.EnableWindow(false);
	lambda2ctrl.EnableWindow(false);
	lambda1ctrl.EnableWindow(true);

	pWnd = GetDlgItem(IDC_mdr2);
	tempString.Format("%.1f", lamb1 *0.547+197.45);
	pWnd->SetWindowText(tempString);
	m_SetButt.SetWindowTextA("Set");
}
void CLuminescenceComplexDlg::OnBnClickedRadioMonochr2()
{
 	whichMonoEnabled = 1;
	lambda2ctrl.EnableWindow(true);
	lambda1ctrl.EnableWindow(false);
	lambda3ctrl.EnableWindow(false);

	pWnd = GetDlgItem(IDC_mdr2);
	tempString.Format("%.1f", lamb2 *0.547+197.45);
	pWnd->SetWindowText(tempString);
	m_SetButt.SetWindowTextA("Set");
}



void CLuminescenceComplexDlg::OnBnClickedRadioMonochr3()
{
 	whichMonoEnabled = 2;
	lambda2ctrl.EnableWindow(false);
	lambda1ctrl.EnableWindow(false);
	lambda3ctrl.EnableWindow(true);
	m_SetButt.SetWindowTextA("Initialize");
}

void CLuminescenceComplexDlg::OnBnClickedButtonSet()
{
	
	if (whichMonoEnabled == 2)
	{
		pWnd = GetDlgItem(IDC_EDIT_Wavelength3);
		if (devARC.InitializeARC(isARCGr300)) 	pWnd->SetWindowText("200.0"); 
//		if (true) 	pWnd->SetWindowText("200.0"); 
		else pWnd->SetWindowText("Error");
		return;
	}

	
	
	
	UpdateData(true);
	double tf = atof(editBoxSetWave);

	float lowLim, upLim;
	if (whichMonoEnabled == 0) { lowLim = pPage4->t_LLIM1; upLim = pPage4->t_ULIM1; }
	else { lowLim = pPage4->t_LLIM2; upLim = pPage4->t_ULIM2; }
	
	

	if ((lowLim <= tf) && (tf <= upLim))
	{
		pWnd = GetDlgItem(IDC_mdr2);
		tempString.Format("%.1f", tf *0.547+197.45);
		pWnd->SetWindowText(tempString);

		if (whichMonoEnabled == 0)
		{
			lamb1 = tf;
			pWnd = GetDlgItem(IDC_EDIT_Wavelength1);
			tempString.Format("%.2f", lamb1);
			pWnd->SetWindowText(tempString);
		}
		else 
		{
			lamb2 = tf;
			pWnd = GetDlgItem(IDC_EDIT_Wavelength2);
			tempString.Format("%.2f", lamb2);
			pWnd->SetWindowText(tempString);
		}
	}
	else AfxMessageBox("The wavelength is out of range");
	
}

void CLuminescenceComplexDlg::OnBnClickedButtonStepup()
{

	double nmPerStep;

	if (whichMonoEnabled == 0) 	nmPerStep = pPage4->stepMotnmPerStep1;
	else 	 	nmPerStep = pPage4->stepMotnmPerStep2; 			
		
	int dirStep;
	if (nmPerStep < 0) { nmPerStep = abs(nmPerStep); dirStep = -1; }
	else dirStep = 1;

	if (nmPerStep < 0.001) return;

	if (whichMonoEnabled == 0) devPic.MotorGo(WORD(0.02 / nmPerStep), 1, dirStep);
	else devPic.MotorGo(WORD(0.02 / nmPerStep), 2, dirStep);

}
void CLuminescenceComplexDlg::OnBnClickedButtonStepdown()
{
	double nmPerStep;

	if (whichMonoEnabled == 0) 	nmPerStep = pPage4->stepMotnmPerStep1;
	else 	 	nmPerStep = pPage4->stepMotnmPerStep2;
	
	int dirStep;
	if (nmPerStep < 0) { nmPerStep = abs(nmPerStep); dirStep = -1; }
	else dirStep = 1;

	if (nmPerStep < 0.001) return;

	if (whichMonoEnabled == 0) devPic.MotorGo(WORD(0.02 / nmPerStep), 1, -1 * dirStep);
	else devPic.MotorGo(WORD(0.02 / nmPerStep), 2, -1 * dirStep);


	/*if (!isMono2Enabled) devPic.FirstMotorGoBack(1);
	else devPic.SecondMotorGoBack(1);*/
}

void CLuminescenceComplexDlg::OnBnClickedButtonGo()
{
	if (isMotorMoving)
	{
		m_Mono1Butt.EnableWindow(true);
		m_Mono2Butt.EnableWindow(true);
		m_buttonStart.EnableWindow(true);
	//	m_StopButt.EnableWindow(true);
		m_ContButt.EnableWindow(true);
		//вбиваємо потік
		DWORD exit_code = NULL; 
		if (m_motorMovingThread != NULL)
		{
			GetExitCodeThread(m_motorMovingThread->m_hThread, &exit_code);
			if (exit_code == STILL_ACTIVE)
			{
				::TerminateThread(m_motorMovingThread->m_hThread, 0);
				CloseHandle(m_motorMovingThread->m_hThread);
			}
			m_motorMovingThread->m_hThread = NULL;
			m_motorMovingThread = NULL;
			
		}

		isMotorMoving = false;
		m_GoButt.SetWindowTextA("Go");
		return;
	}
	m_GoButt.SetWindowTextA("Stop");
	
	UpdateData(true);
	
	if (isPicConnected) 
	{ 
		
		byte delay, delayMks;

		delay = (BYTE)truncf(pPage4->stepMot1Delay);
		delayMks = (BYTE)truncf((pPage4->stepMot1Delay - delay)*10);

		devPic.SetDelayForMotor1(delay, delayMks);

		delay = (BYTE)truncf(pPage4->stepMot2Delay);
		delayMks = (BYTE)truncf((pPage4->stepMot2Delay - delay)*10);

		devPic.SetDelayForMotor2(delay, delayMks);

	}
	else if (!isARCConnected) return;
	
	float lowLim, upLim;
	if (whichMonoEnabled == 0) { lowLim = pPage4->t_LLIM1; upLim = pPage4->t_ULIM1; }
	else { lowLim = pPage4->t_LLIM2; upLim = pPage4->t_ULIM2; }

	float tf =(float) atof(editBoxGoWave);

	
	if ((lowLim <= tf) && (tf <= upLim) && (whichMonoEnabled < 2))
	{
		Sleep(700);
		m_motorMovingThread = AfxBeginThread(MotorMovingThread, this);
		m_Mono1Butt.EnableWindow(false);
		m_Mono2Butt.EnableWindow(false);
		m_Mono3Butt.EnableWindow(false);
		m_buttonStart.EnableWindow(false);
		m_StopButt.EnableWindow(false);
		m_ContButt.EnableWindow(false);
	}
	else if (whichMonoEnabled == 2)
	{
		Sleep(300);
		m_motorMovingThread = AfxBeginThread(MotorMovingThread, this);
		m_Mono1Butt.EnableWindow(false);
		m_Mono2Butt.EnableWindow(false);
		m_Mono3Butt.EnableWindow(false);
		m_buttonStart.EnableWindow(false);
		m_StopButt.EnableWindow(false);
		m_ContButt.EnableWindow(false);
	}
	else AfxMessageBox("The wavelength is out of range");
	
}

UINT CLuminescenceComplexDlg::MotorMovingThread(LPVOID pParam)
{
	CLuminescenceComplexDlg* pThis = (CLuminescenceComplexDlg*)pParam;
	pThis->isMotorMoving = true;

	pThis->StopIdleMeas();

	UINT secToEnd = 0;

	short direction; byte motNum;
	double* lambda;
	float endPosition;
	double nmPerStep;
	double* motdelay;
	double mot1delay = pThis->pPage4->stepMot1Delay; 
	double mot2delay = pThis->pPage4->stepMot2Delay;
	double mot3delay = 8.68e-5;
	float lowLim, upLim;
	clock_t time = 0;
	clock_t time1 = 0; 

	endPosition = (float)atof(pThis->editBoxGoWave);
	if (pThis->whichMonoEnabled == 0)
	{
		nmPerStep = pThis->pPage4->stepMotnmPerStep1;
		lowLim = pThis->pPage4->t_LLIM1;
		upLim = pThis->pPage4->t_ULIM1;
		lambda = &pThis->lamb1;
		motNum = 1;
		motdelay = &mot1delay;
	}
	else if (pThis->whichMonoEnabled == 1)
	{
		nmPerStep = pThis->pPage4->stepMotnmPerStep2;
		lowLim = pThis->pPage4->t_LLIM2;
		upLim = pThis->pPage4->t_ULIM2;
		lambda = &pThis->lamb2;
		motNum = 2;
		motdelay = &mot1delay;
	}
	else // Мотаємо арком
	{
		if (pThis->isARCGr300) nmPerStep = pThis->arc_nmperstep1; else nmPerStep = pThis->arc_nmperstep2;
		lowLim = 0;
		upLim = 20000;
		lambda = &pThis->lamb3;
		motNum = 3;
		motdelay = &mot3delay;
	}

	if (*lambda < endPosition) direction = 1; else direction = -1;
	int dirStep;
	if (nmPerStep < 0) { dirStep = -1; nmPerStep = abs(nmPerStep); }
	else dirStep = 1;

	time1 = time = clock();

	ULONG Steps = ULONG(((endPosition - *lambda) * direction) / nmPerStep);
	ULONG Steps0 = Steps;
	WORD step01nm = WORD(0.1 / nmPerStep);
	if (step01nm < 1) step01nm = 1;

	while (Steps > 0)
	{
		if (Steps > step01nm) Steps -= step01nm; else { step01nm = Steps;  Steps = 0; }
		
		*lambda += (step01nm * nmPerStep) * direction;
		if ((*lambda < (lowLim - 0.1)) || (*lambda > (upLim +0.1))) { *lambda -= (step01nm * nmPerStep) * direction; break; }
		
		if (pThis->isPicConnected && (motNum < 3))
		{
			pThis->devPic.MotorGo(step01nm, motNum, direction * dirStep);
		}
		else 
			if (pThis->isARCConnected && (motNum == 3))
			{
				pThis->devARC.MotorGo(step01nm, direction * dirStep);
			}
		
				
		secToEnd = (UINT) abs(((endPosition - *lambda) / nmPerStep) * (*motdelay)) / 1000;
		pThis->statusString2.Format("Motor will be moving during next %d sec", secToEnd);

		time = clock();
		if ((float(time - time1) / CLOCKS_PER_SEC) > 0.5)
		{
			//if (pThis->isPicConnected && !pThis->isNFConnected) pThis->devPic.GetCounts(pThis->StopsInt, pThis->StopsCorInt);
			if (pThis->isNFConnected) pThis->devNanoFluor.GetIntensities(pThis->StartsInt, pThis->StopsInt, pThis->StartsCorInt, pThis->StopsCorInt);
			else if (pThis->isPicConnected) pThis->devPic.GetCounts(pThis->StopsInt, pThis->StopsCorInt);
			time1 = clock();
		}
	}

	//// доводка

	//while ((*lambda) * direction < (endPosition - 0.006 * direction) * direction)
	//{
	//	*lambda += 0.01 * direction;
	//	if ((*lambda < lowLim) || (*lambda > upLim)) { break; *lambda -= 0.01 * direction; }
	//	
	//	pThis->devPic.MotorGo(WORD(0.01 / nmPerStep), motNum, direction);
	//}

	/////////////////////////////



	
	pThis->isMotorMoving = false;
	pThis->m_GoButt.SetWindowTextA("Go");
	pThis->m_Mono1Butt.EnableWindow(true);
	pThis->m_Mono2Butt.EnableWindow(true);
	pThis->m_buttonStart.EnableWindow(true);
	pThis->m_StopButt.EnableWindow(true);
	pThis->m_ContButt.EnableWindow(true);
	pThis->statusString2 = "Motor moving Completed";
	return 0;
}

UINT CLuminescenceComplexDlg::MeasureThread(LPVOID pParam)
{
	CLuminescenceComplexDlg* pThis = (CLuminescenceComplexDlg*)pParam;
	
	pThis -> StopIdleMeas();
	

	switch (pThis->whatWeMeasure)
	{
	case cm_measKinHard:
		pThis->MeasureKineticHard();
		break;
	case cm_measKinSoft:
		pThis->MeasureKineticSoft();
		break;
	case cm_measLum:
		pThis->MeasureLuminescence();
		break;
	case cm_measComb:
		pThis->MeasureCombo();
		break;
	}
	
	return 0;
}

UINT CLuminescenceComplexDlg::IdlingIntensityMeasurementThread(LPVOID pParam)
{
	CLuminescenceComplexDlg* pThis = (CLuminescenceComplexDlg*)pParam;
	pThis->isIdleMeasuring = true;

	while (!(pThis->isStopIdle))
	{
		if (pThis->isPicConnected && !pThis->isNFConnected) pThis->devPic.GetCounts(pThis->StopsInt, pThis->StopsCorInt);
		if (pThis->isNFConnected) pThis->devNanoFluor.GetIntensities(pThis->StartsInt, pThis->StopsInt, pThis->StartsCorInt, pThis->StopsCorInt);
		Sleep(500);
	}
	pThis->isIdleMeasuring = false;
	return 0;
}

void CLuminescenceComplexDlg::OnBnClickedButtonStart()
{
	is_process_running = true;
	isContinued = false;
	StartMeasurement();
}

void CLuminescenceComplexDlg::StartMeasurement()
{


	m_saveButt.SetTextColor(RGB(0, 0, 0));
	m_saveButt.Invalidate();

	


	switch (m_ctrTab.GetCurSel())
	{
	case 1:
		if (pPage2->isSoftwareHistogram) whatWeMeasure = cm_measKinSoft;
		else whatWeMeasure = cm_measKinHard;
		Scope.Channels[1].Data.Clear();
		Scope.Channels[2].Data.Clear();
		break;
	case 0:
		whatWeMeasure = cm_measLum;
		Scope.Channels[2].Data.Clear();
		break;
	case 2:
		whatWeMeasure = cm_measComb;
		break;
	default:
		return;
		break;
	}

//	if (!is_process_running)
//	{
		UpdateData(true);

		DeactivateControls();

		m_MeasuringThread = AfxBeginThread(MeasureThread, this);
//	}
}

void CLuminescenceComplexDlg::OnBnClickedButtonstop()
{
	m_StopThread = true;
	ActivateControls();
	//SaveTempFile();
	statusString2.Format("Don't forget to save data!");
	m_saveButt.SetTextColor(RGB(255, 0, 0));
	m_saveButt.Invalidate();
}

void CLuminescenceComplexDlg::OnBnClickedButtoncontinue()
{
	is_process_running = true;
	m_saveButt.SetTextColor(RGB(0, 0, 0));
	m_saveButt.Invalidate();
	isContinued = true;
	UpdateData(true);
	StartMeasurement();
}

void CLuminescenceComplexDlg::DeactivateControls()
{
	m_buttonStart.EnableWindow(false);
	m_StopButt.EnableWindow(true);
	m_ContButt.EnableWindow(false);
	m_ctrTab.EnableWindow(false);
	m_GoButt.EnableWindow(false);
	m_SetButt.EnableWindow(false);
	m_Mono1Butt.EnableWindow(false);
	m_Mono2Butt.EnableWindow(false);
	timeWindCombo.EnableWindow(false);
}

void CLuminescenceComplexDlg::ActivateControls()
{
	m_buttonStart.EnableWindow(true);
	m_StopButt.EnableWindow(false);
	m_ContButt.EnableWindow(true);
	m_ctrTab.EnableWindow(true);
	m_GoButt.EnableWindow(true);
	m_SetButt.EnableWindow(true);
	m_Mono1Butt.EnableWindow(true);
	m_Mono2Butt.EnableWindow(true);
	timeWindCombo.EnableWindow(true);
}

void CLuminescenceComplexDlg::OnBnClickedCheckLogyscale()
{
	if (!isYScaleLog) Scope.YAxis.ScaleMode = smLogDecascale; // smLogarithmic;
	else Scope.YAxis.ScaleMode = smLinear; // smLogarithmic;
	isYScaleLog = !isYScaleLog;
}

void CLuminescenceComplexDlg::OnBnClickedSave()
{
	UpdateData(true);
	switch (whatWeMeasure)
	{
	case cm_measKinHard:
		SaveKineticHard();
		break;
	case cm_measKinSoft:
		SaveKineticSoft();
		break;
	case cm_measLum:
		SaveLuminescence();
		break;
	case cm_measComb:
		SaveCombo();
		break;
	}
	m_saveButt.SetTextColor(RGB(0, 0, 0));
	m_saveButt.Invalidate();
	
}

void CLuminescenceComplexDlg::SaveKineticHard(bool isTempFile)
{
	CFile* pFile = NULL;
	CString FileNameEx = FileName + ".txt";
	CString pathname;

	int result = IDOK;
	CFileDialog fileDialog(FALSE, NULL, FileNameEx);
	
	if (!isTempFile)
	{
		result = fileDialog.DoModal();	//запустить диалоговое окно
	}
	else
	{
		CHAR qwerty[512]; GetCurrentDirectory(MAX_PATH, qwerty); 
		pathname = qwerty; pathname += "\\temp\\";
		startTime = CTime::GetCurrentTime(); time_t stTm = startTime.GetTime();
		CString stTime; stTime.Format("%d", stTm); pathname += stTime + "_";
		pathname += FileNameEx;
	}
	
	
	if (result == IDOK)	//если файл выбран
	{
		try
		{
			if (!isTempFile) pathname = fileDialog.GetPathName();
			pFile = new CFile(pathname, CFile::modeCreate | CFile::modeReadWrite);
		}
		catch (CFileException* pEx)
		{
			TCHAR   szCause[255];
			pEx->GetErrorMessage(szCause, 255);
			AfxMessageBox(szCause);
		}

		if (pFile != NULL)
		{
			CString vyv; CString tmp;

			vyv = "time\t";

			vyv += FileName; 	//vyv += "_dt="; vyv += cinaKanala;
			tmp.Format("%i", timeOfMeas); vyv += "_DT="; vyv += tmp; vyv += "s";
			vyv += '\n';
			pFile->Write(vyv, vyv.GetLength());

			for (int i = 0; i < 1024; i++)
			{
				vyv.Empty();
				vyv.Format("%.3f\t%.0f\n", MainXArray[i], MainYArray[i]);
				pFile->Write(vyv, vyv.GetLength());
			}

			pFile->Close();
			delete pFile;
		}
	}
	
}

void CLuminescenceComplexDlg::SaveKineticSoft(bool isTempFile)
{
	CFile* pFile = NULL;
	CString FileNameEx = FileName + ".txt";
	CString pathname;

	int result = IDOK;
	CFileDialog fileDialog(FALSE, NULL, FileNameEx);

	if (!isTempFile)
	{
		result = fileDialog.DoModal();	//запустить диалоговое окно
	}
	else
	{
		CHAR qwerty[512]; GetCurrentDirectory(MAX_PATH, qwerty);
		pathname = qwerty; pathname += "\\temp\\";
		startTime = CTime::GetCurrentTime(); time_t stTm = startTime.GetTime();
		CString stTime; stTime.Format("%d", stTm); pathname += stTime + "_";
		pathname += FileNameEx;
	}


	if (result == IDOK)	//если файл выбран
	{
		try
		{
			if (!isTempFile) pathname = fileDialog.GetPathName();
			pFile = new CFile(pathname, CFile::modeCreate | CFile::modeReadWrite);
		}
		catch (CFileException* pEx)
		{
			TCHAR   szCause[255];
			pEx->GetErrorMessage(szCause, 255);
			AfxMessageBox(szCause);
		}

		if (pFile != NULL)
		{
			CString vyv; CString tmp;

			vyv = "time\t";

			vyv += FileName; //	vyv += "_dt="; vyv += cinaKanala;
			tmp.Format("%i", timeOfMeas); vyv += "_DT="; vyv += tmp; vyv += "s";
			vyv += '\n';
			pFile->Write(vyv, vyv.GetLength());

			for (int i = 0; i < m_numChan; i++)
			{
				vyv.Empty();
				vyv.Format("%.3f\t%.0f\n", MainXArray16[i], MainYArray16[i]);
				pFile->Write(vyv, vyv.GetLength());
			}

			pFile->Close();
			delete pFile;
		}
	}

}

void CLuminescenceComplexDlg::SaveLuminescence(bool isTempFile)
{
	CFile* pFile = NULL;
	CString FileNameEx = FileName + ".txt";
	CString pathname;

	int result = IDOK;
	CFileDialog fileDialog(FALSE, NULL, FileNameEx);

	if (!isTempFile)
	{
		result = fileDialog.DoModal();	//запустить диалоговое окно
	}
	else
	{
		CHAR qwerty[512]; GetCurrentDirectory(MAX_PATH, qwerty);
		pathname = qwerty; pathname += "\\temp\\";
		startTime = CTime::GetCurrentTime(); time_t stTm = startTime.GetTime();
		CString stTime; stTime.Format("%d", stTm); pathname += stTime + "_";
		pathname += FileNameEx;
	}


	if (result == IDOK)	//если файл выбран
	{
		try
		{
			if (!isTempFile) pathname = fileDialog.GetPathName();
			pFile = new CFile(pathname, CFile::modeCreate | CFile::modeReadWrite);
		}
		catch (CFileException* pEx) { TCHAR   szCause[255]; pEx->GetErrorMessage(szCause, 255); AfxMessageBox(szCause);		}

		if (pFile != NULL)
		{
			CString vyv; CString tmp;

			vyv = "nm\t"; vyv += FileName; vyv += '\n';
			pFile->Write(vyv, vyv.GetLength());

			for (int i = 0; i < LumX_Array.GetSize(); i++)
			{
				vyv.Empty();
				vyv.Format("%.3f\t%.1f\t%.1f\n", LumX_Array[i], LumY1_Array[i], LumY2_Array[i]);
				pFile->Write(vyv, vyv.GetLength());
			}

			pFile->Close();
			delete pFile;
		}
	}

}

void CLuminescenceComplexDlg::MeasureKineticHard(bool isForCombo)
{
	
	

	if (!isForCombo)
	{
		m_StopThread = false;
		is_process_running = true;
		Scope.XAxis.AxisLabel.Text = "Time, ns";
		
	}

	if (!isContinued)
	{
		devNanoFluor.CleanAndStartHardwareHistogram();

		float cinkan = ArrOfChaDiv[timeWindCombo.GetCurSel()];
		for (int i = 0; i < 1024; i++) MainXArray[i] = float(i);
		if (cinkan > 0.001) for (int i = 0; i < 1024; i++) MainXArray[i] *= cinkan;
		
	}
	else devNanoFluor.StartHardwareHistogram();

	

	startTime = CTime::GetCurrentTime();
	time_t stTm = startTime.GetTime();

	pPage2->m_MaxCount.GetWindowTextA(tempString);

	int countmax = atoi(tempString);
	if (!countmax) countmax = 1;
	CurrentMeasuredMax = 1;
	double vidsotok;
	
	clock_t time = 0;
	clock_t time1 = 0;

	while (!m_StopThread)
	{
		time = clock();
		if ((float(time - time1) / CLOCKS_PER_SEC) > 0.5)
		{
			if (isPicConnected && !isNFConnected) devPic.GetCounts(StopsInt, StopsCorInt);
			if (isNFConnected) devNanoFluor.GetIntensities(StartsInt, StopsInt, StartsCorInt, StopsCorInt);
			time1 = clock();
		}

		devNanoFluor.GetHardwareHistogram(MainYArray, CurrentMeasuredMax);

		Sleep(100);

		if (pPage2->isMaxTime)
		{
			stopTime = CTime::GetCurrentTime();
			time_t spTm = stopTime.GetTime();
			timeOfMeas = long(spTm - stTm);
			zalyshTime = pPage2->maxTime - timeOfMeas;
			if (!isForCombo) statusString2.Format("Kinetic will be measured during next %d sec", zalyshTime);
			if (timeOfMeas > (long)(pPage2->maxTime))
			{
				if (!isForCombo) m_StopThread = true;
				break;
			}
		}
		else
		{
			vidsotok = (CurrentMeasuredMax+0.0) / countmax;
			stopTime = CTime::GetCurrentTime();
			time_t spTm = stopTime.GetTime();
			timeOfMeas = long(spTm - stTm);
			zalyshTime = (UINT) (timeOfMeas / vidsotok - timeOfMeas);
			if (!isForCombo) statusString2.Format("Kinetic will be measured during next %d sec", zalyshTime);
			if (CurrentMeasuredMax >= countmax)
			{
				if (!isForCombo) m_StopThread = true;
				break;
			}
		}

	}


	stopTime = CTime::GetCurrentTime();
	time_t spTm = stopTime.GetTime();

	timeOfMeas = long(spTm - stTm);

	if (!isForCombo)
	{
		ActivateControls();
		is_process_running = false;
		SaveTempFile();
		statusString2.Format("Kinetic have been measured. Don't forget to save data!");
		m_saveButt.SetTextColor(RGB(255, 0, 0));
		m_saveButt.Invalidate();
	}

	devNanoFluor.StopHardwareHistogram();

}

void CLuminescenceComplexDlg::MeasureKineticSoft(bool isForCombo)
{
	
	
	if (!isForCombo)
	{
		m_StopThread = false;
		is_process_running = true;
		Scope.XAxis.AxisLabel.Text = "Time, ns";
	}

	pPage2->m_comboChanNum.GetWindowTextA(tempString);
	m_numChan = atoi(tempString);
	
	double vidsotok;
	

	devNanoFluor.PreparingForProgramHistogram();

	if (!isContinued)
	{
		double cinkan = ArrOfChaDiv[timeWindCombo.GetCurSel()];
		cinkan = cinkan * 1024.0f / m_numChan;
		for (int i = 0; i < m_numChan; i++) { MainXArray16[i] = float(i); MainYArray16[i] = 0;}
		if (cinkan > 0.00001) for (int i = 0; i < m_numChan; i++) MainXArray16[i] *= cinkan;

	}

	

	startTime = CTime::GetCurrentTime();
	time_t stTm = startTime.GetTime();

	pPage2->m_MaxCount.GetWindowTextA(tempString);

	int countmax = atoi(tempString);
	CurrentMeasuredMax = 0;

	byte numResults;
	byte sleepDuration = 10;

	while (!m_StopThread)
	{
		clock_t time = 0;
		clock_t time1 = 0;
		time = clock();
		if ((float(time - time1) / CLOCKS_PER_SEC) > 0.5)
		{
			if (isPicConnected && !isNFConnected) devPic.GetCounts(StopsInt, StopsCorInt);
			if (isNFConnected) devNanoFluor.GetIntensities(StartsInt, StopsInt, StartsCorInt, StopsCorInt);
			time1 = clock();
		}
		numResults = devNanoFluor.GetDataForSoftwareHistogram(MainYArray16, m_numChan, CurrentMeasuredMax);


		if (numResults > 25)
		{
			if (sleepDuration > 10) sleepDuration /= 2;
			else if (sleepDuration > 1) sleepDuration--;
		}
		if ((numResults < 8) && (sleepDuration < 250)) sleepDuration *= 2;

		Sleep(sleepDuration);

		if (pPage2->isMaxTime)
		{
			stopTime = CTime::GetCurrentTime();
			time_t spTm = stopTime.GetTime();
			timeOfMeas = long(spTm - stTm);
			zalyshTime = pPage2->maxTime - timeOfMeas;
			if (!isForCombo) statusString2.Format("Kinetic will be measured during next %d sec", zalyshTime);
			if (timeOfMeas > (long)(pPage2->maxTime))
			{
				if (!isForCombo) m_StopThread = true;
				break;
			}
		}
		else
		{
			vidsotok = CurrentMeasuredMax / countmax;
			stopTime = CTime::GetCurrentTime();
			time_t spTm = stopTime.GetTime();
			timeOfMeas = long(spTm - stTm);
			zalyshTime = (UINT) (timeOfMeas / vidsotok - timeOfMeas);
			if (!isForCombo) statusString2.Format("Kinetic will be measured during next %d sec", zalyshTime);
			if (CurrentMeasuredMax >= countmax)
			{
				if (!isForCombo) m_StopThread = true;
				break;
			}
		} 
	}

	stopTime = CTime::GetCurrentTime();
	time_t spTm = stopTime.GetTime();

	timeOfMeas = long(spTm - stTm);

	if (!isForCombo)
	{
		ActivateControls();

		is_process_running = false;
		devNanoFluor.StopHardwareHistogram();
		SaveTempFile();
		statusString2.Format("Kinetic have been measured. Don't forget to save data!", zalyshTime);
		m_saveButt.SetTextColor(RGB(255, 0, 0));
		m_saveButt.Invalidate();
	}

}

void CLuminescenceComplexDlg::MeasureLuminescence(void)
{
	m_StopThread = false;
	is_process_running = true;

	UINT secToEnd = 0;

	Scope.XAxis.AxisLabel.Text = "Wavelength, nm";
	
	if (!isContinued)
	{
		LumX_Array.RemoveAll(); LumY1_Array.RemoveAll(); LumY2_Array.RemoveAll();
		CurrentMeasuredMax = 1;
	}
	
	

	double* lambda; double* motdelay; double* nmPerStep;
	double mot1delay = pPage4->stepMot1Delay; double mot2delay = pPage4->stepMot2Delay;
	double mot3delay = 8.68e-5;
	double nmPerStep1 = pPage4->stepMotnmPerStep1; double nmPerStep2 = pPage4->stepMotnmPerStep2;
	float lowLim, upLim;
	byte motNum;
	
	
	if (whichMonoEnabled == 0)
	{
		lambda = &lamb1;
		lowLim = pPage4->t_LLIM1;
		upLim = pPage4->t_ULIM1;
		motdelay = &mot1delay;
		nmPerStep = &nmPerStep1;
		motNum = 1;
	}
	else if (whichMonoEnabled == 1)
		{
			lambda = &lamb2;
			lowLim = pPage4->t_LLIM2;
			upLim = pPage4->t_ULIM2;
			motdelay = &mot2delay;
			nmPerStep = &nmPerStep2;		
			motNum = 2;
		}
		else // мотаємо арком
		{
			lambda = &lamb3;
			lowLim = 0;
			upLim = 20000;
			motdelay = &mot3delay;
			if (isARCGr300) nmPerStep = &arc_nmperstep1; else nmPerStep = &arc_nmperstep2;
			motNum = 3;
		}
			

	if (isPicConnected && (motNum < 3))
	{ 
		byte delay, delayMks;

		delay = (BYTE)truncf(pPage4->stepMot1Delay);
		delayMks = (BYTE)truncf((pPage4->stepMot1Delay - delay)*10);

		devPic.SetDelayForMotor1(delay, delayMks);

		delay = (BYTE)truncf(pPage4->stepMot2Delay);
		delayMks = (BYTE)truncf((pPage4->stepMot2Delay - delay)*10);

		devPic.SetDelayForMotor2(delay, delayMks);
	}
	else if (isARCConnected && (motNum == 3))
		{
			// може щось ініціалізувати в арку? хоча навіщо?
		}
		else return;


	startTime = CTime::GetCurrentTime();
	time_t stTm = startTime.GetTime();

	
		
	double stepNm = pPage1->m_stepNm; if (stepNm < 0.01) stepNm = 0.01;
	double endNm = pPage1->m_endNm;	if ((endNm > upLim) || (endNm < lowLim)) endNm = upLim;

	if (isPicConnected && !isNFConnected) if ((pPage1->m_lumIntegrTime) < 1) pPage1->m_lumIntegrTime = 1;
	if (isNFConnected) if ((pPage1->m_lumIntegrTime) < 0.5) pPage1->m_lumIntegrTime = 0.5;

	UINT NumOfMeas;
	if (isPicConnected && !isNFConnected) NumOfMeas = (UINT)((pPage1->m_lumIntegrTime) / 0.99);
	if (isNFConnected) NumOfMeas = (UINT)((pPage1->m_lumIntegrTime) / 0.49);
	
	UINT i; double integral = 0; double fast = 0; double delayDueMMoving = 0;
	short direction; 


	if (*lambda < endNm) direction = 1; else direction = -1;
	int dirStep;
	if (*nmPerStep < 0) { dirStep = -1; *nmPerStep = abs(*nmPerStep); }
	else dirStep = 1;

	delayDueMMoving = (stepNm / *nmPerStep) * (*motdelay);

	bool isNotFirstMeas = false;


	ULONG Steps = ULONG(((endNm - *lambda) * direction) / *nmPerStep);
	ULONG Steps0 = Steps;
	WORD step01nm = WORD(stepNm / *nmPerStep);
	if (step01nm < 1) step01nm = 1;

	bool motorGoResult = false;

	while (Steps > 0)
	{
		if (m_StopThread) break;

		if (Steps > step01nm) Steps -= step01nm; else { step01nm = Steps;  Steps = 0; }

		if (isNotFirstMeas || isContinued)
		{
			*lambda += (step01nm * (*nmPerStep)) * direction;
			if ((*lambda < (lowLim - 0.1)) || (*lambda > (upLim + 0.1))) { *lambda -= (step01nm * (*nmPerStep)) * direction; break; }
		//	statusString2 = "Motor Started";
			if (motNum < 3) motorGoResult = devPic.MotorGo(step01nm, motNum, direction * dirStep);
			else devARC.MotorGo(step01nm, direction * dirStep);
		//	if (motorGoResult) statusString2 = "true"; else statusString2 = "false";
		}
		
		isNotFirstMeas = true;
		
		// measure intensity
		
		for (i = 0; i < NumOfMeas; i++)
		{
			
			if (isPicConnected && !isNFConnected) devPic.GetCounts(StopsInt, StopsCorInt);
			
			if (isNFConnected)
			{
				Sleep(500);
				devNanoFluor.GetIntensities(StartsInt, StopsInt, StartsCorInt, StopsCorInt);
			}
			integral += StopsInt; fast += StopsCorInt;
		}

		integral /= NumOfMeas; fast /= NumOfMeas;

		if (CurrentMeasuredMax < integral) CurrentMeasuredMax = (DWORD) integral;
		if (CurrentMeasuredMax < fast) CurrentMeasuredMax = (DWORD) fast;

		LumX_Array.Add(*lambda); LumY1_Array.Add(integral); LumY2_Array.Add(fast);
		integral = 0; fast = 0;

		if (delayDueMMoving < 510) Sleep(DWORD(510 - delayDueMMoving));

		secToEnd = UINT (abs(((endNm - *lambda) / *nmPerStep) * (*motdelay)) / 1000 + 0.5 * NumOfMeas * abs((endNm - *lambda) / stepNm));
		statusString2.Format("Spectrum will be measured during next %d sec", secToEnd);

	}

	

	stopTime = CTime::GetCurrentTime();
	time_t spTm = stopTime.GetTime();

	timeOfMeas = long(spTm - stTm);

	ActivateControls();

	is_process_running = false;

	statusString2 = "Spectrum measurement have completed, don`t forget to save data!";
	SaveTempFile();
	m_saveButt.SetTextColor(RGB(255, 0, 0));
	m_saveButt.Invalidate();
	
}

void CLuminescenceComplexDlg::OnBnClickedButton2()
{
		
}

void CLuminescenceComplexDlg::SaveTempFile()
{
	switch (whatWeMeasure)
	{
	case cm_measKinHard:
		SaveKineticHard(true);
		break;
	case cm_measKinSoft:
		SaveKineticSoft(true);
		break;
	case cm_measLum:
		SaveLuminescence(true);
		break;
	case cm_measComb:
		SaveCombo(true);
		break;
	}
}

void CLuminescenceComplexDlg::StartIdleMeas()
{
	m_IdleMeasuringThread = AfxBeginThread(IdlingIntensityMeasurementThread, this);
}

void CLuminescenceComplexDlg::StopIdleMeas()
{
	isStopIdle = true;
	
	while (isIdleMeasuring)
	{
		Sleep(100);
	}
	


	DWORD exit_code = NULL;
	if (m_IdleMeasuringThread != NULL)
	{
		GetExitCodeThread(m_IdleMeasuringThread->m_hThread, &exit_code);
		if (exit_code == STILL_ACTIVE)
		{
			::TerminateThread(m_IdleMeasuringThread->m_hThread, 0);
			CloseHandle(m_IdleMeasuringThread->m_hThread);
		}
		m_IdleMeasuringThread->m_hThread = NULL;
		m_IdleMeasuringThread = NULL;

	}

}

void CLuminescenceComplexDlg::MeasureCombo(void)
{
	m_StopThread = false;
	is_process_running = true;
	
	double* arrPtr;
	double summ;

	double cinkan = ArrOfChaDiv[timeWindCombo.GetCurSel()];

	if (isPlotLuminForCombo) Scope.XAxis.AxisLabel.Text = "Wavelength, nm"; else Scope.XAxis.AxisLabel.Text = "Time, ns";

	if (pPage2->isSoftwareHistogram)
	{
		pPage2->m_comboChanNum.GetWindowTextA(tempString);
		m_numChan = atoi(tempString);
		cinkan = cinkan * 1024.0f / m_numChan;
	}
	


	UINT FWstart = 0; UINT FWstop = 0; UINT SWstart = 0; UINT SWstop = 0;
	UINT secToEnd = 0;

	if (!isContinued)
	{
		LumX_Array.RemoveAll(); LumY1_Array.RemoveAll(); LumY2_Array.RemoveAll(); LumY3_Array.RemoveAll();
		for (int i = 0; i < ComboYArray.GetSize(); i++) delete[] ComboYArray[i]; ComboYArray.RemoveAll();
		
		CurrentMeasuredMax = 1;
		CurrentMeasuredMaxForComboLum = 1;
	}
	
	

	double* lambda; double* motdelay; double* nmPerStep;
	double mot1delay = pPage4->stepMot1Delay; double mot2delay = pPage4->stepMot2Delay;
	double nmPerStep1 = pPage4->stepMotnmPerStep1; double nmPerStep2 = pPage4->stepMotnmPerStep2;
	float lowLim, upLim;
	byte motNum;
	double mot3delay = 8.68e-5;


	if (whichMonoEnabled == 0)
	{
		lambda = &lamb1;
		lowLim = pPage4->t_LLIM1;
		upLim = pPage4->t_ULIM1;
		motdelay = &mot1delay;
		nmPerStep = &nmPerStep1;
		motNum = 1;
	}
	else if (whichMonoEnabled == 1)
		{
			lambda = &lamb2;
			lowLim = pPage4->t_LLIM2;
			upLim = pPage4->t_ULIM2;
			motdelay = &mot2delay;
			nmPerStep = &nmPerStep2;
			motNum = 2;
		}
	else // мотаємо арком
	{
		lambda = &lamb3;
		lowLim = 0;
		upLim = 20000;
		motdelay = &mot3delay;
		if (isARCGr300) nmPerStep = &arc_nmperstep1; else nmPerStep = &arc_nmperstep2;
		motNum = 3;
	}


	if (isPicConnected && (motNum < 3))
	{
		byte delay, delayMks;

		delay = (BYTE)truncf(pPage4->stepMot1Delay);
		delayMks = (BYTE)truncf((pPage4->stepMot1Delay - delay) * 10);

		devPic.SetDelayForMotor1(delay, delayMks);

		delay = (BYTE)truncf(pPage4->stepMot2Delay);
		delayMks = (BYTE)truncf((pPage4->stepMot2Delay - delay) * 10);

		devPic.SetDelayForMotor2(delay, delayMks);
	}
	else if (isARCConnected && (motNum == 3))
	{
		// може щось ініціалізувати в арку? хоча навіщо?
	}
	else return;

	startTime = CTime::GetCurrentTime();
	time_t stTm = startTime.GetTime();



	double stepNm = pPage1->m_stepNm; if (stepNm < 0.01) stepNm = 0.01;
	double endNm = pPage1->m_endNm;	if ((endNm > upLim) || (endNm < lowLim)) endNm = upLim;

	
	UINT i; double integral = 0; double fast = 0; double delayDueMMoving = 0;
	short direction;


	if (*lambda < endNm) direction = 1; else direction = -1;
	int dirStep;
	if (*nmPerStep < 0) { dirStep = -1; *nmPerStep = abs(*nmPerStep); }
	else dirStep = 1;

	delayDueMMoving = (stepNm / *nmPerStep) * (*motdelay);

	if (cinkan != 0)
	{
		FWstart = (UINT)(pPage3->m_FWstart / cinkan);
		FWstop = (UINT)(pPage3->m_FWstop / cinkan);
		SWstart = (UINT)(pPage3->m_SWstart / cinkan);
		SWstop = (UINT)(pPage3->m_SWstop / cinkan);
	}

	bool isNotFirstMeas = false;

	ULONG Steps = ULONG(((endNm - *lambda) * direction) / *nmPerStep);
	ULONG Steps0 = Steps;
	WORD step01nm = WORD(stepNm / *nmPerStep);
	if (step01nm < 1) step01nm = 1;

	while (Steps > 0)
	{
		if (m_StopThread) break;

		if (Steps > step01nm) Steps -= step01nm; else { step01nm = Steps;  Steps = 0; }

		if (isNotFirstMeas || isContinued)
		{
			*lambda += (step01nm * (*nmPerStep)) * direction;
			if ((*lambda < (lowLim - 0.1)) || (*lambda > (upLim + 0.1))) { *lambda -= (step01nm * (*nmPerStep)) * direction; break; }
			if (motNum < 3) devPic.MotorGo(step01nm, motNum, direction * dirStep);
			else devARC.MotorGo(step01nm, direction * dirStep);
		}

		isNotFirstMeas = true;
		isContinued = false;
		// measure 

		if (!pPage2->isSoftwareHistogram)
		{
			
			MeasureKineticHard(true);
			
			//if (!isNotFirstMeas) for (i = 0; i < 1024; i++) ComboHardXArray[i] = MainXArray[i];
			
			arrPtr = new double[1024];
			for (i = 0; i < 1024; i++) arrPtr[i] = MainYArray[i];
			ComboYArray.Add(arrPtr);

			summ = 0;
			for (i = 0; i < 1024; i++) summ += arrPtr[i];
			LumY1_Array.Add(summ);
			if (summ > CurrentMeasuredMaxForComboLum) CurrentMeasuredMaxForComboLum = (DWORD)summ;

			summ = 0;
			for (i = FWstart; i < FWstop; i++) summ += arrPtr[i];
			LumY2_Array.Add(summ);

			summ = 0;
			for (i = SWstart; i < SWstop; i++) summ += arrPtr[i];
			LumY3_Array.Add(summ);
		}
		else
		{
			
			MeasureKineticSoft(true);

			//if (!isNotFirstMeas) for (i = 0; i < UINT(m_numChan); i++) ComboSoftXArray[i] = MainXArray16[i];
			
			arrPtr = new double[m_numChan];
			for (i = 0; i < UINT(m_numChan); i++) arrPtr[i] = MainYArray16[i];
			ComboYArray.Add(arrPtr);

			summ = 0;
			for (i = 0; i < UINT(m_numChan); i++) summ += arrPtr[i];
			LumY1_Array.Add(summ);
			if (summ > CurrentMeasuredMaxForComboLum) CurrentMeasuredMaxForComboLum = (DWORD)summ;

			summ = 0;
			for (i = FWstart; i < FWstop; i++) summ += arrPtr[i];
			LumY2_Array.Add(summ);

			summ = 0;
			for (i = SWstart; i < SWstop; i++) summ += arrPtr[i];
			LumY3_Array.Add(summ);
		}
		
		
					
		LumX_Array.Add(*lambda);

		if (pPage2->isMaxTime)
		{
			secToEnd = UINT(abs(((endNm - *lambda) / *nmPerStep) * (*motdelay)) / 1000 + (pPage2->maxTime) * abs((endNm - *lambda) / stepNm));
			statusString2.Format("Spectrum of kinetics will be measured during next %d sec", secToEnd);
		}
		else statusString2.Format("It is imposible to estimate time to end. Please be patient...", secToEnd);

	}



	stopTime = CTime::GetCurrentTime();
	time_t spTm = stopTime.GetTime();

	timeOfMeas = long(spTm - stTm);

	ActivateControls();

	is_process_running = false;

	statusString2 = "Spectrum of kinetics measurement have completed, don`t forget to save data!";
	SaveTempFile();
	m_saveButt.SetTextColor(RGB(255, 0, 0));
	m_saveButt.Invalidate();

	

}


void CLuminescenceComplexDlg::OnBnClickedRadioShowdec()
{
	isPlotLuminForCombo = false;
	Scope.XAxis.AxisLabel.Text = "Time, ns";
}


void CLuminescenceComplexDlg::OnBnClickedRadioShowlum()
{
	isPlotLuminForCombo = true;
	Scope.XAxis.AxisLabel.Text = "Wavelength, nm";
}


void CLuminescenceComplexDlg::OnBnClickedButton1()
{
	CString A;
	
	CFile* pFile = NULL;
	pFile = new CFile("d:\\data.txt", CFile::modeRead);

	

	ULONGLONG sz = pFile->GetLength();


		
	pFile->Read(A.GetBufferSetLength(sz), sz);
	A.ReleaseBuffer();






	/*pFile->Read(buf, sz);

	buf[sz] = '\0';*/


	//A = CString(buf);
	CString tempStr, ch1, ch2;
	UINT pos = 0, posN;
	int p1, p2;

	int i = 0;

	while (pos < sz)
	{
		posN = A.Find('\n', pos);
		tempStr = A.Mid(pos, posN - pos);

		p1 = tempStr.Find('\t', 0);
		p2 = tempStr.Find('\r', p1);

		ch1 = tempStr.Mid(0, p1);
		ch2 = tempStr.Mid(p1 + 1, p2-2);

		pos = posN + 1;

		MainXArray[i] = atof(ch1);
		MainYArray[i] = atof(ch2);

		whatWeMeasure = cm_measKinHard;

		CurrentMeasuredMax = 8;
		is_process_running = true;

		i++;
	}


	pFile->Close();
	delete pFile;
}


void CLuminescenceComplexDlg::SaveCombo(bool isTempFile)
{
	CFile* pFile = NULL;
	CFile* pFile2 = NULL;
	CString FileNameEx = FileName + "_lumin.txt";
	CString FileNameExCombDec = FileName + "_kinetics.txt";
	CString pathname, pathnameCombDec;
	double* ptrArr;

	int result = IDOK;
	CFileDialog fileDialog(FALSE, NULL, FileNameEx);

	if (!isTempFile)
	{
		result = fileDialog.DoModal();	//запустить диалоговое окно
	}
	else
	{
		CHAR qwerty[512]; GetCurrentDirectory(MAX_PATH, qwerty);
		pathname = qwerty; pathname += "\\temp\\";
		startTime = CTime::GetCurrentTime(); time_t stTm = startTime.GetTime();
		CString stTime; stTime.Format("%d", stTm); pathname += stTime + "_";
		pathname += FileNameEx;
		pathnameCombDec = pathname + FileNameExCombDec;
	}


	if (result == IDOK)	//если файл выбран
	{
		try
		{
			if (!isTempFile) pathname = fileDialog.GetPathName();
			pFile = new CFile(pathname, CFile::modeCreate | CFile::modeReadWrite);
			pathname = pathname.Mid(0, pathname.GetLength() - 4);
			pathnameCombDec = pathname + "_kinetics.txt";
			pFile2 = new CFile(pathnameCombDec, CFile::modeCreate | CFile::modeReadWrite);
		}
		catch (CFileException* pEx) { TCHAR   szCause[255]; pEx->GetErrorMessage(szCause, 255); AfxMessageBox(szCause); }

		if (pFile != NULL)
		{
			CString vyv; 

			vyv = "nm\t"; vyv += FileName; vyv += '\n';
			pFile->Write(vyv, vyv.GetLength());

			for (int i = 0; i < LumX_Array.GetSize(); i++)
			{
				vyv.Empty();
				vyv.Format("%.3f\t%.1f\t%.1f\t%.1f\n", LumX_Array[i], LumY1_Array[i], LumY2_Array[i], LumY3_Array[i]);
				pFile->Write(vyv, vyv.GetLength());
			}

			pFile->Close();
			delete pFile;
		}

		if (pFile2 != NULL)
		{
			CString vyv; CString tmp;

			vyv.Empty();
			vyv = "time\t";
			for (int i = 0; i < LumX_Array.GetSize(); i++)
			{
				tmp.Empty();
				tmp.Format("%.3f\t", LumX_Array[i]);
				vyv += tmp;
			}
			vyv += "\n";
			pFile2->Write(vyv, vyv.GetLength());

			vyv.Empty();
			if (!pPage2->isSoftwareHistogram)
			{
				int sizex = LumX_Array.GetSize();
				int sizey = ComboYArray.GetSize();

				for (int i = 0; i < 1024; i++)
				{
					vyv.Format("%.3f\t", MainXArray[i]);
					tmp.Empty();
					for (int j = 0; j < LumX_Array.GetSize(); j++)
					{
						ptrArr =(double*) ComboYArray[j];
						tmp.Format("%.0f\t", ptrArr[i]);
						vyv += tmp;
					}
					vyv += "\n";
					pFile2->Write(vyv, vyv.GetLength());
				}
			}
			else
			{
				for (int i = 0; i < m_numChan; i++)
				{
					vyv.Format("%.3f\t", MainXArray16[i]);
					tmp.Empty();
					for (int j = 0; j < LumX_Array.GetSize(); j++)
					{
						ptrArr = (double*)ComboYArray[j];
						tmp.Format("%.0f\t", ptrArr[i]);
						vyv += tmp;
					}
					vyv += "\n";
					pFile2->Write(vyv, vyv.GetLength());
				}
			}
			

			pFile2->Close();
			delete pFile2;
		}

	}

}

void CLuminescenceComplexDlg::CalculateDecayConstant()
{

	double a, b, c, d1, d2, x, y;

	double& calcTime = pPage2->m_calcRes;


	double cinkan = ArrOfChaDiv[timeWindCombo.GetCurSel()];
	if (!cinkan) return;

	
	if (pPage2->isSoftwareHistogram)
	{
		pPage2->m_comboChanNum.GetWindowTextA(tempString);
		m_numChan = atoi(tempString);
		cinkan = cinkan * 1024.0f / m_numChan;
	}
	else m_numChan = 1024;


		
	UINT m_begin = (UINT)(pPage2->m_beginCalc / cinkan);
	UINT m_end = (UINT)(pPage2->m_endCalc / cinkan);
	if (m_end > (UINT)(m_numChan - 1)) m_end = m_numChan - 1;
	
	a = float(m_end - m_begin + 1); if (a < 2) { calcTime = -1; pPage2->UpdateData(false); return; }

	b = 0; c = 0; d1 = 0; d2 = 0;
	
	double* LogArray; LogArray = new(double[16384]);

	if (!pPage2->isSoftwareHistogram)
	{
		for (int i = 0; i < m_numChan; i++)
			if (MainYArray[i] == 0) LogArray[i] = 0.0; else LogArray[i] = log(MainYArray[i]);
	}
	else
	{
		for (int i = 0; i < m_numChan; i++)
			if (MainYArray16[i] == 0) LogArray[i] = 0.0; else LogArray[i] = log(MainYArray16[i]);
	}

	

	if (!pPage2->isSoftwareHistogram)
	{
		for (int i = m_begin; i < (int)(m_end + 1); i++)
		{
			b += MainXArray[i];
			c += MainXArray[i] * MainXArray[i];
			d1 += LogArray[i];
			d2 += LogArray[i] * MainXArray[i];
		}
	}
	else
	{
		for (int i = m_begin; i < (int)(m_end + 1); i++)
		{
			b += MainXArray16[i];
			c += MainXArray16[i] * MainXArray16[i];
			d1 += LogArray[i];
			d2 += LogArray[i] * MainXArray16[i];
		}
	}


	delete[] LogArray;

	if ((c - b * b / a) != 0) y = (d2 - b * d1 / a) / (c - b * b / a); else { calcTime = -1; pPage2->UpdateData(false); return; }
	x = d1 / a - b / a * y;

	calcTime = double(-1 / y);

	double tau = calcTime;
	double A = exp(x);
	double *xarr, *yarr; xarr = new(double[1024]); yarr = new(double[1024]);
	

	for (int i = 0; i < (m_end - m_begin + 1); i++)
	{
		xarr[i] = (i + m_begin) * cinkan;
		yarr[i] = A * exp(-xarr[i] / tau);
	}

	

	int t = Scope.Channels.Count;
	if (t < 2) Scope.Channels.Add();
	Scope.Channels[1].Data.SetXYData(xarr, yarr, m_end - m_begin);

	delete[] xarr, yarr;

	pPage2->UpdateData(false);

}



void CLuminescenceComplexDlg::OnBnClickedAndigst()
{
	devNanoFluor.SetSecondInputState(0);
	
		
}


void CLuminescenceComplexDlg::OnBnClickedDigst2()
{
	devNanoFluor.SetSecondInputState(1);
}





void CLuminescenceComplexDlg::OnBnClickedArcSetup()
{
	// TODO: Add your control notification handler code here
}


void CLuminescenceComplexDlg::OnBnClickedRadioGr500()
{
	isARCGr300 = false;
	
	// Initialization
	pWnd = GetDlgItem(IDC_EDIT_Wavelength3);
	if (devARC.InitializeARC(isARCGr300)) 	pWnd->SetWindowText("200.0"); 
//	if (true) 	pWnd->SetWindowText("200.0"); 
	else pWnd->SetWindowText("Error");
}


void CLuminescenceComplexDlg::OnBnClickedRadioGr300()
{
	isARCGr300 = true;

	// Initialization
	pWnd = GetDlgItem(IDC_EDIT_Wavelength3);
	if (devARC.InitializeARC(isARCGr300)) 	pWnd->SetWindowText("200.0"); 
//	if (true) 	pWnd->SetWindowText("200.0"); 
	else pWnd->SetWindowText("Error");
}
