#include "Log.h"
#include "Platform.h"
#include "MemoryBuffer.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG

void* _malloc(size_t size);
void* _realloc(void *ptr, size_t size);
void _free(void *ptr);

#define STBI_MALLOC(sz)    _malloc(sz)
#define STBI_REALLOC(p,sz) _realloc(p,sz)
#define STBI_FREE(p)       _free(p)

#include "stb_image.h"

using namespace std;

void* _malloc(size_t size)
{
  LOGI << "malloc " << size << endl;
  return malloc(size);
}

void* _realloc(void *ptr, size_t size)
{
  LOGI << "realloc " << size << endl;
  return realloc(ptr, size);
}

void _free(void *ptr)
{
  LOGI << "free" << endl;
  free(ptr);
}

int main(int argc, const char *argv[])
{
  for (int i = 0; i < argc; i++)
  {
    LOGI << "{" << argv[i] << "}" << endl;
  }

  // ---

  fs::path path1 = "sub/credits.txt";
  fs::path path2 = "2008.547.1crop_4.jpg";

  if (chr::hasMemoryResources())
  {
    auto memoryBuffer1 = chr::getResourceBuffer(path1);

    if (memoryBuffer1)
    {
      LOGI << "[" << string(reinterpret_cast<const char*>(memoryBuffer1->data()), memoryBuffer1->size()) << "]" << endl;
    }
    else
    {
      LOGE << "ERROR WITH" << path1 << endl;
    }

    // ---

    auto memoryBuffer2 = chr::getResourceBuffer(path2);

    if (memoryBuffer2)
    {
      int x, y, comp;
      stbi_uc *data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(memoryBuffer2->data()), memoryBuffer2->size(), &x, &y, &comp, 0);

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
    else
    {
      LOGE << "ERROR WITH " << path2 << endl;
    }
  }
  else if (chr::hasFileResources())
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
