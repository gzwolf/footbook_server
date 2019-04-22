set(FOOTBOOK_TARGET "footbook_gtest")

set(GTEST_SRCS
        src/gtest-all.cc
  )
source_group(footbook_gtest FILES ${GTEST_SRCS})

add_library(${FOOTBOOK_TARGET} ${GTEST_SRCS})

# The gtest-all.cc file uses #include "gtest/gtest.h"
target_include_directories(${FOOTBOOK_TARGET} PRIVATE "include")

# In order to allow regex matches in gtest to be shared between Windows
# and other systems we tell gtest to always use it's internal engine.
target_compile_definitions(${FOOTBOOK_TARGET} PRIVATE -DGTEST_HAS_POSIX_RE=0 -DGTEST_LANG_CXX11=1)

# All dependent targets are unit tests.
target_compile_definitions(${FOOTBOOK_TARGET} PUBLIC -DUNIT_TEST)

if(OS_WINDOWS)
  # Disable unused variable warning.
  target_compile_options(${FOOTBOOK_TARGET} PRIVATE "/wd4800")
endif()
