#include "pch.h"
#include "CARCControl.h"

CARCControl::CARCControl()
{
	SP = new Serial(comport);
}


bool CARCControl::InitializeARC()
{
   if (!(SP->IsConnected()))
	{
		SP->~Serial();
		SP = new Serial(comport);
	}
   if (!(SP->IsConnected())) return false;
   
   SP->WriteData("init", 4);
   Sleep(1000);

   int readResult = 0;
   char incomingData[36] = "";
   int dataLength = 36;

   for (int i = 0; i < 300; i++)
   {
		readResult = SP->ReadData(incomingData, dataLength);
		if (readResult == 0) { Sleep(1000); continue; } else break;
   }
   if (readResult == 0) return false;
   
   
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
	SP->WriteData(st, st.GetLength());

	int readResult = 0;
	char incomingData[36] = "";
	int dataLength = 36;

	for (long i = 0; i < 300000; i++)
	{
		readResult = SP->ReadData(incomingData, dataLength);
		if (readResult == 0) { Sleep(1); continue; } else break;
	}

	if (readResult == 0) return false;


	return true;
}
