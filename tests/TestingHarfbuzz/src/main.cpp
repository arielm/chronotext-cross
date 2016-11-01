#include "chr/FileSystem.h"

#include "hb-ft.h"
#include "hb-ot.h"

/* Unit tests for hb-ot-math.h - OpenType MATH table  */

static FT_Library ft_library;
static FT_Face ft_face;
static hb_font_t *hb_font;
static hb_face_t *hb_face;

static inline void
initFreeType (void)
{
  FT_Error ft_error;
  if ((ft_error = FT_Init_FreeType (&ft_library)))
    abort();
}

static inline void
cleanupFreeType (void)
{
  FT_Done_FreeType (ft_library);
}

static void
openFont(const char* path)
{
  FT_Error ft_error;
  if ((ft_error = FT_New_Face (ft_library, path, 0, &ft_face))) {
    abort();
  }

  if ((ft_error = FT_Set_Char_Size (ft_face, 2000, 1000, 0, 0)))
    abort();
  hb_font = hb_ft_font_create (ft_face, NULL);
  hb_face = hb_ft_face_create_cached(ft_face);
}

static inline void
closeFont (void)
{
  hb_font_destroy (hb_font);
  FT_Done_Face (ft_face);
}

static void
test_has_data (void)
{
  initFreeType();

  auto resPath = chr::getResourceFilePath("fonts/MathTestFontNone.otf");
  openFont(resPath.string().data());
  assert(!hb_ot_math_has_data (hb_face)); // MATH table not available
  closeFont();

  resPath = chr::getResourceFilePath("fonts/MathTestFontEmpty.otf");
  openFont(resPath.string().data());
  assert(hb_ot_math_has_data (hb_face)); // MATH table available
  closeFont();

  cleanupFreeType();
}

int main (int argc, char **argv)
{
  test_has_data();
  return 0;
}
