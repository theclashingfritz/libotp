#pragma once

#include "util.h"

#include <string>
#include <iostream>
#include <random>
#include <algorithm>
#include <array>

#include <notifyCategoryProxy.h>
#include <typedObject.h>
#include <compress_string.h>
#include <virtualFileSystem.h>
#include <filename.h>
#include <pandabase.h>

NotifyCategoryDecl(AESKeyStore, EXPCL_LIBOTP, EXPTP_LIBOTP);

class AESKeyStore;

class EXPCL_LIBOTP AESKeyStore : public TypedObject {
    
    public:
        AESKeyStore();
        ~AESKeyStore();
        
        void add_key_to_store(char *key, unsigned int index);
        void add_key_to_store(char *key);
        void add_key_to_store(std::string key, unsigned int index);
        void add_key_to_store(std::string key);
        
        void clear_key_store();
        
        void shuffle_keys(std::string rsed);
        
        char *get_key_from_store(unsigned int index);
        
        static AESKeyStore *get_global_ptr();
        
    private:  
        std::vector<std::string> key_store;
        
        static AESKeyStore *_global_ptr;
        
    TYPE_HANDLE(AESKeyStore, TypedObject);
};