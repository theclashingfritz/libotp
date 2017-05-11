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
#include "KeyDeobfuscator.h"
#include "KeyScrambler.h"

Configure(config_libotp);
NotifyCategoryDef(libotp, "");

ConfigureFn(config_libotp) {
    init_libotp();
}

void init_libotp() {
    static bool initialized = false;
    if (initialized)
        return;

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
}

template <class T>
void * get_address_of(T thing) {
    return std::addressof(thing);
}

template <class T>
std::string get_type_name(T thing) {
    return typeid(thing).name();
}

// multi byte to wide char:
std::wstring s2ws(const std::string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

// wide char to multi byte:
std::string ws2s(const std::wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.length() + 1), 0, 0, 0, 0); 
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.length() + 1), &strTo[0], size_needed, 0, 0); 
    return strTo;
}

PyObject* vectorToList_String(std::vector<std::string> &data) {
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
}


BEGIN_PUBLISH
PyObject* list_process_modules() {
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
}
END_PUBLISH

#ifdef WIN32
std::pair<std::string, DWORD> GetProcessNameAndID(DWORD processID) {
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
}

std::string GetProcessName(DWORD processID) {
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
}
#endif

int process_AES_encrypt(char* data, int size, char* key, char* iv, char* ciphertext) {
    EVP_CIPHER_CTX *ctx;
    int ciphertext_len, len;
    string keyS = *new string(key);
    string ivS = *new string(iv);
    unsigned int keySize = keyS.length();
    unsigned int ivSize = ivS.length();

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
}

int process_AES_decrypt(char* data, int size, char* key, char* iv, char* plaintext) {
    EVP_CIPHER_CTX *ctx;
    int plaintext_len, len;
    string keyS = *new string(key);
    string ivS = *new string(iv);
    unsigned int keySize = keyS.length();
    unsigned int ivSize = ivS.length();

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
}

char* AES_encrypt(char* data, char* key, char* iv) {
    int size, keysize, ivsize, cipherkeysize;
    std::string sData = *new string(data);
    std::string sKey = *new string(key);
    std::string sIv = *new string(iv);
    
    PyObject* args = PyTuple_New(3);
    if (!args) {
        Py_DECREF(args);
        throw logic_error("Unable to allocate memory for Python tuple");
    }
    
    PyObject* pdata = Py_BuildValue("s#", data, sData.length());
    PyObject* pkey = Py_BuildValue("s#", key, sKey.length());
    PyObject* piv = Py_BuildValue("s#", iv, sIv.length());
    
    PyTuple_SET_ITEM(args, 0, pdata);
    PyTuple_SET_ITEM(args, 1, pkey);
    PyTuple_SET_ITEM(args, 2, piv);

    if (!PyArg_ParseTuple(args, "s#s#s#", &data, &size, &key, &keysize, &iv, &ivsize)) {
        return NULL;
    }

    if (ivsize <= 15 || keysize <= 15) {
        libotp_cat.warning() << "iv must be 16 bytes and key must be either 16, 24, or 32 bytes. (" << keysize << ", " << ivsize << ") " << std::endl;
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
    int size, keysize, ivsize, cipherkeysize;
    std::string sData = *new string(data);
    std::string sKey = *new string(key);
    std::string sIv = *new string(iv);
    
    PyObject* args = PyTuple_New(3);
    if (!args) {
        Py_DECREF(args);
        throw logic_error("Unable to allocate memory for Python tuple");
    }
    
    PyObject* pdata = Py_BuildValue("s#", data, sData.length());
    PyObject* pkey = Py_BuildValue("s#", key, sKey.length());
    PyObject* piv = Py_BuildValue("s#", iv, sIv.length());
    
    PyTuple_SET_ITEM(args, 0, pdata);
    PyTuple_SET_ITEM(args, 1, pkey);
    PyTuple_SET_ITEM(args, 2, piv);

    if (!PyArg_ParseTuple(args, "s#s#s#", &data, &size, &key, &keysize, &iv, &ivsize)) {
        return NULL;
    }

    if (ivsize <= 15 || keysize <= 15) {
        libotp_cat.warning() << "iv must be 16 bytes and key must be either 16, 24, or 32 bytes. (" << keysize << ", " << ivsize << ") " << std::endl;
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
END_PUBLISH