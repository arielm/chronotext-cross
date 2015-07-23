
list(APPEND INCLUDE_DIRS
  "${CROSS_ROOT}/src"
)

list(APPEND SRC_FILES
  "${CROSS_ROOT}/src/Log.cpp"
  "${CROSS_ROOT}/src/Platform.cpp"
  "${CROSS_ROOT}/src/MemoryBuffer.cpp"
)

file(GLOB_RECURSE RESOURCE_FILES
  RELATIVE ${CMAKE_SOURCE_DIR}
  res/[^.]*
)

list(LENGTH RESOURCE_FILES RESOURCE_COUNT)

# ---

if (PLATFORM MATCHES mxe)
  add_definitions(-DCHR_RUN_EXE)

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
    add_definitions(-DCHR_FS_PURE)

    if (RESOURCE_COUNT)
      set(CONFIG_INSTALL "${CROSS_ROOT}/cmake/install.symlink.sh.in")
    endif()
  endif()

  add_executable(${PROJECT_NAME}
    ${SRC_FILES}
  )

elseif (PLATFORM MATCHES android)
  if (RUN MATCHES APK)
    add_definitions(-DCHR_RUN_APK)
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

  else()
    add_definitions(-DCHR_RUN_EXE)
    add_definitions(-DCHR_FS_PURE)

    add_executable(${PROJECT_NAME}
      ${SRC_FILES}
    )

    if (RESOURCE_COUNT)
      set(CONFIG_INSTALL "${CROSS_ROOT}/cmake/android/install.res+exe.sh.in")
    endif()
  endif()

elseif (PLATFORM MATCHES emscripten)
  if (FS MATCHES JS_EMBED)
    if (RUN MATCHES SPIDERMONKEY)
      add_definitions(-DCHR_RUN_SPIDERMONKEY)
    elseif (RUN MATCHES NODE)
      add_definitions(-DCHR_RUN_NODE)
    else()
      message(FATAL_ERROR "UNSUPPORTED RUN-MODE + FILE-SYSTEM!")
    endif()
        
    add_definitions(-DCHR_FS_JS_EMBED)

    if (RESOURCE_COUNT)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --embed-file ../../res")
    endif()
  
    add_executable(${PROJECT_NAME}
      ${SRC_FILES}
    )

  elseif (FS MATCHES JS_PRELOAD AND RUN MATCHES BROWSER)
    add_definitions(-DCHR_RUN_BROWSER)
    add_definitions(-DCHR_FS_JS_PRELOAD)

    configure_file("${CROSS_ROOT}/cmake/emscripten/template.html.in" template.html)
    set(CMAKE_EXECUTABLE_SUFFIX .html)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --emrun --shell-file template.html")

    if (RESOURCE_COUNT)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --preload-file ../../res")
    endif()
  
    add_executable(${PROJECT_NAME}
      ${SRC_FILES}
    )

  elseif (FS MATCHES JS_NODE AND RUN MATCHES NODE)
    add_definitions(-DCHR_RUN_NODE)
    add_definitions(-DCHR_FS_JS_NODE)

    add_executable(${PROJECT_NAME}
      ${SRC_FILES}
    )

    if (RESOURCE_COUNT)
      em_link_pre_js(${PROJECT_NAME} "${CROSS_ROOT}/cmake/emscripten/pre.js")
    endif()

  else()
    message(FATAL_ERROR "UNSUPPORTED RUN-MODE + FILE-SYSTEM!")
  endif()

elseif (PLATFORM MATCHES ios)
  add_definitions(-DCHR_RUN_APP)
  add_definitions(-DCHR_FS_BUNDLE)

  add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    ${RESOURCE_FILES}
  )

  foreach (resource_file ${RESOURCE_FILES})
    get_filename_component(parent_dir ${resource_file} DIRECTORY)
    set_source_files_properties(${resource_file} PROPERTIES MACOSX_PACKAGE_LOCATION ${PROJECT_NAME}.app/${parent_dir})
  endforeach()

  set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER org.chronotext.${PROJECT_NAME}
    MACOSX_BUNDLE_INFO_PLIST "${CROSS_ROOT}/cmake/ios/Info.plist.in"
    XCODE_ATTRIBUTE_INFOPLIST_PREPROCESS YES
  )

elseif (PLATFORM MATCHES osx)
  add_definitions(-DCHR_RUN_EXE)
  add_definitions(-DCHR_FS_PURE)

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
