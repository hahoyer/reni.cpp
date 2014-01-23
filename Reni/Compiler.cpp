#include "Import.h"
#include "Compiler.h"

#include "Compiler.internal.h"

using namespace Reni;

Compiler::Compiler(String const&fileName)
: _internal(new internal(fileName))
{}

p_implementation(Compiler, Source, source)
{
    return *_internal->SourceCache;
}
