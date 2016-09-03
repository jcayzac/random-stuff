/** Fast function to linearly downscale a 640x480 BGRA image to 320x240, and
  * convert it to luminance map on the fly.
  *
  * @param destination_buffer 32-bit aligned memory storage for the luminance map.
  *                           It must be at least 320*240 bytes long.
  *                           This function assumes 320 bytes per row.
  *
  * @param source_data 32-bit aligned memory pointer for the source image data.
  *                    This function assumes 640*4 bytes per row.
  *
  * Luminance is approximated as (Red + Green*2 + Blue)/4.
  *
  */
static void fast_640x480xBGRA_to_320x240xY(void* destination_buffer, const void* source_data) {
	const unsigned int* __restrict__ bgraValues((const unsigned int*) source_data);
	unsigned int* __restrict__ greyValues((unsigned int*) destination_buffer);
	unsigned int off1(0);
	for (unsigned int y(0); y<240; ++y, greyValues+=80) {
		unsigned int off2(off1+640);
		for (unsigned int x=0; x<80; ++x) {
			unsigned int L1(0), L2(0), L3(0), L4(0);
			// 1st line
			L1+=(bgraValues[off1]&0xff)+(((bgraValues[off1]>>8)&0xff)<<1)+((bgraValues[off1]>>16)&0xff); ++off1;
			L1+=(bgraValues[off1]&0xff)+(((bgraValues[off1]>>8)&0xff)<<1)+((bgraValues[off1]>>16)&0xff); ++off1;
			L2+=(bgraValues[off1]&0xff)+(((bgraValues[off1]>>8)&0xff)<<1)+((bgraValues[off1]>>16)&0xff); ++off1;
			L2+=(bgraValues[off1]&0xff)+(((bgraValues[off1]>>8)&0xff)<<1)+((bgraValues[off1]>>16)&0xff); ++off1;
			L3+=(bgraValues[off1]&0xff)+(((bgraValues[off1]>>8)&0xff)<<1)+((bgraValues[off1]>>16)&0xff); ++off1;
			L3+=(bgraValues[off1]&0xff)+(((bgraValues[off1]>>8)&0xff)<<1)+((bgraValues[off1]>>16)&0xff); ++off1;
			L4+=(bgraValues[off1]&0xff)+(((bgraValues[off1]>>8)&0xff)<<1)+((bgraValues[off1]>>16)&0xff); ++off1;
			L4+=(bgraValues[off1]&0xff)+(((bgraValues[off1]>>8)&0xff)<<1)+((bgraValues[off1]>>16)&0xff); ++off1;
			// 2nd line
			L1+=(bgraValues[off2]&0xff)+(((bgraValues[off2]>>8)&0xff)<<1)+((bgraValues[off2]>>16)&0xff); ++off2;
			L1+=(bgraValues[off2]&0xff)+(((bgraValues[off2]>>8)&0xff)<<1)+((bgraValues[off2]>>16)&0xff); ++off2;
			L2+=(bgraValues[off2]&0xff)+(((bgraValues[off2]>>8)&0xff)<<1)+((bgraValues[off2]>>16)&0xff); ++off2;
			L2+=(bgraValues[off2]&0xff)+(((bgraValues[off2]>>8)&0xff)<<1)+((bgraValues[off2]>>16)&0xff); ++off2;
			L3+=(bgraValues[off2]&0xff)+(((bgraValues[off2]>>8)&0xff)<<1)+((bgraValues[off2]>>16)&0xff); ++off2;
			L3+=(bgraValues[off2]&0xff)+(((bgraValues[off2]>>8)&0xff)<<1)+((bgraValues[off2]>>16)&0xff); ++off2;
			L4+=(bgraValues[off2]&0xff)+(((bgraValues[off2]>>8)&0xff)<<1)+((bgraValues[off2]>>16)&0xff); ++off2;
			L4+=(bgraValues[off2]&0xff)+(((bgraValues[off2]>>8)&0xff)<<1)+((bgraValues[off2]>>16)&0xff); ++off2;
			greyValues[x]=((L4<<20)&0xff000000) | ((L3<<12)&0xff0000) | ((L2<<4)&0xff00) | ((L1>>4)&0xff);
		}
		off1=off2;
	}
}

