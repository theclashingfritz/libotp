#pragma once

#include "util.h"
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
        void set_embedded_mode(bool mode);
        float get_sfx_volume();
        float get_music_volume();
        bool get_embedded_mode(); 
        
    protected:
        bool m_save_settings_exist;
        bool m_embedded_mode;
        float m_music_volume;
        float m_sfx_volume;
        
    TYPE_HANDLE(Settings, TypedObject);
};