package org.chronotext.TestingWebSocket;

import android.app.Activity;
import android.os.Bundle;

public class MainActivity extends Activity
{
  static
  {
    System.loadLibrary("TestingWebSocket");
  }

  @Override
  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);

	  performInit(this);
	  invokeMain();
  }

  public static native void performInit(Activity activity);
  public static native int invokeMain();
}
