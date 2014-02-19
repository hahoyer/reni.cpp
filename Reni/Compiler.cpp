#include "Import.h"
#include "Compiler.h"
#include "Compiler.internal.h"
#include "TokenClass.h"

using namespace Reni;

Compiler::Compiler(String const&fileName)
: _internal(new internal(fileName))
{}

p_implementation(Compiler, Array<Reni::Token>, tokens){
    return _internal->scannerCache.Value.ToArray;
};

p_implementation(Compiler, Ref<Syntax const>, syntax){
    return _internal->syntaxCache.Value;
};
