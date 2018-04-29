set(CROSS_ROOT "$ENV{CROSS_PATH}/core")

if (NOT SKETCH)
  list(APPEND INCLUDE_DIRS
    "${CROSS_ROOT}/src"
  )

  list(APPEND SRC_FILES
    "${CROSS_ROOT}/src/chr/Log.cpp"
    "${CROSS_ROOT}/src/chr/InputSource.cpp"
    "${CROSS_ROOT}/src/chr/FileSystem.cpp"
    "${CROSS_ROOT}/src/chr/ResourceBuffer.cpp"
  )

  if (PLATFORM MATCHES android)
    list(APPEND SRC_FILES "${CROSS_ROOT}/src/chr/android/JNI.cpp")
  endif()
endif()

list(APPEND INCLUDE_DIRS
  "src"
)

file(GLOB_RECURSE RESOURCE_FILES
  RELATIVE ${CMAKE_SOURCE_DIR}
  res/[^.]*
)

list(LENGTH RESOURCE_FILES RESOURCE_COUNT)

# ---

if (PLATFORM MATCHES emscripten)
  set(ZLIB_INCLUDE_DIRS "$ENV{HOME}/.emscripten_cache/ports-builds/zlib")
  set(ZLIB_LIBRARIES "$ENV{HOME}/.emscripten_cache/zlib.bc")

elseif (PLATFORM MATCHES ios)
  set(ZLIB_INCLUDE_DIRS "/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk/usr/include")
  set(ZLIB_LIBRARIES "/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk/usr/lib/libz.tbd")

else()
  find_package(ZLIB)
endif()

list(APPEND INCLUDE_DIRS
  ${ZLIB_INCLUDE_DIRS}
)

list(APPEND LIBRARIES
  ${ZLIB_LIBRARIES}
)

# ---

if (PLATFORM MATCHES mxe)
  add_definitions(-DUNICODE -D_UNICODE)
  add_definitions(-DWIN32)
endif()

# ---

if (PLATFORM MATCHES mxe)
  if (RUN MATCHES EXE)
    add_definitions(-DCHR_RUN_EXE)

    set(CONFIG_INSTALL "${CROSS_ROOT}/cmake/install.nop.sh.in")
    set(CONFIG_RUN "${CROSS_ROOT}/cmake/mxe/run.sh.in")

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

  else()
    message(FATAL_ERROR "UNSUPPORTED RUN-MODE!")
  endif()

elseif (PLATFORM MATCHES android)
  if (RUN MATCHES APK)
    add_definitions(-DCHR_RUN_APK)
    add_definitions(-DCHR_FS_APK)

    set(CONFIG_INSTALL "${CROSS_ROOT}/cmake/android/install.apk.sh.in")
    set(CONFIG_RUN "${CROSS_ROOT}/cmake/android/run.apk.sh.in")

    configure_file("${CROSS_ROOT}/cmake/android/AndroidManifest.xml.in" AndroidManifest.xml)
    configure_file("${CROSS_ROOT}/cmake/android/ant.properties.in" ant.properties)

    set(LIBRARY_OUTPUT_PATH "libs/${ANDROID_ABI}")

    add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

  elseif (RUN MATCHES EXE)
    add_definitions(-DCHR_RUN_EXE)
    add_definitions(-DCHR_FS_PURE)

    set(CONFIG_RUN "${CROSS_ROOT}/cmake/android/run.exe.sh.in")

    if (RESOURCE_COUNT)
      set(CONFIG_INSTALL "${CROSS_ROOT}/cmake/android/install.res+exe.sh.in")
    else()
      set(CONFIG_INSTALL "${CROSS_ROOT}/cmake/android/install.exe.sh.in")
    endif()

    add_executable(${PROJECT_NAME}
      ${SRC_FILES}
    )

  else()
    message(FATAL_ERROR "UNSUPPORTED RUN-MODE!")
  endif()

elseif (PLATFORM MATCHES emscripten)
  set(CONFIG_INSTALL "${CROSS_ROOT}/cmake/install.nop.sh.in")

  if (RUN MATCHES NODE)
    set(CONFIG_RUN "${CROSS_ROOT}/cmake/emscripten/run.node.sh.in")

    if (NOT DEFINED FS)
      set(FS JS_NODE)
    endif()

  elseif (RUN MATCHES SPIDERMONKEY)
    set(CONFIG_RUN "${CROSS_ROOT}/cmake/emscripten/run.js.sh.in")

    if (NOT DEFINED FS)
      set(FS JS_EMBED)
    endif()

  elseif (RUN MATCHES BROWSER)
    set(CONFIG_RUN "${CROSS_ROOT}/cmake/emscripten/run.browser.sh.in")

    if (NOT DEFINED FS)
      set(FS JS_PRELOAD)
    endif()

  else()
    message(FATAL_ERROR "UNSUPPORTED RUN-MODE!")
  endif()

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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 --emrun --shell-file template.html --bind")

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
    message(FATAL_ERROR "UNSUPPORTED FILE-SYSTEM!")
  endif()

elseif (PLATFORM MATCHES ios)
  if (RUN MATCHES TEST)
    add_definitions(-DCHR_RUN_TEST)
    set(CONFIG_RUN "${CROSS_ROOT}/cmake/ios/run.test.sh.in")
  elseif (RUN MATCHES APP)
    add_definitions(-DCHR_RUN_APP)
    set(CONFIG_RUN "${CROSS_ROOT}/cmake/ios/run.app.sh.in")
  endif()

  if (RUN MATCHES TEST|APP)
    add_definitions(-DCHR_FS_BUNDLE)

    set(CONFIG_INSTALL "${CROSS_ROOT}/cmake/ios/install.sh.in")

    add_executable(${PROJECT_NAME}
      ${SRC_FILES}
      ${RESOURCE_FILES}
    )

    foreach (resource_file ${RESOURCE_FILES})
      set_source_files_properties(${resource_file} PROPERTIES MACOSX_PACKAGE_LOCATION ${PROJECT_NAME}.app)
    endforeach()

    set_target_properties(${PROJECT_NAME} PROPERTIES
      MACOSX_BUNDLE_GUI_IDENTIFIER org.chronotext.${PROJECT_NAME}
      MACOSX_BUNDLE_INFO_PLIST "${CROSS_ROOT}/cmake/ios/Info.plist.in"
      XCODE_ATTRIBUTE_INFOPLIST_PREPROCESS YES
    )

  else()
    message(FATAL_ERROR "UNSUPPORTED RUN-MODE!")
  endif()

elseif (PLATFORM MATCHES osx)
  if (RUN MATCHES EXE)
    add_definitions(-DCHR_RUN_EXE)
    add_definitions(-DCHR_FS_PURE)

    set(CONFIG_RUN "${CROSS_ROOT}/cmake/osx/run.sh.in")

    if (RESOURCE_COUNT)
      set(CONFIG_INSTALL "${CROSS_ROOT}/cmake/install.symlink.sh.in")
    else()
      set(CONFIG_INSTALL "${CROSS_ROOT}/cmake/install.nop.sh.in")
    endif()

    add_executable(${PROJECT_NAME}
      ${SRC_FILES}
    )

  else()
    message(FATAL_ERROR "UNSUPPORTED RUN-MODE!")
  endif()
endif()

# ---

if (NOT SKETCH)
  if (PLATFORM MATCHES ios|osx)
    list(APPEND LIBRARIES
      "-framework Foundation"
    )

  elseif (PLATFORM MATCHES android AND RUN MATCHES APK)
    list(APPEND LIBRARIES
      log
      android
      EGL
      GLESv2
    )
  endif()
else()
  if (PLATFORM MATCHES ios)
    list(APPEND LIBRARIES
      "-framework AVFoundation"
      "-framework AudioToolbox"
    )
  endif()
endif()

include_directories(
  ${INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
  ${LIBRARIES}
)

# ---

if (CONFIG_RUN AND CONFIG_INSTALL)
  configure_file(${CONFIG_RUN} run.sh)
  configure_file(${CONFIG_INSTALL} install.sh)

  if (CLION)
    add_custom_target(CLION_BUILD COMMAND ./install.sh)
  else()
    install(CODE "
      execute_process(COMMAND ./install.sh RESULT_VARIABLE result)
      if (result)
        message(FATAL_ERROR)
      endif()
    ")

    enable_testing()
    add_test(NAME TEST_ALL COMMAND ./run.sh)
  endif()
endif()
