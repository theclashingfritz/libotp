#pragma once

#include "util.h"
#include "luse.h"
#include "Python.h"
#include "nodepath.h"
#include <typedObject.h>
#include <pandabase.h>

class NametagGlobals;

class EXPCL_LIBOTP NametagGlobals : public TypedObject
{
    PUBLISHED:
        NametagGlobals();
        ~NametagGlobals();
        void set_me(NodePath me);
        void set_want_active_nametags(bool want);
        void set_force_onscreen_chat(bool want);
        
        
    private:   
        uint8_t CFSpeech;
        uint8_t CFThought;
        uint8_t CFQuicktalker;
        uint8_t CFTimeout;
        uint8_t CFPageButton;
        uint8_t CFQuitButton;
        uint8_t CFReversed;
        uint8_t CFSndOpenchat;
        uint16_t CFNoQuitButton;
        
        bool m_want_active_nametags;
        bool m_force_onscreen_chat;
        
        NodePath m_nodepath; 
        
    TYPE_HANDLE(NametagGlobals, TypedObject);
};