#include "pch.h"

#include "CWinUsbPic.h"

//#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <winusb.h>
#include <usb.h>

#include <cfgmgr32.h>

//WinUsb_FlushPipe(DeviceData->WinusbHandle, 0x81);


HRESULT CWinUsbPic::OpenDevice()
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
VOID CWinUsbPic::CloseDevice()
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
HRESULT CWinUsbPic::RetrieveDevicePath(LPTSTR DevicePath, ULONG  BufLen)
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
            (LPGUID)&GUID_DEVINTERFACE_USBApplication3,
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

        cr = CM_Get_Device_Interface_List((LPGUID)&GUID_DEVINTERFACE_USBApplication3,
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

WINUSB_INTERFACE_HANDLE CWinUsbPic::GetWinUsbIntHandle()
{
    return DeviceData->WinusbHandle;
}


bool CWinUsbPic::FirstMotorGoForward(WORD numberOfSteps)
{
    wordbytes.w = numberOfSteps; //мотаєм
    usb_out_buffer[0] = 0x21;     usb_out_buffer[1] = wordbytes.b[0];     usb_out_buffer[2] = wordbytes.b[1];
    
    if (!WinUsb_WritePipe(GetWinUsbIntHandle(), 0x01, &usb_out_buffer[0], 3, &bytesWritten, NULL)) return false;
    byte b = 64;//чекаєм коли домотає
   // Sleep(100);
   //
    WinUsb_FlushPipe(DeviceData->WinusbHandle, 0x81);
    
    answer[0] = 0;
    
    while (true)
    {
        WinUsb_ReadPipe(DeviceData->WinusbHandle, 0x81, answer, b, &bytesWritten, NULL);
        if (answer[0] == 0x21) break;
    }

    
    return true;
             
}

bool CWinUsbPic::FirstMotorGoBack(WORD numberOfSteps)
{
    wordbytes.w = numberOfSteps; //мотаєм
    usb_out_buffer[0] = 0x22;     usb_out_buffer[1] = wordbytes.b[0];     usb_out_buffer[2] = wordbytes.b[1];
    
    if (!WinUsb_WritePipe(GetWinUsbIntHandle(), 0x01, &usb_out_buffer[0], 3, &bytesWritten, NULL)) return false;
    byte b = 64;//чекаєм коли домотає
    
    answer[0] = 0;
    
    while(true)
    {
        WinUsb_ReadPipe(DeviceData->WinusbHandle, 0x81, answer, b, &bytesWritten, NULL);
        if (answer[0] == 0x22) break;
    }

    return true; 
}

bool CWinUsbPic::SecondMotorGoForward(WORD numberOfSteps)
{
    wordbytes.w = numberOfSteps; //мотаєм
    usb_out_buffer[0] = 0x31;     usb_out_buffer[1] = wordbytes.b[0];     usb_out_buffer[2] = wordbytes.b[1];
    
    if (!WinUsb_WritePipe(GetWinUsbIntHandle(), 0x01, &usb_out_buffer[0], 3, &bytesWritten, NULL)) return false;
    byte b = 64;//чекаєм коли домотає
    


    answer[0] = 0;

    while (true)
    {
        WinUsb_ReadPipe(DeviceData->WinusbHandle, 0x81, answer, b, &bytesWritten, NULL);
        if (answer[0] == 0x31) break;
    }
    return true;
}

bool CWinUsbPic::SecondMotorGoBack(WORD numberOfSteps)
{
    wordbytes.w = numberOfSteps; //мотаєм
    usb_out_buffer[0] = 0x32;     usb_out_buffer[1] = wordbytes.b[0];     usb_out_buffer[2] = wordbytes.b[1];
    
    if (!WinUsb_WritePipe(GetWinUsbIntHandle(), 0x01, &usb_out_buffer[0], 3, &bytesWritten, NULL)) return false;
    byte b = 64;//чекаєм коли домотає
    
    answer[0] = 0;

    while (true)
    {
        WinUsb_ReadPipe(DeviceData->WinusbHandle, 0x81, answer, b, &bytesWritten, NULL);
        if (answer[0] == 0x32) break;
    }
    return true;
}


bool CWinUsbPic::SetDelayForMotor1(byte delay, byte delayMks)
{
    delayForMotor1 = delay; delayForMotor1Mks = delayMks;
    usb_out_buffer[0] = 0x20;     usb_out_buffer[1] = delayForMotor1; usb_out_buffer[2] = delayForMotor1Mks;    // затримка між кроками
    if (!WinUsb_WritePipe(GetWinUsbIntHandle(), 0x01, &usb_out_buffer[0], 3, &bytesWritten, NULL)) return false;
}

bool CWinUsbPic::SetDelayForMotor2(byte delay, byte delayMks)
{
    delayForMotor2 = delay; delayForMotor2Mks = delayMks;
    usb_out_buffer[0] = 0x30;     usb_out_buffer[1] = delayForMotor2; usb_out_buffer[2] = delayForMotor2Mks;    // затримка між кроками
    if (!WinUsb_WritePipe(GetWinUsbIntHandle(), 0x01, &usb_out_buffer[0], 3, &bytesWritten, NULL)) return false;
}


bool CWinUsbPic::MotorGo(WORD numberOfSteps, byte NumberOfMotor, short Direction)
{
    if ((NumberOfMotor == 1) && (Direction == 1)) return FirstMotorGoForward(numberOfSteps);
    if ((NumberOfMotor == 1) && (Direction == -1)) return FirstMotorGoBack(numberOfSteps);
    if ((NumberOfMotor == 2) && (Direction == 1)) return SecondMotorGoForward(numberOfSteps);
    if ((NumberOfMotor == 2) && (Direction == -1)) return SecondMotorGoBack(numberOfSteps);
    return 0;
}


bool CWinUsbPic::FirstMotorGoForward(WORD numberOfSteps, UINT& counter1, UINT& counter2)
{
    wordbytes.w = numberOfSteps; //мотаєм
    usb_out_buffer[0] = 0x21;     usb_out_buffer[1] = wordbytes.b[0];     usb_out_buffer[2] = wordbytes.b[1];
    
    if (!WinUsb_WritePipe(GetWinUsbIntHandle(), 0x01, &usb_out_buffer[0], 3, &bytesWritten, NULL)) return false;
    byte b = 64;//чекаєм коли домотає
    
    answer[0] = 0;

    while (true)
    {
        WinUsb_ReadPipe(DeviceData->WinusbHandle, 0x81, answer, b, &bytesWritten, NULL);
        if (answer[0] == 0x21) break;
    }

    
    counter1 = answer[1] + (answer[2] << 8) + (answer[3] << 16);
    counter2 = answer[4] + (answer[5] << 8) + (answer[6] << 16);
    UINT timeticks = answer[7] + (answer[8] << 8) + (answer[9] << 16);
  
    double time = timeticks / 1500000.0;
    counter1 = UINT (counter1 / time);
    counter2 = UINT(counter2 / time);
    
    return true;
}

bool CWinUsbPic::FirstMotorGoBack(WORD numberOfSteps, UINT& counter1, UINT& counter2)
{
    wordbytes.w = numberOfSteps; //мотаєм
    usb_out_buffer[0] = 0x22;     usb_out_buffer[1] = wordbytes.b[0];     usb_out_buffer[2] = wordbytes.b[1];
    
    if (!WinUsb_WritePipe(GetWinUsbIntHandle(), 0x01, &usb_out_buffer[0], 3, &bytesWritten, NULL)) return false;
    byte b = 64;//чекаєм коли домотає
   
    answer[0] = 0;

    while (true)
    {
        WinUsb_ReadPipe(DeviceData->WinusbHandle, 0x81, answer, b, &bytesWritten, NULL);
        if (answer[0] == 0x22) break;
    }


    counter1 = answer[1] + (answer[2] << 8) + (answer[3] << 16);
    counter2 = answer[4] + (answer[5] << 8) + (answer[6] << 16);
    UINT timeticks = answer[7] + (answer[8] << 8) + (answer[9] << 16);

    double time = timeticks / 1500000.0;
    counter1 = UINT(counter1 / time);
    counter2 = UINT(counter2 / time);

    return true;
}

bool CWinUsbPic::SecondMotorGoForward(WORD numberOfSteps, UINT& counter1, UINT& counter2)
{
    wordbytes.w = numberOfSteps; //мотаєм
    usb_out_buffer[0] = 0x31;     usb_out_buffer[1] = wordbytes.b[0];     usb_out_buffer[2] = wordbytes.b[1];
    
    if (!WinUsb_WritePipe(GetWinUsbIntHandle(), 0x01, &usb_out_buffer[0], 3, &bytesWritten, NULL)) return false;
    byte b = 64;//чекаєм коли домотає
    

    answer[0] = 0;

    while (true)
    {
        WinUsb_ReadPipe(DeviceData->WinusbHandle, 0x81, answer, b, &bytesWritten, NULL);
        if (answer[0] == 0x31) break;
    }

    counter1 = answer[1] + (answer[2] << 8) + (answer[3] << 16);
    counter2 = answer[4] + (answer[5] << 8) + (answer[6] << 16);
    UINT timeticks = answer[7] + (answer[8] << 8) + (answer[9] << 16);

    double time = timeticks / 1500000.0;
    counter1 = UINT(counter1 / time);
    counter2 = UINT(counter2 / time);

    return true;
}

bool CWinUsbPic::SecondMotorGoBack(WORD numberOfSteps, UINT& counter1, UINT& counter2)
{
    wordbytes.w = numberOfSteps; //мотаєм
    usb_out_buffer[0] = 0x32;     usb_out_buffer[1] = wordbytes.b[0];     usb_out_buffer[2] = wordbytes.b[1];
    
    if (!WinUsb_WritePipe(GetWinUsbIntHandle(), 0x01, &usb_out_buffer[0], 3, &bytesWritten, NULL)) return false;
    byte b = 64;//чекаєм коли домотає
    

    answer[0] = 0;

    while (true)
    {
        WinUsb_ReadPipe(DeviceData->WinusbHandle, 0x81, answer, b, &bytesWritten, NULL);
        if (answer[0] == 0x32) break;
    }

    counter1 = answer[1] + (answer[2] << 8) + (answer[3] << 16);
    counter2 = answer[4] + (answer[5] << 8) + (answer[6] << 16);
    UINT timeticks = answer[7] + (answer[8] << 8) + (answer[9] << 16);

    double time = timeticks / 1500000.0;
    counter1 = UINT(counter1 / time);
    counter2 = UINT(counter2 / time);

    return true;
}

bool CWinUsbPic::MotorGo(WORD numberOfSteps, byte NumberOfMotor, short Direction, UINT& counter1, UINT& counter2)
{
    if ((NumberOfMotor == 1) && (Direction == 1)) return FirstMotorGoForward(numberOfSteps, counter1, counter2);
    if ((NumberOfMotor == 1) && (Direction == -1)) return FirstMotorGoBack(numberOfSteps, counter1, counter2);
    if ((NumberOfMotor == 2) && (Direction == 1)) return SecondMotorGoForward(numberOfSteps, counter1, counter2);
    if ((NumberOfMotor == 2) && (Direction == -1)) return SecondMotorGoBack(numberOfSteps, counter1, counter2);

    return 0;
}

bool CWinUsbPic::GetCounts(UINT& counter1, UINT& counter2)
{
    usb_out_buffer[0] = 0x51;     usb_out_buffer[1] = 1;     usb_out_buffer[2] = 0;
    
    if (!WinUsb_WritePipe(DeviceData->WinusbHandle, 0x01, &usb_out_buffer[0], 3, &bytesWritten, NULL)) return false;
    byte b = 64;//чекаєм на відповідь
    
    
    answer[0] = 0;

    while (true)
    {
        WinUsb_ReadPipe(DeviceData->WinusbHandle, 0x81, answer, b, &bytesWritten, NULL);
        if (answer[0] == 0x51) break;
    }
      
    counter1 = answer[1] + (answer[2] << 8) + (answer[3] << 16);
    counter2 = answer[4] + (answer[5] << 8) + (answer[6] << 16);
    return true;
}
