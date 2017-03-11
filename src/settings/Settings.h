#pragma once

#include "util.h"

#include <string>
#include <iostream>

#include <notifyCategoryProxy.h>
#include <filename.h>
#include <compress_string.h>
#include <virtualFileSystem.h>
#include <datagram.h>
#include <datagramIterator.h>
#include <typedReferenceCount.h>
#include <pandabase.h>

NotifyCategoryDecl(Settings, EXPCL_LIBOTP, EXPTP_LIBOTP);

class Settings;

class EXPCL_LIBOTP Settings : public TypedReferenceCount {
    
    PUBLISHED:
        Settings();
        ~Settings();
        
        //Before we set any functions let's define PUBLISHED variables.
        unsigned int GL;
        unsigned int DX9;
        unsigned int TINYDISPLAY;
        
        unsigned int PRODUCTION;
        unsigned int DEBUG;
        unsigned int DEVELOPMENT;
        
        //Now let's do our functions!
        void read_settings();
        void write_settings();
        void set_music(bool mode);
        void set_sfx(bool mode);
        void set_force_sw_midi(bool mode);
        void set_embedded_mode(bool mode);
        void set_chat_log(bool mode);
        void set_show_fpsmeter(bool mode);
        void set_custom_mouse_cursor(bool mode);
        void set_toon_chat_sounds(bool mode);
        void set_accepting_new_friends(bool mode);
        void set_accepting_non_friend_whispers(bool mode);
        void set_sfx_volume(float volume);
        void set_music_volume(float volume);
        void set_server_type(int type);
        void set_display_driver(unsigned int driver);
        void set_windowed_mode(unsigned int mode);
        void set_resolution(unsigned int resolution);
        void set_resolution_dimensions(unsigned int xsize, unsigned int ysize);
        static Settings* get_global_ptr();
        int server_type();
        int get_resolution();
        int get_windowed_mode();
        float get_sfx_volume();
        float get_music_volume();
        bool get_music();
        bool get_sfx();
        bool want_chat_log();
        bool get_show_fpsmeter();
        bool want_custom_mouse_cursor();
        bool get_toon_chat_sounds();
        bool get_accepting_new_friends();
        bool get_accepting_non_friend_whispers();
        bool get_embedded_mode(); 
        bool do_saved_settings_exist();
        
    protected:
        unsigned int m_resolution_dimensions[2];
        unsigned int m_resolution;
        unsigned int m_windowed_mode;
        unsigned int m_server_type;
        unsigned int m_current_driver;
        bool m_want_music;
        bool m_want_sfx;
        bool m_force_sw_midi;
        bool m_save_settings_exist;
        bool m_log_chat;
        bool m_show_fpsmeter;
        bool m_custom_mouse_cursor;
        bool m_toon_chat_sounds;
        bool m_accepting_new_friends;
        bool m_accepting_non_friend_whispers;
        bool m_embedded_mode;
        float m_music_volume;
        float m_sfx_volume;
        
    private:
        std::string m_version;
        std::string m_data;
        Filename m_file;
        VirtualFileSystem* m_vfs;
        
        static Settings* _global_ptr;
        
    TYPE_HANDLE(Settings, TypedObject);
};