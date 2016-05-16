package org.chronotext.TestingDrawing3d;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;

import org.chronotext.cross.CrossBridge;
import org.chronotext.gl.GLView;

public class MainActivity extends Activity
{
  static
  {
    System.loadLibrary("TestingDrawing3d");
  }

  CrossBridge bridge;

  @Override
  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);

    bridge = new CrossBridge(this);
    bridge.setViewProperties(new GLView.Properties().setEGLConfigChooser(8, 8, 8, 0, 24, 0));
    setContentView(bridge.getView());
  }

  @Override
  protected void onPause()
  {
    super.onPause();
    bridge.onPause();
  }

  @Override
  protected void onResume()
  {
    super.onResume();
    bridge.onResume();
  }

  @Override
  public void onBackPressed()
  {
    if (!bridge.onBackPressed())
    {
      super.onBackPressed();
    }
  }
}
