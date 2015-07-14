package org.chronotext;

import android.app.Activity;

public class Bridge
{
  public Bridge(Activity activity)
  {
    performInit(activity);
  }

  public static native void performInit(Activity activity);
  public static native int invokeMain(String[] args);
}
