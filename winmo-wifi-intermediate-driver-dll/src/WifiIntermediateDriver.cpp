/*
Wifi Intermediate Driver DLL for Windows Mobile

Copyright (c) 2008, Julien Cayzac.

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

This small code shows how to enumerated WLAN access points and packages it into a DLL.
*/
#define WINVER _WIN32_WCE
#include <ceconfig.h>
#include <windows.h>
#include <altcecrt.h>

#define WIFIINTERMEDIATEDRIVER_API __declspec(dllexport)
#include "WifiIntermediateDriver.h"
#include <ntddndis.h>
#include <iphlpapi.h>
#pragma comment(lib,"iphlpapi.lib")
#include <winioctl.h>
#include <nuiouser.h>

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

// Main class declaration
class CWifi {
public:
    CWifi(unsigned long bufferLength=16384);
    ~CWifi();
    const wchar_t* const DeviceName() const;
    int  Enumerate(TWifiInfo* out, int count);
private:
    void FindDeviceName();
private:
    wchar_t*        m_pDeviceName;
    unsigned long   m_ulBufferLength;
    unsigned char*  m_pIoCtlBuffer;
};
inline const wchar_t* const CWifi::DeviceName() const {
    return m_pDeviceName;
}

// C exports
extern "C" {
	WIFIINTERMEDIATEDRIVER_API void* WifiOpen() {
		try {
			return new CWifi();
		}
		catch(...) {
			return 0;
		}
	}
	WIFIINTERMEDIATEDRIVER_API int  WifiEnumerate(void* handle, TWifiInfo* out, int count) {
		if (!handle) return 0;
		CWifi* wifi = reinterpret_cast<CWifi*>(handle);
		try {
			return wifi->Enumerate(out,count);
		}
		catch(...) {
			return 0;
		}
	}
	WIFIINTERMEDIATEDRIVER_API void WifiClose(void* handle) {
		if (!handle) return;
		CWifi* wifi = reinterpret_cast<CWifi*>(handle);
		delete wifi;
	}
	WIFIINTERMEDIATEDRIVER_API const wchar_t* const WifiDeviceName(void* handle) {
		if (!handle) return 0;
		CWifi* wifi = reinterpret_cast<CWifi*>(handle);
		try {
			return wifi->DeviceName();
		}
		catch(...) {
			return 0;
		}
	}
}

// Main code
CWifi::CWifi(unsigned long bufferLength)
: m_pIoCtlBuffer(0)
, m_ulBufferLength(bufferLength)
, m_pDeviceName(0)
{
    m_ulBufferLength+=sizeof(NDISUIO_QUERY_OID);
    m_pIoCtlBuffer = new unsigned char[m_ulBufferLength];
    FindDeviceName();
}
CWifi::~CWifi() {
    if (m_pIoCtlBuffer) {
        delete[] m_pIoCtlBuffer;
        m_pIoCtlBuffer=0;
    }
    if (m_pDeviceName) {
        delete[] m_pDeviceName;
        m_pDeviceName=0;
    }
}
void CWifi::FindDeviceName() {
    // If we already found the device, do nothing
    if (m_pDeviceName) return;
 
    // Enumerate all network adapters
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = 0;
 
    ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO *) malloc(sizeof (IP_ADAPTER_INFO));
    if (!pAdapterInfo) return;
    if (GetAdaptersInfo(pAdapterInfo,&ulOutBufLen)==ERROR_BUFFER_OVERFLOW) {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *) malloc(ulOutBufLen);
        if (!pAdapterInfo) return;
    }
 
    if (NO_ERROR==GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) {
        pAdapter = pAdapterInfo;
        while (pAdapter) {
 
            // only consider Ethernet adapters
            if (pAdapter->Type==MIB_IF_TYPE_ETHERNET) {
 
                // Open the NDIS driver in order to make a
                // IOCTL_NDISUIO_NIC_STATISTICS ioctl.
                HANDLE ndisAccess=CreateFile(
                    TEXT("UIO1:"),
                    0,0,0,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
                    INVALID_HANDLE_VALUE
                );
                if (ndisAccess && ndisAccess!=INVALID_HANDLE_VALUE) {
                    // Allocate a new unicode string with the adapter name
                    int len=0;
                    while (pAdapter->AdapterName[len])
                        ++len;
                    wchar_t* unicodeName = new wchar_t[len+1];
                    for (int c=0; c<len; ++c)
                        unicodeName[c]=(wchar_t) pAdapter->AdapterName[c];
                    unicodeName[len]=0;
                    // Make the ioctl call
                    NIC_STATISTICS nicStatistics = {0};
                    nicStatistics.ptcDeviceName = unicodeName;
                    nicStatistics.Size = sizeof(NIC_STATISTICS);
                    DWORD dwBytesWritten = 0;
                    bool success=(DeviceIoControl(
                        ndisAccess,
                        IOCTL_NDISUIO_NIC_STATISTICS,
                        0,0,
                        &nicStatistics,sizeof(NIC_STATISTICS),
                        &dwBytesWritten,
                        0
                    )==TRUE)?true:false;
                    CloseHandle(ndisAccess);
                    // Only consider WIFI adapters
                    success=(success&&(nicStatistics.PhysicalMediaType
                                        ==NdisPhysicalMediumWirelessLan));
                    if (success) {
                        // Save the newly found device name and bail out
                        m_pDeviceName = unicodeName;
                        break;
                    }
                    else {
                        // Free the temporary string
                        delete[] unicodeName;
                    }
                }
            }
 
            // Iterate to next adapter
            pAdapter = pAdapter->Next;
        }
    }
    if (pAdapterInfo) free(pAdapterInfo);
}

int  CWifi::Enumerate(TWifiInfo* out, int count) {
	int result=0;
    if (!m_pDeviceName) return result;
    HANDLE ndisAccess=CreateFile(
        TEXT("UIO1:"),
        0,0,0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
        INVALID_HANDLE_VALUE
    );
    if (!ndisAccess || ndisAccess==INVALID_HANDLE_VALUE)
        return result;
 
    NDISUIO_QUERY_OID* query = (NDISUIO_QUERY_OID*) m_pIoCtlBuffer;
    query->Oid = OID_802_11_BSSID_LIST;
    query->ptcDeviceName = m_pDeviceName;
 
    DWORD dwBytesWritten = 0;
    if (DeviceIoControl(ndisAccess,IOCTL_NDISUIO_QUERY_OID_VALUE,query,m_ulBufferLength,query,m_ulBufferLength,&dwBytesWritten,0)) {
        NDIS_802_11_BSSID_LIST* list;
        list = (NDIS_802_11_BSSID_LIST*)(&query->Data[0]);
        unsigned long numberOfItems = list->NumberOfItems;
		if (numberOfItems>0) {
			NDIS_WLAN_BSSID* ap = &list->Bssid[0];
			for (result=0; result<count; ++result) {
				if (ap->InfrastructureMode==Ndis802_11Infrastructure) {
					int dBm = ap->Rssi;
					if (dBm>0) dBm=-dBm;
					int asu2 = dBm+113;
					if (asu2<0) asu2=0;
					out[result][0] = (unsigned char)asu2;
					for (int c=0; c<6; ++c) out[result][c+2] = (unsigned char) ap->MacAddress[c];
				}
				if (!--numberOfItems) break;
				ap=(NDIS_WLAN_BSSID*)((unsigned char*)ap+ap->Length);
			}
        }
    }
	
	// Init a new scan
    NDISUIO_SET_OID* setoid = (NDISUIO_SET_OID*) m_pIoCtlBuffer;
    setoid->Oid = OID_802_11_BSSID_LIST_SCAN;
    setoid->ptcDeviceName = m_pDeviceName;
 
    dwBytesWritten = 0;
    DeviceIoControl(ndisAccess,IOCTL_NDISUIO_SET_OID_VALUE,setoid,m_ulBufferLength,setoid,m_ulBufferLength,&dwBytesWritten,0);
    CloseHandle(ndisAccess);

    return result;
}
