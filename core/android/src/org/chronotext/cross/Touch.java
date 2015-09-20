/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

package org.chronotext.cross;

import java.util.Vector;

public class Touch
{
  public int index;
  public float x;
  public float y;
  
  public Touch(int index, float x, float y)
  {
    this.index = index;
    this.x = x;
    this.y = y;
  }
}
