#pragma once

#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <typedObject.h>
#include <pandabase.h>

#include <string.h>

EXPCL_LIBOTP string process_key_chunks(string chunk1, string chunk2, string chunk3, string chunk4);
EXPCL_LIBOTP string caculate_deobfuscated_key(string chunk1, string chunk2, string chunk3, string chunk4);
EXPCL_LIBOTP string caculate_deobfuscated_key(string key);