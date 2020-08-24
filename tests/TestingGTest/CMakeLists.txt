cmake_minimum_required (VERSION 3.2.2)

project(TestingGTest)

if (PLATFORM MATCHES rpi)
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

set(GTEST_SRC "$ENV{CROSS_PATH}/tree/gtest/src")

set(SRC_FILES
  "${GTEST_SRC}/test/gtest-message_test.cc"
  "${GTEST_SRC}/test/gtest-printers_test.cc"
)

include("$ENV{CROSS_PATH}/core/cmake/test.cmake")

#add_test(NAME MessageTest  COMMAND ./run.sh "--gtest_filter=MessageTest.*")
#add_test(NAME PrintersTest COMMAND ./run.sh "--gtest_filter=Print*Test.*")
