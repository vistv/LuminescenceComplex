#include "pch.h"

#include "CWinUsbNanoFluor.h"

//#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <winusb.h>
#include <usb.h>

#include <cfgmgr32.h>

HRESULT CWinUsbNanoFluor::OpenDevice()
{
	DeviceData = &deviceData;
	HRESULT hr = S_OK;
	BOOL    bResult;

	DeviceData->HandlesOpen = FALSE;

	hr = RetrieveDevicePath(DeviceData->DevicePath, sizeof(DeviceData->DevicePath));

	if (FAILED(hr)) {

		return hr;
	}



	DeviceData->DeviceHandle = CreateFile(DeviceData->DevicePath,
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);

	if (INVALID_HANDLE_VALUE == DeviceData->DeviceHandle) {

		hr = HRESULT_FROM_WIN32(GetLastError());
		return hr;
	}

	bResult = WinUsb_Initialize(DeviceData->DeviceHandle,
		&DeviceData->WinusbHandle);

	if (FALSE == bResult) {

		hr = HRESULT_FROM_WIN32(GetLastError());
		CloseHandle(DeviceData->DeviceHandle);
		return hr;
	}

	DeviceData->HandlesOpen = TRUE;
	return hr;

}

VOID CWinUsbNanoFluor::CloseDevice()
{
	if (FALSE == DeviceData->HandlesOpen) {

		//
		// Called on an uninitialized DeviceData
		//
		return;
	}

	WinUsb_Free(DeviceData->WinusbHandle);
	CloseHandle(DeviceData->DeviceHandle);
	DeviceData->HandlesOpen = FALSE;

	return;
}

HRESULT CWinUsbNanoFluor::RetrieveDevicePath(LPTSTR DevicePath, ULONG  BufLen)
{
	CONFIGRET cr = CR_SUCCESS;
	HRESULT   hr = S_OK;
	PTSTR     DeviceInterfaceList = NULL;
	ULONG     DeviceInterfaceListLength = 0;

	//if (NULL != FailureDeviceNotFound) {

	//    *FailureDeviceNotFound = FALSE;
	//}

	//
	// Enumerate all devices exposing the interface. Do this in a loop
	// in case a new interface is discovered while this code is executing,
	// causing CM_Get_Device_Interface_List to return CR_BUFFER_SMALL.
	//
	do {
		cr = CM_Get_Device_Interface_List_Size(&DeviceInterfaceListLength,
			(LPGUID)&GUID_DEVINTERFACE_WinUsbForNanoFluor,
			NULL,
			CM_GET_DEVICE_INTERFACE_LIST_PRESENT);

		if (cr != CR_SUCCESS) {
			hr = HRESULT_FROM_WIN32(CM_MapCrToWin32Err(cr, ERROR_INVALID_DATA));
			break;
		}

		DeviceInterfaceList = (PTSTR)HeapAlloc(GetProcessHeap(),
			HEAP_ZERO_MEMORY,
			DeviceInterfaceListLength * sizeof(TCHAR));

		if (DeviceInterfaceList == NULL) {
			hr = E_OUTOFMEMORY;
			break;
		}

		cr = CM_Get_Device_Interface_List((LPGUID)&GUID_DEVINTERFACE_WinUsbForNanoFluor,
			NULL,
			DeviceInterfaceList,
			DeviceInterfaceListLength,
			CM_GET_DEVICE_INTERFACE_LIST_PRESENT);

		if (cr != CR_SUCCESS) {
			HeapFree(GetProcessHeap(), 0, DeviceInterfaceList);

			if (cr != CR_BUFFER_SMALL) {
				hr = HRESULT_FROM_WIN32(CM_MapCrToWin32Err(cr, ERROR_INVALID_DATA));
			}
		}
	} while (cr == CR_BUFFER_SMALL);

	if (FAILED(hr)) {
		return hr;
	}

	//
	// If the interface list is empty, no devices were found.
	//
	if (*DeviceInterfaceList == TEXT('\0')) {
		if (NULL != FailureDeviceNotFound) {
			*FailureDeviceNotFound = TRUE;
		}

		hr = HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
		HeapFree(GetProcessHeap(), 0, DeviceInterfaceList);
		return hr;
	}

	//
	// Give path of the first found device interface instance to the caller. CM_Get_Device_Interface_List ensured
	// the instance is NULL-terminated.
	//
	hr = StringCbCopy(DevicePath,
		BufLen,
		DeviceInterfaceList);

	HeapFree(GetProcessHeap(), 0, DeviceInterfaceList);

	return hr;
}

WINUSB_INTERFACE_HANDLE CWinUsbNanoFluor::GetWinUsbIntHandle()
{
	return DeviceData->WinusbHandle;
}


bool CWinUsbNanoFluor::SetCmd(byte* Cmd_P, byte* answer)
{
	byte b;
	ULONG bytesWritten;

	if (DeviceData->WinusbHandle == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	b = 4;
	if (!WinUsb_WritePipe(DeviceData->WinusbHandle, 0x01, Cmd_P, b, &bytesWritten, NULL))
	{
		return false;
	}

	b = 64;
	if (!WinUsb_ReadPipe(DeviceData->WinusbHandle, 0x81, answer, b, &bytesWritten, NULL))
	{
		return false;
	}
	/*   struct PIPE_ID
{
	UCHAR  PipeInId;
	UCHAR  PipeOutId;
};
WINUSB_PIPE_INFORMATION  Pipe;

int index = 1;

bool bResult = WinUsb_QueryPipe(DeviceData->WinusbHandle, 0, index, &Pipe);*/
	return true;
}

//var b : byte;
//i: integer;
//begin
//OutputReport[0]: = 0;
//for i: = 0 to 4 do OutputReport[i + 1] : = Cmd_P[i];
//b: = 5;
//if not WriteReport(hDevice, @OutPutReport, b) then MessageDlg('Îøèáêà çàïèñè', mtError, [mbOk], 0);
//b: = 65;
//if not ReadReport(hDevice, @InPutReport, b) then MessageDlg('Îøèáêà ÷òåíèÿ', mtError, [mbOk], 0);
//for i: = 0 to 63 do answer[i] : = InPutReport[i + 1];
//end;

bool CWinUsbNanoFluor::GetRanges(CStringArray* _cstarr)
{
	_cstarr->RemoveAll();

	_Cmd_P[0] = 8; _Cmd_P[1] = 0xFF; _Cmd_P[2] = 0; _Cmd_P[3] = 0;
	SetCmd(_Cmd_P, _answer);
	if (_answer[0] == 0) return false;

	CString tempStr;

	while ((_answer[0] != 0) && (_Cmd_P[2] <= 15))
	{
		int xx = 0; tempStr.Empty();
		while (_answer[xx] != 0)
		{
			tempStr += _answer[xx];
			xx++;
		}
		_cstarr->Add(tempStr);
		_Cmd_P[2]++;
		SetCmd(_Cmd_P, _answer);
	}
	return true;
}

byte CWinUsbNanoFluor::GetRangeNumber()
{
	_Cmd_P[0] = 8; _Cmd_P[1] = 0; _Cmd_P[2] = 0; _Cmd_P[3] = 0;
	SetCmd(_Cmd_P, _answer);
	return _answer[0];
}
void CWinUsbNanoFluor::SetRangeNumber(byte rangeNumber)
{
	_Cmd_P[0] = 8; _Cmd_P[1] = 1; _Cmd_P[2] = rangeNumber; _Cmd_P[3] = 0;
	SetCmd(_Cmd_P, _answer);
}

byte CWinUsbNanoFluor::IsFirstInputDigital()
{
	_Cmd_P[0] = 4; _Cmd_P[1] = 0; _Cmd_P[2] = 0; _Cmd_P[3] = 0;
	SetCmd(_Cmd_P, _answer);
	if ((_answer[1] && 0x01) == 0) return 0; else return 1;
}
byte CWinUsbNanoFluor::IsSecondInputDigital()
{
	_Cmd_P[0] = 4; _Cmd_P[1] = 0; _Cmd_P[2] = 0; _Cmd_P[3] = 0;
	SetCmd(_Cmd_P, _answer);
	if ((_answer[1] && 0x02) == 0) return 0; else return 1;
}
byte CWinUsbNanoFluor::IsDelaySet()
{
	_Cmd_P[0] = 4; _Cmd_P[1] = 0; _Cmd_P[2] = 0; _Cmd_P[3] = 0;
	SetCmd(_Cmd_P, _answer);
	if ((_answer[1] && 0x04) == 0) return 0; else return 1;
}
byte CWinUsbNanoFluor::IsIndicationSet()
{
	_Cmd_P[0] = 4; _Cmd_P[1] = 0; _Cmd_P[2] = 0; _Cmd_P[3] = 0;
	SetCmd(_Cmd_P, _answer);
	if ((_answer[1] && 0x80) == 0) return 0; else return 1;
}

void CWinUsbNanoFluor::SetFirstInputState(byte isDigital)
{
	_Cmd_P[0] = 4; _Cmd_P[1] = 0; _Cmd_P[2] = 0; _Cmd_P[3] = 0; //читаємо стан
	SetCmd(_Cmd_P, _answer);

	_Cmd_P[0] = 4; _Cmd_P[1] = 0xFF; _Cmd_P[2] = 0; _Cmd_P[3] = 0;
	if (_answer[1] & 0x01) _Cmd_P[2] |= 0x01;// старт
	if (_answer[1] & 0x02) _Cmd_P[2] |= 0x02;// стоп
	if (_answer[1] & 0x04) _Cmd_P[2] |= 0x04;// затримка
	if (_answer[1] & 0x80) _Cmd_P[2] |= 0xC0;// індикація

	if (isDigital) _Cmd_P[2] |= 0x01; else _Cmd_P[2] &= 0xFE;

	SetCmd(_Cmd_P, _answer);
}
void CWinUsbNanoFluor::SetSecondInputState(byte isDigital)
{
	_Cmd_P[0] = 4; _Cmd_P[1] = 0; _Cmd_P[2] = 0; _Cmd_P[3] = 0; //читаємо стан
	SetCmd(_Cmd_P, _answer);

	_Cmd_P[0] = 4; _Cmd_P[1] = 0xFF; _Cmd_P[2] = 0; _Cmd_P[3] = 0;
	if (_answer[1] & 0x01) _Cmd_P[2] |= 0x01;// старт
	if (_answer[1] & 0x02) _Cmd_P[2] |= 0x02;// стоп
	if (_answer[1] & 0x04) _Cmd_P[2] |= 0x04;// затримка
	if (_answer[1] & 0x80) _Cmd_P[2] |= 0xC0;// індикація

	if (isDigital) _Cmd_P[2] |= 0x02; else _Cmd_P[2] &= 0xFD;

	SetCmd(_Cmd_P, _answer);
}
void CWinUsbNanoFluor::SetDelayState(byte isSet)
{
	_Cmd_P[0] = 4; _Cmd_P[1] = 0; _Cmd_P[2] = 0; _Cmd_P[3] = 0; //читаємо стан
	SetCmd(_Cmd_P, _answer);

	_Cmd_P[0] = 4; _Cmd_P[1] = 0xFF; _Cmd_P[2] = 0; _Cmd_P[3] = 0;
	if (_answer[1] & 0x01) _Cmd_P[2] |= 0x01;// старт
	if (_answer[1] & 0x02) _Cmd_P[2] |= 0x02;// стоп
	if (_answer[1] & 0x04) _Cmd_P[2] |= 0x04;// затримка
	if (_answer[1] & 0x80) _Cmd_P[2] |= 0xC0;// індикація

	if (isSet) _Cmd_P[2] |= 0x04; else _Cmd_P[2] &= 0xFB;

	SetCmd(_Cmd_P, _answer);
}
void CWinUsbNanoFluor::SetIndicationState(byte isVisible)
{
	_Cmd_P[0] = 4; _Cmd_P[1] = 0; _Cmd_P[2] = 0; _Cmd_P[3] = 0; //читаємо стан
	SetCmd(_Cmd_P, _answer);

	_Cmd_P[0] = 4; _Cmd_P[1] = 0xFF; _Cmd_P[2] = 0; _Cmd_P[3] = 0;
	if (_answer[1] & 0x01) _Cmd_P[2] |= 0x01;// старт
	if (_answer[1] & 0x02) _Cmd_P[2] |= 0x02;// стоп
	if (_answer[1] & 0x04) _Cmd_P[2] |= 0x04;// затримка
	if (_answer[1] & 0x80) _Cmd_P[2] |= 0xC0;// індикація

	if (isVisible) _Cmd_P[2] |= 0xC0; else _Cmd_P[2] &= 0x3F;

	SetCmd(_Cmd_P, _answer);
}

byte CWinUsbNanoFluor::IsScaleInverted()
{
	_Cmd_P[0] = 4; _Cmd_P[1] = 0; _Cmd_P[2] = 0; _Cmd_P[3] = 0;
	SetCmd(_Cmd_P, _answer);
	if ((_answer[1] && 0x08) == 0) return 0; else return 1;
}
byte CWinUsbNanoFluor::GetDelayValue()
{
	_Cmd_P[0] = 4; _Cmd_P[1] = 0; _Cmd_P[2] = 0; _Cmd_P[3] = 0;
	SetCmd(_Cmd_P, _answer);
	return _answer[3];
}
void CWinUsbNanoFluor::SetScaleInverted(byte isSet)
{
	_Cmd_P[0] = 4; _Cmd_P[1] = 0xF0; _Cmd_P[2] = 0; _Cmd_P[3] = 0;
	if (isSet) _Cmd_P[2] = 0xFF;
	SetCmd(_Cmd_P, _answer);
}
void CWinUsbNanoFluor::SetDelayValue(byte delayValue, byte isSet)
{
	_Cmd_P[0] = 4; _Cmd_P[1] = 0xFC; _Cmd_P[2] = isSet; _Cmd_P[3] = delayValue;
	SetCmd(_Cmd_P, _answer);
}

byte CWinUsbNanoFluor::PreparingForProgramHistogram()
{
	_Cmd_P[0] = 5; _Cmd_P[1] = 0; _Cmd_P[2] = 0; _Cmd_P[3] = 0;
	SetCmd(_Cmd_P, _answer);
	return _answer[0];
}

byte CWinUsbNanoFluor::GetDataForSoftwareHistogram(double* softData, UINT m_numChan, DWORD& currentMax)
{
	_Cmd_P[0] = 5; _Cmd_P[1] = 0; _Cmd_P[2] = 0; _Cmd_P[3] = 0;
	SetCmd(_Cmd_P, _answer);
	if (_answer[0] == 0) return 0;
	byte div = 0x4000 / m_numChan;
	int nData;
	for (int i = 1; i <= _answer[0]; i++)
	{
		nData = 0x3FFF & (_answer[2 * i] + (_answer[2 * i + 1] << 8));
		nData /= div;
		if (softData[nData] < 0xFFFF) softData[nData]++;
		if (currentMax < softData[nData]) currentMax = softData[nData];
	}
	return _answer[0];
}
void CWinUsbNanoFluor::GetHardwareHistogram(double* hardwareData, DWORD& currentMax)
{
	for (int i = 0; i <= 31; i++)
	{
		_Cmd_P[0] = 7; _Cmd_P[1] = i; _Cmd_P[2] = 0; _Cmd_P[3] = 0;
		SetCmd(_Cmd_P, _answer);
		for (int k = 0; k <= 31; k++)
		{
			hardwareData[k + 32 * i] = _answer[2 * k] + (_answer[2 * k + 1] << 8);
			if (currentMax < hardwareData[k + 32 * i]) currentMax = hardwareData[k + 32 * i];
		}
	}
}

void CWinUsbNanoFluor::CleanAndStartHardwareHistogram()
{
	_Cmd_P[0] = 6; _Cmd_P[1] = 255; _Cmd_P[2] = 0; _Cmd_P[3] = 0;
	SetCmd(_Cmd_P, _answer);
}
void CWinUsbNanoFluor::StopHardwareHistogram()
{
	_Cmd_P[0] = 6; _Cmd_P[1] = 0; _Cmd_P[2] = 0; _Cmd_P[3] = 0;
	SetCmd(_Cmd_P, _answer);
}
void CWinUsbNanoFluor::StartHardwareHistogram()
{
	_Cmd_P[0] = 6; _Cmd_P[1] = 1; _Cmd_P[2] = 0; _Cmd_P[3] = 0;
	SetCmd(_Cmd_P, _answer);
}

void CWinUsbNanoFluor::GetIntensities(UINT& UnCorrStarts, UINT& UnCorrStops, UINT& CorrStarts, UINT& CorrStops)
{
	_Cmd_P[0] = 3; _Cmd_P[1] = 0; _Cmd_P[2] = 0; _Cmd_P[3] = 0;
	SetCmd(_Cmd_P, _answer);

	if ((_answer[0] & _answer[1]) == 0xFF) UnCorrStarts = -1;
	else
	{
		if ((_answer[1] & 0x80) == 0) UnCorrStarts = 2 * (_answer[0] + 256 * _answer[1]);
		else
		{
			UnCorrStarts = 0.2 * (_answer[0] + 256 * (_answer[1] & 0x7F));
			UnCorrStarts *= 1000;
		}
	}

	if ((_answer[2] & _answer[3]) == 0xFF) UnCorrStops = -1;
	else
	{
		if ((_answer[3] & 0x80) == 0) UnCorrStops = 2 * (_answer[2] + 256 * _answer[3]);
		else
		{
			UnCorrStops = 0.2 * (_answer[2] + 256 * (_answer[3] & 0x7F));
			UnCorrStops *= 1000;
		}
	}
		
	if ((_answer[4] & _answer[5]) == 0xFF) CorrStarts = -1;
	else CorrStarts = 2 * (_answer[4] + 256 * _answer[5]);
	
	if ((_answer[6] & _answer[7]) == 0xFF) CorrStops = -1;
	else CorrStops = 2 * (_answer[6] + 256 * _answer[7]);

}
