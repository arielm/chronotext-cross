#include "Log.h"
#include "Platform.h"

#if defined(CHR_RUN_APK)
  #include <cstdlib>
  #include <android/log.h>
#endif

using namespace std;

namespace chr
{
  Log::Log(int level)
  :
  level(level)
  {}

  Log::~Log()
  {
#if defined(CHR_RUN_APK)
    int androidPriority;

    switch (level)
    {
      case FATAL:
        androidPriority = ANDROID_LOG_FATAL;
        break;

      case ERROR:
        androidPriority = ANDROID_LOG_ERROR;
        break;

      case WARNING:
        androidPriority = ANDROID_LOG_WARN;
        break;

      default:
      case INFO:
        androidPriority = ANDROID_LOG_INFO;
        break;

      case DEBUG:
        androidPriority = ANDROID_LOG_DEBUG;
        break;
    }

    __android_log_write(androidPriority, "CHR", ss.str().data());
#else
    if (level < 0)
    {
      cerr << ss.rdbuf();
    }
    else
    {
      cout << ss.rdbuf() << std::flush;
    }
#endif

    if (level == FATAL)
    {
      abort();
    }
  }

  stringstream& Log::stream()
  {
    return ss;
  }
}
