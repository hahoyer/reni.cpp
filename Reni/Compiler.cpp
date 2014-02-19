#include "Import.h"
#include "Compiler.h"
#include "Compiler.internal.h"
#include "TokenClass.h"

using namespace Reni;

Compiler::Compiler(String const&fileName)
: _internal(new internal(fileName))
{}

