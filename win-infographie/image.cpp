#include <iostream>
#include <fstream>
#include <strstream>
#include "image.h"

using namespace std;




////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

bool CImage::readTGA(string filename) {

	ifstream ifs(filename.c_str(),ios::binary);
	// header

	uint8 header[18];
	ifs.get((char*)header,18);
	if ((header[2]!=0x02) && (header[2]!=0x0a)) return false;

	bool rle = (header[2]==0x0a);

	uint16 newwidth  = header[12] + (header[13]<<8);
	uint16 newheight = header[14] + (header[15]<<8);
	if (header[16]!=0x18) return false;
	bool fromtop = (header[17]&0x20!=0);
	// BGR data
	uint32 sz		= newwidth*newheight;
	uint32 pixel	= 0;
	
	width  = newwidth;
	height = newheight;
	data = new CRGBColor[sz];

	char* tmp = new char[sz*3];
	ifs.seekg(18);
	ifs.read(tmp, sz*3);

	if (rle) {
		// to be continue
		
	} else {
		while (pixel<sz) {
			int chat = pixel*3;
			data[pixel].set(tmp[chat+2],tmp[chat+1],tmp[chat]);
			pixel++;
		}
	}
	return true;
}



////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

bool CImage::writeTGA(string filename) {

	bool rle=false;
	uint32 w = GetWidth();
	uint32 h = GetHeight();

	ofstream ofs(filename.c_str(),ios::binary);
	ofs << (uint8)0x00;
	ofs << (uint8)0x00;

	// RLE not supported
	ofs << (uint8)(rle?0x02:0x02);
	
	for (int i=3;i<12;i++) ofs << (uint8)0x00;
	ofs << (uint8)(w&0xff);
	ofs << (uint8)(w/256);
	ofs << (uint8)(h&0xff);
	ofs << (uint8)(h/256);
	ofs << (uint8)0x18;
	ofs << (uint8)0x20;
	
	// Write BGR information
	uint32 sz    = w*h;
	uint32 pixel = 0;

	CRGBColor *buffer = GetBuffer();

	char* tmp = new char[sz*3];
	while (pixel<sz) {
		int chat = pixel*3;
		tmp[chat+0] = data[pixel].getBlue(); 
		tmp[chat+1] = data[pixel].getGreen();
		tmp[chat+2] = data[pixel].getRed();
		pixel++;
	}
	
	if (rle) {
		// to be continued....	

	} else {
		ofs.seekp(18);
		ofs.write(tmp,sz*3); 
		ofs.seekp(18+sz*3);

		uint8 c = 0;
		ofs << c;
		ofs << c;
		ofs << c;
		ofs << c;
		ofs << c;
		ofs << c;
		ofs << c;
		ofs << c;
		ofs << "TRUEVISION-XFILE.";
		ofs << c;
	}
	ofs.flush();
	return true;
}