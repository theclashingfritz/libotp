#include "KeyScrambler.h"

using namespace std;

string XOR(string value, string key) {
    string retval(value);
    long unsigned int klen = key.length();
    long unsigned int vlen = value.length();
    unsigned long int k = 0;
    unsigned long int v = 0;
    for (; v < vlen; v++) {
      retval[v] = value[v] ^ key[k];
      k = (++k < klen ? k : 0);
    }
    return retval;
}

string unscramble_key(string key1, string key2, string C) {
    ROL(key1, 56);
    ROL(key2, 12);
    ROL(C, 32);
    
    // Swap the keys to get the real set and confuse anybody trying to decipher
    // the Key Scrambler.
    swap(key1, key2);
    
    //Create 2 keys to swap for the real key.
    string key = ((((XOR(key1, key2)) + C), key2), XOR(key1, key2));
    string key3 = XOR((XOR(((XOR(key1, key2)) + C), key2)), XOR(key1, key2));
    
    //Whoop! Swap the keys again!
    swap(key, key3);
    
    ROL(key, 2);
    return key;
}

string unscramble_key(char * cKey1, char * cKey2, char * chC) {
    string key1 = *new string(cKey1);
    string key2 = *new string(cKey2);
    string C = *new string(chC);
    
    ROL(key1, 56);
    ROL(key2, 12);
    ROL(C, 32);
    
    // Swap the keys to get the real set and confuse anybody trying to decipher
    // the Key Scrambler.
    swap(key1, key2);
    
    //Create 2 keys to swap for the real key.
    string key = ((((XOR(key1, key2)) + C), key2), XOR(key1, key2));
    string key3 = XOR((XOR(((XOR(key1, key2)) + C), key2)), XOR(key1, key2));
    
    //Whoop! Swap the keys again!
    swap(key, key3);
    
    ROL(key, 2);
    return key;
}