#pragma once
#include "Compiler.h"

#include "CodeVisitor.h"
#include "CppCompilerScripting.h"
#include "MainTokenFactory.h"
#include "RootContext.h"
#include "Scanner.h"
#include "Syntax.h"
#include "TokenClass.h"
#include "..\HWLang\PrioTable.h"
#include "..\HWLang\ScannerInstance.h"
#include "..\HWLang\Token.h"
#include "..\HWLang\Source.h"
#include "..\HWLib\ValueCache.h"
#include "..\HWLang\PrioParser.h"

static bool Trace = true;

using namespace HWLib;
using namespace HWLang;

namespace Reni{

    class ScannerInstance final : public HWLang::ScannerInstance<Token, MainTokenFactory, Scanner>{
        using baseType = HWLang::ScannerInstance<Token, MainTokenFactory, Scanner>;
        using thisType = ScannerInstance;
    public:
        ScannerInstance(Ref<Source const> source)
            :baseType(source)
        {};
    };
}

using namespace Reni;


class Compiler::internal final
{
    String const fileName;
public:
    ValueCache<Ref < CodeItem>> codeCache;
    ValueCache<Reni::ScannerInstance> scannerCache;
    ValueCache<Ref<Syntax>> syntaxCache;
    ValueCache<String> cppCodeCache;
private:
    ValueCache<Source const> sourceCache;
    Ref<RootContext> rootContext;
public:
    internal() = delete;
    internal(internal const&) = delete;

    internal(String const&fileName)
        : fileName  (fileName)
        , rootContext(new RootContext)
        , sourceCache([=]{return Source::FromFile(fileName); })
        , scannerCache([=]{return Reni::ScannerInstance(sourceCache.Value); })
        , syntaxCache([=]{return GetSyntax(); })
        , codeCache  ([=]{return GetCode(); })
        , cppCodeCache([=]{return GetCppCode(); })
    {}

    ExecutionResult const Execute(){
        auto s = cppCodeCache.Value;
        dd(s);
        CppCompilerScripting ccs = s;
        ccs.Execute();
        return ExecutionResult{ ccs.result, ccs.output };

    }

private:
    Ref<Syntax> const GetSyntax()const{
        auto scannerInstance = scannerCache.Value;
        return Parse<Syntax, TokenClass, Token>(prioTable, scannerInstance);
    };

    Ref<CodeItem> const GetCode()const{
        auto syntax = syntaxCache.Value;
        return syntax->Code(*rootContext);
    };

    p(PrioTable, prioTable){
        return
            HWLang::PrioTable::CreateLeft({ Any })
            .ParenthesisLevel({ "(", "[", "{" }, { ")", "]", "}" })
            .ParenthesisLevel(Start, End)
            ;
    };

    class CodeVisitor : public Reni::CodeVisitor
    {
        override_p_function(Array<String>, DumpData){ return{}; };
        virtual String Const(Size const size, Ref<BitsConst> const value) const override;

    };

    String const GetCppCode()const{
        CodeVisitor visitor;
        return CppCompilerScripting(codeCache.Value->ToCpp(visitor)).program;
    };

};


