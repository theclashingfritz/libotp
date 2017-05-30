#pragma once
 
#include <algorithm>
#include <type_traits>
#include <typeinfo>
#include <memory>
#include <string>
#include <cstdlib>
#include <ctime>

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <pandabase.h>
#include <dconfig.h>
#include <notifyCategoryProxy.h>
#include <luse.h>
#include <nodePath.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#include <psapi.h>
#include <process.h>
#include <Objbase.h>
#endif
#include <Python.h>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC  
#include <crtdbg.h>  
#endif


#ifdef NDEBUG
#undef NDEBUG
#endif

#define ROL(x, y) __asm{rol x, y}
#define ROR(x, y) __asm{ror x, y}

#pragma warning (disable : 4273)
#pragma warning (disable : 4275)
#pragma warning (disable : 4217)
#pragma warning (disable : 4309)

#ifdef BUILDING_LIBOTP
    #define EXPCL_LIBOTP EXPORT_CLASS 
    #define EXPTP_LIBOTP EXPORT_TEMPL 
#else 
    #define EXPCL_LIBOTP IMPORT_CLASS 
    #define EXPTP_LIBOTP IMPORT_TEMPL 
#endif 


typedef pvector<LVecBase4f> color_tuple_t; // (LVecBase4f, LVecBase4f)
typedef pvector<color_tuple_t> color_tuple_tuple_t; // ((LVecBase4f, LVecBase4f), (LVecBase4f, LVecBase4f))
typedef pmap<int, color_tuple_tuple_t> state_map_t; // {state: color_tuple_tuple_t}
typedef pmap<int, color_tuple_t> state_map_simple_t; // {state: color_tuple_t}
typedef pmap<int, NodePath> buttons_map_t;
typedef pmap<unsigned int, state_map_t> color_map_t; // {cc: state_map_t}
typedef pmap<unsigned int, state_map_simple_t> whisper_color_map_t; // {wt: state_map_simple_t}

EXPCL_LIBOTP std::wstring s2ws(const std::string& str);
EXPCL_LIBOTP std::string ws2s(const std::wstring& wstr);
EXPCL_LIBOTP std::string XOR(std::string value, std::string key);
EXPCL_LIBOTP char *XOR(char *value, char *key);

EXPCL_LIBOTP INLINE std::string char_to_string(char * chr);

EXPCL_LIBOTP std::string string_to_hex(const std::string& input);
EXPCL_LIBOTP std::string hex_to_string(const std::string& input);
EXPCL_LIBOTP std::string hex_str_XOR(std::string const & s1, std::string const & s2);

EXPCL_LIBOTP INLINE unsigned int value(char c);

#ifdef WIN32
EXPCL_LIBOTP std::pair<std::string, DWORD> GetProcessNameAndID(DWORD processID);
EXPCL_LIBOTP std::string GetProcessName(DWORD processID);
#endif

EXPCL_LIBOTP PyObject* vectorToList_String(std::vector<std::string> &data);
EXPCL_LIBOTP int process_AES_encrypt(char* data, int size, char* key, char* iv, char* ciphertext);
EXPCL_LIBOTP int process_AES_decrypt(char* data, int size, char* key, char* iv, char* ciphertext);

template <class T>
EXPCL_LIBOTP INLINE void * get_address_of(T thing);

template <class T>
EXPCL_LIBOTP INLINE std::string get_type_name(T thing);

EXPCL_LIBOTP INLINE int get_char_length(char * chr);

EXPCL_LIBOTP void gen_random(char *s, const int len);
EXPCL_LIBOTP char gen_random_char();
EXPCL_LIBOTP std::string gen_random_string(const int len);

EXPTP_LIBOTP INLINE unsigned char rolcharleft(unsigned char x, int n);
EXPTP_LIBOTP INLINE unsigned char rolcharright(unsigned char x, int n);

EXPTP_LIBOTP void rotatecharleft(char *s, const int len, int amount);
EXPTP_LIBOTP void rotatecharright(char *s, const int len, int amount);

EXPTP_LIBOTP static unsigned int decrypt_long(unsigned long long value);

EXPCL_LIBOTP char* AES_encrypt(char* data, char* key, char* iv);
EXPCL_LIBOTP char* AES_decrypt(char* data, char* key, char* iv);
EXPCL_LIBOTP std::string process_key_chunks(std::string chunk1, std::string chunk2, std::string chunk3, std::string chunk4);
BEGIN_PUBLISH
EXPCL_LIBOTP PyObject* list_process_modules();
EXPCL_LIBOTP PyObject* AES_decrypt(PyObject* pdata, PyObject* pkey, PyObject* piv);
EXPCL_LIBOTP PyObject* AES_encrypt(PyObject* pdata, PyObject* pkey, PyObject* piv);
EXPCL_LIBOTP std::string unscramble_key(std::string key1, std::string key2, std::string C);
EXPCL_LIBOTP std::string caculate_deobfuscated_key(std::string chunk1, std::string chunk2, std::string chunk3, std::string chunk4);
EXPCL_LIBOTP std::string caculate_deobfuscated_key(std::string key);
END_PUBLISH

NotifyCategoryDecl(libotp, EXPCL_LIBOTP, EXPTP_LIBOTP);
extern void init_libotp();