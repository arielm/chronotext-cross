
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
  if (FS MATCHES RC)
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

  else()
    if (RESOURCE_COUNT)
      set(CONFIG_INSTALL "${CROSS_ROOT}/cmake/install.symlink.sh.in")
    endif()
  endif()

  add_executable(${PROJECT_NAME}
    ${SRC_FILES}
  )

elseif (PLATFORM MATCHES android)
  if (RUN MATCHES EXE)
    add_executable(${PROJECT_NAME}
      ${SRC_FILES}
    )

    if (RESOURCE_COUNT)
      set(CONFIG_INSTALL "${CROSS_ROOT}/cmake/android/install.res+exe.sh.in")
    endif()

  elseif (RUN MATCHES APK)
    add_definitions(-DCHR_FS_APK)

    configure_file("${CROSS_ROOT}/cmake/android/AndroidManifest.xml.in" AndroidManifest.xml)
    configure_file("${CROSS_ROOT}/cmake/android/ant.properties.in" ant.properties)
    configure_file("${CROSS_ROOT}/cmake/android/MainActivity.java.in" "src/org/chronotext/${PROJECT_NAME}/MainActivity.java")

    configure_file("${CROSS_ROOT}/cmake/android/tests/AndroidManifest.xml.in" tests/AndroidManifest.xml)
    configure_file("${CROSS_ROOT}/cmake/android/tests/ant.properties.in" tests/ant.properties)
    configure_file("${CROSS_ROOT}/cmake/android/tests/MainActivityTests.java.in" "tests/src/org/chronotext/${PROJECT_NAME}/MainActivityTests.java")

    set(LIBRARY_OUTPUT_PATH "libs/${ANDROID_ABI}")

    add_library(${PROJECT_NAME} SHARED
      ${SRC_FILES}
      "${CROSS_ROOT}/src/Bridge.cpp"
    )
  endif()

elseif (PLATFORM MATCHES emscripten)
  if (FS MATCHES JS_EMBED)
    add_definitions(-DCHR_FS_JS_EMBED)

    if (RESOURCE_COUNT)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --exclude-file *.DS_Store --embed-file ../../res")
    endif()
  
    add_executable(${PROJECT_NAME}
      ${SRC_FILES}
    )

  else()
    add_definitions(-DCHR_FS_JS_NODE)

    add_executable(${PROJECT_NAME}
      ${SRC_FILES}
    )

    if (RESOURCE_COUNT)
      em_link_pre_js(${PROJECT_NAME} "${CROSS_ROOT}/cmake/emscripten/pre.js")
    endif()
  endif()

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

elseif (PLATFORM MATCHES osx)
  if (RESOURCE_COUNT)
    set(CONFIG_INSTALL "${CROSS_ROOT}/cmake/install.symlink.sh.in")
  endif()

  add_executable(${PROJECT_NAME}
    ${SRC_FILES}
  )
endif()

# ---

if (PLATFORM MATCHES ios|osx)
  list(APPEND LIBRARIES
    "-framework Foundation"
  )

elseif (PLATFORM MATCHES android AND RUN MATCHES APK)
  list(APPEND LIBRARIES
    log
    android
  )
endif()

include_directories(
  ${INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
  ${LIBRARIES}
)

# ---

enable_testing()

if (CONFIG_RUN AND CONFIG_INSTALL)
  configure_file(${CONFIG_RUN} run.sh)
  configure_file(${CONFIG_INSTALL} install.sh)

  install(CODE "
    execute_process(COMMAND ./install.sh RESULT_VARIABLE result)
    if (result)
      message(FATAL_ERROR)
    endif()
  ")
endif()
