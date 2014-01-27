#include "Import.h"
#include "SourcePosition.h"

#include "Source.h"

using namespace Reni;

p_implementation(SourcePosition, bool, IsEnd)
{
    return _source->IsEnd(_position);
}

p_implementation(SourcePosition, Optional<int>, End)
{
    if (IsEnd)
        return 0;
    return Optional<int>();
}
