/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

package org.chronotext.cross;

import android.os.Message;

public interface BridgeListener
{
  /*
   * INVOKED ON THE MAIN-THREAD
   */
  public void handleMessage(int what, String body);

  /*
   * DEPENDING ON THE EVENT: INVOKED EITHER ON THE MAIN-THREAD OR THE RENDERER'S THREAD
   */
  public void handleEvent(int eventId);
}
