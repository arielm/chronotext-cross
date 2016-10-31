cmake_minimum_required(VERSION 3.2.2)

project(harfbuzz)

set(FREETYPE_ROOT "$ENV{CROSS_PATH}/deps/freetype/dist/${PLATFORM}")

add_definitions(-DHB_NO_MT)
add_definitions(-DHAVE_OT)
add_definitions(-DHAVE_UCDN)

add_library(${PROJECT_NAME} STATIC
  "src/hb-blob.cc"
  "src/hb-ft.cc"
  "src/hb-ot-shape-complex-default.cc"
  "src/hb-ot-shape-complex-use.cc"
  "src/hb-ucdn.cc"
  "src/hb-buffer-serialize.cc"
  "src/hb-ot-shape-complex-hangul.cc"
  "src/hb-ot-shape-fallback.cc"
  "src/hb-unicode.cc"
  "src/hb-buffer.cc"
  "src/hb-ot-shape-complex-hebrew.cc"
  "src/hb-ot-shape-normalize.cc"
  "src/hb-common.cc"
  "src/hb-ot-shape-complex-indic-table.cc"
  "src/hb-ot-shape.cc"
  "src/hb-warning.cc"
  "src/hb-ot-shape-complex-indic.cc"
  "src/hb-ot-tag.cc"
  "src/hb-ot-font.cc"
  "src/hb-ot-shape-complex-myanmar.cc"
  "src/hb-set.cc"
  "src/hb-face.cc"
  "src/hb-ot-layout.cc"
  "src/hb-ot-shape-complex-thai.cc"
  "src/hb-shape-plan.cc"
  "src/hb-fallback-shape.cc"
  "src/hb-ot-map.cc"
  "src/hb-ot-shape-complex-tibetan.cc"
  "src/hb-shape.cc"
  "src/hb-font.cc"
  "src/hb-ot-shape-complex-arabic.cc"
  "src/hb-ot-shape-complex-use-table.cc"
  "src/hb-shaper.cc"
  "src/hb-ucdn/ucdn.c"
)

include_directories(
	"${FREETYPE_ROOT}/include"
	"src"
	"src/hb-ucdn"
)

target_link_libraries(${PROJECT_NAME}
	"${FREETYPE_ROOT}/lib/libfreetype.a"
)
