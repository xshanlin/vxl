// -*- c++ -*-
#ifndef vcl_complex_txx_
#define vcl_complex_txx_

#include <vcl/vcl_complex.h>

#if !VCL_USE_NATIVE_COMPLEX
# include <vcl/emulation/vcl_complex.txx>
#elif defined(VCL_EGCS)
# include <vcl/egcs/vcl_complex.txx>
#elif defined(VCL_GCC_295) && !defined(GNU_LIBSTDCXX_V3)
# include <vcl/gcc-295/vcl_complex.txx>
#elif defined(VCL_GCC_295) && defined(GNU_LIBSTDCXX_V3)
# include <vcl/emulation/vcl_complex.txx>
#elif defined(VCL_SUNPRO_CC)
# include <vcl/sunpro/vcl_complex.txx>
#elif defined(VCL_SGI_CC)
# include <vcl/sgi/vcl_complex.txx>
#elif defined(VCL_WIN32)
# include <vcl/win32/vcl_complex.txx>
#else
# include <vcl/iso/vcl_complex.txx>
#endif

#endif
