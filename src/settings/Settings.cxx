#include "Settings.h"


NotifyCategoryDef(Settings, "");

TypeHandle Settings::_type_handle;

Settings* Settings::_global_ptr = nullptr;

Settings::Settings() {
    /**
     * Constructs the Settings class.
     */
    m_vfs = VirtualFileSystem::get_global_ptr();
    m_file = Filename("/useropt"); //First let's use this to set our dir.
    m_file = Filename(m_file.to_os_long_name()); //Now let's do the real file.
    
    //Define our Settings Version!
    m_version = "v1.1.0";
    
    //Now define any PUBLISHED variables
    GL = 1;
    DX9 = 2;
    TINYDISPLAY = 3;
    
    PRODUCTION = 1;
    DEBUG = 2;
    DEVELOPMENT = 3;
    
    //Now to define our default settings.
    m_want_music = 1;
    m_want_sfx = 1;
    m_force_sw_midi = 0;
    m_embedded_mode = 0;
    m_log_chat = 0;
    m_show_fpsmeter = 0;
    m_custom_mouse_cursor = 1;
    m_toon_chat_sounds = 1;
    m_accepting_new_friends = 1;
    m_accepting_non_friend_whispers = 1;
    m_sfx_volume = encrypt_float(100.0f);
    m_music_volume = encrypt_float(100.0f);
    m_current_driver = encrypt_int(0);
    m_resolution = encrypt_int(1);
    m_windowed_mode = encrypt_int(0);
    m_server_type = encrypt_int(1);
    m_resolution_dimensions[0] = encrypt_int(800);
    m_resolution_dimensions[1] = encrypt_int(600);
}

Settings::~Settings() {
    /**
     * Deconstructs the Settings class.
     */
}

void Settings::read_settings() {
    /**
     * Reads the Settings from the Settings file if it exist. If not the 
     * default one is created and the default settings are written to it.
     */
    Filename found(m_file);
    
    if (!m_vfs->exists(found)) {
        Settings_cat.debug() << "Failed to find Settings! Creating file...." << std::endl;
        write_settings();
        return;
    }
    m_vfs->read_file(found, m_data, true);
    if (m_data.size() <= 12) {
        Settings_cat.debug() << "Settings detected to be too small." << std::endl;
        write_settings();
        return;
    };
    std::string header = m_data.substr(0, 13);
    if (header != "UserSettings") {
        Settings_cat.debug() << "Invalid Header: " << header << std::endl;
        write_settings();
        return;
    }
    m_data = m_data.substr(13);
    std::reverse(m_data.begin(), m_data.end());
    m_data = decompress_string(m_data);
    
    char * e_data = new char[m_data.length()];
    memcpy(e_data, m_data.c_str(), m_data.length());
    
    char *mFixedKey = unscramble_key(aes_key_index[0], aes_key_index[1], aes_key_index[7]);
    
    e_data = AES_decrypt(e_data, mFixedKey, aes_key_index[7]);
    
    delete mFixedKey;
    
    mFixedKey = nullptr;
    
    if (e_data != NULL && e_data != nullptr) {
        m_data = *new string(e_data);
    } else {
        Settings_cat.warning() << "Failed to decrypt Settings!" << std::endl;
        delete[] e_data; 
        write_settings();
        return;
    }
    delete[] e_data; 
    
    e_data = nullptr;
    
    Datagram dg(m_data);
    DatagramIterator dgi(dg);
    m_data = "";
    if (dgi.get_string() != m_version) {
        Settings_cat.warning() << "Settings version does not match current version! Resetting settings...." << std::endl;
        write_settings();
        return;
    }
    m_want_music = dgi.get_bool();
    m_want_sfx = dgi.get_bool();
    m_force_sw_midi = dgi.get_bool();
    m_embedded_mode = dgi.get_bool();
    m_log_chat = dgi.get_bool();
    m_show_fpsmeter = dgi.get_bool();
    m_custom_mouse_cursor = dgi.get_bool();
    m_toon_chat_sounds = dgi.get_bool();
    m_accepting_new_friends = dgi.get_bool();
    m_accepting_non_friend_whispers = dgi.get_bool();
    m_sfx_volume = encrypt_float(dgi.get_stdfloat());
    m_music_volume = encrypt_float(dgi.get_stdfloat());
    m_server_type = encrypt_int(dgi.get_uint8());
    m_current_driver = encrypt_int(dgi.get_uint8());
    m_resolution = encrypt_int(dgi.get_uint8());
    m_windowed_mode = encrypt_int(dgi.get_uint8());
    m_resolution_dimensions[0] = encrypt_int(dgi.get_uint16());
    m_resolution_dimensions[1] = encrypt_int(dgi.get_uint16());
}

void Settings::write_settings() {
    /**
     * Writes the currently set settings to the Settings file to loaded up next time the Settings
     * are needed via read_settings()
     */
    Datagram dg;
    dg.add_string(m_version);
    dg.add_bool(m_want_music);
    dg.add_bool(m_want_sfx);
    dg.add_bool(m_force_sw_midi);
    dg.add_bool(m_embedded_mode);
    dg.add_bool(m_log_chat);
    dg.add_bool(m_show_fpsmeter);
    dg.add_bool(m_custom_mouse_cursor);
    dg.add_bool(m_toon_chat_sounds);
    dg.add_bool(m_accepting_new_friends);
    dg.add_bool(m_accepting_non_friend_whispers);
    dg.add_stdfloat(decrypt_float(m_sfx_volume));
    dg.add_stdfloat(decrypt_float(m_music_volume));
    dg.add_uint8(decrypt_int(m_server_type));
    dg.add_uint8(decrypt_int(m_current_driver));
    dg.add_uint8(decrypt_int(m_resolution));
    dg.add_uint8(decrypt_int(m_windowed_mode));
    dg.add_uint16(decrypt_int(m_resolution_dimensions[0]));
    dg.add_uint16(decrypt_int(m_resolution_dimensions[1]));
    DatagramIterator dgi(dg);
    
    m_data = dgi.get_remaining_bytes();
    
    char * e_data = new char[m_data.length()];
    memcpy(e_data, m_data.c_str(), m_data.length());
    
    char *mFixedKey = unscramble_key(aes_key_index[0], aes_key_index[1], aes_key_index[7]);
    
    e_data = AES_encrypt(e_data, mFixedKey, aes_key_index[7]);
    
    delete mFixedKey;
    
    mFixedKey = nullptr;
    
    if (e_data != NULL && e_data != nullptr) {
        m_data = *new string(e_data);
    } else {
        Settings_cat.warning() << "Failed to encrypt Settings!" << std::endl;
        delete[] e_data; 
        return;
    }
    delete[] e_data;
    
    e_data = nullptr;
    
    m_data = compress_string(m_data, 9);
    std::reverse(m_data.begin(), m_data.end());
    m_data = "UserSettings" + m_data;
    if (m_vfs->exists(m_file)) {
        m_vfs->delete_file(m_file);
    }
    m_vfs->write_file(m_file, m_data, 0);
    m_data = "";
}

void Settings::set_music(bool mode) {
    /**
     * Want Music?
     */
    m_want_music = mode;
}

void Settings::set_sfx(bool mode) {
    /**
     * Want SOUND?
     */
    m_want_sfx = mode;
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

void Settings::set_show_fpsmeter(bool mode) {
    m_show_fpsmeter = mode;
}

void Settings::set_custom_mouse_cursor(bool mode) {
    m_custom_mouse_cursor = mode;
}

void Settings::set_toon_chat_sounds(bool mode) {
    m_toon_chat_sounds = mode;
}

void Settings::set_accepting_new_friends(bool mode) {
    m_accepting_new_friends = mode;
}

void Settings::set_accepting_non_friend_whispers(bool mode) {
    m_accepting_non_friend_whispers = mode;
}

void Settings::set_sfx_volume(float volume) {
    m_sfx_volume = encrypt_float(volume);
}

void Settings::set_music_volume(float volume) {
    m_music_volume = encrypt_float(volume);
}

void Settings::set_server_type(uint8_t type) {
    m_server_type = encrypt_int(type);
}

void Settings::set_display_driver(uint8_t driver) {
    /**
     * Sets the display driver by using it corrosponding ID.
     */
    m_current_driver = encrypt_int(driver);
}

void Settings::set_windowed_mode(uint8_t mode) {
    m_windowed_mode = encrypt_int(mode);
}

void Settings::set_resolution(uint8_t resolution) {
    /**
     * Sets the Resolution Mode.
     */
    m_resolution = encrypt_int(resolution);
}

void Settings::set_resolution_dimensions(uint16_t xsize, uint16_t ysize) {
    /**
     * Sets the Resolution Dimensions.
     */
    m_resolution_dimensions[0] = encrypt_int(xsize);
    m_resolution_dimensions[1] = encrypt_int(ysize);
}

uint8_t Settings::server_type() {
    return decrypt_int(m_server_type);
}

uint8_t Settings::get_resolution() {
    return decrypt_int(m_resolution);
}

uint8_t Settings::get_windowed_mode() {
    return decrypt_int(m_windowed_mode);
}

bool Settings::get_music() {
    return m_want_music;
}

bool Settings::get_sfx() {
    return m_want_sfx;
}

bool Settings::want_chat_log() {
    return m_log_chat;
}

bool Settings::get_show_fpsmeter() {
    return m_show_fpsmeter;
}

bool Settings::want_custom_mouse_cursor() {
    return m_custom_mouse_cursor;
}

bool Settings::get_toon_chat_sounds() {
    return m_toon_chat_sounds;
}

bool Settings::get_accepting_new_friends() {
    return m_accepting_new_friends;
}

bool Settings::get_accepting_non_friend_whispers() {
    return m_accepting_non_friend_whispers;
}

float Settings::get_sfx_volume() {
    return decrypt_float(m_sfx_volume);
}

float Settings::get_music_volume() {
    return decrypt_float(m_music_volume);
}

bool Settings::get_embedded_mode() {
    return m_embedded_mode;
}

bool Settings::do_saved_settings_exist() {
    /**
     * If saved Settings exist returns True otherwise False.
     */
    return m_vfs->exists(m_file);
}

Settings* Settings::get_global_ptr() {
    if ((_global_ptr == nullptr) || (_global_ptr == NULL)) {
        _global_ptr = new Settings;
    }
    return _global_ptr;
}