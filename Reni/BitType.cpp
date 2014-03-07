#include "Import.h"
#include "BitType.h"

#include "ArrayType.h"
#include "Size.h"

static bool Trace = true;

using namespace Reni;

override_p_implementation(BitType, Size, size){ return Size(1); }

BitType::operator Ptr<FeatureProvider<DumpPrintToken, ArrayType>>()const{ 
    md_;
    b_;
    return{};
}

