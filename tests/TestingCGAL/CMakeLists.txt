cmake_minimum_required (VERSION 3.2.2)

project(TestingCGAL)

find_package(Boost 1.58.0 REQUIRED COMPONENTS system filesystem)

set(CGAL_INCLUDE_DIRS
  "${CGAL_ROOT}/include"
  "${CGAL_ROOT}/src/Boolean_set_operations_2/include"
  "${CGAL_ROOT}/src/Number_types/include"
  "${CGAL_ROOT}/src/Kernel_23/include"
  "${CGAL_ROOT}/src/Cartesian_Kernel/include"
  "${CGAL_ROOT}/src/Filtered_Kernel/include"
  "${CGAL_ROOT}/src/Installation/include"
  "${CGAL_ROOT}/src/STL_Extension/include"
  "${CGAL_ROOT}/src/Profiling_tools/include"
  "${CGAL_ROOT}/src/Stream_support/include"
  "${CGAL_ROOT}/src/Algebraic_foundations/include"
  "${CGAL_ROOT}/src/Interval_support/include"
  "${CGAL_ROOT}/src/Modular_arithmetic/include"
  "${CGAL_ROOT}/src/Conic_2/include"
  "${CGAL_ROOT}/src/Distance_2/include"
  "${CGAL_ROOT}/src/Distance_3/include"
  "${CGAL_ROOT}/src/Intersections_2/include"
  "${CGAL_ROOT}/src/Intersections_3/include"
  "${CGAL_ROOT}/src/Homogeneous_kernel/include"
  "${CGAL_ROOT}/src/Kernel_d/include"
  "${CGAL_ROOT}/src/Arithmetic_kernel/include"
  "${CGAL_ROOT}/src/Arrangement_on_surface_2/include"
  "${CGAL_ROOT}/src/Circulator/include"
  "${CGAL_ROOT}/src/HalfEdgeDS/include"
  "${CGAL_ROOT}/src/sweep_line_2/include"
  "${CGAL_ROOT}/src/Hash_map/include"
  "${CGAL_ROOT}/src/Union_find/include"
  "${CGAL_ROOT}/src/Polygon/include"
  "${CGAL_ROOT}/src/Polyhedron/include"
  "${CGAL_ROOT}/src/Polyhedron_IO/include"
  "${CGAL_ROOT}/src/Nef_S2/include"
  "${CGAL_ROOT}/src/Nef_2/include"
  "${CGAL_ROOT}/src/Nef_3/include"
  "${CGAL_ROOT}/src/Modifier/include"
  "${CGAL_ROOT}/src/Box_intersection_d/include"
  "${CGAL_ROOT}/src/Triangulation_2/include"
  "${CGAL_ROOT}/src/Triangulation_3/include"
  "${CGAL_ROOT}/src/TDS_2/include"
  "${CGAL_ROOT}/src/Spatial_sorting/include"
  "${CGAL_ROOT}/src/Property_map/include"
)
set(CGAL_LIBRARY "${CGAL_ROOT}/lib/libCGAL.a")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCGAL_HAS_NO_THREADS -DCGAL_NO_DEPRECATED_CODE")

set(INCLUDE_DIRS
  ${Boost_INCLUDE_DIRS}
  ${CGAL_INCLUDE_DIRS}
)

set(LIBRARIES
  ${Boost_LIBRARIES}
  ${CGAL_LIBRARY}
)

set(SRC_FILES
  "src/main.cpp"
  "src/Sketch.cpp"
  "src/BatchPolyhedron.cpp"
)

include("${CROSS_ROOT}/cmake/sketch.cmake")
include("${CROSS_ROOT}/cmake/test.cmake")
