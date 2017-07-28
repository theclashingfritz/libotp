#pragma once

#include "util.h"
#include "ChatBalloon.h"
//#include <Python.h> //Un-comment if needed
#include <luse.h>
#include <nodepath.h>
#include <audioSound.h>
#include <filename.h>
#include <mouseWatcher.h>
#include <typedObject.h>
#include <notifyCategoryProxy.h>
#include <pandabase.h>

NotifyCategoryDecl(NametagGlobals, EXPCL_LIBOTP, EXPTP_LIBOTP);

class NametagGlobals;

class EXPCL_LIBOTP NametagGlobals : public virtual TypedObject {
    PUBLISHED:
        NametagGlobals();
        ~NametagGlobals();
        
        enum {
            CCNormal = 0,
            CCNoChat = 1,
            CCNonPlayer = 2,
            CCSuit = 3,
            CCToonBuilding = 4,
            CCSuitBuilding = 5,
            CCHouseBuilding = 6,
            CCSpeedChat = 7,
            CCFreeChat = 8
        };

        enum {
            WTNormal = 0,
            WTQuickTalker = 1,
            WTSystem = 2,
            WTBattleSOS = 3,
            WTEmote = 4,
            WTToontownBoardingGroup = 5
        };
        
        enum {
            CFSpeech = 1,
            CFThought = 2,
            CFQuicktalker = 4,
            CFTimeout = 8,
            CFPageButton = 16,
            CFQuitButton = 32,
            CFReversed = 64,
            CFSndOpenchat = 128,
            CFNoQuitButton = 256
        };
        
        enum { 
            CName = 1,
            CSpeech = 2,
            CThought = 4
        };

        static color_map_t nametag_colors;
        static whisper_color_map_t whisper_colors;
       
        static void set_toon(NodePath& toon);
        static void set_arrow_model(NodePath& node);
        static void set_card_model(NodePath& node);
        static void set_nametag_card(NodePath& model, VBase4 frame);
        static void set_mouse_watcher(MouseWatcher& watcher);
        static void set_mouse_watcher(NodePath& np);
        static void set_camera(NodePath& node);
        
        static void set_want_active_nametags(bool want);
        static void set_force_onscreen_chat(bool want);
        static void set_force_2d_nametags(bool want);
        
        static void set_max_2d_alpha(float alpha);
        static void set_min_2d_alpha(float alpha);
        
        static void set_click_sound(PT(AudioSound) sound);
        static void set_rollover_sound(PT(AudioSound) sound);
        static void set_page_button(int state, NodePath& model);
        static void set_page_button(NodePath& model, NodePath& model1, NodePath& model2, NodePath& model3);
        static void set_quit_button(int state, NodePath& model);
        static void set_quit_button(NodePath& model, NodePath& model1, NodePath& model2, NodePath& model3);
        static void set_speech_balloon_3d(ChatBalloon* sb3d);
        static void set_thought_balloon_3d(ChatBalloon* tb3d);
        static void set_speech_balloon_2d(ChatBalloon* sb2d);
        static void set_thought_balloon_2d(ChatBalloon* tb2d);
        static NametagGlobals* get_global_ptr();
        static PT(AudioSound) get_click_sound();
        static PT(AudioSound) get_rollover_sound();
        static VBase4 get_nametag_card_frame();
        static LVecBase4f get_name_fg(unsigned int cc, int clickstate);
        static color_tuple_t get_whisper_colors(unsigned int wt, int clickstate);
        static NodePath get_toon();
        static NodePath get_arrow_model();
        static NodePath get_card_model();
        static NodePath get_nametag_card();
        static NodePath get_chat_balloon_3d_model();
        static NodePath get_chat_balloon_2d_model();
        static NodePath get_thought_balloon_model();
        static NodePath get_camera();
        static bool get_want_active_nametags();
        static bool get_force_onscreen_chat();
        static bool get_force_2d_nametags();
    
    public:   
        static void add_color(unsigned int cc, LVecBase4f normal_name_fg, LVecBase4f normal_name_bg, LVecBase4f normal_chat_fg, LVecBase4f normal_chat_bg, LVecBase4f click_name_fg, LVecBase4f click_name_bg, LVecBase4f click_chat_fg, LVecBase4f click_chat_bg, LVecBase4f hover_name_fg, LVecBase4f hover_name_bg, LVecBase4f hover_chat_fg, LVecBase4f hover_chat_bg, LVecBase4f disabled_name_fg, LVecBase4f disabled_name_bg, LVecBase4f disabled_chat_fg, LVecBase4f disabled_chat_bg);
        static void add_whisper_color(unsigned int wt, LVecBase4f normal_fg, LVecBase4f normal_bg, LVecBase4f click_fg, LVecBase4f click_bg, LVecBase4f hover_fg, LVecBase4f hover_bg, LVecBase4f disabled_fg, LVecBase4f disabled_bg);
        
        static void set_master_arrows_on(bool want);
        static void set_master_nametags_active(bool want);
        static void set_master_nametags_visible(bool want);
        
        static void set_global_nametag_scale(float scale);
        
        static bool does_color_code_exist(unsigned int cc);
        static bool does_whisper_type_exist(unsigned int wt);
        
        static MouseWatcher *m_mouse_watcher;
        static PT(AudioSound) m_click_sound;
        static PT(AudioSound) m_rollover_sound;
        
        static buttons_map_t page_buttons;
        static buttons_map_t quit_buttons;
        
        static VBase4 m_nametag_frame;
        
        static ChatBalloon *speech_balloon_2d;
        static ChatBalloon *speech_balloon_3d;
        static ChatBalloon *thought_balloon_2d;
        static ChatBalloon *thought_balloon_3d;
        
        static NodePath *m_nodepath;
        static NodePath *m_nametag_model_nodepath;
        static NodePath *m_arrow_nodepath;
        static NodePath *m_card_nodepath;
        static NodePath *m_card_balloon_3d_nodepath;
        static NodePath *m_card_balloon_2d_nodepath;
        static NodePath *m_thought_balloon_nodepath;
        static NodePath *m_page_button_nodepath;
        static NodePath *m_camera_nodepath;
        
        static UpdateSeq margin_prop_seq;
        
        static bool m_want_active_nametags;
        static bool m_force_onscreen_chat;
        static bool m_force_2d_nametags;
        
        static bool _master_arrows_on;
        static bool _master_nametags_active;
        static bool _master_nametags_visible;
        
        static const double whisper_total_time;
        static const double whisper_priority_time;
        static const double cell_memory_time;
        static const double button_delay_time;
        
        static float _global_nametag_scale;

        static const float billboard_offset;
        static const float far_distance;
        static const float far_scale;
        static const float scale_exponent;
        static const float arrow_scale;
        static const float arrow_offset;
        static const float nominal_avatar_width;
        static const float name_wordwrap;
        static const float house_name_wordwrap;
        static const float building_name_wordwrap;
        static const float chat_2d_wordwrap;
        static const float chat_3d_wordwrap;
        static const float balloon_internal_width;
        static const float balloon_external_width;
        static const float ballon_min_hscale;
        static const float grid_count_horizontal;
        static const float grid_count_vertical;
        static const float grid_spacing_horizontal;
        static const float grid_spacing_vertical;
        
    private:
        static NametagGlobals* _global_ptr;
        
        static float _max_2d_alpha;
        static float _min_2d_alpha;
        
        static AtomicAdjust::Integer n_seq;
        static AtomicAdjust::Integer *seq;
      
    TYPE_HANDLE(NametagGlobals, TypedObject);
};

class UpdateSeq_steal {
  public:
    enum SpecialCases {
      SC_initial = 0,
      SC_old = 1,
      SC_fresh = ~(unsigned int)0,
    };
    AtomicAdjust::Integer _seq;
};