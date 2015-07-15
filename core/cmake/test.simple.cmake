cmake_minimum_required (VERSION 3.2.2)

list(APPEND INCLUDE_DIRS
  "${CROSS_ROOT}/src"
)

list(APPEND SRC_FILES
  "${CROSS_ROOT}/src/Log.cpp"
  "${CROSS_ROOT}/src/Platform.cpp"
)

file(GLOB_RECURSE RESOURCES_FILES
  RELATIVE ${CMAKE_SOURCE_DIR}
  res/[^.]*
)

# ---

if (PLATFORM MATCHES emscripten)
  add_definitions(-DCHR_FS_JS_NODE)

  add_executable(${PROJECT_NAME}
    ${SRC_FILES}
  )

  if (RESOURCE_FILES)
    em_link_pre_js(${PROJECT_NAME} "${CROSS_ROOT}/cmake/emscripten/pre.js")
  endif()

elseif (PLATFORM MATCHES ios)
  add_definitions(-DCHR_FS_BUNDLE)

  add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    ${RESOURCES_FILES}
  )

  foreach (resource_file ${RESOURCES_FILES})
    get_filename_component(parent_dir ${resource_file} DIRECTORY)
    set_source_files_properties(${resource_file} PROPERTIES MACOSX_PACKAGE_LOCATION ${PROJECT_NAME}.app/${parent_dir})
  endforeach()

else()
  add_executable(${PROJECT_NAME}
    ${SRC_FILES}
  )

endif()

if (PLATFORM MATCHES android)
  if (RESOURCE_FILES)
    configure_file("${CROSS_ROOT}/cmake/android/install.res+exe.sh.in" install.sh)
  else()
    configure_file("${CROSS_ROOT}/cmake/android/install.exe.sh.in" install.sh)
  endif()

elseif (PLATFORM MATCHES osx|mxe)
  configure_file("${CROSS_ROOT}/cmake/install.symlink.sh.in" install.sh)
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

enable_testing()
