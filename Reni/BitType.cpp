#include "Import.h"
#include "BitType.h"

#include "Size.h"

using namespace Reni;
static bool Trace = true;

p_implementation(BitType, Size, size){ return Size(1); }
