#ifndef _H_LIBMOVEMENT_CONFIG_
#define _H_LIBMOVEMENT_CONFIG_

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <pandabase.h>
#include <notifyCategoryProxy.h>

#pragma warning (disable : 4273)
#pragma warning (disable : 4275)

#ifdef BUILDING_LIBMOVEMENT 
    #define EXPCL_LIBMOVEMENT EXPORT_CLASS 
    #define EXPTP_LIBMOVEMENT EXPORT_TEMPL 
#else 
    #define EXPCL_LIBMOVEMENT IMPORT_CLASS 
    #define EXPTP_LIBMOVEMENT IMPORT_TEMPL 
#endif 
 
NotifyCategoryDecl(libmovement, EXPCL_LIBMOVEMENT, EXPTP_LIBMOVEMENT);
extern void init_libmovement();

#endif