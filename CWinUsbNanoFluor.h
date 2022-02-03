#pragma once
#include <initguid.h>
#include <wtypes.h>
#include <winusb.h>

DEFINE_GUID(GUID_DEVINTERFACE_WinUsbForNanoFluor,
    0x3de35e36,0xd24a,0x4fed,0xa7,0x9e,0x6e,0x6f,0x69,0x55,0x37,0x17);

//typedef struct _DEVICE_DATA {
//
//    BOOL                    HandlesOpen;
//    WINUSB_INTERFACE_HANDLE WinusbHandle;
//    HANDLE                  DeviceHandle;
//    TCHAR                   DevicePath[MAX_PATH];
//
//} DEVICE_DATA, * PDEVICE_DATA;

struct PIPE_ID
{
    UCHAR  PipeInId;
    UCHAR  PipeOutId;
};

class CWinUsbNanoFluor
{
protected:

    DEVICE_DATA  deviceData;
    PDEVICE_DATA DeviceData;
    PBOOL  FailureDeviceNotFound;
    HRESULT RetrieveDevicePath(LPTSTR DevicePath, ULONG  BufLen);

public:

    HRESULT OpenDevice();
    VOID CloseDevice();
    WINUSB_INTERFACE_HANDLE GetWinUsbIntHandle();

    bool SetCmd(byte *Cmd_P, byte* answer);

	bool GetRanges(CStringArray* _cstarr);
	byte GetRangeNumber();
    void SetRangeNumber(byte rangeNumber);

private:
	byte _Cmd_P[64];
    byte _answer[64];
public:
    byte IsFirstInputDigital();
    byte IsSecondInputDigital();
    byte IsScaleInverted();
    byte IsDelaySet();
    byte IsIndicationSet();
    byte GetDelayValue();
    void SetFirstInputState(byte isDigital);
    void SetSecondInputState(byte isDigital);
    void SetDelayState(byte isSet);
    void SetIndicationState(byte isVisible);
    void SetScaleInverted(byte isSet);
    void SetDelayValue(byte delayValue, byte isSet);
    byte GetDataForSoftwareHistogram(double* softData, UINT m_numChan, DWORD& currentMax);
    byte PreparingForProgramHistogram();
    void GetHardwareHistogram(double* hardwareData, DWORD& currentMax);
    void CleanAndStartHardwareHistogram();
    void StopHardwareHistogram();
    void StartHardwareHistogram();
    void GetIntensities(UINT& UnCorrStarts, UINT& UnCorrStops, UINT& CorrStarts, UINT& CorrStops);
};

