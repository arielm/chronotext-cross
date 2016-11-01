#include "chr/FileSystem.h"

#include <gtest/gtest.h>

#include "hb-ft.h"
#include "hb-ot.h"

/* Unit tests for hb-ot-math.h - OpenType MATH table  */

static FT_Library ft_library;
static FT_Face ft_face;
static hb_font_t *hb_font;
static hb_face_t *hb_face;

static inline void initFreeType (void)
{
  FT_Error ft_error;
  if ((ft_error = FT_Init_FreeType (&ft_library)))
  {
    ADD_FAILURE();
  }
}

static inline void cleanupFreeType (void)
{
  FT_Done_FreeType (ft_library);
}

static void openFont(const fs::path &relativePath)
{
  auto resPath = chr::getResourceFilePath(relativePath);

  FT_Error ft_error;
  if ((ft_error = FT_New_Face (ft_library, resPath.string().data(), 0, &ft_face)))
  {
    ADD_FAILURE();
  }

  if ((ft_error = FT_Set_Char_Size (ft_face, 2000, 1000, 0, 0)))
  {
    ADD_FAILURE();
  }

  hb_font = hb_ft_font_create (ft_face, NULL);
  hb_face = hb_ft_face_create_cached(ft_face);
}

static inline void closeFont (void)
{
  hb_font_destroy (hb_font);
  FT_Done_Face (ft_face);
}

TEST(TestOTMath, test_has_data)
{
  initFreeType();

  openFont("fonts/MathTestFontNone.otf");
  EXPECT_TRUE(!hb_ot_math_has_data (hb_face)); // MATH table not available
  closeFont();

  openFont("fonts/MathTestFontEmpty.otf");
  EXPECT_TRUE(hb_ot_math_has_data (hb_face)); // MATH table available
  closeFont();

  cleanupFreeType();
}

TEST(TestOTMath, test_get_constant)
{
  initFreeType();

  openFont("fonts/MathTestFontEmpty.otf");
  EXPECT_EQ(hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT), 0); // MathConstants not available
  closeFont();

  openFont("fonts/MathTestFontFull.otf");
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT)), 100);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_DISPLAY_OPERATOR_MIN_HEIGHT)), 200);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_MATH_LEADING)), 300);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_AXIS_HEIGHT)), 400);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_ACCENT_BASE_HEIGHT)), 500);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_FLATTENED_ACCENT_BASE_HEIGHT)), 600);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_SUBSCRIPT_SHIFT_DOWN)), 700);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_SUBSCRIPT_TOP_MAX)), 800);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_SUBSCRIPT_BASELINE_DROP_MIN)), 900);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP)), 1100);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP_CRAMPED)), 1200);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MIN)), 1300);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_SUPERSCRIPT_BASELINE_DROP_MAX)), 1400);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_SUB_SUPERSCRIPT_GAP_MIN)), 1500);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MAX_WITH_SUBSCRIPT)), 1600);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_SPACE_AFTER_SCRIPT)), 3400);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_UPPER_LIMIT_GAP_MIN)), 1800);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_UPPER_LIMIT_BASELINE_RISE_MIN)), 1900);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_LOWER_LIMIT_GAP_MIN)), 2200);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_LOWER_LIMIT_BASELINE_DROP_MIN)), 2300);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_STACK_TOP_SHIFT_UP)), 2400);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_STACK_TOP_DISPLAY_STYLE_SHIFT_UP)), 2500);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_STACK_BOTTOM_SHIFT_DOWN)), 2600);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_STACK_BOTTOM_DISPLAY_STYLE_SHIFT_DOWN)), 2700);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_STACK_GAP_MIN)), 2800);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_STACK_DISPLAY_STYLE_GAP_MIN)), 2900);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_STRETCH_STACK_TOP_SHIFT_UP)), 3000);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_STRETCH_STACK_BOTTOM_SHIFT_DOWN)), 3100);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_STRETCH_STACK_GAP_ABOVE_MIN)), 3200);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_STRETCH_STACK_GAP_BELOW_MIN)), 3300);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_SHIFT_UP)), 3400);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_DISPLAY_STYLE_SHIFT_UP)), 3500);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_SHIFT_DOWN)), 3600);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_DISPLAY_STYLE_SHIFT_DOWN)), 3700);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_GAP_MIN)), 3800);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_FRACTION_NUM_DISPLAY_STYLE_GAP_MIN)), 3900);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_FRACTION_RULE_THICKNESS)), 4000);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_GAP_MIN)), 4100);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_FRACTION_DENOM_DISPLAY_STYLE_GAP_MIN)), 4200);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_SKEWED_FRACTION_HORIZONTAL_GAP)), 8600);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_SKEWED_FRACTION_VERTICAL_GAP)), 4400);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_OVERBAR_VERTICAL_GAP)), 4500);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_OVERBAR_RULE_THICKNESS)), 4600);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_OVERBAR_EXTRA_ASCENDER)), 4700);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_UNDERBAR_VERTICAL_GAP)), 4800);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_UNDERBAR_RULE_THICKNESS)), 4900);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_UNDERBAR_EXTRA_DESCENDER)), 5000);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_RADICAL_VERTICAL_GAP)), 5100);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_RADICAL_DISPLAY_STYLE_VERTICAL_GAP)), 5200);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_RADICAL_RULE_THICKNESS)), 5300);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_RADICAL_EXTRA_ASCENDER)), 5400);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_RADICAL_KERN_BEFORE_DEGREE)), 11000);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_RADICAL_KERN_AFTER_DEGREE)), 11200);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN)), 87);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_SCRIPT_SCRIPT_PERCENT_SCALE_DOWN)), 76);
  EXPECT_EQ((hb_ot_math_get_constant (hb_font, HB_OT_MATH_CONSTANT_RADICAL_DEGREE_BOTTOM_RAISE_PERCENT)), 65);
  closeFont();

  cleanupFreeType();
}

TEST(TestOTMath, test_get_glyph_italics_correction)
{
  hb_codepoint_t glyph;
  initFreeType();

  openFont("fonts/MathTestFontEmpty.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_italics_correction (hb_font, glyph), 0); // MathGlyphInfo not available
  closeFont();

  openFont("fonts/MathTestFontPartial1.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_italics_correction (hb_font, glyph), 0); // MathGlyphInfo empty
  closeFont();

  openFont("fonts/MathTestFontPartial2.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_italics_correction (hb_font, glyph), 0); // MathItalicsCorrectionInfo empty
  closeFont();

  openFont("fonts/MathTestFontFull.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_italics_correction (hb_font, glyph), 0); // Glyph without italic correction.
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "A", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_italics_correction (hb_font, glyph), 394);
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "B", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_italics_correction (hb_font, glyph), 300);
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "C", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_italics_correction (hb_font, glyph), 904);
  closeFont();

  cleanupFreeType();
}

TEST(TestOTMath, test_get_glyph_top_accent_attachment)
{
  hb_codepoint_t glyph;
  initFreeType();

  openFont("fonts/MathTestFontEmpty.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_top_accent_attachment (hb_font, glyph), 1000); // MathGlyphInfo not available
  closeFont();

  openFont("fonts/MathTestFontPartial1.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_top_accent_attachment (hb_font, glyph), 1000); // MathGlyphInfo empty
  closeFont();

  openFont("fonts/MathTestFontPartial2.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_top_accent_attachment (hb_font, glyph), 1000); // MathTopAccentAttachment empty
  closeFont();

  openFont("fonts/MathTestFontFull.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_top_accent_attachment (hb_font, glyph), 1000); // Glyph without top accent attachment.
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "D", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_top_accent_attachment (hb_font, glyph), 748);
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "E", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_top_accent_attachment (hb_font, glyph), 692);
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "F", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_top_accent_attachment (hb_font, glyph), 636);
  closeFont();

  cleanupFreeType();
}

TEST(TestOTMath, test_is_glyph_extended_shape)
{
  hb_codepoint_t glyph;
  initFreeType();

  openFont("fonts/MathTestFontEmpty.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_TRUE(!hb_ot_math_is_glyph_extended_shape (hb_face, glyph)); // MathGlyphInfo not available
  closeFont();

  openFont("fonts/MathTestFontPartial1.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_TRUE(!hb_ot_math_is_glyph_extended_shape (hb_face, glyph)); // MathGlyphInfo empty
  closeFont();

  openFont("fonts/MathTestFontFull.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "G", -1, &glyph));
  EXPECT_TRUE(!hb_ot_math_is_glyph_extended_shape (hb_face, glyph));
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "H", -1, &glyph));
  EXPECT_TRUE(hb_ot_math_is_glyph_extended_shape (hb_face, glyph));
  closeFont();

  cleanupFreeType();
}

TEST(TestOTMath, test_get_glyph_kerning)
{
  hb_codepoint_t glyph;
  initFreeType();

  openFont("fonts/MathTestFontEmpty.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_kerning (hb_font, glyph, HB_OT_MATH_KERN_TOP_RIGHT, 0), 0); // MathGlyphInfo not available
  EXPECT_EQ(hb_ot_math_get_glyph_kerning (hb_font, glyph, HB_OT_MATH_KERN_TOP_LEFT, 0), 0); // MathGlyphInfo not available
  EXPECT_EQ(hb_ot_math_get_glyph_kerning (hb_font, glyph, HB_OT_MATH_KERN_BOTTOM_RIGHT, 0), 0); // MathGlyphInfo not available
  EXPECT_EQ(hb_ot_math_get_glyph_kerning (hb_font, glyph, HB_OT_MATH_KERN_BOTTOM_LEFT, 0), 0); // MathGlyphInfo not available
  closeFont();

  openFont("fonts/MathTestFontPartial2.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_kerning (hb_font, glyph, HB_OT_MATH_KERN_TOP_RIGHT, 0), 0); // MathKernInfo empty
  EXPECT_EQ(hb_ot_math_get_glyph_kerning (hb_font, glyph, HB_OT_MATH_KERN_TOP_LEFT, 0), 0); // MathKernInfo empty
  EXPECT_EQ(hb_ot_math_get_glyph_kerning (hb_font, glyph, HB_OT_MATH_KERN_BOTTOM_RIGHT, 0), 0); // MathKernInfo empty
  EXPECT_EQ(hb_ot_math_get_glyph_kerning (hb_font, glyph, HB_OT_MATH_KERN_BOTTOM_LEFT, 0), 0); // MathKernInfo empty
  closeFont();

  openFont("fonts/MathTestFontPartial3.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_kerning (hb_font, glyph, HB_OT_MATH_KERN_TOP_RIGHT, 0), 0); // MathKernInfoRecords empty
  EXPECT_EQ(hb_ot_math_get_glyph_kerning (hb_font, glyph, HB_OT_MATH_KERN_TOP_LEFT, 0), 0); // MathKernInfoRecords empty
  EXPECT_EQ(hb_ot_math_get_glyph_kerning (hb_font, glyph, HB_OT_MATH_KERN_BOTTOM_RIGHT, 0), 0); // MathKernInfoRecords empty
  EXPECT_EQ(hb_ot_math_get_glyph_kerning (hb_font, glyph, HB_OT_MATH_KERN_BOTTOM_LEFT, 0), 0); // MathKernInfoRecords empty
  closeFont();

  openFont("fonts/MathTestFontFull.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "I", -1, &glyph));

  EXPECT_EQ(hb_ot_math_get_glyph_kerning (hb_font, glyph, HB_OT_MATH_KERN_TOP_RIGHT, 7), 62); // lower than min heigth
  EXPECT_EQ(hb_ot_math_get_glyph_kerning (hb_font, glyph, HB_OT_MATH_KERN_TOP_RIGHT, 14), 62); // equal to min height
  EXPECT_EQ(hb_ot_math_get_glyph_kerning (hb_font, glyph, HB_OT_MATH_KERN_TOP_RIGHT, 20), 104);
  EXPECT_EQ(hb_ot_math_get_glyph_kerning (hb_font, glyph, HB_OT_MATH_KERN_TOP_RIGHT, 23), 104);
  EXPECT_EQ(hb_ot_math_get_glyph_kerning (hb_font, glyph, HB_OT_MATH_KERN_TOP_RIGHT, 31), 146);
  EXPECT_EQ(hb_ot_math_get_glyph_kerning (hb_font, glyph, HB_OT_MATH_KERN_TOP_RIGHT, 32), 146);
  EXPECT_EQ(hb_ot_math_get_glyph_kerning (hb_font, glyph, HB_OT_MATH_KERN_TOP_RIGHT, 86), 398); // equal to max height
  EXPECT_EQ(hb_ot_math_get_glyph_kerning (hb_font, glyph, HB_OT_MATH_KERN_TOP_RIGHT, 91), 440); // larger than max height
  EXPECT_EQ(hb_ot_math_get_glyph_kerning (hb_font, glyph, HB_OT_MATH_KERN_TOP_RIGHT, 96), 440); // larger than max height

  EXPECT_EQ(hb_ot_math_get_glyph_kerning (hb_font, glyph, HB_OT_MATH_KERN_TOP_RIGHT, 39), 188); // top right
  EXPECT_EQ(hb_ot_math_get_glyph_kerning (hb_font, glyph, HB_OT_MATH_KERN_TOP_LEFT, 39), 110); // top left
  EXPECT_EQ(hb_ot_math_get_glyph_kerning (hb_font, glyph, HB_OT_MATH_KERN_BOTTOM_RIGHT, 39), 44); // bottom right
  EXPECT_EQ(hb_ot_math_get_glyph_kerning (hb_font, glyph, HB_OT_MATH_KERN_BOTTOM_LEFT, 39), 100); // bottom left

  closeFont();

  cleanupFreeType();
}


static hb_position_t
get_glyph_assembly_italics_correction (hb_font_t *font,
                                       hb_codepoint_t glyph,
                                       hb_bool_t horizontal)
{
  hb_position_t corr;
  hb_ot_math_get_glyph_assembly (font, glyph,
                                 horizontal ? HB_DIRECTION_LTR : HB_DIRECTION_TTB,
                                 0, NULL, NULL,
                                 &corr);
  return corr;
}

TEST(TestOTMath, test_get_glyph_assembly_italics_correction)
{
  hb_codepoint_t glyph;
  initFreeType();

  openFont("fonts/MathTestFontEmpty.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_EQ(get_glyph_assembly_italics_correction (hb_font, glyph, true), 0); // MathVariants not available
  EXPECT_EQ(get_glyph_assembly_italics_correction (hb_font, glyph, false), 0); // MathVariants not available
  closeFont();

  openFont("fonts/MathTestFontPartial1.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_EQ(get_glyph_assembly_italics_correction (hb_font, glyph, true), 0); // VertGlyphCoverage and HorizGlyphCoverage absent
  EXPECT_EQ(get_glyph_assembly_italics_correction (hb_font, glyph, false), 0); // VertGlyphCoverage and HorizGlyphCoverage absent
  closeFont();

  openFont("fonts/MathTestFontPartial2.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_EQ(get_glyph_assembly_italics_correction (hb_font, glyph, true), 0); // VertGlyphCoverage and HorizGlyphCoverage empty
  EXPECT_EQ(get_glyph_assembly_italics_correction (hb_font, glyph, false), 0); // VertGlyphCoverage and HorizGlyphCoverage empty
  closeFont();

  openFont("fonts/MathTestFontPartial3.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_EQ(get_glyph_assembly_italics_correction (hb_font, glyph, true), 0); // HorizGlyphConstruction and VertGlyphConstruction empty
  EXPECT_EQ(get_glyph_assembly_italics_correction (hb_font, glyph, false), 0);  // HorizGlyphConstruction and VertGlyphConstruction empty
  closeFont();

  openFont("fonts/MathTestFontPartial4.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_EQ(get_glyph_assembly_italics_correction (hb_font, glyph, true), 0);
  EXPECT_EQ(get_glyph_assembly_italics_correction (hb_font, glyph, false), 0);
  closeFont();

  openFont("fonts/MathTestFontFull.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "arrowleft", -1, &glyph));
  EXPECT_EQ(get_glyph_assembly_italics_correction (hb_font, glyph, true), 248);
  EXPECT_EQ(get_glyph_assembly_italics_correction (hb_font, glyph, false), 0);
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "arrowup", -1, &glyph));
  EXPECT_EQ(get_glyph_assembly_italics_correction (hb_font, glyph, true), 0);
  EXPECT_EQ(get_glyph_assembly_italics_correction (hb_font, glyph, false), 662);
  closeFont();

  cleanupFreeType();
}

TEST(TestOTMath, test_get_min_connector_overlap)
{
  initFreeType();

  openFont("fonts/MathTestFontEmpty.otf");
  EXPECT_EQ(hb_ot_math_get_min_connector_overlap(hb_font, HB_DIRECTION_LTR), 0); // MathVariants not available
  EXPECT_EQ(hb_ot_math_get_min_connector_overlap(hb_font, HB_DIRECTION_TTB), 0); // MathVariants not available
  closeFont();

  openFont("fonts/MathTestFontPartial1.otf");
  EXPECT_EQ(hb_ot_math_get_min_connector_overlap(hb_font, HB_DIRECTION_LTR), 108);
  EXPECT_EQ(hb_ot_math_get_min_connector_overlap(hb_font, HB_DIRECTION_TTB), 54);
  closeFont();

  cleanupFreeType();
}

TEST(TestOTMath, test_get_glyph_variants)
{
  hb_codepoint_t glyph;
  initFreeType();

  openFont("fonts/MathTestFontEmpty.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_variants (hb_font, glyph, HB_DIRECTION_RTL, 0, NULL, NULL), 0);
  EXPECT_EQ(hb_ot_math_get_glyph_variants (hb_font, glyph, HB_DIRECTION_BTT, 0, NULL, NULL), 0);
  closeFont();

  openFont("fonts/MathTestFontPartial1.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_variants (hb_font, glyph, HB_DIRECTION_RTL, 0, NULL, NULL), 0);
  EXPECT_EQ(hb_ot_math_get_glyph_variants (hb_font, glyph, HB_DIRECTION_BTT, 0, NULL, NULL), 0);
  closeFont();

  openFont("fonts/MathTestFontPartial2.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_variants (hb_font, glyph, HB_DIRECTION_RTL, 0, NULL, NULL), 0);
  EXPECT_EQ(hb_ot_math_get_glyph_variants (hb_font, glyph, HB_DIRECTION_BTT, 0, NULL, NULL), 0);
  closeFont();

  openFont("fonts/MathTestFontPartial3.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_variants (hb_font, glyph, HB_DIRECTION_RTL, 0, NULL, NULL), 0);
  EXPECT_EQ(hb_ot_math_get_glyph_variants (hb_font, glyph, HB_DIRECTION_BTT, 0, NULL, NULL), 0);
  closeFont();

  openFont("fonts/MathTestFontPartial4.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_variants (hb_font, glyph, HB_DIRECTION_RTL, 0, NULL, NULL), 0);
  EXPECT_EQ(hb_ot_math_get_glyph_variants (hb_font, glyph, HB_DIRECTION_BTT, 0, NULL, NULL), 0);
  closeFont();

  openFont("fonts/MathTestFontFull.otf");

  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "arrowleft", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_variants (hb_font,
                                                 glyph,
                                                 HB_DIRECTION_BTT,
                                                 0,
                                                 NULL,
                                                 NULL), 0);
  EXPECT_EQ(hb_ot_math_get_glyph_variants (hb_font,
                                                 glyph,
                                                 HB_DIRECTION_RTL,
                                                 0,
                                                 NULL,
                                                 NULL), 3);

  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "arrowup", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_variants (hb_font,
                                                 glyph,
                                                 HB_DIRECTION_BTT,
                                                 0,
                                                 NULL,
                                                 NULL), 4);
  EXPECT_EQ(hb_ot_math_get_glyph_variants (hb_font,
                                                 glyph,
                                                 HB_DIRECTION_RTL,
                                                 0,
                                                 NULL,
                                                 NULL), 0);

  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "arrowleft", -1, &glyph));
  hb_ot_math_glyph_variant_t variants[20];
  unsigned variantsSize = sizeof (variants) / sizeof (variants[0]);
  unsigned int count;
  unsigned int offset = 0;
  do {
    count = variantsSize;
    hb_ot_math_get_glyph_variants (hb_font,
                                   glyph,
                                   HB_DIRECTION_RTL,
                                   offset,
                                   &count,
                                   variants);
    offset += count;
  } while (count == variantsSize);
  EXPECT_EQ(offset, 3);
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "uni2190_size2", -1, &glyph));
  EXPECT_EQ(variants[0].glyph, glyph);
  EXPECT_EQ(variants[0].advance, 4302);
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "uni2190_size3", -1, &glyph));
  EXPECT_EQ(variants[1].glyph, glyph);
  EXPECT_EQ(variants[1].advance, 4802);
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "uni2190_size4", -1, &glyph));
  EXPECT_EQ(variants[2].glyph, glyph);
  EXPECT_EQ(variants[2].advance, 5802);

  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "arrowup", -1, &glyph));
  offset = 0;
  do {
    count = variantsSize;
    hb_ot_math_get_glyph_variants (hb_font,
                                   glyph,
                                   HB_DIRECTION_BTT,
                                   offset,
                                   &count,
                                   variants);
    offset += count;
  } while (count == variantsSize);
  EXPECT_EQ(offset, 4);
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "uni2191_size2", -1, &glyph));
  EXPECT_EQ(variants[0].glyph, glyph);
  EXPECT_EQ(variants[0].advance, 2251);
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "uni2191_size3", -1, &glyph));
  EXPECT_EQ(variants[1].glyph, glyph);
  EXPECT_EQ(variants[1].advance, 2501);
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "uni2191_size4", -1, &glyph));
  EXPECT_EQ(variants[2].glyph, glyph);
  EXPECT_EQ(variants[2].advance, 3001);
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "uni2191_size5", -1, &glyph));
  EXPECT_EQ(variants[3].glyph, glyph);
  EXPECT_EQ(variants[3].advance, 3751);

  closeFont();

  cleanupFreeType();
}

TEST(TestOTMath, test_get_glyph_assembly)
{
  hb_codepoint_t glyph;
  initFreeType();

  openFont("fonts/MathTestFontEmpty.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_assembly (hb_font, glyph, HB_DIRECTION_RTL, 0, NULL, NULL, NULL), 0);
  EXPECT_EQ(hb_ot_math_get_glyph_assembly (hb_font, glyph, HB_DIRECTION_BTT, 0, NULL, NULL, NULL), 0);
  closeFont();

  openFont("fonts/MathTestFontPartial1.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_assembly (hb_font, glyph, HB_DIRECTION_RTL, 0, NULL, NULL, NULL), 0);
  EXPECT_EQ(hb_ot_math_get_glyph_assembly (hb_font, glyph, HB_DIRECTION_BTT, 0, NULL, NULL, NULL), 0);
  closeFont();

  openFont("fonts/MathTestFontPartial2.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_assembly (hb_font, glyph, HB_DIRECTION_RTL, 0, NULL, NULL, NULL), 0);
  EXPECT_EQ(hb_ot_math_get_glyph_assembly (hb_font, glyph, HB_DIRECTION_BTT, 0, NULL, NULL, NULL), 0);
  closeFont();

  openFont("fonts/MathTestFontPartial3.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_assembly (hb_font, glyph, HB_DIRECTION_RTL, 0, NULL, NULL, NULL), 0);
  EXPECT_EQ(hb_ot_math_get_glyph_assembly (hb_font, glyph, HB_DIRECTION_BTT, 0, NULL, NULL, NULL), 0);
  closeFont();

  openFont("fonts/MathTestFontPartial4.otf");
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "space", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_assembly (hb_font, glyph, HB_DIRECTION_RTL, 0, NULL, NULL, NULL), 0);
  EXPECT_EQ(hb_ot_math_get_glyph_assembly (hb_font, glyph, HB_DIRECTION_BTT, 0, NULL, NULL, NULL), 0);
  closeFont();

  openFont("fonts/MathTestFontFull.otf");

  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "arrowright", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_assembly (hb_font,
                                                 glyph,
                                                 HB_DIRECTION_BTT,
                                                 0,
                                                 NULL,
                                                 NULL,
                                                 NULL), 0);
  EXPECT_EQ(hb_ot_math_get_glyph_assembly (hb_font,
                                                 glyph,
                                                 HB_DIRECTION_RTL,
                                                 0,
                                                 NULL,
                                                 NULL,
                                                 NULL), 3);

  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "arrowdown", -1, &glyph));
  EXPECT_EQ(hb_ot_math_get_glyph_assembly (hb_font,
                                                 glyph,
                                                 HB_DIRECTION_BTT,
                                                 0,
                                                 NULL,
                                                 NULL,
                                                 NULL), 5);
  EXPECT_EQ(hb_ot_math_get_glyph_assembly (hb_font,
                                                 glyph,
                                                 HB_DIRECTION_RTL,
                                                 0,
                                                 NULL,
                                                 NULL,
                                                 NULL), 0);

  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "arrowright", -1, &glyph));
  hb_ot_math_glyph_part_t parts[20];
  unsigned partsSize = sizeof (parts) / sizeof (parts[0]);
  unsigned int count;
  unsigned int offset = 0;
  do {
    count = partsSize;
    hb_ot_math_get_glyph_assembly (hb_font,
                                   glyph,
                                   HB_DIRECTION_RTL,
                                   offset,
                                   &count,
                                   parts,
                                   NULL);
    offset += count;
  } while (count == partsSize);
  EXPECT_EQ(offset, 3);
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "left", -1, &glyph));
  EXPECT_EQ(parts[0].glyph, glyph);
  EXPECT_EQ(parts[0].start_connector_length, 800);
  EXPECT_EQ(parts[0].end_connector_length, 384);
  EXPECT_EQ(parts[0].full_advance, 2000);
  EXPECT_TRUE(!(parts[0].flags & HB_MATH_GLYPH_PART_FLAG_EXTENDER));
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "horizontal", -1, &glyph));
  EXPECT_EQ(parts[1].glyph, glyph);
  EXPECT_EQ(parts[1].start_connector_length, 524);
  EXPECT_EQ(parts[1].end_connector_length, 800);
  EXPECT_EQ(parts[1].full_advance, 2000);
  EXPECT_TRUE(parts[1].flags & HB_MATH_GLYPH_PART_FLAG_EXTENDER);
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "right", -1, &glyph));
  EXPECT_EQ(parts[2].glyph, glyph);
  EXPECT_EQ(parts[2].start_connector_length, 316);
  EXPECT_EQ(parts[2].end_connector_length, 454);
  EXPECT_EQ(parts[2].full_advance, 2000);
  EXPECT_TRUE(!(parts[2].flags & HB_MATH_GLYPH_PART_FLAG_EXTENDER));

  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "arrowdown", -1, &glyph));
  offset = 0;
  do {
    count = partsSize;
    hb_ot_math_get_glyph_assembly (hb_font,
                                   glyph,
                                   HB_DIRECTION_BTT,
                                   offset,
                                   &count,
                                   parts,
                                   NULL);
    offset += count;
  } while (count == partsSize);
  EXPECT_EQ(offset, 5);
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "bottom", -1, &glyph));
  EXPECT_EQ(parts[0].glyph, glyph);
  EXPECT_EQ(parts[0].start_connector_length, 365);
  EXPECT_EQ(parts[0].end_connector_length, 158);
  EXPECT_EQ(parts[0].full_advance, 1000);
  EXPECT_TRUE(!(parts[0].flags & HB_MATH_GLYPH_PART_FLAG_EXTENDER));
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "vertical", -1, &glyph));
  EXPECT_EQ(parts[1].glyph, glyph);
  EXPECT_EQ(parts[1].glyph, glyph);
  EXPECT_EQ(parts[1].start_connector_length, 227);
  EXPECT_EQ(parts[1].end_connector_length, 365);
  EXPECT_EQ(parts[1].full_advance, 1000);
  EXPECT_TRUE(parts[1].flags & HB_MATH_GLYPH_PART_FLAG_EXTENDER);
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "center", -1, &glyph));
  EXPECT_EQ(parts[2].glyph, glyph);
  EXPECT_EQ(parts[2].start_connector_length, 54);
  EXPECT_EQ(parts[2].end_connector_length, 158);
  EXPECT_EQ(parts[2].full_advance, 1000);
  EXPECT_TRUE(!(parts[2].flags & HB_MATH_GLYPH_PART_FLAG_EXTENDER));
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "vertical", -1, &glyph));
  EXPECT_EQ(parts[3].glyph, glyph);
  EXPECT_EQ(parts[3].glyph, glyph);
  EXPECT_EQ(parts[3].glyph, glyph);
  EXPECT_EQ(parts[3].start_connector_length, 400);
  EXPECT_EQ(parts[3].end_connector_length, 296);
  EXPECT_EQ(parts[3].full_advance, 1000);
  EXPECT_TRUE(parts[1].flags & HB_MATH_GLYPH_PART_FLAG_EXTENDER);
  EXPECT_TRUE(hb_font_get_glyph_from_name (hb_font, "top", -1, &glyph));
  EXPECT_EQ(parts[4].glyph, glyph);
  EXPECT_EQ(parts[4].start_connector_length, 123);
  EXPECT_EQ(parts[4].end_connector_length, 192);
  EXPECT_EQ(parts[4].full_advance, 1000);
  EXPECT_TRUE(!(parts[4].flags & HB_MATH_GLYPH_PART_FLAG_EXTENDER));

  closeFont();

  cleanupFreeType();
}
