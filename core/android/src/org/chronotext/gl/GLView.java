/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

package org.chronotext.gl;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;

import java.util.Vector;

import org.chronotext.cross.CrossBridge;
import org.chronotext.cross.CrossRenderer;
import org.chronotext.cross.Touch;
import org.chronotext.utils.Utils;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.View;

public class GLView extends GLSurfaceView
{
  public static class Properties
  {
    int eglContextClientVersion = 2;
    int[] eglConfigChooser;
    int pixelFormat = PixelFormat.UNKNOWN;
    boolean preserveEGLContextOnPause = true;

    public Properties setEGLContextClientVersion(int version)
    {
      eglContextClientVersion = version;
      return this;
    }

    public Properties setEGLConfigChooser(int redSize, int greenSize, int blueSize, int alphaSize, int depthSize, int stencilSize)
    {
      eglConfigChooser = new int[] {redSize, greenSize, blueSize, alphaSize, depthSize, stencilSize};
      return this;
    }

    public Properties setPixelFormat(int format)
    {
      pixelFormat = format;
      return this;
    }

    public Properties setPreserveEGLContextOnPause(boolean preserveOnPause)
    {
      preserveEGLContextOnPause = preserveOnPause;
      return this;
    }
  }

  // ---
  
  protected CrossBridge crossBridge;
  protected CrossRenderer crossRenderer;
  Properties properties;

  protected boolean attached;
  protected boolean paused;
  protected boolean hidden;
  protected boolean exiting;
  protected boolean shutdown;
  protected boolean uninitialized;

  public GLView(Context context)
  {
    super(context);
  }

  public GLView(Context context, AttributeSet attrs)
  {
    super(context, attrs);
  }

  public void setProperties(Properties properties)
  {
    this.properties = properties;
  }

  protected void applyProperties()
  {
    if (properties == null)
    {
      properties = new GLView.Properties();
    }

    setEGLContextClientVersion(properties.eglContextClientVersion);
    setEGLContextFactory(new CustomContextFactory(properties.eglContextClientVersion));

    if (properties.eglConfigChooser != null)
    {
      setEGLConfigChooser(
              properties.eglConfigChooser[0],
              properties.eglConfigChooser[1],
              properties.eglConfigChooser[2],
              properties.eglConfigChooser[3],
              properties.eglConfigChooser[4],
              properties.eglConfigChooser[5]);
    }

    if (properties.pixelFormat != PixelFormat.UNKNOWN)
    {
      getHolder().setFormat(properties.pixelFormat);
    }

    setPreserveEGLContextOnPause(properties.preserveEGLContextOnPause);
  }

  public void bind(CrossBridge bridge)
  {
    applyProperties();

    crossBridge = bridge;
    crossRenderer = new CrossRenderer(crossBridge);
    setRenderer(crossRenderer); // WILL START THE RENDERER'S THREAD
  }

  public boolean isValid()
  {
    return !(shutdown || exiting || uninitialized);
  }

  @Override
  public void surfaceCreated(SurfaceHolder holder)
  {
    Utils.LOGD("GLView.surfaceCreated"); 
    super.surfaceCreated(holder);
  }

  @Override
  public void surfaceDestroyed(SurfaceHolder holder)
  {
    Utils.LOGD("GLView.surfaceDestroyed"); 
    super.surfaceDestroyed(holder);
  }

  @Override
  public void surfaceChanged(SurfaceHolder holder, int format, int width, int height)
  {
    Utils.LOGD("GLView.surfaceChanged: " + width + "x" + height);
    super.surfaceChanged(holder, format, width, height);
  }

  @Override
  public void onWindowFocusChanged(boolean hasFocus)
  {
    Utils.LOGD("GLView.onWindowFocusChanged: " + hasFocus);
    super.onWindowFocusChanged(hasFocus);
  }

  @Override
  protected void onAttachedToWindow()
  {
    Utils.LOGD("GLView.onAttachedToWindow");

    if (!isValid())
    {
      Utils.LOGE("GLView IS INVALID");
      return;
    }
    
    if (!attached)
    {
      attached = true;

      if (!hidden)
      {
        crossBridge.dispatchEvent(CrossBridge.VIEW_WILL_APPEAR);
      }

      super.onAttachedToWindow();

      queueEvent(new Runnable()
      {
        public void run()
        {
          crossRenderer.attachedToWindow();
        }
      });
    }
  }

  @Override
  protected void onDetachedFromWindow()
  {
    Utils.LOGD("GLView.onDetachedFromWindow");

    if (!isValid())
    {
      return;
    }

    if (attached)
    {
      attached = false;

      if (!hidden)
      {
        crossBridge.dispatchEvent(CrossBridge.VIEW_WILL_DISAPPEAR);
      }

      /*
       * SEEMS THE ONLY WAY TO COMMUNICATE WITH THE RENDERER'S THREAD BEFORE IT EXITS
       * SEE ALSO: GLView.CustomContextFactory.destroyContext()
       *
       * CONDITION: GL-CONTEXT MUST HAVE BEEN CREATED
       *
       * PROBLEM: THIS WON'T HAPPEN IF THE GL-VIEW WAS NOT VISIBLE WHILE ATTACHED
       * SOLUTION: NO OBVIOUS GLSurfaceView HACK FOR NOW!
       *
       * CONSEQUENCES: "RENDERER'S THREAD CREATION" AND "GL CONTEXT CREATION" MUST SHARE THE SAME "SETUP" EVENT
       */
      exiting = true;

      /*
       * WILL WAIT UNTIL THE RENDERER'S THREAD HAS EXITED
       */
      super.onDetachedFromWindow();

      crossBridge.performUninit();
      uninitialized = true;
    }
  }

  @Override
  public void onVisibilityChanged(View changedView, final int visibility)
  {
    Utils.LOGD("GLView.visibilityChanged: " + visibility);

    if (changedView == this)
    {
      switch (visibility)
      {
        case View.VISIBLE:
        {
          if (!isValid())
          {
            Utils.LOGE("GLView IS INVALID");
            return;
          }

          hidden = false;
          break;
        }

        case View.GONE:
        case View.INVISIBLE: // WARNING: THIS ONE USED TO TRIGGER SOFTWARE-RENDERING ON OLDER SYSTEMS (E.G. XOOM 1, HONEYCOMB)
        {
          if (!isValid())
          {
            return;
          }

          hidden = true;
          break;
        }
      }

      queueEvent(new Runnable()
      {
        public void run()
        {
          crossRenderer.visibilityChanged(visibility);
        }
      });

      if (attached)
      {
        if (hidden)
        {
          crossBridge.dispatchEvent(CrossBridge.VIEW_WILL_DISAPPEAR);
        }
        else
        {
          crossBridge.dispatchEvent(CrossBridge.VIEW_WILL_APPEAR);
        }
      }
    }
  }

  @Override
  public boolean onTouchEvent(MotionEvent event)
  {
    if (!isValid())
    {
      return false;
    }

    switch (event.getAction() & MotionEvent.ACTION_MASK)
    {
      case MotionEvent.ACTION_DOWN:
      {
        int index = 0;
        final Vector<Touch> touches = new Vector<Touch>();
        touches.add(new Touch(event.getPointerId(index), event.getX(index), event.getY(index)));

        queueEvent(new Runnable()
        {
          public void run()
          {
            crossRenderer.addTouches(touches);
          }
        });

        break;
      }

      case MotionEvent.ACTION_POINTER_DOWN:
      {
        int index = event.getActionIndex();
        final Vector<Touch> touches = new Vector<Touch>();
        touches.add(new Touch(event.getPointerId(index), event.getX(index), event.getY(index)));

        queueEvent(new Runnable()
        {
          public void run()
          {
            crossRenderer.addTouches(touches);
          }
        });

        break;
      }

      case MotionEvent.ACTION_UP:
      {
        int index = 0;
        final Vector<Touch> touches = new Vector<Touch>();
        touches.add(new Touch(event.getPointerId(index), event.getX(index), event.getY(index)));

        queueEvent(new Runnable()
        {
          public void run()
          {
            crossRenderer.removeTouches(touches);
          }
        });

        break;
      }

      case MotionEvent.ACTION_POINTER_UP:
      {
        int index = event.getActionIndex();
        final Vector<Touch> touches = new Vector<Touch>();
        touches.add(new Touch(event.getPointerId(index), event.getX(index), event.getY(index)));

        queueEvent(new Runnable()
        {
          public void run()
          {
            crossRenderer.removeTouches(touches);
          }
        });

        break;
      }

      case MotionEvent.ACTION_MOVE:
      {
        final Vector<Touch> touches = new Vector<Touch>();
          
        for (int i = 0; i < event.getPointerCount(); i++)
        {
          touches.add(new Touch(event.getPointerId(i), event.getX(i), event.getY(i)));
        }

        queueEvent(new Runnable()
        {
          public void run()
          {
            crossRenderer.updateTouches(touches);
          }
        });

        break;
      }
    }

    return true;
  }

  // ---------------------------------------- INVOKED ON THE MAIN-THREAD BY CrossBridge ----------------------------------------

  public boolean resume()
  {
    if (isValid())
    {
      if (attached && paused) // SIMPLE PROTECTION AGAINST SPURIOUS onResume() CALLS
      {
        paused = false;
        onResume();

        queueEvent(new Runnable()
        {
          public void run()
          {
            crossRenderer.resumed();
          }
        });

        return true;
      }
    }

    return false;
  }

  public boolean pause()
  {
    if (isValid())
    {
      if (attached && !paused) // SIMPLE PROTECTION AGAINST SPURIOUS onPause() CALLS
      {
        paused = true;
        onPause();

        queueEvent(new Runnable()
        {
          public void run()
          {
            crossRenderer.paused();
          }
        });
      }

      return true;
    }

    return false;
  }

  public boolean backPressed()
  {
    if (isValid())
    {
      if (attached && !hidden)
      {
        queueEvent(new Runnable()
        {
          public void run()
          {
            crossRenderer.dispatchEvent(CrossRenderer.EVENT_TRIGGER_BACK); 
          }
        });

        return true;
      }
    }

    return false;
  }

  // ---------------------------------------- CustomContextFactory ----------------------------------------

  /*
   * BASED ON GLSurfaceView.DefaultContextFactory
   *
   * NECESSARY BECAUSE:
   *
   * 1) setPreserveEGLContextOnPause(true) IS NOT TRUSTABLE BY DESIGN, SO WE NEED TO BE NOTIFIED WHEN CONTEXT-DESTRUCTION ACTUALLY OCCURS
   *
   * 2) EGLContextFactory.destroyContext() IS THE ONLY "HOOK" FOR COMMUNICATING ON THE RENDERER'S THREAD WHEN:
   *    - THE VIEW HAS BEEN DETACHED (ASSERTION: THE RENDERER'S THREAD WILL EXIT UPON VIEW-DETACHMENT, WHICH IN TURN WILL TRIGGER CONTEXT-DESTRUCTION)
   *    - THE ACTIVITY IS BEING DESTROYED (ASSERTION: ACTIVITY DESTRUCTION WILL TRIGGER VIEW-DETACHMENT)
   */

  protected class CustomContextFactory implements EGLContextFactory
  {
    protected int mEGLContextClientVersion;

    public CustomContextFactory(int eglContextClientVersion)
    {
      mEGLContextClientVersion = eglContextClientVersion;
    }

    public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig config)
    {
      Utils.LOGD("CustomContextFactory.createContext");
      crossRenderer.contextCreated();

      int[] attrib_list = { 0x3098, mEGLContextClientVersion, EGL10.EGL_NONE };
      return egl.eglCreateContext(display, config, EGL10.EGL_NO_CONTEXT, mEGLContextClientVersion != 0 ? attrib_list : null);
    }

    public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context)
    {
      Utils.LOGD("CustomContextFactory.destroyContext");
      crossRenderer.contextDestroyed();

      if (exiting)
      {
        exiting = false;
        shutdown = true;

        /*
         * TODO: THESE 2 COULD BE MERGED INTO A MORE "SPECIFIC" METHOD
         */
        crossRenderer.detachedFromWindow();
        crossRenderer.performShutdown();
      }

      egl.eglDestroyContext(display, context);
    }
  }
}
