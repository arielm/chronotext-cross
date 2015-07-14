cmake_minimum_required (VERSION 3.2.2)

list(APPEND INCLUDE_DIRS
  "${CHR_PATH}/src"
)

list(APPEND SRC_FILES
  "${CHR_PATH}/src/Log.cpp"
  "${CHR_PATH}/src/Platform.cpp"
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

  em_link_pre_js(${PROJECT_NAME} "${CHR_PATH}/cmake/emscripten/pre.js")

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
  configure_file("${CHR_PATH}/cmake/android/install.push.sh.in" install.sh)

elseif (PLATFORM MATCHES osx|mxe)
  configure_file("${CHR_PATH}/cmake/install.symlink.sh.in" install.sh)
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
