package org.chronotext;

import android.os.Bundle;
import android.test.InstrumentationTestRunner;
import android.test.InstrumentationTestSuite;
import android.util.Log;

import junit.framework.TestSuite;

public class TestRunner extends InstrumentationTestRunner
{
  public String[] args;

  @Override
  public void onCreate(Bundle bundle)
  {
    super.onCreate(bundle);

    String bundleArgs = bundle.getString("args");

    if (bundleArgs != null)
    {
      args = bundleArgs.split("\\s+");
    }
    else
    {
      args = new String[]{};
    }
  }
}
