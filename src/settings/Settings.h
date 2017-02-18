#pragma once

#include "util.h"

#include <string>
#include <iostream>

#include <filename.h>
#include <compress_string.h>
#include <virtualFileSystem.h>
#include <datagram.h>
#include <datagramIterator.h>
#include <typedObject.h>
#include <pandabase.h>

class Settings;

class EXPCL_LIBOTP Settings : public TypedObject {
    
    PUBLISHED:
        Settings();
        ~Settings();
        void read_settings();
        void write_settings();
        void set_music(bool mode);
        void set_sfx(bool mode);
        void set_force_sw_midi(bool mode);
        void set_embedded_mode(bool mode);
        void set_chat_log(bool mode);
        void set_sfx_volume(float volume);
        void set_music_volume(float volume);
        void set_display_driver(unsigned int driver);
        void set_windowed_mode(unsigned int mode);
        void set_resolution(unsigned int resolution);
        void set_resolution_dimensions(unsigned int xsize, unsigned int ysize);
        int get_resolution();
        int get_windowed_mode();
        float get_sfx_volume();
        float get_music_volume();
        bool get_music();
        bool get_sfx();
        bool get_embedded_mode(); 
        bool do_saved_settings_exist();
        
    protected:
        unsigned int m_resolution_dimensions[2];
        unsigned int m_resolution;
        unsigned int m_windowed_mode;
        unsigned int m_current_driver;
        bool m_want_music;
        bool m_want_sfx;
        bool m_force_sw_midi;
        bool m_save_settings_exist;
        bool m_log_chat;
        bool m_embedded_mode;
        float m_music_volume;
        float m_sfx_volume;
        
    private:
        std::string m_data;
        Filename m_file;
        VirtualFileSystem* m_vfs;
        
    TYPE_HANDLE(Settings, TypedObject);
};