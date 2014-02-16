#include "Import.h"

#include "Reni.h"
#include "SimpleTokenFactory.h"

static bool Trace = true;

using namespace HWLang;
using namespace HWLang::PrioTableConst;


namespace _HWLang
{

    void ParserBaseStructure()
    {
        using TokenFactory = SimpleTokenFactory;
        using ScannerInstance = TokenFactory::ScannerInstance;
        using TokenClass = TokenFactory::TokenClass;
        using Syntax = TokenFactory::Syntax;

        String text = "asdf";
        Ref<Source const> s = new Source(Source::FromText(text));

        auto pt = PrioTable::Left({ Any });
        pt = pt.ParenthesisLevel(Start, PrioTableConst::End);

        auto sc = ScannerInstance(s);

        auto syntax = Parse<Syntax>(pt, sc);
        
        a_if(syntax           .IsValid, nd(syntax));
        a_if(!syntax->left     .IsValid, nd(syntax));
        a_if(syntax->right      .IsValid, nd(syntax));
        a_if(!syntax->right->left.IsValid, nd(syntax));
        a_if(!syntax->right->right.IsValid, nd(syntax));
        a_is(syntax->right->name, ==, text);
    }
}