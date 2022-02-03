#pragma once
#include <initguid.h>
#include <wtypes.h>
#include <winusb.h>

/* Example
    CWinUsbPic picDevice;

    HRESULT hr = picDevice.OpenDevice();

    picDevice.SetDelayForMotor1(2);
    bool b = picDevice.FirstMotorGoForward(500);

    picDevice.CloseDevice();
*/

DEFINE_GUID(GUID_DEVINTERFACE_USBApplication3,
    0xcde7b613, 0x37ef, 0x4ee8, 0x82, 0x00, 0xe9, 0x60, 0x48, 0xb1, 0x03, 0xda);

typedef struct _DEVICE_DATA {

    BOOL                    HandlesOpen;
    WINUSB_INTERFACE_HANDLE WinusbHandle;
    HANDLE                  DeviceHandle;
    TCHAR                   DevicePath[MAX_PATH];

} DEVICE_DATA, * PDEVICE_DATA;

union WordBytes
{
    WORD w;
    byte b[2];
};

class CWinUsbPic
{
protected:

    DEVICE_DATA  deviceData;
    PDEVICE_DATA DeviceData;
    PBOOL  FailureDeviceNotFound;
    UCHAR usb_out_buffer[64];     
    UCHAR answer[64];
    ULONG bytesWritten;     
    DWORD n;
    WordBytes wordbytes;
    byte delayForMotor1 = 3;
    byte delayForMotor2 = 3;
    byte delayForMotor1Mks = 0;
    byte delayForMotor2Mks = 0;

public:

    HRESULT OpenDevice();
    VOID CloseDevice();
    HRESULT RetrieveDevicePath(LPTSTR DevicePath, ULONG  BufLen);


    WINUSB_INTERFACE_HANDLE GetWinUsbIntHandle();
    bool FirstMotorGoForward(WORD numberOfSteps);
    bool FirstMotorGoBack(WORD numberOfSteps);
    bool SecondMotorGoForward(WORD numberOfSteps);
    bool SecondMotorGoBack(WORD numberOfSteps);

    bool FirstMotorGoForward(WORD numberOfSteps, UINT& counter1, UINT& counter2);
    bool FirstMotorGoBack(WORD numberOfSteps, UINT& counter1, UINT& counter2);
    bool SecondMotorGoForward(WORD numberOfSteps, UINT& counter1, UINT& counter2);
    bool SecondMotorGoBack(WORD numberOfSteps, UINT& counter1, UINT& counter2);
    
    bool MotorGo(WORD numberOfSteps, byte NumberOfMotor = 1, short Direction = -1);
    bool MotorGo(WORD numberOfSteps, byte NumberOfMotor, short Direction, UINT& counter1, UINT& counter2);

    bool SetDelayForMotor1(byte delay, byte delayMks);
    bool SetDelayForMotor2(byte delay, byte delayMks);
    
    bool GetCounts(UINT& counter1, UINT& counter2);
};

