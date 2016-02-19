package org.chronotext.TestingLibPng;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;

import org.chronotext.cross.CrossBridge;

public class MainActivity extends Activity
{
  static
  {
    System.loadLibrary("TestingLibPng");
  }

  CrossBridge bridge;

  @Override
  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);

    bridge = new CrossBridge(this);
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
