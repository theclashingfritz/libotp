#include "KeyDeobfuscator.h"

using namespace std;

// These char maps are for if one if spilt characters raises a error 
// and we can just refer to the char from here to fix the error.
const string big_char_map[26] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
const string small_char_map[26] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
const string number_char_map[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

void gen_random(char *s, const int len) {
    /*
    Fills a char array (The first argument) with the len amount specified (The second argument).
    */
    
	for (int i = 0; i < len; ++i) {
		int randomChar = rand() % (26 + 26 + 10);
		if (randomChar < 26) {
			s[i] = 'a' + randomChar;
		}
		else if (randomChar < 26 + 26) {
			s[i] = 'A' + randomChar - 26;
		}
		else {
			s[i] = '0' + randomChar - 26 - 26;
		}
	}
	s[len] = 0;
}

char gen_random_char() {
    /*
    This function returns a random char.
    */
    
	int randomChar = rand() % (26 + 26 + 10);
	if (randomChar < 26) {
		return 'a' + randomChar;
	}
	else if (randomChar < 26 + 26) {
		return 'A' + randomChar - 26;
	}
	else {
		return '0' + randomChar - 26 - 26;
	}
}

string gen_random_string(const int len) {
    /*
    This function returns a randomly generated string from numbers
    and letters the length of the argument specified. 
    */
    
	string result;
	for (int i = 0; i < len; ++i) {
		result += gen_random_char();
	}
	return result;
}

string process_key_chunks(string chunk1, string chunk2, string chunk3, string chunk4) {
    /*
    This function is for processing four chunks of a key which then returns the original non-obfuscated 
    key. This function is not to be used directly in less you know exactly what you're doing. It's 
    recommended to use caculate_deobfuscated_key() instead.
    */


    /*
    Our chunks which are real should be spilt up in individual characters. Very tedious 
    but worth doing so the full string doesn't just plop up in the raw unran assembly
    and memory.

    The first 2 real key chunks with the other fake 2 attached to the end with
    one fake chunk in the middle of the real key chunks.
    */
    string keyChunks[6];
    keyChunks[0] = chunk1;
    keyChunks[1] = gen_random_string(rand() % 30 + 9);
    keyChunks[2] = chunk2;
    keyChunks[3] = gen_random_string(rand() % 30 + 9);
    keyChunks[4] = gen_random_string(rand() % 30 + 9);
    keyChunks[5] = gen_random_string(rand() % 30 + 9);

    // Create our first part from the real chunks.
    string keyPart1 = *new string(keyChunks[0]) + *new string(keyChunks[2]);

    // The last 2 real key chunks with the fakes attached before them with
    // one fake chunk in the middle of the real key chunks.
    keyChunks[0] = gen_random_string(rand() % 30 + 9);
    keyChunks[1] = gen_random_string(rand() % 30 + 9);
    keyChunks[2] = chunk3;
    keyChunks[3] = gen_random_string(rand() % 30 + 9);
    keyChunks[4] = chunk4;
    keyChunks[5] = gen_random_string(rand() % 30 + 9);

    // Create our second part from the real chunks.
    string keyPart2 = *new string(keyChunks[2]) + *new string(keyChunks[4]);

    // Create our actual key!.
    string originalKey = *new string(keyPart1) + *new string(keyPart2);

    //Clean the key parts.
    memcpy(&keyPart1, &gen_random_string(originalKey.length()), originalKey.length());
    memcpy(&keyPart2, &gen_random_string(originalKey.length()), originalKey.length());

    char *keyArray = new char[originalKey.length()];
    memcpy(keyArray, originalKey.c_str(), originalKey.length());
    int size = *new int(originalKey.length());

    // Clean the original key.
    memcpy(&originalKey, &gen_random_string(originalKey.length()), originalKey.length()); 

    // Time to do some cleanup!
    memcpy(&keyChunks[0], &gen_random_string(keyChunks[0].length()), keyChunks[0].length());
    memcpy(&keyChunks[1], &gen_random_string(keyChunks[1].length()), keyChunks[1].length());
    memcpy(&keyChunks[2], &gen_random_string(keyChunks[2].length()), keyChunks[2].length());
    memcpy(&keyChunks[3], &gen_random_string(keyChunks[3].length()), keyChunks[3].length());
    memcpy(&keyChunks[4], &gen_random_string(keyChunks[4].length()), keyChunks[4].length());
    memcpy(&keyChunks[5], &gen_random_string(keyChunks[5].length()), keyChunks[5].length());

    // Back to obfuscating then solving the key we go!
    char *key1 = new char[size];
    char *key2 = new char[size];
    char *resultKey = new char[size];

    gen_random(key1, size);

    for (int temp = 0; temp < size; temp++) {
        key2[temp] = keyArray[temp] ^ key1[temp];
    }

    for (int temp = 0; temp < size; temp++) {
        resultKey[temp] = key1[temp] ^ key2[temp];
    }

    // This overwrites the Key Array then we free the overwritten memory with delete[]. (Because it's an array!)
    for (int temp = 0; temp < size; temp++) {
        keyArray[temp] = gen_random_char();
    }
    delete[] keyArray;

    // Now let's cleanup our result arrays!

    // Techinally key1 and key2 could be ANY size however we'll clean
    // the key size just in case. 
    for (int temp = 0; temp < size; temp++) {
        key1[temp] = gen_random_char();
    }
    delete[] key1;

    for (int temp = 0; temp < size; temp++) {
        key2[temp] = gen_random_char();
    }
    delete[] key2;
    
    string returnKey;
    for (int temp = 0; temp < size; temp++) {
        returnKey += resultKey[temp];
    }

    // This is the one that matters most when cleaning. It's the actual result!
    for (int temp = 0; temp < size; temp++) {
        resultKey[temp] = gen_random_char();
    }
    delete[] resultKey;

    // Set the size just so people won't know the size from memory if viewed from when after we're finished.
    size = 0;
    
    return returnKey;
}
            
string caculate_deobfuscated_key(string chunk1, string chunk2, string chunk3, string chunk4) {
    /*
    This function is for processing four chunks of a key which then returns the original non-obfuscated 
    key. Makes use of process_key_chunks() to caculate the key. Not having the key spilt up into 4 chunks is 
    not recommended as it'll leave your key in memory then until the function is done.
    */
    
    string key = process_key_chunks(chunk1, chunk2, chunk3, chunk4);
    return key;
}


string caculate_deobfuscated_key(string key) {
    /*
    This function is for processing four chunks of a key which then returns the original non-obfuscated 
    key. Makes use of process_key_chunks() to caculate the key. Not having the key spilt up into 4 chunks is 
    not recommended as it'll leave your key in memory then until the function is done.
    */
    
    unsigned int equalChunkLength = key.length() / 4;
    
    string chunks[4];
    
    unsigned int n = 0;
    for (unsigned i = 0; i < key.length(); i += equalChunkLength) {
         chunks[n] = key.substr(i, equalChunkLength);
         n += 1;
    }
    
    string rkey = process_key_chunks(chunks[0], chunks[1], chunks[2], chunks[3]);
    return rkey;
}