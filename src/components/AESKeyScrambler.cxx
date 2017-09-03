#include "AESKeyScrambler.h"
#include "AESKeyStore.h"

NotifyCategoryDef(AESKeyScrambler, "");

TypeHandle AESKeyScrambler::_type_handle;

AESKeyScrambler *AESKeyScrambler::_global_ptr = nullptr;


AESKeyScrambler::AESKeyScrambler() {
#ifdef HAVE_THEMDIA
    VM_START
#endif
    AESKeyScrambler_cat.spam() << "Begin Crash Test Area" << std::endl;
    VirtualFileSystem *m_vfs = VirtualFileSystem::get_global_ptr();
    Filename m_file = Filename("/ropt"); //First let's use this to set our dir.
    m_file = Filename(m_file.to_os_long_name()); //Now let's do the real file.
    
    std::vector<std::string> aes_key_index;
    char key[17] = {0xE5, 0xE9, 0xFA, 0x1B, 0xA3, 0x1E, 0xCD, 0x1A, 0xE8, 0x4F, 0x75, 0xCA, 0xAA, 0x47, 0x4F, 0x3A, '\0'};
    aes_key_index.push_back(std::string(key));
    char key2[17] = {0xA9, 0x38, 0x7F, 0x9D, 0x28, 0x70, 0x3E, 0x20, 0x93, 0x03, 0xDF, 0x92, 0x07, 0x4C, 0x4A, 0xF7, '\0'};
    aes_key_index.push_back(std::string(key2));
    char key3[17] = {0xB9, 0x8E, 0x95, 0xCE, 0xCA, 0x3E, 0x4D, 0x17, 0x1F, 0x76, 0xA9, 0x4D, 0xE9, 0x34, 0xC0, 0x53, '\0'};
    aes_key_index.push_back(std::string(key3));
    char key4[17] = {0xC6, 0x6E, 0x23, 0x12, 0x8F, 0x28, 0x91, 0x33, 0xF0, 0x4C, 0xDB, 0x87, 0x7A, 0x37, 0x49, 0xF2, '\0'};
    aes_key_index.push_back(std::string(key4));
    char key5[17] = {0xA3, 0x12, 0x33, 0x28, 0x0B, 0xB4, 0xDA, 0xA7, 0x76, 0x13, 0x93, 0xF7, 0x8C, 0x42, 0x49, 0x52, '\0'};
    aes_key_index.push_back(std::string(key5));
    char key6[17] = {0xFF, 0x33, 0x88, 0xEC, 0xD2, 0x17, 0x05, 0xBB, 0x33, 0x9E, 0x96, 0x79, 0x86, 0xDC, 0x49, 0x07, '\0'};
    aes_key_index.push_back(std::string(key6));
    char key7[17] = {0x1F, 0xF9, 0xE9, 0xAA, 0xC5, 0xFE, 0x04, 0x08, 0x02, 0x45, 0x91, 0xDC, 0x5D, 0x52, 0x76, 0x8A, '\0'};
    aes_key_index.push_back(std::string(key7));;
    char key8[17] = {0xE9, 0x3D, 0xA4, 0x65, 0xB3, 0x09, 0xC5, 0x3F, 0xEC, 0x5F, 0xF9, 0x3C, 0x96, 0x37, 0xDA, 0x58, '\0'};
    aes_key_index.push_back(std::string(key8));
    char key9[17] = {0x86, 0xF7, 0xE4, 0x37, 0xFA, 0xA5, 0xA7, 0xFC, 0xE1, 0x5D, 0x1D, 0xDC, 0xB9, 0xEA, 0xEA, 0xEA, '\0'};
    aes_key_index.push_back(std::string(key9));
    char key10[17] = {0x37, 0x76, 0x67, 0xB8, 0x1B, 0x7F, 0xEE, 0xA3, 0x77, 0x1E, 0xAD, 0xAB, 0x99, 0x06, 0x17, 0x11, '\0'};
    aes_key_index.push_back(std::string(key10));
    char key11[17] = {0xC6, 0x6E, 0x23, 0x12, 0x1B, 0xB4, 0xDA, 0xA3, 0x76, 0x13, 0x1D, 0xDC, 0xB9, 0x42, 0x49, 0xEA, '\0'};
    aes_key_index.push_back(std::string(key11));
    char key12[17] = {0x3F, 0xDA, 0x95, 0x24, 0xDB, 0x0B, 0x08, 0xC4, 0x68, 0xA5, 0x37, 0x4A, 0xA8, 0x9B, 0x38, 0xB9, '\0'};
    aes_key_index.push_back(std::string(key12));
    
    
    // Normally i'd delete the keys. But doing so causes a crash so we'll just let them leak.....
    
    AESKeyScrambler_cat.info() << "Crash Test Area 0" << std::endl;
    
    std::string m_data;
    if (!m_vfs->exists(m_file)) {
        AESKeyScrambler_cat.debug() << "Failed to find RSED file! Creating file...." << std::endl;
        m_data = gen_random_string(30);
        m_vfs->write_file(m_file, compress_string(m_data, 9), 0);
    } else {
        m_vfs->read_file(m_file, m_data, true);
        if (m_data.size() > 0) {
            m_data = decompress_string(m_data);
        } else {
            AESKeyScrambler_cat.debug() << "RSED file was invalid! Recreating file...." << std::endl;
            m_vfs->delete_file(m_file);
            m_data = gen_random_string(30);
            m_vfs->write_file(m_file, compress_string(m_data, 9), 0);
        }
    }
    
    AESKeyScrambler_cat.spam() << "Crash Test Area 1" << std::endl;
    
    key_store = new AESKeyStore;
    
    AESKeyScrambler_cat.spam() << "Crash Test Area 2" << std::endl;
    for (unsigned int v = 0; v < aes_key_index.size(); v++) {
        key_store->add_key_to_store(aes_key_index.at(v));
    }
    AESKeyScrambler_cat.spam() << "Crash Test Area 3" << std::endl;
    
    key_store->shuffle_keys(m_data);
    
    std::vector<std::string>::iterator it;
    for (it = aes_key_index.begin(); it != aes_key_index.end(); it++) {
       it = aes_key_index.erase(it);
    }
    aes_key_index.clear();
#ifdef HAVE_THEMDIA
    VM_END
#endif
}

AESKeyScrambler::~AESKeyScrambler() {
    key_store->clear_key_store();
    delete key_store;
}

bool AESKeyScrambler::check_key(char *key) {
    if (get_char_length(key) <= 15) {
        VirtualFileSystem *m_vfs = VirtualFileSystem::get_global_ptr();
        Filename m_file = Filename("/ropt"); //First let's use this to set our dir.
        m_file = Filename(m_file.to_os_long_name()); //Now let's do the real file.
        if (m_vfs->exists(m_file)) {
            AESKeyScrambler_cat.debug() << "RSED file was invalid! (Corrupt Seed?) Recreating file...." << std::endl;
            m_vfs->delete_file(m_file);
            std::string m_data = gen_random_string(30);
            m_vfs->write_file(m_file, compress_string(m_data, 9), 0);
            key_store->shuffle_keys(m_data);
        }
        return 0;
    }
    return 1;
}

char *AESKeyScrambler::get_stored_key(unsigned int index) {
    return key_store->get_key_from_store(index);
}

char *AESKeyScrambler::get_unscrambled_key(char *key, char *key2) {
    char *store_key = key_store->get_key_from_store(7);
    if (!check_key(store_key)) {
        AESKeyScrambler_cat.debug() << "Key Store has been re-shuffled!" << std::endl;
    }
    char *unscrambled_key = unscramble_key(key, key2, store_key);
    AESKeyScrambler_cat.debug() << "Scrambled Key Size is " << get_char_length(unscrambled_key) << "" << std::endl;
    rotate_char_left(unscrambled_key, get_char_length(unscrambled_key), 12);
    char *xored_key = XOR(unscrambled_key, store_key);
    rotate_char_right(xored_key, get_char_length(xored_key), 2);
    rotate_char_right(unscrambled_key, get_char_length(unscrambled_key), 4);
    int carry = 0;
    for (int i = 0; i < get_char_length(unscrambled_key); ++i) {
        int sum = unscrambled_key[i] + xored_key[i] + carry;
        unscrambled_key[i] = static_cast<char>(sum & 0xFF);
        carry = sum >> 8;
    }
    delete xored_key;
    carry = 0;
    for (int i = 0; i < get_char_length(unscrambled_key); ++i) {
        int sum = unscrambled_key[i] + store_key[i] + carry;
        unscrambled_key[i] = static_cast<char>(sum & 0xFF);
        carry = sum >> 8;
    }
    free(store_key);
    store_key = nullptr;
    AESKeyScrambler_cat.debug() << "Unscrambled Key Size is " << get_char_length(unscrambled_key) << "" << std::endl;
    return unscrambled_key;
}

AESKeyScrambler *AESKeyScrambler::get_global_ptr() {
    if ((_global_ptr == nullptr) || (_global_ptr == NULL)) {
        _global_ptr = new AESKeyScrambler;
    }
    return _global_ptr;
}