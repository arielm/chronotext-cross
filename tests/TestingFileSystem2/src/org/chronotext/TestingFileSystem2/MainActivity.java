package org.chronotext.TestingFileSystem2;

import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

public class MainActivity extends Activity
{
  static
  {
    System.loadLibrary("TestingFileSystem2");
  }

  @Override
  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    Log.d("MainActivity", "onCreate");

    View view = new View(this);
    view.setBackgroundColor(Color.YELLOW);
    setContentView(view);
  }

  @Override
  public void onDestroy()
  {
    super.onDestroy();
    Log.d("MainActivity", "onDestroy");
  }
}
