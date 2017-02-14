#pragma once

#include "util.h"
//#include <Python.h> //Un-comment if needed
#include <luse.h>
#include <nodepath.h>
#include <audioSound.h>
#include <mouseWatcher.h>
#include <typedObject.h>
#include <pandabase.h>

class NametagGlobals;

class EXPCL_LIBOTP NametagGlobals : public TypedObject {
    PUBLISHED:
        NametagGlobals();
        ~NametagGlobals();
        void set_toon(NodePath toon);
        void set_arrow_model(NodePath node);
        void set_camera(NodePath node);
        void set_want_active_nametags(bool want);
        void set_force_onscreen_chat(bool want);
        void set_force_2d_nametags(bool want);
        void set_click_sound(PT(AudioSound) sound);
        void set_rollover_sound(PT(AudioSound) sound);
        NodePath get_toon();
        NodePath get_arrow_model();
        NodePath get_camera();
        bool get_want_active_nametags();
        bool get_force_onscreen_chat();
        bool get_force_2d_nametags();
        
    protected:   
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
        bool m_force_2d_nametags;

        PT(AudioSound) m_click_sound;
        PT(AudioSound) m_rollover_sound;
        
    private:
        NodePath m_nodepath; 
        NodePath m_arrow_nodepath;
        NodePath m_page_button_nodepath;
        NodePath m_camera_nodepath;
        
    TYPE_HANDLE(NametagGlobals, TypedObject);
};