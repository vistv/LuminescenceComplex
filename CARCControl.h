#pragma once
#include "Serial.h"

class CARCControl
{
public:
	CARCControl();

	CString comport = "COM3";
	Serial* SP;

	bool InitializeARC();
	bool GotoStep(long stepnum);
};

//if (!(SP->IsConnected()))
//{
//	SP->~Serial();
//	SP = new Serial(comport);
//}

//SP->WriteData("7",1);

//readResult = SP->ReadData(incomingData, dataLength);