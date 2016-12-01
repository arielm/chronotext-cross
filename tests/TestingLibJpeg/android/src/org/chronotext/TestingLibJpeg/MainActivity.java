package org.chronotext.TestingLibJpeg;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;

import org.chronotext.cross.CrossBridge;
import org.chronotext.gl.GLView;

public class MainActivity extends Activity
{
  static
  {
    System.loadLibrary("TestingLibJpeg");
  }

  CrossBridge bridge;

  @Override
  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);

    GLView view = new GLView(this);
    view.setProperties(new GLView.Properties().setEGLConfigChooser(8, 8, 8, 0, 0, 0));

    bridge = new CrossBridge(this);
    bridge.bind(view);

    setContentView(view);
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
