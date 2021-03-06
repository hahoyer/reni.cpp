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
#include "../Util/BitsConst.h"
#include "../Util/CppCompilerScripting.h"
#include "../Util/Scanner.h"
#include "../ReniTest/Reni.h"

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
    String const fileName;
public:
    ValueCache<CodeBase> codeCache;
    ValueCache<Ref<Syntax>> syntaxCache;
private:
    RootContext rootContext;
public:
    internal() = delete;
    internal(internal const&) = delete;

    internal(String const&fileName)
        : fileName  (fileName)
        , syntaxCache(l_(syntax))
        , codeCache(l_(code))
    {}

    p(String, cppCode)
    {
        auto codes = codeCache.Value;
        auto main = codes.cppMain;
        auto functions = codes.cppFunctions;
        auto declarations = codes.cppDeclarations;

        static String const result = R"(
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
            .Replace("${declarations}", declarations)
            .Replace("${main}", main)
            .Replace("${functions}", functions);
    };

    ExecutionResult const Execute()
    {
        CppCompilerScripting ccs = cppCode;
        ccs.Execute();
        return ExecutionResult{ ccs.result, ccs.output };
    }

    static Ref<Syntax> const GetSyntaxFromText(String const& text)
    {
        return GetSyntax(*Source::CreateFromText(text));
    };

private:
    p(Ref<Syntax>,syntax){return GetSyntaxFromFile(fileName);};
    
    static Ref<Syntax> const GetSyntaxFromFile(String const& file){return GetSyntax(*Source::CreateFromFile(file));};

    static Ref<Syntax> const GetSyntax(Source const&source)
    {
        auto scannerInstance = Reni::ScannerInstance(source);
        return Parse<Ref<Syntax>, TokenClass, Token>(PrioTable::Main(), scannerInstance);
    };

    p(CodeBase,code)
    {
        auto syntax = syntaxCache.Value;
        Ref<CodeItem> main = syntax->Code(rootContext);
        Array<Global::Function> functions = rootContext
            .global
            ->functions;

        return CodeBase(main, functions);
    };

};

