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
#ifndef WIFIINTERMEDIATEDRIVER_INCLUDED
#define WIFIINTERMEDIATEDRIVER_INCLUDED

#ifndef WIFIINTERMEDIATEDRIVER_API
#ifdef __cplusplus
#define WIFIINTERMEDIATEDRIVER_API __declspec(dllimport)
#else
#define WIFIINTERMEDIATEDRIVER_API extern __declspec(dllimport)
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/// @brief 8 bytes describing a 802.1x infrastructure access point.
/// Byte offset/Meaning:
/// 0		Signal strength x, where x = dBm + 113
/// 1		Unused (padding)
/// 2-7	MAC address (6 bytes)
/// @note I don't use a structure for this since it's far easier to call this DLL from C# like this
typedef unsigned char TWifiInfo[8];

/// @brief Open the WIFI driver
/// @return A handle to the WIFI device on success, or a null pointer otherwise.
WIFIINTERMEDIATEDRIVER_API void* WifiOpen();

/// @brief Get the list of currently available 802.1x infrastructure access points, and initiate an active scan.
/// @param handle	A valid WIFI device handle.
/// @param out	A pointer to a buffer with at least (count*sizeof(TWifiInfo)) bytes.
/// @param count	Maximum access points count supported by our buffer.
/// @return The number of returned 802.1x infrastructure access points.
WIFIINTERMEDIATEDRIVER_API int   WifiEnumerate(void* handle, TWifiInfo* out, int count);

/// @brief Close a previously opened WIFI device handle.
/// @param handle	A valid WIFI device handle.
WIFIINTERMEDIATEDRIVER_API void  WifiClose(void* handle);

/// @brief Return the low level Win32 device name for our WLAN adapter.
/// @param handle	A valid WIFI device handle.
/// @return The device name. Note that this pointer is only valid while the handle is in an open state.
WIFIINTERMEDIATEDRIVER_API const wchar_t* const WifiDeviceName(void* handle);

#ifdef __cplusplus
}
#endif

#endif
