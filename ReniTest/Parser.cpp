#include "Import.h"
#include "Parser.h"
#include "SimpleTokenFactory.h"
#include "../Reni/Syntax.h"
#include "../HWLang/PrioTable.h"
#include "../HWLang/PrioParser.h"
#include "../HWAnalyse/TestMacros.h"
#include "../HWAnalyse/TestFixture.h"
#include "../HWLib/RefCountContainer.instance.h"

using namespace HWLang;

namespace _HWLang {

    typedef SimpleTokenFactory TokenFactory;
    using ScannerInstance = TokenFactory::ScannerInstance;
    typedef TokenFactory::TokenClass TokenClass;
    typedef TokenFactory::Syntax Syntax;

    void Check(Ref<Syntax, true> const& target, bool isLeft, String const& part, bool isRight, bool isMatch);

    test_(ParserBaseStructure) {
        auto pt = PrioTable::CreateLeft({Any}).ParenthesisLevel({Start}, {End});

        String text = "asdf";
        auto sc = ScannerInstance(text);
        auto syntax = Parse<Ref<Syntax>, Ref<Syntax, true>, TokenClass, HWLang::Token<TokenClass>>(pt, sc);

        a_if(!syntax.IsEmpty, nd(syntax));
        a_if(syntax->left.IsEmpty, nd(syntax));
        a_if(syntax->right.IsEmpty, nd(syntax));
        a_is(syntax->name, == , text);
    };

    test(Parenthesis, ParserBaseStructure) {

        String text = "({)} [(asdf)as][yxcv]";

        auto pt = PrioTable::CreateLeft({ Any })
            .ParenthesisLevel({ "(", "[", "{" }, { ")", "]", "}" })
            .ParenthesisLevel({Start}, {End})
            ;

        auto sc = ScannerInstance(text);
        auto syntax = Parse<Ref<Syntax>, Ref<Syntax, true>, TokenClass, HWLang::Token<TokenClass>>(pt, sc);
        Check(syntax, false, "(", true, false);

        auto rr = syntax->right;
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

    test(PlusTimes, ParserBaseStructure) {
        String text = "a*b+c*d+e*f";

        auto pt = PrioTable::CreateLeft({ Any })
            .Left({ "*" })
            .Left({ "+" })
            .ParenthesisLevel({ "(", "[", "{" }, { ")", "]", "}" })
            .ParenthesisLevel({Start}, {End})
            ;

        auto sc = ScannerInstance(text);
        auto syntax = Parse<Ref<Syntax>, Ref<Syntax, true>, TokenClass, HWLang::Token<TokenClass>>(pt, sc);
        Check(syntax, true, "+", true, false);

        auto rl = syntax->left;
        Check(rl, true, "+", true, false);

        auto rll = rl->left;
        Check(rll, true, "*", true, false);
        
        auto rlll = rll->left;
        Check(rlll, false, "a", false, false);

        auto rllr = rll->right;
        Check(rllr, false, "b", false, false);

        auto rlr = rl->right;
        Check(rlr, true, "*", true, false);

        auto rlrl = rlr->left;
        Check(rlrl, false, "c", false, false);

        auto rlrr = rlr->right;
        Check(rlrr, false, "d", false, false);

        auto rr = syntax->right;
        Check(rr, true, "*", true, false);

        auto rrl = rr->left;
        Check(rrl, false, "e", false, false);

        auto rrr = rr->right;
        Check(rrr, false, "f", false, false);
    }

    test(ThenElse, ParserBaseStructure) {

        bool Trace = true;
        String text = "x then y else z";

        auto pt = PrioTable::CreateLeft({Any})
            .ThenElseLevel({"then"}, {"else"})
            .ParenthesisLevel({Start}, {End})
            ;

        md(pt);

        auto sc = ScannerInstance(text);
        auto syntax = Parse<Ref<Syntax>, Ref<Syntax, true>, TokenClass, Token<TokenClass>>(pt, sc);
        Check(syntax, true, "else", true, true);

        auto rl = syntax->left;
        Check(rl, true, "then", true, false);

        auto rr = syntax->right;
        Check(rr, false, "z", false, false);

        auto rll = rl->left;
        Check(rll, false, "x", false, false);

        auto rlr = rl->right;
        Check(rlr, false, "y", false, false);
    }

    test(TrainWreck, ParserBaseStructure) {

        bool Trace = true;
        String text = "x y z";

        auto pt = PrioTable()
            .Left({Any})
            .ParenthesisLevel({Start}, {End})
            ;

        //md(pt);

        auto sc = ScannerInstance(text);
        auto syntax = Parse<Ref<Syntax>, Ref<Syntax, true>, TokenClass, Token<TokenClass>>(pt, sc);
        Check(syntax, true, "z", false, false);

        auto rl = syntax->left;
        Check(rl, true, "y", false, false);

        auto rll = rl->left;
        Check(rll, false, "x", false, false);
    }

    void Check(Ref<Syntax, true> const& target, bool isLeft, String const& part, bool isRight, bool isMatch) {
        a_if(!target.IsEmpty, nd(target));
        a_is(target->name, == , part);
        a_if(!target->left.IsEmpty == isLeft, nd(target));
        a_is(!target->right.IsEmpty, == , isRight);
        a_is(target->isMatch, == , isMatch);
    }
}

