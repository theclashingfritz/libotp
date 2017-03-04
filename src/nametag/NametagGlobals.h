#pragma once

#include "util.h"
//#include <Python.h> //Un-comment if needed
#include <luse.h>
#include <nodepath.h>
#include <audioSound.h>
#include <mouseWatcher.h>
#include <typedObject.h>
#include <pandabase.h>

#define EMPTY_NODEPATH =NodePath()

class NametagGlobals;

#define CLICKSTATE_NORMAL    0
#define CLICKSTATE_CLICK     1
#define CLICKSTATE_HOVER     2
#define CLICKSTATE_DISABLED  3

typedef pvector<LVecBase4f> color_tuple_t; // (LVecBase4f, LVecBase4f)
typedef pvector<color_tuple_t> color_tuple_tuple_t; // ((LVecBase4f, LVecBase4f), (LVecBase4f, LVecBase4f))
typedef pmap<int, color_tuple_tuple_t> state_map_t; // {state: color_tuple_tuple_t}
typedef pmap<int, color_tuple_t> state_map_simple_t; // {state: color_tuple_t}

enum ColorCode {
    CCNormal,
    CCNoChat,
    CCNonPlayer,
    CCSuit,
    CCToonBuilding,
    CCSuitBuilding,
    CCHouseBuilding,
    CCSpeedChat,
    CCFreeChat
};

enum WhisperType {
    WTNormal,
    WTQuickTalker,
    WTSystem,
    WTBattleSOS,
    WTEmote,
    WTToontownBoardingGroup
};

typedef pmap<ColorCode, state_map_t> color_map_t; // {cc: state_map_t}
typedef pmap<WhisperType, state_map_simple_t> whisper_color_map_t; // {wt: state_map_simple_t}


class EXPCL_LIBOTP NametagGlobals : public TypedObject {
    PUBLISHED:
        NametagGlobals();
        ~NametagGlobals();
        
        static uint8_t CFSpeech;
        static uint8_t CFThought;
        static uint8_t CFQuicktalker;
        static uint8_t CFTimeout;
        static uint8_t CFPageButton;
        static uint8_t CFQuitButton;
        static uint8_t CFReversed;
        static uint8_t CFSndOpenchat;
        static uint16_t CFNoQuitButton;
        
        void set_toon(NodePath toon);
        void set_arrow_model(NodePath node);
        void set_mouse_watcher(NodePath& np);
        void set_camera(NodePath node);
        void set_want_active_nametags(bool want);
        void set_force_onscreen_chat(bool want);
        void set_force_2d_nametags(bool want);
        void set_click_sound(PT(AudioSound) sound);
        void set_rollover_sound(PT(AudioSound) sound);
        PT(AudioSound) get_click_sound();
        PT(AudioSound) get_rollover_sound();
        LVecBase4f get_name_fg(ColorCode cc, int clickstate);
        color_tuple_t get_whisper_colors(WhisperType wt, int clickstate);
        NodePath get_toon();
        NodePath get_arrow_model();
        NodePath get_camera();
        bool get_want_active_nametags();
        bool get_force_onscreen_chat();
        bool get_force_2d_nametags();
    
    public:
        static PT(MouseWatcher) m_mouse_watcher;
        static PT(AudioSound) m_click_sound;
        static PT(AudioSound) m_rollover_sound;
        
    protected:      
        static bool m_want_active_nametags;
        static bool m_force_onscreen_chat;
        static bool m_force_2d_nametags;
        
        static ColorCode CCNormal;
        static ColorCode CCNoChat;
        static ColorCode CCNonPlayer;
        static ColorCode CCSuit;
        static ColorCode CCToonBuilding;
        static ColorCode CCSuitBuilding;
        static ColorCode CCHouseBuilding;
        static ColorCode CCSpeedChat;
        static ColorCode CCFreeChat;
        
        static WhisperType WTNormal;
        static WhisperType WTQuickTalker;
        static WhisperType WTSystem;
        static WhisperType WTBattleSOS;
        static WhisperType WTEmote;
        static WhisperType WTToontownBoardingGroup;
        
        static color_map_t nametag_colors;
        static whisper_color_map_t whisper_colors;

    private:
        void add_color(ColorCode cc, LVecBase4f normal_name_fg, LVecBase4f normal_name_bg, LVecBase4f normal_chat_fg, LVecBase4f normal_chat_bg, LVecBase4f click_name_fg, LVecBase4f click_name_bg, LVecBase4f click_chat_fg, LVecBase4f click_chat_bg, LVecBase4f hover_name_fg, LVecBase4f hover_name_bg, LVecBase4f hover_chat_fg, LVecBase4f hover_chat_bg, LVecBase4f disabled_name_fg, LVecBase4f disabled_name_bg, LVecBase4f disabled_chat_fg, LVecBase4f disabled_chat_bg);
        void add_whisper_color(WhisperType wt, LVecBase4f normal_fg, LVecBase4f normal_bg, LVecBase4f click_fg, LVecBase4f click_bg, LVecBase4f hover_fg, LVecBase4f hover_bg, LVecBase4f disabled_fg, LVecBase4f disabled_bg);
        
        bool does_color_code_exist(ColorCode cc);
        bool does_whisper_type_exist(WhisperType wt);
        
        NodePath m_nodepath; 
        NodePath m_arrow_nodepath;
        NodePath m_page_button_nodepath;
        NodePath m_camera_nodepath;
        
    TYPE_HANDLE(NametagGlobals, TypedObject);
};