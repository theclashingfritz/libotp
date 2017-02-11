#include "config_libmovement.h"

#include "CMover.h"
#include "CImpulse.h"

Configure(config_libmovement);
//NotifyCategoryDef(libmovement, ":libmovement");

ConfigureFn(config_libmovement)
{
    init_libmovement();
}

void init_libmovement()
{
    static bool initialized = false;
    if (initialized)
        return;

    initialized = true;
    
    CMover::init_type();
    CImpulse::init_type();
}