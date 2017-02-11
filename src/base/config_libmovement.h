#pragma once

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <pandabase.h>
#include <notifyCategoryProxy.h>

#pragma warning (disable : 4273)
#pragma warning (disable : 4275)

#ifdef BUILDING_LIBOTP 
    #define EXPCL_LIBOTP EXPORT_CLASS 
    #define EXPTP_LIBOTP EXPORT_TEMPL 
#else 
    #define EXPCL_LIBOTP IMPORT_CLASS 
    #define EXPTP_LIBOTP IMPORT_TEMPL 
#endif 
 
//NotifyCategoryDecl(libmovement, EXPCL_LIBMOVEMENT, EXPTP_LIBMOVEMENT);
extern void init_libmovement();