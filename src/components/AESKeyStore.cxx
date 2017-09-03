#include "AESKeyStore.h"

NotifyCategoryDef(AESKeyStore, "");

TypeHandle AESKeyStore::_type_handle;

AESKeyStore *AESKeyStore::_global_ptr = nullptr;

AESKeyStore::AESKeyStore() {
    key_store.reserve(20);
    for (unsigned int v = 0; v < key_store.size(); v++) {
        key_store.at(v) = gen_random_string(16);
    }
}

AESKeyStore::~AESKeyStore() {
    for (unsigned int v = 0; v < key_store.size(); v++) {
        key_store.at(v) = gen_random_string(key_store.at(v).size());
    }
    key_store.clear();
}

void AESKeyStore::add_key_to_store(char *key, unsigned int index) {
#ifdef HAVE_THEMDIA
    VM_START
#endif
    if (key == nullptr || key == NULL) {
        return;
    }
    if (key_store.size() < index) {
        AESKeyStore_cat.error() << "Index is out of bounds!" << std::endl;
        return;
    }
    
    std::string new_key(key);
    key_store.at(index) = new_key;
#ifdef HAVE_THEMDIA
    VM_END
#endif
}

void AESKeyStore::add_key_to_store(char *key) {
#ifdef HAVE_THEMDIA
    VM_START
#endif
    if (key == nullptr || key == NULL) {
        return;
    }
    
    std::string new_key(key);
    key_store.push_back(new_key);
#ifdef HAVE_THEMDIA
    VM_END
#endif
}

void AESKeyStore::add_key_to_store(std::string key, unsigned int index) {
#ifdef HAVE_THEMDIA
    VM_START
#endif
    if (key_store.size() < index) {
        AESKeyStore_cat.error() << "Index is out of bounds!" << std::endl;
        return;
    }
    
    key_store.at(index) = key;
#ifdef HAVE_THEMDIA
    VM_END
#endif
}

void AESKeyStore::add_key_to_store(std::string key) {
#ifdef HAVE_THEMDIA
    VM_START
#endif
    key_store.push_back(key);
#ifdef HAVE_THEMDIA
    VM_END
#endif
}

void AESKeyStore::clear_key_store() {
#ifdef HAVE_THEMDIA
    MUTATE_START
#endif
    for (unsigned int v = 0; v < key_store.size(); v++) {
        key_store.at(v) = gen_random_string(key_store.at(v).size());
    }
    key_store.clear();
    key_store.reserve(20);
#ifdef HAVE_THEMDIA
    MUTATE_END
#endif
}

void AESKeyStore::shuffle_keys(std::string rsed) {
#ifdef HAVE_THEMDIA
    VM_START
#endif
    std::seed_seq seed(rsed.begin(), rsed.end());
#if __cplusplus >= 201103L
    std::shuffle(key_store.begin(), key_store.end(), std::default_random_engine(seed));
#else
    std::array<unsigned,2> sequence;
    seed.generate(sequence.begin(), sequence.end());
    srand(sequence[0] + sequence[1]);
    std::random_shuffle(key_store.begin(), key_store.end());
#endif
#ifdef HAVE_THEMDIA
    VM_END
#endif
}

char *AESKeyStore::get_key_from_store(unsigned int index) {
    if (key_store.size() < index) {
        AESKeyStore_cat.error() << "Index is out of bounds!" << std::endl;
        return "ERROR";
    }
    
    return strdup(key_store.at(index).c_str());
}

AESKeyStore *AESKeyStore::get_global_ptr() {
    if ((_global_ptr == nullptr) || (_global_ptr == NULL)) {
        _global_ptr = new AESKeyStore;
    }
    return _global_ptr;
}