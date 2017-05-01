#pragma once

#include "util.h"

#include "KeyDeobfuscator.h"

#include <algorithm>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <typedObject.h>
#include <pandabase.h>

#include <string.h>

EXPCL_LIBOTP string XOR(string value, string key);
EXPCL_LIBOTP string unscramble_key(string key1, string key2, string C);
EXPCL_LIBOTP string unscramble_key(char * cKey1, char * cKey2, char * chC);