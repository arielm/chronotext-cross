/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "CrossDelegateBase.h"

using namespace std;

namespace chr
{
    atomic<bool> CrossDelegateBase::LOG_VERBOSE (false);
    atomic<bool> CrossDelegateBase::LOG_WARNING (true);

    void CrossDelegateBase::messageFromBridge(int what, const string &body)
    {
//        sketch->sendMessage(Message(what, body));
    }
    
    // ---
    
    namespace intern
    {
//        shared_ptr<SystemManager> systemManager;
//        shared_ptr<MemoryManager> memoryManager;
//        shared_ptr<TaskManager> taskManager;
        
        /*
         * TODO: MOVE TO TaskManager?
         */
//        boost::asio::io_service *io_service = nullptr;
//        thread::id threadId;
//        bool threadReady = false;
    }
    
    bool CrossDelegateBase::_init()
    {
        assert(!initialized_);
               
//        intern::systemManager = make_shared<SystemManager>();
//        intern::systemManager->setup(initInfo);
        
//        intern::memoryManager = make_shared<MemoryManager>();
//        intern::memoryManager->setup();
        
//        FileHelper::setup(initInfo);
//        DisplayHelper::setup(initInfo);
        
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
        
//        DisplayHelper::shutdown();
//        FileHelper::shutdown();
        
//        intern::memoryManager->shutdown();
//        intern::memoryManager.reset();
        
//        intern::systemManager->shutdown();
//        intern::systemManager.reset();
    }
    
    void CrossDelegateBase::_setup()
    {
        assert(!setup_ && initialized_);
        
        /*
         * TODO: MOVE TO TaskManager?
         */
//        intern::io_service = setupInfo.io_service;
//        intern::threadId = this_thread::get_id();
//        intern::threadReady = true;
        
//        intern::taskManager = TaskManager::create();
        
        // ---
        
        LOGI << "WINDOW INFO: " << setupInfo.windowInfo << endl; // LOG: VERBOSE
        
        sketch->performSetup(setupInfo.windowInfo);
    }
    
    void CrossDelegateBase::_shutdown()
    {
        assert(setup_);
        
        sketch->shutdown();
        
        /*
         * TODO:
         *
         * - HANDLE PROPERLY THE SHUTING-DOWN OF "UNDERGOING" TASKS
         * - SEE RELATED TODOS IN CinderDelegate AND TaskManager
         */
//        intern::taskManager.reset();
//        intern::threadReady = false;
//        intern::io_service = nullptr;
    }
    
    // ---
    
//    SystemManager& systemManager()
//    {
//        return checkedReference(intern::systemManager.get());
//    }
//
//    MemoryManager& memoryManager()
//    {
//        return checkedReference(intern::memoryManager.get());
//    }
//
//    TaskManager& taskManager()
//    {
//        return checkedReference(intern::taskManager.get());
//    }
    
    // ---
    
    /*
     * TODO: MOVE TO TaskManager?
     */
    
//    namespace os
//    {
//        bool isThreadSafe()
//        {
//            if (intern::threadReady)
//            {
//                return intern::threadId == this_thread::get_id();
//            }
//
//            return false;
//        }
//
//        bool post(function<void()> &&fn, bool forceSync)
//        {
//            if (forceSync)
//            {
//                if (isThreadSafe())
//                {
//                    fn();
//                    return true;
//                }
//            }
//            else if (intern::threadReady)
//            {
//                intern::io_service->post(forward<function<void()>>(fn));
//                return true;
//            }
//
//            return false;
//        }
//    }
}
