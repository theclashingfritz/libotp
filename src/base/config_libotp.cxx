#include "config_libotp.h"

#include "CMover.h"
#include "CImpulse.h"
#include "NametagGlobals.h"

Configure(config_libotp);
//NotifyCategoryDef(libotp, ":libotp");

ConfigureFn(config_libotp) 
{
    init_libotp();
}

void init_libotp()
{
    static bool initialized = false;
    if (initialized)
        return;

    initialized = true;
    
    CMover::init_type();
    CImpulse::init_type();
    NametagGlobals::init_type();
}