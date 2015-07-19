#include "Log.h"
#include "Platform.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG

#include "stb_image.h"

using namespace std;

int main(int argc, const char *argv[])
{
  for (int i = 0; i < argc; i++)
  {
    LOGI << "{" << argv[i] << "}" << endl;
  }

  // ---

  fs::path path1 = "sub/credits.txt";
  fs::path path2 = "2008.547.1crop_4.jpg";

  if (chr::hasFileResources())
  {
    auto resPath1 = chr::getResourcePath("sub/credits.txt");
    fs::ifstream in(resPath1, ios::in | ios::binary | ios::ate);
    
    if (in)
    {
      auto fileSize = in.tellg();
      in.seekg(0, ios::beg);

      string result(fileSize, 0);
      in.read(&result[0], fileSize);

      LOGI << "[" << result << "]" << endl;
    }
    else
    {
      LOGE << "ERROR WITH " << path1 << endl;
    }

    // ---

    auto resPath2 = chr::getResourcePath("2008.547.1crop_4.jpg");

    int x, y, comp;
    stbi_uc *data = stbi_load(resPath2.string().data(), &x, &y, &comp, 0);

    if (data)
    {
      LOGI << x << "x" << y << " (" << comp << ")" << endl;
      stbi_image_free(data);
    }
    else
    {
      LOGE << "stbi ERROR WITH " << path2 << endl;
    }
  }

  return 0;
}
