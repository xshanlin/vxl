// -*- c++ -*-
#ifndef vcl_vector_txx_
#define vcl_vector_txx_

#include <vcl/vcl_vector.h>

#undef VCL_VECTOR_INSTANTIATE

#if !VCL_USE_NATIVE_STL
# include <vcl/emulation/vcl_vector.txx>
#elif defined(VCL_EGCS)
# include <vcl/egcs/vcl_vector.txx>
#elif defined(VCL_GCC_295) && !defined(GNU_LIBSTDCXX_V3)
# include <vcl/gcc-295/vcl_vector.txx>
#elif defined(VCL_GCC_295) && defined(GNU_LIBSTDCXX_V3)
# include <vcl/gcc-libstdcxx-v3/vcl_vector.txx>
#elif defined(VCL_SUNPRO_CC)
# include <vcl/sunpro/vcl_vector.txx>
#elif defined(VCL_SGI_CC)
# include <vcl/sgi/vcl_vector.txx>
#elif defined(VCL_WIN32)
# include <vcl/win32/vcl_vector.txx>
#else
# include <vcl/iso/vcl_vector.txx>
#endif

#endif
