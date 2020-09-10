cmake_minimum_required (VERSION 3.2.2)

project(TestingFileSystem2)

if (PLATFORM MATCHES rpi|linux)
  find_package(Boost COMPONENTS system filesystem REQUIRED)

  list(APPEND INCLUDE_DIRS
    ${Boost_INCLUDE_DIRS}
  )

  list(APPEND LIBRARIES
    ${Boost_LIBRARIES}
    -lpthread
  )
else()
  set(BOOST_ROOT "$ENV{CROSS_PATH}/tree/boost/${PLATFORM}")

  list(APPEND INCLUDE_DIRS
    "${BOOST_ROOT}/include"
  )

  list(APPEND LIBRARIES
    "${BOOST_ROOT}/lib/libboost_system.a"
    "${BOOST_ROOT}/lib/libboost_filesystem.a"
  )
endif()

set(GTEST_ROOT "$ENV{CROSS_PATH}/tree/gtest/${PLATFORM}")

list(APPEND INCLUDE_DIRS
  "${GTEST_ROOT}/include"
)

list(APPEND LIBRARIES
  "${GTEST_ROOT}/lib/libgtest.a"
  "${GTEST_ROOT}/lib/libgtest_main.a"
)

set(SRC_FILES
  src/main.cpp
)

if (PLATFORM MATCHES android)
  list(APPEND SRC_FILES
    "src/android/Bridge.cpp"
  )
endif()

include("$ENV{CROSS_PATH}/core/cmake/test.cmake")
