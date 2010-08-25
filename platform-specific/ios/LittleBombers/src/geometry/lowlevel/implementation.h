#if !defined(UUID_3B10B6E4678D4A0497D0D78647CCA5E2)
#define UUID_3B10B6E4678D4A0497D0D78647CCA5E2

#include "geometry/lowlevel/implementations/default.h"
#include "geometry/lowlevel/implementations/vfp.h"
#include "geometry/lowlevel/implementations/neon.h"

#ifdef __ARM_NEON__
typedef geometry::lowlevel::implementation<geometry::lowlevel::platform_neon> simd;
#elif __VFP_FP__
typedef geometry::lowlevel::implementation<geometry::lowlevel::platform_vfp> simd;
#else
typedef geometry::lowlevel::implementation<0> simd;
#endif

#endif // UUID_3B10B6E4678D4A0497D0D78647CCA5E2

