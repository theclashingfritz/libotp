#pragma once

#include "util.h"
#include <string>
#include <typedObject.h>
#include <pandabase.h>

class Settings;

class EXPCL_LIBOTP Settings : public TypedObject {
    
    PUBLISHED:
        Settings();
        ~Settings();
        void read_settings();
        void write_settings();
        void set_sfx_volume(float volume);
        void set_music_volume(float volume);
        void set_force_sw_midi(bool mode);
        void set_embedded_mode(bool mode);
        void set_chat_log(bool mode);
        void set_display_driver(int driver);
        void set_resolution(int resolution);
        void set_resolution_dimensions(unsigned int xsize, unsigned int ysize);
        int get_resolution();
        int get_windowed_mode();
        float get_sfx_volume();
        float get_music_volume();
        bool get_embedded_mode(); 
        bool do_saved_settings_exist();
        
    protected:
        unsigned int m_resolution_dimensions[2];
        int m_resolution;
        int m_current_driver;
        bool m_force_sw_midi;
        bool m_save_settings_exist;
        bool m_log_chat;
        bool m_embedded_mode;
        float m_music_volume;
        float m_sfx_volume;
        
    TYPE_HANDLE(Settings, TypedObject);
};