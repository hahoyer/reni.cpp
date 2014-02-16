#include "Import.h"

#include "Reni.h"
#include "SimpleTokenFactory.h"

static bool Trace = true;

using namespace HWLang;
using namespace HWLang::PrioTableConst;

namespace _HWLang
{
    using TokenFactory = SimpleTokenFactory;
    using ScannerInstance = TokenFactory::ScannerInstance;
    using TokenClass = TokenFactory::TokenClass;
    using Syntax = TokenFactory::Syntax;

    void Check(OptRef<Syntax const> const& target, bool isLeft, String const& part, bool isRight, bool isMatch){
        a_if(target.IsValid, nd(target));
        a_is(target->name, == , part);
        a_if(target->left.IsValid == isLeft, nd(target));
        a_is(target->right.IsValid, == , isRight);
        a_is(target->isMatch, == , isMatch);
    }

    void ParserBaseStructure()
    {
        auto pt = PrioTable::Left({ Any });
        pt = pt.ParenthesisLevel(Start, PrioTableConst::End);

        String text = "asdf";
        auto sc = ScannerInstance(text);
        auto syntax = Parse<_HWLang::Syntax>(pt, sc);
        
        a_if(syntax           .IsValid, nd(syntax));
        a_if(!syntax->left     .IsValid, nd(syntax));
        a_if(syntax->right      .IsValid, nd(syntax));
        a_if(!syntax->right->left.IsValid, nd(syntax));
        a_if(!syntax->right->right.IsValid, nd(syntax));
        a_is(syntax->right->name, ==, text);
    };

    void Parenthesis()
    {

        String text = "({)} [(asdf)as][yxcv]";

        auto pt = PrioTable::Left({ Any });
        pt = pt.ParenthesisLevel(
            {"(", "[", "{"},
            {")", "]", "}"}
        );
        pt = pt.ParenthesisLevel(Start, PrioTableConst::End);

        d(pt);

        auto sc = _HWLang::ScannerInstance(text);
        OptRef<_HWLang::Syntax const> const syntax = Parse<_HWLang::Syntax>(pt, sc);

        a_if(syntax.IsValid, nd(syntax));
        Check(syntax, false, "", true, false);

        auto r = syntax->right;
        Check(r, false, "(", true, false);

        auto rr = r->right;
        Check(rr, true, "]", false, true);

        auto rrl = rr->left;
        Check(rrl, true, "[", true, false);

        auto rrll = rrl->left;
        Check(rrll, true, "]", false, true);

        auto rrlll = rrll->left;
        Check(rrlll, true, "[", true, false);

        auto rrllll = rrlll->left;
        Check(rrllll, true, "}", false, true);

        auto rrlllll = rrllll->left;
        Check(rrlllll, false, "{", true, false);

        auto rrlllllr = rrlllll->right;
        Check(rrlllllr, false, ")", false, false);

        auto rrlllr = rrlll->right;
        Check(rrlllr, true, "as", false, false);

        auto rrlllrl = rrlllr->left;
        Check(rrlllrl, true, ")", false, true);

        auto rrlllrll = rrlllrl->left;
        Check(rrlllrll, false, "(", true, false);

        auto rrlllrllr = rrlllrll->right;
        Check(rrlllrllr, false, "asdf", false, false);

        auto rrlr = rrl->right;
        Check(rrlr, false, "yxcv", false, false);
    }

}