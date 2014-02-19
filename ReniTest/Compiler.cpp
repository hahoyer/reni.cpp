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

    void Check(OptRef<Syntax const> const& target, bool isLeft, String const& part, bool isRight, bool isMatch);

    void ParserBaseStructure()
    {
        auto pt = PrioTable::CreateLeft({ Any }).ParenthesisLevel(Start, End);

        String text = "asdf";
        auto sc = ScannerInstance(text);
        auto syntax = Parse<Syntax const, TokenClass, Token<TokenClass>>(pt, sc);

        a_if(syntax.IsValid, nd(syntax));
        a_if(!syntax->left.IsValid, nd(syntax));
        a_if(syntax->right.IsValid, nd(syntax));
        a_if(!syntax->right->left.IsValid, nd(syntax));
        a_if(!syntax->right->right.IsValid, nd(syntax));
        a_is(syntax->right->name, == , text);
    };

    void Parenthesis()
    {

        String text = "({)} [(asdf)as][yxcv]";

        auto pt = PrioTable::CreateLeft({ Any })
            .ParenthesisLevel({ "(", "[", "{" },{ ")", "]", "}" })
            .ParenthesisLevel(Start, End)
            ;

        auto sc = ScannerInstance(text);
        auto syntax = Parse<Syntax const, TokenClass, Token<TokenClass>>(pt, sc);

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

    void PlusTimes()
    {
        String text = "a*b+c*d+e*f";

        auto pt = PrioTable::CreateLeft({ Any })
            .Left({ "*" })
            .Left({ "+" })
            .ParenthesisLevel({ "(", "[", "{" }, { ")", "]", "}" })
            .ParenthesisLevel(Start, End)
            ;

        auto sc = ScannerInstance(text);
        auto syntax = Parse<Syntax const, TokenClass, Token<TokenClass>>(pt, sc);
        Check(syntax, false, "", true, false);
        
        auto r = syntax->right;
        Check(r, true, "+", true, false);

        auto rl = r->left;
        Check(rl, true, "*", true, false);

        auto rll = rl->left;
        Check(rll, false, "a", false, false);

        auto rlr = rl->right;
        Check(rlr, false, "b", false, false);

        auto rr = r->right;
        Check(rr, true, "+", true, false);
        

        auto rrl = rr->left;
        Check(rrl, true, "*", true, false);

        auto rrll = rrl->left;
        Check(rrll, false, "c", false, false);

        auto rrlr = rrl->right;
        Check(rrlr, false, "d", false, false);


        auto rrr = rr->right;
        Check(rrr, true, "*", true, false);

        auto rrrl = rrr->left;
        Check(rrrl, false, "e", false, false);

        auto rrrr = rrr->right;
        Check(rrrr, false, "f", false, false);
    }

    void Check(OptRef<Syntax const> const& target, bool isLeft, String const& part, bool isRight, bool isMatch){
        a_if(target.IsValid, nd(target));
        a_is(target->name, == , part);
        a_if(target->left.IsValid == isLeft, nd(target));
        a_is(target->right.IsValid, == , isRight);
        a_is(target->isMatch, == , isMatch);
    }
}