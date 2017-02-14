#pragma once

#ifdef NDEBUG
#undef NDEBUG
#endif
 
#include <pandabase.h>
#include <dconfig.h>
#include <notifyCategoryProxy.h>

#pragma warning (disable : 4273)
#pragma warning (disable : 4275)
#pragma warning (disable : 4217)

#define EXPCL_LIBOTP EXPORT_CLASS 
#define EXPTP_LIBOTP EXPORT_TEMPL 
#define IMPCL_LIBOTP IMPORT_CLASS 
#define IMPTP_LIBOTP IMPORT_TEMPL 

NotifyCategoryDecl(libotp, EXPCL_LIBOTP, EXPTP_LIBOTP);
extern void init_libotp();