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
*/
using System;

namespace WifiDLLExampleUse
{
        public class Utils
        {
            public Utils()
            {
            }

            public static IntPtr LocalAlloc(int byteCount)
            {
                IntPtr ptr = Win32.LocalAlloc(Win32.LMEM_ZEROINIT, byteCount);
                if (ptr == IntPtr.Zero)
                {
                    throw new OutOfMemoryException();
                }

                return ptr;
            }

            public static void LocalFree(IntPtr hMem)
            {
                IntPtr ptr = Win32.LocalFree(hMem);
                if (ptr != IntPtr.Zero)
                {
                    throw new ArgumentException();
                }
            }
        }

        public class Win32
        {
            public const int LMEM_ZEROINIT = 0x40;
            [System.Runtime.InteropServices.DllImport("coredll.dll", EntryPoint = "#33", SetLastError = true)]
            public static extern IntPtr LocalAlloc(int flags, int byteCount);

            [System.Runtime.InteropServices.DllImport("coredll.dll", EntryPoint = "#36", SetLastError = true)]
            public static extern IntPtr LocalFree(IntPtr hMem);
        }
}
