#pragma once
 
#include <pandabase.h>
#include <dconfig.h>
#include <notifyCategoryProxy.h>
#include <luse.h>
#include <nodePath.h>
#include <stdio.h>

#ifdef NDEBUG
#undef NDEBUG
#endif

#pragma warning (disable : 4273)
#pragma warning (disable : 4275)
#pragma warning (disable : 4217)

#define EXPCL_LIBOTP EXPORT_CLASS 
#define EXPTP_LIBOTP EXPORT_TEMPL 
#define IMPCL_LIBOTP IMPORT_CLASS 
#define IMPTP_LIBOTP IMPORT_TEMPL 

typedef pvector<LVecBase4f> color_tuple_t; // (LVecBase4f, LVecBase4f)
typedef pvector<color_tuple_t> color_tuple_tuple_t; // ((LVecBase4f, LVecBase4f), (LVecBase4f, LVecBase4f))
typedef pmap<int, color_tuple_tuple_t> state_map_t; // {state: color_tuple_tuple_t}
typedef pmap<int, color_tuple_t> state_map_simple_t; // {state: color_tuple_t}
typedef pmap<int, NodePath> buttons_map_t;
typedef pmap<unsigned int, state_map_t> color_map_t; // {cc: state_map_t}
typedef pmap<unsigned int, state_map_simple_t> whisper_color_map_t; // {wt: state_map_simple_t}

NotifyCategoryDecl(libotp, EXPCL_LIBOTP, EXPTP_LIBOTP);
extern void init_libotp();