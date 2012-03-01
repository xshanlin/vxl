// This is core/vil/vil_math_sse.cxx

#ifndef vil_math_h_
#error "This header should not be included directly, only through vil_math.h"
#endif

#include <vcl_cstddef.h>
#include <vcl_cstring.h>
#include <vxl_config.h>
#include <x86intrin.h>

#include <vcl_iostream.h>

//:
// \file
// \brief Various mathematical manipulations of 2D images implemented with SSE
// intrinsic functions
// \author Chuck Atkins

//: Compute the 1D absolute difference of two images (im_sum = |imA-imB|)
#define VIL_MATH_IMAGE_ABS_DIFFERENCE_1D_SPECIALIZE(T)                     \
template<>                                                                 \
inline void vil_math_image_abs_difference_1d<vxl_byte,vxl_byte,vxl_byte>(  \
  const vxl_byte* pxA, vcl_ptrdiff_t isA,                                  \
  const vxl_byte* pxB, vcl_ptrdiff_t isB,                                  \
        vxl_byte* pxD, vcl_ptrdiff_t isD,                                  \
  unsigned len)                                                            \
{                                                                          \
  if (isA == 1 && isB == 1 && isD == 1)                                    \
  {                                                                        \
    vil_math_image_abs_difference_1d_sse<T>(pxA, pxB, pxD, len);           \
  }                                                                        \
  else                                                                     \
  {                                                                        \
    vil_math_image_abs_difference_1d_generic<T, T, T>(                     \
      pxA, isA, pxB, isB, pxD, isD, len);                                  \
  }                                                                        \
}

//: Compute absolute difference of two 1D images (im_sum = |imA-imB|)
template<class T>
inline void vil_math_image_abs_difference_1d_sse(
  const T* pxA, const T* pxB, T* pxD,
  unsigned len);


//: Compute absolute difference of two images (im_sum = |imA-imB|)
template<>
inline void vil_math_image_abs_difference_1d_sse<vxl_byte>(
  const vxl_byte* pxA, const vxl_byte* pxB, vxl_byte* pxD,
  unsigned len)
{
  const unsigned ni_d_16 = len >> 4;
  const unsigned ni_m_16 = len & 0xF;

  // Use these for the last set of pxs in each row
  vxl_uint_8 pxLastA[16];
  vxl_uint_8 pxLastB[16];
  vxl_uint_8 pxLastD[16];
  __m128i* pxLastAxmm = reinterpret_cast<__m128i*>(pxLastA);
  __m128i* pxLastBxmm = reinterpret_cast<__m128i*>(pxLastB);
  __m128i* pxLastDxmm = reinterpret_cast<__m128i*>(pxLastD);

  const __m128i* pxAxmm = reinterpret_cast<const __m128i*>(pxA);
  const __m128i* pxBxmm = reinterpret_cast<const __m128i*>(pxB);
        __m128i* pxDxmm = reinterpret_cast<__m128i*>(pxD);

  // Loop through the first set of pxs in groups of 16
  for (unsigned i = 0; i < ni_d_16; ++i, ++pxAxmm, ++pxBxmm, ++pxDxmm)
  {
#ifdef __SSE3__
    __m128i xmmA = _mm_lddqu_si128(pxAxmm);
    __m128i xmmB = _mm_lddqu_si128(pxBxmm);
#else // SSE2
    __m128i xmmA = _mm_loadu_si128(pxAxmm);
    __m128i xmmB = _mm_loadu_si128(pxBxmm);
#endif

    __m128i xmmMax = _mm_max_epu8(xmmA, xmmB);
    __m128i xmmMin = _mm_min_epu8(xmmA, xmmB);
    __m128i xmmD = _mm_subs_epu8(xmmMax, xmmMin);

    _mm_storeu_si128(pxDxmm, xmmD);
  }

  // Process the remainder < 16
  vcl_memcpy(pxLastA, pxAxmm, ni_m_16);
  vcl_memcpy(pxLastB, pxBxmm, ni_m_16);
#ifdef __SSE3__
  __m128i xmmA = _mm_lddqu_si128(pxLastAxmm);
  __m128i xmmB = _mm_lddqu_si128(pxLastBxmm);
#else // SSE2
  __m128i xmmA = _mm_loadu_si128(pxLastAxmm);
  __m128i xmmB = _mm_loadu_si128(pxLastBxmm);
#endif

  __m128i xmmMax = _mm_max_epu8(xmmA, xmmB);
  __m128i xmmMin = _mm_min_epu8(xmmA, xmmB);
  __m128i xmmD = _mm_subs_epu8(xmmMax, xmmMin);

  _mm_storeu_si128(pxLastDxmm, xmmD);
  vcl_memcpy(pxDxmm, pxLastD, ni_m_16);
}

VIL_MATH_IMAGE_ABS_DIFFERENCE_1D_SPECIALIZE(vxl_byte)
