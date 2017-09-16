#include "config_libotp.h"

#include "CMover.h"
#include "CMoverGroup.h"
#include "CImpulse.h"
#include "ClickablePopup.h"
#include "MarginCell.h"
#include "MarginManager.h"
#include "MarginPopup.h"
#include "WhisperPopup.h"
#include "ChatBalloon.h"
#include "NametagGlobals.h"
#include "NametagGroup.h"
#include "Nametag2d.h"
#include "Nametag3d.h"
#include "NametagFloat2d.h"
#include "NametagFloat3d.h"
#include "Nametag.h"
#include "Settings.h"

#include "AESKeyScrambler.h"
#include "AESKeyStore.h"
#include "CPyObjectHandler.h"
#include "CRandom.h" 

ConfigVariableBool temp_hpr_fix
("temp-hpr-fix", true,
  PRC_DESC("Set this true to compute hpr's correctly.  Historically, Panda has "
           "applied these in the wrong order, and roll was backwards relative "
           "to the other two.  Set this false if you need compatibility with "
           "Panda's old hpr calculations."));
 

// These char maps are for if one if spilt characters raises a error 
// and we can just refer to the char from here to fix the error.
char big_char_map[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
char small_char_map[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
char number_char_map[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

// REF: openssl enc -md sha1 -aes-256-cbc -nosalt -P
// The above openssl command is what i used to generate AES keys.
                              
Configure(config_libotp);
NotifyCategoryDef(libotp, "");

ConfigureFn(config_libotp) {
    init_libotp();
};

void init_libotp() {
#ifdef HAVE_THEMDIA
    VM_START 
#endif
    static bool initialized = false;
    if (initialized) {
        return;
    }

    initialized = true;
    
    srand(static_cast<unsigned int>(time(0) + _getpid()));
    
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_config(NULL);
    
    CMover::init_type();
    CMoverGroup::init_type();
    CImpulse::init_type();
    ClickablePopup::init_type();
    MarginCell::init_type();
    MarginManager::init_type();
    //MarginPopup::init_type();
    WhisperPopup::init_type();
    ChatBalloon::init_type();
    NametagGlobals::init_type();
    NametagGroup::init_type();
    Nametag2d::init_type();
    Nametag3d::init_type();
    NametagFloat2d::init_type();
    NametagFloat3d::init_type();
    Nametag::init_type();
    Settings::init_type();
    AESKeyScrambler::init_type();
    AESKeyStore::init_type();
    CPyObjectHandler::init_type();
    CRandom::init_type();
#ifdef HAVE_THEMDIA
    VM_END
#endif
};

template <class T>
ALWAYS_INLINE void * get_address_of(T thing) {
    libotp_cat.debug() << "get_address_of(T thing)" << std::endl;
    return std::addressof(thing);
};

template <class T>
ALWAYS_INLINE std::string get_type_name(T thing) {
    libotp_cat.debug() << "get_type_name(T thing)" << std::endl;
    return typeid(thing).name();
};

ALWAYS_INLINE int get_char_length(char *chr) {
    libotp_cat.debug() << "get_char_length(char * chr)" << std::endl;
    if (chr == NULL || chr == nullptr) {
        return 0;
    }
    std::string chr_string(chr);
    return chr_string.length();
};

ALWAYS_INLINE std::string char_to_string(char *chr) {
    if (chr == NULL || chr == nullptr) {
        return "NULL";
    }
    std::string chr_string(chr);
    return chr_string;
};

template <typename T, typename T2>
ALWAYS_INLINE T wrap_rotate_left(T x,T2 amount) {
	const unsigned bits=sizeof(T)*8;
#if POWER_SIZES
	amount&=bits-1;
#else
	amount%=bits;
#endif
	if (amount) {
		return (x<<amount)|(x>>(bits-amount));
    }
	return x;
}

template <typename T, typename T2>
ALWAYS_INLINE T wrap_rotate_right(T x,T2 amount) {
	const unsigned bits=sizeof(T)*8;
#if POWER_SIZES
	amount&=bits-1;
#else
	amount%=bits;
#endif
	if (amount) {
		return (x>>amount)|(x<<(bits-amount));
    }
	return x;
}

ALWAYS_INLINE char *sum_chars(char *a, char *b) {
    int carry = 0;
    for (int i = 0; i < get_char_length(a); ++i) {
        int sum = a[i] + b[i] + carry;
        a[i] = static_cast<char>(sum & 0xFF);
        carry = sum >> 8;
    }
    return a;
}

ALWAYS_INLINE std::string sum_strings(std::string a, std::string b) {
    int carry = 0;
    for (int i = 0; i < a.length(); ++i) {
        int sum = a[i] + b[i] + carry;
        a[i] = (sum & 0xFF);
        carry = sum >> 8;
    }
    return a;
}

ALWAYS_INLINE char *rotate_char_left(char *s, const int len, int amount) {
    for (int i = 0; i < len; ++i) {
        s[i] = static_cast<char>(wrap_rotate_left(static_cast<unsigned char>(s[i]), amount));
    }
    
    return s;
};

ALWAYS_INLINE void rotate_char_left(char **s, const int len, int amount) {
    char *d = *s;
    for (int i = 0; i < len; ++i) {
        d[i] = static_cast<char>(wrap_rotate_left(static_cast<unsigned char>(d[i]), amount));
    }
};

ALWAYS_INLINE char *rotate_char_right(char *s, const int len, int amount) {
    for (int i = 0; i < len; ++i) {
        s[i] = static_cast<char>(wrap_rotate_right(static_cast<unsigned char>(s[i]), amount));
    }
    
    return s;
};

ALWAYS_INLINE void rotate_char_right(char **s, const int len, int amount) {
    char *d = *s;
    for (int i = 0; i < len; ++i) {
        d[i] = static_cast<char>(wrap_rotate_right(static_cast<unsigned char>(d[i]), amount));
    }
};

ALWAYS_INLINE std::string rotate_string_left(std::string s, const int len, int amount) {
    for (int i = 0; i < len; ++i) {
        s[i] = wrap_rotate_right(s[i], amount);
    }
    
    return s;
};

ALWAYS_INLINE std::string rotate_string_right(std::string s, const int len, int amount) {
    for (int i = 0; i < len; ++i) {
        s[i] = wrap_rotate_right(s[i], amount);
    }
    
    return s;
};

// multi byte to wide char:
std::wstring s2ws(const std::string& str) {
    libotp_cat.debug() << "s2ws(const std::string& str)" << std::endl;
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
};

// wide char to multi byte:
std::string ws2s(const std::wstring& wstr) {
    libotp_cat.debug() << "ws2s(const std::wstring& wstr)" << std::endl;
    int size_needed = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.length() + 1), 0, 0, 0, 0); 
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.length() + 1), &strTo[0], size_needed, 0, 0); 
    return strTo;
};

unsigned int decrypt_int(unsigned long long value) {
    // Unpack 64-bit value into (u32, u16, u8, u8) values.
    volatile uint32_t enc = (value & 0xFFFFFFFF);
    volatile uint16_t adjust = ((value >> 32) & 0xFFFF);
    volatile uint8_t shift_val = ((value >> 48) & 0xFF);
    volatile uint8_t chk = ((value >> 56) & 0xFF);
    
    // Validate 8-bit checksum
    if ((((enc >> 0) + (enc >> 8) + (enc >> 16) + (enc >> 24) + 0xBA) & 0xFF) != chk) {
        return 0;
    }
    
    volatile uint8_t left_shift = ((0x1C - shift_val) & 0xFF);
    volatile uint8_t right_shift = 0x20 - left_shift;
    
    // Handle error case: Invalid shift value.
    if (left_shift >= 0x20) {
        return 0 + (enc << right_shift) - (adjust + 0x8F187432);
    }
    
    // This case should occur for all generated values.
    return (enc << left_shift) + (enc >> right_shift) - (adjust + 0x8F187432);
};

unsigned long long encrypt_int(unsigned int value) {
    libotp_cat.debug() << "encrypt_int(" << value << ")" << std::endl;
#ifdef WIN32
    GUID gid;
    if (CoCreateGuid(&gid) != 0x00000000) {
        return 0;
    }
    
    unsigned long long rvalue = gid.Data1 + gid.Data2 + gid.Data3;
#else
    unsigned long long rvalue = 0;

    for (int i = 0; i < 5; ++i) {
        r = (r << 15) | (rand() & 0x7FFF);
    }

    rvalue = rvalue & 0xFFFFFFFFFFFFFFFFULL;
#endif
    
    CRandom *r = new CRandom(rvalue);
    
    volatile uint16_t adjust = r->Next(0x10000);
    volatile uint8_t shift_val = r->Next(0x1A);
    
    delete r;
    
    volatile uint32_t enc = value + adjust + 0x8F187432;
    
    enc = (enc >> (0x1C - shift_val)) + (enc << (shift_val + 4));
    
    volatile uint8_t chk = (((enc >> 0) + (enc >> 8) + (enc >> 16) + (enc >> 24) + 0xBA) & 0xFF);
    
    return ((static_cast<unsigned long long>(enc) << 0) | (static_cast<unsigned long long>(adjust) << 32) | (static_cast<unsigned long long>(shift_val) << 48) | (static_cast<unsigned long long>(chk) << 56));
};

float decrypt_float(unsigned long long value) {
    // Unpack 64-bit value into (u32, u16, u8, u8) values.
    volatile uint32_t enc = (value & 0xFFFFFFFF);
    volatile uint16_t adjust = ((value >> 32) & 0xFFFF);
    volatile uint8_t shift_val = ((value >> 48) & 0xFF);
    volatile uint8_t chk = ((value >> 56) & 0xFF);
    
    // Validate 8-bit checksum
    if ((((enc >> 0) + (enc >> 8) + (enc >> 16) + (enc >> 24) + 0xBA) & 0xFF) != chk) {
        return 0;
    }
    
    volatile uint8_t left_shift = ((0x1C - shift_val) & 0xFF);
    volatile uint8_t right_shift = 0x20 - left_shift;
    
    // Handle error case: Invalid shift value.
    if (left_shift >= 0x20) {
        return 0 + (enc << right_shift) - (adjust + 0x8F187432);
    }
    
    // This case should occur for all generated values.
    return (enc << left_shift) + (enc >> right_shift) - (adjust + 0x8F187432);
};

unsigned long long encrypt_float(float value) {
    libotp_cat.debug() << "encrypt_float(" << value << ")" << std::endl;
#ifdef WIN32
    GUID gid;
    if (CoCreateGuid(&gid) != 0x00000000) {
        return 0;
    }
    
    unsigned long long rvalue = gid.Data1 + gid.Data2 + gid.Data3;
#else
    unsigned long long rvalue = 0;

    for (int i = 0; i < 5; ++i) {
        r = (r << 15) | (rand() & 0x7FFF);
    }

    rvalue = rvalue & 0xFFFFFFFFFFFFFFFFULL;
#endif
    
    CRandom *r = new CRandom(rvalue);
    
    volatile uint16_t adjust = r->Next(0x10000);
    volatile uint8_t shift_val = r->Next(0x1A);
    
    delete r;
    
    volatile uint32_t enc = value + adjust + 0x8F187432;
    
    enc = (enc >> (0x1C - shift_val)) + (enc << (shift_val + 4));
    
    volatile uint8_t chk = (((enc >> 0) + (enc >> 8) + (enc >> 16) + (enc >> 24) + 0xBA) & 0xFF);
    
    return ((static_cast<unsigned long long>(enc) << 0) | (static_cast<unsigned long long>(adjust) << 32) | (static_cast<unsigned long long>(shift_val) << 48) | (static_cast<unsigned long long>(chk) << 56));
};

std::string string_to_hex(const std::string& input) {
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i) {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
};

std::string hex_to_string(const std::string& input) {
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();
    if (len & 1) throw std::invalid_argument("odd length");

    std::string output;
    output.reserve(len / 2);
    for (size_t i = 0; i < len; i += 2) {
        char a = input[i];
        const char* p = std::lower_bound(lut, lut + 16, a);
        if (*p != a) throw std::invalid_argument("not a hex digit");

        char b = input[i + 1];
        const char* q = std::lower_bound(lut, lut + 16, b);
        if (*q != b) throw std::invalid_argument("not a hex digit");

        output.push_back(((p - lut) << 4) | (q - lut));
    }
    return output;
};

ALWAYS_INLINE unsigned int value(char c) {
    if (c >= '0' && c <= '9') {return c - '0';}
    if (c >= 'a' && c <= 'f') {return c - 'a' + 10;}
    if (c >= 'A' && c <= 'F') {return c - 'A' + 10;}
    return -1;
};

std::string hex_str_XOR(std::string const & s1, std::string const & s2) {
    static char const alphabet[] = "0123456789abcdef";

    std::string result;
    result.reserve(s1.length());

    for (std::size_t i = 0; i != s1.length(); ++i) { 
        unsigned int v = value(s1[i]) ^ value(s2[i]);
        result.push_back(alphabet[v]);
    }
    return result;
};

ALWAYS_INLINE std::string XOR(std::string value, std::string key) {
    std::string retval(value);
    long unsigned int klen = key.length();
    long unsigned int vlen = value.length();
    unsigned long int k = 0;
    unsigned long int v = 0;
    for (; v < vlen; v++) {
      retval[v] = value[v] ^ key[k];
      k = (++k < klen ? k : 0);
    }
    return retval;
};

ALWAYS_INLINE char *XOR(char *value, char *key) {
    long unsigned int klen = get_char_length(key);
    long unsigned int vlen = get_char_length(value);
    unsigned long int k = 0;
    unsigned long int v = 0;
    char *retval = new char[vlen + 12]; //Tack on a extra 12 just to be sure.
    strcpy(retval, value);
    for (; v < vlen; v++) {
      retval[v] = value[v] ^ key[k];
      k = (++k < klen ? k : 0);
    }
    return retval;
};


void gen_random(char *s, const int len) {
    /*
    Fills a char array (The first argument) with the len amount specified (The second argument).
    */
    
	for (int i = 0; i < len; ++i) {
        int randomChar = rand() % (256);
		s[i] = 0x00 + randomChar;
	}
	s[len] = 0;
};

ALWAYS_INLINE char gen_random_char() {
    /*
    This function returns a random char.
    */
	int randomChar = rand() % (256);
    return 0x00 + randomChar;
};

std::string gen_random_string(const int len) {
    /*
    This function returns a randomly generated string from numbers
    and letters the length of the argument specified. 
    */
    
	std::string result;
	for (int i = 0; i < len; ++i) {
		result += gen_random_char();
	}
	return result;
};

PyObject* vectorToList_String(std::vector<std::string> &data) {
    libotp_cat.debug() << "vectorToList_String(std::vector<std::string> &data)" << std::endl;
    PyObject* listObj = PyList_New(data.size());
	if (!listObj) {
        throw logic_error("Unable to allocate memory for Python list");
    }
	for (unsigned int i = 0; i < data.size(); i++) {
		PyObject *str = PyString_FromString(data[i].c_str());
		if (!str) {
			Py_DECREF(listObj);
			throw logic_error("Unable to allocate memory for Python list");
		}
		PyList_SET_ITEM(listObj, i, str);
	}
	return listObj;
};


BEGIN_PUBLISH
PyObject* list_process_modules() {
   libotp_cat.debug() << "list_process_modules()" << std::endl;
   // Get the list of process identifiers.  
   DWORD aProcesses[1024], cbNeeded, cProcesses;
   std::vector<std::string> processNames;
  
#ifdef WIN32
   //This returns a list of handles to processes running on the system as an array.
   if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
      return vectorToList_String(processNames);
   }
  
   // Calculate how many process identifiers were returned.  
   cProcesses = cbNeeded / sizeof(DWORD);
  
   // Display the name and process identifier for each process.  
   for (unsigned int i = 0; i < cProcesses; i++) {
      if(aProcesses[i] != 0) {
         std::string processName = GetProcessName(aProcesses[i]);
         if (processName != "<unknown>") { //This check is here because '<unknown>' isn't helpful at all.
             processNames.push_back(processName); 
         }
      }
   }
#endif
   
   return vectorToList_String(processNames);
};
END_PUBLISH

#ifdef WIN32
std::pair<std::string, DWORD> GetProcessNameAndID(DWORD processID) {
   libotp_cat.debug() << " GetProcessNameAndID(DWORD processID)" << std::endl;
   TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
   std::pair<std::string, DWORD> result;
   std::string processName;
  
   // Get a handle to the process.
   HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
  
   // Get the process name.  
   if (NULL != hProcess) {
      HMODULE hMod;
      DWORD cbNeeded;
     
      //Given a handle to a process, this returns all the modules running within the process.
      //The first module is the executable running the process,
      //and subsequent handles describe DLLs loaded into the process.
      if (EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
         //This function returns the short name for a module,
         //typically the file name portion of the EXE or DLL
         GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR));
      }
   }
   
   processName = szProcessName;
   
   result = std::make_pair(processName, processID);
  
   //close the process handle
   CloseHandle( hProcess );
   
   return result;
};

std::string GetProcessName(DWORD processID) {
   libotp_cat.debug() << " GetProcessName(DWORD processID)" << std::endl;
   TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
   std::string processName;
  
   // Get a handle to the process.
   HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
  
   // Get the process name.  
   if (NULL != hProcess) {
      HMODULE hMod;
      DWORD cbNeeded;
     
      //Given a handle to a process, this returns all the modules running within the process.
      //The first module is the executable running the process,
      //and subsequent handles describe DLLs loaded into the process.
      if (EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
         //This function returns the short name for a module,
         //typically the file name portion of the EXE or DLL
         GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR));
      }
   }
   
   processName = szProcessName;
  
   //close the process handle
   CloseHandle( hProcess );
   
   return processName;
};
#endif

int process_AES_encrypt(char* data, int size, char* key, char* iv, char* ciphertext) {
    libotp_cat.debug() << "process_AES_encrypt(char* data, int size, char* key, char* iv, char* ciphertext)" << std::endl;
    EVP_CIPHER_CTX *ctx;
    int ciphertext_len, len;
    unsigned int keySize = get_char_length(key);
    unsigned int ivSize = get_char_length(iv);

    if (!(ctx = EVP_CIPHER_CTX_new()))
        goto error;

    if (keySize == 32 && ivSize == 16) {
        if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, reinterpret_cast<unsigned char*>(key), reinterpret_cast<unsigned char*>(iv)) != 1) {
            goto error;
        }
    } else if (keySize == 24 && ivSize == 16) {
        if (EVP_EncryptInit_ex(ctx, EVP_aes_192_cbc(), NULL, reinterpret_cast<unsigned char*>(key), reinterpret_cast<unsigned char*>(iv)) != 1) {
            goto error;
        }   
    } else if (keySize == 16 && ivSize == 16) {
        if (EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, reinterpret_cast<unsigned char*>(key), reinterpret_cast<unsigned char*>(iv)) != 1) {
            goto error;
        }   
    } else {
        libotp_cat.warning() << "Key (" << keySize << ") or IV (" << ivSize << ") is not a correct byte size!" << std::endl;
        return -1;
    }

    if (EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char*>(ciphertext), &len, reinterpret_cast<unsigned char*>(data), size) != 1)
        goto error;

    ciphertext_len = len;

    unsigned char * fCiphertext = reinterpret_cast<unsigned char*>(ciphertext);
    if (EVP_EncryptFinal_ex(ctx, &fCiphertext[len], &len) != 1)
        goto error;

    EVP_CIPHER_CTX_free(ctx);
    return ciphertext_len + len;
    
error:
    if (ctx) EVP_CIPHER_CTX_free(ctx);
    ERR_print_errors_fp(stderr);
    return -1;
};

int process_AES_decrypt(char* data, int size, char* key, char* iv, char* plaintext) {
    libotp_cat.debug() << "process_AES_decrypt(char* data, int size, char* key, char* iv, char* plaintext)" << std::endl;
    EVP_CIPHER_CTX *ctx;
    int plaintext_len, len;
    unsigned int keySize = get_char_length(key);
    unsigned int ivSize = get_char_length(iv);

    if (!(ctx = EVP_CIPHER_CTX_new()))
        goto error;
    
    if (keySize == 32 && ivSize == 16) {
        if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, reinterpret_cast<unsigned char*>(key), reinterpret_cast<unsigned char*>(iv)) != 1) {
            goto error;
        }
    } else if (keySize == 24 && ivSize == 16) {
        if (EVP_DecryptInit_ex(ctx, EVP_aes_192_cbc(), NULL, reinterpret_cast<unsigned char*>(key), reinterpret_cast<unsigned char*>(iv)) != 1) {
            goto error;
        }
    } else if (keySize == 16 && ivSize == 16) {
        if (EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, reinterpret_cast<unsigned char*>(key), reinterpret_cast<unsigned char*>(iv)) != 1) {
            goto error;
        }
    } else {
        libotp_cat.warning() << "Key (" << keySize << ") or IV (" << ivSize << ") is not a correct byte size!" << std::endl;
        return -1;
    }

    if (EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(plaintext), &len, reinterpret_cast<unsigned char*>(data), size) != 1)
        goto error;

    plaintext_len = len;

    unsigned char * fPlaintext = reinterpret_cast<unsigned char*>(plaintext);
    if (EVP_DecryptFinal_ex(ctx, fPlaintext + len, &len) != 1)
        goto error;

    EVP_CIPHER_CTX_free(ctx);
    return plaintext_len + len;
 
error:
    if (ctx) EVP_CIPHER_CTX_free(ctx);
    ERR_print_errors_fp(stderr);
    return -1;
};

char* AES_encrypt(char* data, char* key, char* iv) {
    libotp_cat.debug() << "AES_encrypt(char* data, char* key, char* iv)" << std::endl;
    int size, keysize, ivsize, cipherkeysize;
    
    PyObject* args = PyTuple_New(3);
    if (!args) {
        Py_DECREF(args);
        throw logic_error("Unable to allocate memory for Python tuple");
    }
    
    PyObject* pdata = Py_BuildValue("s#", data, get_char_length(data));
    PyObject* pkey = Py_BuildValue("s#", key, get_char_length(key));
    PyObject* piv = Py_BuildValue("s#", iv, get_char_length(iv));
    
    PyTuple_SET_ITEM(args, 0, pdata);
    PyTuple_SET_ITEM(args, 1, pkey);
    PyTuple_SET_ITEM(args, 2, piv);

    if (!PyArg_ParseTuple(args, "s#s#s#", &data, &size, &key, &keysize, &iv, &ivsize)) {
        return NULL;
    }

    if (ivsize <= 15 || keysize <= 15) {
        libotp_cat.warning() << "iv (" << iv << ") must be 16 bytes and key (" << key << ") must be either 16, 24, or 32 bytes. (" << keysize << ", " << ivsize << ") " << std::endl;
        return NULL;
    }

    cipherkeysize = ivsize;

    char* ciphertext = new char[size + cipherkeysize];

    int ciphertext_len = process_AES_encrypt(data, size, key, iv, ciphertext);
    if (ciphertext_len == -1) {
        delete[] ciphertext;
        return NULL;
    }

    return ciphertext;
};

char* AES_decrypt(char* data, char* key, char* iv) {
    libotp_cat.debug() << "AES_decrypt(char* data, char* key, char* iv)" << std::endl;
    int size, keysize, ivsize, cipherkeysize;
    
    PyObject* args = PyTuple_New(3);
    if (!args) {
        Py_DECREF(args);
        throw logic_error("Unable to allocate memory for Python tuple");
    }
    
    PyObject* pdata = Py_BuildValue("s#", data, get_char_length(data));
    PyObject* pkey = Py_BuildValue("s#", key, get_char_length(key));
    PyObject* piv = Py_BuildValue("s#", iv, get_char_length(iv));
    
    PyTuple_SET_ITEM(args, 0, pdata);
    PyTuple_SET_ITEM(args, 1, pkey);
    PyTuple_SET_ITEM(args, 2, piv);

    if (!PyArg_ParseTuple(args, "s#s#s#", &data, &size, &key, &keysize, &iv, &ivsize)) {
        return NULL;
    }

    if (ivsize <= 15 || keysize <= 15) {
        libotp_cat.warning() << "iv (" << iv << ") must be 16 bytes and key (" << key << ") must be either 16, 24, or 32 bytes. (" << keysize << ", " << ivsize << ") " << std::endl;
        return NULL;
    }
    
    cipherkeysize = ivsize;

    char* plaintext = new char[size + cipherkeysize];

    int plaintext_len = process_AES_decrypt(data, size, key, iv, plaintext);
    if (plaintext_len == -1) {
        delete[] plaintext;
        return NULL;
    }

    return plaintext;
};

BEGIN_PUBLISH
PyObject* AES_encrypt(PyObject* pdata, PyObject* pkey, PyObject* piv) {
    libotp_cat.debug() << "AES_encrypt(PyObject* pdata, PyObject* pkey, PyObject* piv)" << std::endl;
    char* data;
    char* key;
    char* iv;
    int size, keysize, ivsize, cipherkeysize;
    
    PyObject* args = PyTuple_New(3);
    if (!args) {
        Py_DECREF(args);
        throw logic_error("Unable to allocate memory for Python tuple");
    }
    
    PyTuple_SET_ITEM(args, 0, pdata);
    PyTuple_SET_ITEM(args, 1, pkey);
    PyTuple_SET_ITEM(args, 2, piv);

    if (!PyArg_ParseTuple(args, "s#s#s#", &data, &size, &key, &keysize, &iv, &ivsize)) {
        return NULL;
    }

    if (ivsize <= 15 || keysize <= 15) {
        PyErr_Format(PyExc_ValueError, "iv must be 16 bytes and key must be either 16, 24, or 32 bytes.");
        return NULL;
    }
    
    cipherkeysize = ivsize;

    char* ciphertext = new char[size + cipherkeysize];

    int ciphertext_len = process_AES_encrypt(data, size, key, iv, ciphertext);
    if (ciphertext_len == -1) {
        delete[] ciphertext;
        Py_INCREF(Py_None);
        return Py_None;
    }

    PyObject* v = Py_BuildValue("s#", ciphertext, ciphertext_len);
    delete[] ciphertext;

    return v;
};

PyObject* AES_decrypt(PyObject* pdata, PyObject* pkey, PyObject* piv) {
    libotp_cat.debug() << "AES_decrypt(PyObject* pdata, PyObject* pkey, PyObject* piv)" << std::endl;
    char* data;
    char* key;
    char* iv;
    int size, keysize, ivsize, cipherkeysize;
    
    PyObject* args = PyTuple_New(3);
    if (!args) {
        Py_DECREF(args);
        throw logic_error("Unable to allocate memory for Python tuple");
    }
    
    PyTuple_SET_ITEM(args, 0, pdata);
    PyTuple_SET_ITEM(args, 1, pkey);
    PyTuple_SET_ITEM(args, 2, piv);

    if (!PyArg_ParseTuple(args, "s#s#s#", &data, &size, &key, &keysize, &iv, &ivsize)) {
        return NULL;
    }

    if (ivsize <= 15 || keysize <= 15) {
        PyErr_Format(PyExc_ValueError, "iv must be 16 bytes and key must be either 16, 24, or 32 bytes.");
        return NULL;
    }
    
    cipherkeysize = ivsize;

    char* plaintext = new char[size + cipherkeysize];

    int plaintext_len = process_AES_decrypt(data, size, key, iv, plaintext);
    if (plaintext_len == -1) {
        delete[] plaintext;
        Py_INCREF(Py_None);
        return Py_None;
    }

    PyObject* v = Py_BuildValue("s#", plaintext, plaintext_len);
    delete[] plaintext;

    return v;
};

std::string unscramble_key(std::string key1, std::string key2, std::string C) {
    std::string nKey = rotate_string_left(key1, key1.length(), 2);
    nKey = XOR(nKey, key2);
    nKey = sum_strings(nKey, C);
    nKey = rotate_string_left(nKey, nKey.length(), 87);

    return nKey;
};
END_PUBLISH

char *unscramble_key(char* key1, char* key2, char* C) {
    if (get_char_length(key1) >= 1028) {
        libotp_cat.warning() << "WARNING: key1 size is bigger then char[] size in unscramble_key()!" << std::endl;
        char* a = new char(2);
        a += 'a';
        a += '\0';
        return a;
    }
    char *nKey = new char(1027);
    nKey = rotate_char_left(key1, get_char_length(key1), 2);
    nKey = XOR(nKey, key2);
    nKey = sum_chars(nKey, C);
    nKey = rotate_char_left(nKey, get_char_length(nKey), 87);

    return nKey;
};

std::string process_key_chunks(std::string chunk1, std::string chunk2, std::string chunk3, std::string chunk4) {
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
    std::string keyChunks[6];
    keyChunks[0] = chunk1;
    keyChunks[1] = gen_random_string(rand() % 30 + 9);
    keyChunks[2] = chunk2;
    keyChunks[3] = gen_random_string(rand() % 30 + 9);
    keyChunks[4] = gen_random_string(rand() % 30 + 9);
    keyChunks[5] = gen_random_string(rand() % 30 + 9);

    // Create our first part from the real chunks.
    std::string keyPart1 = *new std::string(keyChunks[0]) + *new std::string(keyChunks[2]);

    // The last 2 real key chunks with the fakes attached before them with
    // one fake chunk in the middle of the real key chunks.
    keyChunks[0] = gen_random_string(rand() % 30 + 9);
    keyChunks[1] = gen_random_string(rand() % 30 + 9);
    keyChunks[2] = chunk3;
    keyChunks[3] = gen_random_string(rand() % 30 + 9);
    keyChunks[4] = chunk4;
    keyChunks[5] = gen_random_string(rand() % 30 + 9);

    // Create our second part from the real chunks.
    std::string keyPart2 = *new std::string(keyChunks[2]) + *new std::string(keyChunks[4]);

    // Create our actual key!.
    std::string originalKey = *new std::string(keyPart1) + *new std::string(keyPart2);

    //Clean the key parts.
    memcpy(&keyPart1, &gen_random_string(originalKey.length()), originalKey.length());
    memcpy(&keyPart2, &gen_random_string(originalKey.length()), originalKey.length());

    char *keyArray = new char[originalKey.length()];
    memcpy(keyArray, originalKey.c_str(), originalKey.length());
    int size = originalKey.length();

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
    
    std::string returnKey;
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
};

BEGIN_PUBLISH
std::string caculate_deobfuscated_key(std::string chunk1, std::string chunk2, std::string chunk3, std::string chunk4) {
    /*
    This function is for processing four chunks of a key which then returns the original non-obfuscated 
    key. Makes use of process_key_chunks() to caculate the key. Not having the key spilt up into 4 chunks is 
    not recommended as it'll leave your key in memory then until the function is done.
    */
    
    std::string key = process_key_chunks(chunk1, chunk2, chunk3, chunk4);
    return key;
};

std::string caculate_deobfuscated_key(std::string key) {
    /*
    This function is for processing four chunks of a key which then returns the original non-obfuscated 
    key. Makes use of process_key_chunks() to caculate the key. Not having the key spilt up into 4 chunks is 
    not recommended as it'll leave your key in memory then until the function is done.
    */
    
    unsigned int equalChunkLength = key.length() / 4;
    
    std::string chunks[4];
    
    unsigned int n = 0;
    for (unsigned i = 0; i < key.length(); i += equalChunkLength) {
         chunks[n] = key.substr(i, equalChunkLength);
         n += 1;
    }
    
    std::string rkey = process_key_chunks(chunks[0], chunks[1], chunks[2], chunks[3]);
    return rkey;
};
END_PUBLISH