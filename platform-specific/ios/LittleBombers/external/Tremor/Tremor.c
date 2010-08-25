/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggVorbis 'TREMOR' CODEC SOURCE CODE.   *
 *                                                                  *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE OggVorbis 'TREMOR' SOURCE CODE IS (C) COPYRIGHT 1994-2002    *
 * BY THE Xiph.Org FOUNDATION http://www.xiph.org/                  *
 *                                                                  *
 ********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <ctype.h>
#include <Tremor/Tremor.h>

#define VI_TRANSFORMB 1
#define VI_WINDOWB 1
#define VI_TIMEB 1
#define VI_FLOORB 2
#define VI_RESB 3
#define VI_MAPB 1

#ifndef _V_IFDEFJAIL_H_
#  define _V_IFDEFJAIL_H_
#  ifdef __GNUC__
#    define STIN static __inline__
#  elif _WIN32
#    define STIN static __inline
#  endif
#else
#  define STIN static
#endif

#ifndef M_PI
#  define M_PI (3.1415926536f)
#endif

#ifdef _WIN32
#  include <malloc.h>
#  define rint(x)   (floor((x)+0.5f)) 
#  define NO_FLOAT_MATH_LIB
#  define FAST_HYPOT(a, b) sqrt((a)*(a) + (b)*(b))
#  define LITTLE_ENDIAN 1
#  define BYTE_ORDER LITTLE_ENDIAN
#endif

#ifdef HAVE_ALLOCA_H
#  include <alloca.h>
#endif

#ifdef USE_MEMORY_H
#  include <memory.h>
#endif

#ifndef min
#  define min(x,y)  ((x)>(y)?(y):(x))
#endif

#ifndef max
#  define max(x,y)  ((x)<(y)?(y):(x))
#endif

#define FROMdB_LOOKUP_SZ 35
#define FROMdB2_LOOKUP_SZ 32
#define FROMdB_SHIFT 5
#define FROMdB2_SHIFT 3
#define FROMdB2_MASK 31

static const ogg_int32_t FROMdB_LOOKUP[FROMdB_LOOKUP_SZ]={
0x003fffff, 0x0028619b, 0x00197a96, 0x0010137a,
0x000a24b0, 0x00066666, 0x000409c3, 0x00028c42,
0x00019b8c, 0x000103ab, 0x0000a3d7, 0x00006760,
0x0000413a, 0x00002928, 0x000019f8, 0x00001062,
0x00000a56, 0x00000686, 0x0000041e, 0x00000299,
0x000001a3, 0x00000109, 0x000000a7, 0x00000069,
0x00000042, 0x0000002a, 0x0000001a, 0x00000011,
0x0000000b, 0x00000007, 0x00000004, 0x00000003,
0x00000002, 0x00000001, 0x00000001};

static const ogg_int32_t FROMdB2_LOOKUP[FROMdB2_LOOKUP_SZ]={
0x000001fc, 0x000001f5, 0x000001ee, 0x000001e7,
0x000001e0, 0x000001d9, 0x000001d2, 0x000001cc,
0x000001c5, 0x000001bf, 0x000001b8, 0x000001b2,
0x000001ac, 0x000001a6, 0x000001a0, 0x0000019a,
0x00000194, 0x0000018e, 0x00000188, 0x00000183,
0x0000017d, 0x00000178, 0x00000172, 0x0000016d,
0x00000168, 0x00000163, 0x0000015e, 0x00000159,
0x00000154, 0x0000014f, 0x0000014a, 0x00000145,
};

#define INVSQ_LOOKUP_I_SHIFT 10
#define INVSQ_LOOKUP_I_MASK 1023
static const long INVSQ_LOOKUP_I[64+1]={
92682,   91966,   91267,   90583,
89915,   89261,   88621,   87995,
87381,   86781,   86192,   85616,
85051,   84497,   83953,   83420,
82897,   82384,   81880,   81385,
80899,   80422,   79953,   79492,
79039,   78594,   78156,   77726,
77302,   76885,   76475,   76072,
75674,   75283,   74898,   74519,
74146,   73778,   73415,   73058,
72706,   72359,   72016,   71679,
71347,   71019,   70695,   70376,
70061,   69750,   69444,   69141,
68842,   68548,   68256,   67969,
67685,   67405,   67128,   66855,
66585,   66318,   66054,   65794,
65536,
};

static const long INVSQ_LOOKUP_IDel[64]={
716,     699,     684,     668,
654,     640,     626,     614,
600,     589,     576,     565,
554,     544,     533,     523,
513,     504,     495,     486,
477,     469,     461,     453,
445,     438,     430,     424,
417,     410,     403,     398,
391,     385,     379,     373,
368,     363,     357,     352,
347,     343,     337,     332,
328,     324,     319,     315,
311,     306,     303,     299,
294,     292,     287,     284,
280,     277,     273,     270,
267,     264,     260,     258,
};

#define COS_LOOKUP_I_SHIFT 9
#define COS_LOOKUP_I_MASK 511
#define COS_LOOKUP_I_SZ 128
static const ogg_int32_t COS_LOOKUP_I[COS_LOOKUP_I_SZ+1]={
16384,   16379,   16364,   16340,
16305,   16261,   16207,   16143,
16069,   15986,   15893,   15791,
15679,   15557,   15426,   15286,
15137,   14978,   14811,   14635,
14449,   14256,   14053,   13842,
13623,   13395,   13160,   12916,
12665,   12406,   12140,   11866,
11585,   11297,   11003,   10702,
10394,   10080,    9760,    9434,
9102,    8765,    8423,    8076,
7723,    7366,    7005,    6639,
6270,    5897,    5520,    5139,
4756,    4370,    3981,    3590,
3196,    2801,    2404,    2006,
1606,    1205,     804,     402,
0,    -401,    -803,   -1204,
-1605,   -2005,   -2403,   -2800,
-3195,   -3589,   -3980,   -4369,
-4755,   -5138,   -5519,   -5896,
-6269,   -6638,   -7004,   -7365,
-7722,   -8075,   -8422,   -8764,
-9101,   -9433,   -9759,  -10079,
-10393,  -10701,  -11002,  -11296,
-11584,  -11865,  -12139,  -12405,
-12664,  -12915,  -13159,  -13394,
-13622,  -13841,  -14052,  -14255,
-14448,  -14634,  -14810,  -14977,
-15136,  -15285,  -15425,  -15556,
-15678,  -15790,  -15892,  -15985,
-16068,  -16142,  -16206,  -16260,
-16304,  -16339,  -16363,  -16378,
-16383,
};

/* {sin(2*i*PI/4096), cos(2*i*PI/4096)}, with i = 0 to 512 */
static LOOKUP_T sincos_lookup0[1026] = {
X(0x00000000), X(0x7fffffff), X(0x003243f5), X(0x7ffff621),
X(0x006487e3), X(0x7fffd886), X(0x0096cbc1), X(0x7fffa72c),
X(0x00c90f88), X(0x7fff6216), X(0x00fb5330), X(0x7fff0943),
X(0x012d96b1), X(0x7ffe9cb2), X(0x015fda03), X(0x7ffe1c65),
X(0x01921d20), X(0x7ffd885a), X(0x01c45ffe), X(0x7ffce093),
X(0x01f6a297), X(0x7ffc250f), X(0x0228e4e2), X(0x7ffb55ce),
X(0x025b26d7), X(0x7ffa72d1), X(0x028d6870), X(0x7ff97c18),
X(0x02bfa9a4), X(0x7ff871a2), X(0x02f1ea6c), X(0x7ff75370),
X(0x03242abf), X(0x7ff62182), X(0x03566a96), X(0x7ff4dbd9),
X(0x0388a9ea), X(0x7ff38274), X(0x03bae8b2), X(0x7ff21553),
X(0x03ed26e6), X(0x7ff09478), X(0x041f6480), X(0x7feeffe1),
X(0x0451a177), X(0x7fed5791), X(0x0483ddc3), X(0x7feb9b85),
X(0x04b6195d), X(0x7fe9cbc0), X(0x04e8543e), X(0x7fe7e841),
X(0x051a8e5c), X(0x7fe5f108), X(0x054cc7b1), X(0x7fe3e616),
X(0x057f0035), X(0x7fe1c76b), X(0x05b137df), X(0x7fdf9508),
X(0x05e36ea9), X(0x7fdd4eec), X(0x0615a48b), X(0x7fdaf519),
X(0x0647d97c), X(0x7fd8878e), X(0x067a0d76), X(0x7fd6064c),
X(0x06ac406f), X(0x7fd37153), X(0x06de7262), X(0x7fd0c8a3),
X(0x0710a345), X(0x7fce0c3e), X(0x0742d311), X(0x7fcb3c23),
X(0x077501be), X(0x7fc85854), X(0x07a72f45), X(0x7fc560cf),
X(0x07d95b9e), X(0x7fc25596), X(0x080b86c2), X(0x7fbf36aa),
X(0x083db0a7), X(0x7fbc040a), X(0x086fd947), X(0x7fb8bdb8),
X(0x08a2009a), X(0x7fb563b3), X(0x08d42699), X(0x7fb1f5fc),
X(0x09064b3a), X(0x7fae7495), X(0x09386e78), X(0x7faadf7c),
X(0x096a9049), X(0x7fa736b4), X(0x099cb0a7), X(0x7fa37a3c),
X(0x09cecf89), X(0x7f9faa15), X(0x0a00ece8), X(0x7f9bc640),
X(0x0a3308bd), X(0x7f97cebd), X(0x0a6522fe), X(0x7f93c38c),
X(0x0a973ba5), X(0x7f8fa4b0), X(0x0ac952aa), X(0x7f8b7227),
X(0x0afb6805), X(0x7f872bf3), X(0x0b2d7baf), X(0x7f82d214),
X(0x0b5f8d9f), X(0x7f7e648c), X(0x0b919dcf), X(0x7f79e35a),
X(0x0bc3ac35), X(0x7f754e80), X(0x0bf5b8cb), X(0x7f70a5fe),
X(0x0c27c389), X(0x7f6be9d4), X(0x0c59cc68), X(0x7f671a05),
X(0x0c8bd35e), X(0x7f62368f), X(0x0cbdd865), X(0x7f5d3f75),
X(0x0cefdb76), X(0x7f5834b7), X(0x0d21dc87), X(0x7f531655),
X(0x0d53db92), X(0x7f4de451), X(0x0d85d88f), X(0x7f489eaa),
X(0x0db7d376), X(0x7f434563), X(0x0de9cc40), X(0x7f3dd87c),
X(0x0e1bc2e4), X(0x7f3857f6), X(0x0e4db75b), X(0x7f32c3d1),
X(0x0e7fa99e), X(0x7f2d1c0e), X(0x0eb199a4), X(0x7f2760af),
X(0x0ee38766), X(0x7f2191b4), X(0x0f1572dc), X(0x7f1baf1e),
X(0x0f475bff), X(0x7f15b8ee), X(0x0f7942c7), X(0x7f0faf25),
X(0x0fab272b), X(0x7f0991c4), X(0x0fdd0926), X(0x7f0360cb),
X(0x100ee8ad), X(0x7efd1c3c), X(0x1040c5bb), X(0x7ef6c418),
X(0x1072a048), X(0x7ef05860), X(0x10a4784b), X(0x7ee9d914),
X(0x10d64dbd), X(0x7ee34636), X(0x11082096), X(0x7edc9fc6),
X(0x1139f0cf), X(0x7ed5e5c6), X(0x116bbe60), X(0x7ecf1837),
X(0x119d8941), X(0x7ec8371a), X(0x11cf516a), X(0x7ec14270),
X(0x120116d5), X(0x7eba3a39), X(0x1232d979), X(0x7eb31e78),
X(0x1264994e), X(0x7eabef2c), X(0x1296564d), X(0x7ea4ac58),
X(0x12c8106f), X(0x7e9d55fc), X(0x12f9c7aa), X(0x7e95ec1a),
X(0x132b7bf9), X(0x7e8e6eb2), X(0x135d2d53), X(0x7e86ddc6),
X(0x138edbb1), X(0x7e7f3957), X(0x13c0870a), X(0x7e778166),
X(0x13f22f58), X(0x7e6fb5f4), X(0x1423d492), X(0x7e67d703),
X(0x145576b1), X(0x7e5fe493), X(0x148715ae), X(0x7e57dea7),
X(0x14b8b17f), X(0x7e4fc53e), X(0x14ea4a1f), X(0x7e47985b),
X(0x151bdf86), X(0x7e3f57ff), X(0x154d71aa), X(0x7e37042a),
X(0x157f0086), X(0x7e2e9cdf), X(0x15b08c12), X(0x7e26221f),
X(0x15e21445), X(0x7e1d93ea), X(0x16139918), X(0x7e14f242),
X(0x16451a83), X(0x7e0c3d29), X(0x1676987f), X(0x7e0374a0),
X(0x16a81305), X(0x7dfa98a8), X(0x16d98a0c), X(0x7df1a942),
X(0x170afd8d), X(0x7de8a670), X(0x173c6d80), X(0x7ddf9034),
X(0x176dd9de), X(0x7dd6668f), X(0x179f429f), X(0x7dcd2981),
X(0x17d0a7bc), X(0x7dc3d90d), X(0x1802092c), X(0x7dba7534),
X(0x183366e9), X(0x7db0fdf8), X(0x1864c0ea), X(0x7da77359),
X(0x18961728), X(0x7d9dd55a), X(0x18c7699b), X(0x7d9423fc),
X(0x18f8b83c), X(0x7d8a5f40), X(0x192a0304), X(0x7d808728),
X(0x195b49ea), X(0x7d769bb5), X(0x198c8ce7), X(0x7d6c9ce9),
X(0x19bdcbf3), X(0x7d628ac6), X(0x19ef0707), X(0x7d58654d),
X(0x1a203e1b), X(0x7d4e2c7f), X(0x1a517128), X(0x7d43e05e),
X(0x1a82a026), X(0x7d3980ec), X(0x1ab3cb0d), X(0x7d2f0e2b),
X(0x1ae4f1d6), X(0x7d24881b), X(0x1b161479), X(0x7d19eebf),
X(0x1b4732ef), X(0x7d0f4218), X(0x1b784d30), X(0x7d048228),
X(0x1ba96335), X(0x7cf9aef0), X(0x1bda74f6), X(0x7ceec873),
X(0x1c0b826a), X(0x7ce3ceb2), X(0x1c3c8b8c), X(0x7cd8c1ae),
X(0x1c6d9053), X(0x7ccda169), X(0x1c9e90b8), X(0x7cc26de5),
X(0x1ccf8cb3), X(0x7cb72724), X(0x1d00843d), X(0x7cabcd28),
X(0x1d31774d), X(0x7ca05ff1), X(0x1d6265dd), X(0x7c94df83),
X(0x1d934fe5), X(0x7c894bde), X(0x1dc4355e), X(0x7c7da505),
X(0x1df5163f), X(0x7c71eaf9), X(0x1e25f282), X(0x7c661dbc),
X(0x1e56ca1e), X(0x7c5a3d50), X(0x1e879d0d), X(0x7c4e49b7),
X(0x1eb86b46), X(0x7c4242f2), X(0x1ee934c3), X(0x7c362904),
X(0x1f19f97b), X(0x7c29fbee), X(0x1f4ab968), X(0x7c1dbbb3),
X(0x1f7b7481), X(0x7c116853), X(0x1fac2abf), X(0x7c0501d2),
X(0x1fdcdc1b), X(0x7bf88830), X(0x200d888d), X(0x7bebfb70),
X(0x203e300d), X(0x7bdf5b94), X(0x206ed295), X(0x7bd2a89e),
X(0x209f701c), X(0x7bc5e290), X(0x20d0089c), X(0x7bb9096b),
X(0x21009c0c), X(0x7bac1d31), X(0x21312a65), X(0x7b9f1de6),
X(0x2161b3a0), X(0x7b920b89), X(0x219237b5), X(0x7b84e61f),
X(0x21c2b69c), X(0x7b77ada8), X(0x21f3304f), X(0x7b6a6227),
X(0x2223a4c5), X(0x7b5d039e), X(0x225413f8), X(0x7b4f920e),
X(0x22847de0), X(0x7b420d7a), X(0x22b4e274), X(0x7b3475e5),
X(0x22e541af), X(0x7b26cb4f), X(0x23159b88), X(0x7b190dbc),
X(0x2345eff8), X(0x7b0b3d2c), X(0x23763ef7), X(0x7afd59a4),
X(0x23a6887f), X(0x7aef6323), X(0x23d6cc87), X(0x7ae159ae),
X(0x24070b08), X(0x7ad33d45), X(0x243743fa), X(0x7ac50dec),
X(0x24677758), X(0x7ab6cba4), X(0x2497a517), X(0x7aa8766f),
X(0x24c7cd33), X(0x7a9a0e50), X(0x24f7efa2), X(0x7a8b9348),
X(0x25280c5e), X(0x7a7d055b), X(0x2558235f), X(0x7a6e648a),
X(0x2588349d), X(0x7a5fb0d8), X(0x25b84012), X(0x7a50ea47),
X(0x25e845b6), X(0x7a4210d8), X(0x26184581), X(0x7a332490),
X(0x26483f6c), X(0x7a24256f), X(0x26783370), X(0x7a151378),
X(0x26a82186), X(0x7a05eead), X(0x26d809a5), X(0x79f6b711),
X(0x2707ebc7), X(0x79e76ca7), X(0x2737c7e3), X(0x79d80f6f),
X(0x27679df4), X(0x79c89f6e), X(0x27976df1), X(0x79b91ca4),
X(0x27c737d3), X(0x79a98715), X(0x27f6fb92), X(0x7999dec4),
X(0x2826b928), X(0x798a23b1), X(0x2856708d), X(0x797a55e0),
X(0x288621b9), X(0x796a7554), X(0x28b5cca5), X(0x795a820e),
X(0x28e5714b), X(0x794a7c12), X(0x29150fa1), X(0x793a6361),
X(0x2944a7a2), X(0x792a37fe), X(0x29743946), X(0x7919f9ec),
X(0x29a3c485), X(0x7909a92d), X(0x29d34958), X(0x78f945c3),
X(0x2a02c7b8), X(0x78e8cfb2), X(0x2a323f9e), X(0x78d846fb),
X(0x2a61b101), X(0x78c7aba2), X(0x2a911bdc), X(0x78b6fda8),
X(0x2ac08026), X(0x78a63d11), X(0x2aefddd8), X(0x789569df),
X(0x2b1f34eb), X(0x78848414), X(0x2b4e8558), X(0x78738bb3),
X(0x2b7dcf17), X(0x786280bf), X(0x2bad1221), X(0x7851633b),
X(0x2bdc4e6f), X(0x78403329), X(0x2c0b83fa), X(0x782ef08b),
X(0x2c3ab2b9), X(0x781d9b65), X(0x2c69daa6), X(0x780c33b8),
X(0x2c98fbba), X(0x77fab989), X(0x2cc815ee), X(0x77e92cd9),
X(0x2cf72939), X(0x77d78daa), X(0x2d263596), X(0x77c5dc01),
X(0x2d553afc), X(0x77b417df), X(0x2d843964), X(0x77a24148),
X(0x2db330c7), X(0x7790583e), X(0x2de2211e), X(0x777e5cc3),
X(0x2e110a62), X(0x776c4edb), X(0x2e3fec8b), X(0x775a2e89),
X(0x2e6ec792), X(0x7747fbce), X(0x2e9d9b70), X(0x7735b6af),
X(0x2ecc681e), X(0x77235f2d), X(0x2efb2d95), X(0x7710f54c),
X(0x2f29ebcc), X(0x76fe790e), X(0x2f58a2be), X(0x76ebea77),
X(0x2f875262), X(0x76d94989), X(0x2fb5fab2), X(0x76c69647),
X(0x2fe49ba7), X(0x76b3d0b4), X(0x30133539), X(0x76a0f8d2),
X(0x3041c761), X(0x768e0ea6), X(0x30705217), X(0x767b1231),
X(0x309ed556), X(0x76680376), X(0x30cd5115), X(0x7654e279),
X(0x30fbc54d), X(0x7641af3d), X(0x312a31f8), X(0x762e69c4),
X(0x3158970e), X(0x761b1211), X(0x3186f487), X(0x7607a828),
X(0x31b54a5e), X(0x75f42c0b), X(0x31e39889), X(0x75e09dbd),
X(0x3211df04), X(0x75ccfd42), X(0x32401dc6), X(0x75b94a9c),
X(0x326e54c7), X(0x75a585cf), X(0x329c8402), X(0x7591aedd),
X(0x32caab6f), X(0x757dc5ca), X(0x32f8cb07), X(0x7569ca99),
X(0x3326e2c3), X(0x7555bd4c), X(0x3354f29b), X(0x75419de7),
X(0x3382fa88), X(0x752d6c6c), X(0x33b0fa84), X(0x751928e0),
X(0x33def287), X(0x7504d345), X(0x340ce28b), X(0x74f06b9e),
X(0x343aca87), X(0x74dbf1ef), X(0x3468aa76), X(0x74c7663a),
X(0x34968250), X(0x74b2c884), X(0x34c4520d), X(0x749e18cd),
X(0x34f219a8), X(0x7489571c), X(0x351fd918), X(0x74748371),
X(0x354d9057), X(0x745f9dd1), X(0x357b3f5d), X(0x744aa63f),
X(0x35a8e625), X(0x74359cbd), X(0x35d684a6), X(0x74208150),
X(0x36041ad9), X(0x740b53fb), X(0x3631a8b8), X(0x73f614c0),
X(0x365f2e3b), X(0x73e0c3a3), X(0x368cab5c), X(0x73cb60a8),
X(0x36ba2014), X(0x73b5ebd1), X(0x36e78c5b), X(0x73a06522),
X(0x3714f02a), X(0x738acc9e), X(0x37424b7b), X(0x73752249),
X(0x376f9e46), X(0x735f6626), X(0x379ce885), X(0x73499838),
X(0x37ca2a30), X(0x7333b883), X(0x37f76341), X(0x731dc70a),
X(0x382493b0), X(0x7307c3d0), X(0x3851bb77), X(0x72f1aed9),
X(0x387eda8e), X(0x72db8828), X(0x38abf0ef), X(0x72c54fc1),
X(0x38d8fe93), X(0x72af05a7), X(0x39060373), X(0x7298a9dd),
X(0x3932ff87), X(0x72823c67), X(0x395ff2c9), X(0x726bbd48),
X(0x398cdd32), X(0x72552c85), X(0x39b9bebc), X(0x723e8a20),
X(0x39e6975e), X(0x7227d61c), X(0x3a136712), X(0x7211107e),
X(0x3a402dd2), X(0x71fa3949), X(0x3a6ceb96), X(0x71e35080),
X(0x3a99a057), X(0x71cc5626), X(0x3ac64c0f), X(0x71b54a41),
X(0x3af2eeb7), X(0x719e2cd2), X(0x3b1f8848), X(0x7186fdde),
X(0x3b4c18ba), X(0x716fbd68), X(0x3b78a007), X(0x71586b74),
X(0x3ba51e29), X(0x71410805), X(0x3bd19318), X(0x7129931f),
X(0x3bfdfecd), X(0x71120cc5), X(0x3c2a6142), X(0x70fa74fc),
X(0x3c56ba70), X(0x70e2cbc6), X(0x3c830a50), X(0x70cb1128),
X(0x3caf50da), X(0x70b34525), X(0x3cdb8e09), X(0x709b67c0),
X(0x3d07c1d6), X(0x708378ff), X(0x3d33ec39), X(0x706b78e3),
X(0x3d600d2c), X(0x70536771), X(0x3d8c24a8), X(0x703b44ad),
X(0x3db832a6), X(0x7023109a), X(0x3de4371f), X(0x700acb3c),
X(0x3e10320d), X(0x6ff27497), X(0x3e3c2369), X(0x6fda0cae),
X(0x3e680b2c), X(0x6fc19385), X(0x3e93e950), X(0x6fa90921),
X(0x3ebfbdcd), X(0x6f906d84), X(0x3eeb889c), X(0x6f77c0b3),
X(0x3f1749b8), X(0x6f5f02b2), X(0x3f430119), X(0x6f463383),
X(0x3f6eaeb8), X(0x6f2d532c), X(0x3f9a5290), X(0x6f1461b0),
X(0x3fc5ec98), X(0x6efb5f12), X(0x3ff17cca), X(0x6ee24b57),
X(0x401d0321), X(0x6ec92683), X(0x40487f94), X(0x6eaff099),
X(0x4073f21d), X(0x6e96a99d), X(0x409f5ab6), X(0x6e7d5193),
X(0x40cab958), X(0x6e63e87f), X(0x40f60dfb), X(0x6e4a6e66),
X(0x4121589b), X(0x6e30e34a), X(0x414c992f), X(0x6e174730),
X(0x4177cfb1), X(0x6dfd9a1c), X(0x41a2fc1a), X(0x6de3dc11),
X(0x41ce1e65), X(0x6dca0d14), X(0x41f93689), X(0x6db02d29),
X(0x42244481), X(0x6d963c54), X(0x424f4845), X(0x6d7c3a98),
X(0x427a41d0), X(0x6d6227fa), X(0x42a5311b), X(0x6d48047e),
X(0x42d0161e), X(0x6d2dd027), X(0x42faf0d4), X(0x6d138afb),
X(0x4325c135), X(0x6cf934fc), X(0x4350873c), X(0x6cdece2f),
X(0x437b42e1), X(0x6cc45698), X(0x43a5f41e), X(0x6ca9ce3b),
X(0x43d09aed), X(0x6c8f351c), X(0x43fb3746), X(0x6c748b3f),
X(0x4425c923), X(0x6c59d0a9), X(0x4450507e), X(0x6c3f055d),
X(0x447acd50), X(0x6c242960), X(0x44a53f93), X(0x6c093cb6),
X(0x44cfa740), X(0x6bee3f62), X(0x44fa0450), X(0x6bd3316a),
X(0x452456bd), X(0x6bb812d1), X(0x454e9e80), X(0x6b9ce39b),
X(0x4578db93), X(0x6b81a3cd), X(0x45a30df0), X(0x6b66536b),
X(0x45cd358f), X(0x6b4af279), X(0x45f7526b), X(0x6b2f80fb),
X(0x4621647d), X(0x6b13fef5), X(0x464b6bbe), X(0x6af86c6c),
X(0x46756828), X(0x6adcc964), X(0x469f59b4), X(0x6ac115e2),
X(0x46c9405c), X(0x6aa551e9), X(0x46f31c1a), X(0x6a897d7d),
X(0x471cece7), X(0x6a6d98a4), X(0x4746b2bc), X(0x6a51a361),
X(0x47706d93), X(0x6a359db9), X(0x479a1d67), X(0x6a1987b0),
X(0x47c3c22f), X(0x69fd614a), X(0x47ed5be6), X(0x69e12a8c),
X(0x4816ea86), X(0x69c4e37a), X(0x48406e08), X(0x69a88c19),
X(0x4869e665), X(0x698c246c), X(0x48935397), X(0x696fac78),
X(0x48bcb599), X(0x69532442), X(0x48e60c62), X(0x69368bce),
X(0x490f57ee), X(0x6919e320), X(0x49389836), X(0x68fd2a3d),
X(0x4961cd33), X(0x68e06129), X(0x498af6df), X(0x68c387e9),
X(0x49b41533), X(0x68a69e81), X(0x49dd282a), X(0x6889a4f6),
X(0x4a062fbd), X(0x686c9b4b), X(0x4a2f2be6), X(0x684f8186),
X(0x4a581c9e), X(0x683257ab), X(0x4a8101de), X(0x68151dbe),
X(0x4aa9dba2), X(0x67f7d3c5), X(0x4ad2a9e2), X(0x67da79c3),
X(0x4afb6c98), X(0x67bd0fbd), X(0x4b2423be), X(0x679f95b7),
X(0x4b4ccf4d), X(0x67820bb7), X(0x4b756f40), X(0x676471c0),
X(0x4b9e0390), X(0x6746c7d8), X(0x4bc68c36), X(0x67290e02),
X(0x4bef092d), X(0x670b4444), X(0x4c177a6e), X(0x66ed6aa1),
X(0x4c3fdff4), X(0x66cf8120), X(0x4c6839b7), X(0x66b187c3),
X(0x4c9087b1), X(0x66937e91), X(0x4cb8c9dd), X(0x6675658c),
X(0x4ce10034), X(0x66573cbb), X(0x4d092ab0), X(0x66390422),
X(0x4d31494b), X(0x661abbc5), X(0x4d595bfe), X(0x65fc63a9),
X(0x4d8162c4), X(0x65ddfbd3), X(0x4da95d96), X(0x65bf8447),
X(0x4dd14c6e), X(0x65a0fd0b), X(0x4df92f46), X(0x65826622),
X(0x4e210617), X(0x6563bf92), X(0x4e48d0dd), X(0x6545095f),
X(0x4e708f8f), X(0x6526438f), X(0x4e984229), X(0x65076e25),
X(0x4ebfe8a5), X(0x64e88926), X(0x4ee782fb), X(0x64c99498),
X(0x4f0f1126), X(0x64aa907f), X(0x4f369320), X(0x648b7ce0),
X(0x4f5e08e3), X(0x646c59bf), X(0x4f857269), X(0x644d2722),
X(0x4faccfab), X(0x642de50d), X(0x4fd420a4), X(0x640e9386),
X(0x4ffb654d), X(0x63ef3290), X(0x50229da1), X(0x63cfc231),
X(0x5049c999), X(0x63b0426d), X(0x5070e92f), X(0x6390b34a),
X(0x5097fc5e), X(0x637114cc), X(0x50bf031f), X(0x635166f9),
X(0x50e5fd6d), X(0x6331a9d4), X(0x510ceb40), X(0x6311dd64),
X(0x5133cc94), X(0x62f201ac), X(0x515aa162), X(0x62d216b3),
X(0x518169a5), X(0x62b21c7b), X(0x51a82555), X(0x6292130c),
X(0x51ced46e), X(0x6271fa69), X(0x51f576ea), X(0x6251d298),
X(0x521c0cc2), X(0x62319b9d), X(0x524295f0), X(0x6211557e),
X(0x5269126e), X(0x61f1003f), X(0x528f8238), X(0x61d09be5),
X(0x52b5e546), X(0x61b02876), X(0x52dc3b92), X(0x618fa5f7),
X(0x53028518), X(0x616f146c), X(0x5328c1d0), X(0x614e73da),
X(0x534ef1b5), X(0x612dc447), X(0x537514c2), X(0x610d05b7),
X(0x539b2af0), X(0x60ec3830), X(0x53c13439), X(0x60cb5bb7),
X(0x53e73097), X(0x60aa7050), X(0x540d2005), X(0x60897601),
X(0x5433027d), X(0x60686ccf), X(0x5458d7f9), X(0x604754bf),
X(0x547ea073), X(0x60262dd6), X(0x54a45be6), X(0x6004f819),
X(0x54ca0a4b), X(0x5fe3b38d), X(0x54efab9c), X(0x5fc26038),
X(0x55153fd4), X(0x5fa0fe1f), X(0x553ac6ee), X(0x5f7f8d46),
X(0x556040e2), X(0x5f5e0db3), X(0x5585adad), X(0x5f3c7f6b),
X(0x55ab0d46), X(0x5f1ae274), X(0x55d05faa), X(0x5ef936d1),
X(0x55f5a4d2), X(0x5ed77c8a), X(0x561adcb9), X(0x5eb5b3a2),
X(0x56400758), X(0x5e93dc1f), X(0x566524aa), X(0x5e71f606),
X(0x568a34a9), X(0x5e50015d), X(0x56af3750), X(0x5e2dfe29),
X(0x56d42c99), X(0x5e0bec6e), X(0x56f9147e), X(0x5de9cc33),
X(0x571deefa), X(0x5dc79d7c), X(0x5742bc06), X(0x5da5604f),
X(0x57677b9d), X(0x5d8314b1), X(0x578c2dba), X(0x5d60baa7),
X(0x57b0d256), X(0x5d3e5237), X(0x57d5696d), X(0x5d1bdb65),
X(0x57f9f2f8), X(0x5cf95638), X(0x581e6ef1), X(0x5cd6c2b5),
X(0x5842dd54), X(0x5cb420e0), X(0x58673e1b), X(0x5c9170bf),
X(0x588b9140), X(0x5c6eb258), X(0x58afd6bd), X(0x5c4be5b0),
X(0x58d40e8c), X(0x5c290acc), X(0x58f838a9), X(0x5c0621b2),
X(0x591c550e), X(0x5be32a67), X(0x594063b5), X(0x5bc024f0),
X(0x59646498), X(0x5b9d1154), X(0x598857b2), X(0x5b79ef96),
X(0x59ac3cfd), X(0x5b56bfbd), X(0x59d01475), X(0x5b3381ce),
X(0x59f3de12), X(0x5b1035cf), X(0x5a1799d1), X(0x5aecdbc5),
X(0x5a3b47ab), X(0x5ac973b5), X(0x5a5ee79a), X(0x5aa5fda5),
X(0x5a82799a), X(0x5a82799a)
};

/* {sin((2*i+1)*PI/4096), cos((2*i+1)*PI/4096)}, with i = 0 to 511 */
static LOOKUP_T sincos_lookup1[1024] = {
X(0x001921fb), X(0x7ffffd88), X(0x004b65ee), X(0x7fffe9cb),
X(0x007da9d4), X(0x7fffc251), X(0x00afeda8), X(0x7fff8719),
X(0x00e23160), X(0x7fff3824), X(0x011474f6), X(0x7ffed572),
X(0x0146b860), X(0x7ffe5f03), X(0x0178fb99), X(0x7ffdd4d7),
X(0x01ab3e97), X(0x7ffd36ee), X(0x01dd8154), X(0x7ffc8549),
X(0x020fc3c6), X(0x7ffbbfe6), X(0x024205e8), X(0x7ffae6c7),
X(0x027447b0), X(0x7ff9f9ec), X(0x02a68917), X(0x7ff8f954),
X(0x02d8ca16), X(0x7ff7e500), X(0x030b0aa4), X(0x7ff6bcf0),
X(0x033d4abb), X(0x7ff58125), X(0x036f8a51), X(0x7ff4319d),
X(0x03a1c960), X(0x7ff2ce5b), X(0x03d407df), X(0x7ff1575d),
X(0x040645c7), X(0x7fefcca4), X(0x04388310), X(0x7fee2e30),
X(0x046abfb3), X(0x7fec7c02), X(0x049cfba7), X(0x7feab61a),
X(0x04cf36e5), X(0x7fe8dc78), X(0x05017165), X(0x7fe6ef1c),
X(0x0533ab20), X(0x7fe4ee06), X(0x0565e40d), X(0x7fe2d938),
X(0x05981c26), X(0x7fe0b0b1), X(0x05ca5361), X(0x7fde7471),
X(0x05fc89b8), X(0x7fdc247a), X(0x062ebf22), X(0x7fd9c0ca),
X(0x0660f398), X(0x7fd74964), X(0x06932713), X(0x7fd4be46),
X(0x06c5598a), X(0x7fd21f72), X(0x06f78af6), X(0x7fcf6ce8),
X(0x0729bb4e), X(0x7fcca6a7), X(0x075bea8c), X(0x7fc9ccb2),
X(0x078e18a7), X(0x7fc6df08), X(0x07c04598), X(0x7fc3dda9),
X(0x07f27157), X(0x7fc0c896), X(0x08249bdd), X(0x7fbd9fd0),
X(0x0856c520), X(0x7fba6357), X(0x0888ed1b), X(0x7fb7132b),
X(0x08bb13c5), X(0x7fb3af4e), X(0x08ed3916), X(0x7fb037bf),
X(0x091f5d06), X(0x7facac7f), X(0x09517f8f), X(0x7fa90d8e),
X(0x0983a0a7), X(0x7fa55aee), X(0x09b5c048), X(0x7fa1949e),
X(0x09e7de6a), X(0x7f9dbaa0), X(0x0a19fb04), X(0x7f99ccf4),
X(0x0a4c1610), X(0x7f95cb9a), X(0x0a7e2f85), X(0x7f91b694),
X(0x0ab0475c), X(0x7f8d8de1), X(0x0ae25d8d), X(0x7f895182),
X(0x0b147211), X(0x7f850179), X(0x0b4684df), X(0x7f809dc5),
X(0x0b7895f0), X(0x7f7c2668), X(0x0baaa53b), X(0x7f779b62),
X(0x0bdcb2bb), X(0x7f72fcb4), X(0x0c0ebe66), X(0x7f6e4a5e),
X(0x0c40c835), X(0x7f698461), X(0x0c72d020), X(0x7f64aabf),
X(0x0ca4d620), X(0x7f5fbd77), X(0x0cd6da2d), X(0x7f5abc8a),
X(0x0d08dc3f), X(0x7f55a7fa), X(0x0d3adc4e), X(0x7f507fc7),
X(0x0d6cda53), X(0x7f4b43f2), X(0x0d9ed646), X(0x7f45f47b),
X(0x0dd0d01f), X(0x7f409164), X(0x0e02c7d7), X(0x7f3b1aad),
X(0x0e34bd66), X(0x7f359057), X(0x0e66b0c3), X(0x7f2ff263),
X(0x0e98a1e9), X(0x7f2a40d2), X(0x0eca90ce), X(0x7f247ba5),
X(0x0efc7d6b), X(0x7f1ea2dc), X(0x0f2e67b8), X(0x7f18b679),
X(0x0f604faf), X(0x7f12b67c), X(0x0f923546), X(0x7f0ca2e7),
X(0x0fc41876), X(0x7f067bba), X(0x0ff5f938), X(0x7f0040f6),
X(0x1027d784), X(0x7ef9f29d), X(0x1059b352), X(0x7ef390ae),
X(0x108b8c9b), X(0x7eed1b2c), X(0x10bd6356), X(0x7ee69217),
X(0x10ef377d), X(0x7edff570), X(0x11210907), X(0x7ed94538),
X(0x1152d7ed), X(0x7ed28171), X(0x1184a427), X(0x7ecbaa1a),
X(0x11b66dad), X(0x7ec4bf36), X(0x11e83478), X(0x7ebdc0c6),
X(0x1219f880), X(0x7eb6aeca), X(0x124bb9be), X(0x7eaf8943),
X(0x127d7829), X(0x7ea85033), X(0x12af33ba), X(0x7ea1039b),
X(0x12e0ec6a), X(0x7e99a37c), X(0x1312a230), X(0x7e922fd6),
X(0x13445505), X(0x7e8aa8ac), X(0x137604e2), X(0x7e830dff),
X(0x13a7b1bf), X(0x7e7b5fce), X(0x13d95b93), X(0x7e739e1d),
X(0x140b0258), X(0x7e6bc8eb), X(0x143ca605), X(0x7e63e03b),
X(0x146e4694), X(0x7e5be40c), X(0x149fe3fc), X(0x7e53d462),
X(0x14d17e36), X(0x7e4bb13c), X(0x1503153a), X(0x7e437a9c),
X(0x1534a901), X(0x7e3b3083), X(0x15663982), X(0x7e32d2f4),
X(0x1597c6b7), X(0x7e2a61ed), X(0x15c95097), X(0x7e21dd73),
X(0x15fad71b), X(0x7e194584), X(0x162c5a3b), X(0x7e109a24),
X(0x165dd9f0), X(0x7e07db52), X(0x168f5632), X(0x7dff0911),
X(0x16c0cef9), X(0x7df62362), X(0x16f2443e), X(0x7ded2a47),
X(0x1723b5f9), X(0x7de41dc0), X(0x17552422), X(0x7ddafdce),
X(0x17868eb3), X(0x7dd1ca75), X(0x17b7f5a3), X(0x7dc883b4),
X(0x17e958ea), X(0x7dbf298d), X(0x181ab881), X(0x7db5bc02),
X(0x184c1461), X(0x7dac3b15), X(0x187d6c82), X(0x7da2a6c6),
X(0x18aec0db), X(0x7d98ff17), X(0x18e01167), X(0x7d8f4409),
X(0x19115e1c), X(0x7d85759f), X(0x1942a6f3), X(0x7d7b93da),
X(0x1973ebe6), X(0x7d719eba), X(0x19a52ceb), X(0x7d679642),
X(0x19d669fc), X(0x7d5d7a74), X(0x1a07a311), X(0x7d534b50),
X(0x1a38d823), X(0x7d4908d9), X(0x1a6a0929), X(0x7d3eb30f),
X(0x1a9b361d), X(0x7d3449f5), X(0x1acc5ef6), X(0x7d29cd8c),
X(0x1afd83ad), X(0x7d1f3dd6), X(0x1b2ea43a), X(0x7d149ad5),
X(0x1b5fc097), X(0x7d09e489), X(0x1b90d8bb), X(0x7cff1af5),
X(0x1bc1ec9e), X(0x7cf43e1a), X(0x1bf2fc3a), X(0x7ce94dfb),
X(0x1c240786), X(0x7cde4a98), X(0x1c550e7c), X(0x7cd333f3),
X(0x1c861113), X(0x7cc80a0f), X(0x1cb70f43), X(0x7cbcccec),
X(0x1ce80906), X(0x7cb17c8d), X(0x1d18fe54), X(0x7ca618f3),
X(0x1d49ef26), X(0x7c9aa221), X(0x1d7adb73), X(0x7c8f1817),
X(0x1dabc334), X(0x7c837ad8), X(0x1ddca662), X(0x7c77ca65),
X(0x1e0d84f5), X(0x7c6c06c0), X(0x1e3e5ee5), X(0x7c602fec),
X(0x1e6f342c), X(0x7c5445e9), X(0x1ea004c1), X(0x7c4848ba),
X(0x1ed0d09d), X(0x7c3c3860), X(0x1f0197b8), X(0x7c3014de),
X(0x1f325a0b), X(0x7c23de35), X(0x1f63178f), X(0x7c179467),
X(0x1f93d03c), X(0x7c0b3777), X(0x1fc4840a), X(0x7bfec765),
X(0x1ff532f2), X(0x7bf24434), X(0x2025dcec), X(0x7be5ade6),
X(0x205681f1), X(0x7bd9047c), X(0x208721f9), X(0x7bcc47fa),
X(0x20b7bcfe), X(0x7bbf7860), X(0x20e852f6), X(0x7bb295b0),
X(0x2118e3dc), X(0x7ba59fee), X(0x21496fa7), X(0x7b989719),
X(0x2179f64f), X(0x7b8b7b36), X(0x21aa77cf), X(0x7b7e4c45),
X(0x21daf41d), X(0x7b710a49), X(0x220b6b32), X(0x7b63b543),
X(0x223bdd08), X(0x7b564d36), X(0x226c4996), X(0x7b48d225),
X(0x229cb0d5), X(0x7b3b4410), X(0x22cd12bd), X(0x7b2da2fa),
X(0x22fd6f48), X(0x7b1feee5), X(0x232dc66d), X(0x7b1227d3),
X(0x235e1826), X(0x7b044dc7), X(0x238e646a), X(0x7af660c2),
X(0x23beab33), X(0x7ae860c7), X(0x23eeec78), X(0x7ada4dd8),
X(0x241f2833), X(0x7acc27f7), X(0x244f5e5c), X(0x7abdef25),
X(0x247f8eec), X(0x7aafa367), X(0x24afb9da), X(0x7aa144bc),
X(0x24dfdf20), X(0x7a92d329), X(0x250ffeb7), X(0x7a844eae),
X(0x25401896), X(0x7a75b74f), X(0x25702cb7), X(0x7a670d0d),
X(0x25a03b11), X(0x7a584feb), X(0x25d0439f), X(0x7a497feb),
X(0x26004657), X(0x7a3a9d0f), X(0x26304333), X(0x7a2ba75a),
X(0x26603a2c), X(0x7a1c9ece), X(0x26902b39), X(0x7a0d836d),
X(0x26c01655), X(0x79fe5539), X(0x26effb76), X(0x79ef1436),
X(0x271fda96), X(0x79dfc064), X(0x274fb3ae), X(0x79d059c8),
X(0x277f86b5), X(0x79c0e062), X(0x27af53a6), X(0x79b15435),
X(0x27df1a77), X(0x79a1b545), X(0x280edb23), X(0x79920392),
X(0x283e95a1), X(0x79823f20), X(0x286e49ea), X(0x797267f2),
X(0x289df7f8), X(0x79627e08), X(0x28cd9fc1), X(0x79528167),
X(0x28fd4140), X(0x79427210), X(0x292cdc6d), X(0x79325006),
X(0x295c7140), X(0x79221b4b), X(0x298bffb2), X(0x7911d3e2),
X(0x29bb87bc), X(0x790179cd), X(0x29eb0957), X(0x78f10d0f),
X(0x2a1a847b), X(0x78e08dab), X(0x2a49f920), X(0x78cffba3),
X(0x2a796740), X(0x78bf56f9), X(0x2aa8ced3), X(0x78ae9fb0),
X(0x2ad82fd2), X(0x789dd5cb), X(0x2b078a36), X(0x788cf94c),
X(0x2b36ddf7), X(0x787c0a36), X(0x2b662b0e), X(0x786b088c),
X(0x2b957173), X(0x7859f44f), X(0x2bc4b120), X(0x7848cd83),
X(0x2bf3ea0d), X(0x7837942b), X(0x2c231c33), X(0x78264849),
X(0x2c52478a), X(0x7814e9df), X(0x2c816c0c), X(0x780378f1),
X(0x2cb089b1), X(0x77f1f581), X(0x2cdfa071), X(0x77e05f91),
X(0x2d0eb046), X(0x77ceb725), X(0x2d3db928), X(0x77bcfc3f),
X(0x2d6cbb10), X(0x77ab2ee2), X(0x2d9bb5f6), X(0x77994f11),
X(0x2dcaa9d5), X(0x77875cce), X(0x2df996a3), X(0x7775581d),
X(0x2e287c5a), X(0x776340ff), X(0x2e575af3), X(0x77511778),
X(0x2e863267), X(0x773edb8b), X(0x2eb502ae), X(0x772c8d3a),
X(0x2ee3cbc1), X(0x771a2c88), X(0x2f128d99), X(0x7707b979),
X(0x2f41482e), X(0x76f5340e), X(0x2f6ffb7a), X(0x76e29c4b),
X(0x2f9ea775), X(0x76cff232), X(0x2fcd4c19), X(0x76bd35c7),
X(0x2ffbe95d), X(0x76aa670d), X(0x302a7f3a), X(0x76978605),
X(0x30590dab), X(0x768492b4), X(0x308794a6), X(0x76718d1c),
X(0x30b61426), X(0x765e7540), X(0x30e48c22), X(0x764b4b23),
X(0x3112fc95), X(0x76380ec8), X(0x31416576), X(0x7624c031),
X(0x316fc6be), X(0x76115f63), X(0x319e2067), X(0x75fdec60),
X(0x31cc7269), X(0x75ea672a), X(0x31fabcbd), X(0x75d6cfc5),
X(0x3228ff5c), X(0x75c32634), X(0x32573a3f), X(0x75af6a7b),
X(0x32856d5e), X(0x759b9c9b), X(0x32b398b3), X(0x7587bc98),
X(0x32e1bc36), X(0x7573ca75), X(0x330fd7e1), X(0x755fc635),
X(0x333debab), X(0x754bafdc), X(0x336bf78f), X(0x7537876c),
X(0x3399fb85), X(0x75234ce8), X(0x33c7f785), X(0x750f0054),
X(0x33f5eb89), X(0x74faa1b3), X(0x3423d78a), X(0x74e63108),
X(0x3451bb81), X(0x74d1ae55), X(0x347f9766), X(0x74bd199f),
X(0x34ad6b32), X(0x74a872e8), X(0x34db36df), X(0x7493ba34),
X(0x3508fa66), X(0x747eef85), X(0x3536b5be), X(0x746a12df),
X(0x356468e2), X(0x74552446), X(0x359213c9), X(0x744023bc),
X(0x35bfb66e), X(0x742b1144), X(0x35ed50c9), X(0x7415ece2),
X(0x361ae2d3), X(0x7400b69a), X(0x36486c86), X(0x73eb6e6e),
X(0x3675edd9), X(0x73d61461), X(0x36a366c6), X(0x73c0a878),
X(0x36d0d746), X(0x73ab2ab4), X(0x36fe3f52), X(0x73959b1b),
X(0x372b9ee3), X(0x737ff9ae), X(0x3758f5f2), X(0x736a4671),
X(0x37864477), X(0x73548168), X(0x37b38a6d), X(0x733eaa96),
X(0x37e0c7cc), X(0x7328c1ff), X(0x380dfc8d), X(0x7312c7a5),
X(0x383b28a9), X(0x72fcbb8c), X(0x38684c19), X(0x72e69db7),
X(0x389566d6), X(0x72d06e2b), X(0x38c278d9), X(0x72ba2cea),
X(0x38ef821c), X(0x72a3d9f7), X(0x391c8297), X(0x728d7557),
X(0x39497a43), X(0x7276ff0d), X(0x39766919), X(0x7260771b),
X(0x39a34f13), X(0x7249dd86), X(0x39d02c2a), X(0x72333251),
X(0x39fd0056), X(0x721c7580), X(0x3a29cb91), X(0x7205a716),
X(0x3a568dd4), X(0x71eec716), X(0x3a834717), X(0x71d7d585),
X(0x3aaff755), X(0x71c0d265), X(0x3adc9e86), X(0x71a9bdba),
X(0x3b093ca3), X(0x71929789), X(0x3b35d1a5), X(0x717b5fd3),
X(0x3b625d86), X(0x7164169d), X(0x3b8ee03e), X(0x714cbbeb),
X(0x3bbb59c7), X(0x71354fc0), X(0x3be7ca1a), X(0x711dd220),
X(0x3c143130), X(0x7106430e), X(0x3c408f03), X(0x70eea28e),
X(0x3c6ce38a), X(0x70d6f0a4), X(0x3c992ec0), X(0x70bf2d53),
X(0x3cc5709e), X(0x70a7589f), X(0x3cf1a91c), X(0x708f728b),
X(0x3d1dd835), X(0x70777b1c), X(0x3d49fde1), X(0x705f7255),
X(0x3d761a19), X(0x70475839), X(0x3da22cd7), X(0x702f2ccd),
X(0x3dce3614), X(0x7016f014), X(0x3dfa35c8), X(0x6ffea212),
X(0x3e262bee), X(0x6fe642ca), X(0x3e52187f), X(0x6fcdd241),
X(0x3e7dfb73), X(0x6fb5507a), X(0x3ea9d4c3), X(0x6f9cbd79),
X(0x3ed5a46b), X(0x6f841942), X(0x3f016a61), X(0x6f6b63d8),
X(0x3f2d26a0), X(0x6f529d40), X(0x3f58d921), X(0x6f39c57d),
X(0x3f8481dd), X(0x6f20dc92), X(0x3fb020ce), X(0x6f07e285),
X(0x3fdbb5ec), X(0x6eeed758), X(0x40074132), X(0x6ed5bb10),
X(0x4032c297), X(0x6ebc8db0), X(0x405e3a16), X(0x6ea34f3d),
X(0x4089a7a8), X(0x6e89ffb9), X(0x40b50b46), X(0x6e709f2a),
X(0x40e064ea), X(0x6e572d93), X(0x410bb48c), X(0x6e3daaf8),
X(0x4136fa27), X(0x6e24175c), X(0x416235b2), X(0x6e0a72c5),
X(0x418d6729), X(0x6df0bd35), X(0x41b88e84), X(0x6dd6f6b1),
X(0x41e3abbc), X(0x6dbd1f3c), X(0x420ebecb), X(0x6da336dc),
X(0x4239c7aa), X(0x6d893d93), X(0x4264c653), X(0x6d6f3365),
X(0x428fbabe), X(0x6d551858), X(0x42baa4e6), X(0x6d3aec6e),
X(0x42e584c3), X(0x6d20afac), X(0x43105a50), X(0x6d066215),
X(0x433b2585), X(0x6cec03af), X(0x4365e65b), X(0x6cd1947c),
X(0x43909ccd), X(0x6cb71482), X(0x43bb48d4), X(0x6c9c83c3),
X(0x43e5ea68), X(0x6c81e245), X(0x44108184), X(0x6c67300b),
X(0x443b0e21), X(0x6c4c6d1a), X(0x44659039), X(0x6c319975),
X(0x449007c4), X(0x6c16b521), X(0x44ba74bd), X(0x6bfbc021),
X(0x44e4d71c), X(0x6be0ba7b), X(0x450f2edb), X(0x6bc5a431),
X(0x45397bf4), X(0x6baa7d49), X(0x4563be60), X(0x6b8f45c7),
X(0x458df619), X(0x6b73fdae), X(0x45b82318), X(0x6b58a503),
X(0x45e24556), X(0x6b3d3bcb), X(0x460c5cce), X(0x6b21c208),
X(0x46366978), X(0x6b0637c1), X(0x46606b4e), X(0x6aea9cf8),
X(0x468a624a), X(0x6acef1b2), X(0x46b44e65), X(0x6ab335f4),
X(0x46de2f99), X(0x6a9769c1), X(0x470805df), X(0x6a7b8d1e),
X(0x4731d131), X(0x6a5fa010), X(0x475b9188), X(0x6a43a29a),
X(0x478546de), X(0x6a2794c1), X(0x47aef12c), X(0x6a0b7689),
X(0x47d8906d), X(0x69ef47f6), X(0x48022499), X(0x69d3090e),
X(0x482badab), X(0x69b6b9d3), X(0x48552b9b), X(0x699a5a4c),
X(0x487e9e64), X(0x697dea7b), X(0x48a805ff), X(0x69616a65),
X(0x48d16265), X(0x6944da10), X(0x48fab391), X(0x6928397e),
X(0x4923f97b), X(0x690b88b5), X(0x494d341e), X(0x68eec7b9),
X(0x49766373), X(0x68d1f68f), X(0x499f8774), X(0x68b5153a),
X(0x49c8a01b), X(0x689823bf), X(0x49f1ad61), X(0x687b2224),
X(0x4a1aaf3f), X(0x685e106c), X(0x4a43a5b0), X(0x6840ee9b),
X(0x4a6c90ad), X(0x6823bcb7), X(0x4a957030), X(0x68067ac3),
X(0x4abe4433), X(0x67e928c5), X(0x4ae70caf), X(0x67cbc6c0),
X(0x4b0fc99d), X(0x67ae54ba), X(0x4b387af9), X(0x6790d2b6),
X(0x4b6120bb), X(0x677340ba), X(0x4b89badd), X(0x67559eca),
X(0x4bb24958), X(0x6737ecea), X(0x4bdacc28), X(0x671a2b20),
X(0x4c034345), X(0x66fc596f), X(0x4c2baea9), X(0x66de77dc),
X(0x4c540e4e), X(0x66c0866d), X(0x4c7c622d), X(0x66a28524),
X(0x4ca4aa41), X(0x66847408), X(0x4ccce684), X(0x6666531d),
X(0x4cf516ee), X(0x66482267), X(0x4d1d3b7a), X(0x6629e1ec),
X(0x4d455422), X(0x660b91af), X(0x4d6d60df), X(0x65ed31b5),
X(0x4d9561ac), X(0x65cec204), X(0x4dbd5682), X(0x65b0429f),
X(0x4de53f5a), X(0x6591b38c), X(0x4e0d1c30), X(0x657314cf),
X(0x4e34ecfc), X(0x6554666d), X(0x4e5cb1b9), X(0x6535a86b),
X(0x4e846a60), X(0x6516dacd), X(0x4eac16eb), X(0x64f7fd98),
X(0x4ed3b755), X(0x64d910d1), X(0x4efb4b96), X(0x64ba147d),
X(0x4f22d3aa), X(0x649b08a0), X(0x4f4a4f89), X(0x647bed3f),
X(0x4f71bf2e), X(0x645cc260), X(0x4f992293), X(0x643d8806),
X(0x4fc079b1), X(0x641e3e38), X(0x4fe7c483), X(0x63fee4f8),
X(0x500f0302), X(0x63df7c4d), X(0x50363529), X(0x63c0043b),
X(0x505d5af1), X(0x63a07cc7), X(0x50847454), X(0x6380e5f6),
X(0x50ab814d), X(0x63613fcd), X(0x50d281d5), X(0x63418a50),
X(0x50f975e6), X(0x6321c585), X(0x51205d7b), X(0x6301f171),
X(0x5147388c), X(0x62e20e17), X(0x516e0715), X(0x62c21b7e),
X(0x5194c910), X(0x62a219aa), X(0x51bb7e75), X(0x628208a1),
X(0x51e22740), X(0x6261e866), X(0x5208c36a), X(0x6241b8ff),
X(0x522f52ee), X(0x62217a72), X(0x5255d5c5), X(0x62012cc2),
X(0x527c4bea), X(0x61e0cff5), X(0x52a2b556), X(0x61c06410),
X(0x52c91204), X(0x619fe918), X(0x52ef61ee), X(0x617f5f12),
X(0x5315a50e), X(0x615ec603), X(0x533bdb5d), X(0x613e1df0),
X(0x536204d7), X(0x611d66de), X(0x53882175), X(0x60fca0d2),
X(0x53ae3131), X(0x60dbcbd1), X(0x53d43406), X(0x60bae7e1),
X(0x53fa29ed), X(0x6099f505), X(0x542012e1), X(0x6078f344),
X(0x5445eedb), X(0x6057e2a2), X(0x546bbdd7), X(0x6036c325),
X(0x54917fce), X(0x601594d1), X(0x54b734ba), X(0x5ff457ad),
X(0x54dcdc96), X(0x5fd30bbc), X(0x5502775c), X(0x5fb1b104),
X(0x55280505), X(0x5f90478a), X(0x554d858d), X(0x5f6ecf53),
X(0x5572f8ed), X(0x5f4d4865), X(0x55985f20), X(0x5f2bb2c5),
X(0x55bdb81f), X(0x5f0a0e77), X(0x55e303e6), X(0x5ee85b82),
X(0x5608426e), X(0x5ec699e9), X(0x562d73b2), X(0x5ea4c9b3),
X(0x565297ab), X(0x5e82eae5), X(0x5677ae54), X(0x5e60fd84),
X(0x569cb7a8), X(0x5e3f0194), X(0x56c1b3a1), X(0x5e1cf71c),
X(0x56e6a239), X(0x5dfade20), X(0x570b8369), X(0x5dd8b6a7),
X(0x5730572e), X(0x5db680b4), X(0x57551d80), X(0x5d943c4e),
X(0x5779d65b), X(0x5d71e979), X(0x579e81b8), X(0x5d4f883b),
X(0x57c31f92), X(0x5d2d189a), X(0x57e7afe4), X(0x5d0a9a9a),
X(0x580c32a7), X(0x5ce80e41), X(0x5830a7d6), X(0x5cc57394),
X(0x58550f6c), X(0x5ca2ca99), X(0x58796962), X(0x5c801354),
X(0x589db5b3), X(0x5c5d4dcc), X(0x58c1f45b), X(0x5c3a7a05),
X(0x58e62552), X(0x5c179806), X(0x590a4893), X(0x5bf4a7d2),
X(0x592e5e19), X(0x5bd1a971), X(0x595265df), X(0x5bae9ce7),
X(0x59765fde), X(0x5b8b8239), X(0x599a4c12), X(0x5b68596d),
X(0x59be2a74), X(0x5b452288), X(0x59e1faff), X(0x5b21dd90),
X(0x5a05bdae), X(0x5afe8a8b), X(0x5a29727b), X(0x5adb297d),
X(0x5a4d1960), X(0x5ab7ba6c), X(0x5a70b258), X(0x5a943d5e),
};

static LOOKUP_T vwin64[32] = {
X(0x001f0003), X(0x01168c98), X(0x030333c8), X(0x05dfe3a4),
X(0x09a49562), X(0x0e45df18), X(0x13b47ef2), X(0x19dcf676),
X(0x20a74d83), X(0x27f7137c), X(0x2fabb05a), X(0x37a1105a),
X(0x3fb0ab28), X(0x47b2dcd1), X(0x4f807bc6), X(0x56f48e70),
X(0x5dedfc79), X(0x64511653), X(0x6a08cfff), X(0x6f079328),
X(0x734796f4), X(0x76cab7f2), X(0x7999d6e8), X(0x7bc3cf9f),
X(0x7d5c20c1), X(0x7e7961df), X(0x7f33a567), X(0x7fa2e1d0),
X(0x7fdd78a5), X(0x7ff6ec6d), X(0x7ffed0e9), X(0x7ffffc3f),
};

static LOOKUP_T vwin128[64] = {
X(0x0007c04d), X(0x0045bb89), X(0x00c18b87), X(0x017ae294),
X(0x02714a4e), X(0x03a4217a), X(0x05129952), X(0x06bbb24f),
X(0x089e38a1), X(0x0ab8c073), X(0x0d09a228), X(0x0f8ef6bd),
X(0x12469488), X(0x152e0c7a), X(0x1842a81c), X(0x1b81686d),
X(0x1ee705d9), X(0x226ff15d), X(0x26185705), X(0x29dc21cc),
X(0x2db700fe), X(0x31a46f08), X(0x359fb9c1), X(0x39a40c0c),
X(0x3dac78b6), X(0x41b40674), X(0x45b5bcb0), X(0x49acb109),
X(0x4d94152b), X(0x516744bd), X(0x5521d320), X(0x58bf98a5),
X(0x5c3cbef4), X(0x5f95cc5d), X(0x62c7add7), X(0x65cfbf64),
X(0x68abd2ba), X(0x6b5a3405), X(0x6dd9acab), X(0x7029840d),
X(0x72497e38), X(0x7439d8ac), X(0x75fb4532), X(0x778ee30a),
X(0x78f6367e), X(0x7a331f1a), X(0x7b47cccd), X(0x7c36b416),
X(0x7d028192), X(0x7dae0d18), X(0x7e3c4caa), X(0x7eb04763),
X(0x7f0d08a7), X(0x7f5593b7), X(0x7f8cd7d5), X(0x7fb5a513),
X(0x7fd2a1fc), X(0x7fe64212), X(0x7ff2bd4c), X(0x7ffa0890),
X(0x7ffdcf39), X(0x7fff6dac), X(0x7fffed01), X(0x7fffffc4),
};

static LOOKUP_T vwin256[128] = {
X(0x0001f018), X(0x00117066), X(0x00306e9e), X(0x005ee5f1),
X(0x009ccf26), X(0x00ea208b), X(0x0146cdea), X(0x01b2c87f),
X(0x022dfedf), X(0x02b85ced), X(0x0351cbbd), X(0x03fa317f),
X(0x04b17167), X(0x05776b90), X(0x064bfcdc), X(0x072efedd),
X(0x082047b4), X(0x091fa9f1), X(0x0a2cf477), X(0x0b47f25d),
X(0x0c706ad2), X(0x0da620ff), X(0x0ee8d3ef), X(0x10383e75),
X(0x11941716), X(0x12fc0ff6), X(0x146fd6c8), X(0x15ef14c2),
X(0x17796e8e), X(0x190e844f), X(0x1aadf196), X(0x1c574d6e),
X(0x1e0a2a62), X(0x1fc61688), X(0x218a9b9c), X(0x23573f12),
X(0x252b823d), X(0x2706e269), X(0x28e8d913), X(0x2ad0dc0e),
X(0x2cbe5dc1), X(0x2eb0cd60), X(0x30a79733), X(0x32a224d5),
X(0x349fdd8b), X(0x36a02690), X(0x38a2636f), X(0x3aa5f65e),
X(0x3caa409e), X(0x3eaea2df), X(0x40b27da6), X(0x42b531b8),
X(0x44b62086), X(0x46b4ac99), X(0x48b03a05), X(0x4aa82ed5),
X(0x4c9bf37d), X(0x4e8af349), X(0x50749ccb), X(0x52586246),
X(0x5435ba1c), X(0x560c1f31), X(0x57db1152), X(0x59a21591),
X(0x5b60b6a3), X(0x5d168535), X(0x5ec31839), X(0x60660d36),
X(0x61ff0886), X(0x638db595), X(0x6511c717), X(0x668af734),
X(0x67f907b0), X(0x695bc207), X(0x6ab2f787), X(0x6bfe815a),
X(0x6d3e4090), X(0x6e721e16), X(0x6f9a0ab5), X(0x70b5fef8),
X(0x71c5fb16), X(0x72ca06cd), X(0x73c2313d), X(0x74ae90b2),
X(0x758f4275), X(0x76646a85), X(0x772e335c), X(0x77eccda0),
X(0x78a06fd7), X(0x79495613), X(0x79e7c19c), X(0x7a7bf894),
X(0x7b064596), X(0x7b86f757), X(0x7bfe6044), X(0x7c6cd615),
X(0x7cd2b16e), X(0x7d304d71), X(0x7d860756), X(0x7dd43e06),
X(0x7e1b51ad), X(0x7e5ba355), X(0x7e95947e), X(0x7ec986bb),
X(0x7ef7db4a), X(0x7f20f2b9), X(0x7f452c7f), X(0x7f64e6a7),
X(0x7f807d71), X(0x7f984aff), X(0x7faca700), X(0x7fbde662),
X(0x7fcc5b04), X(0x7fd85372), X(0x7fe21a99), X(0x7fe9f791),
X(0x7ff02d58), X(0x7ff4fa9e), X(0x7ff89990), X(0x7ffb3faa),
X(0x7ffd1d8b), X(0x7ffe5ecc), X(0x7fff29e0), X(0x7fff9ff3),
X(0x7fffdcd2), X(0x7ffff6d6), X(0x7ffffed0), X(0x7ffffffc),
};

static LOOKUP_T vwin512[256] = {
X(0x00007c06), X(0x00045c32), X(0x000c1c62), X(0x0017bc4c),
X(0x00273b7a), X(0x003a9955), X(0x0051d51c), X(0x006cede7),
X(0x008be2a9), X(0x00aeb22a), X(0x00d55b0d), X(0x00ffdbcc),
X(0x012e32b6), X(0x01605df5), X(0x01965b85), X(0x01d02939),
X(0x020dc4ba), X(0x024f2b83), X(0x02945ae6), X(0x02dd5004),
X(0x032a07d3), X(0x037a7f19), X(0x03ceb26e), X(0x04269e37),
X(0x04823eab), X(0x04e18fcc), X(0x05448d6d), X(0x05ab3329),
X(0x06157c68), X(0x0683645e), X(0x06f4e607), X(0x0769fc25),
X(0x07e2a146), X(0x085ecfbc), X(0x08de819f), X(0x0961b0cc),
X(0x09e856e3), X(0x0a726d46), X(0x0affed1d), X(0x0b90cf4c),
X(0x0c250c79), X(0x0cbc9d0b), X(0x0d577926), X(0x0df598aa),
X(0x0e96f337), X(0x0f3b8026), X(0x0fe3368f), X(0x108e0d42),
X(0x113bfaca), X(0x11ecf56b), X(0x12a0f324), X(0x1357e9ac),
X(0x1411ce70), X(0x14ce9698), X(0x158e3702), X(0x1650a444),
X(0x1715d2aa), X(0x17ddb638), X(0x18a842aa), X(0x19756b72),
X(0x1a4523b9), X(0x1b175e62), X(0x1bec0e04), X(0x1cc324f0),
X(0x1d9c9532), X(0x1e78508a), X(0x1f564876), X(0x20366e2e),
X(0x2118b2a2), X(0x21fd0681), X(0x22e35a37), X(0x23cb9dee),
X(0x24b5c18e), X(0x25a1b4c0), X(0x268f66f1), X(0x277ec74e),
X(0x286fc4cc), X(0x29624e23), X(0x2a5651d7), X(0x2b4bbe34),
X(0x2c428150), X(0x2d3a8913), X(0x2e33c332), X(0x2f2e1d35),
X(0x30298478), X(0x3125e62d), X(0x32232f61), X(0x33214cfc),
X(0x34202bc2), X(0x351fb85a), X(0x361fdf4f), X(0x37208d10),
X(0x3821adf7), X(0x39232e49), X(0x3a24fa3c), X(0x3b26fdf6),
X(0x3c292593), X(0x3d2b5d29), X(0x3e2d90c8), X(0x3f2fac7f),
X(0x40319c5f), X(0x41334c81), X(0x4234a905), X(0x43359e16),
X(0x443617f3), X(0x453602eb), X(0x46354b65), X(0x4733dde1),
X(0x4831a6ff), X(0x492e937f), X(0x4a2a9045), X(0x4b258a5f),
X(0x4c1f6f06), X(0x4d182ba2), X(0x4e0fadce), X(0x4f05e35b),
X(0x4ffaba53), X(0x50ee20fd), X(0x51e005e1), X(0x52d057ca),
X(0x53bf05ca), X(0x54abff3b), X(0x559733c7), X(0x56809365),
X(0x57680e62), X(0x584d955d), X(0x59311952), X(0x5a128b96),
X(0x5af1dddd), X(0x5bcf023a), X(0x5ca9eb27), X(0x5d828b81),
X(0x5e58d68d), X(0x5f2cbffc), X(0x5ffe3be9), X(0x60cd3edf),
X(0x6199bdda), X(0x6263ae45), X(0x632b0602), X(0x63efbb66),
X(0x64b1c53f), X(0x65711ad0), X(0x662db3d7), X(0x66e7888d),
X(0x679e91a5), X(0x6852c84e), X(0x69042635), X(0x69b2a582),
X(0x6a5e40dd), X(0x6b06f36c), X(0x6bacb8d2), X(0x6c4f8d30),
X(0x6cef6d26), X(0x6d8c55d4), X(0x6e2644d4), X(0x6ebd3840),
X(0x6f512ead), X(0x6fe2272e), X(0x7070214f), X(0x70fb1d17),
X(0x71831b06), X(0x72081c16), X(0x728a21b5), X(0x73092dc8),
X(0x738542a6), X(0x73fe631b), X(0x74749261), X(0x74e7d421),
X(0x75582c72), X(0x75c59fd5), X(0x76303333), X(0x7697ebdd),
X(0x76fccf85), X(0x775ee443), X(0x77be308a), X(0x781abb2e),
X(0x78748b59), X(0x78cba88e), X(0x79201aa7), X(0x7971e9cd),
X(0x79c11e79), X(0x7a0dc170), X(0x7a57dbc2), X(0x7a9f76c1),
X(0x7ae49c07), X(0x7b27556b), X(0x7b67ad02), X(0x7ba5ad1b),
X(0x7be1603a), X(0x7c1ad118), X(0x7c520a9e), X(0x7c8717e1),
X(0x7cba0421), X(0x7ceadac3), X(0x7d19a74f), X(0x7d46756e),
X(0x7d7150e5), X(0x7d9a4592), X(0x7dc15f69), X(0x7de6aa71),
X(0x7e0a32c0), X(0x7e2c0479), X(0x7e4c2bc7), X(0x7e6ab4db),
X(0x7e87abe9), X(0x7ea31d24), X(0x7ebd14be), X(0x7ed59edd),
X(0x7eecc7a3), X(0x7f029b21), X(0x7f17255a), X(0x7f2a723f),
X(0x7f3c8daa), X(0x7f4d835d), X(0x7f5d5f00), X(0x7f6c2c1b),
X(0x7f79f617), X(0x7f86c83a), X(0x7f92ada2), X(0x7f9db146),
X(0x7fa7ddf3), X(0x7fb13e46), X(0x7fb9dcb0), X(0x7fc1c36c),
X(0x7fc8fc83), X(0x7fcf91c7), X(0x7fd58cd2), X(0x7fdaf702),
X(0x7fdfd979), X(0x7fe43d1c), X(0x7fe82a8b), X(0x7febaa29),
X(0x7feec412), X(0x7ff1801c), X(0x7ff3e5d6), X(0x7ff5fc86),
X(0x7ff7cb29), X(0x7ff9586f), X(0x7ffaaaba), X(0x7ffbc81e),
X(0x7ffcb660), X(0x7ffd7af3), X(0x7ffe1afa), X(0x7ffe9b42),
X(0x7fff0047), X(0x7fff4e2f), X(0x7fff88c9), X(0x7fffb390),
X(0x7fffd1a6), X(0x7fffe5d7), X(0x7ffff296), X(0x7ffff9fd),
X(0x7ffffdcd), X(0x7fffff6d), X(0x7fffffed), X(0x7fffffff),
};

static LOOKUP_T vwin1024[512] = {
X(0x00001f02), X(0x0001170e), X(0x00030724), X(0x0005ef40),
X(0x0009cf59), X(0x000ea767), X(0x0014775e), X(0x001b3f2e),
X(0x0022fec8), X(0x002bb618), X(0x00356508), X(0x00400b81),
X(0x004ba968), X(0x00583ea0), X(0x0065cb0a), X(0x00744e84),
X(0x0083c8ea), X(0x00943a14), X(0x00a5a1da), X(0x00b80010),
X(0x00cb5488), X(0x00df9f10), X(0x00f4df76), X(0x010b1584),
X(0x01224101), X(0x013a61b2), X(0x01537759), X(0x016d81b6),
X(0x01888087), X(0x01a47385), X(0x01c15a69), X(0x01df34e6),
X(0x01fe02b1), X(0x021dc377), X(0x023e76e7), X(0x02601ca9),
X(0x0282b466), X(0x02a63dc1), X(0x02cab85d), X(0x02f023d6),
X(0x03167fcb), X(0x033dcbd3), X(0x03660783), X(0x038f3270),
X(0x03b94c29), X(0x03e4543a), X(0x04104a2e), X(0x043d2d8b),
X(0x046afdd5), X(0x0499ba8c), X(0x04c9632d), X(0x04f9f734),
X(0x052b7615), X(0x055ddf46), X(0x05913237), X(0x05c56e53),
X(0x05fa9306), X(0x06309fb6), X(0x066793c5), X(0x069f6e93),
X(0x06d82f7c), X(0x0711d5d9), X(0x074c60fe), X(0x0787d03d),
X(0x07c422e4), X(0x0801583e), X(0x083f6f91), X(0x087e681f),
X(0x08be4129), X(0x08fef9ea), X(0x0940919a), X(0x0983076d),
X(0x09c65a92), X(0x0a0a8a38), X(0x0a4f9585), X(0x0a957b9f),
X(0x0adc3ba7), X(0x0b23d4b9), X(0x0b6c45ee), X(0x0bb58e5a),
X(0x0bffad0f), X(0x0c4aa11a), X(0x0c966982), X(0x0ce3054d),
X(0x0d30737b), X(0x0d7eb308), X(0x0dcdc2eb), X(0x0e1da21a),
X(0x0e6e4f83), X(0x0ebfca11), X(0x0f1210ad), X(0x0f652238),
X(0x0fb8fd91), X(0x100da192), X(0x10630d11), X(0x10b93ee0),
X(0x111035cb), X(0x1167f09a), X(0x11c06e13), X(0x1219acf5),
X(0x1273abfb), X(0x12ce69db), X(0x1329e54a), X(0x13861cf3),
X(0x13e30f80), X(0x1440bb97), X(0x149f1fd8), X(0x14fe3ade),
X(0x155e0b40), X(0x15be8f92), X(0x161fc662), X(0x1681ae38),
X(0x16e4459b), X(0x17478b0b), X(0x17ab7d03), X(0x181019fb),
X(0x18756067), X(0x18db4eb3), X(0x1941e34a), X(0x19a91c92),
X(0x1a10f8ea), X(0x1a7976af), X(0x1ae29439), X(0x1b4c4fda),
X(0x1bb6a7e2), X(0x1c219a9a), X(0x1c8d2649), X(0x1cf9492e),
X(0x1d660188), X(0x1dd34d8e), X(0x1e412b74), X(0x1eaf996a),
X(0x1f1e959b), X(0x1f8e1e2f), X(0x1ffe3146), X(0x206ecd01),
X(0x20dfef78), X(0x215196c2), X(0x21c3c0f0), X(0x22366c10),
X(0x22a9962a), X(0x231d3d45), X(0x23915f60), X(0x2405fa7a),
X(0x247b0c8c), X(0x24f09389), X(0x25668d65), X(0x25dcf80c),
X(0x2653d167), X(0x26cb175e), X(0x2742c7d0), X(0x27bae09e),
X(0x28335fa2), X(0x28ac42b3), X(0x292587a5), X(0x299f2c48),
X(0x2a192e69), X(0x2a938bd1), X(0x2b0e4247), X(0x2b894f8d),
X(0x2c04b164), X(0x2c806588), X(0x2cfc69b2), X(0x2d78bb9a),
X(0x2df558f4), X(0x2e723f6f), X(0x2eef6cbb), X(0x2f6cde83),
X(0x2fea9270), X(0x30688627), X(0x30e6b74e), X(0x31652385),
X(0x31e3c86b), X(0x3262a39e), X(0x32e1b2b8), X(0x3360f352),
X(0x33e06303), X(0x345fff5e), X(0x34dfc5f8), X(0x355fb462),
X(0x35dfc82a), X(0x365ffee0), X(0x36e0560f), X(0x3760cb43),
X(0x37e15c05), X(0x386205df), X(0x38e2c657), X(0x39639af5),
X(0x39e4813e), X(0x3a6576b6), X(0x3ae678e3), X(0x3b678547),
X(0x3be89965), X(0x3c69b2c1), X(0x3ceacedc), X(0x3d6beb37),
X(0x3ded0557), X(0x3e6e1abb), X(0x3eef28e6), X(0x3f702d5a),
X(0x3ff1259a), X(0x40720f29), X(0x40f2e789), X(0x4173ac3f),
X(0x41f45ad0), X(0x4274f0c2), X(0x42f56b9a), X(0x4375c8e0),
X(0x43f6061d), X(0x447620db), X(0x44f616a5), X(0x4575e509),
X(0x45f58994), X(0x467501d6), X(0x46f44b62), X(0x477363cb),
X(0x47f248a6), X(0x4870f78e), X(0x48ef6e1a), X(0x496da9e8),
X(0x49eba897), X(0x4a6967c8), X(0x4ae6e521), X(0x4b641e47),
X(0x4be110e5), X(0x4c5dbaa7), X(0x4cda193f), X(0x4d562a5f),
X(0x4dd1ebbd), X(0x4e4d5b15), X(0x4ec87623), X(0x4f433aa9),
X(0x4fbda66c), X(0x5037b734), X(0x50b16acf), X(0x512abf0e),
X(0x51a3b1c5), X(0x521c40ce), X(0x52946a06), X(0x530c2b50),
X(0x53838292), X(0x53fa6db8), X(0x5470eab3), X(0x54e6f776),
X(0x555c91fc), X(0x55d1b844), X(0x56466851), X(0x56baa02f),
X(0x572e5deb), X(0x57a19f98), X(0x58146352), X(0x5886a737),
X(0x58f8696d), X(0x5969a81c), X(0x59da6177), X(0x5a4a93b4),
X(0x5aba3d0f), X(0x5b295bcb), X(0x5b97ee30), X(0x5c05f28d),
X(0x5c736738), X(0x5ce04a8d), X(0x5d4c9aed), X(0x5db856c1),
X(0x5e237c78), X(0x5e8e0a89), X(0x5ef7ff6f), X(0x5f6159b0),
X(0x5fca17d4), X(0x6032386e), X(0x6099ba15), X(0x61009b69),
X(0x6166db11), X(0x61cc77b9), X(0x62317017), X(0x6295c2e7),
X(0x62f96eec), X(0x635c72f1), X(0x63becdc8), X(0x64207e4b),
X(0x6481835a), X(0x64e1dbde), X(0x654186c8), X(0x65a0830e),
X(0x65fecfb1), X(0x665c6bb7), X(0x66b95630), X(0x67158e30),
X(0x677112d7), X(0x67cbe34b), X(0x6825feb9), X(0x687f6456),
X(0x68d81361), X(0x69300b1e), X(0x69874ada), X(0x69ddd1ea),
X(0x6a339fab), X(0x6a88b382), X(0x6add0cdb), X(0x6b30ab2a),
X(0x6b838dec), X(0x6bd5b4a6), X(0x6c271ee2), X(0x6c77cc36),
X(0x6cc7bc3d), X(0x6d16ee9b), X(0x6d6562fb), X(0x6db31911),
X(0x6e001099), X(0x6e4c4955), X(0x6e97c311), X(0x6ee27d9f),
X(0x6f2c78d9), X(0x6f75b4a2), X(0x6fbe30e4), X(0x7005ed91),
X(0x704ceaa1), X(0x70932816), X(0x70d8a5f8), X(0x711d6457),
X(0x7161634b), X(0x71a4a2f3), X(0x71e72375), X(0x7228e500),
X(0x7269e7c8), X(0x72aa2c0a), X(0x72e9b209), X(0x73287a12),
X(0x73668476), X(0x73a3d18f), X(0x73e061bc), X(0x741c3566),
X(0x74574cfa), X(0x7491a8ee), X(0x74cb49be), X(0x75042fec),
X(0x753c5c03), X(0x7573ce92), X(0x75aa882f), X(0x75e08979),
X(0x7615d313), X(0x764a65a7), X(0x767e41e5), X(0x76b16884),
X(0x76e3da40), X(0x771597dc), X(0x7746a221), X(0x7776f9dd),
X(0x77a69fe6), X(0x77d59514), X(0x7803da49), X(0x7831706a),
X(0x785e5861), X(0x788a9320), X(0x78b6219c), X(0x78e104cf),
X(0x790b3dbb), X(0x7934cd64), X(0x795db4d5), X(0x7985f51d),
X(0x79ad8f50), X(0x79d48486), X(0x79fad5de), X(0x7a208478),
X(0x7a45917b), X(0x7a69fe12), X(0x7a8dcb6c), X(0x7ab0fabb),
X(0x7ad38d36), X(0x7af5841a), X(0x7b16e0a3), X(0x7b37a416),
X(0x7b57cfb8), X(0x7b7764d4), X(0x7b9664b6), X(0x7bb4d0b0),
X(0x7bd2aa14), X(0x7beff23b), X(0x7c0caa7f), X(0x7c28d43c),
X(0x7c4470d2), X(0x7c5f81a5), X(0x7c7a081a), X(0x7c940598),
X(0x7cad7b8b), X(0x7cc66b5e), X(0x7cded680), X(0x7cf6be64),
X(0x7d0e247b), X(0x7d250a3c), X(0x7d3b711c), X(0x7d515a95),
X(0x7d66c822), X(0x7d7bbb3c), X(0x7d903563), X(0x7da43814),
X(0x7db7c4d0), X(0x7dcadd16), X(0x7ddd826a), X(0x7defb64d),
X(0x7e017a44), X(0x7e12cfd3), X(0x7e23b87f), X(0x7e3435cc),
X(0x7e444943), X(0x7e53f467), X(0x7e6338c0), X(0x7e7217d5),
X(0x7e80932b), X(0x7e8eac49), X(0x7e9c64b7), X(0x7ea9bdf8),
X(0x7eb6b994), X(0x7ec35910), X(0x7ecf9def), X(0x7edb89b6),
X(0x7ee71de9), X(0x7ef25c09), X(0x7efd4598), X(0x7f07dc16),
X(0x7f122103), X(0x7f1c15dc), X(0x7f25bc1f), X(0x7f2f1547),
X(0x7f3822cd), X(0x7f40e62b), X(0x7f4960d6), X(0x7f519443),
X(0x7f5981e7), X(0x7f612b31), X(0x7f689191), X(0x7f6fb674),
X(0x7f769b45), X(0x7f7d416c), X(0x7f83aa51), X(0x7f89d757),
X(0x7f8fc9df), X(0x7f958348), X(0x7f9b04ef), X(0x7fa0502e),
X(0x7fa56659), X(0x7faa48c7), X(0x7faef8c7), X(0x7fb377a7),
X(0x7fb7c6b3), X(0x7fbbe732), X(0x7fbfda67), X(0x7fc3a196),
X(0x7fc73dfa), X(0x7fcab0ce), X(0x7fcdfb4a), X(0x7fd11ea0),
X(0x7fd41c00), X(0x7fd6f496), X(0x7fd9a989), X(0x7fdc3bff),
X(0x7fdead17), X(0x7fe0fdee), X(0x7fe32f9d), X(0x7fe54337),
X(0x7fe739ce), X(0x7fe9146c), X(0x7fead41b), X(0x7fec79dd),
X(0x7fee06b2), X(0x7fef7b94), X(0x7ff0d97b), X(0x7ff22158),
X(0x7ff35417), X(0x7ff472a3), X(0x7ff57de0), X(0x7ff676ac),
X(0x7ff75de3), X(0x7ff8345a), X(0x7ff8fae4), X(0x7ff9b24b),
X(0x7ffa5b58), X(0x7ffaf6cd), X(0x7ffb8568), X(0x7ffc07e2),
X(0x7ffc7eed), X(0x7ffceb38), X(0x7ffd4d6d), X(0x7ffda631),
X(0x7ffdf621), X(0x7ffe3dd8), X(0x7ffe7dea), X(0x7ffeb6e7),
X(0x7ffee959), X(0x7fff15c4), X(0x7fff3ca9), X(0x7fff5e80),
X(0x7fff7bc0), X(0x7fff94d6), X(0x7fffaa2d), X(0x7fffbc29),
X(0x7fffcb29), X(0x7fffd786), X(0x7fffe195), X(0x7fffe9a3),
X(0x7fffeffa), X(0x7ffff4dd), X(0x7ffff889), X(0x7ffffb37),
X(0x7ffffd1a), X(0x7ffffe5d), X(0x7fffff29), X(0x7fffffa0),
X(0x7fffffdd), X(0x7ffffff7), X(0x7fffffff), X(0x7fffffff),
};

static LOOKUP_T vwin2048[1024] = {
X(0x000007c0), X(0x000045c4), X(0x0000c1ca), X(0x00017bd3),
X(0x000273de), X(0x0003a9eb), X(0x00051df9), X(0x0006d007),
X(0x0008c014), X(0x000aee1e), X(0x000d5a25), X(0x00100428),
X(0x0012ec23), X(0x00161216), X(0x001975fe), X(0x001d17da),
X(0x0020f7a8), X(0x00251564), X(0x0029710c), X(0x002e0a9e),
X(0x0032e217), X(0x0037f773), X(0x003d4ab0), X(0x0042dbca),
X(0x0048aabe), X(0x004eb788), X(0x00550224), X(0x005b8a8f),
X(0x006250c5), X(0x006954c1), X(0x0070967e), X(0x007815f9),
X(0x007fd32c), X(0x0087ce13), X(0x009006a9), X(0x00987ce9),
X(0x00a130cc), X(0x00aa224f), X(0x00b3516b), X(0x00bcbe1a),
X(0x00c66856), X(0x00d0501a), X(0x00da755f), X(0x00e4d81f),
X(0x00ef7853), X(0x00fa55f4), X(0x010570fc), X(0x0110c963),
X(0x011c5f22), X(0x01283232), X(0x0134428c), X(0x01409027),
X(0x014d1afb), X(0x0159e302), X(0x0166e831), X(0x01742a82),
X(0x0181a9ec), X(0x018f6665), X(0x019d5fe5), X(0x01ab9663),
X(0x01ba09d6), X(0x01c8ba34), X(0x01d7a775), X(0x01e6d18d),
X(0x01f63873), X(0x0205dc1e), X(0x0215bc82), X(0x0225d997),
X(0x02363350), X(0x0246c9a3), X(0x02579c86), X(0x0268abed),
X(0x0279f7cc), X(0x028b801a), X(0x029d44c9), X(0x02af45ce),
X(0x02c1831d), X(0x02d3fcaa), X(0x02e6b269), X(0x02f9a44c),
X(0x030cd248), X(0x03203c4f), X(0x0333e255), X(0x0347c44b),
X(0x035be225), X(0x03703bd5), X(0x0384d14d), X(0x0399a280),
X(0x03aeaf5e), X(0x03c3f7d9), X(0x03d97be4), X(0x03ef3b6e),
X(0x0405366a), X(0x041b6cc8), X(0x0431de78), X(0x04488b6c),
X(0x045f7393), X(0x047696dd), X(0x048df53b), X(0x04a58e9b),
X(0x04bd62ee), X(0x04d57223), X(0x04edbc28), X(0x050640ed),
X(0x051f0060), X(0x0537fa70), X(0x05512f0a), X(0x056a9e1e),
X(0x05844798), X(0x059e2b67), X(0x05b84978), X(0x05d2a1b8),
X(0x05ed3414), X(0x06080079), X(0x062306d3), X(0x063e470f),
X(0x0659c119), X(0x067574dd), X(0x06916247), X(0x06ad8941),
X(0x06c9e9b8), X(0x06e68397), X(0x070356c8), X(0x07206336),
X(0x073da8cb), X(0x075b2772), X(0x0778df15), X(0x0796cf9c),
X(0x07b4f8f3), X(0x07d35b01), X(0x07f1f5b1), X(0x0810c8eb),
X(0x082fd497), X(0x084f189e), X(0x086e94e9), X(0x088e495e),
X(0x08ae35e6), X(0x08ce5a68), X(0x08eeb6cc), X(0x090f4af8),
X(0x093016d3), X(0x09511a44), X(0x09725530), X(0x0993c77f),
X(0x09b57115), X(0x09d751d8), X(0x09f969ae), X(0x0a1bb87c),
X(0x0a3e3e26), X(0x0a60fa91), X(0x0a83eda2), X(0x0aa7173c),
X(0x0aca7743), X(0x0aee0d9b), X(0x0b11da28), X(0x0b35dccc),
X(0x0b5a156a), X(0x0b7e83e5), X(0x0ba3281f), X(0x0bc801fa),
X(0x0bed1159), X(0x0c12561c), X(0x0c37d025), X(0x0c5d7f55),
X(0x0c83638d), X(0x0ca97cae), X(0x0ccfca97), X(0x0cf64d2a),
X(0x0d1d0444), X(0x0d43efc7), X(0x0d6b0f92), X(0x0d926383),
X(0x0db9eb79), X(0x0de1a752), X(0x0e0996ee), X(0x0e31ba29),
X(0x0e5a10e2), X(0x0e829af6), X(0x0eab5841), X(0x0ed448a2),
X(0x0efd6bf4), X(0x0f26c214), X(0x0f504ade), X(0x0f7a062e),
X(0x0fa3f3df), X(0x0fce13cd), X(0x0ff865d2), X(0x1022e9ca),
X(0x104d9f8e), X(0x107886f9), X(0x10a39fe5), X(0x10ceea2c),
X(0x10fa65a6), X(0x1126122d), X(0x1151ef9a), X(0x117dfdc5),
X(0x11aa3c87), X(0x11d6abb6), X(0x12034b2c), X(0x12301ac0),
X(0x125d1a48), X(0x128a499b), X(0x12b7a891), X(0x12e536ff),
X(0x1312f4bb), X(0x1340e19c), X(0x136efd75), X(0x139d481e),
X(0x13cbc16a), X(0x13fa692f), X(0x14293f40), X(0x14584371),
X(0x14877597), X(0x14b6d585), X(0x14e6630d), X(0x15161e04),
X(0x1546063b), X(0x15761b85), X(0x15a65db3), X(0x15d6cc99),
X(0x16076806), X(0x16382fcd), X(0x166923bf), X(0x169a43ab),
X(0x16cb8f62), X(0x16fd06b5), X(0x172ea973), X(0x1760776b),
X(0x1792706e), X(0x17c49449), X(0x17f6e2cb), X(0x18295bc3),
X(0x185bfeff), X(0x188ecc4c), X(0x18c1c379), X(0x18f4e452),
X(0x19282ea4), X(0x195ba23c), X(0x198f3ee6), X(0x19c3046e),
X(0x19f6f2a1), X(0x1a2b094a), X(0x1a5f4833), X(0x1a93af28),
X(0x1ac83df3), X(0x1afcf460), X(0x1b31d237), X(0x1b66d744),
X(0x1b9c034e), X(0x1bd15621), X(0x1c06cf84), X(0x1c3c6f40),
X(0x1c72351e), X(0x1ca820e6), X(0x1cde3260), X(0x1d146953),
X(0x1d4ac587), X(0x1d8146c3), X(0x1db7eccd), X(0x1deeb76c),
X(0x1e25a667), X(0x1e5cb982), X(0x1e93f085), X(0x1ecb4b33),
X(0x1f02c953), X(0x1f3a6aaa), X(0x1f722efb), X(0x1faa160b),
X(0x1fe21f9e), X(0x201a4b79), X(0x2052995d), X(0x208b0910),
X(0x20c39a53), X(0x20fc4cea), X(0x21352097), X(0x216e151c),
X(0x21a72a3a), X(0x21e05fb5), X(0x2219b54d), X(0x22532ac3),
X(0x228cbfd8), X(0x22c6744d), X(0x230047e2), X(0x233a3a58),
X(0x23744b6d), X(0x23ae7ae3), X(0x23e8c878), X(0x242333ec),
X(0x245dbcfd), X(0x24986369), X(0x24d326f1), X(0x250e0750),
X(0x25490446), X(0x25841d90), X(0x25bf52ec), X(0x25faa417),
X(0x263610cd), X(0x267198cc), X(0x26ad3bcf), X(0x26e8f994),
X(0x2724d1d6), X(0x2760c451), X(0x279cd0c0), X(0x27d8f6e0),
X(0x2815366a), X(0x28518f1b), X(0x288e00ac), X(0x28ca8ad8),
X(0x29072d5a), X(0x2943e7eb), X(0x2980ba45), X(0x29bda422),
X(0x29faa53c), X(0x2a37bd4a), X(0x2a74ec07), X(0x2ab2312b),
X(0x2aef8c6f), X(0x2b2cfd8b), X(0x2b6a8437), X(0x2ba8202c),
X(0x2be5d120), X(0x2c2396cc), X(0x2c6170e7), X(0x2c9f5f29),
X(0x2cdd6147), X(0x2d1b76fa), X(0x2d599ff7), X(0x2d97dbf5),
X(0x2dd62aab), X(0x2e148bcf), X(0x2e52ff16), X(0x2e918436),
X(0x2ed01ae5), X(0x2f0ec2d9), X(0x2f4d7bc6), X(0x2f8c4562),
X(0x2fcb1f62), X(0x300a097a), X(0x3049035f), X(0x30880cc6),
X(0x30c72563), X(0x31064cea), X(0x3145830f), X(0x3184c786),
X(0x31c41a03), X(0x32037a39), X(0x3242e7dc), X(0x3282629f),
X(0x32c1ea36), X(0x33017e53), X(0x33411ea9), X(0x3380caec),
X(0x33c082ce), X(0x34004602), X(0x34401439), X(0x347fed27),
X(0x34bfd07e), X(0x34ffbdf0), X(0x353fb52e), X(0x357fb5ec),
X(0x35bfbfda), X(0x35ffd2aa), X(0x363fee0f), X(0x368011b9),
X(0x36c03d5a), X(0x370070a4), X(0x3740ab48), X(0x3780ecf7),
X(0x37c13562), X(0x3801843a), X(0x3841d931), X(0x388233f7),
X(0x38c2943d), X(0x3902f9b4), X(0x3943640d), X(0x3983d2f8),
X(0x39c44626), X(0x3a04bd48), X(0x3a45380e), X(0x3a85b62a),
X(0x3ac6374a), X(0x3b06bb20), X(0x3b47415c), X(0x3b87c9ae),
X(0x3bc853c7), X(0x3c08df57), X(0x3c496c0f), X(0x3c89f99f),
X(0x3cca87b6), X(0x3d0b1605), X(0x3d4ba43d), X(0x3d8c320e),
X(0x3dccbf27), X(0x3e0d4b3a), X(0x3e4dd5f6), X(0x3e8e5f0c),
X(0x3ecee62b), X(0x3f0f6b05), X(0x3f4fed49), X(0x3f906ca8),
X(0x3fd0e8d2), X(0x40116177), X(0x4051d648), X(0x409246f6),
X(0x40d2b330), X(0x41131aa7), X(0x41537d0c), X(0x4193da10),
X(0x41d43162), X(0x421482b4), X(0x4254cdb7), X(0x4295121b),
X(0x42d54f91), X(0x431585ca), X(0x4355b477), X(0x4395db49),
X(0x43d5f9f1), X(0x44161021), X(0x44561d8a), X(0x449621dd),
X(0x44d61ccc), X(0x45160e08), X(0x4555f544), X(0x4595d230),
X(0x45d5a47f), X(0x46156be3), X(0x4655280e), X(0x4694d8b2),
X(0x46d47d82), X(0x4714162f), X(0x4753a26d), X(0x479321ef),
X(0x47d29466), X(0x4811f987), X(0x48515104), X(0x48909a91),
X(0x48cfd5e1), X(0x490f02a7), X(0x494e2098), X(0x498d2f66),
X(0x49cc2ec7), X(0x4a0b1e6f), X(0x4a49fe11), X(0x4a88cd62),
X(0x4ac78c18), X(0x4b0639e6), X(0x4b44d683), X(0x4b8361a2),
X(0x4bc1dafa), X(0x4c004241), X(0x4c3e972c), X(0x4c7cd970),
X(0x4cbb08c5), X(0x4cf924e1), X(0x4d372d7a), X(0x4d752247),
X(0x4db30300), X(0x4df0cf5a), X(0x4e2e870f), X(0x4e6c29d6),
X(0x4ea9b766), X(0x4ee72f78), X(0x4f2491c4), X(0x4f61de02),
X(0x4f9f13ec), X(0x4fdc333b), X(0x50193ba8), X(0x50562ced),
X(0x509306c3), X(0x50cfc8e5), X(0x510c730d), X(0x514904f6),
X(0x51857e5a), X(0x51c1def5), X(0x51fe2682), X(0x523a54bc),
X(0x52766961), X(0x52b2642c), X(0x52ee44d9), X(0x532a0b26),
X(0x5365b6d0), X(0x53a14793), X(0x53dcbd2f), X(0x54181760),
X(0x545355e5), X(0x548e787d), X(0x54c97ee6), X(0x550468e1),
X(0x553f362c), X(0x5579e687), X(0x55b479b3), X(0x55eeef70),
X(0x5629477f), X(0x566381a1), X(0x569d9d97), X(0x56d79b24),
X(0x57117a0a), X(0x574b3a0a), X(0x5784dae9), X(0x57be5c69),
X(0x57f7be4d), X(0x5831005a), X(0x586a2254), X(0x58a32400),
X(0x58dc0522), X(0x5914c57f), X(0x594d64de), X(0x5985e305),
X(0x59be3fba), X(0x59f67ac3), X(0x5a2e93e9), X(0x5a668af2),
X(0x5a9e5fa6), X(0x5ad611ce), X(0x5b0da133), X(0x5b450d9d),
X(0x5b7c56d7), X(0x5bb37ca9), X(0x5bea7ede), X(0x5c215d41),
X(0x5c58179d), X(0x5c8eadbe), X(0x5cc51f6f), X(0x5cfb6c7c),
X(0x5d3194b2), X(0x5d6797de), X(0x5d9d75cf), X(0x5dd32e51),
X(0x5e08c132), X(0x5e3e2e43), X(0x5e737551), X(0x5ea8962d),
X(0x5edd90a7), X(0x5f12648e), X(0x5f4711b4), X(0x5f7b97ea),
X(0x5faff702), X(0x5fe42ece), X(0x60183f20), X(0x604c27cc),
X(0x607fe8a6), X(0x60b38180), X(0x60e6f22f), X(0x611a3a89),
X(0x614d5a62), X(0x61805190), X(0x61b31fe9), X(0x61e5c545),
X(0x62184179), X(0x624a945d), X(0x627cbdca), X(0x62aebd98),
X(0x62e0939f), X(0x63123fba), X(0x6343c1c1), X(0x6375198f),
X(0x63a646ff), X(0x63d749ec), X(0x64082232), X(0x6438cfad),
X(0x64695238), X(0x6499a9b3), X(0x64c9d5f9), X(0x64f9d6ea),
X(0x6529ac63), X(0x65595643), X(0x6588d46a), X(0x65b826b8),
X(0x65e74d0e), X(0x6616474b), X(0x66451552), X(0x6673b704),
X(0x66a22c44), X(0x66d074f4), X(0x66fe90f8), X(0x672c8033),
X(0x675a428a), X(0x6787d7e1), X(0x67b5401f), X(0x67e27b27),
X(0x680f88e1), X(0x683c6934), X(0x68691c05), X(0x6895a13e),
X(0x68c1f8c7), X(0x68ee2287), X(0x691a1e68), X(0x6945ec54),
X(0x69718c35), X(0x699cfdf5), X(0x69c8417f), X(0x69f356c0),
X(0x6a1e3da3), X(0x6a48f615), X(0x6a738002), X(0x6a9ddb5a),
X(0x6ac80808), X(0x6af205fd), X(0x6b1bd526), X(0x6b457575),
X(0x6b6ee6d8), X(0x6b982940), X(0x6bc13c9f), X(0x6bea20e5),
X(0x6c12d605), X(0x6c3b5bf1), X(0x6c63b29c), X(0x6c8bd9fb),
X(0x6cb3d200), X(0x6cdb9aa0), X(0x6d0333d0), X(0x6d2a9d86),
X(0x6d51d7b7), X(0x6d78e25a), X(0x6d9fbd67), X(0x6dc668d3),
X(0x6dece498), X(0x6e1330ad), X(0x6e394d0c), X(0x6e5f39ae),
X(0x6e84f68d), X(0x6eaa83a2), X(0x6ecfe0ea), X(0x6ef50e5e),
X(0x6f1a0bfc), X(0x6f3ed9bf), X(0x6f6377a4), X(0x6f87e5a8),
X(0x6fac23c9), X(0x6fd03206), X(0x6ff4105c), X(0x7017becc),
X(0x703b3d54), X(0x705e8bf5), X(0x7081aaaf), X(0x70a49984),
X(0x70c75874), X(0x70e9e783), X(0x710c46b2), X(0x712e7605),
X(0x7150757f), X(0x71724523), X(0x7193e4f6), X(0x71b554fd),
X(0x71d6953e), X(0x71f7a5bd), X(0x72188681), X(0x72393792),
X(0x7259b8f5), X(0x727a0ab2), X(0x729a2cd2), X(0x72ba1f5d),
X(0x72d9e25c), X(0x72f975d8), X(0x7318d9db), X(0x73380e6f),
X(0x735713a0), X(0x7375e978), X(0x73949003), X(0x73b3074c),
X(0x73d14f61), X(0x73ef684f), X(0x740d5222), X(0x742b0ce9),
X(0x744898b1), X(0x7465f589), X(0x74832381), X(0x74a022a8),
X(0x74bcf30e), X(0x74d994c3), X(0x74f607d8), X(0x75124c5f),
X(0x752e6268), X(0x754a4a05), X(0x7566034b), X(0x75818e4a),
X(0x759ceb16), X(0x75b819c4), X(0x75d31a66), X(0x75eded12),
X(0x760891dc), X(0x762308da), X(0x763d5221), X(0x76576dc8),
X(0x76715be4), X(0x768b1c8c), X(0x76a4afd9), X(0x76be15e0),
X(0x76d74ebb), X(0x76f05a82), X(0x7709394d), X(0x7721eb35),
X(0x773a7054), X(0x7752c8c4), X(0x776af49f), X(0x7782f400),
X(0x779ac701), X(0x77b26dbd), X(0x77c9e851), X(0x77e136d8),
X(0x77f8596f), X(0x780f5032), X(0x78261b3f), X(0x783cbab2),
X(0x78532eaa), X(0x78697745), X(0x787f94a0), X(0x789586db),
X(0x78ab4e15), X(0x78c0ea6d), X(0x78d65c03), X(0x78eba2f7),
X(0x7900bf68), X(0x7915b179), X(0x792a7949), X(0x793f16fb),
X(0x79538aaf), X(0x7967d488), X(0x797bf4a8), X(0x798feb31),
X(0x79a3b846), X(0x79b75c0a), X(0x79cad6a1), X(0x79de282e),
X(0x79f150d5), X(0x7a0450bb), X(0x7a172803), X(0x7a29d6d3),
X(0x7a3c5d50), X(0x7a4ebb9f), X(0x7a60f1e6), X(0x7a73004a),
X(0x7a84e6f2), X(0x7a96a604), X(0x7aa83da7), X(0x7ab9ae01),
X(0x7acaf73a), X(0x7adc1979), X(0x7aed14e6), X(0x7afde9a8),
X(0x7b0e97e8), X(0x7b1f1fcd), X(0x7b2f8182), X(0x7b3fbd2d),
X(0x7b4fd2f9), X(0x7b5fc30f), X(0x7b6f8d98), X(0x7b7f32bd),
X(0x7b8eb2a9), X(0x7b9e0d85), X(0x7bad437d), X(0x7bbc54b9),
X(0x7bcb4166), X(0x7bda09ae), X(0x7be8adbc), X(0x7bf72dbc),
X(0x7c0589d8), X(0x7c13c23d), X(0x7c21d716), X(0x7c2fc88f),
X(0x7c3d96d5), X(0x7c4b4214), X(0x7c58ca78), X(0x7c66302d),
X(0x7c737362), X(0x7c809443), X(0x7c8d92fc), X(0x7c9a6fbc),
X(0x7ca72aaf), X(0x7cb3c404), X(0x7cc03be8), X(0x7ccc9288),
X(0x7cd8c814), X(0x7ce4dcb9), X(0x7cf0d0a5), X(0x7cfca406),
X(0x7d08570c), X(0x7d13e9e5), X(0x7d1f5cbf), X(0x7d2aafca),
X(0x7d35e335), X(0x7d40f72e), X(0x7d4bebe4), X(0x7d56c188),
X(0x7d617848), X(0x7d6c1054), X(0x7d7689db), X(0x7d80e50e),
X(0x7d8b221b), X(0x7d954133), X(0x7d9f4286), X(0x7da92643),
X(0x7db2ec9b), X(0x7dbc95bd), X(0x7dc621da), X(0x7dcf9123),
X(0x7dd8e3c6), X(0x7de219f6), X(0x7deb33e2), X(0x7df431ba),
X(0x7dfd13af), X(0x7e05d9f2), X(0x7e0e84b4), X(0x7e171424),
X(0x7e1f8874), X(0x7e27e1d4), X(0x7e302074), X(0x7e384487),
X(0x7e404e3c), X(0x7e483dc4), X(0x7e501350), X(0x7e57cf11),
X(0x7e5f7138), X(0x7e66f9f4), X(0x7e6e6979), X(0x7e75bff5),
X(0x7e7cfd9a), X(0x7e842298), X(0x7e8b2f22), X(0x7e922366),
X(0x7e98ff97), X(0x7e9fc3e4), X(0x7ea6707f), X(0x7ead0598),
X(0x7eb38360), X(0x7eb9ea07), X(0x7ec039bf), X(0x7ec672b7),
X(0x7ecc9521), X(0x7ed2a12c), X(0x7ed8970a), X(0x7ede76ea),
X(0x7ee440fd), X(0x7ee9f573), X(0x7eef947d), X(0x7ef51e4b),
X(0x7efa930d), X(0x7efff2f2), X(0x7f053e2b), X(0x7f0a74e8),
X(0x7f0f9758), X(0x7f14a5ac), X(0x7f19a013), X(0x7f1e86bc),
X(0x7f2359d8), X(0x7f281995), X(0x7f2cc623), X(0x7f315fb1),
X(0x7f35e66e), X(0x7f3a5a8a), X(0x7f3ebc33), X(0x7f430b98),
X(0x7f4748e7), X(0x7f4b7450), X(0x7f4f8e01), X(0x7f539629),
X(0x7f578cf5), X(0x7f5b7293), X(0x7f5f4732), X(0x7f630b00),
X(0x7f66be2b), X(0x7f6a60df), X(0x7f6df34b), X(0x7f71759b),
X(0x7f74e7fe), X(0x7f784aa0), X(0x7f7b9daf), X(0x7f7ee156),
X(0x7f8215c3), X(0x7f853b22), X(0x7f88519f), X(0x7f8b5967),
X(0x7f8e52a6), X(0x7f913d87), X(0x7f941a36), X(0x7f96e8df),
X(0x7f99a9ad), X(0x7f9c5ccb), X(0x7f9f0265), X(0x7fa19aa5),
X(0x7fa425b5), X(0x7fa6a3c1), X(0x7fa914f3), X(0x7fab7974),
X(0x7fadd16f), X(0x7fb01d0d), X(0x7fb25c78), X(0x7fb48fd9),
X(0x7fb6b75a), X(0x7fb8d323), X(0x7fbae35d), X(0x7fbce831),
X(0x7fbee1c7), X(0x7fc0d047), X(0x7fc2b3d9), X(0x7fc48ca5),
X(0x7fc65ad3), X(0x7fc81e88), X(0x7fc9d7ee), X(0x7fcb872a),
X(0x7fcd2c63), X(0x7fcec7bf), X(0x7fd05966), X(0x7fd1e17c),
X(0x7fd36027), X(0x7fd4d58d), X(0x7fd641d3), X(0x7fd7a51e),
X(0x7fd8ff94), X(0x7fda5157), X(0x7fdb9a8e), X(0x7fdcdb5b),
X(0x7fde13e2), X(0x7fdf4448), X(0x7fe06caf), X(0x7fe18d3b),
X(0x7fe2a60e), X(0x7fe3b74b), X(0x7fe4c114), X(0x7fe5c38b),
X(0x7fe6bed2), X(0x7fe7b30a), X(0x7fe8a055), X(0x7fe986d4),
X(0x7fea66a7), X(0x7feb3ff0), X(0x7fec12cd), X(0x7fecdf5f),
X(0x7feda5c5), X(0x7fee6620), X(0x7fef208d), X(0x7fefd52c),
X(0x7ff0841c), X(0x7ff12d7a), X(0x7ff1d164), X(0x7ff26ff9),
X(0x7ff30955), X(0x7ff39d96), X(0x7ff42cd9), X(0x7ff4b739),
X(0x7ff53cd4), X(0x7ff5bdc5), X(0x7ff63a28), X(0x7ff6b217),
X(0x7ff725af), X(0x7ff7950a), X(0x7ff80043), X(0x7ff86773),
X(0x7ff8cab4), X(0x7ff92a21), X(0x7ff985d1), X(0x7ff9dddf),
X(0x7ffa3262), X(0x7ffa8374), X(0x7ffad12c), X(0x7ffb1ba1),
X(0x7ffb62ec), X(0x7ffba723), X(0x7ffbe85c), X(0x7ffc26b0),
X(0x7ffc6233), X(0x7ffc9afb), X(0x7ffcd11e), X(0x7ffd04b1),
X(0x7ffd35c9), X(0x7ffd647b), X(0x7ffd90da), X(0x7ffdbafa),
X(0x7ffde2f0), X(0x7ffe08ce), X(0x7ffe2ca7), X(0x7ffe4e8e),
X(0x7ffe6e95), X(0x7ffe8cce), X(0x7ffea94a), X(0x7ffec41b),
X(0x7ffedd52), X(0x7ffef4ff), X(0x7fff0b33), X(0x7fff1ffd),
X(0x7fff336e), X(0x7fff4593), X(0x7fff567d), X(0x7fff663a),
X(0x7fff74d8), X(0x7fff8265), X(0x7fff8eee), X(0x7fff9a81),
X(0x7fffa52b), X(0x7fffaef8), X(0x7fffb7f5), X(0x7fffc02d),
X(0x7fffc7ab), X(0x7fffce7c), X(0x7fffd4a9), X(0x7fffda3e),
X(0x7fffdf44), X(0x7fffe3c6), X(0x7fffe7cc), X(0x7fffeb60),
X(0x7fffee8a), X(0x7ffff153), X(0x7ffff3c4), X(0x7ffff5e3),
X(0x7ffff7b8), X(0x7ffff94b), X(0x7ffffaa1), X(0x7ffffbc1),
X(0x7ffffcb2), X(0x7ffffd78), X(0x7ffffe19), X(0x7ffffe9a),
X(0x7ffffeff), X(0x7fffff4e), X(0x7fffff89), X(0x7fffffb3),
X(0x7fffffd2), X(0x7fffffe6), X(0x7ffffff3), X(0x7ffffffa),
X(0x7ffffffe), X(0x7fffffff), X(0x7fffffff), X(0x7fffffff),
};

static LOOKUP_T vwin4096[2048] = {
X(0x000001f0), X(0x00001171), X(0x00003072), X(0x00005ef5),
X(0x00009cf8), X(0x0000ea7c), X(0x00014780), X(0x0001b405),
X(0x0002300b), X(0x0002bb91), X(0x00035698), X(0x0004011e),
X(0x0004bb25), X(0x000584ac), X(0x00065db3), X(0x0007463a),
X(0x00083e41), X(0x000945c7), X(0x000a5ccc), X(0x000b8350),
X(0x000cb954), X(0x000dfed7), X(0x000f53d8), X(0x0010b857),
X(0x00122c55), X(0x0013afd1), X(0x001542ca), X(0x0016e541),
X(0x00189735), X(0x001a58a7), X(0x001c2995), X(0x001e09ff),
X(0x001ff9e6), X(0x0021f948), X(0x00240826), X(0x00262680),
X(0x00285454), X(0x002a91a3), X(0x002cde6c), X(0x002f3aaf),
X(0x0031a66b), X(0x003421a0), X(0x0036ac4f), X(0x00394675),
X(0x003bf014), X(0x003ea92a), X(0x004171b7), X(0x004449bb),
X(0x00473135), X(0x004a2824), X(0x004d2e8a), X(0x00504463),
X(0x005369b2), X(0x00569e74), X(0x0059e2aa), X(0x005d3652),
X(0x0060996d), X(0x00640bf9), X(0x00678df7), X(0x006b1f66),
X(0x006ec045), X(0x00727093), X(0x00763051), X(0x0079ff7d),
X(0x007dde16), X(0x0081cc1d), X(0x0085c991), X(0x0089d671),
X(0x008df2bc), X(0x00921e71), X(0x00965991), X(0x009aa41a),
X(0x009efe0c), X(0x00a36766), X(0x00a7e028), X(0x00ac6850),
X(0x00b0ffde), X(0x00b5a6d1), X(0x00ba5d28), X(0x00bf22e4),
X(0x00c3f802), X(0x00c8dc83), X(0x00cdd065), X(0x00d2d3a8),
X(0x00d7e64a), X(0x00dd084c), X(0x00e239ac), X(0x00e77a69),
X(0x00ecca83), X(0x00f229f9), X(0x00f798ca), X(0x00fd16f5),
X(0x0102a479), X(0x01084155), X(0x010ded89), X(0x0113a913),
X(0x011973f3), X(0x011f4e27), X(0x012537af), X(0x012b308a),
X(0x013138b7), X(0x01375035), X(0x013d7702), X(0x0143ad1f),
X(0x0149f289), X(0x01504741), X(0x0156ab44), X(0x015d1e92),
X(0x0163a12a), X(0x016a330b), X(0x0170d433), X(0x017784a3),
X(0x017e4458), X(0x01851351), X(0x018bf18e), X(0x0192df0d),
X(0x0199dbcd), X(0x01a0e7cd), X(0x01a8030c), X(0x01af2d89),
X(0x01b66743), X(0x01bdb038), X(0x01c50867), X(0x01cc6fd0),
X(0x01d3e670), X(0x01db6c47), X(0x01e30153), X(0x01eaa593),
X(0x01f25907), X(0x01fa1bac), X(0x0201ed81), X(0x0209ce86),
X(0x0211beb8), X(0x0219be17), X(0x0221cca2), X(0x0229ea56),
X(0x02321733), X(0x023a5337), X(0x02429e60), X(0x024af8af),
X(0x02536220), X(0x025bdab3), X(0x02646267), X(0x026cf93a),
X(0x02759f2a), X(0x027e5436), X(0x0287185d), X(0x028feb9d),
X(0x0298cdf4), X(0x02a1bf62), X(0x02aabfe5), X(0x02b3cf7b),
X(0x02bcee23), X(0x02c61bdb), X(0x02cf58a2), X(0x02d8a475),
X(0x02e1ff55), X(0x02eb693e), X(0x02f4e230), X(0x02fe6a29),
X(0x03080127), X(0x0311a729), X(0x031b5c2d), X(0x03252031),
X(0x032ef334), X(0x0338d534), X(0x0342c630), X(0x034cc625),
X(0x0356d512), X(0x0360f2f6), X(0x036b1fce), X(0x03755b99),
X(0x037fa655), X(0x038a0001), X(0x0394689a), X(0x039ee020),
X(0x03a9668f), X(0x03b3fbe6), X(0x03bea024), X(0x03c95347),
X(0x03d4154d), X(0x03dee633), X(0x03e9c5f9), X(0x03f4b49b),
X(0x03ffb219), X(0x040abe71), X(0x0415d9a0), X(0x042103a5),
X(0x042c3c7d), X(0x04378428), X(0x0442daa2), X(0x044e3fea),
X(0x0459b3fd), X(0x046536db), X(0x0470c880), X(0x047c68eb),
X(0x0488181a), X(0x0493d60b), X(0x049fa2bc), X(0x04ab7e2a),
X(0x04b76854), X(0x04c36137), X(0x04cf68d1), X(0x04db7f21),
X(0x04e7a424), X(0x04f3d7d8), X(0x05001a3b), X(0x050c6b4a),
X(0x0518cb04), X(0x05253966), X(0x0531b66e), X(0x053e421a),
X(0x054adc68), X(0x05578555), X(0x05643cdf), X(0x05710304),
X(0x057dd7c1), X(0x058abb15), X(0x0597acfd), X(0x05a4ad76),
X(0x05b1bc7f), X(0x05beda14), X(0x05cc0635), X(0x05d940dd),
X(0x05e68a0b), X(0x05f3e1bd), X(0x060147f0), X(0x060ebca1),
X(0x061c3fcf), X(0x0629d176), X(0x06377194), X(0x06452027),
X(0x0652dd2c), X(0x0660a8a2), X(0x066e8284), X(0x067c6ad1),
X(0x068a6186), X(0x069866a1), X(0x06a67a1e), X(0x06b49bfc),
X(0x06c2cc38), X(0x06d10acf), X(0x06df57bf), X(0x06edb304),
X(0x06fc1c9d), X(0x070a9487), X(0x07191abe), X(0x0727af40),
X(0x0736520b), X(0x0745031c), X(0x0753c270), X(0x07629004),
X(0x07716bd6), X(0x078055e2), X(0x078f4e26), X(0x079e549f),
X(0x07ad694b), X(0x07bc8c26), X(0x07cbbd2e), X(0x07dafc5f),
X(0x07ea49b7), X(0x07f9a533), X(0x08090ed1), X(0x0818868c),
X(0x08280c62), X(0x0837a051), X(0x08474255), X(0x0856f26b),
X(0x0866b091), X(0x08767cc3), X(0x088656fe), X(0x08963f3f),
X(0x08a63584), X(0x08b639c8), X(0x08c64c0a), X(0x08d66c45),
X(0x08e69a77), X(0x08f6d69d), X(0x090720b3), X(0x091778b7),
X(0x0927dea5), X(0x0938527a), X(0x0948d433), X(0x095963cc),
X(0x096a0143), X(0x097aac94), X(0x098b65bb), X(0x099c2cb6),
X(0x09ad0182), X(0x09bde41a), X(0x09ced47d), X(0x09dfd2a5),
X(0x09f0de90), X(0x0a01f83b), X(0x0a131fa3), X(0x0a2454c3),
X(0x0a359798), X(0x0a46e820), X(0x0a584656), X(0x0a69b237),
X(0x0a7b2bc0), X(0x0a8cb2ec), X(0x0a9e47ba), X(0x0aafea24),
X(0x0ac19a29), X(0x0ad357c3), X(0x0ae522ef), X(0x0af6fbab),
X(0x0b08e1f1), X(0x0b1ad5c0), X(0x0b2cd712), X(0x0b3ee5e5),
X(0x0b510234), X(0x0b632bfd), X(0x0b75633b), X(0x0b87a7eb),
X(0x0b99fa08), X(0x0bac5990), X(0x0bbec67e), X(0x0bd140cf),
X(0x0be3c87e), X(0x0bf65d89), X(0x0c08ffeb), X(0x0c1bafa1),
X(0x0c2e6ca6), X(0x0c4136f6), X(0x0c540e8f), X(0x0c66f36c),
X(0x0c79e588), X(0x0c8ce4e1), X(0x0c9ff172), X(0x0cb30b37),
X(0x0cc6322c), X(0x0cd9664d), X(0x0ceca797), X(0x0cfff605),
X(0x0d135193), X(0x0d26ba3d), X(0x0d3a2fff), X(0x0d4db2d5),
X(0x0d6142ba), X(0x0d74dfac), X(0x0d8889a5), X(0x0d9c40a1),
X(0x0db0049d), X(0x0dc3d593), X(0x0dd7b380), X(0x0deb9e60),
X(0x0dff962f), X(0x0e139ae7), X(0x0e27ac85), X(0x0e3bcb05),
X(0x0e4ff662), X(0x0e642e98), X(0x0e7873a2), X(0x0e8cc57d),
X(0x0ea12423), X(0x0eb58f91), X(0x0eca07c2), X(0x0ede8cb1),
X(0x0ef31e5b), X(0x0f07bcba), X(0x0f1c67cb), X(0x0f311f88),
X(0x0f45e3ee), X(0x0f5ab4f7), X(0x0f6f92a0), X(0x0f847ce3),
X(0x0f9973bc), X(0x0fae7726), X(0x0fc3871e), X(0x0fd8a39d),
X(0x0fedcca1), X(0x10030223), X(0x1018441f), X(0x102d9291),
X(0x1042ed74), X(0x105854c3), X(0x106dc879), X(0x10834892),
X(0x1098d508), X(0x10ae6dd8), X(0x10c412fc), X(0x10d9c46f),
X(0x10ef822d), X(0x11054c30), X(0x111b2274), X(0x113104f5),
X(0x1146f3ac), X(0x115cee95), X(0x1172f5ab), X(0x118908e9),
X(0x119f284a), X(0x11b553ca), X(0x11cb8b62), X(0x11e1cf0f),
X(0x11f81ecb), X(0x120e7a90), X(0x1224e25a), X(0x123b5624),
X(0x1251d5e9), X(0x126861a3), X(0x127ef94e), X(0x12959ce3),
X(0x12ac4c5f), X(0x12c307bb), X(0x12d9cef2), X(0x12f0a200),
X(0x130780df), X(0x131e6b8a), X(0x133561fa), X(0x134c642c),
X(0x1363721a), X(0x137a8bbe), X(0x1391b113), X(0x13a8e214),
X(0x13c01eba), X(0x13d76702), X(0x13eebae5), X(0x14061a5e),
X(0x141d8567), X(0x1434fbfb), X(0x144c7e14), X(0x14640bae),
X(0x147ba4c1), X(0x14934949), X(0x14aaf941), X(0x14c2b4a2),
X(0x14da7b67), X(0x14f24d8a), X(0x150a2b06), X(0x152213d5),
X(0x153a07f1), X(0x15520755), X(0x156a11fb), X(0x158227dd),
X(0x159a48f5), X(0x15b2753d), X(0x15caacb1), X(0x15e2ef49),
X(0x15fb3d01), X(0x161395d2), X(0x162bf9b6), X(0x164468a8),
X(0x165ce2a1), X(0x1675679c), X(0x168df793), X(0x16a69280),
X(0x16bf385c), X(0x16d7e922), X(0x16f0a4cc), X(0x17096b54),
X(0x17223cb4), X(0x173b18e5), X(0x1753ffe2), X(0x176cf1a5),
X(0x1785ee27), X(0x179ef562), X(0x17b80750), X(0x17d123eb),
X(0x17ea4b2d), X(0x18037d10), X(0x181cb98d), X(0x1836009e),
X(0x184f523c), X(0x1868ae63), X(0x1882150a), X(0x189b862c),
X(0x18b501c4), X(0x18ce87c9), X(0x18e81836), X(0x1901b305),
X(0x191b582f), X(0x193507ad), X(0x194ec17a), X(0x1968858f),
X(0x198253e5), X(0x199c2c75), X(0x19b60f3a), X(0x19cffc2d),
X(0x19e9f347), X(0x1a03f482), X(0x1a1dffd7), X(0x1a381540),
X(0x1a5234b5), X(0x1a6c5e31), X(0x1a8691ac), X(0x1aa0cf21),
X(0x1abb1687), X(0x1ad567da), X(0x1aefc311), X(0x1b0a2826),
X(0x1b249712), X(0x1b3f0fd0), X(0x1b599257), X(0x1b741ea1),
X(0x1b8eb4a7), X(0x1ba95462), X(0x1bc3fdcd), X(0x1bdeb0de),
X(0x1bf96d91), X(0x1c1433dd), X(0x1c2f03bc), X(0x1c49dd27),
X(0x1c64c017), X(0x1c7fac85), X(0x1c9aa269), X(0x1cb5a1be),
X(0x1cd0aa7c), X(0x1cebbc9c), X(0x1d06d816), X(0x1d21fce4),
X(0x1d3d2aff), X(0x1d586260), X(0x1d73a2fe), X(0x1d8eecd4),
X(0x1daa3fda), X(0x1dc59c09), X(0x1de1015a), X(0x1dfc6fc5),
X(0x1e17e743), X(0x1e3367cd), X(0x1e4ef15b), X(0x1e6a83e7),
X(0x1e861f6a), X(0x1ea1c3da), X(0x1ebd7133), X(0x1ed9276b),
X(0x1ef4e67c), X(0x1f10ae5e), X(0x1f2c7f0a), X(0x1f485879),
X(0x1f643aa2), X(0x1f80257f), X(0x1f9c1908), X(0x1fb81536),
X(0x1fd41a00), X(0x1ff02761), X(0x200c3d4f), X(0x20285bc3),
X(0x204482b7), X(0x2060b221), X(0x207ce9fb), X(0x20992a3e),
X(0x20b572e0), X(0x20d1c3dc), X(0x20ee1d28), X(0x210a7ebe),
X(0x2126e895), X(0x21435aa6), X(0x215fd4ea), X(0x217c5757),
X(0x2198e1e8), X(0x21b57493), X(0x21d20f51), X(0x21eeb21b),
X(0x220b5ce7), X(0x22280fb0), X(0x2244ca6c), X(0x22618d13),
X(0x227e579f), X(0x229b2a06), X(0x22b80442), X(0x22d4e649),
X(0x22f1d015), X(0x230ec19d), X(0x232bbad9), X(0x2348bbc1),
X(0x2365c44c), X(0x2382d474), X(0x239fec30), X(0x23bd0b78),
X(0x23da3244), X(0x23f7608b), X(0x24149646), X(0x2431d36c),
X(0x244f17f5), X(0x246c63da), X(0x2489b711), X(0x24a71193),
X(0x24c47358), X(0x24e1dc57), X(0x24ff4c88), X(0x251cc3e2),
X(0x253a425e), X(0x2557c7f4), X(0x2575549a), X(0x2592e848),
X(0x25b082f7), X(0x25ce249e), X(0x25ebcd34), X(0x26097cb2),
X(0x2627330e), X(0x2644f040), X(0x2662b441), X(0x26807f07),
X(0x269e5089), X(0x26bc28c1), X(0x26da07a4), X(0x26f7ed2b),
X(0x2715d94d), X(0x2733cc02), X(0x2751c540), X(0x276fc500),
X(0x278dcb39), X(0x27abd7e2), X(0x27c9eaf3), X(0x27e80463),
X(0x28062429), X(0x28244a3e), X(0x28427697), X(0x2860a92d),
X(0x287ee1f7), X(0x289d20eb), X(0x28bb6603), X(0x28d9b134),
X(0x28f80275), X(0x291659c0), X(0x2934b709), X(0x29531a49),
X(0x29718378), X(0x298ff28b), X(0x29ae677b), X(0x29cce23e),
X(0x29eb62cb), X(0x2a09e91b), X(0x2a287523), X(0x2a4706dc),
X(0x2a659e3c), X(0x2a843b39), X(0x2aa2ddcd), X(0x2ac185ec),
X(0x2ae0338f), X(0x2afee6ad), X(0x2b1d9f3c), X(0x2b3c5d33),
X(0x2b5b208b), X(0x2b79e939), X(0x2b98b734), X(0x2bb78a74),
X(0x2bd662ef), X(0x2bf5409d), X(0x2c142374), X(0x2c330b6b),
X(0x2c51f87a), X(0x2c70ea97), X(0x2c8fe1b9), X(0x2caeddd6),
X(0x2ccddee7), X(0x2cece4e1), X(0x2d0befbb), X(0x2d2aff6d),
X(0x2d4a13ec), X(0x2d692d31), X(0x2d884b32), X(0x2da76de4),
X(0x2dc69540), X(0x2de5c13d), X(0x2e04f1d0), X(0x2e2426f0),
X(0x2e436095), X(0x2e629eb4), X(0x2e81e146), X(0x2ea1283f),
X(0x2ec07398), X(0x2edfc347), X(0x2eff1742), X(0x2f1e6f80),
X(0x2f3dcbf8), X(0x2f5d2ca0), X(0x2f7c916f), X(0x2f9bfa5c),
X(0x2fbb675d), X(0x2fdad869), X(0x2ffa4d76), X(0x3019c67b),
X(0x3039436f), X(0x3058c448), X(0x307848fc), X(0x3097d183),
X(0x30b75dd3), X(0x30d6ede2), X(0x30f681a6), X(0x31161917),
X(0x3135b42b), X(0x315552d8), X(0x3174f514), X(0x31949ad7),
X(0x31b44417), X(0x31d3f0ca), X(0x31f3a0e6), X(0x32135462),
X(0x32330b35), X(0x3252c555), X(0x327282b7), X(0x32924354),
X(0x32b20720), X(0x32d1ce13), X(0x32f19823), X(0x33116546),
X(0x33313573), X(0x3351089f), X(0x3370dec2), X(0x3390b7d1),
X(0x33b093c3), X(0x33d0728f), X(0x33f05429), X(0x3410388a),
X(0x34301fa7), X(0x34500977), X(0x346ff5ef), X(0x348fe506),
X(0x34afd6b3), X(0x34cfcaeb), X(0x34efc1a5), X(0x350fbad7),
X(0x352fb678), X(0x354fb47d), X(0x356fb4dd), X(0x358fb78e),
X(0x35afbc86), X(0x35cfc3bc), X(0x35efcd25), X(0x360fd8b8),
X(0x362fe66c), X(0x364ff636), X(0x3670080c), X(0x36901be5),
X(0x36b031b7), X(0x36d04978), X(0x36f0631e), X(0x37107ea0),
X(0x37309bf3), X(0x3750bb0e), X(0x3770dbe6), X(0x3790fe73),
X(0x37b122aa), X(0x37d14881), X(0x37f16fee), X(0x381198e8),
X(0x3831c365), X(0x3851ef5a), X(0x38721cbe), X(0x38924b87),
X(0x38b27bac), X(0x38d2ad21), X(0x38f2dfde), X(0x391313d8),
X(0x39334906), X(0x39537f5d), X(0x3973b6d4), X(0x3993ef60),
X(0x39b428f9), X(0x39d46393), X(0x39f49f25), X(0x3a14dba6),
X(0x3a35190a), X(0x3a555748), X(0x3a759657), X(0x3a95d62c),
X(0x3ab616be), X(0x3ad65801), X(0x3af699ed), X(0x3b16dc78),
X(0x3b371f97), X(0x3b576341), X(0x3b77a76c), X(0x3b97ec0d),
X(0x3bb8311b), X(0x3bd8768b), X(0x3bf8bc55), X(0x3c19026d),
X(0x3c3948ca), X(0x3c598f62), X(0x3c79d62b), X(0x3c9a1d1b),
X(0x3cba6428), X(0x3cdaab48), X(0x3cfaf271), X(0x3d1b3999),
X(0x3d3b80b6), X(0x3d5bc7be), X(0x3d7c0ea8), X(0x3d9c5569),
X(0x3dbc9bf7), X(0x3ddce248), X(0x3dfd2852), X(0x3e1d6e0c),
X(0x3e3db36c), X(0x3e5df866), X(0x3e7e3cf2), X(0x3e9e8106),
X(0x3ebec497), X(0x3edf079b), X(0x3eff4a09), X(0x3f1f8bd7),
X(0x3f3fccfa), X(0x3f600d69), X(0x3f804d1a), X(0x3fa08c02),
X(0x3fc0ca19), X(0x3fe10753), X(0x400143a7), X(0x40217f0a),
X(0x4041b974), X(0x4061f2da), X(0x40822b32), X(0x40a26272),
X(0x40c29891), X(0x40e2cd83), X(0x41030140), X(0x412333bd),
X(0x414364f1), X(0x416394d2), X(0x4183c355), X(0x41a3f070),
X(0x41c41c1b), X(0x41e4464a), X(0x42046ef4), X(0x42249610),
X(0x4244bb92), X(0x4264df72), X(0x428501a5), X(0x42a52222),
X(0x42c540de), X(0x42e55dd0), X(0x430578ed), X(0x4325922d),
X(0x4345a985), X(0x4365beeb), X(0x4385d255), X(0x43a5e3ba),
X(0x43c5f30f), X(0x43e6004b), X(0x44060b65), X(0x44261451),
X(0x44461b07), X(0x44661f7c), X(0x448621a7), X(0x44a6217d),
X(0x44c61ef6), X(0x44e61a07), X(0x450612a6), X(0x452608ca),
X(0x4545fc69), X(0x4565ed79), X(0x4585dbf1), X(0x45a5c7c6),
X(0x45c5b0ef), X(0x45e59761), X(0x46057b15), X(0x46255bfe),
X(0x46453a15), X(0x4665154f), X(0x4684eda2), X(0x46a4c305),
X(0x46c4956e), X(0x46e464d3), X(0x4704312b), X(0x4723fa6c),
X(0x4743c08d), X(0x47638382), X(0x47834344), X(0x47a2ffc9),
X(0x47c2b906), X(0x47e26ef2), X(0x48022183), X(0x4821d0b1),
X(0x48417c71), X(0x486124b9), X(0x4880c981), X(0x48a06abe),
X(0x48c00867), X(0x48dfa272), X(0x48ff38d6), X(0x491ecb8a),
X(0x493e5a84), X(0x495de5b9), X(0x497d6d22), X(0x499cf0b4),
X(0x49bc7066), X(0x49dbec2e), X(0x49fb6402), X(0x4a1ad7db),
X(0x4a3a47ad), X(0x4a59b370), X(0x4a791b1a), X(0x4a987ea1),
X(0x4ab7ddfd), X(0x4ad73924), X(0x4af6900c), X(0x4b15e2ad),
X(0x4b3530fc), X(0x4b547af1), X(0x4b73c082), X(0x4b9301a6),
X(0x4bb23e53), X(0x4bd17681), X(0x4bf0aa25), X(0x4c0fd937),
X(0x4c2f03ae), X(0x4c4e297f), X(0x4c6d4aa3), X(0x4c8c670f),
X(0x4cab7eba), X(0x4cca919c), X(0x4ce99fab), X(0x4d08a8de),
X(0x4d27ad2c), X(0x4d46ac8b), X(0x4d65a6f3), X(0x4d849c5a),
X(0x4da38cb7), X(0x4dc27802), X(0x4de15e31), X(0x4e003f3a),
X(0x4e1f1b16), X(0x4e3df1ba), X(0x4e5cc31e), X(0x4e7b8f3a),
X(0x4e9a5603), X(0x4eb91771), X(0x4ed7d37b), X(0x4ef68a18),
X(0x4f153b3f), X(0x4f33e6e7), X(0x4f528d08), X(0x4f712d97),
X(0x4f8fc88e), X(0x4fae5de1), X(0x4fcced8a), X(0x4feb777f),
X(0x5009fbb6), X(0x50287a28), X(0x5046f2cc), X(0x50656598),
X(0x5083d284), X(0x50a23988), X(0x50c09a9a), X(0x50def5b1),
X(0x50fd4ac7), X(0x511b99d0), X(0x5139e2c5), X(0x5158259e),
X(0x51766251), X(0x519498d6), X(0x51b2c925), X(0x51d0f334),
X(0x51ef16fb), X(0x520d3473), X(0x522b4b91), X(0x52495c4e),
X(0x526766a2), X(0x52856a83), X(0x52a367e9), X(0x52c15ecd),
X(0x52df4f24), X(0x52fd38e8), X(0x531b1c10), X(0x5338f892),
X(0x5356ce68), X(0x53749d89), X(0x539265eb), X(0x53b02788),
X(0x53cde257), X(0x53eb964f), X(0x54094369), X(0x5426e99c),
X(0x544488df), X(0x5462212c), X(0x547fb279), X(0x549d3cbe),
X(0x54babff4), X(0x54d83c12), X(0x54f5b110), X(0x55131ee7),
X(0x5530858d), X(0x554de4fc), X(0x556b3d2a), X(0x55888e11),
X(0x55a5d7a8), X(0x55c319e7), X(0x55e054c7), X(0x55fd883f),
X(0x561ab447), X(0x5637d8d8), X(0x5654f5ea), X(0x56720b75),
X(0x568f1971), X(0x56ac1fd7), X(0x56c91e9e), X(0x56e615c0),
X(0x57030534), X(0x571fecf2), X(0x573cccf3), X(0x5759a530),
X(0x577675a0), X(0x57933e3c), X(0x57affefd), X(0x57ccb7db),
X(0x57e968ce), X(0x580611cf), X(0x5822b2d6), X(0x583f4bdd),
X(0x585bdcdb), X(0x587865c9), X(0x5894e69f), X(0x58b15f57),
X(0x58cdcfe9), X(0x58ea384e), X(0x5906987d), X(0x5922f071),
X(0x593f4022), X(0x595b8788), X(0x5977c69c), X(0x5993fd57),
X(0x59b02bb2), X(0x59cc51a6), X(0x59e86f2c), X(0x5a04843c),
X(0x5a2090d0), X(0x5a3c94e0), X(0x5a589065), X(0x5a748359),
X(0x5a906db4), X(0x5aac4f70), X(0x5ac82884), X(0x5ae3f8ec),
X(0x5affc09f), X(0x5b1b7f97), X(0x5b3735cd), X(0x5b52e33a),
X(0x5b6e87d8), X(0x5b8a239f), X(0x5ba5b689), X(0x5bc1408f),
X(0x5bdcc1aa), X(0x5bf839d5), X(0x5c13a907), X(0x5c2f0f3b),
X(0x5c4a6c6a), X(0x5c65c08d), X(0x5c810b9e), X(0x5c9c4d97),
X(0x5cb78670), X(0x5cd2b623), X(0x5ceddcaa), X(0x5d08f9ff),
X(0x5d240e1b), X(0x5d3f18f8), X(0x5d5a1a8f), X(0x5d7512da),
X(0x5d9001d3), X(0x5daae773), X(0x5dc5c3b5), X(0x5de09692),
X(0x5dfb6004), X(0x5e162004), X(0x5e30d68d), X(0x5e4b8399),
X(0x5e662721), X(0x5e80c11f), X(0x5e9b518e), X(0x5eb5d867),
X(0x5ed055a4), X(0x5eeac940), X(0x5f053334), X(0x5f1f937b),
X(0x5f39ea0f), X(0x5f5436ea), X(0x5f6e7a06), X(0x5f88b35d),
X(0x5fa2e2e9), X(0x5fbd08a6), X(0x5fd7248d), X(0x5ff13698),
X(0x600b3ec2), X(0x60253d05), X(0x603f315b), X(0x60591bc0),
X(0x6072fc2d), X(0x608cd29e), X(0x60a69f0b), X(0x60c06171),
X(0x60da19ca), X(0x60f3c80f), X(0x610d6c3d), X(0x6127064d),
X(0x6140963a), X(0x615a1bff), X(0x61739797), X(0x618d08fc),
X(0x61a67029), X(0x61bfcd1a), X(0x61d91fc8), X(0x61f2682f),
X(0x620ba64a), X(0x6224da13), X(0x623e0386), X(0x6257229d),
X(0x62703754), X(0x628941a6), X(0x62a2418e), X(0x62bb3706),
X(0x62d4220a), X(0x62ed0296), X(0x6305d8a3), X(0x631ea42f),
X(0x63376533), X(0x63501bab), X(0x6368c793), X(0x638168e5),
X(0x6399ff9e), X(0x63b28bb8), X(0x63cb0d2f), X(0x63e383ff),
X(0x63fbf022), X(0x64145195), X(0x642ca853), X(0x6444f457),
X(0x645d359e), X(0x64756c22), X(0x648d97e0), X(0x64a5b8d3),
X(0x64bdcef6), X(0x64d5da47), X(0x64eddabf), X(0x6505d05c),
X(0x651dbb19), X(0x65359af2), X(0x654d6fe3), X(0x656539e7),
X(0x657cf8fb), X(0x6594ad1b), X(0x65ac5643), X(0x65c3f46e),
X(0x65db8799), X(0x65f30fc0), X(0x660a8ce0), X(0x6621fef3),
X(0x663965f7), X(0x6650c1e7), X(0x666812c1), X(0x667f5880),
X(0x66969320), X(0x66adc29e), X(0x66c4e6f7), X(0x66dc0026),
X(0x66f30e28), X(0x670a10fa), X(0x67210898), X(0x6737f4ff),
X(0x674ed62b), X(0x6765ac19), X(0x677c76c5), X(0x6793362c),
X(0x67a9ea4b), X(0x67c0931f), X(0x67d730a3), X(0x67edc2d6),
X(0x680449b3), X(0x681ac538), X(0x68313562), X(0x68479a2d),
X(0x685df396), X(0x6874419b), X(0x688a8438), X(0x68a0bb6a),
X(0x68b6e72e), X(0x68cd0782), X(0x68e31c63), X(0x68f925cd),
X(0x690f23be), X(0x69251633), X(0x693afd29), X(0x6950d89e),
X(0x6966a88f), X(0x697c6cf8), X(0x699225d9), X(0x69a7d32d),
X(0x69bd74f3), X(0x69d30b27), X(0x69e895c8), X(0x69fe14d2),
X(0x6a138844), X(0x6a28f01b), X(0x6a3e4c54), X(0x6a539ced),
X(0x6a68e1e4), X(0x6a7e1b37), X(0x6a9348e3), X(0x6aa86ae6),
X(0x6abd813d), X(0x6ad28be7), X(0x6ae78ae2), X(0x6afc7e2b),
X(0x6b1165c0), X(0x6b26419f), X(0x6b3b11c7), X(0x6b4fd634),
X(0x6b648ee6), X(0x6b793bda), X(0x6b8ddd0e), X(0x6ba27281),
X(0x6bb6fc31), X(0x6bcb7a1b), X(0x6bdfec3e), X(0x6bf45299),
X(0x6c08ad29), X(0x6c1cfbed), X(0x6c313ee4), X(0x6c45760a),
X(0x6c59a160), X(0x6c6dc0e4), X(0x6c81d493), X(0x6c95dc6d),
X(0x6ca9d86f), X(0x6cbdc899), X(0x6cd1acea), X(0x6ce5855f),
X(0x6cf951f7), X(0x6d0d12b1), X(0x6d20c78c), X(0x6d347087),
X(0x6d480da0), X(0x6d5b9ed6), X(0x6d6f2427), X(0x6d829d94),
X(0x6d960b1a), X(0x6da96cb9), X(0x6dbcc270), X(0x6dd00c3c),
X(0x6de34a1f), X(0x6df67c16), X(0x6e09a221), X(0x6e1cbc3f),
X(0x6e2fca6e), X(0x6e42ccaf), X(0x6e55c300), X(0x6e68ad60),
X(0x6e7b8bd0), X(0x6e8e5e4d), X(0x6ea124d8), X(0x6eb3df70),
X(0x6ec68e13), X(0x6ed930c3), X(0x6eebc77d), X(0x6efe5242),
X(0x6f10d111), X(0x6f2343e9), X(0x6f35aacb), X(0x6f4805b5),
X(0x6f5a54a8), X(0x6f6c97a2), X(0x6f7ecea4), X(0x6f90f9ae),
X(0x6fa318be), X(0x6fb52bd6), X(0x6fc732f4), X(0x6fd92e19),
X(0x6feb1d44), X(0x6ffd0076), X(0x700ed7ad), X(0x7020a2eb),
X(0x7032622f), X(0x7044157a), X(0x7055bcca), X(0x70675821),
X(0x7078e77e), X(0x708a6ae2), X(0x709be24c), X(0x70ad4dbd),
X(0x70bead36), X(0x70d000b5), X(0x70e1483d), X(0x70f283cc),
X(0x7103b363), X(0x7114d704), X(0x7125eead), X(0x7136fa60),
X(0x7147fa1c), X(0x7158ede4), X(0x7169d5b6), X(0x717ab193),
X(0x718b817d), X(0x719c4573), X(0x71acfd76), X(0x71bda988),
X(0x71ce49a8), X(0x71deddd7), X(0x71ef6617), X(0x71ffe267),
X(0x721052ca), X(0x7220b73e), X(0x72310fc6), X(0x72415c62),
X(0x72519d14), X(0x7261d1db), X(0x7271faba), X(0x728217b1),
X(0x729228c0), X(0x72a22dea), X(0x72b22730), X(0x72c21491),
X(0x72d1f611), X(0x72e1cbaf), X(0x72f1956c), X(0x7301534c),
X(0x7311054d), X(0x7320ab72), X(0x733045bc), X(0x733fd42d),
X(0x734f56c5), X(0x735ecd86), X(0x736e3872), X(0x737d9789),
X(0x738ceacf), X(0x739c3243), X(0x73ab6de7), X(0x73ba9dbe),
X(0x73c9c1c8), X(0x73d8da08), X(0x73e7e67f), X(0x73f6e72e),
X(0x7405dc17), X(0x7414c53c), X(0x7423a29f), X(0x74327442),
X(0x74413a26), X(0x744ff44d), X(0x745ea2b9), X(0x746d456c),
X(0x747bdc68), X(0x748a67ae), X(0x7498e741), X(0x74a75b23),
X(0x74b5c356), X(0x74c41fdb), X(0x74d270b6), X(0x74e0b5e7),
X(0x74eeef71), X(0x74fd1d57), X(0x750b3f9a), X(0x7519563c),
X(0x75276140), X(0x753560a8), X(0x75435477), X(0x75513cae),
X(0x755f1951), X(0x756cea60), X(0x757aafdf), X(0x758869d1),
X(0x75961837), X(0x75a3bb14), X(0x75b1526a), X(0x75bede3c),
X(0x75cc5e8d), X(0x75d9d35f), X(0x75e73cb5), X(0x75f49a91),
X(0x7601ecf6), X(0x760f33e6), X(0x761c6f65), X(0x76299f74),
X(0x7636c417), X(0x7643dd51), X(0x7650eb24), X(0x765ded93),
X(0x766ae4a0), X(0x7677d050), X(0x7684b0a4), X(0x7691859f),
X(0x769e4f45), X(0x76ab0d98), X(0x76b7c09c), X(0x76c46852),
X(0x76d104bf), X(0x76dd95e6), X(0x76ea1bc9), X(0x76f6966b),
X(0x770305d0), X(0x770f69fb), X(0x771bc2ef), X(0x772810af),
X(0x7734533e), X(0x77408aa0), X(0x774cb6d7), X(0x7758d7e8),
X(0x7764edd5), X(0x7770f8a2), X(0x777cf852), X(0x7788ece8),
X(0x7794d668), X(0x77a0b4d5), X(0x77ac8833), X(0x77b85085),
X(0x77c40dce), X(0x77cfc013), X(0x77db6756), X(0x77e7039b),
X(0x77f294e6), X(0x77fe1b3b), X(0x7809969c), X(0x7815070e),
X(0x78206c93), X(0x782bc731), X(0x783716ea), X(0x78425bc3),
X(0x784d95be), X(0x7858c4e1), X(0x7863e92d), X(0x786f02a8),
X(0x787a1156), X(0x78851539), X(0x78900e56), X(0x789afcb1),
X(0x78a5e04d), X(0x78b0b92f), X(0x78bb875b), X(0x78c64ad4),
X(0x78d1039e), X(0x78dbb1be), X(0x78e65537), X(0x78f0ee0e),
X(0x78fb7c46), X(0x7905ffe4), X(0x791078ec), X(0x791ae762),
X(0x79254b4a), X(0x792fa4a7), X(0x7939f380), X(0x794437d7),
X(0x794e71b0), X(0x7958a111), X(0x7962c5fd), X(0x796ce078),
X(0x7976f087), X(0x7980f62f), X(0x798af173), X(0x7994e258),
X(0x799ec8e2), X(0x79a8a515), X(0x79b276f7), X(0x79bc3e8b),
X(0x79c5fbd6), X(0x79cfaedc), X(0x79d957a2), X(0x79e2f62c),
X(0x79ec8a7f), X(0x79f6149f), X(0x79ff9492), X(0x7a090a5a),
X(0x7a1275fe), X(0x7a1bd781), X(0x7a252ee9), X(0x7a2e7c39),
X(0x7a37bf77), X(0x7a40f8a7), X(0x7a4a27ce), X(0x7a534cf0),
X(0x7a5c6813), X(0x7a65793b), X(0x7a6e806d), X(0x7a777dad),
X(0x7a807100), X(0x7a895a6b), X(0x7a9239f4), X(0x7a9b0f9e),
X(0x7aa3db6f), X(0x7aac9d6b), X(0x7ab55597), X(0x7abe03f9),
X(0x7ac6a895), X(0x7acf4370), X(0x7ad7d48f), X(0x7ae05bf6),
X(0x7ae8d9ac), X(0x7af14db5), X(0x7af9b815), X(0x7b0218d2),
X(0x7b0a6ff2), X(0x7b12bd78), X(0x7b1b016a), X(0x7b233bce),
X(0x7b2b6ca7), X(0x7b3393fc), X(0x7b3bb1d1), X(0x7b43c62c),
X(0x7b4bd111), X(0x7b53d286), X(0x7b5bca90), X(0x7b63b935),
X(0x7b6b9e78), X(0x7b737a61), X(0x7b7b4cf3), X(0x7b831634),
X(0x7b8ad629), X(0x7b928cd8), X(0x7b9a3a45), X(0x7ba1de77),
X(0x7ba97972), X(0x7bb10b3c), X(0x7bb893d9), X(0x7bc01350),
X(0x7bc789a6), X(0x7bcef6e0), X(0x7bd65b03), X(0x7bddb616),
X(0x7be5081c), X(0x7bec511c), X(0x7bf3911b), X(0x7bfac81f),
X(0x7c01f62c), X(0x7c091b49), X(0x7c10377b), X(0x7c174ac7),
X(0x7c1e5532), X(0x7c2556c4), X(0x7c2c4f80), X(0x7c333f6c),
X(0x7c3a268e), X(0x7c4104ec), X(0x7c47da8a), X(0x7c4ea76f),
X(0x7c556ba1), X(0x7c5c2724), X(0x7c62d9fe), X(0x7c698435),
X(0x7c7025cf), X(0x7c76bed0), X(0x7c7d4f40), X(0x7c83d723),
X(0x7c8a567f), X(0x7c90cd5a), X(0x7c973bb9), X(0x7c9da1a2),
X(0x7ca3ff1b), X(0x7caa542a), X(0x7cb0a0d3), X(0x7cb6e51e),
X(0x7cbd210f), X(0x7cc354ac), X(0x7cc97ffc), X(0x7ccfa304),
X(0x7cd5bdc9), X(0x7cdbd051), X(0x7ce1daa3), X(0x7ce7dcc3),
X(0x7cedd6b8), X(0x7cf3c888), X(0x7cf9b238), X(0x7cff93cf),
X(0x7d056d51), X(0x7d0b3ec5), X(0x7d110830), X(0x7d16c99a),
X(0x7d1c8306), X(0x7d22347c), X(0x7d27de00), X(0x7d2d7f9a),
X(0x7d33194f), X(0x7d38ab24), X(0x7d3e351f), X(0x7d43b748),
X(0x7d4931a2), X(0x7d4ea435), X(0x7d540f06), X(0x7d59721b),
X(0x7d5ecd7b), X(0x7d64212a), X(0x7d696d2f), X(0x7d6eb190),
X(0x7d73ee53), X(0x7d79237e), X(0x7d7e5117), X(0x7d837723),
X(0x7d8895a9), X(0x7d8dacae), X(0x7d92bc3a), X(0x7d97c451),
X(0x7d9cc4f9), X(0x7da1be39), X(0x7da6b017), X(0x7dab9a99),
X(0x7db07dc4), X(0x7db5599e), X(0x7dba2e2f), X(0x7dbefb7b),
X(0x7dc3c189), X(0x7dc8805e), X(0x7dcd3802), X(0x7dd1e879),
X(0x7dd691ca), X(0x7ddb33fb), X(0x7ddfcf12), X(0x7de46315),
X(0x7de8f00a), X(0x7ded75f8), X(0x7df1f4e3), X(0x7df66cd3),
X(0x7dfaddcd), X(0x7dff47d7), X(0x7e03aaf8), X(0x7e080735),
X(0x7e0c5c95), X(0x7e10ab1e), X(0x7e14f2d5), X(0x7e1933c1),
X(0x7e1d6de8), X(0x7e21a150), X(0x7e25cdff), X(0x7e29f3fc),
X(0x7e2e134c), X(0x7e322bf5), X(0x7e363dfd), X(0x7e3a496b),
X(0x7e3e4e45), X(0x7e424c90), X(0x7e464454), X(0x7e4a3595),
X(0x7e4e205a), X(0x7e5204aa), X(0x7e55e289), X(0x7e59b9ff),
X(0x7e5d8b12), X(0x7e6155c7), X(0x7e651a24), X(0x7e68d831),
X(0x7e6c8ff2), X(0x7e70416e), X(0x7e73ecac), X(0x7e7791b0),
X(0x7e7b3082), X(0x7e7ec927), X(0x7e825ba6), X(0x7e85e804),
X(0x7e896e48), X(0x7e8cee77), X(0x7e906899), X(0x7e93dcb2),
X(0x7e974aca), X(0x7e9ab2e5), X(0x7e9e150b), X(0x7ea17141),
X(0x7ea4c78e), X(0x7ea817f7), X(0x7eab6283), X(0x7eaea737),
X(0x7eb1e61a), X(0x7eb51f33), X(0x7eb85285), X(0x7ebb8019),
X(0x7ebea7f4), X(0x7ec1ca1d), X(0x7ec4e698), X(0x7ec7fd6d),
X(0x7ecb0ea1), X(0x7ece1a3a), X(0x7ed1203f), X(0x7ed420b6),
X(0x7ed71ba4), X(0x7eda110f), X(0x7edd00ff), X(0x7edfeb78),
X(0x7ee2d081), X(0x7ee5b01f), X(0x7ee88a5a), X(0x7eeb5f36),
X(0x7eee2eba), X(0x7ef0f8ed), X(0x7ef3bdd3), X(0x7ef67d73),
X(0x7ef937d3), X(0x7efbecf9), X(0x7efe9ceb), X(0x7f0147ae),
X(0x7f03ed4a), X(0x7f068dc4), X(0x7f092922), X(0x7f0bbf69),
X(0x7f0e50a1), X(0x7f10dcce), X(0x7f1363f7), X(0x7f15e622),
X(0x7f186355), X(0x7f1adb95), X(0x7f1d4ee9), X(0x7f1fbd57),
X(0x7f2226e4), X(0x7f248b96), X(0x7f26eb74), X(0x7f294683),
X(0x7f2b9cc9), X(0x7f2dee4d), X(0x7f303b13), X(0x7f328322),
X(0x7f34c680), X(0x7f370533), X(0x7f393f40), X(0x7f3b74ad),
X(0x7f3da581), X(0x7f3fd1c1), X(0x7f41f972), X(0x7f441c9c),
X(0x7f463b43), X(0x7f48556d), X(0x7f4a6b21), X(0x7f4c7c64),
X(0x7f4e893c), X(0x7f5091ae), X(0x7f5295c1), X(0x7f54957a),
X(0x7f5690e0), X(0x7f5887f7), X(0x7f5a7ac5), X(0x7f5c6951),
X(0x7f5e53a0), X(0x7f6039b8), X(0x7f621b9e), X(0x7f63f958),
X(0x7f65d2ed), X(0x7f67a861), X(0x7f6979ba), X(0x7f6b46ff),
X(0x7f6d1034), X(0x7f6ed560), X(0x7f709687), X(0x7f7253b1),
X(0x7f740ce1), X(0x7f75c21f), X(0x7f777370), X(0x7f7920d8),
X(0x7f7aca5f), X(0x7f7c7008), X(0x7f7e11db), X(0x7f7fafdd),
X(0x7f814a13), X(0x7f82e082), X(0x7f847331), X(0x7f860224),
X(0x7f878d62), X(0x7f8914f0), X(0x7f8a98d4), X(0x7f8c1912),
X(0x7f8d95b0), X(0x7f8f0eb5), X(0x7f908425), X(0x7f91f605),
X(0x7f93645c), X(0x7f94cf2f), X(0x7f963683), X(0x7f979a5d),
X(0x7f98fac4), X(0x7f9a57bb), X(0x7f9bb14a), X(0x7f9d0775),
X(0x7f9e5a41), X(0x7f9fa9b4), X(0x7fa0f5d3), X(0x7fa23ea4),
X(0x7fa3842b), X(0x7fa4c66f), X(0x7fa60575), X(0x7fa74141),
X(0x7fa879d9), X(0x7fa9af42), X(0x7faae182), X(0x7fac109e),
X(0x7fad3c9a), X(0x7fae657d), X(0x7faf8b4c), X(0x7fb0ae0b),
X(0x7fb1cdc0), X(0x7fb2ea70), X(0x7fb40420), X(0x7fb51ad5),
X(0x7fb62e95), X(0x7fb73f64), X(0x7fb84d48), X(0x7fb95846),
X(0x7fba6062), X(0x7fbb65a2), X(0x7fbc680c), X(0x7fbd67a3),
X(0x7fbe646d), X(0x7fbf5e70), X(0x7fc055af), X(0x7fc14a31),
X(0x7fc23bf9), X(0x7fc32b0d), X(0x7fc41773), X(0x7fc5012e),
X(0x7fc5e844), X(0x7fc6ccba), X(0x7fc7ae94), X(0x7fc88dd8),
X(0x7fc96a8a), X(0x7fca44af), X(0x7fcb1c4c), X(0x7fcbf167),
X(0x7fccc403), X(0x7fcd9425), X(0x7fce61d3), X(0x7fcf2d11),
X(0x7fcff5e3), X(0x7fd0bc4f), X(0x7fd1805a), X(0x7fd24207),
X(0x7fd3015c), X(0x7fd3be5d), X(0x7fd47910), X(0x7fd53178),
X(0x7fd5e79b), X(0x7fd69b7c), X(0x7fd74d21), X(0x7fd7fc8e),
X(0x7fd8a9c8), X(0x7fd954d4), X(0x7fd9fdb5), X(0x7fdaa471),
X(0x7fdb490b), X(0x7fdbeb89), X(0x7fdc8bef), X(0x7fdd2a42),
X(0x7fddc685), X(0x7fde60be), X(0x7fdef8f0), X(0x7fdf8f20),
X(0x7fe02353), X(0x7fe0b58d), X(0x7fe145d3), X(0x7fe1d428),
X(0x7fe26091), X(0x7fe2eb12), X(0x7fe373b0), X(0x7fe3fa6f),
X(0x7fe47f53), X(0x7fe50260), X(0x7fe5839b), X(0x7fe60308),
X(0x7fe680ab), X(0x7fe6fc88), X(0x7fe776a4), X(0x7fe7ef02),
X(0x7fe865a7), X(0x7fe8da97), X(0x7fe94dd6), X(0x7fe9bf68),
X(0x7fea2f51), X(0x7fea9d95), X(0x7feb0a39), X(0x7feb7540),
X(0x7febdeae), X(0x7fec4687), X(0x7fecaccf), X(0x7fed118b),
X(0x7fed74be), X(0x7fedd66c), X(0x7fee3698), X(0x7fee9548),
X(0x7feef27e), X(0x7fef4e3f), X(0x7fefa88e), X(0x7ff0016f),
X(0x7ff058e7), X(0x7ff0aef8), X(0x7ff103a6), X(0x7ff156f6),
X(0x7ff1a8eb), X(0x7ff1f988), X(0x7ff248d2), X(0x7ff296cc),
X(0x7ff2e37a), X(0x7ff32edf), X(0x7ff378ff), X(0x7ff3c1de),
X(0x7ff4097e), X(0x7ff44fe5), X(0x7ff49515), X(0x7ff4d911),
X(0x7ff51bde), X(0x7ff55d7f), X(0x7ff59df7), X(0x7ff5dd4a),
X(0x7ff61b7b), X(0x7ff6588d), X(0x7ff69485), X(0x7ff6cf65),
X(0x7ff70930), X(0x7ff741eb), X(0x7ff77998), X(0x7ff7b03b),
X(0x7ff7e5d7), X(0x7ff81a6f), X(0x7ff84e06), X(0x7ff880a1),
X(0x7ff8b241), X(0x7ff8e2ea), X(0x7ff912a0), X(0x7ff94165),
X(0x7ff96f3d), X(0x7ff99c2b), X(0x7ff9c831), X(0x7ff9f354),
X(0x7ffa1d95), X(0x7ffa46f9), X(0x7ffa6f81), X(0x7ffa9731),
X(0x7ffabe0d), X(0x7ffae416), X(0x7ffb0951), X(0x7ffb2dbf),
X(0x7ffb5164), X(0x7ffb7442), X(0x7ffb965d), X(0x7ffbb7b8),
X(0x7ffbd854), X(0x7ffbf836), X(0x7ffc175f), X(0x7ffc35d3),
X(0x7ffc5394), X(0x7ffc70a5), X(0x7ffc8d09), X(0x7ffca8c2),
X(0x7ffcc3d4), X(0x7ffcde3f), X(0x7ffcf809), X(0x7ffd1132),
X(0x7ffd29be), X(0x7ffd41ae), X(0x7ffd5907), X(0x7ffd6fc9),
X(0x7ffd85f9), X(0x7ffd9b97), X(0x7ffdb0a7), X(0x7ffdc52b),
X(0x7ffdd926), X(0x7ffdec99), X(0x7ffdff88), X(0x7ffe11f4),
X(0x7ffe23e0), X(0x7ffe354f), X(0x7ffe4642), X(0x7ffe56bc),
X(0x7ffe66bf), X(0x7ffe764e), X(0x7ffe856a), X(0x7ffe9416),
X(0x7ffea254), X(0x7ffeb026), X(0x7ffebd8e), X(0x7ffeca8f),
X(0x7ffed72a), X(0x7ffee362), X(0x7ffeef38), X(0x7ffefaaf),
X(0x7fff05c9), X(0x7fff1087), X(0x7fff1aec), X(0x7fff24f9),
X(0x7fff2eb1), X(0x7fff3816), X(0x7fff4128), X(0x7fff49eb),
X(0x7fff5260), X(0x7fff5a88), X(0x7fff6266), X(0x7fff69fc),
X(0x7fff714b), X(0x7fff7854), X(0x7fff7f1a), X(0x7fff859f),
X(0x7fff8be3), X(0x7fff91ea), X(0x7fff97b3), X(0x7fff9d41),
X(0x7fffa296), X(0x7fffa7b3), X(0x7fffac99), X(0x7fffb14b),
X(0x7fffb5c9), X(0x7fffba15), X(0x7fffbe31), X(0x7fffc21d),
X(0x7fffc5dc), X(0x7fffc96f), X(0x7fffccd8), X(0x7fffd016),
X(0x7fffd32d), X(0x7fffd61c), X(0x7fffd8e7), X(0x7fffdb8d),
X(0x7fffde0f), X(0x7fffe071), X(0x7fffe2b1), X(0x7fffe4d2),
X(0x7fffe6d5), X(0x7fffe8bb), X(0x7fffea85), X(0x7fffec34),
X(0x7fffedc9), X(0x7fffef45), X(0x7ffff0aa), X(0x7ffff1f7),
X(0x7ffff330), X(0x7ffff453), X(0x7ffff562), X(0x7ffff65f),
X(0x7ffff749), X(0x7ffff823), X(0x7ffff8ec), X(0x7ffff9a6),
X(0x7ffffa51), X(0x7ffffaee), X(0x7ffffb7e), X(0x7ffffc02),
X(0x7ffffc7a), X(0x7ffffce7), X(0x7ffffd4a), X(0x7ffffda3),
X(0x7ffffdf4), X(0x7ffffe3c), X(0x7ffffe7c), X(0x7ffffeb6),
X(0x7ffffee8), X(0x7fffff15), X(0x7fffff3c), X(0x7fffff5e),
X(0x7fffff7b), X(0x7fffff95), X(0x7fffffaa), X(0x7fffffbc),
X(0x7fffffcb), X(0x7fffffd7), X(0x7fffffe2), X(0x7fffffea),
X(0x7ffffff0), X(0x7ffffff5), X(0x7ffffff9), X(0x7ffffffb),
X(0x7ffffffd), X(0x7ffffffe), X(0x7fffffff), X(0x7fffffff),
X(0x7fffffff), X(0x7fffffff), X(0x7fffffff), X(0x7fffffff),
};

static LOOKUP_T vwin8192[4096] = {
X(0x0000007c), X(0x0000045c), X(0x00000c1d), X(0x000017bd),
X(0x0000273e), X(0x00003a9f), X(0x000051e0), X(0x00006d02),
X(0x00008c03), X(0x0000aee5), X(0x0000d5a7), X(0x00010049),
X(0x00012ecb), X(0x0001612d), X(0x00019770), X(0x0001d193),
X(0x00020f96), X(0x00025178), X(0x0002973c), X(0x0002e0df),
X(0x00032e62), X(0x00037fc5), X(0x0003d509), X(0x00042e2c),
X(0x00048b30), X(0x0004ec13), X(0x000550d7), X(0x0005b97a),
X(0x000625fe), X(0x00069661), X(0x00070aa4), X(0x000782c8),
X(0x0007fecb), X(0x00087eae), X(0x00090271), X(0x00098a14),
X(0x000a1597), X(0x000aa4f9), X(0x000b383b), X(0x000bcf5d),
X(0x000c6a5f), X(0x000d0941), X(0x000dac02), X(0x000e52a3),
X(0x000efd23), X(0x000fab84), X(0x00105dc3), X(0x001113e3),
X(0x0011cde2), X(0x00128bc0), X(0x00134d7e), X(0x0014131b),
X(0x0014dc98), X(0x0015a9f4), X(0x00167b30), X(0x0017504a),
X(0x00182945), X(0x0019061e), X(0x0019e6d7), X(0x001acb6f),
X(0x001bb3e6), X(0x001ca03c), X(0x001d9071), X(0x001e8485),
X(0x001f7c79), X(0x0020784b), X(0x002177fc), X(0x00227b8c),
X(0x002382fb), X(0x00248e49), X(0x00259d76), X(0x0026b081),
X(0x0027c76b), X(0x0028e234), X(0x002a00dc), X(0x002b2361),
X(0x002c49c6), X(0x002d7409), X(0x002ea22a), X(0x002fd42a),
X(0x00310a08), X(0x003243c5), X(0x00338160), X(0x0034c2d9),
X(0x00360830), X(0x00375165), X(0x00389e78), X(0x0039ef6a),
X(0x003b4439), X(0x003c9ce6), X(0x003df971), X(0x003f59da),
X(0x0040be20), X(0x00422645), X(0x00439247), X(0x00450226),
X(0x004675e3), X(0x0047ed7e), X(0x004968f5), X(0x004ae84b),
X(0x004c6b7d), X(0x004df28d), X(0x004f7d7a), X(0x00510c44),
X(0x00529eeb), X(0x00543570), X(0x0055cfd1), X(0x00576e0f),
X(0x00591029), X(0x005ab621), X(0x005c5ff5), X(0x005e0da6),
X(0x005fbf33), X(0x0061749d), X(0x00632de4), X(0x0064eb06),
X(0x0066ac05), X(0x006870e0), X(0x006a3998), X(0x006c062b),
X(0x006dd69b), X(0x006faae6), X(0x0071830d), X(0x00735f10),
X(0x00753eef), X(0x007722a9), X(0x00790a3f), X(0x007af5b1),
X(0x007ce4fe), X(0x007ed826), X(0x0080cf29), X(0x0082ca08),
X(0x0084c8c2), X(0x0086cb57), X(0x0088d1c7), X(0x008adc11),
X(0x008cea37), X(0x008efc37), X(0x00911212), X(0x00932bc7),
X(0x00954957), X(0x00976ac2), X(0x00999006), X(0x009bb925),
X(0x009de61e), X(0x00a016f1), X(0x00a24b9e), X(0x00a48425),
X(0x00a6c086), X(0x00a900c0), X(0x00ab44d4), X(0x00ad8cc2),
X(0x00afd889), X(0x00b22829), X(0x00b47ba2), X(0x00b6d2f5),
X(0x00b92e21), X(0x00bb8d26), X(0x00bdf004), X(0x00c056ba),
X(0x00c2c149), X(0x00c52fb1), X(0x00c7a1f1), X(0x00ca180a),
X(0x00cc91fb), X(0x00cf0fc5), X(0x00d19166), X(0x00d416df),
X(0x00d6a031), X(0x00d92d5a), X(0x00dbbe5b), X(0x00de5333),
X(0x00e0ebe3), X(0x00e3886b), X(0x00e628c9), X(0x00e8ccff),
X(0x00eb750c), X(0x00ee20f0), X(0x00f0d0ab), X(0x00f3843d),
X(0x00f63ba5), X(0x00f8f6e4), X(0x00fbb5fa), X(0x00fe78e5),
X(0x01013fa7), X(0x01040a3f), X(0x0106d8ae), X(0x0109aaf2),
X(0x010c810c), X(0x010f5afb), X(0x011238c0), X(0x01151a5b),
X(0x0117ffcb), X(0x011ae910), X(0x011dd62a), X(0x0120c719),
X(0x0123bbdd), X(0x0126b476), X(0x0129b0e4), X(0x012cb126),
X(0x012fb53c), X(0x0132bd27), X(0x0135c8e6), X(0x0138d879),
X(0x013bebdf), X(0x013f031a), X(0x01421e28), X(0x01453d0a),
X(0x01485fbf), X(0x014b8648), X(0x014eb0a4), X(0x0151ded2),
X(0x015510d4), X(0x015846a8), X(0x015b8050), X(0x015ebdc9),
X(0x0161ff15), X(0x01654434), X(0x01688d24), X(0x016bd9e6),
X(0x016f2a7b), X(0x01727ee1), X(0x0175d718), X(0x01793321),
X(0x017c92fc), X(0x017ff6a7), X(0x01835e24), X(0x0186c972),
X(0x018a3890), X(0x018dab7f), X(0x0191223f), X(0x01949ccf),
X(0x01981b2f), X(0x019b9d5f), X(0x019f235f), X(0x01a2ad2f),
X(0x01a63acf), X(0x01a9cc3e), X(0x01ad617c), X(0x01b0fa8a),
X(0x01b49767), X(0x01b83813), X(0x01bbdc8d), X(0x01bf84d6),
X(0x01c330ee), X(0x01c6e0d4), X(0x01ca9488), X(0x01ce4c0b),
X(0x01d2075b), X(0x01d5c679), X(0x01d98964), X(0x01dd501d),
X(0x01e11aa3), X(0x01e4e8f6), X(0x01e8bb17), X(0x01ec9104),
X(0x01f06abd), X(0x01f44844), X(0x01f82996), X(0x01fc0eb5),
X(0x01fff7a0), X(0x0203e456), X(0x0207d4d9), X(0x020bc926),
X(0x020fc140), X(0x0213bd24), X(0x0217bcd4), X(0x021bc04e),
X(0x021fc793), X(0x0223d2a3), X(0x0227e17d), X(0x022bf421),
X(0x02300a90), X(0x023424c8), X(0x023842ca), X(0x023c6495),
X(0x02408a2a), X(0x0244b389), X(0x0248e0b0), X(0x024d11a0),
X(0x02514659), X(0x02557eda), X(0x0259bb24), X(0x025dfb35),
X(0x02623f0f), X(0x026686b1), X(0x026ad21a), X(0x026f214b),
X(0x02737443), X(0x0277cb02), X(0x027c2588), X(0x028083d5),
X(0x0284e5e9), X(0x02894bc2), X(0x028db562), X(0x029222c8),
X(0x029693f4), X(0x029b08e6), X(0x029f819d), X(0x02a3fe19),
X(0x02a87e5b), X(0x02ad0261), X(0x02b18a2c), X(0x02b615bb),
X(0x02baa50f), X(0x02bf3827), X(0x02c3cf03), X(0x02c869a3),
X(0x02cd0807), X(0x02d1aa2d), X(0x02d65017), X(0x02daf9c4),
X(0x02dfa734), X(0x02e45866), X(0x02e90d5b), X(0x02edc612),
X(0x02f2828b), X(0x02f742c6), X(0x02fc06c3), X(0x0300ce80),
X(0x030599ff), X(0x030a6940), X(0x030f3c40), X(0x03141302),
X(0x0318ed84), X(0x031dcbc6), X(0x0322adc8), X(0x0327938a),
X(0x032c7d0c), X(0x03316a4c), X(0x03365b4d), X(0x033b500c),
X(0x03404889), X(0x034544c6), X(0x034a44c0), X(0x034f4879),
X(0x03544ff0), X(0x03595b24), X(0x035e6a16), X(0x03637cc5),
X(0x03689331), X(0x036dad5a), X(0x0372cb40), X(0x0377ece2),
X(0x037d1240), X(0x03823b5a), X(0x03876830), X(0x038c98c1),
X(0x0391cd0e), X(0x03970516), X(0x039c40d8), X(0x03a18055),
X(0x03a6c38d), X(0x03ac0a7f), X(0x03b1552b), X(0x03b6a390),
X(0x03bbf5af), X(0x03c14b88), X(0x03c6a519), X(0x03cc0263),
X(0x03d16366), X(0x03d6c821), X(0x03dc3094), X(0x03e19cc0),
X(0x03e70ca2), X(0x03ec803d), X(0x03f1f78e), X(0x03f77296),
X(0x03fcf155), X(0x040273cb), X(0x0407f9f7), X(0x040d83d9),
X(0x04131170), X(0x0418a2bd), X(0x041e37c0), X(0x0423d077),
X(0x04296ce4), X(0x042f0d04), X(0x0434b0da), X(0x043a5863),
X(0x044003a0), X(0x0445b290), X(0x044b6534), X(0x04511b8b),
X(0x0456d595), X(0x045c9352), X(0x046254c1), X(0x046819e1),
X(0x046de2b4), X(0x0473af39), X(0x04797f6e), X(0x047f5355),
X(0x04852aec), X(0x048b0635), X(0x0490e52d), X(0x0496c7d6),
X(0x049cae2e), X(0x04a29836), X(0x04a885ed), X(0x04ae7753),
X(0x04b46c68), X(0x04ba652b), X(0x04c0619d), X(0x04c661bc),
X(0x04cc658a), X(0x04d26d04), X(0x04d8782c), X(0x04de8701),
X(0x04e49983), X(0x04eaafb0), X(0x04f0c98a), X(0x04f6e710),
X(0x04fd0842), X(0x05032d1e), X(0x050955a6), X(0x050f81d8),
X(0x0515b1b5), X(0x051be53d), X(0x05221c6e), X(0x05285748),
X(0x052e95cd), X(0x0534d7fa), X(0x053b1dd0), X(0x0541674e),
X(0x0547b475), X(0x054e0544), X(0x055459bb), X(0x055ab1d9),
X(0x05610d9e), X(0x05676d0a), X(0x056dd01c), X(0x057436d5),
X(0x057aa134), X(0x05810f38), X(0x058780e2), X(0x058df631),
X(0x05946f25), X(0x059aebbe), X(0x05a16bfa), X(0x05a7efdb),
X(0x05ae775f), X(0x05b50287), X(0x05bb9152), X(0x05c223c0),
X(0x05c8b9d0), X(0x05cf5382), X(0x05d5f0d6), X(0x05dc91cc),
X(0x05e33663), X(0x05e9de9c), X(0x05f08a75), X(0x05f739ee),
X(0x05fded07), X(0x0604a3c0), X(0x060b5e19), X(0x06121c11),
X(0x0618dda8), X(0x061fa2dd), X(0x06266bb1), X(0x062d3822),
X(0x06340831), X(0x063adbde), X(0x0641b328), X(0x06488e0e),
X(0x064f6c91), X(0x06564eaf), X(0x065d346a), X(0x06641dc0),
X(0x066b0ab1), X(0x0671fb3d), X(0x0678ef64), X(0x067fe724),
X(0x0686e27f), X(0x068de173), X(0x0694e400), X(0x069bea27),
X(0x06a2f3e6), X(0x06aa013d), X(0x06b1122c), X(0x06b826b3),
X(0x06bf3ed1), X(0x06c65a86), X(0x06cd79d1), X(0x06d49cb3),
X(0x06dbc32b), X(0x06e2ed38), X(0x06ea1adb), X(0x06f14c13),
X(0x06f880df), X(0x06ffb940), X(0x0706f535), X(0x070e34bd),
X(0x071577d9), X(0x071cbe88), X(0x072408c9), X(0x072b569d),
X(0x0732a802), X(0x0739fcf9), X(0x07415582), X(0x0748b19b),
X(0x07501145), X(0x0757747f), X(0x075edb49), X(0x076645a3),
X(0x076db38c), X(0x07752503), X(0x077c9a09), X(0x0784129e),
X(0x078b8ec0), X(0x07930e70), X(0x079a91ac), X(0x07a21876),
X(0x07a9a2cc), X(0x07b130ad), X(0x07b8c21b), X(0x07c05714),
X(0x07c7ef98), X(0x07cf8ba6), X(0x07d72b3f), X(0x07dece62),
X(0x07e6750e), X(0x07ee1f43), X(0x07f5cd01), X(0x07fd7e48),
X(0x08053316), X(0x080ceb6d), X(0x0814a74a), X(0x081c66af),
X(0x0824299a), X(0x082bf00c), X(0x0833ba03), X(0x083b8780),
X(0x08435882), X(0x084b2d09), X(0x08530514), X(0x085ae0a3),
X(0x0862bfb6), X(0x086aa24c), X(0x08728865), X(0x087a7201),
X(0x08825f1e), X(0x088a4fbe), X(0x089243de), X(0x089a3b80),
X(0x08a236a2), X(0x08aa3545), X(0x08b23767), X(0x08ba3d09),
X(0x08c2462a), X(0x08ca52c9), X(0x08d262e7), X(0x08da7682),
X(0x08e28d9c), X(0x08eaa832), X(0x08f2c645), X(0x08fae7d4),
X(0x09030cdf), X(0x090b3566), X(0x09136168), X(0x091b90e5),
X(0x0923c3dc), X(0x092bfa4d), X(0x09343437), X(0x093c719b),
X(0x0944b277), X(0x094cf6cc), X(0x09553e99), X(0x095d89dd),
X(0x0965d899), X(0x096e2acb), X(0x09768073), X(0x097ed991),
X(0x09873625), X(0x098f962e), X(0x0997f9ac), X(0x09a0609e),
X(0x09a8cb04), X(0x09b138dd), X(0x09b9aa29), X(0x09c21ee8),
X(0x09ca9719), X(0x09d312bc), X(0x09db91d0), X(0x09e41456),
X(0x09ec9a4b), X(0x09f523b1), X(0x09fdb087), X(0x0a0640cc),
X(0x0a0ed47f), X(0x0a176ba2), X(0x0a200632), X(0x0a28a42f),
X(0x0a31459a), X(0x0a39ea72), X(0x0a4292b5), X(0x0a4b3e65),
X(0x0a53ed80), X(0x0a5ca006), X(0x0a6555f7), X(0x0a6e0f51),
X(0x0a76cc16), X(0x0a7f8c44), X(0x0a884fda), X(0x0a9116d9),
X(0x0a99e140), X(0x0aa2af0e), X(0x0aab8043), X(0x0ab454df),
X(0x0abd2ce1), X(0x0ac60849), X(0x0acee716), X(0x0ad7c948),
X(0x0ae0aedf), X(0x0ae997d9), X(0x0af28437), X(0x0afb73f7),
X(0x0b04671b), X(0x0b0d5da0), X(0x0b165788), X(0x0b1f54d0),
X(0x0b285579), X(0x0b315983), X(0x0b3a60ec), X(0x0b436bb5),
X(0x0b4c79dd), X(0x0b558b63), X(0x0b5ea048), X(0x0b67b88a),
X(0x0b70d429), X(0x0b79f324), X(0x0b83157c), X(0x0b8c3b30),
X(0x0b95643f), X(0x0b9e90a8), X(0x0ba7c06c), X(0x0bb0f38a),
X(0x0bba2a01), X(0x0bc363d1), X(0x0bcca0f9), X(0x0bd5e17a),
X(0x0bdf2552), X(0x0be86c81), X(0x0bf1b706), X(0x0bfb04e2),
X(0x0c045613), X(0x0c0daa99), X(0x0c170274), X(0x0c205da3),
X(0x0c29bc25), X(0x0c331dfb), X(0x0c3c8323), X(0x0c45eb9e),
X(0x0c4f576a), X(0x0c58c688), X(0x0c6238f6), X(0x0c6baeb5),
X(0x0c7527c3), X(0x0c7ea421), X(0x0c8823cd), X(0x0c91a6c8),
X(0x0c9b2d10), X(0x0ca4b6a6), X(0x0cae4389), X(0x0cb7d3b8),
X(0x0cc16732), X(0x0ccafdf8), X(0x0cd49809), X(0x0cde3564),
X(0x0ce7d609), X(0x0cf179f7), X(0x0cfb212e), X(0x0d04cbad),
X(0x0d0e7974), X(0x0d182a83), X(0x0d21ded8), X(0x0d2b9673),
X(0x0d355154), X(0x0d3f0f7b), X(0x0d48d0e6), X(0x0d529595),
X(0x0d5c5d88), X(0x0d6628be), X(0x0d6ff737), X(0x0d79c8f2),
X(0x0d839dee), X(0x0d8d762c), X(0x0d9751aa), X(0x0da13068),
X(0x0dab1266), X(0x0db4f7a3), X(0x0dbee01e), X(0x0dc8cbd8),
X(0x0dd2bace), X(0x0ddcad02), X(0x0de6a272), X(0x0df09b1e),
X(0x0dfa9705), X(0x0e049627), X(0x0e0e9883), X(0x0e189e19),
X(0x0e22a6e8), X(0x0e2cb2f0), X(0x0e36c230), X(0x0e40d4a8),
X(0x0e4aea56), X(0x0e55033b), X(0x0e5f1f56), X(0x0e693ea7),
X(0x0e73612c), X(0x0e7d86e5), X(0x0e87afd3), X(0x0e91dbf3),
X(0x0e9c0b47), X(0x0ea63dcc), X(0x0eb07383), X(0x0ebaac6b),
X(0x0ec4e883), X(0x0ecf27cc), X(0x0ed96a44), X(0x0ee3afea),
X(0x0eedf8bf), X(0x0ef844c2), X(0x0f0293f2), X(0x0f0ce64e),
X(0x0f173bd6), X(0x0f21948a), X(0x0f2bf069), X(0x0f364f72),
X(0x0f40b1a5), X(0x0f4b1701), X(0x0f557f86), X(0x0f5feb32),
X(0x0f6a5a07), X(0x0f74cc02), X(0x0f7f4124), X(0x0f89b96b),
X(0x0f9434d8), X(0x0f9eb369), X(0x0fa9351e), X(0x0fb3b9f7),
X(0x0fbe41f3), X(0x0fc8cd11), X(0x0fd35b51), X(0x0fddecb2),
X(0x0fe88134), X(0x0ff318d6), X(0x0ffdb397), X(0x10085177),
X(0x1012f275), X(0x101d9691), X(0x10283dca), X(0x1032e81f),
X(0x103d9591), X(0x1048461e), X(0x1052f9c5), X(0x105db087),
X(0x10686a62), X(0x10732756), X(0x107de763), X(0x1088aa87),
X(0x109370c2), X(0x109e3a14), X(0x10a9067c), X(0x10b3d5f9),
X(0x10bea88b), X(0x10c97e31), X(0x10d456eb), X(0x10df32b8),
X(0x10ea1197), X(0x10f4f387), X(0x10ffd889), X(0x110ac09b),
X(0x1115abbe), X(0x112099ef), X(0x112b8b2f), X(0x11367f7d),
X(0x114176d9), X(0x114c7141), X(0x11576eb6), X(0x11626f36),
X(0x116d72c1), X(0x11787957), X(0x118382f6), X(0x118e8f9e),
X(0x11999f4f), X(0x11a4b208), X(0x11afc7c7), X(0x11bae08e),
X(0x11c5fc5a), X(0x11d11b2c), X(0x11dc3d02), X(0x11e761dd),
X(0x11f289ba), X(0x11fdb49b), X(0x1208e27e), X(0x12141362),
X(0x121f4748), X(0x122a7e2d), X(0x1235b812), X(0x1240f4f6),
X(0x124c34d9), X(0x125777b9), X(0x1262bd96), X(0x126e0670),
X(0x12795245), X(0x1284a115), X(0x128ff2e0), X(0x129b47a5),
X(0x12a69f63), X(0x12b1fa19), X(0x12bd57c7), X(0x12c8b86c),
X(0x12d41c08), X(0x12df829a), X(0x12eaec21), X(0x12f6589d),
X(0x1301c80c), X(0x130d3a6f), X(0x1318afc4), X(0x1324280b),
X(0x132fa344), X(0x133b216d), X(0x1346a286), X(0x1352268e),
X(0x135dad85), X(0x1369376a), X(0x1374c43c), X(0x138053fb),
X(0x138be6a5), X(0x13977c3b), X(0x13a314bc), X(0x13aeb026),
X(0x13ba4e79), X(0x13c5efb5), X(0x13d193d9), X(0x13dd3ae4),
X(0x13e8e4d6), X(0x13f491ad), X(0x1400416a), X(0x140bf40b),
X(0x1417a98f), X(0x142361f7), X(0x142f1d41), X(0x143adb6d),
X(0x14469c7a), X(0x14526067), X(0x145e2734), X(0x1469f0df),
X(0x1475bd69), X(0x14818cd0), X(0x148d5f15), X(0x14993435),
X(0x14a50c31), X(0x14b0e708), X(0x14bcc4b8), X(0x14c8a542),
X(0x14d488a5), X(0x14e06edf), X(0x14ec57f1), X(0x14f843d9),
X(0x15043297), X(0x1510242b), X(0x151c1892), X(0x15280fcd),
X(0x153409dc), X(0x154006bc), X(0x154c066e), X(0x155808f1),
X(0x15640e44), X(0x15701666), X(0x157c2157), X(0x15882f16),
X(0x15943fa2), X(0x15a052fb), X(0x15ac691f), X(0x15b8820f),
X(0x15c49dc8), X(0x15d0bc4c), X(0x15dcdd98), X(0x15e901ad),
X(0x15f52888), X(0x1601522b), X(0x160d7e93), X(0x1619adc1),
X(0x1625dfb3), X(0x16321469), X(0x163e4be2), X(0x164a861d),
X(0x1656c31a), X(0x166302d8), X(0x166f4555), X(0x167b8a92),
X(0x1687d28e), X(0x16941d47), X(0x16a06abe), X(0x16acbaf0),
X(0x16b90ddf), X(0x16c56388), X(0x16d1bbeb), X(0x16de1708),
X(0x16ea74dd), X(0x16f6d56a), X(0x170338ae), X(0x170f9ea8),
X(0x171c0758), X(0x172872bd), X(0x1734e0d6), X(0x174151a2),
X(0x174dc520), X(0x175a3b51), X(0x1766b432), X(0x17732fc4),
X(0x177fae05), X(0x178c2ef4), X(0x1798b292), X(0x17a538dd),
X(0x17b1c1d4), X(0x17be4d77), X(0x17cadbc5), X(0x17d76cbc),
X(0x17e4005e), X(0x17f096a7), X(0x17fd2f98), X(0x1809cb31),
X(0x1816696f), X(0x18230a53), X(0x182faddc), X(0x183c5408),
X(0x1848fcd8), X(0x1855a849), X(0x1862565d), X(0x186f0711),
X(0x187bba64), X(0x18887057), X(0x189528e9), X(0x18a1e418),
X(0x18aea1e3), X(0x18bb624b), X(0x18c8254e), X(0x18d4eaeb),
X(0x18e1b321), X(0x18ee7df1), X(0x18fb4b58), X(0x19081b57),
X(0x1914edec), X(0x1921c317), X(0x192e9ad6), X(0x193b7529),
X(0x19485210), X(0x19553189), X(0x19621393), X(0x196ef82e),
X(0x197bdf59), X(0x1988c913), X(0x1995b55c), X(0x19a2a432),
X(0x19af9595), X(0x19bc8983), X(0x19c97ffd), X(0x19d67900),
X(0x19e3748e), X(0x19f072a3), X(0x19fd7341), X(0x1a0a7665),
X(0x1a177c10), X(0x1a248440), X(0x1a318ef4), X(0x1a3e9c2c),
X(0x1a4babe7), X(0x1a58be24), X(0x1a65d2e2), X(0x1a72ea20),
X(0x1a8003de), X(0x1a8d201a), X(0x1a9a3ed5), X(0x1aa7600c),
X(0x1ab483bf), X(0x1ac1a9ee), X(0x1aced297), X(0x1adbfdba),
X(0x1ae92b56), X(0x1af65b69), X(0x1b038df4), X(0x1b10c2f5),
X(0x1b1dfa6b), X(0x1b2b3456), X(0x1b3870b5), X(0x1b45af87),
X(0x1b52f0ca), X(0x1b60347f), X(0x1b6d7aa4), X(0x1b7ac339),
X(0x1b880e3c), X(0x1b955bad), X(0x1ba2ab8b), X(0x1baffdd5),
X(0x1bbd528a), X(0x1bcaa9a9), X(0x1bd80332), X(0x1be55f24),
X(0x1bf2bd7d), X(0x1c001e3d), X(0x1c0d8164), X(0x1c1ae6ef),
X(0x1c284edf), X(0x1c35b932), X(0x1c4325e7), X(0x1c5094fe),
X(0x1c5e0677), X(0x1c6b7a4f), X(0x1c78f086), X(0x1c86691b),
X(0x1c93e40d), X(0x1ca1615c), X(0x1caee107), X(0x1cbc630c),
X(0x1cc9e76b), X(0x1cd76e23), X(0x1ce4f733), X(0x1cf2829a),
X(0x1d001057), X(0x1d0da06a), X(0x1d1b32d1), X(0x1d28c78c),
X(0x1d365e9a), X(0x1d43f7f9), X(0x1d5193a9), X(0x1d5f31aa),
X(0x1d6cd1f9), X(0x1d7a7497), X(0x1d881982), X(0x1d95c0ba),
X(0x1da36a3d), X(0x1db1160a), X(0x1dbec422), X(0x1dcc7482),
X(0x1dda272b), X(0x1de7dc1a), X(0x1df59350), X(0x1e034ccb),
X(0x1e11088a), X(0x1e1ec68c), X(0x1e2c86d1), X(0x1e3a4958),
X(0x1e480e20), X(0x1e55d527), X(0x1e639e6d), X(0x1e7169f1),
X(0x1e7f37b2), X(0x1e8d07b0), X(0x1e9ad9e8), X(0x1ea8ae5b),
X(0x1eb68507), X(0x1ec45dec), X(0x1ed23908), X(0x1ee0165b),
X(0x1eedf5e4), X(0x1efbd7a1), X(0x1f09bb92), X(0x1f17a1b6),
X(0x1f258a0d), X(0x1f337494), X(0x1f41614b), X(0x1f4f5032),
X(0x1f5d4147), X(0x1f6b3489), X(0x1f7929f7), X(0x1f872192),
X(0x1f951b56), X(0x1fa31744), X(0x1fb1155b), X(0x1fbf159a),
X(0x1fcd17ff), X(0x1fdb1c8b), X(0x1fe9233b), X(0x1ff72c0f),
X(0x20053706), X(0x20134420), X(0x2021535a), X(0x202f64b4),
X(0x203d782e), X(0x204b8dc6), X(0x2059a57c), X(0x2067bf4e),
X(0x2075db3b), X(0x2083f943), X(0x20921964), X(0x20a03b9e),
X(0x20ae5fef), X(0x20bc8657), X(0x20caaed5), X(0x20d8d967),
X(0x20e7060e), X(0x20f534c7), X(0x21036592), X(0x2111986e),
X(0x211fcd59), X(0x212e0454), X(0x213c3d5d), X(0x214a7873),
X(0x2158b594), X(0x2166f4c1), X(0x217535f8), X(0x21837938),
X(0x2191be81), X(0x21a005d0), X(0x21ae4f26), X(0x21bc9a81),
X(0x21cae7e0), X(0x21d93743), X(0x21e788a8), X(0x21f5dc0e),
X(0x22043174), X(0x221288da), X(0x2220e23e), X(0x222f3da0),
X(0x223d9afe), X(0x224bfa58), X(0x225a5bac), X(0x2268bef9),
X(0x2277243f), X(0x22858b7d), X(0x2293f4b0), X(0x22a25fda),
X(0x22b0ccf8), X(0x22bf3c09), X(0x22cdad0d), X(0x22dc2002),
X(0x22ea94e8), X(0x22f90bbe), X(0x23078482), X(0x2315ff33),
X(0x23247bd1), X(0x2332fa5b), X(0x23417acf), X(0x234ffd2c),
X(0x235e8173), X(0x236d07a0), X(0x237b8fb4), X(0x238a19ae),
X(0x2398a58c), X(0x23a7334d), X(0x23b5c2f1), X(0x23c45477),
X(0x23d2e7dd), X(0x23e17d22), X(0x23f01446), X(0x23fead47),
X(0x240d4825), X(0x241be4dd), X(0x242a8371), X(0x243923dd),
X(0x2447c622), X(0x24566a3e), X(0x24651031), X(0x2473b7f8),
X(0x24826194), X(0x24910d03), X(0x249fba44), X(0x24ae6957),
X(0x24bd1a39), X(0x24cbccea), X(0x24da816a), X(0x24e937b7),
X(0x24f7efcf), X(0x2506a9b3), X(0x25156560), X(0x252422d6),
X(0x2532e215), X(0x2541a31a), X(0x255065e4), X(0x255f2a74),
X(0x256df0c7), X(0x257cb8dd), X(0x258b82b4), X(0x259a4e4c),
X(0x25a91ba4), X(0x25b7eaba), X(0x25c6bb8e), X(0x25d58e1e),
X(0x25e46269), X(0x25f3386e), X(0x2602102d), X(0x2610e9a4),
X(0x261fc4d3), X(0x262ea1b7), X(0x263d8050), X(0x264c609e),
X(0x265b429e), X(0x266a2650), X(0x26790bb3), X(0x2687f2c6),
X(0x2696db88), X(0x26a5c5f7), X(0x26b4b213), X(0x26c39fda),
X(0x26d28f4c), X(0x26e18067), X(0x26f0732b), X(0x26ff6796),
X(0x270e5da7), X(0x271d555d), X(0x272c4eb7), X(0x273b49b5),
X(0x274a4654), X(0x27594495), X(0x27684475), X(0x277745f4),
X(0x27864910), X(0x27954dc9), X(0x27a4541e), X(0x27b35c0d),
X(0x27c26596), X(0x27d170b7), X(0x27e07d6f), X(0x27ef8bbd),
X(0x27fe9ba0), X(0x280dad18), X(0x281cc022), X(0x282bd4be),
X(0x283aeaeb), X(0x284a02a7), X(0x28591bf2), X(0x286836cb),
X(0x28775330), X(0x28867120), X(0x2895909b), X(0x28a4b19e),
X(0x28b3d42a), X(0x28c2f83d), X(0x28d21dd5), X(0x28e144f3),
X(0x28f06d94), X(0x28ff97b8), X(0x290ec35d), X(0x291df082),
X(0x292d1f27), X(0x293c4f4a), X(0x294b80eb), X(0x295ab407),
X(0x2969e89e), X(0x29791eaf), X(0x29885639), X(0x29978f3b),
X(0x29a6c9b3), X(0x29b605a0), X(0x29c54302), X(0x29d481d7),
X(0x29e3c21e), X(0x29f303d6), X(0x2a0246fd), X(0x2a118b94),
X(0x2a20d198), X(0x2a301909), X(0x2a3f61e6), X(0x2a4eac2c),
X(0x2a5df7dc), X(0x2a6d44f4), X(0x2a7c9374), X(0x2a8be359),
X(0x2a9b34a2), X(0x2aaa8750), X(0x2ab9db60), X(0x2ac930d1),
X(0x2ad887a3), X(0x2ae7dfd3), X(0x2af73962), X(0x2b06944e),
X(0x2b15f096), X(0x2b254e38), X(0x2b34ad34), X(0x2b440d89),
X(0x2b536f34), X(0x2b62d236), X(0x2b72368d), X(0x2b819c38),
X(0x2b910336), X(0x2ba06b86), X(0x2bafd526), X(0x2bbf4015),
X(0x2bceac53), X(0x2bde19de), X(0x2bed88b5), X(0x2bfcf8d7),
X(0x2c0c6a43), X(0x2c1bdcf7), X(0x2c2b50f3), X(0x2c3ac635),
X(0x2c4a3cbd), X(0x2c59b488), X(0x2c692d97), X(0x2c78a7e7),
X(0x2c882378), X(0x2c97a049), X(0x2ca71e58), X(0x2cb69da4),
X(0x2cc61e2c), X(0x2cd59ff0), X(0x2ce522ed), X(0x2cf4a723),
X(0x2d042c90), X(0x2d13b334), X(0x2d233b0d), X(0x2d32c41a),
X(0x2d424e5a), X(0x2d51d9cc), X(0x2d61666e), X(0x2d70f440),
X(0x2d808340), X(0x2d90136e), X(0x2d9fa4c7), X(0x2daf374c),
X(0x2dbecafa), X(0x2dce5fd1), X(0x2dddf5cf), X(0x2ded8cf4),
X(0x2dfd253d), X(0x2e0cbeab), X(0x2e1c593b), X(0x2e2bf4ed),
X(0x2e3b91c0), X(0x2e4b2fb1), X(0x2e5acec1), X(0x2e6a6eee),
X(0x2e7a1037), X(0x2e89b29b), X(0x2e995618), X(0x2ea8faad),
X(0x2eb8a05a), X(0x2ec8471c), X(0x2ed7eef4), X(0x2ee797df),
X(0x2ef741dc), X(0x2f06eceb), X(0x2f16990a), X(0x2f264639),
X(0x2f35f475), X(0x2f45a3bd), X(0x2f555412), X(0x2f650570),
X(0x2f74b7d8), X(0x2f846b48), X(0x2f941fbe), X(0x2fa3d53a),
X(0x2fb38bbb), X(0x2fc3433f), X(0x2fd2fbc5), X(0x2fe2b54c),
X(0x2ff26fd3), X(0x30022b58), X(0x3011e7db), X(0x3021a55a),
X(0x303163d4), X(0x30412348), X(0x3050e3b5), X(0x3060a519),
X(0x30706773), X(0x30802ac3), X(0x308fef06), X(0x309fb43d),
X(0x30af7a65), X(0x30bf417d), X(0x30cf0985), X(0x30ded27a),
X(0x30ee9c5d), X(0x30fe672b), X(0x310e32e3), X(0x311dff85),
X(0x312dcd0f), X(0x313d9b80), X(0x314d6ad7), X(0x315d3b12),
X(0x316d0c30), X(0x317cde31), X(0x318cb113), X(0x319c84d4),
X(0x31ac5974), X(0x31bc2ef1), X(0x31cc054b), X(0x31dbdc7f),
X(0x31ebb48e), X(0x31fb8d74), X(0x320b6733), X(0x321b41c7),
X(0x322b1d31), X(0x323af96e), X(0x324ad67e), X(0x325ab45f),
X(0x326a9311), X(0x327a7291), X(0x328a52e0), X(0x329a33fb),
X(0x32aa15e1), X(0x32b9f892), X(0x32c9dc0c), X(0x32d9c04d),
X(0x32e9a555), X(0x32f98b22), X(0x330971b4), X(0x33195909),
X(0x3329411f), X(0x333929f6), X(0x3349138c), X(0x3358fde1),
X(0x3368e8f2), X(0x3378d4c0), X(0x3388c147), X(0x3398ae89),
X(0x33a89c82), X(0x33b88b32), X(0x33c87a98), X(0x33d86ab2),
X(0x33e85b80), X(0x33f84d00), X(0x34083f30), X(0x34183210),
X(0x3428259f), X(0x343819db), X(0x34480ec3), X(0x34580455),
X(0x3467fa92), X(0x3477f176), X(0x3487e902), X(0x3497e134),
X(0x34a7da0a), X(0x34b7d384), X(0x34c7cda0), X(0x34d7c85e),
X(0x34e7c3bb), X(0x34f7bfb7), X(0x3507bc50), X(0x3517b985),
X(0x3527b756), X(0x3537b5c0), X(0x3547b4c3), X(0x3557b45d),
X(0x3567b48d), X(0x3577b552), X(0x3587b6aa), X(0x3597b895),
X(0x35a7bb12), X(0x35b7be1e), X(0x35c7c1b9), X(0x35d7c5e1),
X(0x35e7ca96), X(0x35f7cfd6), X(0x3607d5a0), X(0x3617dbf3),
X(0x3627e2cd), X(0x3637ea2d), X(0x3647f212), X(0x3657fa7b),
X(0x36680366), X(0x36780cd2), X(0x368816bf), X(0x3698212b),
X(0x36a82c14), X(0x36b83779), X(0x36c8435a), X(0x36d84fb4),
X(0x36e85c88), X(0x36f869d2), X(0x37087793), X(0x371885c9),
X(0x37289473), X(0x3738a38f), X(0x3748b31d), X(0x3758c31a),
X(0x3768d387), X(0x3778e461), X(0x3788f5a7), X(0x37990759),
X(0x37a91975), X(0x37b92bf9), X(0x37c93ee4), X(0x37d95236),
X(0x37e965ed), X(0x37f97a08), X(0x38098e85), X(0x3819a363),
X(0x3829b8a2), X(0x3839ce3f), X(0x3849e43a), X(0x3859fa91),
X(0x386a1143), X(0x387a284f), X(0x388a3fb4), X(0x389a5770),
X(0x38aa6f83), X(0x38ba87ea), X(0x38caa0a5), X(0x38dab9b2),
X(0x38ead311), X(0x38faecbf), X(0x390b06bc), X(0x391b2107),
X(0x392b3b9e), X(0x393b5680), X(0x394b71ac), X(0x395b8d20),
X(0x396ba8dc), X(0x397bc4dd), X(0x398be124), X(0x399bfdae),
X(0x39ac1a7a), X(0x39bc3788), X(0x39cc54d5), X(0x39dc7261),
X(0x39ec902a), X(0x39fcae2f), X(0x3a0ccc70), X(0x3a1ceaea),
X(0x3a2d099c), X(0x3a3d2885), X(0x3a4d47a5), X(0x3a5d66f9),
X(0x3a6d8680), X(0x3a7da63a), X(0x3a8dc625), X(0x3a9de63f),
X(0x3aae0688), X(0x3abe26fe), X(0x3ace47a0), X(0x3ade686d),
X(0x3aee8963), X(0x3afeaa82), X(0x3b0ecbc7), X(0x3b1eed32),
X(0x3b2f0ec2), X(0x3b3f3075), X(0x3b4f524a), X(0x3b5f7440),
X(0x3b6f9656), X(0x3b7fb889), X(0x3b8fdada), X(0x3b9ffd46),
X(0x3bb01fce), X(0x3bc0426e), X(0x3bd06526), X(0x3be087f6),
X(0x3bf0aada), X(0x3c00cdd4), X(0x3c10f0e0), X(0x3c2113fe),
X(0x3c31372d), X(0x3c415a6b), X(0x3c517db7), X(0x3c61a110),
X(0x3c71c475), X(0x3c81e7e4), X(0x3c920b5c), X(0x3ca22edc),
X(0x3cb25262), X(0x3cc275ee), X(0x3cd2997e), X(0x3ce2bd11),
X(0x3cf2e0a6), X(0x3d03043b), X(0x3d1327cf), X(0x3d234b61),
X(0x3d336ef0), X(0x3d43927a), X(0x3d53b5ff), X(0x3d63d97c),
X(0x3d73fcf1), X(0x3d84205c), X(0x3d9443bd), X(0x3da46711),
X(0x3db48a58), X(0x3dc4ad91), X(0x3dd4d0ba), X(0x3de4f3d1),
X(0x3df516d7), X(0x3e0539c9), X(0x3e155ca6), X(0x3e257f6d),
X(0x3e35a21d), X(0x3e45c4b4), X(0x3e55e731), X(0x3e660994),
X(0x3e762bda), X(0x3e864e03), X(0x3e96700d), X(0x3ea691f7),
X(0x3eb6b3bf), X(0x3ec6d565), X(0x3ed6f6e8), X(0x3ee71845),
X(0x3ef7397c), X(0x3f075a8c), X(0x3f177b73), X(0x3f279c30),
X(0x3f37bcc2), X(0x3f47dd27), X(0x3f57fd5f), X(0x3f681d68),
X(0x3f783d40), X(0x3f885ce7), X(0x3f987c5c), X(0x3fa89b9c),
X(0x3fb8baa7), X(0x3fc8d97c), X(0x3fd8f819), X(0x3fe9167e),
X(0x3ff934a8), X(0x40095296), X(0x40197049), X(0x40298dbd),
X(0x4039aaf2), X(0x4049c7e7), X(0x4059e49a), X(0x406a010a),
X(0x407a1d36), X(0x408a391d), X(0x409a54bd), X(0x40aa7015),
X(0x40ba8b25), X(0x40caa5ea), X(0x40dac063), X(0x40eada90),
X(0x40faf46e), X(0x410b0dfe), X(0x411b273d), X(0x412b402a),
X(0x413b58c4), X(0x414b710a), X(0x415b88fa), X(0x416ba093),
X(0x417bb7d5), X(0x418bcebe), X(0x419be54c), X(0x41abfb7e),
X(0x41bc1153), X(0x41cc26ca), X(0x41dc3be2), X(0x41ec5099),
X(0x41fc64ef), X(0x420c78e1), X(0x421c8c6f), X(0x422c9f97),
X(0x423cb258), X(0x424cc4b2), X(0x425cd6a2), X(0x426ce827),
X(0x427cf941), X(0x428d09ee), X(0x429d1a2c), X(0x42ad29fb),
X(0x42bd3959), X(0x42cd4846), X(0x42dd56bf), X(0x42ed64c3),
X(0x42fd7252), X(0x430d7f6a), X(0x431d8c0a), X(0x432d9831),
X(0x433da3dd), X(0x434daf0d), X(0x435db9c0), X(0x436dc3f5),
X(0x437dcdab), X(0x438dd6df), X(0x439ddf92), X(0x43ade7c1),
X(0x43bdef6c), X(0x43cdf691), X(0x43ddfd2f), X(0x43ee0345),
X(0x43fe08d2), X(0x440e0dd4), X(0x441e124b), X(0x442e1634),
X(0x443e198f), X(0x444e1c5a), X(0x445e1e95), X(0x446e203e),
X(0x447e2153), X(0x448e21d5), X(0x449e21c0), X(0x44ae2115),
X(0x44be1fd1), X(0x44ce1df4), X(0x44de1b7d), X(0x44ee186a),
X(0x44fe14ba), X(0x450e106b), X(0x451e0b7e), X(0x452e05ef),
X(0x453dffbf), X(0x454df8eb), X(0x455df173), X(0x456de956),
X(0x457de092), X(0x458dd726), X(0x459dcd10), X(0x45adc251),
X(0x45bdb6e5), X(0x45cdaacd), X(0x45dd9e06), X(0x45ed9091),
X(0x45fd826a), X(0x460d7392), X(0x461d6407), X(0x462d53c8),
X(0x463d42d4), X(0x464d3129), X(0x465d1ec6), X(0x466d0baa),
X(0x467cf7d3), X(0x468ce342), X(0x469ccdf3), X(0x46acb7e7),
X(0x46bca11c), X(0x46cc8990), X(0x46dc7143), X(0x46ec5833),
X(0x46fc3e5f), X(0x470c23c6), X(0x471c0867), X(0x472bec40),
X(0x473bcf50), X(0x474bb196), X(0x475b9311), X(0x476b73c0),
X(0x477b53a1), X(0x478b32b4), X(0x479b10f6), X(0x47aaee67),
X(0x47bacb06), X(0x47caa6d1), X(0x47da81c7), X(0x47ea5be7),
X(0x47fa3530), X(0x480a0da1), X(0x4819e537), X(0x4829bbf3),
X(0x483991d3), X(0x484966d6), X(0x48593afb), X(0x48690e3f),
X(0x4878e0a3), X(0x4888b225), X(0x489882c4), X(0x48a8527e),
X(0x48b82153), X(0x48c7ef41), X(0x48d7bc47), X(0x48e78863),
X(0x48f75396), X(0x49071ddc), X(0x4916e736), X(0x4926afa2),
X(0x4936771f), X(0x49463dac), X(0x49560347), X(0x4965c7ef),
X(0x49758ba4), X(0x49854e63), X(0x4995102c), X(0x49a4d0fe),
X(0x49b490d7), X(0x49c44fb6), X(0x49d40d9a), X(0x49e3ca82),
X(0x49f3866c), X(0x4a034159), X(0x4a12fb45), X(0x4a22b430),
X(0x4a326c19), X(0x4a4222ff), X(0x4a51d8e1), X(0x4a618dbd),
X(0x4a714192), X(0x4a80f45f), X(0x4a90a623), X(0x4aa056dd),
X(0x4ab0068b), X(0x4abfb52c), X(0x4acf62c0), X(0x4adf0f44),
X(0x4aeebab9), X(0x4afe651c), X(0x4b0e0e6c), X(0x4b1db6a9),
X(0x4b2d5dd1), X(0x4b3d03e2), X(0x4b4ca8dd), X(0x4b5c4cbf),
X(0x4b6bef88), X(0x4b7b9136), X(0x4b8b31c8), X(0x4b9ad13d),
X(0x4baa6f93), X(0x4bba0ccb), X(0x4bc9a8e2), X(0x4bd943d7),
X(0x4be8dda9), X(0x4bf87658), X(0x4c080de1), X(0x4c17a444),
X(0x4c27397f), X(0x4c36cd92), X(0x4c46607b), X(0x4c55f239),
X(0x4c6582cb), X(0x4c75122f), X(0x4c84a065), X(0x4c942d6c),
X(0x4ca3b942), X(0x4cb343e6), X(0x4cc2cd57), X(0x4cd25594),
X(0x4ce1dc9c), X(0x4cf1626d), X(0x4d00e707), X(0x4d106a68),
X(0x4d1fec8f), X(0x4d2f6d7a), X(0x4d3eed2a), X(0x4d4e6b9d),
X(0x4d5de8d1), X(0x4d6d64c5), X(0x4d7cdf79), X(0x4d8c58eb),
X(0x4d9bd11a), X(0x4dab4804), X(0x4dbabdaa), X(0x4dca3209),
X(0x4dd9a520), X(0x4de916ef), X(0x4df88774), X(0x4e07f6ae),
X(0x4e17649c), X(0x4e26d13c), X(0x4e363c8f), X(0x4e45a692),
X(0x4e550f44), X(0x4e6476a4), X(0x4e73dcb2), X(0x4e83416c),
X(0x4e92a4d1), X(0x4ea206df), X(0x4eb16796), X(0x4ec0c6f5),
X(0x4ed024fa), X(0x4edf81a5), X(0x4eeedcf3), X(0x4efe36e5),
X(0x4f0d8f79), X(0x4f1ce6ad), X(0x4f2c3c82), X(0x4f3b90f4),
X(0x4f4ae405), X(0x4f5a35b1), X(0x4f6985fa), X(0x4f78d4dc),
X(0x4f882257), X(0x4f976e6a), X(0x4fa6b914), X(0x4fb60254),
X(0x4fc54a28), X(0x4fd49090), X(0x4fe3d58b), X(0x4ff31917),
X(0x50025b33), X(0x50119bde), X(0x5020db17), X(0x503018dd),
X(0x503f552f), X(0x504e900b), X(0x505dc971), X(0x506d0160),
X(0x507c37d7), X(0x508b6cd3), X(0x509aa055), X(0x50a9d25b),
X(0x50b902e4), X(0x50c831ef), X(0x50d75f7b), X(0x50e68b87),
X(0x50f5b612), X(0x5104df1a), X(0x5114069f), X(0x51232ca0),
X(0x5132511a), X(0x5141740f), X(0x5150957b), X(0x515fb55f),
X(0x516ed3b8), X(0x517df087), X(0x518d0bca), X(0x519c257f),
X(0x51ab3da7), X(0x51ba543f), X(0x51c96947), X(0x51d87cbd),
X(0x51e78ea1), X(0x51f69ef1), X(0x5205adad), X(0x5214bad3),
X(0x5223c662), X(0x5232d05a), X(0x5241d8b9), X(0x5250df7d),
X(0x525fe4a7), X(0x526ee835), X(0x527dea26), X(0x528cea78),
X(0x529be92c), X(0x52aae63f), X(0x52b9e1b0), X(0x52c8db80),
X(0x52d7d3ac), X(0x52e6ca33), X(0x52f5bf15), X(0x5304b251),
X(0x5313a3e5), X(0x532293d0), X(0x53318212), X(0x53406ea8),
X(0x534f5993), X(0x535e42d2), X(0x536d2a62), X(0x537c1043),
X(0x538af475), X(0x5399d6f6), X(0x53a8b7c4), X(0x53b796e0),
X(0x53c67447), X(0x53d54ffa), X(0x53e429f6), X(0x53f3023b),
X(0x5401d8c8), X(0x5410ad9c), X(0x541f80b5), X(0x542e5213),
X(0x543d21b5), X(0x544bef9a), X(0x545abbc0), X(0x54698627),
X(0x54784ece), X(0x548715b3), X(0x5495dad6), X(0x54a49e35),
X(0x54b35fd0), X(0x54c21fa6), X(0x54d0ddb5), X(0x54df99fd),
X(0x54ee547c), X(0x54fd0d32), X(0x550bc41d), X(0x551a793d),
X(0x55292c91), X(0x5537de16), X(0x55468dce), X(0x55553bb6),
X(0x5563e7cd), X(0x55729213), X(0x55813a87), X(0x558fe127),
X(0x559e85f2), X(0x55ad28e9), X(0x55bbca08), X(0x55ca6950),
X(0x55d906c0), X(0x55e7a257), X(0x55f63c13), X(0x5604d3f4),
X(0x561369f8), X(0x5621fe1f), X(0x56309067), X(0x563f20d1),
X(0x564daf5a), X(0x565c3c02), X(0x566ac6c7), X(0x56794faa),
X(0x5687d6a8), X(0x56965bc1), X(0x56a4def4), X(0x56b36040),
X(0x56c1dfa4), X(0x56d05d1f), X(0x56ded8af), X(0x56ed5255),
X(0x56fbca0f), X(0x570a3fdc), X(0x5718b3bc), X(0x572725ac),
X(0x573595ad), X(0x574403bd), X(0x57526fdb), X(0x5760da07),
X(0x576f423f), X(0x577da883), X(0x578c0cd1), X(0x579a6f29),
X(0x57a8cf8a), X(0x57b72df2), X(0x57c58a61), X(0x57d3e4d6),
X(0x57e23d50), X(0x57f093cd), X(0x57fee84e), X(0x580d3ad1),
X(0x581b8b54), X(0x5829d9d8), X(0x5838265c), X(0x584670dd),
X(0x5854b95c), X(0x5862ffd8), X(0x5871444f), X(0x587f86c1),
X(0x588dc72c), X(0x589c0591), X(0x58aa41ed), X(0x58b87c40),
X(0x58c6b489), X(0x58d4eac7), X(0x58e31ef9), X(0x58f1511f),
X(0x58ff8137), X(0x590daf40), X(0x591bdb3a), X(0x592a0524),
X(0x59382cfc), X(0x594652c2), X(0x59547675), X(0x59629815),
X(0x5970b79f), X(0x597ed513), X(0x598cf071), X(0x599b09b7),
X(0x59a920e5), X(0x59b735f9), X(0x59c548f4), X(0x59d359d2),
X(0x59e16895), X(0x59ef753b), X(0x59fd7fc4), X(0x5a0b882d),
X(0x5a198e77), X(0x5a2792a0), X(0x5a3594a9), X(0x5a43948e),
X(0x5a519251), X(0x5a5f8df0), X(0x5a6d876a), X(0x5a7b7ebe),
X(0x5a8973ec), X(0x5a9766f2), X(0x5aa557d0), X(0x5ab34685),
X(0x5ac1330f), X(0x5acf1d6f), X(0x5add05a3), X(0x5aeaebaa),
X(0x5af8cf84), X(0x5b06b12f), X(0x5b1490ab), X(0x5b226df7),
X(0x5b304912), X(0x5b3e21fc), X(0x5b4bf8b2), X(0x5b59cd35),
X(0x5b679f84), X(0x5b756f9e), X(0x5b833d82), X(0x5b91092e),
X(0x5b9ed2a3), X(0x5bac99e0), X(0x5bba5ee3), X(0x5bc821ac),
X(0x5bd5e23a), X(0x5be3a08c), X(0x5bf15ca1), X(0x5bff1679),
X(0x5c0cce12), X(0x5c1a836c), X(0x5c283686), X(0x5c35e760),
X(0x5c4395f7), X(0x5c51424c), X(0x5c5eec5e), X(0x5c6c942b),
X(0x5c7a39b4), X(0x5c87dcf7), X(0x5c957df3), X(0x5ca31ca8),
X(0x5cb0b915), X(0x5cbe5338), X(0x5ccbeb12), X(0x5cd980a1),
X(0x5ce713e5), X(0x5cf4a4dd), X(0x5d023387), X(0x5d0fbfe4),
X(0x5d1d49f2), X(0x5d2ad1b1), X(0x5d38571f), X(0x5d45da3c),
X(0x5d535b08), X(0x5d60d981), X(0x5d6e55a7), X(0x5d7bcf78),
X(0x5d8946f5), X(0x5d96bc1c), X(0x5da42eec), X(0x5db19f65),
X(0x5dbf0d86), X(0x5dcc794e), X(0x5dd9e2bd), X(0x5de749d1),
X(0x5df4ae8a), X(0x5e0210e7), X(0x5e0f70e7), X(0x5e1cce8a),
X(0x5e2a29ce), X(0x5e3782b4), X(0x5e44d93a), X(0x5e522d5f),
X(0x5e5f7f23), X(0x5e6cce85), X(0x5e7a1b85), X(0x5e876620),
X(0x5e94ae58), X(0x5ea1f42a), X(0x5eaf3797), X(0x5ebc789d),
X(0x5ec9b73c), X(0x5ed6f372), X(0x5ee42d41), X(0x5ef164a5),
X(0x5efe999f), X(0x5f0bcc2f), X(0x5f18fc52), X(0x5f262a09),
X(0x5f335553), X(0x5f407e2f), X(0x5f4da49d), X(0x5f5ac89b),
X(0x5f67ea29), X(0x5f750946), X(0x5f8225f2), X(0x5f8f402b),
X(0x5f9c57f2), X(0x5fa96d44), X(0x5fb68023), X(0x5fc3908c),
X(0x5fd09e7f), X(0x5fdda9fc), X(0x5feab302), X(0x5ff7b990),
X(0x6004bda5), X(0x6011bf40), X(0x601ebe62), X(0x602bbb09),
X(0x6038b534), X(0x6045ace4), X(0x6052a216), X(0x605f94cb),
X(0x606c8502), X(0x607972b9), X(0x60865df2), X(0x609346aa),
X(0x60a02ce1), X(0x60ad1096), X(0x60b9f1c9), X(0x60c6d079),
X(0x60d3aca5), X(0x60e0864d), X(0x60ed5d70), X(0x60fa320d),
X(0x61070424), X(0x6113d3b4), X(0x6120a0bc), X(0x612d6b3c),
X(0x613a3332), X(0x6146f89f), X(0x6153bb82), X(0x61607bd9),
X(0x616d39a5), X(0x6179f4e5), X(0x6186ad98), X(0x619363bd),
X(0x61a01753), X(0x61acc85b), X(0x61b976d3), X(0x61c622bc),
X(0x61d2cc13), X(0x61df72d8), X(0x61ec170c), X(0x61f8b8ad),
X(0x620557ba), X(0x6211f434), X(0x621e8e18), X(0x622b2568),
X(0x6237ba21), X(0x62444c44), X(0x6250dbd0), X(0x625d68c4),
X(0x6269f320), X(0x62767ae2), X(0x6283000b), X(0x628f829a),
X(0x629c028e), X(0x62a87fe6), X(0x62b4faa2), X(0x62c172c2),
X(0x62cde844), X(0x62da5b29), X(0x62e6cb6e), X(0x62f33915),
X(0x62ffa41c), X(0x630c0c83), X(0x63187248), X(0x6324d56d),
X(0x633135ef), X(0x633d93ce), X(0x6349ef0b), X(0x635647a3),
X(0x63629d97), X(0x636ef0e6), X(0x637b418f), X(0x63878f92),
X(0x6393daef), X(0x63a023a4), X(0x63ac69b1), X(0x63b8ad15),
X(0x63c4edd1), X(0x63d12be3), X(0x63dd674b), X(0x63e9a008),
X(0x63f5d61a), X(0x64020980), X(0x640e3a39), X(0x641a6846),
X(0x642693a5), X(0x6432bc56), X(0x643ee258), X(0x644b05ab),
X(0x6457264e), X(0x64634441), X(0x646f5f83), X(0x647b7814),
X(0x64878df3), X(0x6493a120), X(0x649fb199), X(0x64abbf5f),
X(0x64b7ca71), X(0x64c3d2ce), X(0x64cfd877), X(0x64dbdb69),
X(0x64e7dba6), X(0x64f3d92b), X(0x64ffd3fa), X(0x650bcc11),
X(0x6517c16f), X(0x6523b415), X(0x652fa402), X(0x653b9134),
X(0x65477bad), X(0x6553636a), X(0x655f486d), X(0x656b2ab3),
X(0x65770a3d), X(0x6582e70a), X(0x658ec11a), X(0x659a986d),
X(0x65a66d00), X(0x65b23ed5), X(0x65be0deb), X(0x65c9da41),
X(0x65d5a3d7), X(0x65e16aac), X(0x65ed2ebf), X(0x65f8f011),
X(0x6604aea1), X(0x66106a6e), X(0x661c2377), X(0x6627d9be),
X(0x66338d40), X(0x663f3dfd), X(0x664aebf5), X(0x66569728),
X(0x66623f95), X(0x666de53b), X(0x6679881b), X(0x66852833),
X(0x6690c583), X(0x669c600b), X(0x66a7f7ca), X(0x66b38cc0),
X(0x66bf1eec), X(0x66caae4f), X(0x66d63ae6), X(0x66e1c4b3),
X(0x66ed4bb4), X(0x66f8cfea), X(0x67045153), X(0x670fcfef),
X(0x671b4bbe), X(0x6726c4bf), X(0x67323af3), X(0x673dae58),
X(0x67491eee), X(0x67548cb5), X(0x675ff7ab), X(0x676b5fd2),
X(0x6776c528), X(0x678227ad), X(0x678d8761), X(0x6798e443),
X(0x67a43e52), X(0x67af958f), X(0x67bae9f9), X(0x67c63b8f),
X(0x67d18a52), X(0x67dcd640), X(0x67e81f59), X(0x67f3659d),
X(0x67fea90c), X(0x6809e9a5), X(0x68152768), X(0x68206254),
X(0x682b9a68), X(0x6836cfa6), X(0x6842020b), X(0x684d3199),
X(0x68585e4d), X(0x68638829), X(0x686eaf2b), X(0x6879d354),
X(0x6884f4a2), X(0x68901316), X(0x689b2eb0), X(0x68a6476d),
X(0x68b15d50), X(0x68bc7056), X(0x68c78080), X(0x68d28dcd),
X(0x68dd983e), X(0x68e89fd0), X(0x68f3a486), X(0x68fea65d),
X(0x6909a555), X(0x6914a16f), X(0x691f9aa9), X(0x692a9104),
X(0x69358480), X(0x6940751b), X(0x694b62d5), X(0x69564daf),
X(0x696135a7), X(0x696c1abe), X(0x6976fcf3), X(0x6981dc46),
X(0x698cb8b6), X(0x69979243), X(0x69a268ed), X(0x69ad3cb4),
X(0x69b80d97), X(0x69c2db96), X(0x69cda6b0), X(0x69d86ee5),
X(0x69e33436), X(0x69edf6a1), X(0x69f8b626), X(0x6a0372c5),
X(0x6a0e2c7e), X(0x6a18e350), X(0x6a23973c), X(0x6a2e4840),
X(0x6a38f65d), X(0x6a43a191), X(0x6a4e49de), X(0x6a58ef42),
X(0x6a6391be), X(0x6a6e3151), X(0x6a78cdfa), X(0x6a8367ba),
X(0x6a8dfe90), X(0x6a98927c), X(0x6aa3237d), X(0x6aadb194),
X(0x6ab83cc0), X(0x6ac2c500), X(0x6acd4a55), X(0x6ad7ccbf),
X(0x6ae24c3c), X(0x6aecc8cd), X(0x6af74271), X(0x6b01b929),
X(0x6b0c2cf4), X(0x6b169dd1), X(0x6b210bc1), X(0x6b2b76c2),
X(0x6b35ded6), X(0x6b4043fc), X(0x6b4aa632), X(0x6b55057a),
X(0x6b5f61d3), X(0x6b69bb3d), X(0x6b7411b7), X(0x6b7e6541),
X(0x6b88b5db), X(0x6b930385), X(0x6b9d4e3f), X(0x6ba79607),
X(0x6bb1dadf), X(0x6bbc1cc6), X(0x6bc65bbb), X(0x6bd097bf),
X(0x6bdad0d0), X(0x6be506f0), X(0x6bef3a1d), X(0x6bf96a58),
X(0x6c0397a0), X(0x6c0dc1f5), X(0x6c17e957), X(0x6c220dc6),
X(0x6c2c2f41), X(0x6c364dc9), X(0x6c40695c), X(0x6c4a81fc),
X(0x6c5497a7), X(0x6c5eaa5d), X(0x6c68ba1f), X(0x6c72c6eb),
X(0x6c7cd0c3), X(0x6c86d7a6), X(0x6c90db92), X(0x6c9adc8a),
X(0x6ca4da8b), X(0x6caed596), X(0x6cb8cdab), X(0x6cc2c2ca),
X(0x6cccb4f2), X(0x6cd6a424), X(0x6ce0905e), X(0x6cea79a1),
X(0x6cf45fee), X(0x6cfe4342), X(0x6d0823a0), X(0x6d120105),
X(0x6d1bdb73), X(0x6d25b2e8), X(0x6d2f8765), X(0x6d3958ea),
X(0x6d432777), X(0x6d4cf30a), X(0x6d56bba5), X(0x6d608147),
X(0x6d6a43f0), X(0x6d7403a0), X(0x6d7dc056), X(0x6d877a13),
X(0x6d9130d6), X(0x6d9ae4a0), X(0x6da4956f), X(0x6dae4345),
X(0x6db7ee20), X(0x6dc19601), X(0x6dcb3ae7), X(0x6dd4dcd3),
X(0x6dde7bc4), X(0x6de817bb), X(0x6df1b0b6), X(0x6dfb46b7),
X(0x6e04d9bc), X(0x6e0e69c7), X(0x6e17f6d5), X(0x6e2180e9),
X(0x6e2b0801), X(0x6e348c1d), X(0x6e3e0d3d), X(0x6e478b62),
X(0x6e51068a), X(0x6e5a7eb7), X(0x6e63f3e7), X(0x6e6d661b),
X(0x6e76d552), X(0x6e80418e), X(0x6e89aacc), X(0x6e93110f),
X(0x6e9c7454), X(0x6ea5d49d), X(0x6eaf31e9), X(0x6eb88c37),
X(0x6ec1e389), X(0x6ecb37de), X(0x6ed48936), X(0x6eddd790),
X(0x6ee722ee), X(0x6ef06b4d), X(0x6ef9b0b0), X(0x6f02f315),
X(0x6f0c327c), X(0x6f156ee6), X(0x6f1ea852), X(0x6f27dec1),
X(0x6f311232), X(0x6f3a42a5), X(0x6f43701a), X(0x6f4c9a91),
X(0x6f55c20a), X(0x6f5ee686), X(0x6f680803), X(0x6f712682),
X(0x6f7a4203), X(0x6f835a86), X(0x6f8c700b), X(0x6f958291),
X(0x6f9e921a), X(0x6fa79ea4), X(0x6fb0a830), X(0x6fb9aebd),
X(0x6fc2b24c), X(0x6fcbb2dd), X(0x6fd4b06f), X(0x6fddab03),
X(0x6fe6a299), X(0x6fef9730), X(0x6ff888c9), X(0x70017763),
X(0x700a62ff), X(0x70134b9c), X(0x701c313b), X(0x702513dc),
X(0x702df37e), X(0x7036d021), X(0x703fa9c6), X(0x7048806d),
X(0x70515415), X(0x705a24bf), X(0x7062f26b), X(0x706bbd17),
X(0x707484c6), X(0x707d4976), X(0x70860b28), X(0x708ec9dc),
X(0x70978591), X(0x70a03e48), X(0x70a8f400), X(0x70b1a6bb),
X(0x70ba5677), X(0x70c30335), X(0x70cbacf5), X(0x70d453b6),
X(0x70dcf77a), X(0x70e59840), X(0x70ee3607), X(0x70f6d0d1),
X(0x70ff689d), X(0x7107fd6b), X(0x71108f3b), X(0x71191e0d),
X(0x7121a9e2), X(0x712a32b9), X(0x7132b892), X(0x713b3b6e),
X(0x7143bb4c), X(0x714c382d), X(0x7154b211), X(0x715d28f7),
X(0x71659ce0), X(0x716e0dcc), X(0x71767bbb), X(0x717ee6ac),
X(0x71874ea1), X(0x718fb399), X(0x71981594), X(0x71a07493),
X(0x71a8d094), X(0x71b1299a), X(0x71b97fa2), X(0x71c1d2af),
X(0x71ca22bf), X(0x71d26fd2), X(0x71dab9ea), X(0x71e30106),
X(0x71eb4526), X(0x71f3864a), X(0x71fbc472), X(0x7203ff9e),
X(0x720c37cf), X(0x72146d05), X(0x721c9f3f), X(0x7224ce7e),
X(0x722cfac2), X(0x7235240b), X(0x723d4a59), X(0x72456dad),
X(0x724d8e05), X(0x7255ab63), X(0x725dc5c7), X(0x7265dd31),
X(0x726df1a0), X(0x72760315), X(0x727e1191), X(0x72861d12),
X(0x728e259a), X(0x72962b28), X(0x729e2dbd), X(0x72a62d59),
X(0x72ae29fc), X(0x72b623a5), X(0x72be1a56), X(0x72c60e0e),
X(0x72cdfece), X(0x72d5ec95), X(0x72ddd764), X(0x72e5bf3b),
X(0x72eda41a), X(0x72f58601), X(0x72fd64f1), X(0x730540e9),
X(0x730d19e9), X(0x7314eff3), X(0x731cc305), X(0x73249321),
X(0x732c6046), X(0x73342a75), X(0x733bf1ad), X(0x7343b5ef),
X(0x734b773b), X(0x73533591), X(0x735af0f2), X(0x7362a95d),
X(0x736a5ed3), X(0x73721153), X(0x7379c0df), X(0x73816d76),
X(0x73891719), X(0x7390bdc7), X(0x73986181), X(0x73a00247),
X(0x73a7a01a), X(0x73af3af8), X(0x73b6d2e4), X(0x73be67dc),
X(0x73c5f9e1), X(0x73cd88f3), X(0x73d51513), X(0x73dc9e40),
X(0x73e4247c), X(0x73eba7c5), X(0x73f3281c), X(0x73faa582),
X(0x74021ff7), X(0x7409977b), X(0x74110c0d), X(0x74187daf),
X(0x741fec61), X(0x74275822), X(0x742ec0f3), X(0x743626d5),
X(0x743d89c7), X(0x7444e9c9), X(0x744c46dd), X(0x7453a101),
X(0x745af837), X(0x74624c7f), X(0x74699dd8), X(0x7470ec44),
X(0x747837c2), X(0x747f8052), X(0x7486c5f5), X(0x748e08ac),
X(0x74954875), X(0x749c8552), X(0x74a3bf43), X(0x74aaf648),
X(0x74b22a62), X(0x74b95b90), X(0x74c089d2), X(0x74c7b52a),
X(0x74cedd97), X(0x74d6031a), X(0x74dd25b2), X(0x74e44561),
X(0x74eb6226), X(0x74f27c02), X(0x74f992f5), X(0x7500a6ff),
X(0x7507b820), X(0x750ec659), X(0x7515d1aa), X(0x751cda14),
X(0x7523df96), X(0x752ae231), X(0x7531e1e5), X(0x7538deb2),
X(0x753fd89a), X(0x7546cf9b), X(0x754dc3b7), X(0x7554b4ed),
X(0x755ba33e), X(0x75628eaa), X(0x75697732), X(0x75705cd5),
X(0x75773f95), X(0x757e1f71), X(0x7584fc6a), X(0x758bd67f),
X(0x7592adb2), X(0x75998203), X(0x75a05371), X(0x75a721fe),
X(0x75adeda9), X(0x75b4b673), X(0x75bb7c5c), X(0x75c23f65),
X(0x75c8ff8d), X(0x75cfbcd6), X(0x75d6773f), X(0x75dd2ec8),
X(0x75e3e373), X(0x75ea953f), X(0x75f1442d), X(0x75f7f03d),
X(0x75fe996f), X(0x76053fc5), X(0x760be33d), X(0x761283d8),
X(0x76192197), X(0x761fbc7b), X(0x76265482), X(0x762ce9af),
X(0x76337c01), X(0x763a0b78), X(0x76409814), X(0x764721d7),
X(0x764da8c1), X(0x76542cd1), X(0x765aae08), X(0x76612c67),
X(0x7667a7ee), X(0x766e209d), X(0x76749675), X(0x767b0975),
X(0x7681799f), X(0x7687e6f3), X(0x768e5170), X(0x7694b918),
X(0x769b1deb), X(0x76a17fe9), X(0x76a7df13), X(0x76ae3b68),
X(0x76b494ea), X(0x76baeb98), X(0x76c13f74), X(0x76c7907c),
X(0x76cddeb3), X(0x76d42a18), X(0x76da72ab), X(0x76e0b86d),
X(0x76e6fb5e), X(0x76ed3b7f), X(0x76f378d0), X(0x76f9b352),
X(0x76ffeb05), X(0x77061fe8), X(0x770c51fe), X(0x77128145),
X(0x7718adbf), X(0x771ed76c), X(0x7724fe4c), X(0x772b225f),
X(0x773143a7), X(0x77376223), X(0x773d7dd3), X(0x774396ba),
X(0x7749acd5), X(0x774fc027), X(0x7755d0af), X(0x775bde6f),
X(0x7761e965), X(0x7767f193), X(0x776df6fa), X(0x7773f998),
X(0x7779f970), X(0x777ff681), X(0x7785f0cd), X(0x778be852),
X(0x7791dd12), X(0x7797cf0d), X(0x779dbe43), X(0x77a3aab6),
X(0x77a99465), X(0x77af7b50), X(0x77b55f79), X(0x77bb40e0),
X(0x77c11f85), X(0x77c6fb68), X(0x77ccd48a), X(0x77d2aaec),
X(0x77d87e8d), X(0x77de4f6f), X(0x77e41d92), X(0x77e9e8f5),
X(0x77efb19b), X(0x77f57782), X(0x77fb3aad), X(0x7800fb1a),
X(0x7806b8ca), X(0x780c73bf), X(0x78122bf7), X(0x7817e175),
X(0x781d9438), X(0x78234440), X(0x7828f18f), X(0x782e9c25),
X(0x78344401), X(0x7839e925), X(0x783f8b92), X(0x78452b46),
X(0x784ac844), X(0x7850628b), X(0x7855fa1c), X(0x785b8ef8),
X(0x7861211e), X(0x7866b090), X(0x786c3d4d), X(0x7871c757),
X(0x78774ead), X(0x787cd351), X(0x78825543), X(0x7887d483),
X(0x788d5111), X(0x7892caef), X(0x7898421c), X(0x789db69a),
X(0x78a32868), X(0x78a89787), X(0x78ae03f8), X(0x78b36dbb),
X(0x78b8d4d1), X(0x78be393a), X(0x78c39af6), X(0x78c8fa06),
X(0x78ce566c), X(0x78d3b026), X(0x78d90736), X(0x78de5b9c),
X(0x78e3ad58), X(0x78e8fc6c), X(0x78ee48d7), X(0x78f3929b),
X(0x78f8d9b7), X(0x78fe1e2c), X(0x79035ffb), X(0x79089f24),
X(0x790ddba8), X(0x79131587), X(0x79184cc2), X(0x791d8159),
X(0x7922b34d), X(0x7927e29e), X(0x792d0f4d), X(0x7932395a),
X(0x793760c6), X(0x793c8591), X(0x7941a7bd), X(0x7946c749),
X(0x794be435), X(0x7950fe84), X(0x79561634), X(0x795b2b47),
X(0x79603dbc), X(0x79654d96), X(0x796a5ad4), X(0x796f6576),
X(0x79746d7e), X(0x797972eb), X(0x797e75bf), X(0x798375f9),
X(0x7988739b), X(0x798d6ea5), X(0x79926717), X(0x79975cf2),
X(0x799c5037), X(0x79a140e6), X(0x79a62f00), X(0x79ab1a85),
X(0x79b00376), X(0x79b4e9d3), X(0x79b9cd9d), X(0x79beaed4),
X(0x79c38d79), X(0x79c8698d), X(0x79cd4310), X(0x79d21a03),
X(0x79d6ee66), X(0x79dbc03a), X(0x79e08f7f), X(0x79e55c36),
X(0x79ea265f), X(0x79eeedfc), X(0x79f3b30c), X(0x79f87590),
X(0x79fd3589), X(0x7a01f2f7), X(0x7a06addc), X(0x7a0b6636),
X(0x7a101c08), X(0x7a14cf52), X(0x7a198013), X(0x7a1e2e4d),
X(0x7a22da01), X(0x7a27832f), X(0x7a2c29d7), X(0x7a30cdfa),
X(0x7a356f99), X(0x7a3a0eb4), X(0x7a3eab4c), X(0x7a434561),
X(0x7a47dcf5), X(0x7a4c7207), X(0x7a510498), X(0x7a5594a9),
X(0x7a5a223a), X(0x7a5ead4d), X(0x7a6335e0), X(0x7a67bbf6),
X(0x7a6c3f8f), X(0x7a70c0ab), X(0x7a753f4b), X(0x7a79bb6f),
X(0x7a7e3519), X(0x7a82ac48), X(0x7a8720fe), X(0x7a8b933b),
X(0x7a9002ff), X(0x7a94704b), X(0x7a98db20), X(0x7a9d437e),
X(0x7aa1a967), X(0x7aa60cd9), X(0x7aaa6dd7), X(0x7aaecc61),
X(0x7ab32877), X(0x7ab7821b), X(0x7abbd94b), X(0x7ac02e0a),
X(0x7ac48058), X(0x7ac8d035), X(0x7acd1da3), X(0x7ad168a1),
X(0x7ad5b130), X(0x7ad9f751), X(0x7ade3b05), X(0x7ae27c4c),
X(0x7ae6bb27), X(0x7aeaf796), X(0x7aef319a), X(0x7af36934),
X(0x7af79e64), X(0x7afbd12c), X(0x7b00018a), X(0x7b042f81),
X(0x7b085b10), X(0x7b0c8439), X(0x7b10aafc), X(0x7b14cf5a),
X(0x7b18f153), X(0x7b1d10e8), X(0x7b212e1a), X(0x7b2548e9),
X(0x7b296155), X(0x7b2d7761), X(0x7b318b0b), X(0x7b359c55),
X(0x7b39ab3f), X(0x7b3db7cb), X(0x7b41c1f8), X(0x7b45c9c8),
X(0x7b49cf3b), X(0x7b4dd251), X(0x7b51d30b), X(0x7b55d16b),
X(0x7b59cd70), X(0x7b5dc71b), X(0x7b61be6d), X(0x7b65b366),
X(0x7b69a608), X(0x7b6d9653), X(0x7b718447), X(0x7b756fe5),
X(0x7b79592e), X(0x7b7d4022), X(0x7b8124c3), X(0x7b850710),
X(0x7b88e70a), X(0x7b8cc4b3), X(0x7b90a00a), X(0x7b947911),
X(0x7b984fc8), X(0x7b9c242f), X(0x7b9ff648), X(0x7ba3c612),
X(0x7ba79390), X(0x7bab5ec1), X(0x7baf27a5), X(0x7bb2ee3f),
X(0x7bb6b28e), X(0x7bba7493), X(0x7bbe344e), X(0x7bc1f1c1),
X(0x7bc5acec), X(0x7bc965cf), X(0x7bcd1c6c), X(0x7bd0d0c3),
X(0x7bd482d4), X(0x7bd832a1), X(0x7bdbe02a), X(0x7bdf8b70),
X(0x7be33473), X(0x7be6db34), X(0x7bea7fb4), X(0x7bee21f4),
X(0x7bf1c1f3), X(0x7bf55fb3), X(0x7bf8fb35), X(0x7bfc9479),
X(0x7c002b7f), X(0x7c03c04a), X(0x7c0752d8), X(0x7c0ae32b),
X(0x7c0e7144), X(0x7c11fd23), X(0x7c1586c9), X(0x7c190e36),
X(0x7c1c936c), X(0x7c20166b), X(0x7c239733), X(0x7c2715c6),
X(0x7c2a9224), X(0x7c2e0c4e), X(0x7c318444), X(0x7c34fa07),
X(0x7c386d98), X(0x7c3bdef8), X(0x7c3f4e26), X(0x7c42bb25),
X(0x7c4625f4), X(0x7c498e95), X(0x7c4cf507), X(0x7c50594c),
X(0x7c53bb65), X(0x7c571b51), X(0x7c5a7913), X(0x7c5dd4aa),
X(0x7c612e17), X(0x7c64855b), X(0x7c67da76), X(0x7c6b2d6a),
X(0x7c6e7e37), X(0x7c71ccdd), X(0x7c75195e), X(0x7c7863ba),
X(0x7c7babf1), X(0x7c7ef206), X(0x7c8235f7), X(0x7c8577c6),
X(0x7c88b774), X(0x7c8bf502), X(0x7c8f306f), X(0x7c9269bd),
X(0x7c95a0ec), X(0x7c98d5fe), X(0x7c9c08f2), X(0x7c9f39cb),
X(0x7ca26887), X(0x7ca59528), X(0x7ca8bfb0), X(0x7cabe81d),
X(0x7caf0e72), X(0x7cb232af), X(0x7cb554d4), X(0x7cb874e2),
X(0x7cbb92db), X(0x7cbeaebe), X(0x7cc1c88d), X(0x7cc4e047),
X(0x7cc7f5ef), X(0x7ccb0984), X(0x7cce1b08), X(0x7cd12a7b),
X(0x7cd437dd), X(0x7cd74330), X(0x7cda4c74), X(0x7cdd53aa),
X(0x7ce058d3), X(0x7ce35bef), X(0x7ce65cff), X(0x7ce95c04),
X(0x7cec58ff), X(0x7cef53f0), X(0x7cf24cd7), X(0x7cf543b7),
X(0x7cf8388f), X(0x7cfb2b60), X(0x7cfe1c2b), X(0x7d010af1),
X(0x7d03f7b2), X(0x7d06e26f), X(0x7d09cb29), X(0x7d0cb1e0),
X(0x7d0f9696), X(0x7d12794b), X(0x7d1559ff), X(0x7d1838b4),
X(0x7d1b156a), X(0x7d1df022), X(0x7d20c8dd), X(0x7d239f9b),
X(0x7d26745e), X(0x7d294725), X(0x7d2c17f1), X(0x7d2ee6c4),
X(0x7d31b39f), X(0x7d347e81), X(0x7d37476b), X(0x7d3a0e5f),
X(0x7d3cd35d), X(0x7d3f9665), X(0x7d425779), X(0x7d451699),
X(0x7d47d3c6), X(0x7d4a8f01), X(0x7d4d484b), X(0x7d4fffa3),
X(0x7d52b50c), X(0x7d556885), X(0x7d581a0f), X(0x7d5ac9ac),
X(0x7d5d775c), X(0x7d60231f), X(0x7d62ccf6), X(0x7d6574e3),
X(0x7d681ae6), X(0x7d6abeff), X(0x7d6d612f), X(0x7d700178),
X(0x7d729fd9), X(0x7d753c54), X(0x7d77d6e9), X(0x7d7a6f9a),
X(0x7d7d0666), X(0x7d7f9b4f), X(0x7d822e55), X(0x7d84bf79),
X(0x7d874ebc), X(0x7d89dc1e), X(0x7d8c67a1), X(0x7d8ef144),
X(0x7d91790a), X(0x7d93fef2), X(0x7d9682fd), X(0x7d99052d),
X(0x7d9b8581), X(0x7d9e03fb), X(0x7da0809b), X(0x7da2fb62),
X(0x7da57451), X(0x7da7eb68), X(0x7daa60a8), X(0x7dacd413),
X(0x7daf45a9), X(0x7db1b56a), X(0x7db42357), X(0x7db68f71),
X(0x7db8f9b9), X(0x7dbb6230), X(0x7dbdc8d6), X(0x7dc02dac),
X(0x7dc290b3), X(0x7dc4f1eb), X(0x7dc75156), X(0x7dc9aef4),
X(0x7dcc0ac5), X(0x7dce64cc), X(0x7dd0bd07), X(0x7dd31379),
X(0x7dd56821), X(0x7dd7bb01), X(0x7dda0c1a), X(0x7ddc5b6b),
X(0x7ddea8f7), X(0x7de0f4bd), X(0x7de33ebe), X(0x7de586fc),
X(0x7de7cd76), X(0x7dea122e), X(0x7dec5525), X(0x7dee965a),
X(0x7df0d5d0), X(0x7df31386), X(0x7df54f7e), X(0x7df789b8),
X(0x7df9c235), X(0x7dfbf8f5), X(0x7dfe2dfa), X(0x7e006145),
X(0x7e0292d5), X(0x7e04c2ac), X(0x7e06f0cb), X(0x7e091d32),
X(0x7e0b47e1), X(0x7e0d70db), X(0x7e0f981f), X(0x7e11bdaf),
X(0x7e13e18a), X(0x7e1603b3), X(0x7e182429), X(0x7e1a42ed),
X(0x7e1c6001), X(0x7e1e7b64), X(0x7e209518), X(0x7e22ad1d),
X(0x7e24c375), X(0x7e26d81f), X(0x7e28eb1d), X(0x7e2afc70),
X(0x7e2d0c17), X(0x7e2f1a15), X(0x7e31266a), X(0x7e333115),
X(0x7e353a1a), X(0x7e374177), X(0x7e39472e), X(0x7e3b4b3f),
X(0x7e3d4dac), X(0x7e3f4e75), X(0x7e414d9a), X(0x7e434b1e),
X(0x7e4546ff), X(0x7e474140), X(0x7e4939e0), X(0x7e4b30e2),
X(0x7e4d2644), X(0x7e4f1a09), X(0x7e510c30), X(0x7e52fcbc),
X(0x7e54ebab), X(0x7e56d900), X(0x7e58c4bb), X(0x7e5aaedd),
X(0x7e5c9766), X(0x7e5e7e57), X(0x7e6063b2), X(0x7e624776),
X(0x7e6429a5), X(0x7e660a3f), X(0x7e67e945), X(0x7e69c6b8),
X(0x7e6ba299), X(0x7e6d7ce7), X(0x7e6f55a5), X(0x7e712cd3),
X(0x7e730272), X(0x7e74d682), X(0x7e76a904), X(0x7e7879f9),
X(0x7e7a4962), X(0x7e7c173f), X(0x7e7de392), X(0x7e7fae5a),
X(0x7e817799), X(0x7e833f50), X(0x7e85057f), X(0x7e86ca27),
X(0x7e888d49), X(0x7e8a4ee5), X(0x7e8c0efd), X(0x7e8dcd91),
X(0x7e8f8aa1), X(0x7e914630), X(0x7e93003c), X(0x7e94b8c8),
X(0x7e966fd4), X(0x7e982560), X(0x7e99d96e), X(0x7e9b8bfe),
X(0x7e9d3d10), X(0x7e9eeca7), X(0x7ea09ac2), X(0x7ea24762),
X(0x7ea3f288), X(0x7ea59c35), X(0x7ea7446a), X(0x7ea8eb27),
X(0x7eaa906c), X(0x7eac343c), X(0x7eadd696), X(0x7eaf777b),
X(0x7eb116ed), X(0x7eb2b4eb), X(0x7eb45177), X(0x7eb5ec91),
X(0x7eb7863b), X(0x7eb91e74), X(0x7ebab53e), X(0x7ebc4a99),
X(0x7ebdde87), X(0x7ebf7107), X(0x7ec1021b), X(0x7ec291c3),
X(0x7ec42001), X(0x7ec5acd5), X(0x7ec7383f), X(0x7ec8c241),
X(0x7eca4adb), X(0x7ecbd20d), X(0x7ecd57da), X(0x7ecedc41),
X(0x7ed05f44), X(0x7ed1e0e2), X(0x7ed3611d), X(0x7ed4dff6),
X(0x7ed65d6d), X(0x7ed7d983), X(0x7ed95438), X(0x7edacd8f),
X(0x7edc4586), X(0x7eddbc20), X(0x7edf315c), X(0x7ee0a53c),
X(0x7ee217c1), X(0x7ee388ea), X(0x7ee4f8b9), X(0x7ee6672f),
X(0x7ee7d44c), X(0x7ee94012), X(0x7eeaaa80), X(0x7eec1397),
X(0x7eed7b59), X(0x7eeee1c6), X(0x7ef046df), X(0x7ef1aaa5),
X(0x7ef30d18), X(0x7ef46e39), X(0x7ef5ce09), X(0x7ef72c88),
X(0x7ef889b8), X(0x7ef9e599), X(0x7efb402c), X(0x7efc9972),
X(0x7efdf16b), X(0x7eff4818), X(0x7f009d79), X(0x7f01f191),
X(0x7f03445f), X(0x7f0495e4), X(0x7f05e620), X(0x7f073516),
X(0x7f0882c5), X(0x7f09cf2d), X(0x7f0b1a51), X(0x7f0c6430),
X(0x7f0daccc), X(0x7f0ef425), X(0x7f103a3b), X(0x7f117f11),
X(0x7f12c2a5), X(0x7f1404fa), X(0x7f15460f), X(0x7f1685e6),
X(0x7f17c47f), X(0x7f1901db), X(0x7f1a3dfb), X(0x7f1b78e0),
X(0x7f1cb28a), X(0x7f1deafa), X(0x7f1f2231), X(0x7f20582f),
X(0x7f218cf5), X(0x7f22c085), X(0x7f23f2de), X(0x7f252401),
X(0x7f2653f0), X(0x7f2782ab), X(0x7f28b032), X(0x7f29dc87),
X(0x7f2b07aa), X(0x7f2c319c), X(0x7f2d5a5e), X(0x7f2e81f0),
X(0x7f2fa853), X(0x7f30cd88), X(0x7f31f18f), X(0x7f33146a),
X(0x7f343619), X(0x7f35569c), X(0x7f3675f6), X(0x7f379425),
X(0x7f38b12c), X(0x7f39cd0a), X(0x7f3ae7c0), X(0x7f3c0150),
X(0x7f3d19ba), X(0x7f3e30fe), X(0x7f3f471e), X(0x7f405c1a),
X(0x7f416ff3), X(0x7f4282a9), X(0x7f43943e), X(0x7f44a4b2),
X(0x7f45b405), X(0x7f46c239), X(0x7f47cf4e), X(0x7f48db45),
X(0x7f49e61f), X(0x7f4aefdc), X(0x7f4bf87e), X(0x7f4d0004),
X(0x7f4e0670), X(0x7f4f0bc2), X(0x7f500ffb), X(0x7f51131c),
X(0x7f521525), X(0x7f531618), X(0x7f5415f4), X(0x7f5514bb),
X(0x7f56126e), X(0x7f570f0c), X(0x7f580a98), X(0x7f590511),
X(0x7f59fe78), X(0x7f5af6ce), X(0x7f5bee14), X(0x7f5ce44a),
X(0x7f5dd972), X(0x7f5ecd8b), X(0x7f5fc097), X(0x7f60b296),
X(0x7f61a389), X(0x7f629370), X(0x7f63824e), X(0x7f647021),
X(0x7f655ceb), X(0x7f6648ad), X(0x7f673367), X(0x7f681d19),
X(0x7f6905c6), X(0x7f69ed6d), X(0x7f6ad40f), X(0x7f6bb9ad),
X(0x7f6c9e48), X(0x7f6d81e0), X(0x7f6e6475), X(0x7f6f460a),
X(0x7f70269d), X(0x7f710631), X(0x7f71e4c6), X(0x7f72c25c),
X(0x7f739ef4), X(0x7f747a8f), X(0x7f75552e), X(0x7f762ed1),
X(0x7f770779), X(0x7f77df27), X(0x7f78b5db), X(0x7f798b97),
X(0x7f7a605a), X(0x7f7b3425), X(0x7f7c06fa), X(0x7f7cd8d9),
X(0x7f7da9c2), X(0x7f7e79b7), X(0x7f7f48b8), X(0x7f8016c5),
X(0x7f80e3e0), X(0x7f81b009), X(0x7f827b40), X(0x7f834588),
X(0x7f840edf), X(0x7f84d747), X(0x7f859ec1), X(0x7f86654d),
X(0x7f872aec), X(0x7f87ef9e), X(0x7f88b365), X(0x7f897641),
X(0x7f8a3832), X(0x7f8af93a), X(0x7f8bb959), X(0x7f8c7890),
X(0x7f8d36df), X(0x7f8df448), X(0x7f8eb0ca), X(0x7f8f6c67),
X(0x7f90271e), X(0x7f90e0f2), X(0x7f9199e2), X(0x7f9251f0),
X(0x7f93091b), X(0x7f93bf65), X(0x7f9474ce), X(0x7f952958),
X(0x7f95dd01), X(0x7f968fcd), X(0x7f9741ba), X(0x7f97f2ca),
X(0x7f98a2fd), X(0x7f995254), X(0x7f9a00d0), X(0x7f9aae71),
X(0x7f9b5b38), X(0x7f9c0726), X(0x7f9cb23b), X(0x7f9d5c78),
X(0x7f9e05de), X(0x7f9eae6e), X(0x7f9f5627), X(0x7f9ffd0b),
X(0x7fa0a31b), X(0x7fa14856), X(0x7fa1ecbf), X(0x7fa29054),
X(0x7fa33318), X(0x7fa3d50b), X(0x7fa4762c), X(0x7fa5167e),
X(0x7fa5b601), X(0x7fa654b5), X(0x7fa6f29b), X(0x7fa78fb3),
X(0x7fa82bff), X(0x7fa8c77f), X(0x7fa96234), X(0x7fa9fc1e),
X(0x7faa953e), X(0x7fab2d94), X(0x7fabc522), X(0x7fac5be8),
X(0x7facf1e6), X(0x7fad871d), X(0x7fae1b8f), X(0x7faeaf3b),
X(0x7faf4222), X(0x7fafd445), X(0x7fb065a4), X(0x7fb0f641),
X(0x7fb1861b), X(0x7fb21534), X(0x7fb2a38c), X(0x7fb33124),
X(0x7fb3bdfb), X(0x7fb44a14), X(0x7fb4d56f), X(0x7fb5600c),
X(0x7fb5e9ec), X(0x7fb6730f), X(0x7fb6fb76), X(0x7fb78323),
X(0x7fb80a15), X(0x7fb8904d), X(0x7fb915cc), X(0x7fb99a92),
X(0x7fba1ea0), X(0x7fbaa1f7), X(0x7fbb2497), X(0x7fbba681),
X(0x7fbc27b5), X(0x7fbca835), X(0x7fbd2801), X(0x7fbda719),
X(0x7fbe257e), X(0x7fbea331), X(0x7fbf2032), X(0x7fbf9c82),
X(0x7fc01821), X(0x7fc09311), X(0x7fc10d52), X(0x7fc186e4),
X(0x7fc1ffc8), X(0x7fc277ff), X(0x7fc2ef89), X(0x7fc36667),
X(0x7fc3dc9a), X(0x7fc45221), X(0x7fc4c6ff), X(0x7fc53b33),
X(0x7fc5aebe), X(0x7fc621a0), X(0x7fc693db), X(0x7fc7056f),
X(0x7fc7765c), X(0x7fc7e6a3), X(0x7fc85645), X(0x7fc8c542),
X(0x7fc9339b), X(0x7fc9a150), X(0x7fca0e63), X(0x7fca7ad3),
X(0x7fcae6a2), X(0x7fcb51cf), X(0x7fcbbc5c), X(0x7fcc2649),
X(0x7fcc8f97), X(0x7fccf846), X(0x7fcd6058), X(0x7fcdc7cb),
X(0x7fce2ea2), X(0x7fce94dd), X(0x7fcefa7b), X(0x7fcf5f7f),
X(0x7fcfc3e8), X(0x7fd027b7), X(0x7fd08aed), X(0x7fd0ed8b),
X(0x7fd14f90), X(0x7fd1b0fd), X(0x7fd211d4), X(0x7fd27214),
X(0x7fd2d1bf), X(0x7fd330d4), X(0x7fd38f55), X(0x7fd3ed41),
X(0x7fd44a9a), X(0x7fd4a761), X(0x7fd50395), X(0x7fd55f37),
X(0x7fd5ba48), X(0x7fd614c9), X(0x7fd66eba), X(0x7fd6c81b),
X(0x7fd720ed), X(0x7fd77932), X(0x7fd7d0e8), X(0x7fd82812),
X(0x7fd87eae), X(0x7fd8d4bf), X(0x7fd92a45), X(0x7fd97f40),
X(0x7fd9d3b0), X(0x7fda2797), X(0x7fda7af5), X(0x7fdacdca),
X(0x7fdb2018), X(0x7fdb71dd), X(0x7fdbc31c), X(0x7fdc13d5),
X(0x7fdc6408), X(0x7fdcb3b6), X(0x7fdd02df), X(0x7fdd5184),
X(0x7fdd9fa5), X(0x7fdded44), X(0x7fde3a60), X(0x7fde86fb),
X(0x7fded314), X(0x7fdf1eac), X(0x7fdf69c4), X(0x7fdfb45d),
X(0x7fdffe76), X(0x7fe04811), X(0x7fe0912e), X(0x7fe0d9ce),
X(0x7fe121f0), X(0x7fe16996), X(0x7fe1b0c1), X(0x7fe1f770),
X(0x7fe23da4), X(0x7fe2835f), X(0x7fe2c89f), X(0x7fe30d67),
X(0x7fe351b5), X(0x7fe3958c), X(0x7fe3d8ec), X(0x7fe41bd4),
X(0x7fe45e46), X(0x7fe4a042), X(0x7fe4e1c8), X(0x7fe522da),
X(0x7fe56378), X(0x7fe5a3a1), X(0x7fe5e358), X(0x7fe6229b),
X(0x7fe6616d), X(0x7fe69fcc), X(0x7fe6ddbb), X(0x7fe71b39),
X(0x7fe75847), X(0x7fe794e5), X(0x7fe7d114), X(0x7fe80cd5),
X(0x7fe84827), X(0x7fe8830c), X(0x7fe8bd84), X(0x7fe8f78f),
X(0x7fe9312f), X(0x7fe96a62), X(0x7fe9a32b), X(0x7fe9db8a),
X(0x7fea137e), X(0x7fea4b09), X(0x7fea822b), X(0x7feab8e5),
X(0x7feaef37), X(0x7feb2521), X(0x7feb5aa4), X(0x7feb8fc1),
X(0x7febc478), X(0x7febf8ca), X(0x7fec2cb6), X(0x7fec603e),
X(0x7fec9363), X(0x7fecc623), X(0x7fecf881), X(0x7fed2a7c),
X(0x7fed5c16), X(0x7fed8d4e), X(0x7fedbe24), X(0x7fedee9b),
X(0x7fee1eb1), X(0x7fee4e68), X(0x7fee7dc0), X(0x7feeacb9),
X(0x7feedb54), X(0x7fef0991), X(0x7fef3771), X(0x7fef64f5),
X(0x7fef921d), X(0x7fefbee8), X(0x7fefeb59), X(0x7ff0176f),
X(0x7ff0432a), X(0x7ff06e8c), X(0x7ff09995), X(0x7ff0c444),
X(0x7ff0ee9c), X(0x7ff1189b), X(0x7ff14243), X(0x7ff16b94),
X(0x7ff1948e), X(0x7ff1bd32), X(0x7ff1e581), X(0x7ff20d7b),
X(0x7ff2351f), X(0x7ff25c70), X(0x7ff2836d), X(0x7ff2aa17),
X(0x7ff2d06d), X(0x7ff2f672), X(0x7ff31c24), X(0x7ff34185),
X(0x7ff36695), X(0x7ff38b55), X(0x7ff3afc4), X(0x7ff3d3e4),
X(0x7ff3f7b4), X(0x7ff41b35), X(0x7ff43e69), X(0x7ff4614e),
X(0x7ff483e6), X(0x7ff4a631), X(0x7ff4c82f), X(0x7ff4e9e1),
X(0x7ff50b47), X(0x7ff52c62), X(0x7ff54d33), X(0x7ff56db9),
X(0x7ff58df5), X(0x7ff5ade7), X(0x7ff5cd90), X(0x7ff5ecf1),
X(0x7ff60c09), X(0x7ff62ada), X(0x7ff64963), X(0x7ff667a5),
X(0x7ff685a1), X(0x7ff6a357), X(0x7ff6c0c7), X(0x7ff6ddf1),
X(0x7ff6fad7), X(0x7ff71778), X(0x7ff733d6), X(0x7ff74fef),
X(0x7ff76bc6), X(0x7ff78759), X(0x7ff7a2ab), X(0x7ff7bdba),
X(0x7ff7d888), X(0x7ff7f315), X(0x7ff80d61), X(0x7ff8276c),
X(0x7ff84138), X(0x7ff85ac4), X(0x7ff87412), X(0x7ff88d20),
X(0x7ff8a5f0), X(0x7ff8be82), X(0x7ff8d6d7), X(0x7ff8eeef),
X(0x7ff906c9), X(0x7ff91e68), X(0x7ff935cb), X(0x7ff94cf2),
X(0x7ff963dd), X(0x7ff97a8f), X(0x7ff99105), X(0x7ff9a742),
X(0x7ff9bd45), X(0x7ff9d30f), X(0x7ff9e8a0), X(0x7ff9fdf9),
X(0x7ffa131a), X(0x7ffa2803), X(0x7ffa3cb4), X(0x7ffa512f),
X(0x7ffa6573), X(0x7ffa7981), X(0x7ffa8d59), X(0x7ffaa0fc),
X(0x7ffab46a), X(0x7ffac7a3), X(0x7ffadaa8), X(0x7ffaed78),
X(0x7ffb0015), X(0x7ffb127f), X(0x7ffb24b6), X(0x7ffb36bb),
X(0x7ffb488d), X(0x7ffb5a2e), X(0x7ffb6b9d), X(0x7ffb7cdb),
X(0x7ffb8de9), X(0x7ffb9ec6), X(0x7ffbaf73), X(0x7ffbbff1),
X(0x7ffbd03f), X(0x7ffbe05e), X(0x7ffbf04f), X(0x7ffc0012),
X(0x7ffc0fa6), X(0x7ffc1f0d), X(0x7ffc2e47), X(0x7ffc3d54),
X(0x7ffc4c35), X(0x7ffc5ae9), X(0x7ffc6971), X(0x7ffc77ce),
X(0x7ffc8600), X(0x7ffc9407), X(0x7ffca1e4), X(0x7ffcaf96),
X(0x7ffcbd1f), X(0x7ffcca7e), X(0x7ffcd7b4), X(0x7ffce4c1),
X(0x7ffcf1a5), X(0x7ffcfe62), X(0x7ffd0af6), X(0x7ffd1763),
X(0x7ffd23a9), X(0x7ffd2fc8), X(0x7ffd3bc1), X(0x7ffd4793),
X(0x7ffd533f), X(0x7ffd5ec5), X(0x7ffd6a27), X(0x7ffd7563),
X(0x7ffd807a), X(0x7ffd8b6e), X(0x7ffd963d), X(0x7ffda0e8),
X(0x7ffdab70), X(0x7ffdb5d5), X(0x7ffdc017), X(0x7ffdca36),
X(0x7ffdd434), X(0x7ffdde0f), X(0x7ffde7c9), X(0x7ffdf161),
X(0x7ffdfad8), X(0x7ffe042f), X(0x7ffe0d65), X(0x7ffe167b),
X(0x7ffe1f71), X(0x7ffe2848), X(0x7ffe30ff), X(0x7ffe3997),
X(0x7ffe4211), X(0x7ffe4a6c), X(0x7ffe52a9), X(0x7ffe5ac8),
X(0x7ffe62c9), X(0x7ffe6aae), X(0x7ffe7275), X(0x7ffe7a1f),
X(0x7ffe81ad), X(0x7ffe891f), X(0x7ffe9075), X(0x7ffe97b0),
X(0x7ffe9ece), X(0x7ffea5d2), X(0x7ffeacbb), X(0x7ffeb38a),
X(0x7ffeba3e), X(0x7ffec0d8), X(0x7ffec758), X(0x7ffecdbf),
X(0x7ffed40d), X(0x7ffeda41), X(0x7ffee05d), X(0x7ffee660),
X(0x7ffeec4b), X(0x7ffef21f), X(0x7ffef7da), X(0x7ffefd7e),
X(0x7fff030b), X(0x7fff0881), X(0x7fff0de0), X(0x7fff1328),
X(0x7fff185b), X(0x7fff1d77), X(0x7fff227e), X(0x7fff276f),
X(0x7fff2c4b), X(0x7fff3112), X(0x7fff35c4), X(0x7fff3a62),
X(0x7fff3eeb), X(0x7fff4360), X(0x7fff47c2), X(0x7fff4c0f),
X(0x7fff504a), X(0x7fff5471), X(0x7fff5885), X(0x7fff5c87),
X(0x7fff6076), X(0x7fff6452), X(0x7fff681d), X(0x7fff6bd6),
X(0x7fff6f7d), X(0x7fff7313), X(0x7fff7698), X(0x7fff7a0c),
X(0x7fff7d6f), X(0x7fff80c2), X(0x7fff8404), X(0x7fff8736),
X(0x7fff8a58), X(0x7fff8d6b), X(0x7fff906e), X(0x7fff9362),
X(0x7fff9646), X(0x7fff991c), X(0x7fff9be3), X(0x7fff9e9c),
X(0x7fffa146), X(0x7fffa3e2), X(0x7fffa671), X(0x7fffa8f1),
X(0x7fffab65), X(0x7fffadca), X(0x7fffb023), X(0x7fffb26f),
X(0x7fffb4ae), X(0x7fffb6e0), X(0x7fffb906), X(0x7fffbb20),
X(0x7fffbd2e), X(0x7fffbf30), X(0x7fffc126), X(0x7fffc311),
X(0x7fffc4f1), X(0x7fffc6c5), X(0x7fffc88f), X(0x7fffca4d),
X(0x7fffcc01), X(0x7fffcdab), X(0x7fffcf4a), X(0x7fffd0e0),
X(0x7fffd26b), X(0x7fffd3ec), X(0x7fffd564), X(0x7fffd6d2),
X(0x7fffd838), X(0x7fffd993), X(0x7fffdae6), X(0x7fffdc31),
X(0x7fffdd72), X(0x7fffdeab), X(0x7fffdfdb), X(0x7fffe104),
X(0x7fffe224), X(0x7fffe33c), X(0x7fffe44d), X(0x7fffe556),
X(0x7fffe657), X(0x7fffe751), X(0x7fffe844), X(0x7fffe930),
X(0x7fffea15), X(0x7fffeaf3), X(0x7fffebca), X(0x7fffec9b),
X(0x7fffed66), X(0x7fffee2a), X(0x7fffeee8), X(0x7fffefa0),
X(0x7ffff053), X(0x7ffff0ff), X(0x7ffff1a6), X(0x7ffff247),
X(0x7ffff2e4), X(0x7ffff37a), X(0x7ffff40c), X(0x7ffff499),
X(0x7ffff520), X(0x7ffff5a3), X(0x7ffff621), X(0x7ffff69b),
X(0x7ffff710), X(0x7ffff781), X(0x7ffff7ee), X(0x7ffff857),
X(0x7ffff8bb), X(0x7ffff91c), X(0x7ffff979), X(0x7ffff9d2),
X(0x7ffffa27), X(0x7ffffa79), X(0x7ffffac8), X(0x7ffffb13),
X(0x7ffffb5b), X(0x7ffffba0), X(0x7ffffbe2), X(0x7ffffc21),
X(0x7ffffc5d), X(0x7ffffc96), X(0x7ffffccd), X(0x7ffffd01),
X(0x7ffffd32), X(0x7ffffd61), X(0x7ffffd8e), X(0x7ffffdb8),
X(0x7ffffde0), X(0x7ffffe07), X(0x7ffffe2b), X(0x7ffffe4d),
X(0x7ffffe6d), X(0x7ffffe8b), X(0x7ffffea8), X(0x7ffffec3),
X(0x7ffffedc), X(0x7ffffef4), X(0x7fffff0a), X(0x7fffff1f),
X(0x7fffff33), X(0x7fffff45), X(0x7fffff56), X(0x7fffff66),
X(0x7fffff75), X(0x7fffff82), X(0x7fffff8f), X(0x7fffff9a),
X(0x7fffffa5), X(0x7fffffaf), X(0x7fffffb8), X(0x7fffffc0),
X(0x7fffffc8), X(0x7fffffce), X(0x7fffffd5), X(0x7fffffda),
X(0x7fffffdf), X(0x7fffffe4), X(0x7fffffe8), X(0x7fffffeb),
X(0x7fffffef), X(0x7ffffff1), X(0x7ffffff4), X(0x7ffffff6),
X(0x7ffffff8), X(0x7ffffff9), X(0x7ffffffb), X(0x7ffffffc),
X(0x7ffffffd), X(0x7ffffffd), X(0x7ffffffe), X(0x7fffffff),
X(0x7fffffff), X(0x7fffffff), X(0x7fffffff), X(0x7fffffff),
X(0x7fffffff), X(0x7fffffff), X(0x7fffffff), X(0x7fffffff),
X(0x7fffffff), X(0x7fffffff), X(0x7fffffff), X(0x7fffffff),
};



static int _ilog(unsigned int v){
	int ret=0;
	while(v){
		++ret;
		v>>=1;
	}
	return(ret);
}

#define DATA_TYPE ogg_int32_t
#define REG_TYPE  register ogg_int32_t

#define cPI3_8 (0x30fbc54d)
#define cPI2_8 (0x5a82799a)
#define cPI1_8 (0x7641af3d)

static void mdct_forward(int n, DATA_TYPE *in, DATA_TYPE *out);
static void mdct_backward(int n, DATA_TYPE *in, DATA_TYPE *out);

#ifdef _ARM_ASSEM_

#if !defined(_V_WIDE_MATH)
#define _V_WIDE_MATH

static inline ogg_int32_t MULT32(ogg_int32_t x, ogg_int32_t y) {
	int lo,hi;
	asm volatile("smull\t%0, %1, %2, %3"
				 : "=&r"(lo),"=&r"(hi)
				 : "%r"(x),"r"(y)
				 : "cc");
	return(hi);
}

static inline ogg_int32_t MULT31(ogg_int32_t x, ogg_int32_t y) {
	return MULT32(x,y)<<1;
}

static inline ogg_int32_t MULT31_SHIFT15(ogg_int32_t x, ogg_int32_t y) {
	int lo,hi;
	asm volatile("smull	%0, %1, %2, %3\n\t"
				 "movs	%0, %0, lsr #15\n\t"
				 "adc	%1, %0, %1, lsl #17\n\t"
				 : "=&r"(lo),"=&r"(hi)
				 : "%r"(x),"r"(y)
				 : "cc");
	return(hi);
}

#define MB() asm volatile ("" : : : "memory")

static inline void XPROD32(ogg_int32_t  a, ogg_int32_t  b,
						   ogg_int32_t  t, ogg_int32_t  v,
						   ogg_int32_t *x, ogg_int32_t *y)
{
	int x1, y1, l;
	asm(	"smull	%0, %1, %4, %6\n\t"
		"smlal	%0, %1, %5, %7\n\t"
		"rsb	%3, %4, #0\n\t"
		"smull	%0, %2, %5, %6\n\t"
		"smlal	%0, %2, %3, %7"
		: "=&r" (l), "=&r" (x1), "=&r" (y1), "=r" (a)
		: "3" (a), "r" (b), "r" (t), "r" (v)
		: "cc" );
	*x = x1;
	MB();
	*y = y1;
}

static inline void XPROD31(ogg_int32_t  a, ogg_int32_t  b,
						   ogg_int32_t  t, ogg_int32_t  v,
						   ogg_int32_t *x, ogg_int32_t *y)
{
	int x1, y1, l;
	asm(	"smull	%0, %1, %4, %6\n\t"
		"smlal	%0, %1, %5, %7\n\t"
		"rsb	%3, %4, #0\n\t"
		"smull	%0, %2, %5, %6\n\t"
		"smlal	%0, %2, %3, %7"
		: "=&r" (l), "=&r" (x1), "=&r" (y1), "=r" (a)
		: "3" (a), "r" (b), "r" (t), "r" (v)
		: "cc" );
	*x = x1 << 1;
	MB();
	*y = y1 << 1;
}

static inline void XNPROD31(ogg_int32_t  a, ogg_int32_t  b,
							ogg_int32_t  t, ogg_int32_t  v,
							ogg_int32_t *x, ogg_int32_t *y)
{
	int x1, y1, l;
	asm(	"rsb	%2, %4, #0\n\t"
		"smull	%0, %1, %3, %5\n\t"
		"smlal	%0, %1, %2, %6\n\t"
		"smull	%0, %2, %4, %5\n\t"
		"smlal	%0, %2, %3, %6"
		: "=&r" (l), "=&r" (x1), "=&r" (y1)
		: "r" (a), "r" (b), "r" (t), "r" (v)
		: "cc" );
	*x = x1 << 1;
	MB();
	*y = y1 << 1;
}

#endif

#ifndef _V_CLIP_MATH
#define _V_CLIP_MATH

static inline ogg_int32_t CLIP_TO_15(ogg_int32_t x) {
	int tmp;
	asm volatile("subs	%1, %0, #32768\n\t"
				 "movpl	%0, #0x7f00\n\t"
				 "orrpl	%0, %0, #0xff\n"
				 "adds	%1, %0, #32768\n\t"
				 "movmi	%0, #0x8000"
				 : "+r"(x),"=r"(tmp)
				 :
				 : "cc");
	return(x);
}

#endif

#ifndef _V_LSP_MATH_ASM
#define _V_LSP_MATH_ASM

static inline void lsp_loop_asm(ogg_uint32_t *qip,ogg_uint32_t *pip,
								ogg_int32_t *qexpp,
								ogg_int32_t *ilsp,ogg_int32_t wi,
								ogg_int32_t m){
	
	ogg_uint32_t qi=*qip,pi=*pip;
	ogg_int32_t qexp=*qexpp;
	
	asm("mov     r0,%3;"
		"mov     r1,%5,asr#1;"
		"add     r0,r0,r1,lsl#3;"
		"1:"
		
		"ldmdb   r0!,{r1,r3};"
		"subs    r1,r1,%4;"          //ilsp[j]-wi
		"rsbmi   r1,r1,#0;"          //labs(ilsp[j]-wi)
		"umull   %0,r2,r1,%0;"       //qi*=labs(ilsp[j]-wi)
		
		"subs    r1,r3,%4;"          //ilsp[j+1]-wi
		"rsbmi   r1,r1,#0;"          //labs(ilsp[j+1]-wi)
		"umull   %1,r3,r1,%1;"       //pi*=labs(ilsp[j+1]-wi)
		
		"cmn     r2,r3;"             // shift down 16?
		"beq     0f;"
		"add     %2,%2,#16;"
		"mov     %0,%0,lsr #16;"
		"orr     %0,%0,r2,lsl #16;"
		"mov     %1,%1,lsr #16;"
		"orr     %1,%1,r3,lsl #16;"
		"0:"
		"cmp     r0,%3;\n"
		"bhi     1b;\n"
		
		// odd filter assymetry
		"ands    r0,%5,#1;\n"
		"beq     2f;\n"
		"add     r0,%3,%5,lsl#2;\n"
		
		"ldr     r1,[r0,#-4];\n"
		"mov     r0,#0x4000;\n"
		
		"subs    r1,r1,%4;\n"          //ilsp[j]-wi
		"rsbmi   r1,r1,#0;\n"          //labs(ilsp[j]-wi)
		"umull   %0,r2,r1,%0;\n"       //qi*=labs(ilsp[j]-wi)
		"umull   %1,r3,r0,%1;\n"       //pi*=labs(ilsp[j+1]-wi)
		
		"cmn     r2,r3;\n"             // shift down 16?
		"beq     2f;\n"
		"add     %2,%2,#16;\n"
		"mov     %0,%0,lsr #16;\n"
		"orr     %0,%0,r2,lsl #16;\n"
		"mov     %1,%1,lsr #16;\n"
		"orr     %1,%1,r3,lsl #16;\n"
		
		//qi=(pi>>shift)*labs(ilsp[j]-wi);
		//pi=(qi>>shift)*labs(ilsp[j+1]-wi);
		//qexp+=shift;
		
		//}
		
		/* normalize to max 16 sig figs */
		"2:"
		"mov     r2,#0;"
		"orr     r1,%0,%1;"
		"tst     r1,#0xff000000;"
		"addne   r2,r2,#8;"
		"movne   r1,r1,lsr #8;"
		"tst     r1,#0x00f00000;"
		"addne   r2,r2,#4;"
		"movne   r1,r1,lsr #4;"
		"tst     r1,#0x000c0000;"
		"addne   r2,r2,#2;"
		"movne   r1,r1,lsr #2;"
		"tst     r1,#0x00020000;"
		"addne   r2,r2,#1;"
		"movne   r1,r1,lsr #1;"
		"tst     r1,#0x00010000;"
		"addne   r2,r2,#1;"
		"mov     %0,%0,lsr r2;"
		"mov     %1,%1,lsr r2;"
		"add     %2,%2,r2;"
		
		: "+r"(qi),"+r"(pi),"+r"(qexp)
		: "r"(ilsp),"r"(wi),"r"(m)
		: "r0","r1","r2","r3","cc");
	
	*qip=qi;
	*pip=pi;
	*qexpp=qexp;
}

static inline void lsp_norm_asm(ogg_uint32_t *qip,ogg_int32_t *qexpp){
	
	ogg_uint32_t qi=*qip;
	ogg_int32_t qexp=*qexpp;
	
	asm("tst     %0,#0x0000ff00;"
		"moveq   %0,%0,lsl #8;"
		"subeq   %1,%1,#8;"
		"tst     %0,#0x0000f000;"
		"moveq   %0,%0,lsl #4;"
		"subeq   %1,%1,#4;"
		"tst     %0,#0x0000c000;"
		"moveq   %0,%0,lsl #2;"
		"subeq   %1,%1,#2;"
		"tst     %0,#0x00008000;"
		"moveq   %0,%0,lsl #1;"
		"subeq   %1,%1,#1;"
		: "+r"(qi),"+r"(qexp)
		:
		: "cc");
	*qip=qi;
	*qexpp=qexp;
}

#endif
#endif

#ifndef _V_WIDE_MATH
#define _V_WIDE_MATH

#if BYTE_ORDER==LITTLE_ENDIAN
union magic {
	struct {
		ogg_int32_t lo;
		ogg_int32_t hi;
	} halves;
	ogg_int64_t whole;
};
#endif 

#if BYTE_ORDER==BIG_ENDIAN
union magic {
	struct {
		ogg_int32_t hi;
		ogg_int32_t lo;
	} halves;
	ogg_int64_t whole;
};
#endif

STIN ogg_int32_t MULT32(ogg_int32_t x, ogg_int32_t y) {
	union magic magic;
	magic.whole = (ogg_int64_t)x * y;
	return magic.halves.hi;
}

STIN ogg_int32_t MULT31(ogg_int32_t x, ogg_int32_t y) {
	return MULT32(x,y)<<1;
}

STIN ogg_int32_t MULT31_SHIFT15(ogg_int32_t x, ogg_int32_t y) {
	union magic magic;
	magic.whole  = (ogg_int64_t)x * y;
	return ((ogg_uint32_t)(magic.halves.lo)>>15) | ((magic.halves.hi)<<17);
}

/*
 * This should be used as a memory barrier, forcing all cached values in
 * registers to wr writen back to memory.  Might or might not be beneficial
 * depending on the architecture and compiler.
 */
#define MB()

/*
 * The XPROD functions are meant to optimize the cross products found all
 * over the place in mdct.c by forcing memory operation ordering to avoid
 * unnecessary register reloads as soon as memory is being written to.
 */

STIN void XPROD32(ogg_int32_t  a, ogg_int32_t  b,
				  ogg_int32_t  t, ogg_int32_t  v,
				  ogg_int32_t *x, ogg_int32_t *y)
{
	*x = MULT32(a, t) + MULT32(b, v);
	*y = MULT32(b, t) - MULT32(a, v);
}

STIN void XPROD31(ogg_int32_t  a, ogg_int32_t  b,
				  ogg_int32_t  t, ogg_int32_t  v,
				  ogg_int32_t *x, ogg_int32_t *y)
{
	*x = MULT31(a, t) + MULT31(b, v);
	*y = MULT31(b, t) - MULT31(a, v);
}

STIN void XNPROD31(ogg_int32_t  a, ogg_int32_t  b,
				   ogg_int32_t  t, ogg_int32_t  v,
				   ogg_int32_t *x, ogg_int32_t *y)
{
	*x = MULT31(a, t) - MULT31(b, v);
	*y = MULT31(b, t) + MULT31(a, v);
}

#endif

#ifndef _V_CLIP_MATH
#define _V_CLIP_MATH

STIN ogg_int32_t CLIP_TO_15(ogg_int32_t x) {
	int ret=x;
	ret-= ((x<=32767)-1)&(x-32767);
	ret-= ((x>=-32768)-1)&(x+32768);
	return(ret);
}

#endif

STIN ogg_int32_t VFLOAT_MULT(ogg_int32_t a,ogg_int32_t ap,
							 ogg_int32_t b,ogg_int32_t bp,
							 ogg_int32_t *p){
	if(a && b){
		*p=ap+bp+32;
		return MULT32(a,b);
	}else
		return 0;
}

STIN ogg_int32_t VFLOAT_MULTI(ogg_int32_t a,ogg_int32_t ap,
							  ogg_int32_t i,
							  ogg_int32_t *p){
	
	int ip=_ilog(abs(i))-31;
	return VFLOAT_MULT(a,ap,i<<-ip,ip,p);
}

STIN ogg_int32_t VFLOAT_ADD(ogg_int32_t a,ogg_int32_t ap,
							ogg_int32_t b,ogg_int32_t bp,
							ogg_int32_t *p){
	
	if(!a){
		*p=bp;
		return b;
	}else if(!b){
		*p=ap;
		return a;
	}
	
	/* yes, this can leak a bit. */
	if(ap>bp){
		int shift=ap-bp+1;
		*p=ap+1;
		a>>=1;
		if(shift<32){
			b=(b+(1<<(shift-1)))>>shift;
		}else{
			b=0;
		}
	}else{
		int shift=bp-ap+1;
		*p=bp+1;
		b>>=1;
		if(shift<32){
			a=(a+(1<<(shift-1)))>>shift;
		}else{
			a=0;
		}
	}
	
	a+=b;
	if((a&0xc0000000)==0xc0000000 || 
	   (a&0xc0000000)==0){
		a<<=1;
		(*p)--;
	}
	return(a);
}






/* This structure encapsulates huffman and VQ style encoding books; it
 doesn't do anything specific to either.
 
 valuelist/quantlist are nonNULL (and q_* significant) only if
 there's entry->value mapping to be done.
 
 If encode-side mapping must be done (and thus the entry needs to be
 hunted), the auxiliary encode pointer will point to a decision
 tree.  This is true of both VQ and huffman, but is mostly useful
 with VQ.
 
 */

typedef struct static_codebook{
	long   dim;            /* codebook dimensions (elements per vector) */
	long   entries;        /* codebook entries */
	long  *lengthlist;     /* codeword lengths in bits */
	
	/* mapping ***************************************************************/
	int    maptype;        /* 0=none
	 1=implicitly populated values from map column 
	 2=listed arbitrary values */
	
	/* The below does a linear, single monotonic sequence mapping. */
	long     q_min;       /* packed 32 bit float; quant value 0 maps to minval */
	long     q_delta;     /* packed 32 bit float; val 1 - val 0 == delta */
	int      q_quant;     /* bits: 0 < quant <= 16 */
	int      q_sequencep; /* bitflag */
	
	long     *quantlist;  /* map == 1: (int)(entries^(1/dim)) element column map
	 map == 2: list of dim*entries quantized entry vals
	 */
} static_codebook;

typedef struct codebook{
	long dim;           /* codebook dimensions (elements per vector) */
	long entries;       /* codebook entries */
	long used_entries;  /* populated codebook entries */
	
	/* the below are ordered by bitreversed codeword and only used
     entries are populated */
	int           binarypoint;
	ogg_int32_t  *valuelist;  /* list of dim*entries actual entry values */  
	ogg_uint32_t *codelist;   /* list of bitstream codewords for each entry */
	
	int          *dec_index;  
	char         *dec_codelengths;
	ogg_uint32_t *dec_firsttable;
	int           dec_firsttablen;
	int           dec_maxlength;
	
	long     q_min;       /* packed 32 bit float; quant value 0 maps to minval */
	long     q_delta;     /* packed 32 bit float; val 1 - val 0 == delta */
	
} codebook;

static void vorbis_staticbook_clear(static_codebook *b);
static void vorbis_staticbook_destroy(static_codebook *b);
static int vorbis_book_init_decode(codebook *dest,const static_codebook *source);

static void vorbis_book_clear(codebook *b);
static long _book_maptype1_quantvals(const static_codebook *b);

static int vorbis_staticbook_unpack(oggpack_buffer *b,static_codebook *c);

static long vorbis_book_decode(codebook *book, oggpack_buffer *b);
static long vorbis_book_decodevs_add(codebook *book, ogg_int32_t *a, 
									 oggpack_buffer *b,int n,int point);
static long vorbis_book_decodev_set(codebook *book, ogg_int32_t *a, 
									oggpack_buffer *b,int n,int point);
static long vorbis_book_decodev_add(codebook *book, ogg_int32_t *a, 
									oggpack_buffer *b,int n,int point);
static long vorbis_book_decodevv_add(codebook *book, ogg_int32_t **a,
									 long off,int ch, 
									 oggpack_buffer *b,int n,int point);

typedef void vorbis_look_mapping;
typedef void vorbis_look_floor;
typedef void vorbis_look_residue;
typedef void vorbis_look_transform;

/* mode ************************************************************/
typedef struct {
	int blockflag;
	int windowtype;
	int transformtype;
	int mapping;
} vorbis_info_mode;

typedef void vorbis_info_floor;
typedef void vorbis_info_residue;
typedef void vorbis_info_mapping;

typedef struct private_state {
	/* local lookup storage */
	const void             *window[2];
	
	/* backend lookups are tied to the mode, not the backend or naked mapping */
	int                     modebits;
	vorbis_look_mapping   **mode;
	
	ogg_int64_t sample_count;
	
} private_state;

/* codec_setup_info contains all the setup information specific to the
 specific compression/decompression mode in progress (eg,
 psychoacoustic settings, channel setup, options, codebook
 etc).  
 *********************************************************************/

typedef struct codec_setup_info {
	
	/* Vorbis supports only short and long blocks, but allows the
     encoder to choose the sizes */
	
	long blocksizes[2];
	
	/* modes are the primary means of supporting on-the-fly different
     blocksizes, different channel mappings (LR or M/A),
     different residue backends, etc.  Each mode consists of a
     blocksize flag and a mapping (along with the mapping setup */
	
	int        modes;
	int        maps;
	int        times;
	int        floors;
	int        residues;
	int        books;
	
	vorbis_info_mode       *mode_param[64];
	int                     map_type[64];
	vorbis_info_mapping    *map_param[64];
	int                     time_type[64];
	int                     floor_type[64];
	vorbis_info_floor      *floor_param[64];
	int                     residue_type[64];
	vorbis_info_residue    *residue_param[64];
	static_codebook        *book_param[256];
	codebook               *fullbooks;
	
	int    passlimit[32];     /* iteration limit per couple/quant pass */
	int    coupling_passes;
} codec_setup_info;


/* this would all be simpler/shorter with templates, but.... */
/* Transform backend generic *************************************/

/* only mdct right now.  Flesh it out more if we ever transcend mdct
 in the transform domain */

/* Floor backend generic *****************************************/
typedef struct{
	vorbis_info_floor     *(*unpack)(vorbis_info *,oggpack_buffer *);
	vorbis_look_floor     *(*look)  (vorbis_dsp_state *,vorbis_info_mode *,
									 vorbis_info_floor *);
	void (*free_info) (vorbis_info_floor *);
	void (*free_look) (vorbis_look_floor *);
	void *(*inverse1)  (struct vorbis_block *,vorbis_look_floor *);
	int   (*inverse2)  (struct vorbis_block *,vorbis_look_floor *,
						void *buffer,ogg_int32_t *);
} vorbis_func_floor;

typedef struct{
	int   order;
	long  rate;
	long  barkmap;
	
	int   ampbits;
	int   ampdB;
	
	int   numbooks; /* <= 16 */
	int   books[16];
	
} vorbis_info_floor0;

#define VIF_POSIT 63
#define VIF_CLASS 16
#define VIF_PARTS 31
typedef struct{
	int   partitions;                /* 0 to 31 */
	int   partitionclass[VIF_PARTS]; /* 0 to 15 */
	
	int   class_dim[VIF_CLASS];        /* 1 to 8 */
	int   class_subs[VIF_CLASS];       /* 0,1,2,3 (bits: 1<<n poss) */
	int   class_book[VIF_CLASS];       /* subs ^ dim entries */
	int   class_subbook[VIF_CLASS][8]; /* [VIF_CLASS][subs] */
	
	
	int   mult;                      /* 1 2 3 or 4 */ 
	int   postlist[VIF_POSIT+2];    /* first two implicit */ 
	
} vorbis_info_floor1;


/* Residue backend generic *****************************************/
typedef struct{
	vorbis_info_residue *(*unpack)(vorbis_info *,oggpack_buffer *);
	vorbis_look_residue *(*look)  (vorbis_dsp_state *,vorbis_info_mode *,
								   vorbis_info_residue *);
	void (*free_info)    (vorbis_info_residue *);
	void (*free_look)    (vorbis_look_residue *);
	int  (*inverse)      (struct vorbis_block *,vorbis_look_residue *,
						  ogg_int32_t **,int *,int);
} vorbis_func_residue;

typedef struct vorbis_info_residue0{
	/* block-partitioned VQ coded straight residue */
	long  begin;
	long  end;
	
	/* first stage (lossless partitioning) */
	int    grouping;         /* group n vectors per partition */
	int    partitions;       /* possible codebooks for a partition */
	int    groupbook;        /* huffbook for partitioning */
	int    secondstages[64]; /* expanded out to pointers in lookup */
	int    booklist[256];    /* list of second stage books */
} vorbis_info_residue0;

/* Mapping backend generic *****************************************/
typedef struct{
	vorbis_info_mapping *(*unpack)(vorbis_info *,oggpack_buffer *);
	vorbis_look_mapping *(*look)  (vorbis_dsp_state *,vorbis_info_mode *,
								   vorbis_info_mapping *);
	void (*free_info)    (vorbis_info_mapping *);
	void (*free_look)    (vorbis_look_mapping *);
	int  (*inverse)      (struct vorbis_block *vb,vorbis_look_mapping *);
} vorbis_func_mapping;

typedef struct vorbis_info_mapping0{
	int   submaps;  /* <= 16 */
	int   chmuxlist[256];   /* up to 256 channels in a Vorbis stream */
	
	int   floorsubmap[16];   /* [mux] submap to floors */
	int   residuesubmap[16]; /* [mux] submap to residue */
	
	int   psy[2]; /* by blocktype; impulse/padding for short,
	 transition/normal for long */
	
	int   coupling_steps;
	int   coupling_mag[256];
	int   coupling_ang[256];
} vorbis_info_mapping0;

static void _vorbis_block_ripcord(vorbis_block *vb);
static void *_vorbis_block_alloc(vorbis_block *vb,long bytes);

vorbis_func_floor     floor0_exportbundle;
vorbis_func_floor     floor1_exportbundle;
vorbis_func_residue   residue0_exportbundle;
vorbis_func_residue   residue1_exportbundle;
vorbis_func_residue   residue2_exportbundle;
vorbis_func_mapping   mapping0_exportbundle;

vorbis_func_floor     *_floor_P[]={
&floor0_exportbundle,
&floor1_exportbundle,
};

vorbis_func_residue   *_residue_P[]={
&residue0_exportbundle,
&residue1_exportbundle,
&residue2_exportbundle,
};

vorbis_func_mapping   *_mapping_P[]={
&mapping0_exportbundle,
};

//*****************************************************************
//*****************************************************************
//*****************************************************************
//*****************************************************************
//*****************************************************************
//*****************************************************************
//*****************************************************************
//*****************************************************************


static const unsigned long mask[]=
{0x00000000,0x00000001,0x00000003,0x00000007,0x0000000f,
0x0000001f,0x0000003f,0x0000007f,0x000000ff,0x000001ff,
0x000003ff,0x000007ff,0x00000fff,0x00001fff,0x00003fff,
0x00007fff,0x0000ffff,0x0001ffff,0x0003ffff,0x0007ffff,
0x000fffff,0x001fffff,0x003fffff,0x007fffff,0x00ffffff,
0x01ffffff,0x03ffffff,0x07ffffff,0x0fffffff,0x1fffffff,
0x3fffffff,0x7fffffff,0xffffffff };

/* mark read process as having run off the end */
static void _adv_halt(oggpack_buffer *b){
	b->headptr=b->head->buffer->data+b->head->begin+b->head->length;
	b->headend=-1;
	b->headbit=0;
}

/* spans forward, skipping as many bytes as headend is negative; if
 headend is zero, simply finds next byte.  If we're up to the end
 of the buffer, leaves headend at zero.  If we've read past the end,
 halt the decode process. */
static void _span(oggpack_buffer *b){
	while(b->headend<1){
		if(b->head->next){
			b->count+=b->head->length;
			b->head=b->head->next;
			b->headptr=b->head->buffer->data+b->head->begin-b->headend; 
			b->headend+=b->head->length;      
		}else{
			/* we've either met the end of decode, or gone past it. halt
			 only if we're past */
			if(b->headend<0 || b->headbit)
			/* read has fallen off the end */
				_adv_halt(b);
			
			break;
		}
	}
}

void oggpack_readinit(oggpack_buffer *b,ogg_reference *r){
	memset(b,0,sizeof(*b));
	
	b->tail=b->head=r;
	b->count=0;
	b->headptr=b->head->buffer->data+b->head->begin;
	b->headend=b->head->length;
	_span(b);
}

#define _lookspan()   while(!end){\
head=head->next;\
if(!head) return -1;\
ptr=head->buffer->data + head->begin;\
end=head->length;\
}

/* Read in bits without advancing the bitptr; bits <= 32 */
long oggpack_look(oggpack_buffer *b,int bits){
	unsigned long m=mask[bits];
	unsigned long ret=-1;
	
	bits+=b->headbit;
	
	if(bits >= b->headend<<3){
		int            end=b->headend;
		unsigned char *ptr=b->headptr;
		ogg_reference *head=b->head;
		
		if(end<0)return -1;
		
		if(bits){
			_lookspan();
			ret=*ptr++>>b->headbit;
			if(bits>8){
				--end;
				_lookspan();
				ret|=*ptr++<<(8-b->headbit);  
				if(bits>16){
					--end;
					_lookspan();
					ret|=*ptr++<<(16-b->headbit);  
					if(bits>24){
						--end;
						_lookspan();
						ret|=*ptr++<<(24-b->headbit);  
						if(bits>32 && b->headbit){
							--end;
							_lookspan();
							ret|=*ptr<<(32-b->headbit);
						}
					}
				}
			}
		}
		
	}else{
		
		/* make this a switch jump-table */
		ret=b->headptr[0]>>b->headbit;
		if(bits>8){
			ret|=b->headptr[1]<<(8-b->headbit);  
			if(bits>16){
				ret|=b->headptr[2]<<(16-b->headbit);  
				if(bits>24){
					ret|=b->headptr[3]<<(24-b->headbit);  
					if(bits>32 && b->headbit)
						ret|=b->headptr[4]<<(32-b->headbit);
				}
			}
		}
	}
	
	ret&=m;
	return ret;
}

/* limited to 32 at a time */
void oggpack_adv(oggpack_buffer *b,int bits){
	bits+=b->headbit;
	b->headbit=bits&7;
	b->headptr+=bits/8;
	if((b->headend-=(bits>>3))<1)_span(b);
}

/* spans forward and finds next byte.  Never halts */
static void _span_one(oggpack_buffer *b){
	while(b->headend<1){
		if(b->head->next){
			b->count+=b->head->length;
			b->head=b->head->next;
			b->headptr=b->head->buffer->data+b->head->begin; 
			b->headend=b->head->length;      
		}else
			break;
	}
}

static int _halt_one(oggpack_buffer *b){
	if(b->headend<1){
		_adv_halt(b);
		return -1;
	}
	return 0;
}

int oggpack_eop(oggpack_buffer *b){
	if(b->headend<0)return -1;
	return 0;
}

/* bits <= 32 */
long oggpack_read(oggpack_buffer *b,int bits){
	unsigned long m=mask[bits];
	ogg_uint32_t ret=-1;
	
	bits+=b->headbit;
	
	if(bits >= b->headend<<3){
		
		if(b->headend<0)return -1;
		
		if(bits){
			if (_halt_one(b)) return -1;
			ret=*b->headptr>>b->headbit;
			
			if(bits>=8){
				++b->headptr;
				--b->headend;
				_span_one(b);
				if(bits>8){
					if (_halt_one(b)) return -1;
					ret|=*b->headptr<<(8-b->headbit);   
					
					if(bits>=16){
						++b->headptr;
						--b->headend;
						_span_one(b);
						if(bits>16){
							if (_halt_one(b)) return -1;
							ret|=*b->headptr<<(16-b->headbit);  
							
							if(bits>=24){
								++b->headptr;
								--b->headend;
								_span_one(b);
								if(bits>24){
									if (_halt_one(b)) return -1;
									ret|=*b->headptr<<(24-b->headbit);
									
									if(bits>=32){
										++b->headptr;
										--b->headend;
										_span_one(b);
										if(bits>32){
											if (_halt_one(b)) return -1;
											if(b->headbit)ret|=*b->headptr<<(32-b->headbit);
											
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}else{
		
		ret=b->headptr[0]>>b->headbit;
		if(bits>8){
			ret|=b->headptr[1]<<(8-b->headbit);  
			if(bits>16){
				ret|=b->headptr[2]<<(16-b->headbit);  
				if(bits>24){
					ret|=b->headptr[3]<<(24-b->headbit);  
					if(bits>32 && b->headbit){
						ret|=b->headptr[4]<<(32-b->headbit);
					}
				}
			}
		}
		
		b->headptr+=(bits>>3);
		b->headend-=(bits>>3);
	}
	
	ret&=m;
	b->headbit=bits&7;   
	return ret;
}

long oggpack_bytes(oggpack_buffer *b){
	return(b->count+b->headptr-b->head->buffer->data-b->head->begin+
		   ((b->headbit+7)>>3));
}

long oggpack_bits(oggpack_buffer *b){
	return((b->count+b->headptr-b->head->buffer->data-b->head->begin)*8+
		   b->headbit);
}

static int ilog1(unsigned int v){
	int ret=0;
	if(v)--v;
	while(v){
		ret++;
		v>>=1;
	}
	return(ret);
}

/* pcm accumulator examples (not exhaustive):
 
 <-------------- lW ---------------->
 <--------------- W ---------------->
 :            .....|.....       _______________         |
 :        .'''     |     '''_---      |       |\        |
 :.....'''         |_____--- '''......|       | \_______|
 :.................|__________________|_______|__|______|
 |<------ Sl ------>|      > Sr <     |endW
 |beginSl           |endSl  |  |endSr   
 |beginW            |endlW  |beginSr
 
 
 |< lW >|       
 <--------------- W ---------------->
 |   |  ..  ______________            |
 |   | '  `/        |     ---_        |
 |___.'___/`.       |         ---_____| 
 |_______|__|_______|_________________|
 |      >|Sl|<      |<------ Sr ----->|endW
 |       |  |endSl  |beginSr          |endSr
 |beginW |  |endlW                     
 mult[0] |beginSl                     mult[n]
 
 <-------------- lW ----------------->
 |<--W-->|                               
 :            ..............  ___  |   |                    
 :        .'''             |`/   \ |   |                       
 :.....'''                 |/`....\|...|                    
 :.........................|___|___|___|                  
 |Sl |Sr |endW    
 |   |   |endSr
 |   |beginSr
 |   |endSl
 |beginSl
 |beginW
 */

/* block abstraction setup *********************************************/

#ifndef WORD_ALIGN
#define WORD_ALIGN 8
#endif


static const void *_vorbis_window(int type, int left){
    switch(left){
		case 32:   return type?0:vwin64;
		case 64:   return type?0:vwin128;
		case 128:  return type?0:vwin256;
		case 256:  return type?0:vwin512;
		case 512:  return type?0:vwin1024;
		case 1024: return type?0:vwin2048;
		case 2048: return type?0:vwin4096;
		case 4096: return type?0:vwin8192;
		default:   return 0;
    }
}

int vorbis_block_init(vorbis_dsp_state *v, vorbis_block *vb){
	memset(vb,0,sizeof(*vb));
	vb->vd=v;
	vb->localalloc=0;
	vb->localstore=NULL;
	
	return(0);
}

static void *_vorbis_block_alloc(vorbis_block *vb,long bytes){
	bytes=(bytes+(WORD_ALIGN-1)) & ~(WORD_ALIGN-1);
	if(bytes+vb->localtop>vb->localalloc){
		/* can't just _ogg_realloc... there are outstanding pointers */
		if(vb->localstore){
			struct alloc_chain *link=(struct alloc_chain *)_ogg_malloc(sizeof(*link));
			vb->totaluse+=vb->localtop;
			link->next=vb->reap;
			link->ptr=vb->localstore;
			vb->reap=link;
		}
		/* highly conservative */
		vb->localalloc=bytes;
		vb->localstore=_ogg_malloc(vb->localalloc);
		vb->localtop=0;
	}
	{
		void *ret=(void *)(((char *)vb->localstore)+vb->localtop);
		vb->localtop+=bytes;
		return ret;
	}
}

/* reap the chain, pull the ripcord */
static void _vorbis_block_ripcord(vorbis_block *vb){
	/* reap the chain */
	struct alloc_chain *reap=vb->reap;
	while(reap){
		struct alloc_chain *next=reap->next;
		_ogg_free(reap->ptr);
		memset(reap,0,sizeof(*reap));
		_ogg_free(reap);
		reap=next;
	}
	/* consolidate storage */
	if(vb->totaluse){
		vb->localstore=_ogg_realloc(vb->localstore,vb->totaluse+vb->localalloc);
		vb->localalloc+=vb->totaluse;
		vb->totaluse=0;
	}
	
	/* pull the ripcord */
	vb->localtop=0;
	vb->reap=NULL;
}

int vorbis_block_clear(vorbis_block *vb){
	_vorbis_block_ripcord(vb);
	if(vb->localstore)_ogg_free(vb->localstore);
	
	memset(vb,0,sizeof(*vb));
	return(0);
}

static int _vds_init(vorbis_dsp_state *v,vorbis_info *vi){
	int i;
	codec_setup_info *ci=(codec_setup_info *)vi->codec_setup;
	private_state *b=NULL;
	
	memset(v,0,sizeof(*v));
	b=(private_state *)(v->backend_state=_ogg_calloc(1,sizeof(*b)));
	
	v->vi=vi;
	b->modebits=ilog1(ci->modes);
	
	/* Vorbis I uses only window type 0 */
	b->window[0]=_vorbis_window(0,ci->blocksizes[0]/2);
	b->window[1]=_vorbis_window(0,ci->blocksizes[1]/2);
	
	/* finish the codebooks */
	if(!ci->fullbooks){
		ci->fullbooks=(codebook *)_ogg_calloc(ci->books,sizeof(*ci->fullbooks));
		for(i=0;i<ci->books;i++){
			vorbis_book_init_decode(ci->fullbooks+i,ci->book_param[i]);
			/* decode codebooks are now standalone after init */
			vorbis_staticbook_destroy(ci->book_param[i]);
			ci->book_param[i]=NULL;
		}
	}
	
	v->pcm_storage=ci->blocksizes[1];
	v->pcm=(ogg_int32_t **)_ogg_malloc(vi->channels*sizeof(*v->pcm));
	v->pcmret=(ogg_int32_t **)_ogg_malloc(vi->channels*sizeof(*v->pcmret));
	for(i=0;i<vi->channels;i++)
		v->pcm[i]=(ogg_int32_t *)_ogg_calloc(v->pcm_storage,sizeof(*v->pcm[i]));
	
	/* all 1 (large block) or 0 (small block) */
	/* explicitly set for the sake of clarity */
	v->lW=0; /* previous window size */
	v->W=0;  /* current window size */
	
	/* initialize all the mapping/backend lookups */
	b->mode=(vorbis_look_mapping **)_ogg_calloc(ci->modes,sizeof(*b->mode));
	for(i=0;i<ci->modes;i++){
		int mapnum=ci->mode_param[i]->mapping;
		int maptype=ci->map_type[mapnum];
		b->mode[i]=_mapping_P[maptype]->look(v,ci->mode_param[i],
											 ci->map_param[mapnum]);
	}
	return(0);
}

int vorbis_synthesis_restart(vorbis_dsp_state *v){
	vorbis_info *vi=v->vi;
	codec_setup_info *ci;
	
	if(!v->backend_state)return -1;
	if(!vi)return -1;
	ci=vi->codec_setup;
	if(!ci)return -1;
	
	v->centerW=ci->blocksizes[1]/2;
	v->pcm_current=v->centerW;
	
	v->pcm_returned=-1;
	v->granulepos=-1;
	v->sequence=-1;
	((private_state *)(v->backend_state))->sample_count=-1;
	
	return(0);
}

int vorbis_synthesis_init(vorbis_dsp_state *v,vorbis_info *vi){
	_vds_init(v,vi);
	vorbis_synthesis_restart(v);
	
	return(0);
}

void vorbis_dsp_clear(vorbis_dsp_state *v){
	int i;
	if(v){
		vorbis_info *vi=v->vi;
		codec_setup_info *ci=(codec_setup_info *)(vi?vi->codec_setup:NULL);
		private_state *b=(private_state *)v->backend_state;
		
		if(v->pcm){
			for(i=0;i<vi->channels;i++)
				if(v->pcm[i])_ogg_free(v->pcm[i]);
			_ogg_free(v->pcm);
			if(v->pcmret)_ogg_free(v->pcmret);
		}
		
		/* free mode lookups; these are actually vorbis_look_mapping structs */
		if(ci){
			for(i=0;i<ci->modes;i++){
				int mapnum=ci->mode_param[i]->mapping;
				int maptype=ci->map_type[mapnum];
				if(b && b->mode)_mapping_P[maptype]->free_look(b->mode[i]);
			}
		}
		
		if(b){
			if(b->mode)_ogg_free(b->mode);    
			_ogg_free(b);
		}
		
		memset(v,0,sizeof(*v));
	}
}

/* Unlike in analysis, the window is only partially applied for each
 block.  The time domain envelope is not yet handled at the point of
 calling (as it relies on the previous block). */

int vorbis_synthesis_blockin(vorbis_dsp_state *v,vorbis_block *vb){
	vorbis_info *vi=v->vi;
	codec_setup_info *ci=(codec_setup_info *)vi->codec_setup;
	private_state *b=v->backend_state;
	int i,j;
	
	if(v->pcm_current>v->pcm_returned  && v->pcm_returned!=-1)return(OV_EINVAL);
	
	v->lW=v->W;
	v->W=vb->W;
	v->nW=-1;
	
	if((v->sequence==-1)||
	   (v->sequence+1 != vb->sequence)){
		v->granulepos=-1; /* out of sequence; lose count */
		b->sample_count=-1;
	}
	
	v->sequence=vb->sequence;
	
	if(vb->pcm){  /* no pcm to process if vorbis_synthesis_trackonly 
	 was called on block */
		int n=ci->blocksizes[v->W]/2;
		int n0=ci->blocksizes[0]/2;
		int n1=ci->blocksizes[1]/2;
		
		int thisCenter;
		int prevCenter;
		
		if(v->centerW){
			thisCenter=n1;
			prevCenter=0;
		}else{
			thisCenter=0;
			prevCenter=n1;
		}
		
		/* v->pcm is now used like a two-stage double buffer.  We don't want
		 to have to constantly shift *or* adjust memory usage.  Don't
		 accept a new block until the old is shifted out */
		
		/* overlap/add PCM */
		
		for(j=0;j<vi->channels;j++){
			/* the overlap/add section */
			if(v->lW){
				if(v->W){
					/* large/large */
					ogg_int32_t *pcm=v->pcm[j]+prevCenter;
					ogg_int32_t *p=vb->pcm[j];
					for(i=0;i<n1;i++)
						pcm[i]+=p[i];
				}else{
					/* large/small */
					ogg_int32_t *pcm=v->pcm[j]+prevCenter+n1/2-n0/2;
					ogg_int32_t *p=vb->pcm[j];
					for(i=0;i<n0;i++)
						pcm[i]+=p[i];
				}
			}else{
				if(v->W){
					/* small/large */
					ogg_int32_t *pcm=v->pcm[j]+prevCenter;
					ogg_int32_t *p=vb->pcm[j]+n1/2-n0/2;
					for(i=0;i<n0;i++)
						pcm[i]+=p[i];
					for(;i<n1/2+n0/2;i++)
						pcm[i]=p[i];
				}else{
					/* small/small */
					ogg_int32_t *pcm=v->pcm[j]+prevCenter;
					ogg_int32_t *p=vb->pcm[j];
					for(i=0;i<n0;i++)
						pcm[i]+=p[i];
				}
			}
			
			/* the copy section */
			{
				ogg_int32_t *pcm=v->pcm[j]+thisCenter;
				ogg_int32_t *p=vb->pcm[j]+n;
				for(i=0;i<n;i++)
					pcm[i]=p[i];
			}
		}
		
		if(v->centerW)
			v->centerW=0;
		else
			v->centerW=n1;
		
		/* deal with initial packet state; we do this using the explicit
		 pcm_returned==-1 flag otherwise we're sensitive to first block
		 being short or long */
		
		if(v->pcm_returned==-1){
			v->pcm_returned=thisCenter;
			v->pcm_current=thisCenter;
		}else{
			v->pcm_returned=prevCenter;
			v->pcm_current=prevCenter+
			ci->blocksizes[v->lW]/4+
			ci->blocksizes[v->W]/4;
		}
		
	}
    
	/* track the frame number... This is for convenience, but also
     making sure our last packet doesn't end with added padding.  If
     the last packet is partial, the number of samples we'll have to
     return will be past the vb->granulepos.
     
     This is not foolproof!  It will be confused if we begin
     decoding at the last page after a seek or hole.  In that case,
     we don't have a starting point to judge where the last frame
     is.  For this reason, vorbisfile will always try to make sure
     it reads the last two marked pages in proper sequence */
	
	if(b->sample_count==-1){
		b->sample_count=0;
	}else{
		b->sample_count+=ci->blocksizes[v->lW]/4+ci->blocksizes[v->W]/4;
	}
    
	if(v->granulepos==-1){
		if(vb->granulepos!=-1){ /* only set if we have a position to set to */
			
			v->granulepos=vb->granulepos;
			
			/* is this a short page? */
			if(b->sample_count>v->granulepos){
				/* corner case; if this is both the first and last audio page,
				 then spec says the end is cut, not beginning */
				if(vb->eofflag){
					/* trim the end */
					/* no preceeding granulepos; assume we started at zero (we'd
					 have to in a short single-page stream) */
					/* granulepos could be -1 due to a seek, but that would result
					 in a long coun`t, not short count */
					
					v->pcm_current-=(int)(b->sample_count-v->granulepos);
				}else{
					/* trim the beginning */
					v->pcm_returned+=(int)(b->sample_count-v->granulepos);
					if(v->pcm_returned>v->pcm_current)
						v->pcm_returned=v->pcm_current;
				}
				
			}
			
		}
	}else{
		v->granulepos+=ci->blocksizes[v->lW]/4+ci->blocksizes[v->W]/4;
		if(vb->granulepos!=-1 && v->granulepos!=vb->granulepos){
			
			if(v->granulepos>vb->granulepos){
				long extra=(int)(v->granulepos-vb->granulepos);
				
				if(extra)
					if(vb->eofflag){
						/* partial last frame.  Strip the extra samples off */
						v->pcm_current-=extra;
					} /* else {Shouldn't happen *unless* the bitstream is out of
					 spec.  Either way, believe the bitstream } */
			} /* else {Shouldn't happen *unless* the bitstream is out of
			 spec.  Either way, believe the bitstream } */
			v->granulepos=vb->granulepos;
		}
	}
	
	/* Update, cleanup */
	
	if(vb->eofflag)v->eofflag=1;
	return(0);
}

/* pcm==NULL indicates we just want the pending samples, no more */
int vorbis_synthesis_pcmout(vorbis_dsp_state *v,ogg_int32_t ***pcm){
	vorbis_info *vi=v->vi;
	if(v->pcm_returned>-1 && v->pcm_returned<v->pcm_current){
		if(pcm){
			int i;
			for(i=0;i<vi->channels;i++)
				v->pcmret[i]=v->pcm[i]+v->pcm_returned;
			*pcm=v->pcmret;
		}
		return(v->pcm_current-v->pcm_returned);
	}
	return(0);
}

int vorbis_synthesis_read(vorbis_dsp_state *v,int bytes){
	if(bytes && v->pcm_returned+bytes>v->pcm_current)return(OV_EINVAL);
	v->pcm_returned+=bytes;
	return(0);
}

/* unpacks a codebook from the packet buffer into the codebook struct,
 readies the codebook auxiliary structures for decode *************/
static int vorbis_staticbook_unpack(oggpack_buffer *opb,static_codebook *s){
	long i,j;
	memset(s,0,sizeof(*s));
	
	/* make sure alignment is correct */
	if(oggpack_read(opb,24)!=0x564342)goto _eofout;
	
	/* first the basic parameters */
	s->dim=oggpack_read(opb,16);
	s->entries=oggpack_read(opb,24);
	if(s->entries==-1)goto _eofout;
	
	/* codeword ordering.... length ordered or unordered? */
	switch((int)oggpack_read(opb,1)){
		case 0:
			/* unordered */
			s->lengthlist=(long *)_ogg_malloc(sizeof(*s->lengthlist)*s->entries);
			
			/* allocated but unused entries? */
			if(oggpack_read(opb,1)){
				/* yes, unused entries */
				
				for(i=0;i<s->entries;i++){
					if(oggpack_read(opb,1)){
						long num=oggpack_read(opb,5);
						if(num==-1)goto _eofout;
						s->lengthlist[i]=num+1;
					}else
						s->lengthlist[i]=0;
				}
			}else{
				/* all entries used; no tagging */
				for(i=0;i<s->entries;i++){
					long num=oggpack_read(opb,5);
					if(num==-1)goto _eofout;
					s->lengthlist[i]=num+1;
				}
			}
			
			break;
		case 1:
			/* ordered */
		{
			long length=oggpack_read(opb,5)+1;
			s->lengthlist=(long *)_ogg_malloc(sizeof(*s->lengthlist)*s->entries);
			
			for(i=0;i<s->entries;){
				long num=oggpack_read(opb,_ilog(s->entries-i));
				if(num==-1)goto _eofout;
				for(j=0;j<num && i<s->entries;j++,i++)
					s->lengthlist[i]=length;
				length++;
			}
		}
			break;
		default:
			/* EOF */
			return(-1);
	}
	
	/* Do we have a mapping to unpack? */
	switch((s->maptype=oggpack_read(opb,4))){
		case 0:
			/* no mapping */
			break;
		case 1: case 2:
			/* implicitly populated value mapping */
			/* explicitly populated value mapping */
			
			s->q_min=oggpack_read(opb,32);
			s->q_delta=oggpack_read(opb,32);
			s->q_quant=oggpack_read(opb,4)+1;
			s->q_sequencep=oggpack_read(opb,1);
			
		{
			int quantvals=0;
			switch(s->maptype){
				case 1:
					quantvals=_book_maptype1_quantvals(s);
					break;
				case 2:
					quantvals=s->entries*s->dim;
					break;
			}
			
			/* quantized values */
			s->quantlist=(long *)_ogg_malloc(sizeof(*s->quantlist)*quantvals);
			for(i=0;i<quantvals;i++)
				s->quantlist[i]=oggpack_read(opb,s->q_quant);
			
			if(quantvals&&s->quantlist[quantvals-1]==-1)goto _eofout;
		}
			break;
		default:
			goto _errout;
	}
	
	/* all set */
	return(0);
	
_errout:
_eofout:
	vorbis_staticbook_clear(s);
	return(-1); 
}

/* the 'eliminate the decode tree' optimization actually requires the
 codewords to be MSb first, not LSb.  This is an annoying inelegancy
 (and one of the first places where carefully thought out design
 turned out to be wrong; Vorbis II and future Ogg codecs should go
 to an MSb bitpacker), but not actually the huge hit it appears to
 be.  The first-stage decode table catches most words so that
 bitreverse is not in the main execution path. */

static ogg_uint32_t bitreverse(ogg_uint32_t x){
	x=    ((x>>16)&0x0000ffff) | ((x<<16)&0xffff0000);
	x=    ((x>> 8)&0x00ff00ff) | ((x<< 8)&0xff00ff00);
	x=    ((x>> 4)&0x0f0f0f0f) | ((x<< 4)&0xf0f0f0f0);
	x=    ((x>> 2)&0x33333333) | ((x<< 2)&0xcccccccc);
	return((x>> 1)&0x55555555) | ((x<< 1)&0xaaaaaaaa);
}

STIN long decode_packed_entry_number(codebook *book, 
									 oggpack_buffer *b){
	int  read=book->dec_maxlength;
	long lo,hi;
	long lok = oggpack_look(b,book->dec_firsttablen);
	
	if (lok >= 0) {
		long entry = book->dec_firsttable[lok];
		if(entry&0x80000000UL){
			lo=(entry>>15)&0x7fff;
			hi=book->used_entries-(entry&0x7fff);
		}else{
			oggpack_adv(b, book->dec_codelengths[entry-1]);
			return(entry-1);
		}
	}else{
		lo=0;
		hi=book->used_entries;
	}
	
	lok = oggpack_look(b, read);
	
	while(lok<0 && read>1)
		lok = oggpack_look(b, --read);
	
	if(lok<0){
		oggpack_adv(b,1); /* force eop */
		return -1;
	}
	
	/* bisect search for the codeword in the ordered list */
	{
		ogg_uint32_t testword=bitreverse((ogg_uint32_t)lok);
		
		while(hi-lo>1){
			long p=(hi-lo)>>1;
			long test=book->codelist[lo+p]>testword;    
			lo+=p&(test-1);
			hi-=p&(-test);
		}
		
		if(book->dec_codelengths[lo]<=read){
			oggpack_adv(b, book->dec_codelengths[lo]);
			return(lo);
		}
	}
	
	oggpack_adv(b, read+1);
	return(-1);
}

/* Decode side is specced and easier, because we don't need to find
 matches using different criteria; we simply read and map.  There are
 two things we need to do 'depending':
 
 We may need to support interleave.  We don't really, but it's
 convenient to do it here rather than rebuild the vector later.
 
 Cascades may be additive or multiplicitive; this is not inherent in
 the codebook, but set in the code using the codebook.  Like
 interleaving, it's easiest to do it here.  
 addmul==0 -> declarative (set the value)
 addmul==1 -> additive
 addmul==2 -> multiplicitive */

/* returns the [original, not compacted] entry number or -1 on eof *********/
static long vorbis_book_decode(codebook *book, oggpack_buffer *b){
	if(book->used_entries>0){
		long packed_entry=decode_packed_entry_number(book,b);
		if(packed_entry>=0)
			return(book->dec_index[packed_entry]);
	}
	
	/* if there's no dec_index, the codebook unpacking isn't collapsed */
	return(-1);
}

/* returns 0 on OK or -1 on eof *************************************/
static long vorbis_book_decodevs_add(codebook *book,ogg_int32_t *a,
									 oggpack_buffer *b,int n,int point){
	if(book->used_entries>0){  
		int step=n/book->dim;
		long *entry = (long *)alloca(sizeof(*entry)*step);
		ogg_int32_t **t = (ogg_int32_t **)alloca(sizeof(*t)*step);
		int i,j,o;
		int shift=point-book->binarypoint;
		
		if(shift>=0){
			for (i = 0; i < step; i++) {
				entry[i]=decode_packed_entry_number(book,b);
				if(entry[i]==-1)return(-1);
				t[i] = book->valuelist+entry[i]*book->dim;
			}
			for(i=0,o=0;i<book->dim;i++,o+=step)
				for (j=0;j<step;j++)
					a[o+j]+=t[j][i]>>shift;
		}else{
			for (i = 0; i < step; i++) {
				entry[i]=decode_packed_entry_number(book,b);
				if(entry[i]==-1)return(-1);
				t[i] = book->valuelist+entry[i]*book->dim;
			}
			for(i=0,o=0;i<book->dim;i++,o+=step)
				for (j=0;j<step;j++)
					a[o+j]+=t[j][i]<<-shift;
		}
	}
	return(0);
}

static long vorbis_book_decodev_add(codebook *book,ogg_int32_t *a,
									oggpack_buffer *b,int n,int point){
	if(book->used_entries>0){
		int i,j,entry;
		ogg_int32_t *t;
		int shift=point-book->binarypoint;
		
		if(shift>=0){
			for(i=0;i<n;){
				entry = decode_packed_entry_number(book,b);
				if(entry==-1)return(-1);
				t     = book->valuelist+entry*book->dim;
				for (j=0;j<book->dim;)
					a[i++]+=t[j++]>>shift;
			}
		}else{
			for(i=0;i<n;){
				entry = decode_packed_entry_number(book,b);
				if(entry==-1)return(-1);
				t     = book->valuelist+entry*book->dim;
				for (j=0;j<book->dim;)
					a[i++]+=t[j++]<<-shift;
			}
		}
	}
	return(0);
}

static long vorbis_book_decodev_set(codebook *book,ogg_int32_t *a,
									oggpack_buffer *b,int n,int point){
	if(book->used_entries>0){
		int i,j,entry;
		ogg_int32_t *t;
		int shift=point-book->binarypoint;
		
		if(shift>=0){
			
			for(i=0;i<n;){
				entry = decode_packed_entry_number(book,b);
				if(entry==-1)return(-1);
				t     = book->valuelist+entry*book->dim;
				for (j=0;j<book->dim;){
					a[i++]=t[j++]>>shift;
				}
			}
		}else{
			
			for(i=0;i<n;){
				entry = decode_packed_entry_number(book,b);
				if(entry==-1)return(-1);
				t     = book->valuelist+entry*book->dim;
				for (j=0;j<book->dim;){
					a[i++]=t[j++]<<-shift;
				}
			}
		}
	}else{
		
		int i,j;
		for(i=0;i<n;){
			for (j=0;j<book->dim;){
				a[i++]=0;
			}
		}
	}
	return(0);
}

static long vorbis_book_decodevv_add(codebook *book,ogg_int32_t **a,\
									 long offset,int ch,
									 oggpack_buffer *b,int n,int point){
	if(book->used_entries>0){
		long i,j,entry;
		int chptr=0;
		int shift=point-book->binarypoint;
		
		if(shift>=0){
			
			for(i=offset;i<offset+n;){
				entry = decode_packed_entry_number(book,b);
				if(entry==-1)return(-1);
				{
					const ogg_int32_t *t = book->valuelist+entry*book->dim;
					for (j=0;j<book->dim;j++){
						a[chptr++][i]+=t[j]>>shift;
						if(chptr==ch){
							chptr=0;
							i++;
						}
					}
				}
			}
		}else{
			
			for(i=offset;i<offset+n;){
				entry = decode_packed_entry_number(book,b);
				if(entry==-1)return(-1);
				{
					const ogg_int32_t *t = book->valuelist+entry*book->dim;
					for (j=0;j<book->dim;j++){
						a[chptr++][i]+=t[j]<<-shift;
						if(chptr==ch){
							chptr=0;
							i++;
						}
					}
				}
			}
		}
	}
	return(0);
}

#define LSP_FRACBITS 14

typedef struct {
	long n;
	int ln;
	int  m;
	int *linearmap;
	
	vorbis_info_floor0 *vi;
	ogg_int32_t *lsp_look;
	
} vorbis_look_floor0;

/*************** LSP decode ********************/

/* interpolated 1./sqrt(p) where .5 <= a < 1. (.100000... to .111111...) in
 16.16 format 
 returns in m.8 format */

static long ADJUST_SQRT2[2]={8192,5792};
STIN ogg_int32_t vorbis_invsqlook_i(long a,long e){
	long i=(a&0x7fff)>>(INVSQ_LOOKUP_I_SHIFT-1); 
	long d=a&INVSQ_LOOKUP_I_MASK;                              /*  0.10 */
	long val=INVSQ_LOOKUP_I[i]-                                /*  1.16 */
    ((INVSQ_LOOKUP_IDel[i]*d)>>INVSQ_LOOKUP_I_SHIFT);        /* result 1.16 */
	val*=ADJUST_SQRT2[e&1];
	e=(e>>1)+21;
	return(val>>e);
}

/* interpolated lookup based fromdB function, domain -140dB to 0dB only */
/* a is in n.12 format */
STIN ogg_int32_t vorbis_fromdBlook_i(long a){
	int i=(-a)>>(12-FROMdB2_SHIFT);
	if(i<0) return 0x7fffffff;
	if(i>=(FROMdB_LOOKUP_SZ<<FROMdB_SHIFT))return 0;
	
	return FROMdB_LOOKUP[i>>FROMdB_SHIFT] * FROMdB2_LOOKUP[i&FROMdB2_MASK];
}

/* interpolated lookup based cos function, domain 0 to PI only */
/* a is in 0.16 format, where 0==0, 2^^16-1==PI, return 0.14 */
STIN ogg_int32_t vorbis_coslook_i(long a){
	int i=a>>COS_LOOKUP_I_SHIFT;
	int d=a&COS_LOOKUP_I_MASK;
	return COS_LOOKUP_I[i]- ((d*(COS_LOOKUP_I[i]-COS_LOOKUP_I[i+1]))>>
							 COS_LOOKUP_I_SHIFT);
}

/* interpolated lookup based cos function */
/* a is in 0.16 format, where 0==0, 2^^16==PI, return .LSP_FRACBITS */
STIN ogg_int32_t vorbis_coslook2_i(long a){
	a=a&0x1ffff;
	
	if(a>0x10000)a=0x20000-a;
	{               
		int i=a>>COS_LOOKUP_I_SHIFT;
		int d=a&COS_LOOKUP_I_MASK;
		a=((COS_LOOKUP_I[i]<<COS_LOOKUP_I_SHIFT)-
		   d*(COS_LOOKUP_I[i]-COS_LOOKUP_I[i+1]))>>
		(COS_LOOKUP_I_SHIFT-LSP_FRACBITS+14);
	}
	
	return(a);
}

static const int barklook[28]={
0,100,200,301,          405,516,635,766,
912,1077,1263,1476,     1720,2003,2333,2721,
3184,3742,4428,5285,    6376,7791,9662,12181,
15624,20397,27087,36554
};

/* used in init only; interpolate the long way */
STIN ogg_int32_t toBARK(int n){
	int i;
	for(i=0;i<27;i++) 
		if(n>=barklook[i] && n<barklook[i+1])break;
	
	if(i==27){
		return 27<<15;
	}else{
		int gap=barklook[i+1]-barklook[i];
		int del=n-barklook[i];
		
		return((i<<15)+((del<<15)/gap));
	}
}

static const unsigned char MLOOP_1[64]={
0,10,11,11, 12,12,12,12, 13,13,13,13, 13,13,13,13,
14,14,14,14, 14,14,14,14, 14,14,14,14, 14,14,14,14,
15,15,15,15, 15,15,15,15, 15,15,15,15, 15,15,15,15,
15,15,15,15, 15,15,15,15, 15,15,15,15, 15,15,15,15,
};

static const unsigned char MLOOP_2[64]={
0,4,5,5, 6,6,6,6, 7,7,7,7, 7,7,7,7,
8,8,8,8, 8,8,8,8, 8,8,8,8, 8,8,8,8,
9,9,9,9, 9,9,9,9, 9,9,9,9, 9,9,9,9,
9,9,9,9, 9,9,9,9, 9,9,9,9, 9,9,9,9,
};

static const unsigned char MLOOP_3[8]={0,1,2,2,3,3,3,3};

static void vorbis_lsp_to_curve(ogg_int32_t *curve,int *map,int n,int ln,
								ogg_int32_t *lsp,int m,
								ogg_int32_t amp,
								ogg_int32_t ampoffset,
								ogg_int32_t *icos){
	
	/* 0 <= m < 256 */
	
	/* set up for using all int later */
	int i;
	int ampoffseti=ampoffset*4096;
	int ampi=amp;
	ogg_int32_t *ilsp=(ogg_int32_t *)alloca(m*sizeof(*ilsp));
	/* lsp is in 8.24, range 0 to PI; coslook wants it in .16 0 to 1*/
	for(i=0;i<m;i++){
		ogg_int32_t val=MULT32(lsp[i],0x517cc2);
		
		/* safeguard against a malicious stream */
		if(val<0 || (val>>COS_LOOKUP_I_SHIFT)>=COS_LOOKUP_I_SZ){
			memset(curve,0,sizeof(*curve)*n);
			return;
		}
		
		ilsp[i]=vorbis_coslook_i(val);
	}
	
	i=0;
	while(i<n){
		int k=map[i];
		ogg_uint32_t pi=46341; /* 2**-.5 in 0.16 */
		ogg_uint32_t qi=46341;
		ogg_int32_t qexp=0;
		ogg_int32_t wi=icos[k];
		
#ifdef _V_LSP_MATH_ASM
		lsp_loop_asm(&qi,&pi,&qexp,ilsp,wi,m);
		
		pi=((pi*pi)>>16);
		qi=((qi*qi)>>16);
		
		if(m&1){
			qexp= qexp*2-28*((m+1)>>1)+m;	     
			pi*=(1<<14)-((wi*wi)>>14);
			qi+=pi>>14;     
		}else{
			qexp= qexp*2-13*m;
			
			pi*=(1<<14)-wi;
			qi*=(1<<14)+wi;
			
			qi=(qi+pi)>>14;
		}
		
		if(qi&0xffff0000){ /* checks for 1.xxxxxxxxxxxxxxxx */
			qi>>=1; qexp++; 
		}else
			lsp_norm_asm(&qi,&qexp);
		
#else
		int j;
		ogg_int32_t shift;
		qi*=labs(ilsp[0]-wi);
		pi*=labs(ilsp[1]-wi);
		
		for(j=3;j<m;j+=2){
			if(!(shift=MLOOP_1[(pi|qi)>>25]))
				if(!(shift=MLOOP_2[(pi|qi)>>19]))
					shift=MLOOP_3[(pi|qi)>>16];
			qi=(qi>>shift)*labs(ilsp[j-1]-wi);
			pi=(pi>>shift)*labs(ilsp[j]-wi);
			qexp+=shift;
		}
		if(!(shift=MLOOP_1[(pi|qi)>>25]))
			if(!(shift=MLOOP_2[(pi|qi)>>19]))
				shift=MLOOP_3[(pi|qi)>>16];
		
		/* pi,qi normalized collectively, both tracked using qexp */
		
		if(m&1){
			/* odd order filter; slightly assymetric */
			/* the last coefficient */
			qi=(qi>>shift)*labs(ilsp[j-1]-wi);
			pi=(pi>>shift)<<14;
			qexp+=shift;
			
			if(!(shift=MLOOP_1[(pi|qi)>>25]))
				if(!(shift=MLOOP_2[(pi|qi)>>19]))
					shift=MLOOP_3[(pi|qi)>>16];
			
			pi>>=shift;
			qi>>=shift;
			qexp+=shift-14*((m+1)>>1);
			
			pi=((pi*pi)>>16);
			qi=((qi*qi)>>16);
			qexp=qexp*2+m;
			
			pi*=(1<<14)-((wi*wi)>>14);
			qi+=pi>>14;
			
		}else{
			/* even order filter; still symmetric */
			
			/* p*=p(1-w), q*=q(1+w), let normalization drift because it isn't
			 worth tracking step by step */
			
			pi>>=shift;
			qi>>=shift;
			qexp+=shift-7*m;
			
			pi=((pi*pi)>>16);
			qi=((qi*qi)>>16);
			qexp=qexp*2+m;
			
			pi*=(1<<14)-wi;
			qi*=(1<<14)+wi;
			qi=(qi+pi)>>14;
			
		}
		
		
		/* we've let the normalization drift because it wasn't important;
		 however, for the lookup, things must be normalized again.  We
		 need at most one right shift or a number of left shifts */
		
		if(qi&0xffff0000){ /* checks for 1.xxxxxxxxxxxxxxxx */
			qi>>=1; qexp++; 
		}else
			while(qi && !(qi&0x8000)){ /* checks for 0.0xxxxxxxxxxxxxxx or less*/
				qi<<=1; qexp--; 
			}
		
#endif
		
		amp=vorbis_fromdBlook_i(ampi*                     /*  n.4         */
								vorbis_invsqlook_i(qi,qexp)- 
								/*  m.8, m+n<=8 */
								ampoffseti);              /*  8.12[0]     */
		
		curve[i]= MULT31_SHIFT15(curve[i],amp);
		while(map[++i]==k) curve[i]= MULT31_SHIFT15(curve[i],amp);
	}
}

/*************** vorbis decode glue ************/

static void floor0_free_info(vorbis_info_floor *i){
	vorbis_info_floor0 *info=(vorbis_info_floor0 *)i;
	if(info){
		memset(info,0,sizeof(*info));
		_ogg_free(info);
	}
}

static void floor0_free_look(vorbis_look_floor *i){
	vorbis_look_floor0 *look=(vorbis_look_floor0 *)i;
	if(look){
		
		if(look->linearmap)_ogg_free(look->linearmap);
		if(look->lsp_look)_ogg_free(look->lsp_look);
		memset(look,0,sizeof(*look));
		_ogg_free(look);
	}
}

static vorbis_info_floor *floor0_unpack (vorbis_info *vi,oggpack_buffer *opb){
	codec_setup_info     *ci=(codec_setup_info *)vi->codec_setup;
	int j;
	
	vorbis_info_floor0 *info=(vorbis_info_floor0 *)_ogg_malloc(sizeof(*info));
	info->order=oggpack_read(opb,8);
	info->rate=oggpack_read(opb,16);
	info->barkmap=oggpack_read(opb,16);
	info->ampbits=oggpack_read(opb,6);
	info->ampdB=oggpack_read(opb,8);
	info->numbooks=oggpack_read(opb,4)+1;
	
	if(info->order<1)goto err_out;
	if(info->rate<1)goto err_out;
	if(info->barkmap<1)goto err_out;
	if(info->numbooks<1)goto err_out;
    
	for(j=0;j<info->numbooks;j++){
		info->books[j]=oggpack_read(opb,8);
		if(info->books[j]<0 || info->books[j]>=ci->books)goto err_out;
	}
	return(info);
	
err_out:
	floor0_free_info(info);
	return(NULL);
}

/* initialize Bark scale and normalization lookups.  We could do this
 with static tables, but Vorbis allows a number of possible
 combinations, so it's best to do it computationally.
 
 The below is authoritative in terms of defining scale mapping.
 Note that the scale depends on the sampling rate as well as the
 linear block and mapping sizes */

static vorbis_look_floor *floor0_look (vorbis_dsp_state *vd,vorbis_info_mode *mi,
									   vorbis_info_floor *i){
	int j;
	vorbis_info        *vi=vd->vi;
	codec_setup_info   *ci=(codec_setup_info *)vi->codec_setup;
	vorbis_info_floor0 *info=(vorbis_info_floor0 *)i;
	vorbis_look_floor0 *look=(vorbis_look_floor0 *)_ogg_calloc(1,sizeof(*look));
	look->m=info->order;
	look->n=ci->blocksizes[mi->blockflag]/2;
	look->ln=info->barkmap;
	look->vi=info;
	
	/* the mapping from a linear scale to a smaller bark scale is
     straightforward.  We do *not* make sure that the linear mapping
     does not skip bark-scale bins; the decoder simply skips them and
     the encoder may do what it wishes in filling them.  They're
     necessary in some mapping combinations to keep the scale spacing
     accurate */
	look->linearmap=(int *)_ogg_malloc((look->n+1)*sizeof(*look->linearmap));
	for(j=0;j<look->n;j++){
		
		int val=(look->ln*
				 ((toBARK(info->rate/2*j/look->n)<<11)/toBARK(info->rate/2)))>>11;
		
		if(val>=look->ln)val=look->ln-1; /* guard against the approximation */
		look->linearmap[j]=val;
	}
	look->linearmap[j]=-1;
	
	look->lsp_look=(ogg_int32_t *)_ogg_malloc(look->ln*sizeof(*look->lsp_look));
	for(j=0;j<look->ln;j++)
		look->lsp_look[j]=vorbis_coslook2_i(0x10000*j/look->ln);
	
	return look;
}

static void *floor0_inverse1(vorbis_block *vb,vorbis_look_floor *i){
	vorbis_look_floor0 *look=(vorbis_look_floor0 *)i;
	vorbis_info_floor0 *info=look->vi;
	int j,k;
	
	int ampraw=oggpack_read(&vb->opb,info->ampbits);
	if(ampraw>0){ /* also handles the -1 out of data case */
		long maxval=(1<<info->ampbits)-1;
		int amp=((ampraw*info->ampdB)<<4)/maxval;
		int booknum=oggpack_read(&vb->opb,_ilog(info->numbooks));
		
		if(booknum!=-1 && booknum<info->numbooks){ /* be paranoid */
			codec_setup_info  *ci=(codec_setup_info *)vb->vd->vi->codec_setup;
			codebook *b=ci->fullbooks+info->books[booknum];
			ogg_int32_t last=0;
			ogg_int32_t *lsp=(ogg_int32_t *)_vorbis_block_alloc(vb,sizeof(*lsp)*(look->m+1));
            
			for(j=0;j<look->m;j+=b->dim)
				if(vorbis_book_decodev_set(b,lsp+j,&vb->opb,b->dim,-24)==-1)goto eop;
			for(j=0;j<look->m;){
				for(k=0;k<b->dim;k++,j++)lsp[j]+=last;
				last=lsp[j-1];
			}
			
			lsp[look->m]=amp;
			return(lsp);
		}
	}
eop:
	return(NULL);
}

static int floor0_inverse2(vorbis_block *vb,vorbis_look_floor *i,
						   void *memo,ogg_int32_t *out){
	vorbis_look_floor0 *look=(vorbis_look_floor0 *)i;
	vorbis_info_floor0 *info=look->vi;
	
	if(memo){
		ogg_int32_t *lsp=(ogg_int32_t *)memo;
		ogg_int32_t amp=lsp[look->m];
		
		/* take the coefficients back to a spectral envelope curve */
		vorbis_lsp_to_curve(out,look->linearmap,look->n,look->ln,
							lsp,look->m,amp,info->ampdB,look->lsp_look);
		return(1);
	}
	memset(out,0,sizeof(*out)*look->n);
	return(0);
}

/* export hooks */
vorbis_func_floor floor0_exportbundle={
&floor0_unpack,&floor0_look,&floor0_free_info,
&floor0_free_look,&floor0_inverse1,&floor0_inverse2
};


#define floor1_rangedB 140 /* floor 1 fixed at -140dB to 0dB range */

typedef struct {
	int forward_index[VIF_POSIT+2];
	
	int hineighbor[VIF_POSIT];
	int loneighbor[VIF_POSIT];
	int posts;
	
	int n;
	int quant_q;
	vorbis_info_floor1 *vi;
	
} vorbis_look_floor1;

/***********************************************/

static void floor1_free_info(vorbis_info_floor *i){
	vorbis_info_floor1 *info=(vorbis_info_floor1 *)i;
	if(info){
		memset(info,0,sizeof(*info));
		_ogg_free(info);
	}
}

static void floor1_free_look(vorbis_look_floor *i){
	vorbis_look_floor1 *look=(vorbis_look_floor1 *)i;
	if(look){
		memset(look,0,sizeof(*look));
		_ogg_free(look);
	}
}

static int ilog2(unsigned int v){
	int ret=0;
	while(v){
		ret++;
		v>>=1;
	}
	return(ret);
}

static vorbis_info_floor *floor1_unpack (vorbis_info *vi,oggpack_buffer *opb){
	codec_setup_info     *ci=(codec_setup_info *)vi->codec_setup;
	int j,k,count=0,maxclass=-1,rangebits;
	
	vorbis_info_floor1 *info=(vorbis_info_floor1 *)_ogg_calloc(1,sizeof(*info));
	/* read partitions */
	info->partitions=oggpack_read(opb,5); /* only 0 to 31 legal */
	for(j=0;j<info->partitions;j++){
		info->partitionclass[j]=oggpack_read(opb,4); /* only 0 to 15 legal */
		if(maxclass<info->partitionclass[j])maxclass=info->partitionclass[j];
	}
	
	/* read partition classes */
	for(j=0;j<maxclass+1;j++){
		info->class_dim[j]=oggpack_read(opb,3)+1; /* 1 to 8 */
		info->class_subs[j]=oggpack_read(opb,2); /* 0,1,2,3 bits */
		if(info->class_subs[j]<0)
			goto err_out;
		if(info->class_subs[j])info->class_book[j]=oggpack_read(opb,8);
		if(info->class_book[j]<0 || info->class_book[j]>=ci->books)
			goto err_out;
		for(k=0;k<(1<<info->class_subs[j]);k++){
			info->class_subbook[j][k]=oggpack_read(opb,8)-1;
			if(info->class_subbook[j][k]<-1 || info->class_subbook[j][k]>=ci->books)
				goto err_out;
		}
	}
	
	/* read the post list */
	info->mult=oggpack_read(opb,2)+1;     /* only 1,2,3,4 legal now */ 
	rangebits=oggpack_read(opb,4);
	
	for(j=0,k=0;j<info->partitions;j++){
		count+=info->class_dim[info->partitionclass[j]]; 
		for(;k<count;k++){
			int t=info->postlist[k+2]=oggpack_read(opb,rangebits);
			if(t<0 || t>=(1<<rangebits))
				goto err_out;
		}
	}
	info->postlist[0]=0;
	info->postlist[1]=1<<rangebits;
	
	return(info);
	
err_out:
	floor1_free_info(info);
	return(NULL);
}

static int icomp(const void *a,const void *b){
	return(**(const int *const*)a-**(const int *const*)b);
}

static vorbis_look_floor *floor1_look(vorbis_dsp_state *vd,vorbis_info_mode *mi,
									  vorbis_info_floor *in){
	
	int *sortpointer[VIF_POSIT+2];
	vorbis_info_floor1 *info=(vorbis_info_floor1 *)in;
	vorbis_look_floor1 *look=(vorbis_look_floor1 *)_ogg_calloc(1,sizeof(*look));
	int i,j,n=0;
	
	look->vi=info;
	look->n=info->postlist[1];
	
	/* we drop each position value in-between already decoded values,
     and use linear interpolation to predict each new value past the
     edges.  The positions are read in the order of the position
     list... we precompute the bounding positions in the lookup.  Of
     course, the neighbors can change (if a position is declined), but
     this is an initial mapping */
	
	for(i=0;i<info->partitions;i++)n+=info->class_dim[info->partitionclass[i]];
	n+=2;
	look->posts=n;
	
	/* also store a sorted position index */
	for(i=0;i<n;i++)sortpointer[i]=info->postlist+i;
	qsort(sortpointer,n,sizeof(*sortpointer),icomp);
	
	/* points from sort order back to range number */
	for(i=0;i<n;i++)look->forward_index[i]=sortpointer[i]-info->postlist;
	
	/* quantize values to multiplier spec */
	switch(info->mult){
		case 1: /* 1024 -> 256 */
			look->quant_q=256;
			break;
		case 2: /* 1024 -> 128 */
			look->quant_q=128;
			break;
		case 3: /* 1024 -> 86 */
			look->quant_q=86;
			break;
		case 4: /* 1024 -> 64 */
			look->quant_q=64;
			break;
	}
	
	/* discover our neighbors for decode where we don't use fit flags
     (that would push the neighbors outward) */
	for(i=0;i<n-2;i++){
		int lo=0;
		int hi=1;
		int lx=0;
		int hx=look->n;
		int currentx=info->postlist[i+2];
		for(j=0;j<i+2;j++){
			int x=info->postlist[j];
			if(x>lx && x<currentx){
				lo=j;
				lx=x;
			}
			if(x<hx && x>currentx){
				hi=j;
				hx=x;
			}
		}
		look->loneighbor[i]=lo;
		look->hineighbor[i]=hi;
	}
	
	return(look);
}

static int render_point(int x0,int x1,int y0,int y1,int x){
	y0&=0x7fff; /* mask off flag */
	y1&=0x7fff;
    
	{
		int dy=y1-y0;
		int adx=x1-x0;
		int ady=abs(dy);
		int err=ady*(x-x0);
		
		int off=err/adx;
		if(dy<0)return(y0-off);
		return(y0+off);
	}
}

#define XdB(n) (n)

static const ogg_int32_t FLOOR_fromdB_LOOKUP[256]={
XdB(0x000000e5), XdB(0x000000f4), XdB(0x00000103), XdB(0x00000114),
XdB(0x00000126), XdB(0x00000139), XdB(0x0000014e), XdB(0x00000163),
XdB(0x0000017a), XdB(0x00000193), XdB(0x000001ad), XdB(0x000001c9),
XdB(0x000001e7), XdB(0x00000206), XdB(0x00000228), XdB(0x0000024c),
XdB(0x00000272), XdB(0x0000029b), XdB(0x000002c6), XdB(0x000002f4),
XdB(0x00000326), XdB(0x0000035a), XdB(0x00000392), XdB(0x000003cd),
XdB(0x0000040c), XdB(0x00000450), XdB(0x00000497), XdB(0x000004e4),
XdB(0x00000535), XdB(0x0000058c), XdB(0x000005e8), XdB(0x0000064a),
XdB(0x000006b3), XdB(0x00000722), XdB(0x00000799), XdB(0x00000818),
XdB(0x0000089e), XdB(0x0000092e), XdB(0x000009c6), XdB(0x00000a69),
XdB(0x00000b16), XdB(0x00000bcf), XdB(0x00000c93), XdB(0x00000d64),
XdB(0x00000e43), XdB(0x00000f30), XdB(0x0000102d), XdB(0x0000113a),
XdB(0x00001258), XdB(0x0000138a), XdB(0x000014cf), XdB(0x00001629),
XdB(0x0000179a), XdB(0x00001922), XdB(0x00001ac4), XdB(0x00001c82),
XdB(0x00001e5c), XdB(0x00002055), XdB(0x0000226f), XdB(0x000024ac),
XdB(0x0000270e), XdB(0x00002997), XdB(0x00002c4b), XdB(0x00002f2c),
XdB(0x0000323d), XdB(0x00003581), XdB(0x000038fb), XdB(0x00003caf),
XdB(0x000040a0), XdB(0x000044d3), XdB(0x0000494c), XdB(0x00004e10),
XdB(0x00005323), XdB(0x0000588a), XdB(0x00005e4b), XdB(0x0000646b),
XdB(0x00006af2), XdB(0x000071e5), XdB(0x0000794c), XdB(0x0000812e),
XdB(0x00008993), XdB(0x00009283), XdB(0x00009c09), XdB(0x0000a62d),
XdB(0x0000b0f9), XdB(0x0000bc79), XdB(0x0000c8b9), XdB(0x0000d5c4),
XdB(0x0000e3a9), XdB(0x0000f274), XdB(0x00010235), XdB(0x000112fd),
XdB(0x000124dc), XdB(0x000137e4), XdB(0x00014c29), XdB(0x000161bf),
XdB(0x000178bc), XdB(0x00019137), XdB(0x0001ab4a), XdB(0x0001c70e),
XdB(0x0001e4a1), XdB(0x0002041f), XdB(0x000225aa), XdB(0x00024962),
XdB(0x00026f6d), XdB(0x000297f0), XdB(0x0002c316), XdB(0x0002f109),
XdB(0x000321f9), XdB(0x00035616), XdB(0x00038d97), XdB(0x0003c8b4),
XdB(0x000407a7), XdB(0x00044ab2), XdB(0x00049218), XdB(0x0004de23),
XdB(0x00052f1e), XdB(0x0005855c), XdB(0x0005e135), XdB(0x00064306),
XdB(0x0006ab33), XdB(0x00071a24), XdB(0x0007904b), XdB(0x00080e20),
XdB(0x00089422), XdB(0x000922da), XdB(0x0009bad8), XdB(0x000a5cb6),
XdB(0x000b091a), XdB(0x000bc0b1), XdB(0x000c8436), XdB(0x000d5471),
XdB(0x000e3233), XdB(0x000f1e5f), XdB(0x001019e4), XdB(0x001125c1),
XdB(0x00124306), XdB(0x001372d5), XdB(0x0014b663), XdB(0x00160ef7),
XdB(0x00177df0), XdB(0x001904c1), XdB(0x001aa4f9), XdB(0x001c603d),
XdB(0x001e384f), XdB(0x00202f0f), XdB(0x0022467a), XdB(0x002480b1),
XdB(0x0026dff7), XdB(0x002966b3), XdB(0x002c1776), XdB(0x002ef4fc),
XdB(0x0032022d), XdB(0x00354222), XdB(0x0038b828), XdB(0x003c67c2),
XdB(0x004054ae), XdB(0x004482e8), XdB(0x0048f6af), XdB(0x004db488),
XdB(0x0052c142), XdB(0x005821ff), XdB(0x005ddc33), XdB(0x0063f5b0),
XdB(0x006a74a7), XdB(0x00715faf), XdB(0x0078bdce), XdB(0x0080967f),
XdB(0x0088f1ba), XdB(0x0091d7f9), XdB(0x009b5247), XdB(0x00a56a41),
XdB(0x00b02a27), XdB(0x00bb9ce2), XdB(0x00c7ce12), XdB(0x00d4ca17),
XdB(0x00e29e20), XdB(0x00f15835), XdB(0x0101074b), XdB(0x0111bb4e),
XdB(0x01238531), XdB(0x01367704), XdB(0x014aa402), XdB(0x016020a7),
XdB(0x017702c3), XdB(0x018f6190), XdB(0x01a955cb), XdB(0x01c4f9cf),
XdB(0x01e269a8), XdB(0x0201c33b), XdB(0x0223265a), XdB(0x0246b4ea),
XdB(0x026c9302), XdB(0x0294e716), XdB(0x02bfda13), XdB(0x02ed9793),
XdB(0x031e4e09), XdB(0x03522ee4), XdB(0x03896ed0), XdB(0x03c445e2),
XdB(0x0402efd6), XdB(0x0445ac4b), XdB(0x048cbefc), XdB(0x04d87013),
XdB(0x05290c67), XdB(0x057ee5ca), XdB(0x05da5364), XdB(0x063bb204),
XdB(0x06a36485), XdB(0x0711d42b), XdB(0x0787710e), XdB(0x0804b299),
XdB(0x088a17ef), XdB(0x0918287e), XdB(0x09af747c), XdB(0x0a50957e),
XdB(0x0afc2f19), XdB(0x0bb2ef7f), XdB(0x0c759034), XdB(0x0d44d6ca),
XdB(0x0e2195bc), XdB(0x0f0cad0d), XdB(0x10070b62), XdB(0x1111aeea),
XdB(0x122da66c), XdB(0x135c120f), XdB(0x149e24d9), XdB(0x15f525b1),
XdB(0x176270e3), XdB(0x18e7794b), XdB(0x1a85c9ae), XdB(0x1c3f06d1),
XdB(0x1e14f07d), XdB(0x200963d7), XdB(0x221e5ccd), XdB(0x2455f870),
XdB(0x26b2770b), XdB(0x29363e2b), XdB(0x2be3db5c), XdB(0x2ebe06b6),
XdB(0x31c7a55b), XdB(0x3503ccd4), XdB(0x3875c5aa), XdB(0x3c210f44),
XdB(0x4009632b), XdB(0x4432b8cf), XdB(0x48a149bc), XdB(0x4d59959e),
XdB(0x52606733), XdB(0x57bad899), XdB(0x5d6e593a), XdB(0x6380b298),
XdB(0x69f80e9a), XdB(0x70dafda8), XdB(0x78307d76), XdB(0x7fffffff),
};

static void render_line(int n, int x0,int x1,int y0,int y1,ogg_int32_t *d){
	int dy=y1-y0;
	int adx=x1-x0;
	int ady=abs(dy);
	int base=dy/adx;
	int sy=(dy<0?base-1:base+1);
	int x=x0;
	int y=y0;
	int err=0;
	
	if(n>x1)n=x1;
	ady-=abs(base*adx);
	
	if(x<n)
		d[x]= MULT31_SHIFT15(d[x],FLOOR_fromdB_LOOKUP[y]);
	
	while(++x<n){
		err=err+ady;
		if(err>=adx){
			err-=adx;
			y+=sy;
		}else{
			y+=base;
		}
		d[x]= MULT31_SHIFT15(d[x],FLOOR_fromdB_LOOKUP[y]);
	}
}

static void *floor1_inverse1(vorbis_block *vb,vorbis_look_floor *in){
	vorbis_look_floor1 *look=(vorbis_look_floor1 *)in;
	vorbis_info_floor1 *info=look->vi;
	codec_setup_info   *ci=(codec_setup_info *)vb->vd->vi->codec_setup;
	
	int i,j,k;
	codebook *books=ci->fullbooks;   
	
	/* unpack wrapped/predicted values from stream */
	if(oggpack_read(&vb->opb,1)==1){
		int *fit_value=(int *)_vorbis_block_alloc(vb,(look->posts)*sizeof(*fit_value));
		
		fit_value[0]=oggpack_read(&vb->opb,ilog2(look->quant_q-1));
		fit_value[1]=oggpack_read(&vb->opb,ilog2(look->quant_q-1));
		
		/* partition by partition */
		/* partition by partition */
		for(i=0,j=2;i<info->partitions;i++){
			int classv=info->partitionclass[i];
			int cdim=info->class_dim[classv];
			int csubbits=info->class_subs[classv];
			int csub=1<<csubbits;
			int cval=0;
			
			/* decode the partition's first stage cascade value */
			if(csubbits){
				cval=vorbis_book_decode(books+info->class_book[classv],&vb->opb);
				
				if(cval==-1)goto eop;
			}
			
			for(k=0;k<cdim;k++){
				int book=info->class_subbook[classv][cval&(csub-1)];
				cval>>=csubbits;
				if(book>=0){
					if((fit_value[j+k]=vorbis_book_decode(books+book,&vb->opb))==-1)
						goto eop;
				}else{
					fit_value[j+k]=0;
				}
			}
			j+=cdim;
		}
		
		/* unwrap positive values and reconsitute via linear interpolation */
		for(i=2;i<look->posts;i++){
			int predicted=render_point(info->postlist[look->loneighbor[i-2]],
									   info->postlist[look->hineighbor[i-2]],
									   fit_value[look->loneighbor[i-2]],
									   fit_value[look->hineighbor[i-2]],
									   info->postlist[i]);
			int hiroom=look->quant_q-predicted;
			int loroom=predicted;
			int room=(hiroom<loroom?hiroom:loroom)<<1;
			int val=fit_value[i];
			
			if(val){
				if(val>=room){
					if(hiroom>loroom){
						val = val-loroom;
					}else{
						val = -1-(val-hiroom);
					}
				}else{
					if(val&1){
						val= -((val+1)>>1);
					}else{
						val>>=1;
					}
				}
				
				fit_value[i]=val+predicted;
				fit_value[look->loneighbor[i-2]]&=0x7fff;
				fit_value[look->hineighbor[i-2]]&=0x7fff;
				
			}else{
				fit_value[i]=predicted|0x8000;
			}
			
		}
		
		return(fit_value);
	}
eop:
	return(NULL);
}

static int floor1_inverse2(vorbis_block *vb,vorbis_look_floor *in,void *memo,
						   ogg_int32_t *out){
	vorbis_look_floor1 *look=(vorbis_look_floor1 *)in;
	vorbis_info_floor1 *info=look->vi;
	
	codec_setup_info   *ci=(codec_setup_info *)vb->vd->vi->codec_setup;
	int                  n=ci->blocksizes[vb->W]/2;
	int j;
	
	if(memo){
		/* render the lines */
		int *fit_value=(int *)memo;
		int hx=0;
		int lx=0;
		int ly=fit_value[0]*info->mult;
		for(j=1;j<look->posts;j++){
			int current=look->forward_index[j];
			int hy=fit_value[current]&0x7fff;
			if(hy==fit_value[current]){
				
				hy*=info->mult;
				hx=info->postlist[current];
				
				render_line(n,lx,hx,ly,hy,out);
				
				lx=hx;
				ly=hy;
			}
		}
		for(j=hx;j<n;j++)out[j]*=ly; /* be certain */    
		return(1);
	}
	memset(out,0,sizeof(*out)*n);
	return(0);
}

/* export hooks */
vorbis_func_floor floor1_exportbundle={
&floor1_unpack,&floor1_look,&floor1_free_info,
&floor1_free_look,&floor1_inverse1,&floor1_inverse2
};

/* A complete description of Ogg framing exists in docs/framing.html */

/* basic, centralized Ogg memory management based on linked lists of
 references to refcounted memory buffers.  References and buffers
 are both recycled.  Buffers are passed around and consumed in
 reference form. */

static ogg_buffer_state *ogg_buffer_create(void){
	ogg_buffer_state *bs=_ogg_calloc(1,sizeof(*bs));
	return bs;
}

/* destruction is 'lazy'; there may be memory references outstanding,
 and yanking the buffer state out from underneath would be
 antisocial.  Dealloc what is currently unused and have
 _release_one watch for the stragglers to come in.  When they do,
 finish destruction. */

/* call the helper while holding lock */
static void _ogg_buffer_destroy(ogg_buffer_state *bs){
	ogg_buffer *bt;
	ogg_reference *rt;
	
	if(bs->shutdown){
		
		bt=bs->unused_buffers;
		rt=bs->unused_references;
		
		while(bt){
			ogg_buffer *b=bt;
			bt=b->ptr.next;
			if(b->data)_ogg_free(b->data);
			_ogg_free(b);
		}
		bs->unused_buffers=0;
		while(rt){
			ogg_reference *r=rt;
			rt=r->next;
			_ogg_free(r);
		}
		bs->unused_references=0;
		
		if(!bs->outstanding)
			_ogg_free(bs);
		
	}
}

static void ogg_buffer_destroy(ogg_buffer_state *bs){
	bs->shutdown=1;
	_ogg_buffer_destroy(bs);
}

static ogg_buffer *_fetch_buffer(ogg_buffer_state *bs,long bytes){
	ogg_buffer    *ob;
	bs->outstanding++;
	
	/* do we have an unused buffer sitting in the pool? */
	if(bs->unused_buffers){
		ob=bs->unused_buffers;
		bs->unused_buffers=ob->ptr.next;
		
		/* if the unused buffer is too small, grow it */
		if(ob->size<bytes){
			ob->data=_ogg_realloc(ob->data,bytes);
			ob->size=bytes;
		}
	}else{
		/* allocate a new buffer */
		ob=_ogg_malloc(sizeof(*ob));
		ob->data=_ogg_malloc(bytes<16?16:bytes);
		ob->size=bytes;
	}
	
	ob->refcount=1;
	ob->ptr.owner=bs;
	return ob;
}

static ogg_reference *_fetch_ref(ogg_buffer_state *bs){
	ogg_reference *or;
	bs->outstanding++;
	
	/* do we have an unused reference sitting in the pool? */
	if(bs->unused_references){
		or=bs->unused_references;
		bs->unused_references=or->next;
	}else{
		/* allocate a new reference */
		or=_ogg_malloc(sizeof(*or));
	}
	
	or->begin=0;
	or->length=0;
	or->next=0;
	return or;
}

/* fetch a reference pointing to a fresh, initially continguous buffer
 of at least [bytes] length */
static ogg_reference *ogg_buffer_alloc(ogg_buffer_state *bs,long bytes){
	ogg_buffer    *ob=_fetch_buffer(bs,bytes);
	ogg_reference *or=_fetch_ref(bs);
	or->buffer=ob;
	return or;
}

/* enlarge the data buffer in the current link */
static void ogg_buffer_realloc(ogg_reference *or,long bytes){
	ogg_buffer    *ob=or->buffer;
	
	/* if the unused buffer is too small, grow it */
	if(ob->size<bytes){
		ob->data=_ogg_realloc(ob->data,bytes);
		ob->size=bytes;
	}
}

static void _ogg_buffer_mark_one(ogg_reference *or){
	or->buffer->refcount++;
}

/* increase the refcount of the buffers to which the reference points */
static void ogg_buffer_mark(ogg_reference *or){
	while(or){
		_ogg_buffer_mark_one(or);
		or=or->next;
	}
}

/* duplicate a reference (pointing to the same actual buffer memory)
 and increment buffer refcount.  If the desired segment begins out
 of range, NULL is returned; if the desired segment is simply zero
 length, a zero length ref is returned.  Partial range overlap
 returns the overlap of the ranges */
static ogg_reference *ogg_buffer_sub(ogg_reference *or,long begin,long length){
	ogg_reference *ret=0,*head=0;
	
	/* walk past any preceeding fragments we don't want */
	while(or && begin>=or->length){
		begin-=or->length;
		or=or->next;
	}
	
	/* duplicate the reference chain; increment refcounts */
	while(or && length){
		ogg_reference *temp=_fetch_ref(or->buffer->ptr.owner);
		if(head)
			head->next=temp;
		else
			ret=temp;
		head=temp;
		head->buffer=or->buffer;    
		head->begin=or->begin+begin;
		head->length=length;
		if(head->length>or->length-begin)
			head->length=or->length-begin;
		
		begin=0;
		length-=head->length;
		or=or->next;
	}
	
	ogg_buffer_mark(ret);
	return ret;
}

static ogg_reference *ogg_buffer_dup(ogg_reference *or){
	ogg_reference *ret=0,*head=0;
	/* duplicate the reference chain; increment refcounts */
	while(or){
		ogg_reference *temp=_fetch_ref(or->buffer->ptr.owner);
		if(head)
			head->next=temp;
		else
			ret=temp;
		head=temp;
		head->buffer=or->buffer;    
		head->begin=or->begin;
		head->length=or->length;
		or=or->next;
	}
	
	ogg_buffer_mark(ret);
	return ret;
}

/* split a reference into two references; 'return' is a reference to
 the buffer preceeding pos and 'head'/'tail' are the buffer past the
 split.  If pos is at or past the end of the passed in segment,
 'head/tail' are NULL */
static ogg_reference *ogg_buffer_split(ogg_reference **tail,
									   ogg_reference **head,long pos){
	
	/* walk past any preceeding fragments to one of:
     a) the exact boundary that seps two fragments
     b) the fragment that needs split somewhere in the middle */
	ogg_reference *ret=*tail;
	ogg_reference *or=*tail;
	
	while(or && pos>or->length){
		pos-=or->length;
		or=or->next;
	}
	
	if(!or || pos==0){
		
		return 0;
		
	}else{
		
		if(pos>=or->length){
			/* exact split, or off the end? */
			if(or->next){
				
				/* a split */
				*tail=or->next;
				or->next=0;
				
			}else{
				
				/* off or at the end */
				*tail=*head=0;
				
			}
		}else{
			
			/* split within a fragment */
			long lengthA=pos;
			long beginB=or->begin+pos;
			long lengthB=or->length-pos;
			
			/* make a new reference to tail the second piece */
			*tail=_fetch_ref(or->buffer->ptr.owner);
			
			(*tail)->buffer=or->buffer;
			(*tail)->begin=beginB;
			(*tail)->length=lengthB;
			(*tail)->next=or->next;
			_ogg_buffer_mark_one(*tail);
			if(head && or==*head)*head=*tail;    
			
			/* update the first piece */
			or->next=0;
			or->length=lengthA;
			
		}
	}
	return ret;
}

static void ogg_buffer_release_one(ogg_reference *or){
	ogg_buffer *ob=or->buffer;
	ogg_buffer_state *bs=ob->ptr.owner;
	
	ob->refcount--;
	if(ob->refcount==0){
		bs->outstanding--; /* for the returned buffer */
		ob->ptr.next=bs->unused_buffers;
		bs->unused_buffers=ob;
	}
	
	bs->outstanding--; /* for the returned reference */
	or->next=bs->unused_references;
	bs->unused_references=or;
	
	_ogg_buffer_destroy(bs); /* lazy cleanup (if needed) */
	
}

/* release the references, decrease the refcounts of buffers to which
 they point, release any buffers with a refcount that drops to zero */
static void ogg_buffer_release(ogg_reference *or){
	while(or){
		ogg_reference *next=or->next;
		ogg_buffer_release_one(or);
		or=next;
	}
}

static ogg_reference *ogg_buffer_pretruncate(ogg_reference *or,long pos){
	/* release preceeding fragments we don't want */
	while(or && pos>=or->length){
		ogg_reference *next=or->next;
		pos-=or->length;
		ogg_buffer_release_one(or);
		or=next;
	}
	if (or) {
		or->begin+=pos;
		or->length-=pos;
	}
	return or;
}

static ogg_reference *ogg_buffer_walk(ogg_reference *or){
	if(!or)return NULL;
	while(or->next){
		or=or->next;
	}
	return(or);
}

/* *head is appended to the front end (head) of *tail; both continue to
 be valid pointers, with *tail at the tail and *head at the head */
static ogg_reference *ogg_buffer_cat(ogg_reference *tail, ogg_reference *head){
	if(!tail)return head;
	
	while(tail->next){
		tail=tail->next;
	}
	tail->next=head;
	return ogg_buffer_walk(head);
}

static void _positionB(oggbyte_buffer *b,int pos){
	if(pos<b->pos){
		/* start at beginning, scan forward */
		b->ref=b->baseref;
		b->pos=0;
		b->end=b->pos+b->ref->length;
		b->ptr=b->ref->buffer->data+b->ref->begin;
	}
}

static void _positionF(oggbyte_buffer *b,int pos){
	/* scan forward for position */
	while(pos>=b->end){
		/* just seek forward */
		b->pos+=b->ref->length;
		b->ref=b->ref->next;
		b->end=b->ref->length+b->pos;
		b->ptr=b->ref->buffer->data+b->ref->begin;
	}
}

static int oggbyte_init(oggbyte_buffer *b,ogg_reference *or){
	memset(b,0,sizeof(*b));
	if(or){
		b->ref=b->baseref=or;
		b->pos=0;
		b->end=b->ref->length;
		b->ptr=b->ref->buffer->data+b->ref->begin;  
		return 0;
	}else
		return -1;
}

static void oggbyte_set4(oggbyte_buffer *b,ogg_uint32_t val,int pos){
	int i;
	_positionB(b,pos);
	for(i=0;i<4;i++){
		_positionF(b,pos);
		b->ptr[pos-b->pos]=val;
		val>>=8;
		++pos;
	}
}

static unsigned char oggbyte_read1(oggbyte_buffer *b,int pos){
	_positionB(b,pos);
	_positionF(b,pos);
	return b->ptr[pos-b->pos];
}

static ogg_uint32_t oggbyte_read4(oggbyte_buffer *b,int pos){
	ogg_uint32_t ret;
	_positionB(b,pos);
	_positionF(b,pos);
	ret=b->ptr[pos-b->pos];
	_positionF(b,++pos);
	ret|=b->ptr[pos-b->pos]<<8;
	_positionF(b,++pos);
	ret|=b->ptr[pos-b->pos]<<16;
	_positionF(b,++pos);
	ret|=b->ptr[pos-b->pos]<<24;
	return ret;
}

static ogg_int64_t oggbyte_read8(oggbyte_buffer *b,int pos){
	ogg_int64_t ret;
	unsigned char t[7];
	int i;
	_positionB(b,pos);
	for(i=0;i<7;i++){
		_positionF(b,pos);
		t[i]=b->ptr[pos++ -b->pos];
	}
	
	_positionF(b,pos);
	ret=b->ptr[pos-b->pos];
	
	for(i=6;i>=0;--i)
		ret= ret<<8 | t[i];
	
	return ret;
}

/* Now we get to the actual framing code */

int ogg_page_version(ogg_page *og){
	oggbyte_buffer ob;
	oggbyte_init(&ob,og->header);
	return oggbyte_read1(&ob,4);
}

int ogg_page_continued(ogg_page *og){
	oggbyte_buffer ob;
	oggbyte_init(&ob,og->header);
	return oggbyte_read1(&ob,5)&0x01;
}

int ogg_page_bos(ogg_page *og){
	oggbyte_buffer ob;
	oggbyte_init(&ob,og->header);
	return oggbyte_read1(&ob,5)&0x02;
}

int ogg_page_eos(ogg_page *og){
	oggbyte_buffer ob;
	oggbyte_init(&ob,og->header);
	return oggbyte_read1(&ob,5)&0x04;
}

ogg_int64_t ogg_page_granulepos(ogg_page *og){
	oggbyte_buffer ob;
	oggbyte_init(&ob,og->header);
	return oggbyte_read8(&ob,6);
}

ogg_uint32_t ogg_page_serialno(ogg_page *og){
	oggbyte_buffer ob;
	oggbyte_init(&ob,og->header);
	return oggbyte_read4(&ob,14);
}

ogg_uint32_t ogg_page_pageno(ogg_page *og){
	oggbyte_buffer ob;
	oggbyte_init(&ob,og->header);
	return oggbyte_read4(&ob,18);
}

/* returns the number of packets that are completed on this page (if
 the leading packet is begun on a previous page, but ends on this
 page, it's counted */

/* NOTE:
 If a page consists of a packet begun on a previous page, and a new
 packet begun (but not completed) on this page, the return will be:
 ogg_page_packets(page)   ==1, 
 ogg_page_continued(page) !=0
 
 If a page happens to be a single packet that was begun on a
 previous page, and spans to the next page (in the case of a three or
 more page packet), the return will be: 
 ogg_page_packets(page)   ==0, 
 ogg_page_continued(page) !=0
 */

int ogg_page_packets(ogg_page *og){
	int i;
	int n;
	int count=0;
	oggbyte_buffer ob;
	oggbyte_init(&ob,og->header);
	
	n=oggbyte_read1(&ob,26);
	for(i=0;i<n;i++)
		if(oggbyte_read1(&ob,27+i)<255)count++;
	return(count);
}

/* Static CRC calculation table.  See older code in CVS for dead
 run-time initialization code. */

static const ogg_uint32_t crc_lookup[256]={
0x00000000,0x04c11db7,0x09823b6e,0x0d4326d9,
0x130476dc,0x17c56b6b,0x1a864db2,0x1e475005,
0x2608edb8,0x22c9f00f,0x2f8ad6d6,0x2b4bcb61,
0x350c9b64,0x31cd86d3,0x3c8ea00a,0x384fbdbd,
0x4c11db70,0x48d0c6c7,0x4593e01e,0x4152fda9,
0x5f15adac,0x5bd4b01b,0x569796c2,0x52568b75,
0x6a1936c8,0x6ed82b7f,0x639b0da6,0x675a1011,
0x791d4014,0x7ddc5da3,0x709f7b7a,0x745e66cd,
0x9823b6e0,0x9ce2ab57,0x91a18d8e,0x95609039,
0x8b27c03c,0x8fe6dd8b,0x82a5fb52,0x8664e6e5,
0xbe2b5b58,0xbaea46ef,0xb7a96036,0xb3687d81,
0xad2f2d84,0xa9ee3033,0xa4ad16ea,0xa06c0b5d,
0xd4326d90,0xd0f37027,0xddb056fe,0xd9714b49,
0xc7361b4c,0xc3f706fb,0xceb42022,0xca753d95,
0xf23a8028,0xf6fb9d9f,0xfbb8bb46,0xff79a6f1,
0xe13ef6f4,0xe5ffeb43,0xe8bccd9a,0xec7dd02d,
0x34867077,0x30476dc0,0x3d044b19,0x39c556ae,
0x278206ab,0x23431b1c,0x2e003dc5,0x2ac12072,
0x128e9dcf,0x164f8078,0x1b0ca6a1,0x1fcdbb16,
0x018aeb13,0x054bf6a4,0x0808d07d,0x0cc9cdca,
0x7897ab07,0x7c56b6b0,0x71159069,0x75d48dde,
0x6b93dddb,0x6f52c06c,0x6211e6b5,0x66d0fb02,
0x5e9f46bf,0x5a5e5b08,0x571d7dd1,0x53dc6066,
0x4d9b3063,0x495a2dd4,0x44190b0d,0x40d816ba,
0xaca5c697,0xa864db20,0xa527fdf9,0xa1e6e04e,
0xbfa1b04b,0xbb60adfc,0xb6238b25,0xb2e29692,
0x8aad2b2f,0x8e6c3698,0x832f1041,0x87ee0df6,
0x99a95df3,0x9d684044,0x902b669d,0x94ea7b2a,
0xe0b41de7,0xe4750050,0xe9362689,0xedf73b3e,
0xf3b06b3b,0xf771768c,0xfa325055,0xfef34de2,
0xc6bcf05f,0xc27dede8,0xcf3ecb31,0xcbffd686,
0xd5b88683,0xd1799b34,0xdc3abded,0xd8fba05a,
0x690ce0ee,0x6dcdfd59,0x608edb80,0x644fc637,
0x7a089632,0x7ec98b85,0x738aad5c,0x774bb0eb,
0x4f040d56,0x4bc510e1,0x46863638,0x42472b8f,
0x5c007b8a,0x58c1663d,0x558240e4,0x51435d53,
0x251d3b9e,0x21dc2629,0x2c9f00f0,0x285e1d47,
0x36194d42,0x32d850f5,0x3f9b762c,0x3b5a6b9b,
0x0315d626,0x07d4cb91,0x0a97ed48,0x0e56f0ff,
0x1011a0fa,0x14d0bd4d,0x19939b94,0x1d528623,
0xf12f560e,0xf5ee4bb9,0xf8ad6d60,0xfc6c70d7,
0xe22b20d2,0xe6ea3d65,0xeba91bbc,0xef68060b,
0xd727bbb6,0xd3e6a601,0xdea580d8,0xda649d6f,
0xc423cd6a,0xc0e2d0dd,0xcda1f604,0xc960ebb3,
0xbd3e8d7e,0xb9ff90c9,0xb4bcb610,0xb07daba7,
0xae3afba2,0xaafbe615,0xa7b8c0cc,0xa379dd7b,
0x9b3660c6,0x9ff77d71,0x92b45ba8,0x9675461f,
0x8832161a,0x8cf30bad,0x81b02d74,0x857130c3,
0x5d8a9099,0x594b8d2e,0x5408abf7,0x50c9b640,
0x4e8ee645,0x4a4ffbf2,0x470cdd2b,0x43cdc09c,
0x7b827d21,0x7f436096,0x7200464f,0x76c15bf8,
0x68860bfd,0x6c47164a,0x61043093,0x65c52d24,
0x119b4be9,0x155a565e,0x18197087,0x1cd86d30,
0x029f3d35,0x065e2082,0x0b1d065b,0x0fdc1bec,
0x3793a651,0x3352bbe6,0x3e119d3f,0x3ad08088,
0x2497d08d,0x2056cd3a,0x2d15ebe3,0x29d4f654,
0xc5a92679,0xc1683bce,0xcc2b1d17,0xc8ea00a0,
0xd6ad50a5,0xd26c4d12,0xdf2f6bcb,0xdbee767c,
0xe3a1cbc1,0xe760d676,0xea23f0af,0xeee2ed18,
0xf0a5bd1d,0xf464a0aa,0xf9278673,0xfde69bc4,
0x89b8fd09,0x8d79e0be,0x803ac667,0x84fbdbd0,
0x9abc8bd5,0x9e7d9662,0x933eb0bb,0x97ffad0c,
0xafb010b1,0xab710d06,0xa6322bdf,0xa2f33668,
0xbcb4666d,0xb8757bda,0xb5365d03,0xb1f740b4};

ogg_sync_state *ogg_sync_create(void){
	ogg_sync_state *oy=_ogg_calloc(1,sizeof(*oy));
	memset(oy,0,sizeof(*oy));
	oy->bufferpool=ogg_buffer_create();
	return oy;
}

int ogg_sync_destroy(ogg_sync_state *oy){
	if(oy){
		ogg_sync_reset(oy);
		ogg_buffer_destroy(oy->bufferpool);
		memset(oy,0,sizeof(*oy));
		_ogg_free(oy);
	}
	return OGG_SUCCESS;
}

unsigned char *ogg_sync_bufferin(ogg_sync_state *oy, long bytes){
	
	/* [allocate and] expose a buffer for data submission.
	 
     If there is no head fragment
	 allocate one and expose it
     else
	 if the current head fragment has sufficient unused space
	 expose it
	 else
	 if the current head fragment is unused
	 resize and expose it
	 else
	 allocate new fragment and expose it
	 */
	
	/* base case; fifo uninitialized */
	if(!oy->fifo_head){
		oy->fifo_head=oy->fifo_tail=ogg_buffer_alloc(oy->bufferpool,bytes);
		return oy->fifo_head->buffer->data;
	}
	
	/* space left in current fragment case */
	if(oy->fifo_head->buffer->size-
	   oy->fifo_head->length-
	   oy->fifo_head->begin >= bytes)
		return oy->fifo_head->buffer->data+
		oy->fifo_head->length+oy->fifo_head->begin;
	
	/* current fragment is unused, but too small */
	if(!oy->fifo_head->length){
		ogg_buffer_realloc(oy->fifo_head,bytes);
		return oy->fifo_head->buffer->data+oy->fifo_head->begin;
	}
	
	/* current fragment used/full; get new fragment */
	{
		ogg_reference *new=ogg_buffer_alloc(oy->bufferpool,bytes);
		oy->fifo_head->next=new;
		oy->fifo_head=new;
	}
	return oy->fifo_head->buffer->data;
}

int ogg_sync_wrote(ogg_sync_state *oy, long bytes){ 
	if(!oy->fifo_head)return OGG_EINVAL;
	if(oy->fifo_head->buffer->size-oy->fifo_head->length-oy->fifo_head->begin < 
	   bytes)return OGG_EINVAL;
	oy->fifo_head->length+=bytes;
	oy->fifo_fill+=bytes;
	return OGG_SUCCESS;
}

static ogg_uint32_t _checksum(ogg_reference *or, int bytes){
	ogg_uint32_t crc_reg=0;
	int j,post;
	
	while(or){
		unsigned char *data=or->buffer->data+or->begin;
		post=(bytes<or->length?bytes:or->length);
		for(j=0;j<post;++j)
			crc_reg=(crc_reg<<8)^crc_lookup[((crc_reg >> 24)&0xff)^data[j]];
		bytes-=j;
		or=or->next;
	}
	
	return crc_reg;
}


/* sync the stream.  This is meant to be useful for finding page
 boundaries.
 
 return values for this:
 -n) skipped n bytes
 0) page not ready; more data (no bytes skipped)
 n) page synced at current location; page length n bytes
 
 */

long ogg_sync_pageseek(ogg_sync_state *oy,ogg_page *og){
	oggbyte_buffer page;
	long           bytes,ret=0;
	
	ogg_page_release(og);
	
	bytes=oy->fifo_fill;
	oggbyte_init(&page,oy->fifo_tail);
	
	if(oy->headerbytes==0){
		if(bytes<27)goto sync_out; /* not enough for even a minimal header */
		
		/* verify capture pattern */
		if(oggbyte_read1(&page,0)!=(int)'O' ||
		   oggbyte_read1(&page,1)!=(int)'g' ||
		   oggbyte_read1(&page,2)!=(int)'g' ||
		   oggbyte_read1(&page,3)!=(int)'S'    ) goto sync_fail;
		
		oy->headerbytes=oggbyte_read1(&page,26)+27;
	}
	if(bytes<oy->headerbytes)goto sync_out; /* not enough for header +
	 seg table */
	if(oy->bodybytes==0){
		int i;
		/* count up body length in the segment table */
		for(i=0;i<oy->headerbytes-27;i++)
			oy->bodybytes+=oggbyte_read1(&page,27+i);
	}
	
	if(oy->bodybytes+oy->headerbytes>bytes)goto sync_out;
	
	/* we have what appears to be a complete page; last test: verify
     checksum */
	{
		ogg_uint32_t chksum=oggbyte_read4(&page,22);
		oggbyte_set4(&page,0,22);
		
		/* Compare checksums; memory continues to be common access */
		if(chksum!=_checksum(oy->fifo_tail,oy->bodybytes+oy->headerbytes)){
			
			/* D'oh.  Mismatch! Corrupt page (or miscapture and not a page
			 at all). replace the computed checksum with the one actually
			 read in; remember all the memory is common access */
			
			oggbyte_set4(&page,chksum,22);
			goto sync_fail;
		}
		oggbyte_set4(&page,chksum,22);
	}
	
	/* We have a page.  Set up page return. */
	if(og){
		/* set up page output */
		og->header=ogg_buffer_split(&oy->fifo_tail,&oy->fifo_head,oy->headerbytes);
		og->header_len=oy->headerbytes;
		og->body=ogg_buffer_split(&oy->fifo_tail,&oy->fifo_head,oy->bodybytes);
		og->body_len=oy->bodybytes;
	}else{
		/* simply advance */
		oy->fifo_tail=
		ogg_buffer_pretruncate(oy->fifo_tail,oy->headerbytes+oy->bodybytes);
		if(!oy->fifo_tail)oy->fifo_head=0;
	}
	
	ret=oy->headerbytes+oy->bodybytes;
	oy->unsynced=0;
	oy->headerbytes=0;
	oy->bodybytes=0;
	oy->fifo_fill-=ret;
	
	return ret;
	
sync_fail:
	
	oy->headerbytes=0;
	oy->bodybytes=0;
	oy->fifo_tail=ogg_buffer_pretruncate(oy->fifo_tail,1);
	ret--;
	
	/* search forward through fragments for possible capture */
	while(oy->fifo_tail){
		/* invariant: fifo_cursor points to a position in fifo_tail */
		unsigned char *now=oy->fifo_tail->buffer->data+oy->fifo_tail->begin;
		unsigned char *next=memchr(now, 'O', oy->fifo_tail->length);
		
		if(next){
			/* possible capture in this segment */
			long bytes=next-now;
			oy->fifo_tail=ogg_buffer_pretruncate(oy->fifo_tail,bytes);
			ret-=bytes;
			break;
		}else{
			/* no capture.  advance to next segment */
			long bytes=oy->fifo_tail->length;
			ret-=bytes;
			oy->fifo_tail=ogg_buffer_pretruncate(oy->fifo_tail,bytes);
		}
	}
	if(!oy->fifo_tail)oy->fifo_head=0;
	oy->fifo_fill+=ret;
	
sync_out:
	return ret;
}

/* sync the stream and get a page.  Keep trying until we find a page.
 Supress 'sync errors' after reporting the first.
 
 return values:
 OGG_HOLE) recapture (hole in data)
 0) need more data
 1) page returned
 
 Returns pointers into buffered data; invalidated by next call to
 _stream, _clear, _init, or _buffer */

int ogg_sync_pageout(ogg_sync_state *oy, ogg_page *og){
	
	/* all we need to do is verify a page at the head of the stream
     buffer.  If it doesn't verify, we look for the next potential
     frame */
	
	while(1){
		long ret=ogg_sync_pageseek(oy,og);
		if(ret>0){
			/* have a page */
			return 1;
		}
		if(ret==0){
			/* need more data */
			return 0;
		}
		
		/* head did not start a synced page... skipped some bytes */
		if(!oy->unsynced){
			oy->unsynced=1;
			return OGG_HOLE;
		}
		
		/* loop. keep looking */
		
	}
}

/* clear things to an initial state.  Good to call, eg, before seeking */
int ogg_sync_reset(ogg_sync_state *oy){
	
	ogg_buffer_release(oy->fifo_tail);
	oy->fifo_tail=0;
	oy->fifo_head=0;
	oy->fifo_fill=0;
	
	oy->unsynced=0;
	oy->headerbytes=0;
	oy->bodybytes=0;
	return OGG_SUCCESS;
}

ogg_stream_state *ogg_stream_create(int serialno){
	ogg_stream_state *os=_ogg_calloc(1,sizeof(*os));
	os->serialno=serialno;
	os->pageno=-1;
	return os;
} 

int ogg_stream_destroy(ogg_stream_state *os){
	if(os){
		ogg_buffer_release(os->header_tail);
		ogg_buffer_release(os->body_tail);
		memset(os,0,sizeof(*os));    
		_ogg_free(os);
	}
	return OGG_SUCCESS;
} 


#define FINFLAG 0x80000000UL
#define FINMASK 0x7fffffffUL

static void _next_lace(oggbyte_buffer *ob,ogg_stream_state *os){
	/* search ahead one lace */
	os->body_fill_next=0;
	while(os->laceptr<os->lacing_fill){
		int val=oggbyte_read1(ob,27+os->laceptr++);
		os->body_fill_next+=val;
		if(val<255){
			os->body_fill_next|=FINFLAG;
			os->clearflag=1;
			break;
		}
	}
}

static void _span_queued_page(ogg_stream_state *os){ 
	while( !(os->body_fill&FINFLAG) ){
		
		if(!os->header_tail)break;
		
		/* first flush out preceeding page header (if any).  Body is
		 flushed as it's consumed, so that's not done here. */
		
		if(os->lacing_fill>=0)
			os->header_tail=ogg_buffer_pretruncate(os->header_tail,
												   os->lacing_fill+27);
		os->lacing_fill=0;
		os->laceptr=0;
		os->clearflag=0;
		
		if(!os->header_tail){
			os->header_head=0;
			break;
		}else{
			
			/* process/prepare next page, if any */
			
			long pageno;
			oggbyte_buffer ob;
			ogg_page og;               /* only for parsing header values */
			og.header=os->header_tail; /* only for parsing header values */
			pageno=ogg_page_pageno(&og);
			
			oggbyte_init(&ob,os->header_tail);
			os->lacing_fill=oggbyte_read1(&ob,26);
			
			/* are we in sequence? */
			if(pageno!=os->pageno){
				if(os->pageno==-1) /* indicates seek or reset */
					os->holeflag=1;  /* set for internal use */
				else
					os->holeflag=2;  /* set for external reporting */
				
				os->body_tail=ogg_buffer_pretruncate(os->body_tail,
													 os->body_fill);
				if(os->body_tail==0)os->body_head=0;
				os->body_fill=0;
				
			}
			
			if(ogg_page_continued(&og)){
				if(os->body_fill==0){
					/* continued packet, but no preceeding data to continue */
					/* dump the first partial packet on the page */
					_next_lace(&ob,os);   
					os->body_tail=
					ogg_buffer_pretruncate(os->body_tail,os->body_fill_next&FINMASK);
					if(os->body_tail==0)os->body_head=0;
					/* set span flag */
					if(!os->spanflag && !os->holeflag)os->spanflag=2;
				}
			}else{
				if(os->body_fill>0){
					/* preceeding data to continue, but not a continued page */
					/* dump body_fill */
					os->body_tail=ogg_buffer_pretruncate(os->body_tail,
														 os->body_fill);
					if(os->body_tail==0)os->body_head=0;
					os->body_fill=0;
					
					/* set espan flag */
					if(!os->spanflag && !os->holeflag)os->spanflag=2;
				}
			}
			
			if(os->laceptr<os->lacing_fill){
				os->granulepos=ogg_page_granulepos(&og);
				
				/* get current packet size & flag */
				_next_lace(&ob,os);
				os->body_fill+=os->body_fill_next; /* addition handles the flag fine;
				 unsigned on purpose */
				/* ...and next packet size & flag */
				_next_lace(&ob,os);
				
			}
			
			os->pageno=pageno+1;
			os->e_o_s=ogg_page_eos(&og);
			os->b_o_s=ogg_page_bos(&og);
			
		}
	}
}

/* add the incoming page to the stream state; we decompose the page
 into packet segments here as well. */

int ogg_stream_pagein(ogg_stream_state *os, ogg_page *og){
	
	int serialno=ogg_page_serialno(og);
	int version=ogg_page_version(og);
	
	/* check the serial number */
	if(serialno!=os->serialno){
		ogg_page_release(og);
		return OGG_ESERIAL;
	}
	if(version>0){
		ogg_page_release(og);
		return OGG_EVERSION;
	}
	
	/* add to fifos */
	if(!os->body_tail){
		os->body_tail=og->body;
		os->body_head=ogg_buffer_walk(og->body);
	}else{
		os->body_head=ogg_buffer_cat(os->body_head,og->body);
	}
	if(!os->header_tail){
		os->header_tail=og->header;
		os->header_head=ogg_buffer_walk(og->header);
		os->lacing_fill=-27;
	}else{
		os->header_head=ogg_buffer_cat(os->header_head,og->header);
	}
	
	memset(og,0,sizeof(*og));
	return OGG_SUCCESS;
}

int ogg_stream_reset(ogg_stream_state *os){
	
	ogg_buffer_release(os->header_tail);
	ogg_buffer_release(os->body_tail);
	os->header_tail=os->header_head=0;
	os->body_tail=os->body_head=0;
	
	os->e_o_s=0;
	os->b_o_s=0;
	os->pageno=-1;
	os->packetno=0;
	os->granulepos=0;
	
	os->body_fill=0;
	os->lacing_fill=0;
	
	os->holeflag=0;
	os->spanflag=0;
	os->clearflag=0;
	os->laceptr=0;
	os->body_fill_next=0;
	
	return OGG_SUCCESS;
}

int ogg_stream_reset_serialno(ogg_stream_state *os,int serialno){
	ogg_stream_reset(os);
	os->serialno=serialno;
	return OGG_SUCCESS;
}

static int _packetout(ogg_stream_state *os,ogg_packet *op,int adv){
	
	ogg_packet_release(op);
	_span_queued_page(os);
	
	if(os->holeflag){
		int temp=os->holeflag;
		if(os->clearflag)
			os->holeflag=0;
		else
			os->holeflag=1;
		if(temp==2){
			os->packetno++;
			return OGG_HOLE;
		}
	}
	if(os->spanflag){
		int temp=os->spanflag;
		if(os->clearflag)
			os->spanflag=0;
		else
			os->spanflag=1;
		if(temp==2){
			os->packetno++;
			return OGG_SPAN;
		}
	}
	
	if(!(os->body_fill&FINFLAG)) return 0;
	if(!op && !adv)return 1; /* just using peek as an inexpensive way
	 to ask if there's a whole packet
	 waiting */
	if(op){
		op->b_o_s=os->b_o_s;
		if(os->e_o_s && os->body_fill_next==0)
			op->e_o_s=os->e_o_s;
		else
			op->e_o_s=0;
		if( (os->body_fill&FINFLAG) && !(os->body_fill_next&FINFLAG) )
			op->granulepos=os->granulepos;
		else
			op->granulepos=-1;
		op->packetno=os->packetno;
	}
	
	if(adv){
		oggbyte_buffer ob;
		oggbyte_init(&ob,os->header_tail);
		
		/* split the body contents off */
		if(op){
			op->packet=ogg_buffer_split(&os->body_tail,&os->body_head,
										os->body_fill&FINMASK);
			op->bytes=os->body_fill&FINMASK;
		}else{
			os->body_tail=ogg_buffer_pretruncate(os->body_tail,
												 os->body_fill&FINMASK);
			if(os->body_tail==0)os->body_head=0;
		}
		
		/* update lacing pointers */
		os->body_fill=os->body_fill_next;
		_next_lace(&ob,os);
	}else{
		if(op){
			op->packet=ogg_buffer_sub(os->body_tail,0,os->body_fill&FINMASK);
			op->bytes=os->body_fill&FINMASK;
		}
	}
	
	if(adv){
		os->packetno++;
		os->b_o_s=0;
	}
	
	return 1;
}

int ogg_stream_packetout(ogg_stream_state *os,ogg_packet *op){
	return _packetout(os,op,1);
}

int ogg_stream_packetpeek(ogg_stream_state *os,ogg_packet *op){
	return _packetout(os,op,0);
}

int ogg_packet_release(ogg_packet *op) {
	if(op){
		ogg_buffer_release(op->packet);
		memset(op, 0, sizeof(*op));
	}
	return OGG_SUCCESS;
}

int ogg_page_release(ogg_page *og) {
	if(og){
		ogg_buffer_release(og->header);
		ogg_buffer_release(og->body);
		memset(og, 0, sizeof(*og));
	}
	return OGG_SUCCESS;
}

void ogg_page_dup(ogg_page *dup,ogg_page *orig){
	dup->header_len=orig->header_len;
	dup->body_len=orig->body_len;
	dup->header=ogg_buffer_dup(orig->header);
	dup->body=ogg_buffer_dup(orig->body);
}

/* helpers */
static void _v_readstring(oggpack_buffer *o,char *buf,int bytes){
	while(bytes--){
		*buf++=oggpack_read(o,8);
	}
}

void vorbis_comment_init(vorbis_comment *vc){
	memset(vc,0,sizeof(*vc));
}

/* This is more or less the same as strncasecmp - but that doesn't exist
 * everywhere, and this is a fairly trivial function, so we include it */
static int tagcompare(const char *s1, const char *s2, int n){
	int c=0;
	while(c < n){
		if(toupper(s1[c]) != toupper(s2[c]))
			return !0;
		c++;
	}
	return 0;
}

char *vorbis_comment_query(vorbis_comment *vc, char *tag, int count){
	long i;
	int found = 0;
	int taglen = strlen(tag)+1; /* +1 for the = we append */
	char *fulltag = (char *)alloca(taglen+ 1);
	
	strcpy(fulltag, tag);
	strcat(fulltag, "=");
	
	for(i=0;i<vc->comments;i++){
		if(!tagcompare(vc->user_comments[i], fulltag, taglen)){
			if(count == found)
			/* We return a pointer to the data, not a copy */
				return vc->user_comments[i] + taglen;
			else
				found++;
		}
	}
	return NULL; /* didn't find anything */
}

int vorbis_comment_query_count(vorbis_comment *vc, char *tag){
	int i,count=0;
	int taglen = strlen(tag)+1; /* +1 for the = we append */
	char *fulltag = (char *)alloca(taglen+1);
	strcpy(fulltag,tag);
	strcat(fulltag, "=");
	
	for(i=0;i<vc->comments;i++){
		if(!tagcompare(vc->user_comments[i], fulltag, taglen))
			count++;
	}
	
	return count;
}

void vorbis_comment_clear(vorbis_comment *vc){
	if(vc){
		long i;
		for(i=0;i<vc->comments;i++)
			if(vc->user_comments[i])_ogg_free(vc->user_comments[i]);
		if(vc->user_comments)_ogg_free(vc->user_comments);
		if(vc->comment_lengths)_ogg_free(vc->comment_lengths);
		if(vc->vendor)_ogg_free(vc->vendor);
		memset(vc,0,sizeof(*vc));
	}
}

/* blocksize 0 is guaranteed to be short, 1 is guarantted to be long.
 They may be equal, but short will never ge greater than long */
int vorbis_info_blocksize(vorbis_info *vi,int zo){
	codec_setup_info *ci = (codec_setup_info *)vi->codec_setup;
	return ci ? ci->blocksizes[zo] : -1;
}

/* used by synthesis, which has a full, alloced vi */
void vorbis_info_init(vorbis_info *vi){
	memset(vi,0,sizeof(*vi));
	vi->codec_setup=(codec_setup_info *)_ogg_calloc(1,sizeof(codec_setup_info));
}

void vorbis_info_clear(vorbis_info *vi){
	codec_setup_info     *ci=(codec_setup_info *)vi->codec_setup;
	int i;
	
	if(ci){
		
		for(i=0;i<ci->modes;i++)
			if(ci->mode_param[i])_ogg_free(ci->mode_param[i]);
		
		for(i=0;i<ci->maps;i++) /* unpack does the range checking */
			if(ci->map_param[i])
				_mapping_P[ci->map_type[i]]->free_info(ci->map_param[i]);
		
		for(i=0;i<ci->floors;i++) /* unpack does the range checking */
			if(ci->floor_param[i])
				_floor_P[ci->floor_type[i]]->free_info(ci->floor_param[i]);
		
		for(i=0;i<ci->residues;i++) /* unpack does the range checking */
			if(ci->residue_param[i])
				_residue_P[ci->residue_type[i]]->free_info(ci->residue_param[i]);
		
		for(i=0;i<ci->books;i++){
			if(ci->book_param[i]){
				/* knows if the book was not alloced */
				vorbis_staticbook_destroy(ci->book_param[i]);
			}
			if(ci->fullbooks)
				vorbis_book_clear(ci->fullbooks+i);
		}
		if(ci->fullbooks)
			_ogg_free(ci->fullbooks);
		
		_ogg_free(ci);
	}
	
	memset(vi,0,sizeof(*vi));
}

/* Header packing/unpacking ********************************************/

static int _vorbis_unpack_info(vorbis_info *vi,oggpack_buffer *opb){
	codec_setup_info     *ci=(codec_setup_info *)vi->codec_setup;
	if(!ci)return(OV_EFAULT);
	
	vi->version=oggpack_read(opb,32);
	if(vi->version!=0)return(OV_EVERSION);
	
	vi->channels=oggpack_read(opb,8);
	vi->rate=oggpack_read(opb,32);
	
	vi->bitrate_upper=oggpack_read(opb,32);
	vi->bitrate_nominal=oggpack_read(opb,32);
	vi->bitrate_lower=oggpack_read(opb,32);
	
	ci->blocksizes[0]=1<<oggpack_read(opb,4);
	ci->blocksizes[1]=1<<oggpack_read(opb,4);
	
	if(vi->rate<1)goto err_out;
	if(vi->channels<1)goto err_out;
	if(ci->blocksizes[0]<64)goto err_out; 
	if(ci->blocksizes[1]<ci->blocksizes[0])goto err_out;
	if(ci->blocksizes[1]>8192)goto err_out;
	
	if(oggpack_read(opb,1)!=1)goto err_out; /* EOP check */
	
	return(0);
err_out:
	vorbis_info_clear(vi);
	return(OV_EBADHEADER);
}

static int _vorbis_unpack_comment(vorbis_comment *vc,oggpack_buffer *opb){
	int i;
	int vendorlen=oggpack_read(opb,32);
	if(vendorlen<0)goto err_out;
	vc->vendor=(char *)_ogg_calloc(vendorlen+1,1);
	_v_readstring(opb,vc->vendor,vendorlen);
	vc->comments=oggpack_read(opb,32);
	if(vc->comments<0)goto err_out;
	vc->user_comments=(char **)_ogg_calloc(vc->comments+1,sizeof(*vc->user_comments));
	vc->comment_lengths=(int *)_ogg_calloc(vc->comments+1, sizeof(*vc->comment_lengths));
	
	for(i=0;i<vc->comments;i++){
		int len=oggpack_read(opb,32);
		if(len<0)goto err_out;
		vc->comment_lengths[i]=len;
		vc->user_comments[i]=(char *)_ogg_calloc(len+1,1);
		_v_readstring(opb,vc->user_comments[i],len);
	}	  
	if(oggpack_read(opb,1)!=1)goto err_out; /* EOP check */
	
	return(0);
err_out:
	vorbis_comment_clear(vc);
	return(OV_EBADHEADER);
}

/* all of the real encoding details are here.  The modes, books,
 everything */
static int _vorbis_unpack_books(vorbis_info *vi,oggpack_buffer *opb){
	codec_setup_info     *ci=(codec_setup_info *)vi->codec_setup;
	int i;
	if(!ci)return(OV_EFAULT);
	
	/* codebooks */
	ci->books=oggpack_read(opb,8)+1;
	/*ci->book_param=_ogg_calloc(ci->books,sizeof(*ci->book_param));*/
	for(i=0;i<ci->books;i++){
		ci->book_param[i]=(static_codebook *)_ogg_calloc(1,sizeof(*ci->book_param[i]));
		if(vorbis_staticbook_unpack(opb,ci->book_param[i]))goto err_out;
	}
	
	/* time backend settings */
	ci->times=oggpack_read(opb,6)+1;
	/*ci->time_type=_ogg_malloc(ci->times*sizeof(*ci->time_type));*/
	/*ci->time_param=_ogg_calloc(ci->times,sizeof(void *));*/
	for(i=0;i<ci->times;i++){
		ci->time_type[i]=oggpack_read(opb,16);
		if(ci->time_type[i]<0 || ci->time_type[i]>=VI_TIMEB)goto err_out;
		/* ci->time_param[i]=_time_P[ci->time_type[i]]->unpack(vi,opb);
		 Vorbis I has no time backend */
		/*if(!ci->time_param[i])goto err_out;*/
	}
	
	/* floor backend settings */
	ci->floors=oggpack_read(opb,6)+1;
	/*ci->floor_type=_ogg_malloc(ci->floors*sizeof(*ci->floor_type));*/
	/*ci->floor_param=_ogg_calloc(ci->floors,sizeof(void *));*/
	for(i=0;i<ci->floors;i++){
		ci->floor_type[i]=oggpack_read(opb,16);
		if(ci->floor_type[i]<0 || ci->floor_type[i]>=VI_FLOORB)goto err_out;
		ci->floor_param[i]=_floor_P[ci->floor_type[i]]->unpack(vi,opb);
		if(!ci->floor_param[i])goto err_out;
	}
	
	/* residue backend settings */
	ci->residues=oggpack_read(opb,6)+1;
	/*ci->residue_type=_ogg_malloc(ci->residues*sizeof(*ci->residue_type));*/
	/*ci->residue_param=_ogg_calloc(ci->residues,sizeof(void *));*/
	for(i=0;i<ci->residues;i++){
		ci->residue_type[i]=oggpack_read(opb,16);
		if(ci->residue_type[i]<0 || ci->residue_type[i]>=VI_RESB)goto err_out;
		ci->residue_param[i]=_residue_P[ci->residue_type[i]]->unpack(vi,opb);
		if(!ci->residue_param[i])goto err_out;
	}
	
	/* map backend settings */
	ci->maps=oggpack_read(opb,6)+1;
	/*ci->map_type=_ogg_malloc(ci->maps*sizeof(*ci->map_type));*/
	/*ci->map_param=_ogg_calloc(ci->maps,sizeof(void *));*/
	for(i=0;i<ci->maps;i++){
		ci->map_type[i]=oggpack_read(opb,16);
		if(ci->map_type[i]<0 || ci->map_type[i]>=VI_MAPB)goto err_out;
		ci->map_param[i]=_mapping_P[ci->map_type[i]]->unpack(vi,opb);
		if(!ci->map_param[i])goto err_out;
	}
	
	/* mode settings */
	ci->modes=oggpack_read(opb,6)+1;
	/*vi->mode_param=_ogg_calloc(vi->modes,sizeof(void *));*/
	for(i=0;i<ci->modes;i++){
		ci->mode_param[i]=(vorbis_info_mode *)_ogg_calloc(1,sizeof(*ci->mode_param[i]));
		ci->mode_param[i]->blockflag=oggpack_read(opb,1);
		ci->mode_param[i]->windowtype=oggpack_read(opb,16);
		ci->mode_param[i]->transformtype=oggpack_read(opb,16);
		ci->mode_param[i]->mapping=oggpack_read(opb,8);
		
		if(ci->mode_param[i]->windowtype>=VI_WINDOWB)goto err_out;
		if(ci->mode_param[i]->transformtype>=VI_WINDOWB)goto err_out;
		if(ci->mode_param[i]->mapping>=ci->maps)goto err_out;
	}
	
	if(oggpack_read(opb,1)!=1)goto err_out; /* top level EOP check */
	
	return(0);
err_out:
	vorbis_info_clear(vi);
	return(OV_EBADHEADER);
}

/* The Vorbis header is in three packets; the initial small packet in
 the first page that identifies basic parameters, a second packet
 with bitstream comments and a third packet that holds the
 codebook. */

int vorbis_synthesis_headerin(vorbis_info *vi,vorbis_comment *vc,ogg_packet *op){
	oggpack_buffer opb;
	
	if(op){
		oggpack_readinit(&opb,op->packet);
		
		/* Which of the three types of header is this? */
		/* Also verify header-ness, vorbis */
		{
			char buffer[6];
			int packtype=oggpack_read(&opb,8);
			memset(buffer,0,6);
			_v_readstring(&opb,buffer,6);
			if(memcmp(buffer,"vorbis",6)){
				/* not a vorbis header */
				return(OV_ENOTVORBIS);
			}
			switch(packtype){
				case 0x01: /* least significant *bit* is read first */
					if(!op->b_o_s){
						/* Not the initial packet */
						return(OV_EBADHEADER);
					}
					if(vi->rate!=0){
						/* previously initialized info header */
						return(OV_EBADHEADER);
					}
					
					return(_vorbis_unpack_info(vi,&opb));
					
				case 0x03: /* least significant *bit* is read first */
					if(vi->rate==0){
						/* um... we didn't get the initial header */
						return(OV_EBADHEADER);
					}
					
					return(_vorbis_unpack_comment(vc,&opb));
					
				case 0x05: /* least significant *bit* is read first */
					if(vi->rate==0 || vc->vendor==NULL){
						/* um... we didn;t get the initial header or comments yet */
						return(OV_EBADHEADER);
					}
					
					return(_vorbis_unpack_books(vi,&opb));
					
				default:
					/* Not a valid vorbis header type */
					return(OV_EBADHEADER);
					break;
			}
		}
	}
	return(OV_EBADHEADER);
}

/* simplistic, wasteful way of doing this (unique lookup for each
 mode/submapping); there should be a central repository for
 identical lookups.  That will require minor work, so I'm putting it
 off as low priority.
 
 Why a lookup for each backend in a given mode?  Because the
 blocksize is set by the mode, and low backend lookups may require
 parameters from other areas of the mode/mapping */

typedef struct {
	vorbis_info_mode *mode;
	vorbis_info_mapping0 *map;
	
	vorbis_look_floor **floor_look;
	
	vorbis_look_residue **residue_look;
	
	vorbis_func_floor **floor_func;
	vorbis_func_residue **residue_func;
	
	int ch;
	long lastframe; /* if a different mode is called, we need to 
	 invalidate decay */
} vorbis_look_mapping0;

static void mapping0_free_info(vorbis_info_mapping *i){
	vorbis_info_mapping0 *info=(vorbis_info_mapping0 *)i;
	if(info){
		memset(info,0,sizeof(*info));
		_ogg_free(info);
	}
}

static void mapping0_free_look(vorbis_look_mapping *look){
	int i;
	vorbis_look_mapping0 *l=(vorbis_look_mapping0 *)look;
	if(l){
		
		for(i=0;i<l->map->submaps;i++){
			l->floor_func[i]->free_look(l->floor_look[i]);
			l->residue_func[i]->free_look(l->residue_look[i]);
		}
		
		_ogg_free(l->floor_func);
		_ogg_free(l->residue_func);
		_ogg_free(l->floor_look);
		_ogg_free(l->residue_look);
		memset(l,0,sizeof(*l));
		_ogg_free(l);
	}
}

static vorbis_look_mapping *mapping0_look(vorbis_dsp_state *vd,vorbis_info_mode *vm,
										  vorbis_info_mapping *m){
	int i;
	vorbis_info          *vi=vd->vi;
	codec_setup_info     *ci=(codec_setup_info *)vi->codec_setup;
	vorbis_look_mapping0 *look=(vorbis_look_mapping0 *)_ogg_calloc(1,sizeof(*look));
	vorbis_info_mapping0 *info=look->map=(vorbis_info_mapping0 *)m;
	look->mode=vm;
	
	look->floor_look=(vorbis_look_floor **)_ogg_calloc(info->submaps,sizeof(*look->floor_look));
	
	look->residue_look=(vorbis_look_residue **)_ogg_calloc(info->submaps,sizeof(*look->residue_look));
	
	look->floor_func=(vorbis_func_floor **)_ogg_calloc(info->submaps,sizeof(*look->floor_func));
	look->residue_func=(vorbis_func_residue **)_ogg_calloc(info->submaps,sizeof(*look->residue_func));
	
	for(i=0;i<info->submaps;i++){
		int floornum=info->floorsubmap[i];
		int resnum=info->residuesubmap[i];
		
		look->floor_func[i]=_floor_P[ci->floor_type[floornum]];
		look->floor_look[i]=look->floor_func[i]->
		look(vd,vm,ci->floor_param[floornum]);
		look->residue_func[i]=_residue_P[ci->residue_type[resnum]];
		look->residue_look[i]=look->residue_func[i]->
		look(vd,vm,ci->residue_param[resnum]);
		
	}
	
	look->ch=vi->channels;
	
	return(look);
}

static int ilog3(unsigned int v){
	int ret=0;
	if(v)--v;
	while(v){
		ret++;
		v>>=1;
	}
	return(ret);
}

/* also responsible for range checking */
static vorbis_info_mapping *mapping0_unpack(vorbis_info *vi,oggpack_buffer *opb){
	int i;
	vorbis_info_mapping0 *info=(vorbis_info_mapping0 *)_ogg_calloc(1,sizeof(*info));
	codec_setup_info     *ci=(codec_setup_info *)vi->codec_setup;
	memset(info,0,sizeof(*info));
	
	if(oggpack_read(opb,1))
		info->submaps=oggpack_read(opb,4)+1;
	else
		info->submaps=1;
	
	if(oggpack_read(opb,1)){
		info->coupling_steps=oggpack_read(opb,8)+1;
		
		for(i=0;i<info->coupling_steps;i++){
			int testM=info->coupling_mag[i]=oggpack_read(opb,ilog3(vi->channels));
			int testA=info->coupling_ang[i]=oggpack_read(opb,ilog3(vi->channels));
			
			if(testM<0 || 
			   testA<0 || 
			   testM==testA || 
			   testM>=vi->channels ||
			   testA>=vi->channels) goto err_out;
		}
		
	}
	
	if(oggpack_read(opb,2)>0)goto err_out; /* 2,3:reserved */
    
	if(info->submaps>1){
		for(i=0;i<vi->channels;i++){
			info->chmuxlist[i]=oggpack_read(opb,4);
			if(info->chmuxlist[i]>=info->submaps)goto err_out;
		}
	}
	for(i=0;i<info->submaps;i++){
		int temp=oggpack_read(opb,8);
		if(temp>=ci->times)goto err_out;
		info->floorsubmap[i]=oggpack_read(opb,8);
		if(info->floorsubmap[i]>=ci->floors)goto err_out;
		info->residuesubmap[i]=oggpack_read(opb,8);
		if(info->residuesubmap[i]>=ci->residues)goto err_out;
	}
	
	return info;
	
err_out:
	mapping0_free_info(info);
	return(NULL);
}


static void _vorbis_apply_window(ogg_int32_t *d,const void *window_p[2],
								 long *blocksizes,
								 int lW,int W,int nW){
	
	LOOKUP_T *window[2]={window_p[0],window_p[1]};
	long n=blocksizes[W];
	long ln=blocksizes[lW];
	long rn=blocksizes[nW];
	
	long leftbegin=(n>>2)-(ln>>2);
	long leftend=leftbegin+(ln>>1);
	
	long rightbegin=(n>>1)+(n>>2)-(rn>>2);
	long rightend=rightbegin+(rn>>1);
	
	int i,p;
	
	for(i=0;i<leftbegin;i++)
		d[i]=0;
	
	for(p=0;i<leftend;i++,p++)
		d[i]=MULT31(d[i],window[lW][p]);
	
	for(i=rightbegin,p=(rn>>1)-1;i<rightend;i++,p--)
		d[i]=MULT31(d[i],window[nW][p]);
	
	for(;i<n;i++)
		d[i]=0;
}

static int seq=0;
static int mapping0_inverse(vorbis_block *vb,vorbis_look_mapping *l){
	vorbis_dsp_state     *vd=vb->vd;
	vorbis_info          *vi=vd->vi;
	codec_setup_info     *ci=(codec_setup_info *)vi->codec_setup;
	private_state        *b=(private_state *)vd->backend_state;
	vorbis_look_mapping0 *look=(vorbis_look_mapping0 *)l;
	vorbis_info_mapping0 *info=look->map;
	
	int                   i,j;
	long                  n=vb->pcmend=ci->blocksizes[vb->W];
	
	ogg_int32_t **pcmbundle=(ogg_int32_t **)alloca(sizeof(*pcmbundle)*vi->channels);
	int    *zerobundle=(int *)alloca(sizeof(*zerobundle)*vi->channels);
	
	int   *nonzero  =(int *)alloca(sizeof(*nonzero)*vi->channels);
	void **floormemo=(void **)alloca(sizeof(*floormemo)*vi->channels);
	
	/* time domain information decode (note that applying the
     information would have to happen later; we'll probably add a
     function entry to the harness for that later */
	/* NOT IMPLEMENTED */
	
	/* recover the spectral envelope; store it in the PCM vector for now */
	for(i=0;i<vi->channels;i++){
		int submap=info->chmuxlist[i];
		floormemo[i]=look->floor_func[submap]->
		inverse1(vb,look->floor_look[submap]);
		if(floormemo[i])
			nonzero[i]=1;
		else
			nonzero[i]=0;      
		memset(vb->pcm[i],0,(sizeof(*vb->pcm[i])*n)>>1);
	}
	
	/* channel coupling can 'dirty' the nonzero listing */
	for(i=0;i<info->coupling_steps;i++){
		if(nonzero[info->coupling_mag[i]] ||
		   nonzero[info->coupling_ang[i]]){
			nonzero[info->coupling_mag[i]]=1; 
			nonzero[info->coupling_ang[i]]=1; 
		}
	}
	
	/* recover the residue into our working vectors */
	for(i=0;i<info->submaps;i++){
		int ch_in_bundle=0;
		for(j=0;j<vi->channels;j++){
			if(info->chmuxlist[j]==i){
				if(nonzero[j])
					zerobundle[ch_in_bundle]=1;
				else
					zerobundle[ch_in_bundle]=0;
				pcmbundle[ch_in_bundle++]=vb->pcm[j];
			}
		}
		
		look->residue_func[i]->inverse(vb,look->residue_look[i],
									   pcmbundle,zerobundle,ch_in_bundle);
	}
	
	//for(j=0;j<vi->channels;j++)
	//_analysis_output("coupled",seq+j,vb->pcm[j],-8,n/2,0,0);
	
	
	/* channel coupling */
	for(i=info->coupling_steps-1;i>=0;--i){
		ogg_int32_t *pcmM=vb->pcm[info->coupling_mag[i]];
		ogg_int32_t *pcmA=vb->pcm[info->coupling_ang[i]];
		
		for(j=0;j<(n>>1);++j){
			ogg_int32_t mag=pcmM[j];
			ogg_int32_t ang=pcmA[j];
			
			if(mag>0)
				if(ang>0){
					pcmM[j]=mag;
					pcmA[j]=mag-ang;
				}else{
					pcmA[j]=mag;
					pcmM[j]=mag+ang;
				}
				else
					if(ang>0){
						pcmM[j]=mag;
						pcmA[j]=mag+ang;
					}else{
						pcmA[j]=mag;
						pcmM[j]=mag-ang;
					}
		}
	}
	
	//for(j=0;j<vi->channels;j++)
	//_analysis_output("residue",seq+j,vb->pcm[j],-8,n/2,0,0);
	
	/* compute and apply spectral envelope */
	for(i=0;i<vi->channels;++i){
		ogg_int32_t *pcm=vb->pcm[i];
		int submap=info->chmuxlist[i];
		look->floor_func[submap]->
		inverse2(vb,look->floor_look[submap],floormemo[i],pcm);
	}
	
	//for(j=0;j<vi->channels;j++)
	//_analysis_output("mdct",seq+j,vb->pcm[j],-24,n/2,0,1);
	
	/* transform the PCM data; takes PCM vector, vb; modifies PCM vector */
	/* only MDCT right now.... */
	for(i=0;i<vi->channels;i++){
		ogg_int32_t *pcm=vb->pcm[i];
		mdct_backward(n,pcm,pcm);
	}
	
	//for(j=0;j<vi->channels;j++)
	//_analysis_output("imdct",seq+j,vb->pcm[j],-24,n,0,0);
	
	/* window the data */
	for(i=0;i<vi->channels;i++){
		ogg_int32_t *pcm=vb->pcm[i];
		if(nonzero[i])
			_vorbis_apply_window(pcm,b->window,ci->blocksizes,vb->lW,vb->W,vb->nW);
		else
			for(j=0;j<n;j++)
				pcm[j]=0;
		
	}
	
	//for(j=0;j<vi->channels;j++)
	//_analysis_output("window",seq+j,vb->pcm[j],-24,n,0,0);
	
	seq+=vi->channels;
	/* all done! */
	return(0);
}

/* export hooks */
vorbis_func_mapping mapping0_exportbundle={
&mapping0_unpack,
&mapping0_look,
&mapping0_free_info,
&mapping0_free_look,
&mapping0_inverse
};

/*
 function: normalized modified discrete cosine transform
 power of two length transform only [64 <= n ]
 last mod: $Id: mdct.c,v 1.9 2002/10/16 09:17:39 xiphmont Exp $
 
 Original algorithm adapted long ago from _The use of multirate filter
 banks for coding of high quality digital audio_, by T. Sporer,
 K. Brandenburg and B. Edler, collection of the European Signal
 Processing Conference (EUSIPCO), Amsterdam, June 1992, Vol.1, pp
 211-214
 
 The below code implements an algorithm that no longer looks much like
 that presented in the paper, but the basic structure remains if you
 dig deep enough to see it.
 
 This module DOES NOT INCLUDE code to generate/apply the window
 function.  Everybody has their own weird favorite including me... I
 happen to like the properties of y=sin(.5PI*sin^2(x)), but others may
 vehemently disagree.
 
 ********************************************************************/
/* 8 point butterfly (in place) */
STIN void mdct_butterfly_8(DATA_TYPE *x){
	
	REG_TYPE r0   = x[4] + x[0];
	REG_TYPE r1   = x[4] - x[0];
	REG_TYPE r2   = x[5] + x[1];
	REG_TYPE r3   = x[5] - x[1];
	REG_TYPE r4   = x[6] + x[2];
	REG_TYPE r5   = x[6] - x[2];
	REG_TYPE r6   = x[7] + x[3];
	REG_TYPE r7   = x[7] - x[3];
	
	x[0] = r5   + r3;
	x[1] = r7   - r1;
	x[2] = r5   - r3;
	x[3] = r7   + r1;
	x[4] = r4   - r0;
	x[5] = r6   - r2;
	x[6] = r4   + r0;
	x[7] = r6   + r2;
	MB();
}

/* 16 point butterfly (in place, 4 register) */
STIN void mdct_butterfly_16(DATA_TYPE *x){
	
	REG_TYPE r0, r1;
	
	r0 = x[ 0] - x[ 8]; x[ 8] += x[ 0];
	r1 = x[ 1] - x[ 9]; x[ 9] += x[ 1];
	x[ 0] = MULT31((r0 + r1) , cPI2_8);
	x[ 1] = MULT31((r1 - r0) , cPI2_8);
	MB();
	
	r0 = x[10] - x[ 2]; x[10] += x[ 2];
	r1 = x[ 3] - x[11]; x[11] += x[ 3];
	x[ 2] = r1; x[ 3] = r0;
	MB();
	
	r0 = x[12] - x[ 4]; x[12] += x[ 4];
	r1 = x[13] - x[ 5]; x[13] += x[ 5];
	x[ 4] = MULT31((r0 - r1) , cPI2_8);
	x[ 5] = MULT31((r0 + r1) , cPI2_8);
	MB();
	
	r0 = x[14] - x[ 6]; x[14] += x[ 6];
	r1 = x[15] - x[ 7]; x[15] += x[ 7];
	x[ 6] = r0; x[ 7] = r1;
	MB();
	
	mdct_butterfly_8(x);
	mdct_butterfly_8(x+8);
}

/* 32 point butterfly (in place, 4 register) */
STIN void mdct_butterfly_32(DATA_TYPE *x){
	
	REG_TYPE r0, r1;
	
	r0 = x[30] - x[14]; x[30] += x[14];           
	r1 = x[31] - x[15]; x[31] += x[15];
	x[14] = r0; x[15] = r1;
	MB();
	
	r0 = x[28] - x[12]; x[28] += x[12];           
	r1 = x[29] - x[13]; x[29] += x[13];
	XNPROD31( r0, r1, cPI1_8, cPI3_8, &x[12], &x[13] );
	MB();
	
	r0 = x[26] - x[10]; x[26] += x[10];
	r1 = x[27] - x[11]; x[27] += x[11];
	x[10] = MULT31((r0 - r1) , cPI2_8);
	x[11] = MULT31((r0 + r1) , cPI2_8);
	MB();
	
	r0 = x[24] - x[ 8]; x[24] += x[ 8];
	r1 = x[25] - x[ 9]; x[25] += x[ 9];
	XNPROD31( r0, r1, cPI3_8, cPI1_8, &x[ 8], &x[ 9] );
	MB();
	
	r0 = x[22] - x[ 6]; x[22] += x[ 6];
	r1 = x[ 7] - x[23]; x[23] += x[ 7];
	x[ 6] = r1; x[ 7] = r0;
	MB();
	
	r0 = x[ 4] - x[20]; x[20] += x[ 4];
	r1 = x[ 5] - x[21]; x[21] += x[ 5];
	XPROD31 ( r0, r1, cPI3_8, cPI1_8, &x[ 4], &x[ 5] );
	MB();
	
	r0 = x[ 2] - x[18]; x[18] += x[ 2];
	r1 = x[ 3] - x[19]; x[19] += x[ 3];
	x[ 2] = MULT31((r1 + r0) , cPI2_8);
	x[ 3] = MULT31((r1 - r0) , cPI2_8);
	MB();
	
	r0 = x[ 0] - x[16]; x[16] += x[ 0];
	r1 = x[ 1] - x[17]; x[17] += x[ 1];
	XPROD31 ( r0, r1, cPI1_8, cPI3_8, &x[ 0], &x[ 1] );
	MB();
	
	mdct_butterfly_16(x);
	mdct_butterfly_16(x+16);
}

/* N/stage point generic N stage butterfly (in place, 2 register) */
STIN void mdct_butterfly_generic(DATA_TYPE *x,int points,int step){
	
	LOOKUP_T *T   = sincos_lookup0;
	DATA_TYPE *x1        = x + points      - 8;
	DATA_TYPE *x2        = x + (points>>1) - 8;
	REG_TYPE   r0;
	REG_TYPE   r1;
	
	do{
		r0 = x1[6] - x2[6]; x1[6] += x2[6];
		r1 = x2[7] - x1[7]; x1[7] += x2[7];
		XPROD31( r1, r0, T[0], T[1], &x2[6], &x2[7] ); T+=step;
		
		r0 = x1[4] - x2[4]; x1[4] += x2[4];
		r1 = x2[5] - x1[5]; x1[5] += x2[5];
		XPROD31( r1, r0, T[0], T[1], &x2[4], &x2[5] ); T+=step;
		
		r0 = x1[2] - x2[2]; x1[2] += x2[2];
		r1 = x2[3] - x1[3]; x1[3] += x2[3];
		XPROD31( r1, r0, T[0], T[1], &x2[2], &x2[3] ); T+=step;
		
		r0 = x1[0] - x2[0]; x1[0] += x2[0];
		r1 = x2[1] - x1[1]; x1[1] += x2[1];
		XPROD31( r1, r0, T[0], T[1], &x2[0], &x2[1] ); T+=step;
		
		x1-=8; x2-=8;
	}while(T<sincos_lookup0+1024);
	do{
		r0 = x1[6] - x2[6]; x1[6] += x2[6];
		r1 = x1[7] - x2[7]; x1[7] += x2[7];
		XNPROD31( r0, r1, T[0], T[1], &x2[6], &x2[7] ); T-=step;
		
		r0 = x1[4] - x2[4]; x1[4] += x2[4];
		r1 = x1[5] - x2[5]; x1[5] += x2[5];
		XNPROD31( r0, r1, T[0], T[1], &x2[4], &x2[5] ); T-=step;
		
		r0 = x1[2] - x2[2]; x1[2] += x2[2];
		r1 = x1[3] - x2[3]; x1[3] += x2[3];
		XNPROD31( r0, r1, T[0], T[1], &x2[2], &x2[3] ); T-=step;
		
		r0 = x1[0] - x2[0]; x1[0] += x2[0];
		r1 = x1[1] - x2[1]; x1[1] += x2[1];
		XNPROD31( r0, r1, T[0], T[1], &x2[0], &x2[1] ); T-=step;
		
		x1-=8; x2-=8;
	}while(T>sincos_lookup0);
	do{
		r0 = x2[6] - x1[6]; x1[6] += x2[6];
		r1 = x2[7] - x1[7]; x1[7] += x2[7];
		XPROD31( r0, r1, T[0], T[1], &x2[6], &x2[7] ); T+=step;
		
		r0 = x2[4] - x1[4]; x1[4] += x2[4];
		r1 = x2[5] - x1[5]; x1[5] += x2[5];
		XPROD31( r0, r1, T[0], T[1], &x2[4], &x2[5] ); T+=step;
		
		r0 = x2[2] - x1[2]; x1[2] += x2[2];
		r1 = x2[3] - x1[3]; x1[3] += x2[3];
		XPROD31( r0, r1, T[0], T[1], &x2[2], &x2[3] ); T+=step;
		
		r0 = x2[0] - x1[0]; x1[0] += x2[0];
		r1 = x2[1] - x1[1]; x1[1] += x2[1];
		XPROD31( r0, r1, T[0], T[1], &x2[0], &x2[1] ); T+=step;
		
		x1-=8; x2-=8;
	}while(T<sincos_lookup0+1024);
	do{
		r0 = x1[6] - x2[6]; x1[6] += x2[6];
		r1 = x2[7] - x1[7]; x1[7] += x2[7];
		XNPROD31( r1, r0, T[0], T[1], &x2[6], &x2[7] ); T-=step;
		
		r0 = x1[4] - x2[4]; x1[4] += x2[4];
		r1 = x2[5] - x1[5]; x1[5] += x2[5];
		XNPROD31( r1, r0, T[0], T[1], &x2[4], &x2[5] ); T-=step;
		
		r0 = x1[2] - x2[2]; x1[2] += x2[2];
		r1 = x2[3] - x1[3]; x1[3] += x2[3];
		XNPROD31( r1, r0, T[0], T[1], &x2[2], &x2[3] ); T-=step;
		
		r0 = x1[0] - x2[0]; x1[0] += x2[0];
		r1 = x2[1] - x1[1]; x1[1] += x2[1];
		XNPROD31( r1, r0, T[0], T[1], &x2[0], &x2[1] ); T-=step;
		
		x1-=8; x2-=8;
	}while(T>sincos_lookup0);
}

STIN void mdct_butterflies(DATA_TYPE *x,int points,int shift){
	
	int stages=8-shift;
	int i,j;
	
	for(i=0;--stages>0;i++){
		for(j=0;j<(1<<i);j++)
			mdct_butterfly_generic(x+(points>>i)*j,points>>i,4<<(i+shift));
	}
	
	for(j=0;j<points;j+=32)
		mdct_butterfly_32(x+j);
	
}

static unsigned char bitrev[16]={0,8,4,12,2,10,6,14,1,9,5,13,3,11,7,15};

STIN int bitrev12(int x){
	return bitrev[x>>8]|(bitrev[(x&0x0f0)>>4]<<4)|(((int)bitrev[x&0x00f])<<8);
}

STIN void mdct_bitreverse(DATA_TYPE *x,int n,int step,int shift){
	
	int          bit   = 0;
	DATA_TYPE   *w0    = x;
	DATA_TYPE   *w1    = x = w0+(n>>1);
	LOOKUP_T    *T = (step>=4)?(sincos_lookup0+(step>>1)):sincos_lookup1;
	LOOKUP_T    *Ttop  = T+1024;
	DATA_TYPE    r2;
	
	do{
		DATA_TYPE r3     = bitrev12(bit++);
		DATA_TYPE *x0    = x + ((r3 ^ 0xfff)>>shift) -1;
		DATA_TYPE *x1    = x + (r3>>shift);
		
		REG_TYPE  r0     = x0[0]  + x1[0];
		REG_TYPE  r1     = x1[1]  - x0[1];
		
		XPROD32( r0, r1, T[1], T[0], &r2, &r3 ); T+=step;
		
		w1    -= 4;
		
		r0     = (x0[1] + x1[1])>>1;
		r1     = (x0[0] - x1[0])>>1;
		w0[0]  = r0     + r2;
		w0[1]  = r1     + r3;
		w1[2]  = r0     - r2;
		w1[3]  = r3     - r1;
		
		r3     = bitrev12(bit++);
		x0     = x + ((r3 ^ 0xfff)>>shift) -1;
		x1     = x + (r3>>shift);
		
		r0     = x0[0]  + x1[0];
		r1     = x1[1]  - x0[1];
		
		XPROD32( r0, r1, T[1], T[0], &r2, &r3 ); T+=step;
		
		r0     = (x0[1] + x1[1])>>1;
		r1     = (x0[0] - x1[0])>>1;
		w0[2]  = r0     + r2;
		w0[3]  = r1     + r3;
		w1[0]  = r0     - r2;
		w1[1]  = r3     - r1;
		
		w0    += 4;
	}while(T<Ttop);
	do{
		DATA_TYPE r3     = bitrev12(bit++);
		DATA_TYPE *x0    = x + ((r3 ^ 0xfff)>>shift) -1;
		DATA_TYPE *x1    = x + (r3>>shift);
		
		REG_TYPE  r0     = x0[0]  + x1[0];
		REG_TYPE  r1     = x1[1]  - x0[1];
		
		T-=step; XPROD32( r0, r1, T[0], T[1], &r2, &r3 );
		
		w1    -= 4;
		
		r0     = (x0[1] + x1[1])>>1;
		r1     = (x0[0] - x1[0])>>1;
		w0[0]  = r0     + r2;
		w0[1]  = r1     + r3;
		w1[2]  = r0     - r2;
		w1[3]  = r3     - r1;
		
		r3     = bitrev12(bit++);
		x0     = x + ((r3 ^ 0xfff)>>shift) -1;
		x1     = x + (r3>>shift);
		
		r0     = x0[0]  + x1[0];
		r1     = x1[1]  - x0[1];
		
		T-=step; XPROD32( r0, r1, T[0], T[1], &r2, &r3 );
		
		r0     = (x0[1] + x1[1])>>1;
		r1     = (x0[0] - x1[0])>>1;
		w0[2]  = r0     + r2;
		w0[3]  = r1     + r3;
		w1[0]  = r0     - r2;
		w1[1]  = r3     - r1;
		
		w0    += 4;
	}while(w0<w1);
}

void mdct_backward(int n, DATA_TYPE *in, DATA_TYPE *out){
	int n2=n>>1;
	int n4=n>>2;
	DATA_TYPE *iX;
	DATA_TYPE *oX;
	LOOKUP_T *T;
	LOOKUP_T *V;
	int shift;
	int step;
	
	for (shift=6;!(n&(1<<shift));shift++);
	shift=13-shift;
	step=2<<shift;
	
	/* rotate */
	
	iX            = in+n2-7;
	oX            = out+n2+n4;
	T             = sincos_lookup0;
	
	do{
		oX-=4;
		XPROD31( iX[4], iX[6], T[0], T[1], &oX[2], &oX[3] ); T+=step;
		XPROD31( iX[0], iX[2], T[0], T[1], &oX[0], &oX[1] ); T+=step;
		iX-=8;
	}while(iX>=in+n4);
	do{
		oX-=4;
		XPROD31( iX[4], iX[6], T[1], T[0], &oX[2], &oX[3] ); T-=step;
		XPROD31( iX[0], iX[2], T[1], T[0], &oX[0], &oX[1] ); T-=step;
		iX-=8;
	}while(iX>=in);
	
	iX            = in+n2-8;
	oX            = out+n2+n4;
	T             = sincos_lookup0;
	
	do{
		T+=step; XNPROD31( iX[6], iX[4], T[0], T[1], &oX[0], &oX[1] );
		T+=step; XNPROD31( iX[2], iX[0], T[0], T[1], &oX[2], &oX[3] );
		iX-=8;
		oX+=4;
	}while(iX>=in+n4);
	do{
		T-=step; XNPROD31( iX[6], iX[4], T[1], T[0], &oX[0], &oX[1] );
		T-=step; XNPROD31( iX[2], iX[0], T[1], T[0], &oX[2], &oX[3] );
		iX-=8;
		oX+=4;
	}while(iX>=in);
	
	mdct_butterflies(out+n2,n2,shift);
	mdct_bitreverse(out,n,step,shift);
	
	/* rotate + window */
	
	step>>=2;
	{
		DATA_TYPE *oX1=out+n2+n4;
		DATA_TYPE *oX2=out+n2+n4;
		DATA_TYPE *iX =out;
		
		switch(step) {
			default: {
				T=(step>=4)?(sincos_lookup0+(step>>1)):sincos_lookup1;
				do{
					oX1-=4;
					XPROD31( iX[0], -iX[1], T[0], T[1], &oX1[3], &oX2[0] ); T+=step;
					XPROD31( iX[2], -iX[3], T[0], T[1], &oX1[2], &oX2[1] ); T+=step;
					XPROD31( iX[4], -iX[5], T[0], T[1], &oX1[1], &oX2[2] ); T+=step;
					XPROD31( iX[6], -iX[7], T[0], T[1], &oX1[0], &oX2[3] ); T+=step;
					oX2+=4;
					iX+=8;
				}while(iX<oX1);
				break;
			}
				
			case 1: {
				/* linear interpolation between table values: offset=0.5, step=1 */
				REG_TYPE  t0,t1,v0,v1;
				T         = sincos_lookup0;
				V         = sincos_lookup1;
				t0        = (*T++)>>1;
				t1        = (*T++)>>1;
				do{
					oX1-=4;
					
					t0 += (v0 = (*V++)>>1);
					t1 += (v1 = (*V++)>>1);
					XPROD31( iX[0], -iX[1], t0, t1, &oX1[3], &oX2[0] );
					v0 += (t0 = (*T++)>>1);
					v1 += (t1 = (*T++)>>1);
					XPROD31( iX[2], -iX[3], v0, v1, &oX1[2], &oX2[1] );
					t0 += (v0 = (*V++)>>1);
					t1 += (v1 = (*V++)>>1);
					XPROD31( iX[4], -iX[5], t0, t1, &oX1[1], &oX2[2] );
					v0 += (t0 = (*T++)>>1);
					v1 += (t1 = (*T++)>>1);
					XPROD31( iX[6], -iX[7], v0, v1, &oX1[0], &oX2[3] );
					
					oX2+=4;
					iX+=8;
				}while(iX<oX1);
				break;
			}
				
			case 0: {
				/* linear interpolation between table values: offset=0.25, step=0.5 */
				REG_TYPE  t0,t1,v0,v1,q0,q1;
				T         = sincos_lookup0;
				V         = sincos_lookup1;
				t0        = *T++;
				t1        = *T++;
				do{
					oX1-=4;
					
					v0  = *V++;
					v1  = *V++;
					t0 +=  (q0 = (v0-t0)>>2);
					t1 +=  (q1 = (v1-t1)>>2);
					XPROD31( iX[0], -iX[1], t0, t1, &oX1[3], &oX2[0] );
					t0  = v0-q0;
					t1  = v1-q1;
					XPROD31( iX[2], -iX[3], t0, t1, &oX1[2], &oX2[1] );
					
					t0  = *T++;
					t1  = *T++;
					v0 += (q0 = (t0-v0)>>2);
					v1 += (q1 = (t1-v1)>>2);
					XPROD31( iX[4], -iX[5], v0, v1, &oX1[1], &oX2[2] );
					v0  = t0-q0;
					v1  = t1-q1;
					XPROD31( iX[6], -iX[7], v0, v1, &oX1[0], &oX2[3] );
					
					oX2+=4;
					iX+=8;
				}while(iX<oX1);
				break;
			}
		}
		
		iX=out+n2+n4;
		oX1=out+n4;
		oX2=oX1;
		
		do{
			oX1-=4;
			iX-=4;
			
			oX2[0] = -(oX1[3] = iX[3]);
			oX2[1] = -(oX1[2] = iX[2]);
			oX2[2] = -(oX1[1] = iX[1]);
			oX2[3] = -(oX1[0] = iX[0]);
			
			oX2+=4;
		}while(oX2<iX);
		
		iX=out+n2+n4;
		oX1=out+n2+n4;
		oX2=out+n2;
		
		do{
			oX1-=4;
			oX1[0]= iX[3];
			oX1[1]= iX[2];
			oX1[2]= iX[1];
			oX1[3]= iX[0];
			iX+=4;
		}while(oX1>oX2);
	}
}


typedef struct {
	vorbis_info_residue0 *info;
	int         map;
	
	int         parts;
	int         stages;
	codebook   *fullbooks;
	codebook   *phrasebook;
	codebook ***partbooks;
	
	int         partvals;
	int       **decodemap;
	
} vorbis_look_residue0;

static void res0_free_info(vorbis_info_residue *i){
	vorbis_info_residue0 *info=(vorbis_info_residue0 *)i;
	if(info){
		memset(info,0,sizeof(*info));
		_ogg_free(info);
	}
}

static void res0_free_look(vorbis_look_residue *i){
	int j;
	if(i){
		
		vorbis_look_residue0 *look=(vorbis_look_residue0 *)i;
		
		for(j=0;j<look->parts;j++)
			if(look->partbooks[j])_ogg_free(look->partbooks[j]);
		_ogg_free(look->partbooks);
		for(j=0;j<look->partvals;j++)
			_ogg_free(look->decodemap[j]);
		_ogg_free(look->decodemap);
		
		memset(look,0,sizeof(*look));
		_ogg_free(look);
	}
}

static int ilog4(unsigned int v){
	int ret=0;
	while(v){
		ret++;
		v>>=1;
	}
	return(ret);
}

static int icount(unsigned int v){
	int ret=0;
	while(v){
		ret+=v&1;
		v>>=1;
	}
	return(ret);
}

/* vorbis_info is for range checking */
static vorbis_info_residue *res0_unpack(vorbis_info *vi,oggpack_buffer *opb){
	int j,acc=0;
	vorbis_info_residue0 *info=(vorbis_info_residue0 *)_ogg_calloc(1,sizeof(*info));
	codec_setup_info     *ci=(codec_setup_info *)vi->codec_setup;
	
	info->begin=oggpack_read(opb,24);
	info->end=oggpack_read(opb,24);
	info->grouping=oggpack_read(opb,24)+1;
	info->partitions=oggpack_read(opb,6)+1;
	info->groupbook=oggpack_read(opb,8);
	
	for(j=0;j<info->partitions;j++){
		int cascade=oggpack_read(opb,3);
		if(oggpack_read(opb,1))
			cascade|=(oggpack_read(opb,5)<<3);
		info->secondstages[j]=cascade;
		
		acc+=icount(cascade);
	}
	for(j=0;j<acc;j++)
		info->booklist[j]=oggpack_read(opb,8);
	
	if(info->groupbook>=ci->books)goto errout;
	for(j=0;j<acc;j++)
		if(info->booklist[j]>=ci->books)goto errout;
	
	return(info);
errout:
	res0_free_info(info);
	return(NULL);
}

static vorbis_look_residue *res0_look(vorbis_dsp_state *vd,vorbis_info_mode *vm,
									  vorbis_info_residue *vr){
	vorbis_info_residue0 *info=(vorbis_info_residue0 *)vr;
	vorbis_look_residue0 *look=(vorbis_look_residue0 *)_ogg_calloc(1,sizeof(*look));
	codec_setup_info     *ci=(codec_setup_info *)vd->vi->codec_setup;
	
	int j,k,acc=0;
	int dim;
	int maxstage=0;
	look->info=info;
	look->map=vm->mapping;
	
	look->parts=info->partitions;
	look->fullbooks=ci->fullbooks;
	look->phrasebook=ci->fullbooks+info->groupbook;
	dim=look->phrasebook->dim;
	
	look->partbooks=(codebook ***)_ogg_calloc(look->parts,sizeof(*look->partbooks));
	
	for(j=0;j<look->parts;j++){
		int stages=ilog4(info->secondstages[j]);
		if(stages){
			if(stages>maxstage)maxstage=stages;
			look->partbooks[j]=(codebook **)_ogg_calloc(stages,sizeof(*look->partbooks[j]));
			for(k=0;k<stages;k++)
				if(info->secondstages[j]&(1<<k)){
					look->partbooks[j][k]=ci->fullbooks+info->booklist[acc++];
#ifdef TRAIN_RES
					look->training_data[k][j]=calloc(look->partbooks[j][k]->entries,
													 sizeof(***look->training_data));
#endif
				}
		}
	}
	
	// J.CAYZAC: Replaced some divisions with 64-bit multiplies
	long long invparts = (1LL<<16) + (1LL<<32)/look->parts;
	look->partvals=look->parts;
	for(j=1;j<dim;j++)look->partvals*=look->parts;
	look->stages=maxstage;
	look->decodemap=(int **)_ogg_malloc(look->partvals*sizeof(*look->decodemap));
	for(j=0;j<look->partvals;j++){
		long val=j;
		long mult=(long)((invparts*look->partvals)>>32);
		look->decodemap[j]=(int *)_ogg_malloc(dim*sizeof(*look->decodemap[j]));
		for(k=0;k<dim;k++){
			long deco=val/mult;
			val-=deco*mult;
			mult=(long)((invparts*mult)>>32);
			look->decodemap[j][k]=deco;
		}
	}
	return(look);
}


/* a truncated packet here just means 'stop working'; it's not an error */
static int _01inverse(vorbis_block *vb,vorbis_look_residue *vl,
					  ogg_int32_t **in,int ch,
					  long (*decodepart)(codebook *, ogg_int32_t *, 
										 oggpack_buffer *,int,int)){
	
	long i,j,k,l,s;
	vorbis_look_residue0 *look=(vorbis_look_residue0 *)vl;
	vorbis_info_residue0 *info=look->info;
	
	/* move all this setup out later */
	int samples_per_partition=info->grouping;
	int partitions_per_word=look->phrasebook->dim;
	int max=vb->pcmend>>1;
	int end=(info->end<max?info->end:max);
	int n=end-info->begin;
	
	if(n>0){
		int partvals=n/samples_per_partition;
		int partwords=(partvals+partitions_per_word-1)/partitions_per_word;
		int ***partword=(int ***)alloca(ch*sizeof(*partword));
		
		for(j=0;j<ch;j++)
			partword[j]=(int **)_vorbis_block_alloc(vb,partwords*sizeof(*partword[j]));
		
		for(s=0;s<look->stages;s++){
			
			/* each loop decodes on partition codeword containing 
			 partitions_pre_word partitions */
			for(i=0,l=0;i<partvals;l++){
				if(s==0){
					/* fetch the partition word for each channel */
					for(j=0;j<ch;j++){
						int temp=vorbis_book_decode(look->phrasebook,&vb->opb);
						if(temp==-1)goto eopbreak;
						partword[j][l]=look->decodemap[temp];
						if(partword[j][l]==NULL)goto errout;
					}
				}
				
				/* now we decode residual values for the partitions */
				for(k=0;k<partitions_per_word && i<partvals;k++,i++)
					for(j=0;j<ch;j++){
						long offset=info->begin+i*samples_per_partition;
						if(info->secondstages[partword[j][l][k]]&(1<<s)){
							codebook *stagebook=look->partbooks[partword[j][l][k]][s];
							if(stagebook){
								if(decodepart(stagebook,in[j]+offset,&vb->opb,
											  samples_per_partition,-8)==-1)goto eopbreak;
							}
						}
					}
			} 
		}
	}
errout:
eopbreak:
	return(0);
}

static int res0_inverse(vorbis_block *vb,vorbis_look_residue *vl,
						ogg_int32_t **in,int *nonzero,int ch){
	int i,used=0;
	for(i=0;i<ch;i++)
		if(nonzero[i])
			in[used++]=in[i];
	if(used)
		return(_01inverse(vb,vl,in,used,vorbis_book_decodevs_add));
	else
		return(0);
}

static int res1_inverse(vorbis_block *vb,vorbis_look_residue *vl,
						ogg_int32_t **in,int *nonzero,int ch){
	int i,used=0;
	for(i=0;i<ch;i++)
		if(nonzero[i])
			in[used++]=in[i];
	if(used)
		return(_01inverse(vb,vl,in,used,vorbis_book_decodev_add));
	else
		return(0);
}

/* duplicate code here as speed is somewhat more important */
static int res2_inverse(vorbis_block *vb,vorbis_look_residue *vl,
						ogg_int32_t **in,int *nonzero,int ch){
	long i,k,l,s;
	vorbis_look_residue0 *look=(vorbis_look_residue0 *)vl;
	vorbis_info_residue0 *info=look->info;
	
	/* move all this setup out later */
	int samples_per_partition=info->grouping;
	int partitions_per_word=look->phrasebook->dim;
	int max=(vb->pcmend*ch)>>1;
	int end=(info->end<max?info->end:max);
	int n=end-info->begin;
	
	if(n>0){
		
		int partvals=n/samples_per_partition;
		int partwords=(partvals+partitions_per_word-1)/partitions_per_word;
		int **partword=(int **)_vorbis_block_alloc(vb,partwords*sizeof(*partword));
		int beginoff=info->begin/ch;
		
		for(i=0;i<ch;i++)if(nonzero[i])break;
		if(i==ch)return(0); /* no nonzero vectors */
		
		samples_per_partition/=ch;
		
		for(s=0;s<look->stages;s++){
			for(i=0,l=0;i<partvals;l++){
				
				if(s==0){
					/* fetch the partition word */
					int temp=vorbis_book_decode(look->phrasebook,&vb->opb);
					if(temp==-1)goto eopbreak;
					partword[l]=look->decodemap[temp];
					if(partword[l]==NULL)goto errout;
				}
				
				/* now we decode residual values for the partitions */
				for(k=0;k<partitions_per_word && i<partvals;k++,i++)
					if(info->secondstages[partword[l][k]]&(1<<s)){
						codebook *stagebook=look->partbooks[partword[l][k]][s];
						
						if(stagebook){
							if(vorbis_book_decodevv_add(stagebook,in,
														i*samples_per_partition+beginoff,ch,
														&vb->opb,
														samples_per_partition,-8)==-1)
								goto eopbreak;
						}
					}
			} 
		}
	}
errout:
eopbreak:
	return(0);
}


vorbis_func_residue residue0_exportbundle={
&res0_unpack,
&res0_look,
&res0_free_info,
&res0_free_look,
&res0_inverse
};

vorbis_func_residue residue1_exportbundle={
&res0_unpack,
&res0_look,
&res0_free_info,
&res0_free_look,
&res1_inverse
};

vorbis_func_residue residue2_exportbundle={
&res0_unpack,
&res0_look,
&res0_free_info,
&res0_free_look,
&res2_inverse
};

/**** pack/unpack helpers ******************************************/

/* 32 bit float (not IEEE; nonnormalized mantissa +
 biased exponent) : neeeeeee eeemmmmm mmmmmmmm mmmmmmmm 
 Why not IEEE?  It's just not that important here. */

#define VQ_FEXP 10
#define VQ_FMAN 21
#define VQ_FEXP_BIAS 768 /* bias toward values smaller than 1. */

static ogg_int32_t _float32_unpack(long val,int *point){
	long   mant=val&0x1fffff;
	int    sign=val&0x80000000;
	long   exp =(val&0x7fe00000L)>>VQ_FMAN;
	
	exp-=(VQ_FMAN-1)+VQ_FEXP_BIAS;
	
	if(mant){
		while(!(mant&0x40000000)){
			mant<<=1;
			exp-=1;
		}
		
		if(sign)mant= -mant;
	}else{
		sign=0;
		exp=-9999;
	}
	
	*point=exp;
	return mant;
}

/* given a list of word lengths, generate a list of codewords.  Works
 for length ordered or unordered, always assigns the lowest valued
 codewords first.  Extended to handle unused entries (length 0) */
static ogg_uint32_t *_make_words(long *l,long n,long sparsecount){
	long i,j,count=0;
	ogg_uint32_t marker[33];
	ogg_uint32_t *r=(ogg_uint32_t *)_ogg_malloc((sparsecount?sparsecount:n)*sizeof(*r));
	memset(marker,0,sizeof(marker));
	
	for(i=0;i<n;i++){
		long length=l[i];
		if(length>0){
			ogg_uint32_t entry=marker[length];
			
			/* when we claim a node for an entry, we also claim the nodes
			 below it (pruning off the imagined tree that may have dangled
			 from it) as well as blocking the use of any nodes directly
			 above for leaves */
			
			/* update ourself */
			if(length<32 && (entry>>length)){
				/* error condition; the lengths must specify an overpopulated tree */
				_ogg_free(r);
				return(NULL);
			}
			r[count++]=entry;
			
			/* Look to see if the next shorter marker points to the node
			 above. if so, update it and repeat.  */
			{
				for(j=length;j>0;j--){
					
					if(marker[j]&1){
						/* have to jump branches */
						if(j==1)
							marker[1]++;
						else
							marker[j]=marker[j-1]<<1;
						break; /* invariant says next upper marker would already
						 have been moved if it was on the same path */
					}
					marker[j]++;
				}
			}
			
			/* prune the tree; the implicit invariant says all the longer
			 markers were dangling from our just-taken node.  Dangle them
			 from our *new* node. */
			for(j=length+1;j<33;j++)
				if((marker[j]>>1) == entry){
					entry=marker[j];
					marker[j]=marker[j-1]<<1;
				}else
					break;
		}else
			if(sparsecount==0)count++;
	}
    
	/* bitreverse the words because our bitwise packer/unpacker is LSb
     endian */
	for(i=0,count=0;i<n;i++){
		ogg_uint32_t temp=0;
		for(j=0;j<l[i];j++){
			temp<<=1;
			temp|=(r[count]>>j)&1;
		}
		
		if(sparsecount){
			if(l[i])
				r[count++]=temp;
		}else
			r[count++]=temp;
	}
	
	return(r);
}

/* there might be a straightforward one-line way to do the below
 that's portable and totally safe against roundoff, but I haven't
 thought of it.  Therefore, we opt on the side of caution */
long _book_maptype1_quantvals(const static_codebook *b){
	/* get us a starting hint, we'll polish it below */
	int bits=_ilog(b->entries);
	int vals=b->entries>>((bits-1)*(b->dim-1)/b->dim);
	
	while(1){
		long acc=1;
		long acc1=1;
		int i;
		for(i=0;i<b->dim;i++){
			acc*=vals;
			acc1*=vals+1;
		}
		if(acc<=b->entries && acc1>b->entries){
			return(vals);
		}else{
			if(acc>b->entries){
				vals--;
			}else{
				vals++;
			}
		}
	}
}

/* different than what _book_unquantize does for mainline:
 we repack the book in a fixed point format that shares the same
 binary point.  Upon first use, we can shift point if needed */

/* we need to deal with two map types: in map type 1, the values are
 generated algorithmically (each column of the vector counts through
 the values in the quant vector). in map type 2, all the values came
 in in an explicit list.  Both value lists must be unpacked */

static ogg_int32_t *_book_unquantize(const static_codebook *b,int n,int *sparsemap,
									 int *maxpoint){
	long j,k,count=0;
	if(b->maptype==1 || b->maptype==2){
		int quantvals;
		int minpoint,delpoint;
		ogg_int32_t mindel=_float32_unpack(b->q_min,&minpoint);
		ogg_int32_t delta=_float32_unpack(b->q_delta,&delpoint);
		ogg_int32_t *r=(ogg_int32_t *)_ogg_calloc(n*b->dim,sizeof(*r));
		int *rp=(int *)_ogg_calloc(n*b->dim,sizeof(*rp));
		
		*maxpoint=minpoint;
		
		/* maptype 1 and 2 both use a quantized value vector, but
		 different sizes */
		switch(b->maptype){
			case 1:
				/* most of the time, entries%dimensions == 0, but we need to be
				 well defined.  We define that the possible vales at each
				 scalar is values == entries/dim.  If entries%dim != 0, we'll
				 have 'too few' values (values*dim<entries), which means that
				 we'll have 'left over' entries; left over entries use zeroed
				 values (and are wasted).  So don't generate codebooks like
				 that */
				quantvals=_book_maptype1_quantvals(b);
				for(j=0;j<b->entries;j++){
					if((sparsemap && b->lengthlist[j]) || !sparsemap){
						ogg_int32_t last=0;
						int lastpoint=0;
						int indexdiv=1;
						for(k=0;k<b->dim;k++){
							int index= (j/indexdiv)%quantvals;
							int point=0;
							int val=VFLOAT_MULTI(delta,delpoint,
												 abs(b->quantlist[index]),&point);
							
							val=VFLOAT_ADD(mindel,minpoint,val,point,&point);
							val=VFLOAT_ADD(last,lastpoint,val,point,&point);
							
							if(b->q_sequencep){
								last=val;	  
								lastpoint=point;
							}
							
							if(sparsemap){
								r[sparsemap[count]*b->dim+k]=val;
								rp[sparsemap[count]*b->dim+k]=point;
							}else{
								r[count*b->dim+k]=val;
								rp[count*b->dim+k]=point;
							}
							if(*maxpoint<point)*maxpoint=point;
							indexdiv*=quantvals;
						}
						count++;
					}
					
				}
				break;
			case 2:
				for(j=0;j<b->entries;j++){
					if((sparsemap && b->lengthlist[j]) || !sparsemap){
						ogg_int32_t last=0;
						int         lastpoint=0;
						
						for(k=0;k<b->dim;k++){
							int point=0;
							int val=VFLOAT_MULTI(delta,delpoint,
												 abs(b->quantlist[j*b->dim+k]),&point);
							
							val=VFLOAT_ADD(mindel,minpoint,val,point,&point);
							val=VFLOAT_ADD(last,lastpoint,val,point,&point);
							
							if(b->q_sequencep){
								last=val;	  
								lastpoint=point;
							}
							
							if(sparsemap){
								r[sparsemap[count]*b->dim+k]=val;
								rp[sparsemap[count]*b->dim+k]=point;
							}else{
								r[count*b->dim+k]=val;
								rp[count*b->dim+k]=point;
							}
							if(*maxpoint<point)*maxpoint=point;
						}
						count++;
					}
				}
				break;
		}
		
		for(j=0;j<n*b->dim;j++)
			if(rp[j]<*maxpoint)
				r[j]>>=*maxpoint-rp[j];
	    
		_ogg_free(rp);
		return(r);
	}
	return(NULL);
}

void vorbis_staticbook_clear(static_codebook *b){
	if(b->quantlist)_ogg_free(b->quantlist);
	if(b->lengthlist)_ogg_free(b->lengthlist);
	memset(b,0,sizeof(*b));
	
}

void vorbis_staticbook_destroy(static_codebook *b){
	vorbis_staticbook_clear(b);
	_ogg_free(b);
}

void vorbis_book_clear(codebook *b){
	/* static book is not cleared; we're likely called on the lookup and
     the static codebook belongs to the info struct */
	if(b->valuelist)_ogg_free(b->valuelist);
	if(b->codelist)_ogg_free(b->codelist);
	
	if(b->dec_index)_ogg_free(b->dec_index);
	if(b->dec_codelengths)_ogg_free(b->dec_codelengths);
	if(b->dec_firsttable)_ogg_free(b->dec_firsttable);
	
	memset(b,0,sizeof(*b));
}

static int sort32a(const void *a,const void *b){
	return (**(const ogg_uint32_t * const *)a>**(const ogg_uint32_t * const *)b)-
    (**(const ogg_uint32_t * const *)a<**(const ogg_uint32_t * const *)b);
}

/* decode codebook arrangement is more heavily optimized than encode */
int vorbis_book_init_decode(codebook *c,const static_codebook *s){
	int i,j,n=0,tabn;
	int *sortindex;
	memset(c,0,sizeof(*c));
	
	/* count actually used entries */
	for(i=0;i<s->entries;i++)
		if(s->lengthlist[i]>0)
			n++;
	
	c->entries=s->entries;
	c->used_entries=n;
	c->dim=s->dim;
	
	if(n>0){
		/* two different remappings go on here.  
		 
		 First, we collapse the likely sparse codebook down only to
		 actually represented values/words.  This collapsing needs to be
		 indexed as map-valueless books are used to encode original entry
		 positions as integers.
		 
		 Second, we reorder all vectors, including the entry index above,
		 by sorted bitreversed codeword to allow treeless decode. */
		
		/* perform sort */
		ogg_uint32_t *codes=_make_words(s->lengthlist,s->entries,c->used_entries);
		ogg_uint32_t **codep=(ogg_uint32_t **)alloca(sizeof(*codep)*n);
		
		if(codes==NULL)goto err_out;
		
		for(i=0;i<n;i++){
			codes[i]=bitreverse(codes[i]);
			codep[i]=codes+i;
		}
		
		qsort(codep,n,sizeof(*codep),sort32a);
		
		sortindex=(int *)alloca(n*sizeof(*sortindex));
		c->codelist=(ogg_uint32_t *)_ogg_malloc(n*sizeof(*c->codelist));
		/* the index is a reverse index */
		for(i=0;i<n;i++){
			int position=codep[i]-codes;
			sortindex[position]=i;
		}
		
		for(i=0;i<n;i++)
			c->codelist[sortindex[i]]=codes[i];
		_ogg_free(codes);
		
		
		
		c->valuelist=_book_unquantize(s,n,sortindex,&c->binarypoint);
		c->dec_index=(int *)_ogg_malloc(n*sizeof(*c->dec_index));
		
		for(n=0,i=0;i<s->entries;i++)
			if(s->lengthlist[i]>0)
				c->dec_index[sortindex[n++]]=i;
		
		c->dec_codelengths=(char *)_ogg_malloc(n*sizeof(*c->dec_codelengths));
		for(n=0,i=0;i<s->entries;i++)
			if(s->lengthlist[i]>0)
				c->dec_codelengths[sortindex[n++]]=s->lengthlist[i];
		
		c->dec_firsttablen=_ilog(c->used_entries)-4; /* this is magic */
		if(c->dec_firsttablen<5)c->dec_firsttablen=5;
		if(c->dec_firsttablen>8)c->dec_firsttablen=8;
		
		tabn=1<<c->dec_firsttablen;
		c->dec_firsttable=(ogg_uint32_t *)_ogg_calloc(tabn,sizeof(*c->dec_firsttable));
		c->dec_maxlength=0;
		
		for(i=0;i<n;i++){
			if(c->dec_maxlength<c->dec_codelengths[i])
				c->dec_maxlength=c->dec_codelengths[i];
			if(c->dec_codelengths[i]<=c->dec_firsttablen){
				ogg_uint32_t orig=bitreverse(c->codelist[i]);
				for(j=0;j<(1<<(c->dec_firsttablen-c->dec_codelengths[i]));j++)
					c->dec_firsttable[orig|(j<<c->dec_codelengths[i])]=i+1;
			}
		}
		
		/* now fill in 'unused' entries in the firsttable with hi/lo search
		 hints for the non-direct-hits */
		{
			ogg_uint32_t mask=0xfffffffeUL<<(31-c->dec_firsttablen);
			long lo=0,hi=0;
			
			for(i=0;i<tabn;i++){
				ogg_uint32_t word=i<<(32-c->dec_firsttablen);
				if(c->dec_firsttable[bitreverse(word)]==0){
					while((lo+1)<n && c->codelist[lo+1]<=word)lo++;
					while(    hi<n && word>=(c->codelist[hi]&mask))hi++;
					
					/* we only actually have 15 bits per hint to play with here.
					 In order to overflow gracefully (nothing breaks, efficiency
					 just drops), encode as the difference from the extremes. */
					{
						unsigned long loval=lo;
						unsigned long hival=n-hi;
						
						if(loval>0x7fff)loval=0x7fff;
						if(hival>0x7fff)hival=0x7fff;
						c->dec_firsttable[bitreverse(word)]=
						0x80000000UL | (loval<<15) | hival;
					}
				}
			}
		}
	}
	
	return(0);
err_out:
	vorbis_book_clear(c);
	return(-1);
}

int vorbis_synthesis(vorbis_block *vb,ogg_packet *op,int decodep){
	vorbis_dsp_state     *vd=vb->vd;
	private_state        *b=(private_state *)vd->backend_state;
	vorbis_info          *vi=vd->vi;
	codec_setup_info     *ci=(codec_setup_info *)vi->codec_setup;
	oggpack_buffer       *opb=&vb->opb;
	int                   type,mode,i;
	
	/* first things first.  Make sure decode is ready */
	_vorbis_block_ripcord(vb);
	oggpack_readinit(opb,op->packet);
	
	/* Check the packet type */
	if(oggpack_read(opb,1)!=0){
		/* Oops.  This is not an audio data packet */
		return(OV_ENOTAUDIO);
	}
	
	/* read our mode and pre/post windowsize */
	mode=oggpack_read(opb,b->modebits);
	if(mode==-1)return(OV_EBADPACKET);
	
	vb->mode=mode;
	vb->W=ci->mode_param[mode]->blockflag;
	if(vb->W){
		vb->lW=oggpack_read(opb,1);
		vb->nW=oggpack_read(opb,1);
		if(vb->nW==-1)   return(OV_EBADPACKET);
	}else{
		vb->lW=0;
		vb->nW=0;
	}
	
	/* more setup */
	vb->granulepos=op->granulepos;
	vb->sequence=op->packetno-3; /* first block is third packet */
	vb->eofflag=op->e_o_s;
	
	if(decodep){
		/* alloc pcm passback storage */
		vb->pcmend=ci->blocksizes[vb->W];
		vb->pcm=(ogg_int32_t **)_vorbis_block_alloc(vb,sizeof(*vb->pcm)*vi->channels);
		for(i=0;i<vi->channels;i++)
			vb->pcm[i]=(ogg_int32_t *)_vorbis_block_alloc(vb,vb->pcmend*sizeof(*vb->pcm[i]));
		
		/* unpack_header enforces range checking */
		type=ci->map_type[ci->mode_param[mode]->mapping];
		
		return(_mapping_P[type]->inverse(vb,b->mode[mode]));
	}else{
		/* no pcm */
		vb->pcmend=0;
		vb->pcm=NULL;
		
		return(0);
	}
}

long vorbis_packet_blocksize(vorbis_info *vi,ogg_packet *op){
	codec_setup_info     *ci=(codec_setup_info *)vi->codec_setup;
	oggpack_buffer       opb;
	int                  mode;
	
	oggpack_readinit(&opb,op->packet);
	
	/* Check the packet type */
	if(oggpack_read(&opb,1)!=0){
		/* Oops.  This is not an audio data packet */
		return(OV_ENOTAUDIO);
	}
	
	{
		int modebits=0;
		int v=ci->modes;
		while(v>1){
			modebits++;
			v>>=1;
		}
		
		/* read our mode and pre/post windowsize */
		mode=oggpack_read(&opb,modebits);
	}
	if(mode==-1)return(OV_EBADPACKET);
	return(ci->blocksizes[ci->mode_param[mode]->blockflag]);
}

/* A 'chained bitstream' is a Vorbis bitstream that contains more than
 one logical bitstream arranged end to end (the only form of Ogg
 multiplexing allowed in a Vorbis bitstream; grouping [parallel
 multiplexing] is not allowed in Vorbis) */

/* A Vorbis file can be played beginning to end (streamed) without
 worrying ahead of time about chaining (see decoder_example.c).  If
 we have the whole file, however, and want random access
 (seeking/scrubbing) or desire to know the total length/time of a
 file, we need to account for the possibility of chaining. */

/* We can handle things a number of ways; we can determine the entire
 bitstream structure right off the bat, or find pieces on demand.
 This example determines and caches structure for the entire
 bitstream, but builds a virtual decoder on the fly when moving
 between links in the chain. */

/* There are also different ways to implement seeking.  Enough
 information exists in an Ogg bitstream to seek to
 sample-granularity positions in the output.  Or, one can seek by
 picking some portion of the stream roughly in the desired area if
 we only want coarse navigation through the stream. */

/*************************************************************************
 * Many, many internal helpers.  The intention is not to be confusing; 
 * rampant duplication and monolithic function implementation would be 
 * harder to understand anyway.  The high level functions are last.  Begin
 * grokking near the end of the file */


/* read a little more data from the file/pipe into the ogg_sync framer */
static long _get_data(OggVorbis_File *vf){
	errno=0;
	if(vf->datasource){
		unsigned char *buffer=ogg_sync_bufferin(vf->oy,CHUNKSIZE);
		long bytes=(vf->callbacks.read_func)(buffer,1,CHUNKSIZE,vf->datasource);
		if(bytes>0)ogg_sync_wrote(vf->oy,bytes);
		if(bytes==0 && errno)return(-1);
		return(bytes);
	}else
		return(0);
}

/* save a tiny smidge of verbosity to make the code more readable */
static void _seek_helper(OggVorbis_File *vf,ogg_int64_t offset){
	if(vf->datasource){ 
		(vf->callbacks.seek_func)(vf->datasource, offset, SEEK_SET);
		vf->offset=offset;
		ogg_sync_reset(vf->oy);
	}else{
		/* shouldn't happen unless someone writes a broken callback */
		return;
	}
}

/* The read/seek functions track absolute position within the stream */

/* from the head of the stream, get the next page.  boundary specifies
 if the function is allowed to fetch more data from the stream (and
 how much) or only use internally buffered data.
 
 boundary: -1) unbounded search
 0) read no additional data; use cached only
 n) search for a new page beginning for n bytes
 
 return:   <0) did not find a page (OV_FALSE, OV_EOF, OV_EREAD)
 n) found a page at absolute offset n 
 
 produces a refcounted page */

static ogg_int64_t _get_next_page(OggVorbis_File *vf,ogg_page *og,
								  ogg_int64_t boundary){
	if(boundary>0)boundary+=vf->offset;
	while(1){
		long more;
		
		if(boundary>0 && vf->offset>=boundary)return(OV_FALSE);
		more=ogg_sync_pageseek(vf->oy,og);
		
		if(more<0){
			/* skipped n bytes */
			vf->offset-=more;
		}else{
			if(more==0){
				/* send more paramedics */
				if(!boundary)return(OV_FALSE);
				{
					long ret=_get_data(vf);
					if(ret==0)return(OV_EOF);
					if(ret<0)return(OV_EREAD);
				}
			}else{
				/* got a page.  Return the offset at the page beginning,
				 advance the internal offset past the page end */
				ogg_int64_t ret=vf->offset;
				vf->offset+=more;
				return(ret);
				
			}
		}
	}
}

/* find the latest page beginning before the current stream cursor
 position. Much dirtier than the above as Ogg doesn't have any
 backward search linkage.  no 'readp' as it will certainly have to
 read. */
/* returns offset or OV_EREAD, OV_FAULT and produces a refcounted page */

static ogg_int64_t _get_prev_page(OggVorbis_File *vf,ogg_page *og){
	ogg_int64_t begin=vf->offset;
	ogg_int64_t end=begin;
	ogg_int64_t ret;
	ogg_int64_t offset=-1;
	
	while(offset==-1){
		begin-=CHUNKSIZE;
		if(begin<0)
			begin=0;
		_seek_helper(vf,begin);
		while(vf->offset<end){
			ret=_get_next_page(vf,og,end-vf->offset);
			if(ret==OV_EREAD)return(OV_EREAD);
			if(ret<0){
				break;
			}else{
				offset=ret;
			}
		}
	}
	
	/* we have the offset.  Actually snork and hold the page now */
	_seek_helper(vf,offset);
	ret=_get_next_page(vf,og,CHUNKSIZE);
	if(ret<0)
    /* this shouldn't be possible */
		return(OV_EFAULT);
	
	return(offset);
}

/* finds each bitstream link one at a time using a bisection search
 (has to begin by knowing the offset of the lb's initial page).
 Recurses for each link so it can alloc the link storage after
 finding them all, then unroll and fill the cache at the same time */
static int _bisect_forward_serialno(OggVorbis_File *vf,
									ogg_int64_t begin,
									ogg_int64_t searched,
									ogg_int64_t end,
									ogg_uint32_t currentno,
									long m){
	ogg_int64_t endsearched=end;
	ogg_int64_t next=end;
	ogg_page og={0,0,0,0};
	ogg_int64_t ret;
	
	/* the below guards against garbage seperating the last and
     first pages of two links. */
	while(searched<endsearched){
		ogg_int64_t bisect;
		
		if(endsearched-searched<CHUNKSIZE){
			bisect=searched;
		}else{
			bisect=(searched+endsearched)/2;
		}
		
		_seek_helper(vf,bisect);
		ret=_get_next_page(vf,&og,-1);
		if(ret==OV_EREAD)return(OV_EREAD);
		if(ret<0 || ogg_page_serialno(&og)!=currentno){
			endsearched=bisect;
			if(ret>=0)next=ret;
		}else{
			searched=ret+og.header_len+og.body_len;
		}
		ogg_page_release(&og);
	}
	
	_seek_helper(vf,next);
	ret=_get_next_page(vf,&og,-1);
	if(ret==OV_EREAD)return(OV_EREAD);
	
	if(searched>=end || ret<0){
		ogg_page_release(&og);
		vf->links=m+1;
		vf->offsets=_ogg_malloc((vf->links+1)*sizeof(*vf->offsets));
		vf->serialnos=_ogg_malloc(vf->links*sizeof(*vf->serialnos));
		vf->offsets[m+1]=searched;
	}else{
		ret=_bisect_forward_serialno(vf,next,vf->offset,
									 end,ogg_page_serialno(&og),m+1);
		ogg_page_release(&og);
		if(ret==OV_EREAD)return(OV_EREAD);
	}
	
	vf->offsets[m]=begin;
	vf->serialnos[m]=currentno;
	return(0);
}

/* uses the local ogg_stream storage in vf; this is important for
 non-streaming input sources */
/* consumes the page that's passed in (if any) */

static int _fetch_headers(OggVorbis_File *vf,
						  vorbis_info *vi,
						  vorbis_comment *vc,
						  ogg_uint32_t *serialno,
						  ogg_page *og_ptr){
	ogg_page og={0,0,0,0};
	ogg_packet op={0,0,0,0,0,0};
	int i,ret;
	
	if(!og_ptr){
		ogg_int64_t llret=_get_next_page(vf,&og,CHUNKSIZE);
		if(llret==OV_EREAD)return(OV_EREAD);
		if(llret<0)return OV_ENOTVORBIS;
		og_ptr=&og;
	}
	
	ogg_stream_reset_serialno(vf->os,ogg_page_serialno(og_ptr));
	if(serialno)*serialno=vf->os->serialno;
	vf->ready_state=STREAMSET;
	
	/* extract the initial header from the first page and verify that the
     Ogg bitstream is in fact Vorbis data */
	
	vorbis_info_init(vi);
	vorbis_comment_init(vc);
	
	i=0;
	while(i<3){
		ogg_stream_pagein(vf->os,og_ptr);
		while(i<3){
			int result=ogg_stream_packetout(vf->os,&op);
			if(result==0)break;
			if(result==-1){
				ret=OV_EBADHEADER;
				goto bail_header;
			}
			if((ret=vorbis_synthesis_headerin(vi,vc,&op))){
				goto bail_header;
			}
			i++;
		}
		if(i<3)
			if(_get_next_page(vf,og_ptr,CHUNKSIZE)<0){
				ret=OV_EBADHEADER;
				goto bail_header;
			}
	}
	
	ogg_packet_release(&op);
	ogg_page_release(&og);
	return 0; 
	
bail_header:
	ogg_packet_release(&op);
	ogg_page_release(&og);
	vorbis_info_clear(vi);
	vorbis_comment_clear(vc);
	vf->ready_state=OPENED;
	
	return ret;
}

/* last step of the OggVorbis_File initialization; get all the
 vorbis_info structs and PCM positions.  Only called by the seekable
 initialization (local stream storage is hacked slightly; pay
 attention to how that's done) */

/* this is void and does not propogate errors up because we want to be
 able to open and use damaged bitstreams as well as we can.  Just
 watch out for missing information for links in the OggVorbis_File
 struct */
static void _prefetch_all_headers(OggVorbis_File *vf, ogg_int64_t dataoffset){
	ogg_page og={0,0,0,0};
	int i;
	ogg_int64_t ret;
	
	vf->vi=_ogg_realloc(vf->vi,vf->links*sizeof(*vf->vi));
	vf->vc=_ogg_realloc(vf->vc,vf->links*sizeof(*vf->vc));
	vf->dataoffsets=_ogg_malloc(vf->links*sizeof(*vf->dataoffsets));
	vf->pcmlengths=_ogg_malloc(vf->links*2*sizeof(*vf->pcmlengths));
	
	for(i=0;i<vf->links;i++){
		if(i==0){
			/* we already grabbed the initial header earlier.  Just set the offset */
			vf->dataoffsets[i]=dataoffset;
			_seek_helper(vf,dataoffset);
			
		}else{
			
			/* seek to the location of the initial header */
			
			_seek_helper(vf,vf->offsets[i]);
			if(_fetch_headers(vf,vf->vi+i,vf->vc+i,NULL,NULL)<0){
				vf->dataoffsets[i]=-1;
			}else{
				vf->dataoffsets[i]=vf->offset;
			}
		}
		
		/* fetch beginning PCM offset */
		
		if(vf->dataoffsets[i]!=-1){
			ogg_int64_t accumulated=0,pos;
			long        lastblock=-1;
			int         result;
			
			ogg_stream_reset_serialno(vf->os,vf->serialnos[i]);
			
			while(1){
				ogg_packet op={0,0,0,0,0,0};
				
				ret=_get_next_page(vf,&og,-1);
				if(ret<0)
				/* this should not be possible unless the file is
				 truncated/mangled */
					break;
				
				if(ogg_page_serialno(&og)!=vf->serialnos[i])
					break;
				
				pos=ogg_page_granulepos(&og);
				
				/* count blocksizes of all frames in the page */
				ogg_stream_pagein(vf->os,&og);
				while((result=ogg_stream_packetout(vf->os,&op))){
					if(result>0){ /* ignore holes */
						long thisblock=vorbis_packet_blocksize(vf->vi+i,&op);
						if(lastblock!=-1)
							accumulated+=(lastblock+thisblock)>>2;
						lastblock=thisblock;
					}
				}
				ogg_packet_release(&op);
				
				if(pos!=-1){
					/* pcm offset of last packet on the first audio page */
					accumulated= pos-accumulated;
					break;
				}
			}
			
			/* less than zero?  This is a stream with samples trimmed off
			 the beginning, a normal occurrence; set the offset to zero */
			if(accumulated<0)accumulated=0;
			
			vf->pcmlengths[i*2]=accumulated;
		}
		
		/* get the PCM length of this link. To do this,
		 get the last page of the stream */
		{
			ogg_int64_t end=vf->offsets[i+1];
			_seek_helper(vf,end);
			
			while(1){
				ret=_get_prev_page(vf,&og);
				if(ret<0){
					/* this should not be possible */
					vorbis_info_clear(vf->vi+i);
					vorbis_comment_clear(vf->vc+i);
					break;
				}
				if(ogg_page_granulepos(&og)!=-1){
					vf->pcmlengths[i*2+1]=ogg_page_granulepos(&og)-vf->pcmlengths[i*2];
					break;
				}
				vf->offset=ret;
			}
		}
	}
	ogg_page_release(&og);
}

static void _make_decode_ready(OggVorbis_File *vf){
	if(vf->ready_state!=STREAMSET)return;
	if(vf->seekable){
		vorbis_synthesis_init(&vf->vd,vf->vi+vf->current_link);
	}else{
		vorbis_synthesis_init(&vf->vd,vf->vi);
	}    
	vorbis_block_init(&vf->vd,&vf->vb);
	vf->ready_state=INITSET;
	vf->bittrack=0;
	vf->samptrack=0;
	return;
}

static int _open_seekable2(OggVorbis_File *vf){
	ogg_uint32_t serialno=vf->current_serialno;
	ogg_uint32_t tempserialno;
	ogg_int64_t dataoffset=vf->offset, end;
	ogg_page og={0,0,0,0};
	
	/* we're partially open and have a first link header state in
     storage in vf */
	/* we can seek, so set out learning all about this file */
	(vf->callbacks.seek_func)(vf->datasource,0,SEEK_END);
	vf->offset=vf->end=(vf->callbacks.tell_func)(vf->datasource);
	
	/* We get the offset for the last page of the physical bitstream.
     Most OggVorbis files will contain a single logical bitstream */
	end=_get_prev_page(vf,&og);
	if(end<0)return((int)end);
	
	/* more than one logical bitstream? */
	tempserialno=ogg_page_serialno(&og);
	ogg_page_release(&og);
	
	if(tempserialno!=serialno){
		
		/* Chained bitstream. Bisect-search each logical bitstream
		 section.  Do so based on serial number only */
		if(_bisect_forward_serialno(vf,0,0,end+1,serialno,0)<0)return(OV_EREAD);
		
	}else{
		
		/* Only one logical bitstream */
		if(_bisect_forward_serialno(vf,0,end,end+1,serialno,0))return(OV_EREAD);
		
	}
	
	/* the initial header memory is referenced by vf after; don't free it */
	_prefetch_all_headers(vf,dataoffset);
	return(ov_raw_seek(vf,0));
}

/* clear out the current logical bitstream decoder */ 
static void _decode_clear(OggVorbis_File *vf){
	vorbis_dsp_clear(&vf->vd);
	vorbis_block_clear(&vf->vb);
	vf->ready_state=OPENED;
}

/* fetch and process a packet.  Handles the case where we're at a
 bitstream boundary and dumps the decoding machine.  If the decoding
 machine is unloaded, it loads it.  It also keeps pcm_offset up to
 date (seek and read both use this.  seek uses a special hack with
 readp). 
 
 return: <0) error, OV_HOLE (lost packet) or OV_EOF
 0) need more data (only if readp==0)
 1) got a packet 
 */

static int _fetch_and_process_packet(OggVorbis_File *vf,
									 int readp,
									 int spanp){
	ogg_page og={0,0,0,0};
	ogg_packet op={0,0,0,0,0,0};
	int ret=0;
	
	/* handle one packet.  Try to fetch it from current stream state */
	/* extract packets from page */
	while(1){
		
		/* process a packet if we can.  If the machine isn't loaded,
		 neither is a page */
		if(vf->ready_state==INITSET){
			while(1) {
				int result=ogg_stream_packetout(vf->os,&op);
				ogg_int64_t granulepos;
				
				if(result<0){
					ret=OV_HOLE; /* hole in the data. */
					goto cleanup;
				}
				if(result>0){
					/* got a packet.  process it */
					granulepos=op.granulepos;
					if(!vorbis_synthesis(&vf->vb,&op,1)){ /* lazy check for lazy
					 header handling.  The
					 header packets aren't
					 audio, so if/when we
					 submit them,
					 vorbis_synthesis will
					 reject them */
						
						/* suck in the synthesis data and track bitrate */
						{
							int oldsamples=vorbis_synthesis_pcmout(&vf->vd,NULL);
							/* for proper use of libvorbis within libvorbisfile,
							 oldsamples will always be zero. */
							if(oldsamples){
								ret=OV_EFAULT;
								goto cleanup;
							}
							
							vorbis_synthesis_blockin(&vf->vd,&vf->vb);
							vf->samptrack+=vorbis_synthesis_pcmout(&vf->vd,NULL)-oldsamples;
							vf->bittrack+=op.bytes*8;
						}
						
						/* update the pcm offset. */
						if(granulepos!=-1 && !op.e_o_s){
							int link=(vf->seekable?vf->current_link:0);
							int i,samples;
							
							/* this packet has a pcm_offset on it (the last packet
							 completed on a page carries the offset) After processing
							 (above), we know the pcm position of the *last* sample
							 ready to be returned. Find the offset of the *first*
							 
							 As an aside, this trick is inaccurate if we begin
							 reading anew right at the last page; the end-of-stream
							 granulepos declares the last frame in the stream, and the
							 last packet of the last page may be a partial frame.
							 So, we need a previous granulepos from an in-sequence page
							 to have a reference point.  Thus the !op.e_o_s clause
							 above */
							
							if(vf->seekable && link>0)
								granulepos-=vf->pcmlengths[link*2];
							if(granulepos<0)granulepos=0; /* actually, this
							 shouldn't be possible
							 here unless the stream
							 is very broken */
							
							samples=vorbis_synthesis_pcmout(&vf->vd,NULL);
							
							granulepos-=samples;
							for(i=0;i<link;i++)
								granulepos+=vf->pcmlengths[i*2+1];
							vf->pcm_offset=granulepos;
						}
						ret=1;
						goto cleanup;
					}
				}
				else 
					break;
			}
		}
		
		if(vf->ready_state>=OPENED){
			int ret;
			if(!readp){
				ret=0;
				goto cleanup;
			}
			if((ret=(int)_get_next_page(vf,&og,-1))<0){
				ret=OV_EOF; /* eof. leave unitialized */
				goto cleanup;
			}
			
			/* bitrate tracking; add the header's bytes here, the body bytes
			 are done by packet above */
			vf->bittrack+=og.header_len*8;
			
			/* has our decoding just traversed a bitstream boundary? */
			if(vf->ready_state==INITSET){
				if(vf->current_serialno!=ogg_page_serialno(&og)){
					if(!spanp){
						ret=OV_EOF;
						goto cleanup;
					}
					
					_decode_clear(vf);
					
					if(!vf->seekable){
						vorbis_info_clear(vf->vi);
						vorbis_comment_clear(vf->vc);
					}
				}
			}
		}
		
		/* Do we need to load a new machine before submitting the page? */
		/* This is different in the seekable and non-seekable cases.  
		 
		 In the seekable case, we already have all the header
		 information loaded and cached; we just initialize the machine
		 with it and continue on our merry way.
		 
		 In the non-seekable (streaming) case, we'll only be at a
		 boundary if we just left the previous logical bitstream and
		 we're now nominally at the header of the next bitstream
		 */
		
		if(vf->ready_state!=INITSET){ 
			int link;
			
			if(vf->ready_state<STREAMSET){
				if(vf->seekable){
					vf->current_serialno=ogg_page_serialno(&og);
					
					/* match the serialno to bitstream section.  We use this rather than
					 offset positions to avoid problems near logical bitstream
					 boundaries */
					for(link=0;link<vf->links;link++)
						if(vf->serialnos[link]==vf->current_serialno)break;
					if(link==vf->links){
						ret=OV_EBADLINK; /* sign of a bogus stream.  error out,
						 leave machine uninitialized */
						goto cleanup;
					}
					
					vf->current_link=link;
					
					ogg_stream_reset_serialno(vf->os,vf->current_serialno);
					vf->ready_state=STREAMSET;
					
				}else{
					/* we're streaming */
					/* fetch the three header packets, build the info struct */
					
					int ret=_fetch_headers(vf,vf->vi,vf->vc,&vf->current_serialno,&og);
					if(ret) goto cleanup;
					vf->current_link++;
					link=0;
				}
			}
			
			_make_decode_ready(vf);
		}
		ogg_stream_pagein(vf->os,&og);
	}
cleanup:
	ogg_packet_release(&op);
	ogg_page_release(&og);
	return ret;
}

/* if, eg, 64 bit stdio is configured by default, this will build with
 fseek64 */
static int _fseek64_wrap(FILE *f,ogg_int64_t off,int whence){
	if(f==NULL)return(-1);
	return (int) fseek(f,(long)off,whence);
}

static int _ov_open1(void *f,OggVorbis_File *vf,char *initial,
					 long ibytes, ov_callbacks callbacks){
	int offsettest=(f?callbacks.seek_func(f,0,SEEK_CUR):-1);
	int ret;
	
	memset(vf,0,sizeof(*vf));
	vf->datasource=f;
	vf->callbacks = callbacks;
	
	/* init the framing state */
	vf->oy=ogg_sync_create();
	
	/* perhaps some data was previously read into a buffer for testing
     against other stream types.  Allow initialization from this
     previously read data (as we may be reading from a non-seekable
     stream) */
	if(initial){
		unsigned char *buffer=ogg_sync_bufferin(vf->oy,ibytes);
		memcpy(buffer,initial,ibytes);
		ogg_sync_wrote(vf->oy,ibytes);
	}
	
	/* can we seek? Stevens suggests the seek test was portable */
	if(offsettest!=-1)vf->seekable=1;
	
	/* No seeking yet; Set up a 'single' (current) logical bitstream
     entry for partial open */
	vf->links=1;
	vf->vi=_ogg_calloc(vf->links,sizeof(*vf->vi));
	vf->vc=_ogg_calloc(vf->links,sizeof(*vf->vc));
	vf->os=ogg_stream_create(-1); /* fill in the serialno later */
	
	/* Try to fetch the headers, maintaining all the storage */
	if((ret=_fetch_headers(vf,vf->vi,vf->vc,&vf->current_serialno,NULL))<0){
		vf->datasource=NULL;
		ov_clear(vf);
	}else if(vf->ready_state < PARTOPEN)
		vf->ready_state=PARTOPEN;
	return(ret);
}

static int _ov_open2(OggVorbis_File *vf){
	if(vf->ready_state < OPENED)
		vf->ready_state=OPENED;
	if(vf->seekable){
		int ret=_open_seekable2(vf);
		if(ret){
			vf->datasource=NULL;
			ov_clear(vf);
		}
		return(ret);
	}
	return 0;
}


/* clear out the OggVorbis_File struct */
int ov_clear(OggVorbis_File *vf){
	if(vf){
		vorbis_block_clear(&vf->vb);
		vorbis_dsp_clear(&vf->vd);
		ogg_stream_destroy(vf->os);
		
		if(vf->vi && vf->links){
			int i;
			for(i=0;i<vf->links;i++){
				vorbis_info_clear(vf->vi+i);
				vorbis_comment_clear(vf->vc+i);
			}
			_ogg_free(vf->vi);
			_ogg_free(vf->vc);
		}
		if(vf->dataoffsets)_ogg_free(vf->dataoffsets);
		if(vf->pcmlengths)_ogg_free(vf->pcmlengths);
		if(vf->serialnos)_ogg_free(vf->serialnos);
		if(vf->offsets)_ogg_free(vf->offsets);
		ogg_sync_destroy(vf->oy);
		
		if(vf->datasource)(vf->callbacks.close_func)(vf->datasource);
		memset(vf,0,sizeof(*vf));
	}
#ifdef DEBUG_LEAKS
	_VDBG_dump();
#endif
	return(0);
}

/* inspects the OggVorbis file and finds/documents all the logical
 bitstreams contained in it.  Tries to be tolerant of logical
 bitstream sections that are truncated/woogie. 
 
 return: -1) error
 0) OK
 */

int ov_open_callbacks(void *f,OggVorbis_File *vf,char *initial,long ibytes,
					  ov_callbacks callbacks){
	int ret=_ov_open1(f,vf,initial,ibytes,callbacks);
	if(ret)return ret;
	return _ov_open2(vf);
}

int ov_open(FILE *f,OggVorbis_File *vf,char *initial,long ibytes){
	ov_callbacks callbacks = {
		(size_t (*)(void *, size_t, size_t, void *))  fread,
		(int (*)(void *, ogg_int64_t, int))              _fseek64_wrap,
		(int (*)(void *))                             fclose,
		(long (*)(void *))                            ftell
	};
	
	return ov_open_callbacks((void *)f, vf, initial, ibytes, callbacks);
}

/* Only partially open the vorbis file; test for Vorbisness, and load
 the headers for the first chain.  Do not seek (although test for
 seekability).  Use ov_test_open to finish opening the file, else
 ov_clear to close/free it. Same return codes as open. */

int ov_test_callbacks(void *f,OggVorbis_File *vf,char *initial,long ibytes,
					  ov_callbacks callbacks)
{
	return _ov_open1(f,vf,initial,ibytes,callbacks);
}

int ov_test(FILE *f,OggVorbis_File *vf,char *initial,long ibytes){
	ov_callbacks callbacks = {
		(size_t (*)(void *, size_t, size_t, void *))  fread,
		(int (*)(void *, ogg_int64_t, int))              _fseek64_wrap,
		(int (*)(void *))                             fclose,
		(long (*)(void *))                            ftell
	};
	
	return ov_test_callbacks((void *)f, vf, initial, ibytes, callbacks);
}

int ov_test_open(OggVorbis_File *vf){
	if(vf->ready_state!=PARTOPEN)return(OV_EINVAL);
	return _ov_open2(vf);
}

/* How many logical bitstreams in this physical bitstream? */
long ov_streams(OggVorbis_File *vf){
	return vf->links;
}

/* Is the FILE * associated with vf seekable? */
long ov_seekable(OggVorbis_File *vf){
	return vf->seekable;
}

/* returns the bitrate for a given logical bitstream or the entire
 physical bitstream.  If the file is open for random access, it will
 find the *actual* average bitrate.  If the file is streaming, it
 returns the nominal bitrate (if set) else the average of the
 upper/lower bounds (if set) else -1 (unset).
 
 If you want the actual bitrate field settings, get them from the
 vorbis_info structs */

long ov_bitrate(OggVorbis_File *vf,int i){
	if(vf->ready_state<OPENED)return(OV_EINVAL);
	if(i>=vf->links)return(OV_EINVAL);
	if(!vf->seekable && i!=0)return(ov_bitrate(vf,0));
	if(i<0){
		ogg_int64_t bits=0;
		int i;
		for(i=0;i<vf->links;i++)
			bits+=(vf->offsets[i+1]-vf->dataoffsets[i])*8;
		/* This once read: return(rint(bits/ov_time_total(vf,-1)));
		 * gcc 3.x on x86 miscompiled this at optimisation level 2 and above,
		 * so this is slightly transformed to make it work.
		 */
		return (long)(bits*1000/ov_time_total(vf,-1));
	}else{
		if(vf->seekable){
			/* return the actual bitrate */
			return (long) ((vf->offsets[i+1]-vf->dataoffsets[i])*8000/ov_time_total(vf,i));
		}else{
			/* return nominal if set */
			if(vf->vi[i].bitrate_nominal>0){
				return vf->vi[i].bitrate_nominal;
			}else{
				if(vf->vi[i].bitrate_upper>0){
					if(vf->vi[i].bitrate_lower>0){
						return ((vf->vi[i].bitrate_upper+vf->vi[i].bitrate_lower))>>1;
					}else{
						return vf->vi[i].bitrate_upper;
					}
				}
				return(OV_FALSE);
			}
		}
	}
}

/* returns the actual bitrate since last call.  returns -1 if no
 additional data to offer since last call (or at beginning of stream),
 EINVAL if stream is only partially open 
 */
long ov_bitrate_instant(OggVorbis_File *vf){
	int link=(vf->seekable?vf->current_link:0);
	long ret;
	if(vf->ready_state<OPENED)return(OV_EINVAL);
	if(vf->samptrack==0)return(OV_FALSE);
	ret=(long)(vf->bittrack/vf->samptrack*vf->vi[link].rate);
	vf->bittrack=0;
	vf->samptrack=0;
	return(ret);
}

/* Guess */
long ov_serialnumber(OggVorbis_File *vf,int i){
	if(i>=vf->links)return(ov_serialnumber(vf,vf->links-1));
	if(!vf->seekable && i>=0)return(ov_serialnumber(vf,-1));
	if(i<0){
		return(vf->current_serialno);
	}else{
		return(vf->serialnos[i]);
	}
}

/* returns: total raw (compressed) length of content if i==-1
 raw (compressed) length of that logical bitstream for i==0 to n
 OV_EINVAL if the stream is not seekable (we can't know the length)
 or if stream is only partially open
 */
ogg_int64_t ov_raw_total(OggVorbis_File *vf,int i){
	if(vf->ready_state<OPENED)return(OV_EINVAL);
	if(!vf->seekable || i>=vf->links)return(OV_EINVAL);
	if(i<0){
		ogg_int64_t acc=0;
		int i;
		for(i=0;i<vf->links;i++)
			acc+=ov_raw_total(vf,i);
		return(acc);
	}else{
		return(vf->offsets[i+1]-vf->offsets[i]);
	}
}

/* returns: total PCM length (samples) of content if i==-1 PCM length
 (samples) of that logical bitstream for i==0 to n
 OV_EINVAL if the stream is not seekable (we can't know the
 length) or only partially open 
 */
ogg_int64_t ov_pcm_total(OggVorbis_File *vf,int i){
	if(vf->ready_state<OPENED)return(OV_EINVAL);
	if(!vf->seekable || i>=vf->links)return(OV_EINVAL);
	if(i<0){
		ogg_int64_t acc=0;
		int i;
		for(i=0;i<vf->links;i++)
			acc+=ov_pcm_total(vf,i);
		return(acc);
	}else{
		return(vf->pcmlengths[i*2+1]);
	}
}

/* returns: total milliseconds of content if i==-1
 milliseconds in that logical bitstream for i==0 to n
 OV_EINVAL if the stream is not seekable (we can't know the
 length) or only partially open 
 */
ogg_int64_t ov_time_total(OggVorbis_File *vf,int i){
	if(vf->ready_state<OPENED)return(OV_EINVAL);
	if(!vf->seekable || i>=vf->links)return(OV_EINVAL);
	if(i<0){
		ogg_int64_t acc=0;
		int i;
		for(i=0;i<vf->links;i++)
			acc+=ov_time_total(vf,i);
		return(acc);
	}else{
		return(((ogg_int64_t)vf->pcmlengths[i*2+1])*1000/vf->vi[i].rate);
	}
}

/* seek to an offset relative to the *compressed* data. This also
 scans packets to update the PCM cursor. It will cross a logical
 bitstream boundary, but only if it can't get any packets out of the
 tail of the bitstream we seek to (so no surprises).
 
 returns zero on success, nonzero on failure */

int ov_raw_seek(OggVorbis_File *vf,ogg_int64_t pos){
	ogg_stream_state *work_os=NULL;
	ogg_page og={0,0,0,0};
	ogg_packet op={0,0,0,0,0,0};
	
	if(vf->ready_state<OPENED)return(OV_EINVAL);
	if(!vf->seekable)
		return(OV_ENOSEEK); /* don't dump machine if we can't seek */
	
	if(pos<0 || pos>vf->end)return(OV_EINVAL);
	
	/* don't yet clear out decoding machine (if it's initialized), in
     the case we're in the same link.  Restart the decode lapping, and
     let _fetch_and_process_packet deal with a potential bitstream
     boundary */
	vf->pcm_offset=-1;
	ogg_stream_reset_serialno(vf->os,
							  vf->current_serialno); /* must set serialno */
	vorbis_synthesis_restart(&vf->vd);
    
	_seek_helper(vf,pos);
	
	/* we need to make sure the pcm_offset is set, but we don't want to
     advance the raw cursor past good packets just to get to the first
     with a granulepos.  That's not equivalent behavior to beginning
     decoding as immediately after the seek position as possible.
	 
     So, a hack.  We use two stream states; a local scratch state and
     the shared vf->os stream state.  We use the local state to
     scan, and the shared state as a buffer for later decode. 
	 
     Unfortuantely, on the last page we still advance to last packet
     because the granulepos on the last page is not necessarily on a
     packet boundary, and we need to make sure the granpos is
     correct. 
	 */
	
	{
		int lastblock=0;
		int accblock=0;
		int thisblock;
		int eosflag=0;
		
		work_os=ogg_stream_create(vf->current_serialno); /* get the memory ready */
		while(1){
			if(vf->ready_state>=STREAMSET){
				/* snarf/scan a packet if we can */
				int result=ogg_stream_packetout(work_os,&op);
				
				if(result>0){
					
					if(vf->vi[vf->current_link].codec_setup){
						thisblock=vorbis_packet_blocksize(vf->vi+vf->current_link,&op);
						if(thisblock<0){
							ogg_stream_packetout(vf->os,NULL);
							thisblock=0;
						}else{
							
							if(eosflag)
								ogg_stream_packetout(vf->os,NULL);
							else
								if(lastblock)accblock+=(lastblock+thisblock)>>2;
						}	    
						
						if(op.granulepos!=-1){
							int i,link=vf->current_link;
							ogg_int64_t granulepos=op.granulepos-vf->pcmlengths[link*2];
							if(granulepos<0)granulepos=0;
							
							for(i=0;i<link;i++)
								granulepos+=vf->pcmlengths[i*2+1];
							vf->pcm_offset=granulepos-accblock;
							break;
						}
						lastblock=thisblock;
						continue;
					}else
						ogg_stream_packetout(vf->os,NULL);
				}
			}
			
			if(!lastblock){
				if(_get_next_page(vf,&og,-1)<0){
					vf->pcm_offset=ov_pcm_total(vf,-1);
					break;
				}
			}else{
				/* huh?  Bogus stream with packets but no granulepos */
				vf->pcm_offset=-1;
				break;
			}
			
			/* has our decoding just traversed a bitstream boundary? */
			if(vf->ready_state>=STREAMSET)
				if(vf->current_serialno!=ogg_page_serialno(&og)){
					_decode_clear(vf); /* clear out stream state */
					ogg_stream_destroy(work_os);
				}
			
			if(vf->ready_state<STREAMSET){
				int link;
				
				vf->current_serialno=ogg_page_serialno(&og);
				for(link=0;link<vf->links;link++)
					if(vf->serialnos[link]==vf->current_serialno)break;
				if(link==vf->links)
					goto seek_error; /* sign of a bogus stream.  error out,
					 leave machine uninitialized */
				
				vf->current_link=link;
				
				ogg_stream_reset_serialno(vf->os,vf->current_serialno);
				ogg_stream_reset_serialno(work_os,vf->current_serialno); 
				vf->ready_state=STREAMSET;
				
			}
			
			{
				ogg_page dup;
				ogg_page_dup(&dup,&og);
				eosflag=ogg_page_eos(&og);
				ogg_stream_pagein(vf->os,&og);
				ogg_stream_pagein(work_os,&dup);
			}
		}
	}
	
	ogg_packet_release(&op);
	ogg_page_release(&og);
	ogg_stream_destroy(work_os);
	vf->bittrack=0;
	vf->samptrack=0;
	return(0);
	
seek_error:
	ogg_packet_release(&op);
	ogg_page_release(&og);
	
	/* dump the machine so we're in a known state */
	vf->pcm_offset=-1;
	ogg_stream_destroy(work_os);
	_decode_clear(vf);
	return OV_EBADLINK;
}

/* Page granularity seek (faster than sample granularity because we
 don't do the last bit of decode to find a specific sample).
 
 Seek to the last [granule marked] page preceeding the specified pos
 location, such that decoding past the returned point will quickly
 arrive at the requested position. */
int ov_pcm_seek_page(OggVorbis_File *vf,ogg_int64_t pos){
	int link=-1;
	ogg_int64_t result=0;
	ogg_int64_t total=ov_pcm_total(vf,-1);
	ogg_page og={0,0,0,0};
	ogg_packet op={0,0,0,0,0,0};
	
	if(vf->ready_state<OPENED)return(OV_EINVAL);
	if(!vf->seekable)return(OV_ENOSEEK);
	if(pos<0 || pos>total)return(OV_EINVAL);
	
	/* which bitstream section does this pcm offset occur in? */
	for(link=vf->links-1;link>=0;link--){
		total-=vf->pcmlengths[link*2+1];
		if(pos>=total)break;
	}
	
	/* search within the logical bitstream for the page with the highest
     pcm_pos preceeding (or equal to) pos.  There is a danger here;
     missing pages or incorrect frame number information in the
     bitstream could make our task impossible.  Account for that (it
     would be an error condition) */
	
	/* new search algorithm by HB (Nicholas Vinen) */
	{
		ogg_int64_t end=vf->offsets[link+1];
		ogg_int64_t begin=vf->offsets[link];
		ogg_int64_t begintime = vf->pcmlengths[link*2];
		ogg_int64_t endtime = vf->pcmlengths[link*2+1]+begintime;
		ogg_int64_t target=pos-total+begintime;
		ogg_int64_t best=begin;
		
		while(begin<end){
			ogg_int64_t bisect;
			
			if(end-begin<CHUNKSIZE){
				bisect=begin;
			}else{
				/* take a (pretty decent) guess. */
				bisect=begin + 
				(target-begintime)*(end-begin)/(endtime-begintime) - CHUNKSIZE;
				if(bisect<=begin)
					bisect=begin+1;
			}
			
			_seek_helper(vf,bisect);
			
			while(begin<end){
				result=_get_next_page(vf,&og,end-vf->offset);
				if(result==OV_EREAD) goto seek_error;
				if(result<0){
					if(bisect<=begin+1)
						end=begin; /* found it */
					else{
						if(bisect==0) goto seek_error;
						bisect-=CHUNKSIZE;
						if(bisect<=begin)bisect=begin+1;
						_seek_helper(vf,bisect);
					}
				}else{
					ogg_int64_t granulepos=ogg_page_granulepos(&og);
					if(granulepos==-1)continue;
					if(granulepos<target){
						best=result;  /* raw offset of packet with granulepos */ 
						begin=vf->offset; /* raw offset of next page */
						begintime=granulepos;
						
						if(target-begintime>44100)break;
						bisect=begin; /* *not* begin + 1 */
					}else{
						if(bisect<=begin+1)
							end=begin;  /* found it */
						else{
							if(end==vf->offset){ /* we're pretty close - we'd be stuck in */
								end=result;
								bisect-=CHUNKSIZE; /* an endless loop otherwise. */
								if(bisect<=begin)bisect=begin+1;
								_seek_helper(vf,bisect);
							}else{
								end=result;
								endtime=granulepos;
								break;
							}
						}
					}
				}
			}
		}
		
		/* found our page. seek to it, update pcm offset. Easier case than
		 raw_seek, don't keep packets preceeding granulepos. */
		{
			
			/* seek */
			_seek_helper(vf,best);
			vf->pcm_offset=-1;
			
			if(_get_next_page(vf,&og,-1)<0){
				ogg_page_release(&og);
				return(OV_EOF); /* shouldn't happen */
			}
			
			if(link!=vf->current_link){
				/* Different link; dump entire decode machine */
				_decode_clear(vf);  
				
				vf->current_link=link;
				vf->current_serialno=ogg_page_serialno(&og);
				vf->ready_state=STREAMSET;
				
			}else{
				vorbis_synthesis_restart(&vf->vd);
			}
			
			ogg_stream_reset_serialno(vf->os,vf->current_serialno);
			ogg_stream_pagein(vf->os,&og);
			
			/* pull out all but last packet; the one with granulepos */
			while(1){
				result=ogg_stream_packetpeek(vf->os,&op);
				if(result==0){
					/* !!! the packet finishing this page originated on a
					 preceeding page. Keep fetching previous pages until we
					 get one with a granulepos or without the 'continued' flag
					 set.  Then just use raw_seek for simplicity. */
					
					_seek_helper(vf,best);
					
					while(1){
						result=_get_prev_page(vf,&og);
						if(result<0) goto seek_error;
						if(ogg_page_granulepos(&og)>-1 ||
						   !ogg_page_continued(&og)){
							return ov_raw_seek(vf,result);
						}
						vf->offset=result;
					}
				}
				if(result<0){
					result = OV_EBADPACKET; 
					goto seek_error;
				}
				if(op.granulepos!=-1){
					vf->pcm_offset=op.granulepos-vf->pcmlengths[vf->current_link*2];
					if(vf->pcm_offset<0)vf->pcm_offset=0;
					vf->pcm_offset+=total;
					break;
				}else
					result=ogg_stream_packetout(vf->os,NULL);
			}
		}
	}
	
	/* verify result */
	if(vf->pcm_offset>pos || pos>ov_pcm_total(vf,-1)){
		result=OV_EFAULT;
		goto seek_error;
	}
	vf->bittrack=0;
	vf->samptrack=0;
	
	ogg_page_release(&og);
	ogg_packet_release(&op);
	return(0);
	
seek_error:
	
	ogg_page_release(&og);
	ogg_packet_release(&op);
	
	/* dump machine so we're in a known state */
	vf->pcm_offset=-1;
	_decode_clear(vf);
	return (int)result;
}

/* seek to a sample offset relative to the decompressed pcm stream 
 returns zero on success, nonzero on failure */

int ov_pcm_seek(OggVorbis_File *vf,ogg_int64_t pos){
	ogg_packet op={0,0,0,0,0,0};
	ogg_page og={0,0,0,0};
	int thisblock,lastblock=0;
	int ret=ov_pcm_seek_page(vf,pos);
	if(ret<0)return(ret);
	_make_decode_ready(vf);
	
	/* discard leading packets we don't need for the lapping of the
     position we want; don't decode them */
	
	while(1){
		
		int ret=ogg_stream_packetpeek(vf->os,&op);
		if(ret>0){
			thisblock=vorbis_packet_blocksize(vf->vi+vf->current_link,&op);
			if(thisblock<0){
				ogg_stream_packetout(vf->os,NULL);
				continue; /* non audio packet */
			}
			if(lastblock)vf->pcm_offset+=(lastblock+thisblock)>>2;
			
			if(vf->pcm_offset+((thisblock+
								vorbis_info_blocksize(vf->vi,1))>>2)>=pos)break;
			
			/* remove the packet from packet queue and track its granulepos */
			ogg_stream_packetout(vf->os,NULL);
			vorbis_synthesis(&vf->vb,&op,0);  /* set up a vb with
			 only tracking, no
			 pcm_decode */
			vorbis_synthesis_blockin(&vf->vd,&vf->vb); 
			
			/* end of logical stream case is hard, especially with exact
			 length positioning. */
			
			if(op.granulepos>-1){
				int i;
				/* always believe the stream markers */
				vf->pcm_offset=op.granulepos-vf->pcmlengths[vf->current_link*2];
				if(vf->pcm_offset<0)vf->pcm_offset=0;
				for(i=0;i<vf->current_link;i++)
					vf->pcm_offset+=vf->pcmlengths[i*2+1];
			}
			
			lastblock=thisblock;
			
		}else{
			if(ret<0 && ret!=OV_HOLE)break;
			
			/* suck in a new page */
			if(_get_next_page(vf,&og,-1)<0)break;
			if(vf->current_serialno!=ogg_page_serialno(&og))_decode_clear(vf);
			
			if(vf->ready_state<STREAMSET){
				int link;
				
				vf->current_serialno=ogg_page_serialno(&og);
				for(link=0;link<vf->links;link++)
					if(vf->serialnos[link]==vf->current_serialno)break;
				if(link==vf->links){
					ogg_page_release(&og);
					ogg_packet_release(&op);
					return(OV_EBADLINK);
				}
				vf->current_link=link;
				
				ogg_stream_reset_serialno(vf->os,vf->current_serialno); 
				vf->ready_state=STREAMSET;      
				_make_decode_ready(vf);
				lastblock=0;
			}
			
			ogg_stream_pagein(vf->os,&og);
		}
	}
	
	vf->bittrack=0;
	vf->samptrack=0;
	/* discard samples until we reach the desired position. Crossing a
     logical bitstream boundary with abandon is OK. */
	while(vf->pcm_offset<pos){
		ogg_int64_t target=pos-vf->pcm_offset;
		long samples=vorbis_synthesis_pcmout(&vf->vd,NULL);
		
		if(samples>target)samples=(long)target;
		vorbis_synthesis_read(&vf->vd,samples);
		vf->pcm_offset+=samples;
		
		if(samples<target)
			if(_fetch_and_process_packet(vf,1,1)<=0)
				vf->pcm_offset=ov_pcm_total(vf,-1); /* eof */
	}
	
	ogg_page_release(&og);
	ogg_packet_release(&op);
	return 0;
}

/* seek to a playback time relative to the decompressed pcm stream 
 returns zero on success, nonzero on failure */
int ov_time_seek(OggVorbis_File *vf,ogg_int64_t milliseconds){
	/* translate time to PCM position and call ov_pcm_seek */
	
	int link=-1;
	ogg_int64_t pcm_total=ov_pcm_total(vf,-1);
	ogg_int64_t time_total=ov_time_total(vf,-1);
	
	if(vf->ready_state<OPENED)return(OV_EINVAL);
	if(!vf->seekable)return(OV_ENOSEEK);
	if(milliseconds<0 || milliseconds>time_total)return(OV_EINVAL);
	
	/* which bitstream section does this time offset occur in? */
	for(link=vf->links-1;link>=0;link--){
		pcm_total-=vf->pcmlengths[link*2+1];
		time_total-=ov_time_total(vf,link);
		if(milliseconds>=time_total)break;
	}
	
	/* enough information to convert time offset to pcm offset */
	{
		ogg_int64_t target=pcm_total+(milliseconds-time_total)*vf->vi[link].rate/1000;
		return(ov_pcm_seek(vf,target));
	}
}

/* page-granularity version of ov_time_seek 
 returns zero on success, nonzero on failure */
int ov_time_seek_page(OggVorbis_File *vf,ogg_int64_t milliseconds){
	/* translate time to PCM position and call ov_pcm_seek */
	
	int link=-1;
	ogg_int64_t pcm_total=ov_pcm_total(vf,-1);
	ogg_int64_t time_total=ov_time_total(vf,-1);
	
	if(vf->ready_state<OPENED)return(OV_EINVAL);
	if(!vf->seekable)return(OV_ENOSEEK);
	if(milliseconds<0 || milliseconds>time_total)return(OV_EINVAL);
	
	/* which bitstream section does this time offset occur in? */
	for(link=vf->links-1;link>=0;link--){
		pcm_total-=vf->pcmlengths[link*2+1];
		time_total-=ov_time_total(vf,link);
		if(milliseconds>=time_total)break;
	}
	
	/* enough information to convert time offset to pcm offset */
	{
		ogg_int64_t target=pcm_total+(milliseconds-time_total)*vf->vi[link].rate/1000;
		return(ov_pcm_seek_page(vf,target));
	}
}

/* tell the current stream offset cursor.  Note that seek followed by
 tell will likely not give the set offset due to caching */
ogg_int64_t ov_raw_tell(OggVorbis_File *vf){
	if(vf->ready_state<OPENED)return(OV_EINVAL);
	return(vf->offset);
}

/* return PCM offset (sample) of next PCM sample to be read */
ogg_int64_t ov_pcm_tell(OggVorbis_File *vf){
	if(vf->ready_state<OPENED)return(OV_EINVAL);
	return(vf->pcm_offset);
}

/* return time offset (milliseconds) of next PCM sample to be read */
ogg_int64_t ov_time_tell(OggVorbis_File *vf){
	int link=0;
	ogg_int64_t pcm_total=0;
	ogg_int64_t time_total=0;
	
	if(vf->ready_state<OPENED)return(OV_EINVAL);
	if(vf->seekable){
		pcm_total=ov_pcm_total(vf,-1);
		time_total=ov_time_total(vf,-1);
		
		/* which bitstream section does this time offset occur in? */
		for(link=vf->links-1;link>=0;link--){
			pcm_total-=vf->pcmlengths[link*2+1];
			time_total-=ov_time_total(vf,link);
			if(vf->pcm_offset>=pcm_total)break;
		}
	}
	
	return(time_total+(1000*vf->pcm_offset-pcm_total)/vf->vi[link].rate);
}

/*  link:   -1) return the vorbis_info struct for the bitstream section
 currently being decoded
 0-n) to request information for a specific bitstream section
 
 In the case of a non-seekable bitstream, any call returns the
 current bitstream.  NULL in the case that the machine is not
 initialized */

vorbis_info *ov_info(OggVorbis_File *vf,int link){
	if(vf->seekable){
		if(link<0)
			if(vf->ready_state>=STREAMSET)
				return vf->vi+vf->current_link;
			else
				return vf->vi;
			else
				if(link>=vf->links)
					return NULL;
				else
					return vf->vi+link;
	}else{
		return vf->vi;
	}
}

/* grr, strong typing, grr, no templates/inheritence, grr */
vorbis_comment *ov_comment(OggVorbis_File *vf,int link){
	if(vf->seekable){
		if(link<0)
			if(vf->ready_state>=STREAMSET)
				return vf->vc+vf->current_link;
			else
				return vf->vc;
			else
				if(link>=vf->links)
					return NULL;
				else
					return vf->vc+link;
	}else{
		return vf->vc;
	}
}

/* up to this point, everything could more or less hide the multiple
 logical bitstream nature of chaining from the toplevel application
 if the toplevel application didn't particularly care.  However, at
 the point that we actually read audio back, the multiple-section
 nature must surface: Multiple bitstream sections do not necessarily
 have to have the same number of channels or sampling rate.
 
 ov_read returns the sequential logical bitstream number currently
 being decoded along with the PCM data in order that the toplevel
 application can take action on channel/sample rate changes.  This
 number will be incremented even for streamed (non-seekable) streams
 (for seekable streams, it represents the actual logical bitstream
 index within the physical bitstream.  Note that the accessor
 functions above are aware of this dichotomy).
 
 input values: buffer) a buffer to hold packed PCM data for return
 length) the byte length requested to be placed into buffer
 
 return values: <0) error/hole in data (OV_HOLE), partial open (OV_EINVAL)
 0) EOF
 n) number of bytes of PCM actually returned.  The
 below works on a packet-by-packet basis, so the
 return length is not related to the 'length' passed
 in, just guaranteed to fit.
 
 *section) set to the logical bitstream number */

long ov_read(OggVorbis_File *vf,char *buffer,int bytes_req,int *bitstream){
	int i,j;
	
	ogg_int32_t **pcm;
	long samples;
	
	if(vf->ready_state<OPENED)return(OV_EINVAL);
	
	while(1){
		if(vf->ready_state==INITSET){
			samples=vorbis_synthesis_pcmout(&vf->vd,&pcm);
			if(samples)break;
		}
		
		/* suck in another packet */
		{
			int ret=_fetch_and_process_packet(vf,1,1);
			if(ret==OV_EOF)
				return(0);
			if(ret<=0)
				return(ret);
		}
		
	}
	
	if(samples>0){
		
		/* yay! proceed to pack data into the byte buffer */
		
		long channels=ov_info(vf,-1)->channels;
		
		if(samples>(bytes_req/(2*channels)))
			samples=bytes_req/(2*channels);      
		
		for(i=0;i<channels;i++) { /* It's faster in this order */
			ogg_int32_t *src=pcm[i];
			short *dest=((short *)buffer)+i;
			for(j=0;j<samples;j++) {
				*dest=CLIP_TO_15(src[j]>>9);
				dest+=channels;
			}
		}
		
		vorbis_synthesis_read(&vf->vd,samples);
		vf->pcm_offset+=samples;
		if(bitstream)*bitstream=vf->current_link;
		return(samples*2*channels);
	}else{
		return(samples);
	}
}
