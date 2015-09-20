/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

package org.chronotext.utils;

import android.content.Context;
import android.graphics.Point;
import android.os.Build;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.Surface;
import android.view.WindowManager;

public class DisplayUtils
{
  public static Display getDisplay(Context context)
  {
    return ((WindowManager) context.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
  }

  public static DisplayMetrics getMetrics(Display display)
  {
    DisplayMetrics metrics = new DisplayMetrics();
    display.getMetrics(metrics);
    return metrics;
  }

  public static Point getRealSize(Display display)
  {
    DisplayMetrics metrics = getMetrics(display);

    /*
     * PROBLEM: THE FOLLOWING IS AFFECTED BY "SYSTEM DECORATIONS" (E.G. NAVIGATION BAR)
     */
    int widthPixels = metrics.widthPixels;
    int heightPixels = metrics.heightPixels;

    /*
     * SOLUTIONS...
     *
     * REFERENCE: http://stackoverflow.com/a/15699681/50335
     */

    if (Build.VERSION.SDK_INT >= 14 && Build.VERSION.SDK_INT < 17)
    {
      try
      {
        widthPixels = (Integer) Display.class.getMethod("getRawWidth").invoke(display);
        heightPixels = (Integer) Display.class.getMethod("getRawHeight").invoke(display);
      }
      catch (Exception ignored)
      {}
    }

    if (Build.VERSION.SDK_INT >= 17)
    {
      try
      {
        Point realSize = new Point();
        Display.class.getMethod("getRealSize", Point.class).invoke(display, realSize);

        widthPixels = realSize.x;
        heightPixels = realSize.y;
      }
      catch (Exception ignored)
      {}
    }

    return new Point(widthPixels, heightPixels);
  }

  public static Point getRealNaturalSize(Display display)
  {
    Point realSize = getRealSize(display);
    int rotation = display.getRotation();

    if ((rotation == Surface.ROTATION_0) || (rotation == Surface.ROTATION_180))
    {
      return realSize;
    }
    else
    {
      return new Point(realSize.y, realSize.x);
    }
  }

  /*
   * MORE ACCURATE THAN DisplayMetrics.densityDpi WHICH IS (BY DESIGN)
   * APPROXIMATED TO "BUCKETS" OF DENSITY (120, 160, 240, 320, ETC.)
   */
  public static float getRealDensity(Display display)
  {
    DisplayMetrics metrics = getMetrics(display);

    Point realNaturalSize = getRealNaturalSize(display); // SEEKING THE "NATURAL SIZE" FOR (HOPEFULLY) APPLYING xdpi AND ydpi TO THE RIGHT AXES
    float widthInches = realNaturalSize.x / metrics.xdpi;
    float heightInches = realNaturalSize.y / metrics.ydpi;

    double diagonal = Math.sqrt(widthInches * widthInches + heightInches * heightInches);
    double density = Math.sqrt(realNaturalSize.x * realNaturalSize.x + realNaturalSize.y * realNaturalSize.y) / diagonal;

    return (float) density;
  }
}
