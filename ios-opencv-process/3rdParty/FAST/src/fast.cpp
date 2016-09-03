#include <stdlib.h>
#include "fast.h"

FASTMem gFASTMem;

xy* fast9_detect_nonmax(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners)
{
	int num_corners;
	xy* corners = fast9_detect(im, xsize, ysize, stride, b, &num_corners);
	int* scores = fast9_score(im, stride, corners, num_corners, b);
	xy* nonmax = nonmax_suppression(corners, scores, num_corners, ret_num_corners);
	return nonmax;
}
