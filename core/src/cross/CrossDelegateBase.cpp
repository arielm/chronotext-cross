/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "cross/CrossDelegateBase.h"
#include "cross/Context.h"
#include "system/DisplayHelper.h"

using namespace std;

namespace chr
{
    void CrossDelegateBase::messageFromBridge(int what, const string &body)
    {
//        sketch->sendMessage(Message(what, body));
    }
    
    // ---
    
    namespace intern
    {
        shared_ptr<SystemManager> systemManager;
    }
    
    bool CrossDelegateBase::_init()
    {
        assert(!initialized_);
               
        intern::systemManager = make_shared<SystemManager>();
        intern::systemManager->setup(initInfo);

        DisplayHelper::setup(initInfo);
        
        // ---
        
        sketch = createSketch();
        sketchCreated(sketch);
        return sketch->init();
    }
    
    void CrossDelegateBase::_uninit()
    {
        assert(initialized_ && !setup_);
        
        sketch->uninit();
        delete sketch;
        sketchDestroyed(sketch);
        sketch = nullptr;
        
        // ---
        
        DisplayHelper::shutdown();

        intern::systemManager->shutdown();
        intern::systemManager.reset();
    }
    
    void CrossDelegateBase::_setup()
    {
        assert(!setup_ && initialized_);

        // ---
        
        LOGI << "WINDOW INFO: " << setupInfo.windowInfo << endl; // LOG: VERBOSE
        
        sketch->performSetup(setupInfo.windowInfo);
    }
    
    void CrossDelegateBase::_shutdown()
    {
        assert(setup_);
        
        sketch->shutdown();
    }
    
    // ---
    
    SystemManager& systemManager()
    {
        return checkedReference(intern::systemManager.get());
    }
}
