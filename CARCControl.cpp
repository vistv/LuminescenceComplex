#include "pch.h"
#include "CARCControl.h"

CARCControl::CARCControl()
{
	
}

void CARCControl::SetComportAndConnect(CString Comport)
{
	comport = Comport;
	SP = new Serial(comport);
//	bool test = SP->IsConnected();
}


bool CARCControl::InitializeARC(bool isGr300)
{
   if (!(SP->IsConnected()))
	{
		SP->~Serial();
		SP = new Serial(comport);
	}
   if (!(SP->IsConnected())) return false;
   
   SP->WriteData("i", 1);
   Sleep(1000);

   int readResult = 0;
   char incomingData[36] = "";
   int dataLength = 36;

   for (int i = 0; i < 100; i++)
   {
		readResult = SP->ReadData(incomingData, dataLength);
		if (readResult == 0) { Sleep(1000); continue; } else break;
   }
   if (readResult == 0) return false;
   
   // їдемо на 200нм залежно від гратки

   long stepnum;
   
   if (isGr300) stepnum = a1 + 200 / b1;
   else stepnum = a2 + 200 / b2;

   GotoStep(stepnum);
   
   return true;
}


bool CARCControl::GotoStep(long stepnum)
{
	if (!(SP->IsConnected()))
	{
		SP->~Serial();
		SP = new Serial(comport);
	}
	if (!(SP->IsConnected())) return false;

	CString st;
	st.Format("%2d", stepnum);
	st = "g" + st;
	SP->WriteData(st, st.GetLength());

	int readResult = 0;
	char incomingData[36] = "";
	int dataLength = 36;

	for (long i = 0; i < 50000; i++)
	{
		readResult = SP->ReadData(incomingData, dataLength);
		if (readResult == 0) { Sleep(1); continue; } else break;
	}

	if (readResult == 0) return false;


	return true;
}


bool CARCControl::IsConnected()
{
	bool res = SP->WriteData("a", 1);
	
	int readResult = 0;
	char incomingData[36] = "";
	int dataLength = 36;
	Sleep(100);
	for (int i = 0; i < 3; i++)
	{
		readResult = SP->ReadData(incomingData, dataLength);
		if (readResult == 0) { Sleep(1000); continue; }
		else break;
	}
	if (incomingData[0] == 'a') return true;
	return false;
}


bool CARCControl::MotorGo(WORD numberOfSteps, short Direction)
{
	if (!(SP->IsConnected()))
	{
		SP->~Serial();
		SP = new Serial(comport);
	}
	if (!(SP->IsConnected())) return false;
	
	CString data = "d";
	CString tmp;
	tmp.Format("%d", numberOfSteps);
	
	if (Direction == -1) data += "-";
	data += tmp;
	
	bool res = SP->WriteData(data, data.GetLength());

	int readResult = 0;
	char incomingData[36] = "";
	int dataLength = 36;

	for (long i = 0; i < 50000; i++)
	{
		readResult = SP->ReadData(incomingData, dataLength);
		if (readResult == 0) { Sleep(1); continue; }
		else break;
	}

	if (readResult == 0) return false;
	
	return true;
}
