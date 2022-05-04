#pragma once
#include "Serial.h"

class CARCControl
{
public:
	
	CARCControl();

	CString comport;
	Serial* SP;

	void SetComportAndConnect(CString Comport);
	bool InitializeARC(bool isGr300);
	bool GotoStep(long stepnum);
	bool IsConnected();
	bool MotorGo(WORD numberOfSteps, short Direction = -1);
	bool isInitialized;

	double b1;
	double b2;
	long a1;
	long a2;
};

//if (!(SP->IsConnected()))
//{
//	SP->~Serial();
//	SP = new Serial(comport);
//}

//SP->WriteData("7",1);

//readResult = SP->ReadData(incomingData, dataLength);