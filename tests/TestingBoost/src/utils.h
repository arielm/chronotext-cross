#pragma once

#include <gtest/gtest.h>

#if defined(GTEST_HAS_RTTI)

static std::string demangle(const std::type_info &ti)
{
  std::string result;

#if defined(GTEST_HAS_CXXABI_H_)
  int status;
  char *demangled = abi::__cxa_demangle(ti.name(), 0, 0, &status);

  result = status ? ti.name() : demangled;
  free(demangled);
#else
  result = ti.name(); // TODO: TEST WITH VISUAL-STUDIO
#endif // GTEST_HAS_CXXABI_H_

  return result;
}

#endif // GTEST_HAS_RTTI
