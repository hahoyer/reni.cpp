#include "Import.h"
#include "Compiler.h"

#include "Compiler.internal.h"

using namespace Reni;

Compiler::Compiler(String const&fileName)
: _internal(new internal(fileName))
{}

p_implementation(Compiler, Ref<Source const>, source)
{
    return _internal->sourceCache.Value;
}


p_implementation(Compiler, Ref<Compiler::Scanner const>, scanner)
{
    return _internal->scannerCache.Value;
}
