#include "NametagGlobals.h"

TypeHandle NametagGlobals::_type_handle;


NametagGlobals::NametagGlobals() {
    CFSpeech = 1;
    CFThought = 2;
    CFQuicktalker = 4;
    CFTimeout = 8;
    CFPageButton = 16;
    CFQuitButton = 32;
    CFReversed = 64;
    CFSndOpenchat = 128;
    CFNoQuitButton = 256;
}

NametagGlobals::~NametagGlobals() {

}