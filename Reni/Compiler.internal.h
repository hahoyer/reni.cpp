#pragma once
#include "Compiler.h"

#include "CodeVisitor.h"
#include "CodeBase.h"
#include "Global.h"
#include "MainTokenFactory.h"
#include "PrioTable.h"
#include "RootContext.h"
#include "Syntax.h"
#include "TokenClass.h"
#include "../HWLang/ScannerInstance.h"
#include "../HWLang/PrioParser.h"
#include "../HWLib/ValueCache.h"
#include "../Util/CppCompilerScripting.h"
#include "../Util/Scanner.h"

using namespace HWLib;
using namespace HWLang;

namespace Reni{

    class ScannerInstance final : public HWLang::ScannerInstance<Token, MainTokenFactory, Scanner>{
        using baseType = HWLang::ScannerInstance<Token, MainTokenFactory, Scanner>;
        using thisType = ScannerInstance;
    public:
        ScannerInstance(Source const& source)
            :baseType(source){
            SetDumpString();
        };
    };
}

using namespace Reni;

class Compiler::internal final
{
    string const fileName;
public:
    ValueCache<CodeBase> codeCache;
    ValueCache<Ref<Syntax>> syntaxCache;
private:
    RootContext rootContext;
public:
    internal() = delete;
    internal(internal const&) = delete;

    internal(string const&fileName)
        : fileName  (fileName)
        , codeCache(l_(code))
        , syntaxCache(l_(syntax))
    {}

    p(string, cppCode)
    {
      const auto codes = codeCache.Value;
      const auto main = codes.cppMain;
      const auto functions = codes.cppFunctions;
      const auto declarations = codes.cppDeclarations;

        static string const result = R"(
#include "Common.h"
using namespace ReniRuntime; 
    
${declarations}

int main(void)
{
    ${main}
}
    
${functions}
)";
        return result
            |Replace("${declarations}", declarations)
            |Replace("${main}", main)
            |Replace("${functions}", functions);
    };

    ExecutionResult Execute() const
    {
        CppCompilerScripting ccs = cppCode;
        ccs.Execute();
        return ExecutionResult{ ccs.result, ccs.output };
    }

    static Ref<Syntax> GetSyntaxFromText(string const& text)
    {
        return GetSyntax(*Source::CreateFromText(text));
    };

private:
    p(Ref<Syntax>,syntax){return GetSyntaxFromFile(fileName);};
    
    static Ref<Syntax> GetSyntaxFromFile(string const& file) {return GetSyntax(*Source::CreateFromFile(file));};

    static Ref<Syntax> GetSyntax(Source const& source)
    {
        auto scannerInstance = Reni::ScannerInstance(source);
        return Parse<Ref<Syntax>, TokenClass, Token>(PrioTable::Main(), scannerInstance);
    };

    p(CodeBase,code)
    {
        auto syntax = syntaxCache.Value;
        const Ref<CodeItem> main = syntax->GetCode(rootContext);
        const Array<Global::Function> functions = rootContext
                                                  .global
                                                  ->functions;

        return CodeBase(main, functions);
    };

};

