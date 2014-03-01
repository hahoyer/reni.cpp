#include "Import.h"
#include "BitType.h"

#include "Size.h"

static bool Trace = true;

using namespace Reni;

override_p_implementation(BitType, Size, size){ return Size(1); }
