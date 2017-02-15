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

void Settings::set_force_sw_midi(bool mode) {
    m_force_sw_midi = mode;
}

void Settings::set_embedded_mode(bool mode) {
    m_embedded_mode = mode;
}

void Settings::set_chat_log(bool mode) {
    m_log_chat = mode;
}

void Settings::set_display_driver(int driver) {
    m_current_driver = driver;
}

void Settings::set_resolution(int resolution) {
    m_resolution = resolution;
}

void Settings::set_resolution_dimensions(unsigned int xsize, unsigned int ysize) {
    m_resolution_dimensions[0] = xsize;
    m_resolution_dimensions[1] = ysize;
}

int Settings::get_resolution() {
    return m_resolution;
}

int Settings::get_windowed_mode() {
    return 1;
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

bool Settings::do_saved_settings_exist() {
    return 0;
}

