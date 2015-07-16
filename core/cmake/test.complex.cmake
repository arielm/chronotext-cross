cmake_minimum_required (VERSION 3.2.2)

include("${CROSS_ROOT}/cmake/test.common.cmake")

list(APPEND INCLUDE_DIRS
  "${CROSS_ROOT}/src"
)

list(APPEND SRC_FILES
  "${CROSS_ROOT}/src/Log.cpp"
  "${CROSS_ROOT}/src/Platform.cpp"
)

file(GLOB_RECURSE RESOURCE_FILES
  RELATIVE ${CMAKE_SOURCE_DIR}
  res/[^.]*
)

list(LENGTH RESOURCE_FILES RESOURCE_COUNT)

# ---

if (PLATFORM MATCHES mxe)
  add_definitions(-DCHR_FS_RC)

  set(DATA_RC "")
  set(DATA_CPP "")
  set(counter 128)

  foreach (resource_file ${RESOURCE_FILES})
    set(DATA_RC "${DATA_RC}${counter} RCDATA \"../../${resource_file}\"\r\n")
    set(DATA_CPP "{\"${resource_file}\", ${counter}}, ${DATA_CPP}")
    math(EXPR counter "${counter} + 1")
  endforeach()

  configure_file("${CROSS_ROOT}/cmake/mxe/resources.cpp.in" resources.cpp)
  list(APPEND SRC_FILES "${CMAKE_CURRENT_BINARY_DIR}/resources.cpp")

  if (RESOURCE_COUNT)
    configure_file("${CROSS_ROOT}/cmake/mxe/resources.rc.in" resources.rc)
    list(APPEND SRC_FILES "${CMAKE_CURRENT_BINARY_DIR}/resources.rc")
  endif()

  add_executable(${PROJECT_NAME}
    ${SRC_FILES}
  )

elseif (PLATFORM MATCHES android)
  add_definitions(-DCHR_FS_APK)

  configure_file("${CROSS_ROOT}/cmake/android/AndroidManifest.xml.in" AndroidManifest.xml)
  configure_file("${CROSS_ROOT}/cmake/android/ant.properties.in" ant.properties)
  configure_file("${CROSS_ROOT}/cmake/android/MainActivity.java.in" "src/org/chronotext/${PROJECT_NAME}/MainActivity.java")

  configure_file("${CROSS_ROOT}/cmake/android/tests/AndroidManifest.xml.in" tests/AndroidManifest.xml)
  configure_file("${CROSS_ROOT}/cmake/android/tests/ant.properties.in" tests/ant.properties)
  configure_file("${CROSS_ROOT}/cmake/android/tests/MainActivityTests.java.in" "tests/src/org/chronotext/${PROJECT_NAME}/MainActivityTests.java")

  set(CONFIG_INSTALL "${CROSS_ROOT}/cmake/android/install.instrument.sh.in")
  set(CONFIG_RUN "${CROSS_ROOT}/cmake/android/run.instrument.sh.in")

  set(LIBRARY_OUTPUT_PATH "libs/armeabi-v7a")

  add_library(${PROJECT_NAME} SHARED
    ${SRC_FILES}
    "${CROSS_ROOT}/src/Bridge.cpp"
  )

elseif (PLATFORM MATCHES emscripten)
  add_definitions(-DCHR_FS_JS_EMBED)

  if (RESOURCE_COUNT)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --exclude-file *.DS_Store --embed-file ../../res")
  endif()
  
  add_executable(${PROJECT_NAME}
    ${SRC_FILES}
  )

elseif (PLATFORM MATCHES ios)
  add_definitions(-DCHR_FS_BUNDLE)

  add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    ${RESOURCE_FILES}
  )

  foreach (resource_file ${RESOURCE_FILES})
    get_filename_component(parent_dir ${resource_file} DIRECTORY)
    set_source_files_properties(${resource_file} PROPERTIES MACOSX_PACKAGE_LOCATION ${PROJECT_NAME}.app/${parent_dir})
  endforeach()

else()
  add_executable(${PROJECT_NAME}
    ${SRC_FILES}
  )
endif()

if (PLATFORM MATCHES osx)
  if (RESOURCE_COUNT)
    set(CONFIG_INSTALL "${CROSS_ROOT}/cmake/install.symlink.sh.in")
  endif()
endif()

# ---

include_directories(
  ${INCLUDE_DIRS}
)

if (PLATFORM MATCHES ios|osx)
  target_link_libraries(${PROJECT_NAME}
    "-framework Foundation"
    ${LIBRARIES}
  )
elseif (PLATFORM MATCHES android)
  target_link_libraries(${PROJECT_NAME}
    log
    android
    ${LIBRARIES}
  )
else()
  target_link_libraries(${PROJECT_NAME}
    ${LIBRARIES}
  )
endif()

# ---

setupTesting()
