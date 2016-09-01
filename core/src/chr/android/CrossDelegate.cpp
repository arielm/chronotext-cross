#include "chr/android/CrossDelegate.h"
#include "chr/cross/Context.h"

using namespace std;

namespace chr
{
  namespace intern
  {
    CrossDelegate *instance = nullptr;
  }
  
  CrossDelegate& delegate()
  {
    return checkedReference(intern::instance);
  }
  
  // ---
  
  bool CrossDelegate::performInit(JNIEnv *env, jobject androidContext, jobject androidDisplay, const glm::vec2 &displaySize, float displayDensity)
  {
    if (!initialized_)
    {
      initInfo.androidContext = env->NewGlobalRef(androidContext);
      initInfo.androidDisplay = env->NewGlobalRef(androidDisplay);
      initInfo.displaySize = displaySize;
      initInfo.displayDensity = displayDensity;

      intern::instance = this;
      initialized_ = _init();
    }
    
    return initialized_;
  }
  
  void CrossDelegate::performUninit(JNIEnv *env)
  {
    if (initialized_ && !setup_)
    {
      _uninit();
      
      env->DeleteGlobalRef(initInfo.androidContext);
      env->DeleteGlobalRef(initInfo.androidDisplay);
      
      // ---

      initialized_ = false;
      intern::instance = nullptr;
    }
  }
  
  void CrossDelegate::performSetup(JNIEnv *env, const glm::vec2 &size)
  {
    if (!setup_ && initialized_)
    {
      startIOService();
      createSensorEventQueue();
      
      setupInfo.windowInfo.size = size;
      
      _setup();
      
      // ---
      
      setup_ = true;
    }
  }
  
  void CrossDelegate::performShutdown(JNIEnv *env)
  {
    if (setup_)
    {
      _shutdown();
      
      destroySensorEventQueue();
      stopIOService();
      
      // ---
      
      setup_ = false;
    }
  }
  
  void CrossDelegate::performResize(const glm::vec2 &size)
  {
    assert(setup_);
    
    setupInfo.windowInfo.size = size;
    sketch->performResize(size);
  }
  
  void CrossDelegate::performUpdate()
  {
    assert(setup_);
    
    /*
     * SHOULD TAKE PLACE BEFORE IO-SERVICE-POLLING
     *
     * SUBSEQUENT CALLS TO FrameClock::getTime() DURING THE FRAME WILL RETURN THE SAME TIME-SAMPLE
     */
     sketch->clock()->update(true);
    
    pollSensorEvents(); // WHERE handleAcceleration IS INVOKED
    io->poll(); // WHERE addTouch, updateTouch, removeTouch, ETC. ARE INVOKED
    
    sketch->performUpdate();
    updateCount++;
  }
  
  void CrossDelegate::performDraw()
  {
    assert(setup_);
    sketch->draw();
  }
  
#pragma mark ---------------------------------------- SKETCH <-> BRIDGE COMMUNICATION ----------------------------------------
  
  /*
   * TODO: FORMAT body FOR LOG
   *
   * 1) LEADING AND TRAILING WHITE-SPACE TRIMMED
   * 2) LINE-BREAKS AND TABS REPLACED BY SPACES
   * 3) TEXT-LENGTH LIMITED
   */
  
  /*
   * WILL BE QUEUED TO THE RENDERER'S THREAD (VIA CPP-HANDLER)
   */
  void CrossDelegate::messageFromBridge(int what, const string &body)
  {
    LOGI << "MESSAGE RECEIVED FROM BRIDGE: " << what << " " << body << endl; // LOG: VERBOSE
    
    CrossDelegateBase::messageFromBridge(what, body);
  }
  
  /*
   * WILL BE QUEUED TO THE MAIN-THREAD THREAD (VIA JAVA-HANDLER)
   */
  void CrossDelegate::sendMessageToBridge(int what, const string &body)
  {
    LOGI << "MESSAGE SENT TO BRIDGE: " << what << " " << body << endl; // LOG: VERBOSE
    
    jni::callVoidMethodOnBridge("messageFromSketch", "(ILjava/lang/String;)V", what, jni::toJString(body));
  }

  void CrossDelegate::handleEvent(int eventId)
  {
    switch (eventId)
    {
      case CrossSketch::EVENT_RESUMED:
        sketch->performStart(CrossSketch::START_REASON_APP_RESUMED);
        return;
        
      case CrossSketch::EVENT_SHOWN:
        sketch->performStart(CrossSketch::START_REASON_VIEW_SHOWN);
        return;
        
      case CrossSketch::EVENT_PAUSED:
        sketch->performStop(CrossSketch::STOP_REASON_APP_PAUSED);
        return;
        
      case CrossSketch::EVENT_HIDDEN:
        sketch->performStop(CrossSketch::STOP_REASON_VIEW_HIDDEN);
        return;
    }
    
    sketch->event(eventId);
  }
  
  /*
   * TODO: FINALIZE THREAD-SAFETY POLICY
   */
  void CrossDelegate::performAction(int actionId)
  {
    jni::callBooleanMethodOnBridge("handleAction", "(I)Z", actionId);
  }
  
  // ---
  
  /*
   * CURRENT LIMITATION: MUST BE CALLED FROM THE MAIN-THREAD OR THE RENDERER'S THREAD
   *
   * TODO:
   *
   * 1) ADD SUPPORT FOR JAVA-THREAD-ATTACHMENT IN os/Task
   * 2) FINALIZE THREAD-SAFETY POLICY
   * 3) HANDLE POTENTIAL JNI EXCEPTIONS IN callObjectMethodOnBridge()
   */
  
  // JsonTree CrossDelegate::jsonQuery(const char *methodName)
  // {
  //   const string &query = jni::toString((jstring)jni::callObjectMethodOnBridge(methodName, "()Ljava/lang/String;"));
    
  //   if (!query.empty())
  //   {
  //     try
  //     {
  //       return JsonTree(query);
  //     }
  //     catch (exception &e)
  //     {
  //       LOGI_IF(LOG_WARNING)  << "JSON-QUERY FAILED | REASON: " << e.what() << endl;
  //     }
  //   }
    
  //   return JsonTree();
  // }
  
#pragma mark ---------------------------------------- IO-SERVICE ----------------------------------------
  
  void CrossDelegate::startIOService()
  {
    if (!io)
    {
      io = make_shared<boost::asio::io_service>();
      ioWork = make_shared<boost::asio::io_service::work>(*io);
    }
  }
  
  void CrossDelegate::stopIOService()
  {
    if (io)
    {
      io->stop();
      
      ioWork.reset();
      io.reset();
    }
  }
  
#pragma mark ---------------------------------------- TOUCH ----------------------------------------
  
  void CrossDelegate::addTouch(int index, float x, float y)
  {
    sketch->addTouch(index, x, y);
  }
  
  void CrossDelegate::updateTouch(int index, float x, float y)
  {
    sketch->updateTouch(index, x, y);
  }
  
  void CrossDelegate::removeTouch(int index, float x, float y)
  {
    sketch->removeTouch(index, x, y);
  }
  
#pragma mark ---------------------------------------- ACCELEROMETER ----------------------------------------
  
  /*
   * REFERENCES:
   *
   * http://android-developers.blogspot.co.il/2010/09/one-screen-turn-deserves-another.html
   * http://developer.download.nvidia.com/tegra/docs/tegra_android_accelerometer_v5f.pdf
   *
   * PROBABLY OVERKILL...
   */
  static void canonicalToWorld(int displayRotation, float *canVec, glm::vec3 &worldVec)
  {
    struct AxisSwap
    {
      int negateX;
      int negateY;
      int xSrc;
      int ySrc;
    };
    
    static const AxisSwap axisSwap[] =
    {
      { 1,  1, 0, 1 }, // ROTATION_0
      {-1,  1, 1, 0 }, // ROTATION_90
      {-1, -1, 0, 1 }, // ROTATION_180
      { 1, -1, 1, 0 }  // ROTATION_270
    };
    
    const AxisSwap &as = axisSwap[displayRotation];
    
    worldVec.x = as.negateX * canVec[as.xSrc];
    worldVec.y = as.negateY * canVec[as.ySrc];
    worldVec.z = canVec[2];
  }
  
  void CrossDelegate::enableAccelerometer(float updateFrequency, float filterFactor)
  {
    accelFilter = AccelEvent::Filter(filterFactor);
    
    int delay = 1000000 / updateFrequency;
    int min = ASensor_getMinDelay(accelerometerSensor);
    
    if (delay < min)
    {
      delay = min;
    }
    
    ASensorEventQueue_enableSensor(sensorEventQueue, accelerometerSensor);
    ASensorEventQueue_setEventRate(sensorEventQueue, accelerometerSensor, delay);
  }
  
  void CrossDelegate::disableAccelerometer()
  {
    ASensorEventQueue_disableSensor(sensorEventQueue, accelerometerSensor);
  }
  
  void CrossDelegate::handleAcceleration(ASensorEvent event)
  {
    int displayRotation = getDisplayRotation();
    
    glm::vec3 transformed;
    canonicalToWorld(displayRotation, (float*)&event.acceleration.v, transformed);
    
    /*
     * ADDITIONAL TRANSFORMATION: FOR CONSISTENCY WITH iOS
     */
    transformed *= glm::vec3(-1, -1, +1) / ASENSOR_STANDARD_GRAVITY; // TODO: DOUBLE-CHECK Z AXIS
    
    sketch->accelerated(accelFilter.process(transformed));
  }
  
  /*
   * TODO: CONSIDER HANDLING IN DisplayHelper
   */
  int CrossDelegate::getDisplayRotation()
  {
    JNIEnv *env = jni::getEnv();
    
    jmethodID getRotationMethod = env->GetMethodID(env->GetObjectClass(initInfo.androidDisplay), "getRotation", "()I");
    return env->CallIntMethod(initInfo.androidDisplay, getRotationMethod);
  }
  
#pragma mark ---------------------------------------- SENSOR EVENTS ----------------------------------------
  
  void CrossDelegate::createSensorEventQueue()
  {
    auto looper = ALooper_forThread();
    
    if (!looper)
    {
      looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    }
    
    sensorManager = ASensorManager_getInstance();
    accelerometerSensor = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_ACCELEROMETER);
    sensorEventQueue = ASensorManager_createEventQueue(sensorManager, looper, 3, nullptr, nullptr);
  }
  
  void CrossDelegate::destroySensorEventQueue()
  {
    ASensorManager_destroyEventQueue(sensorManager, sensorEventQueue);
  }
  
  void CrossDelegate::pollSensorEvents()
  {
    ASensorEvent event;
    
    while (ASensorEventQueue_getEvents(sensorEventQueue, &event, 1) > 0)
    {
      if (event.type == ASENSOR_TYPE_ACCELEROMETER)
      {
        handleAcceleration(event);
      }
    }
  }
}
