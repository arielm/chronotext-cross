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

if (PLATFORM MATCHES emscripten)
  add_definitions(-DCHR_FS_JS_NODE)

  add_executable(${PROJECT_NAME}
    ${SRC_FILES}
  )

  if (RESOURCE_COUNT)
    em_link_pre_js(${PROJECT_NAME} "${CROSS_ROOT}/cmake/emscripten/pre.js")
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

else()
  add_executable(${PROJECT_NAME}
    ${SRC_FILES}
  )

endif()

if (PLATFORM MATCHES android)
  if (RESOURCE_COUNT)
    set(CONFIG_INSTALL "${CROSS_ROOT}/cmake/android/install.res+exe.sh.in")
  else()
    set(CONFIG_INSTALL "${CROSS_ROOT}/cmake/android/install.exe.sh.in")
  endif()

elseif (PLATFORM MATCHES osx|mxe)
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
else()
  target_link_libraries(${PROJECT_NAME}
    ${LIBRARIES}
  )
endif()

# ---

setupTesting()
