#if !defined(UUID_79BDE83E6E594AE9AD6BCD8638C7A0A7)
#define UUID_79BDE83E6E594AE9AD6BCD8638C7A0A7

#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS


#ifndef DEBUG
#define NDEBUG
#endif

#ifdef __cplusplus

// new (nothrow) x
#include <new>
using std::nothrow;

// Useful macro for debugging
#if DEBUG
#include <iostream>
#define DEBUG_LOG(x) { std::cerr << x << std::dec << "\n"; }
#else
#define DEBUG_LOG(x) { void(); }
#endif // DEBUG
#endif // __cplusplus

// Standard headers
#include <stdint.h>
#include <float.h>
#include <stddef.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <assert.h>

// This seems to be needed in XCode..?
#define asm __asm__

// Config

#ifdef __arm__
#define USE_ARM_ASM 1
#define ARM_DEVICE 1
#endif

#ifdef __thumb__
#error "«Thumb» sounds like «Dumb»! Compile me in ARM mode! (-marm -mno-thumb)"
#endif

#ifndef __FAST_MATH__
#warning "Didn't you forget to add «-ffast-math» to your CFLAGS?"
#endif
// -fsingle-precision-constant -fprefetch-loop-arrays -ftree-loop-ivcanon -fivopts -ftree-vectorize
// -fsingle-precision-constant -fprefetch-loop-arrays -ftree-vectorize 
#ifdef __VFP_FP__
#define USE_VFP_ASM 1
#endif

#ifdef __ARM_NEON__
#define USE_NEON_ASM 1
#endif


// Configuration for the Tremor OGG Vorbis audio codec
#ifdef USE_ARM_ASM
#define _ARM_ASSEM_ 1
#endif
#define USE_MEMORY_H 1

#endif // UUID_79BDE83E6E594AE9AD6BCD8638C7A0A7
