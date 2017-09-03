#pragma once

#include "util.h"

#include <random>
#include <string>
#include <iostream>

#include <pandabase.h>
#include <notifyCategoryProxy.h>
#include <compress_string.h>
#include <virtualFileSystem.h>
#include <filename.h>
#include <typedObject.h>

NotifyCategoryDecl(AESKeyScrambler, EXPCL_LIBOTP, EXPTP_LIBOTP);

class AESKeyScrambler;
class AESKeyStore;

class EXPCL_LIBOTP AESKeyScrambler : public virtual TypedObject {
    
    public:
        AESKeyScrambler();
        ~AESKeyScrambler();
        
        bool check_key(char *key);
        
        char *get_stored_key(unsigned int index);
        char *get_unscrambled_key(char *key, char *key2);
        
        static AESKeyScrambler *get_global_ptr();
        
    private:     
        AESKeyStore *key_store;
        static AESKeyScrambler *_global_ptr;
        
    TYPE_HANDLE(AESKeyScrambler, TypedObject);
};