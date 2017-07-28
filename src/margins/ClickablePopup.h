#pragma once

#include <cstdint>
#include <cassert>
#include <exception>
#include <stdexcept>

#include "util.h"
#include "nametagGlobals.h"

#include <audioSound.h>
#include <pandabase.h>
#include <pandaNode.h>
#include <nodePath.h>
#include <typeinfo>
#include <mouseWatcher.h>
#include <mouseWatcherBase.h>
#include <mouseWatcherRegion.h>
#include <PGButton.h>
#include <notifyCategoryProxy.h>

NotifyCategoryDecl(ClickablePopup, EXPCL_LIBOTP, EXPTP_LIBOTP);

class ClickablePopup;

class EXPCL_LIBOTP ClickablePopup : public virtual TypedObject {
    
    public:
        ClickablePopup();
        virtual ~ClickablePopup();
        
        enum State {
            stateStopped = 0,
            statePaused = 1,
            stateRunning = 2
        };
        
        virtual void destroy();
        virtual void update_contents();
        
        void click();
        
        void set_state(State state);
        uint32_t get_state();
        
        void press(MouseWatcherParameter param);
        void release(MouseWatcherParameter param);
        
        void enter_region(MouseWatcherParameter param);
        void exit_region(MouseWatcherParameter param);

        
    protected:
        PT(AudioSound) m_click_sound, m_rollover_sound;
        
        State m_click_state;
    
    private:      
        static unsigned int ClickablePopup_serial;
        
    TYPE_HANDLE(ClickablePopup, TypedObject);
};