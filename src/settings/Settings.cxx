#include "Settings.h"

TypeHandle Settings::_type_handle;

Settings::Settings() {

}

Settings::~Settings() {
    
}

void Settings::read_settings() {
    return;
}

void Settings::write_settings() {
    return;
}

void Settings::set_sfx_volume(float volume) {
    m_sfx_volume = volume;
}

void Settings::set_music_volume(float volume) {
    m_music_volume = volume;
}

void Settings::set_embedded_mode(bool mode) {
    m_embedded_mode = mode;
}

float Settings::get_sfx_volume() {
    return m_sfx_volume;
}

float Settings::get_music_volume() {
    return m_music_volume;
}

bool Settings::get_embedded_mode() {
    return m_embedded_mode;
}


