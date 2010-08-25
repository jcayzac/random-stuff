using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.IO;
using System.Diagnostics;

namespace gsm2geo
{
    class Program
    {
        static byte[] PostData(int MCC, int MNC, int LAC, int CID)
        {
            byte[] pd = new byte[] {
                0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
                0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00
            };

            // GSM: 4 hex digits, UTMS: 6 hex digits
            bool isUMTSCell = ((Int64)CID > 0xffff);
            if (isUMTSCell)
                pd[0x1c] = 5;
            else
                pd[0x1c] = 3;

            pd[0x11] = (byte)((MNC>>24) & 0xFF);
            pd[0x12] = (byte)((MNC>>16) & 0xFF);
            pd[0x13] = (byte)((MNC>> 8) & 0xFF);
            pd[0x14] = (byte)( MNC      & 0xFF);

            pd[0x15] = (byte)((MCC>>24) & 0xFF);
            pd[0x16] = (byte)((MCC>>16) & 0xFF);
            pd[0x17] = (byte)((MCC>> 8) & 0xFF);
            pd[0x18] = (byte)( MCC      & 0xFF);

            pd[0x27] = (byte)((MNC>>24) & 0xFF);
            pd[0x28] = (byte)((MNC>>16) & 0xFF);
            pd[0x29] = (byte)((MNC>> 8) & 0xFF);
            pd[0x2a] = (byte)( MNC      & 0xFF);

            pd[0x2b] = (byte)((MCC>>24) & 0xFF);
            pd[0x2c] = (byte)((MCC>>16) & 0xFF);
            pd[0x2d] = (byte)((MCC>> 8) & 0xFF);
            pd[0x2e] = (byte)( MCC      & 0xFF);

            pd[0x1f] = (byte)((CID>>24) & 0xFF);
            pd[0x20] = (byte)((CID>>16) & 0xFF);
            pd[0x21] = (byte)((CID>> 8) & 0xFF);
            pd[0x22] = (byte)( CID      & 0xFF);

            pd[0x23] = (byte)((LAC>>24) & 0xFF);
            pd[0x24] = (byte)((LAC>>16) & 0xFF);
            pd[0x25] = (byte)((LAC>> 8) & 0xFF);
            pd[0x26] = (byte)( LAC      & 0xFF);

            return pd;
        }

        static bool GeoCodeZone(int mcc, int mnc, int lac, int cid, ref double lon, ref double lat, ref int range, ref int dBm)
        {
            String url = "http://www.google.com/glm/mmap";
            HttpWebRequest req = (HttpWebRequest)WebRequest.Create(new Uri(url));
            req.Method = "POST";
            byte[] pd = PostData(mcc, mnc, lac, cid);
            req.ContentLength = pd.Length;
            req.ContentType = "application/binary";
            Stream outputStream = req.GetRequestStream();
            outputStream.Write(pd, 0, pd.Length);
            outputStream.Close();
            HttpWebResponse res = (HttpWebResponse)req.GetResponse();
            byte[] ps = new byte[res.ContentLength];
            int totalBytesRead = 0;
            while (totalBytesRead < ps.Length)
            {
                totalBytesRead += res.GetResponseStream().Read(ps, totalBytesRead, ps.Length - totalBytesRead);
            }

            if (res.StatusCode == HttpStatusCode.OK)
            {
                short opcode1 = (short)(ps[0] << 8 | ps[1]);
                byte opcode2 = ps[2];
                System.Diagnostics.Debug.Assert(opcode1 == 0x0e);
                System.Diagnostics.Debug.Assert(opcode2 == 0x1b);
                int ret_code = (int)((ps[3] << 24) | (ps[4] << 16) | (ps[5] << 8) | (ps[6]));

                if (ret_code == 0)
                {
                    lon = ((double)((ps[11] << 24) | (ps[12] << 16) | (ps[13] << 8) | (ps[14]))) / 1000000;
                    lat = ((double)((ps[7] << 24) | (ps[8] << 16) | (ps[9] << 8) | (ps[10]))) / 1000000;
                    range = ((int)((ps[15] << 24) | (ps[16] << 16) | (ps[17] << 8) | (ps[18])));
                    dBm = ((int)((ps[19] << 24) | (ps[20] << 16) | (ps[21] << 8) | (ps[22])));
                    return true;
                }
            }
            return false;
        }

        static void Main(string[] args)
        {
            if (args.Length < 4)
            {
                Console.WriteLine("Usage: gsm2geo <MCC> <MNC> <LAC> <CID>");
                return;
            }

            int MCC = Convert.ToInt32(args[0]);
            int MNC = Convert.ToInt32(args[1]);
            int LAC = Convert.ToInt32(args[2]);
            int CID = Convert.ToInt32(args[3]);
            double[] position = new double[2];
            int range = 0;
            int dBm = 0;
            if (GeoCodeZone(MCC, MNC, LAC, CID, ref position[0], ref position[1], ref range, ref dBm))
            {
                Console.WriteLine("Position=({0},{1}), Range={2}m, Brss=-{3}dBm", position[0], position[1], range, dBm);
            }
        }
    }
}
