/*
Example use of the Wifi intermediate driver DLL for Windows Mobile from managed code.

Copyright (c) 2008, Julien Cayzac.
http://julien.cayzac.name/

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

Usage:
Wifi wifi = new Wifi();
wifi.Open()
Queue<WifiSignal> signals = wifi.GetWifiSignals();
... do something with signals ...

*/
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace WifiDLLExampleUse
{
    public class WifiSignal
    {
        int     mSignalStrength;
        public int SignalStrength
        {
            get { return mSignalStrength; }
            set { mSignalStrength=value; }
        }
        byte[]  mHMAC = new byte[6];
        public byte[] HMAC
        {
            get { return mHMAC; }
            set {
                if (value.Length != 6)
                    throw new System.NotSupportedException("HMAC addresses are 6-byte long!");
                mHMAC = value;
            }
        }
        public WifiSignal()
        {
            SignalStrength = 0;
            mHMAC[0] = mHMAC[1] = mHMAC[2] = mHMAC[3] = mHMAC[4] = mHMAC[5] = 0;
        }
        public WifiSignal(int ss, byte[] mac)
        {
            SignalStrength = ss;
            HMAC = mac;
        }
    }
    public class Wifi
    {
        private IntPtr mHandle = IntPtr.Zero;

        public bool Opened
        {
            get { return mHandle != IntPtr.Zero; }
        }
        
        public Wifi() { }
        ~Wifi()
        {
            Close();
        }

        public void Open()
        {
            if (!Opened)
            {
                mHandle = WifiOpen();
            }
        }
        public void Close()
        {
            if (Opened)
            {
                WifiClose(mHandle);
                mHandle = IntPtr.Zero;
            }
        }

        public string DeviceName()
        {
            if (!Opened) return null;
            return WifiDeviceName(mHandle);
        }

        public Queue<WifiSignal> GetWifiSignals()
        {
            Queue<WifiSignal> queue = new Queue<WifiSignal>();
            if (Opened)
            {
                IntPtr buffer = Utils.LocalAlloc(800);
                byte[] tmpMAC = new byte[6];
                int count = WifiEnumerate(mHandle, buffer, 100);
                for (int i = 0; i < count; ++i)
                {
                    int ss = (int)Marshal.ReadByte(buffer, 0);
                    for (int c = 0; c < 6; ++c)
                        tmpMAC[c] = Marshal.ReadByte(buffer, c + 2);
                    queue.Enqueue(new WifiSignal(ss, tmpMAC));
                }
                Utils.LocalFree(buffer);
            }
            return queue;
        }

        // Unmanaged code
        [DllImport("WifiIntermediateDriver.dll", CharSet = CharSet.Auto)]
        private static extern IntPtr WifiOpen();
        [DllImport("WifiIntermediateDriver.dll", CharSet = CharSet.Auto)]
        private static extern int WifiEnumerate(IntPtr handle, IntPtr result, int count);
        [DllImport("WifiIntermediateDriver.dll", CharSet = CharSet.Auto)]
        private static extern void WifiClose(IntPtr handle);
        [DllImport("WifiIntermediateDriver.dll", CharSet = CharSet.Auto)]
        private static extern string WifiDeviceName(IntPtr handle);
    }
}
