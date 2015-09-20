/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

package org.chronotext.utils;

import android.util.Log;

public class Utils
{
  public static String TAG = "CHR";
  public static boolean DEBUG;

  public static void LOGD(String message)
  {
    if (DEBUG)
    {
      Log.i(TAG, message);
    }
  }

  public static void LOGI(String message)
  {
    Log.i(TAG, message);
  }

  public static void LOGE(String message)
  {
    Log.e(TAG, message);
  }

  public static void LOGW(String message)
  {
    Log.w(TAG, message);
  }
}
