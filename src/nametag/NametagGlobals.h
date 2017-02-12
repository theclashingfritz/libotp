#pragma once

#include "util.h"
#include <typedObject.h>
#include <pandabase.h>

class NametagGlobals;

class EXPCL_LIBOTP NametagGlobals : public TypedObject
{
    PUBLISHED:
        NametagGlobals();
        ~NametagGlobals();
        
    private:   
        uint8_t CFSpeech;
        uint8_t CFThought;
        uint8_t CFQuicktalker;
        uint8_t CFTimeout;
        uint8_t CFPageButton;
        uint8_t CFQuitButton;
        uint8_t CFReversed;
        uint8_t CFSndOpenchat;
        uint16_t CFNoQuitButton;
        
    TYPE_HANDLE(NametagGlobals, TypedObject);
};