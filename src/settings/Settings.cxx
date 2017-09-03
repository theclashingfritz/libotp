#include "Settings.h"


NotifyCategoryDef(Settings, "");

TypeHandle Settings::_type_handle;

Settings* Settings::_global_ptr = nullptr;

bool Settings::sanity_check = true;

Settings::Settings() {
    /**
     * Constructs the Settings class.
     */
#ifdef HAVE_THEMDIA
    CODEREPLACE_START 
#endif
    Settings_cat.spam() << "Crash Test Area 1" << std::endl;
    m_vfs = VirtualFileSystem::get_global_ptr();
    m_file = Filename("/useropt"); //First let's use this to set our dir.
    Settings_cat.spam() << "Crash Test Area 2" << std::endl;
    m_file = Filename(m_file.to_os_long_name()); //Now let's do the real file.
    
    //Define our Settings Version!
    m_version = "v1.1.0";
    Settings_cat.spam() << "Crash Test Area 3" << std::endl;
    //Now define any PUBLISHED variables
    GL = 1;
    DX9 = 2;
    TINYDISPLAY = 3;
    
    PRODUCTION = 1;
    DEBUG = 2;
    DEVELOPMENT = 3;
#ifdef HAVE_THEMDIA
    CODEREPLACE_END
#endif
    
#ifdef HAVE_THEMDIA
    MUTATE_START
#endif
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
#ifdef HAVE_THEMDIA
    MUTATE_END
#endif
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
     
#ifdef HAVE_THEMDIA
    MUTATE_START
#endif
    Settings_cat.debug() << "read_settings()" << std::endl;
    
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
#ifdef HAVE_THEMDIA
    MUTATE_END
#endif

#ifdef HAVE_THEMDIA
    if (sanity_check != true) {
        Settings_cat.error() << "Sanity Check failed!" << std::endl;
        return;
    }
#endif
    
#ifdef HAVE_THEMDIA
    VM_START
#endif
    m_data = m_data.substr(13);
    std::reverse(m_data.begin(), m_data.end());
    m_data = decompress_string(m_data);
    
    char * e_data = new char[m_data.length()];
    memcpy(e_data, m_data.c_str(), m_data.length());
    
    AESKeyScrambler *key_scrambler = AESKeyScrambler::get_global_ptr();
    
    char *key3 = key_scrambler->get_stored_key(7);
    if (!key_scrambler->check_key(key3)) {
        Settings_cat.warning() << "Settings encryption is no longer valid! Resetting..." << std::endl;
        delete[] e_data;
        free(key3);
        write_settings();
        return;
    }
    free(key3);
    key3 = key_scrambler->get_stored_key(7);
    
    char *key1 = key_scrambler->get_stored_key(0);
    char *key2 = key_scrambler->get_stored_key(1);
    char *mFixedKey = key_scrambler->get_unscrambled_key(key1, key2);
    free(key1);
    free(key2);
    key1 = nullptr;
    key2 = nullptr;
    
    e_data = AES_decrypt(e_data, mFixedKey, key3);
    free(key3);
    key3 = nullptr;
    
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
    
#ifdef HAVE_THEMDIA
    VM_END
#endif
    
#ifdef HAVE_THEMDIA
    MUTATE_START
#endif
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
#ifdef HAVE_THEMDIA
    MUTATE_END
#endif
}

void Settings::write_settings() {
    /**
     * Writes the currently set settings to the Settings file to loaded up next time the Settings
     * are needed via read_settings()
     */
    Settings_cat.debug() << "write_settings()" << std::endl;
    
#ifdef HAVE_THEMDIA
    MUTATE_START
#endif
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
#ifdef HAVE_THEMDIA
    MUTATE_END
#endif

#ifdef HAVE_THEMDIA
    if (sanity_check != true) {
        Settings_cat.error() << "Sanity Check failed!" << std::endl;
        return;
    }
#endif
    
#ifdef HAVE_THEMDIA
    VM_START
#endif
    char * e_data = new char[m_data.length()];
    memcpy(e_data, m_data.c_str(), m_data.length());
    
    AESKeyScrambler *key_scrambler = AESKeyScrambler::get_global_ptr();
    
    char *key3 = key_scrambler->get_stored_key(7);
    if (!key_scrambler->check_key(key3)) {
        Settings_cat.warning() << "Settings encryption has changed before write!" << std::endl;
    }
    free(key3);
    key3 = key_scrambler->get_stored_key(7);
    
    char *key1 = key_scrambler->get_stored_key(0);
    char *key2 = key_scrambler->get_stored_key(1);
    char *mFixedKey = key_scrambler->get_unscrambled_key(key1, key2);
    free(key1);
    free(key2);
    key1 = nullptr;
    key2 = nullptr;
    
    e_data = AES_encrypt(e_data, mFixedKey, key3);
    free(key3);
    key3 = nullptr;
    
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
    
#ifdef HAVE_THEMDIA
    VM_END
#endif
    
#ifdef HAVE_THEMDIA
    MUTATE_START
#endif
    m_data = compress_string(m_data, 9);
    std::reverse(m_data.begin(), m_data.end());
    m_data = "UserSettings" + m_data;
    if (m_vfs->exists(m_file)) {
        m_vfs->delete_file(m_file);
    }
    m_vfs->write_file(m_file, m_data, 0);
    m_data = "";
#ifdef HAVE_THEMDIA
    MUTATE_END
#endif
}

void Settings::set_music(bool mode) {
    /**
     * Want Music?
     */
#ifdef HAVE_THEMDIA
    MUTATE_START
#endif
    Settings_cat.debug() << "set_music(" << mode << ")" << std::endl;
    m_want_music = mode;
#ifdef HAVE_THEMDIA
    MUTATE_END
#endif
}

void Settings::set_sfx(bool mode) {
    /**
     * Want SOUND?
     */
#ifdef HAVE_THEMDIA
    MUTATE_START
#endif
    Settings_cat.debug() << "set_sfx(" << mode << ")" << std::endl;
    m_want_sfx = mode;
#ifdef HAVE_THEMDIA
    MUTATE_END
#endif
}

void Settings::set_force_sw_midi(bool mode) {
    Settings_cat.debug() << "set_force_sw_midi(" << mode << ")" << std::endl;
    m_force_sw_midi = mode;
}

void Settings::set_embedded_mode(bool mode) {
    Settings_cat.debug() << "set_embedded_mode(" << mode << ")" << std::endl;
    m_embedded_mode = mode;
}

void Settings::set_chat_log(bool mode) {
    Settings_cat.debug() << "set_chat_log(" << mode << ")" << std::endl;
    m_log_chat = mode;
}

void Settings::set_show_fpsmeter(bool mode) {
    Settings_cat.debug() << "set_fpsmeter(" << mode << ")" << std::endl;
    m_show_fpsmeter = mode;
}

void Settings::set_custom_mouse_cursor(bool mode) {
    Settings_cat.debug() << "set_custom_mouse_cursor(" << mode << ")" << std::endl;
    m_custom_mouse_cursor = mode;
}

void Settings::set_toon_chat_sounds(bool mode) {
    Settings_cat.debug() << "set_toon_chat_sounds(" << mode << ")" << std::endl;
    m_toon_chat_sounds = mode;
}

void Settings::set_accepting_new_friends(bool mode) {
    Settings_cat.debug() << "set_accepting_new_friends(" << mode << ")" << std::endl;
    m_accepting_new_friends = mode;
}

void Settings::set_accepting_non_friend_whispers(bool mode) {
    Settings_cat.debug() << "set_accepting_non_friend_whispers(" << mode << ")" << std::endl;
    m_accepting_non_friend_whispers = mode;
}

void Settings::set_sfx_volume(float volume) {
#ifdef HAVE_THEMDIA
    MUTATE_START
#endif
    Settings_cat.debug() << "set_sfx_volume(" << volume << ")" << std::endl;
    m_sfx_volume = encrypt_float(volume);
#ifdef HAVE_THEMDIA
    MUTATE_END
#endif
}

void Settings::set_music_volume(float volume) {
#ifdef HAVE_THEMDIA
    MUTATE_START
#endif
    Settings_cat.debug() << "set_music_volume(" << volume << ")" << std::endl;
    m_music_volume = encrypt_float(volume);
#ifdef HAVE_THEMDIA
    MUTATE_END
#endif
}

void Settings::set_server_type(uint8_t type) {
#ifdef HAVE_THEMDIA
    MUTATE_START
#endif
    Settings_cat.debug() << "set_server_type(" << type << ")" << std::endl;
    m_server_type = encrypt_int(type);
#ifdef HAVE_THEMDIA
    MUTATE_END
#endif
}

void Settings::set_display_driver(uint8_t driver) {
    /**
     * Sets the display driver by using it corrosponding ID.
     */
#ifdef HAVE_THEMDIA
    MUTATE_START
#endif
    Settings_cat.debug() << "set_display_driver(" << driver << ")" << std::endl;
    m_current_driver = encrypt_int(driver);
#ifdef HAVE_THEMDIA
    MUTATE_END
#endif
}

void Settings::set_windowed_mode(uint8_t mode) {
#ifdef HAVE_THEMDIA
    MUTATE_START
#endif
    Settings_cat.debug() << "set_windowed_mode(" << mode << ")" << std::endl;
    m_windowed_mode = encrypt_int(mode);
#ifdef HAVE_THEMDIA
    MUTATE_END
#endif
}

void Settings::set_resolution(uint8_t resolution) {
    /**
     * Sets the Resolution Mode.
     */
#ifdef HAVE_THEMDIA
    MUTATE_START
#endif
    Settings_cat.debug() << "set_resolution(" << resolution << ")" << std::endl;
    m_resolution = encrypt_int(resolution);
#ifdef HAVE_THEMDIA
    MUTATE_END
#endif
}

void Settings::set_resolution_dimensions(uint16_t xsize, uint16_t ysize) {
    /**
     * Sets the Resolution Dimensions.
     */
#ifdef HAVE_THEMDIA
    MUTATE_START
#endif
    Settings_cat.debug() << "set_resolution_dimensions(" << xsize << " " << ysize << ")" << std::endl;
    m_resolution_dimensions[0] = encrypt_int(xsize);
    m_resolution_dimensions[1] = encrypt_int(ysize);
#ifdef HAVE_THEMDIA
    MUTATE_END
#endif
}

uint8_t Settings::server_type() {
    Settings_cat.debug() << "server_type()" << std::endl;
    return decrypt_int(m_server_type);
}

uint8_t Settings::get_resolution() {
    Settings_cat.debug() << "get_resolution()" << std::endl;
    return decrypt_int(m_resolution);
}

uint8_t Settings::get_windowed_mode() {
    Settings_cat.debug() << "get_windowed_mode()" << std::endl;
    return decrypt_int(m_windowed_mode);
}

bool Settings::get_music() {
    Settings_cat.debug() << "get_music()" << std::endl;
    return m_want_music;
}

bool Settings::get_sfx() {
    Settings_cat.debug() << "get_sfx()" << std::endl;
    return m_want_sfx;
}

bool Settings::want_chat_log() {
    Settings_cat.debug() << "want_chat_log()" << std::endl;
    return m_log_chat;
}

bool Settings::get_show_fpsmeter() {
    Settings_cat.debug() << "get_show_fpsmeter()" << std::endl;
    return m_show_fpsmeter;
}

bool Settings::want_custom_mouse_cursor() {
    Settings_cat.debug() << "want_custom_mouse_cursor()" << std::endl;
    return m_custom_mouse_cursor;
}

bool Settings::get_toon_chat_sounds() {
    Settings_cat.debug() << "get_toon_chat_sounds()" << std::endl;
    return m_toon_chat_sounds;
}

bool Settings::get_accepting_new_friends() {
    Settings_cat.debug() << "get_accepting_new_friends()" << std::endl;
    return m_accepting_new_friends;
}

bool Settings::get_accepting_non_friend_whispers() {
    Settings_cat.debug() << "get_accepting_non_friend_whispers()" << std::endl;
    return m_accepting_non_friend_whispers;
}

float Settings::get_sfx_volume() {
    Settings_cat.debug() << "get_sfx_volume()" << std::endl;
    return decrypt_float(m_sfx_volume);
}

float Settings::get_music_volume() {
    Settings_cat.debug() << "get_music_volume()" << std::endl;
    return decrypt_float(m_music_volume);
}

bool Settings::get_embedded_mode() {
    Settings_cat.debug() << "get_embedded_mode()" << std::endl;
    return m_embedded_mode;
}

bool Settings::do_saved_settings_exist() {
    /**
     * If saved Settings exist returns True otherwise False.
     */
    Settings_cat.debug() << "do_saved_settings_exist()" << std::endl;
    return m_vfs->exists(m_file);
}

Settings* Settings::get_global_ptr() {
    if ((_global_ptr == nullptr) || (_global_ptr == NULL)) {
        _global_ptr = new Settings;
    }
    return _global_ptr;
}