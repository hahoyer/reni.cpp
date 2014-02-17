#include "Import.h"
#include "Compiler.h"

#include "Compiler.internal.h"
#include "MainTokenFactory.h"

using namespace Reni;

class ScannerInstance final : public HWLang::ScannerInstance<Token<TokenClass>, MainTokenFactory, Scanner>{
    using baseType = HWLang::ScannerInstance<Token<TokenClass>, MainTokenFactory, Scanner>;
    using thisType = ScannerInstance;
public:
    ScannerInstance(String const&text)
        :baseType(new Source(Source::FromText(text)))
    {};
    ScannerInstance(File const&file)
        :baseType(new Source(Source::FromFile(file.FullName)))
    {};
};


Compiler::Compiler(String const&fileName)
: _internal(new internal(fileName))
{}

p_implementation(Compiler, Ref<Source const>, source)
{
    return _internal->sourceCache.Value;
}