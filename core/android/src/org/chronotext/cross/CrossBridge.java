/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

package org.chronotext.cross;

import android.app.Activity;
import android.app.ActivityManager;
import android.app.ActivityManager.MemoryInfo;
import android.content.Context;
import android.graphics.Point;
import android.os.Handler;
import android.os.Message;
import android.view.Display;

import org.chronotext.gl.GLView;
import org.chronotext.utils.DisplayUtils;
import org.chronotext.utils.Utils;
import org.json.JSONException;
import org.json.JSONObject;

public class CrossBridge extends Handler implements BridgeListener
{
  /*
   * PARALLEL TO chronotext/cross/CrossSketch.h
   */
  public static final int ACTION_CAPTURE_BACK = 1;
  public static final int ACTION_RELEASE_BACK = 2;

  /*
   * INVOKED ON THE MAIN-THREAD
   */
  public static final int SKETCH_WILL_INIT = 1;
  public static final int SKETCH_DID_INIT = 2;
  public static final int SKETCH_WILL_UNINIT = 3;
  public static final int SKETCH_DID_UNINIT = 4;
  public static final int APP_DID_RESUME = 5;
  public static final int APP_WILL_PAUSE = 6;
  public static final int VIEW_WILL_APPEAR = 7;
  public static final int VIEW_WILL_DISAPPEAR = 8;

  /*
   * INVOKED ON THE RENDERER'S THREAD
   */
  public static final int SKETCH_WILL_SETUP = 9;
  public static final int SKETCH_DID_SETUP = 10;
  public static final int SKETCH_WILL_SHUTDOWN = 11;
  public static final int SKETCH_DID_SHUTDOWN = 12;

  // ---

  protected Activity activity;
  protected BridgeListener listener;

  protected GLView view;

  protected boolean initialized;
  protected boolean backCaptured;

  public CrossBridge(Activity activity, BridgeListener listener)
  {
    this.activity = activity;
    this.listener = listener;

    performInit(); // WILL CREATE THE C++ CrossDelegate
  }

  public CrossBridge(Activity activity)
  {
    this(activity, null);
  }


  public void bind(GLView view)
  {
    this.view = view;
    view.bind(this);
  }

  public Activity getActivity()
  {
    return activity;
  }

  // ---------------------------------------- LIFE-CYCLE ----------------------------------------

  protected void performInit()
  {
    if (!initialized)
    {
      Display display = DisplayUtils.getDisplay(activity);
      Point displaySize = DisplayUtils.getRealSize(display);
      float displayDensity = DisplayUtils.getRealDensity(display);

      Utils.LOGD("CrossBridge.performInit: " + displaySize.x + "x" + displaySize.y + " (" + displayDensity + " dpi)");

      dispatchEvent(SKETCH_WILL_INIT);
      init(this, activity, display, displaySize.x, displaySize.y, displayDensity);
      dispatchEvent(SKETCH_DID_INIT);

      initialized = true;
    }
  }

  public void performUninit()
  {
    if (initialized)
    {
      Utils.LOGD("CrossBridge.performUninit");

      dispatchEvent(SKETCH_WILL_UNINIT);
      uninit();
      dispatchEvent(SKETCH_DID_UNINIT);

      initialized = false;
    }
  }

  // ---------------------------------------- BRIDGE CALLBACKS ----------------------------------------

  @Override
  public void handleMessage(Message msg)
  {
    if (listener != null)
    {
      listener.handleMessage(msg.what, (String) msg.obj);
    }
    else
    {
      handleMessage(msg.what, (String) msg.obj);
    }
  }

  public void dispatchEvent(int eventId)
  {
    if (listener != null)
    {
      listener.handleEvent(eventId);
    }
    else
    {
      handleEvent(eventId);
    }
  }

  // ---

  @Override
  public void handleMessage(int what, String body)
  {}

  @Override
  public void handleEvent(int eventId)
  {}

  // ---------------------------------------- TO BE FORWARDED FROM THE HOST ACTIVITY (DO NOT OVERRIDE) ----------------------------------------

  public void onPause()
  {
    if (view != null)
    {
      if (view.pause())
      {
        Utils.LOGD("CrossBridge.onPause");
        dispatchEvent(APP_WILL_PAUSE);
      }
    }
  }

  public void onResume()
  {
    if (view != null)
    {
      if (view.resume())
      {
        Utils.LOGD("CrossBridge.onResume");
        dispatchEvent(APP_DID_RESUME);
      }
    }
  }
  
  public boolean onBackPressed()
  {
    if (view != null)
    {
      Utils.LOGD("CrossBridge.onBackPressed");

      if (backCaptured)
      {
        return view.backPressed();
      }
    }

    return false;
  }

  // ---------------------------------------- SKETCH <-> BRIDGE COMMUNICATION ----------------------------------------

  /*
   * WILL BE QUEUED TO THE MAIN-THREAD (VIA JAVA-HANDLER)
   *
   * TODO: DO NOT NECESSARILY QUEUE EVERY MESSSAGE TO THE MAIN-THREAD?
   * PRE-REQUISITE: RECEIVING ADDITIONAL INFO FROM SKETCH REGARDING THE "CURRENT" AND "DESTINATION" THREADS, ETC.
   */
  public void messageFromSketch(int what, String body)
  {
    sendMessage(Message.obtain(this, what, body));
  }

  public void sendMessageToSketch(final int what, final String body)
  {
    view.queueEvent(new Runnable()
    {
      public void run()
      {
        view.crossRenderer.sendMessageToSketch(what, body);
      }
    });
  }

  public void sendMessageToSketch(int what)
  {
    sendMessageToSketch(what, (String) null);
  }

  /*
   * TODO: FINALIZE THREAD-SAFETY POLICY
   */
  public boolean handleAction(int actionId)
  {
    switch (actionId)
    {
      case ACTION_CAPTURE_BACK:
        backCaptured = true;
        return true;

      case ACTION_RELEASE_BACK:
        backCaptured = false;
        return true;
    }

    return false;
  }

  // ---------------------------------------- QUERIES (TO BE CALLED FROM ANY THREAD ATTACHED TO JAVA) ----------------------------------------

  public synchronized String getMemoryInfo()
  {
    MemoryInfo memoryInfo = new MemoryInfo();
    ((ActivityManager) activity.getSystemService(Context.ACTIVITY_SERVICE)).getMemoryInfo(memoryInfo);

    JSONObject json = new JSONObject();

    try 
    {
      json.put("availMem", memoryInfo.availMem);
      json.put("threshold", memoryInfo.threshold);
      json.put("lowMemory", memoryInfo.lowMemory);
    } 
    catch (JSONException e) 
    {}

    return json.toString();
  }

  // ---------------------------------------- JNI ----------------------------------------

  protected native void init(Object bridge, Context context, Display display, int displayWidth, int displayHeight, float displayDensity);
  protected native void uninit();
}
