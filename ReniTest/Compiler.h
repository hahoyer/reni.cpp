#pragma once
#include "SimpleTokenFactory.h"
#include "../Reni/Syntax.h"
#include "../HWLang/PrioTable.h"
#include "../HWLang/PrioParser.h"
#include "../HWAnalyse/TestMacros.h"
#include "../HWAnalyse/TestFixture.h"

using namespace HWLang;

namespace _HWLang
{
    typedef SimpleTokenFactory TokenFactory;
    using ScannerInstance = TokenFactory::ScannerInstance;
    typedef TokenFactory::TokenClass TokenClass;
    typedef TokenFactory::Syntax Syntax;

    void Check(Ref<Syntax, true> const& target, bool isLeft, String const& part, bool isRight, bool isMatch);

    test_(ParserBaseStructure)
    {
        auto pt = PrioTable::CreateLeft({Any}).ParenthesisLevel(Start, End);

        String text = "asdf";
        auto sc = ScannerInstance(text);
        auto syntax = Parse<Ref<Syntax>, Ref<Syntax, true>, TokenClass, HWLang::Token<TokenClass>>(pt, sc);

        a_if(!syntax.IsEmpty, nd(syntax));
        a_if(syntax->left.IsEmpty, nd(syntax));
        a_if(syntax->right.IsEmpty, nd(syntax));
        a_is(syntax->name, == , text);
    };

    test(Parenthesis, ParserBaseStructure)
    {

        String text = "({)} [(asdf)as][yxcv]";

        auto pt = PrioTable::CreateLeft({Any})
            .ParenthesisLevel({"(", "[", "{"}, {")", "]", "}"})
            .ParenthesisLevel(Start, End)
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

    test_(PlusTimes)
    {
        String text = "a*b+c*d+e*f";

        auto pt = PrioTable::CreateLeft({Any})
            .Left({"*"})
            .Left({"+"})
            .ParenthesisLevel({"(", "[", "{"}, {")", "]", "}"})
            .ParenthesisLevel(Start, End)
            ;

        auto sc = ScannerInstance(text);
        auto syntax = Parse<Ref<Syntax>, Ref<Syntax, true>, TokenClass, HWLang::Token<TokenClass>>(pt, sc);
        Check(syntax, true, "+", true, false);

        auto rl = syntax->left;
        Check(rl, true, "*", true, false);

        auto rll = rl->left;
        Check(rll, false, "a", false, false);

        auto rlr = rl->right;
        Check(rlr, false, "b", false, false);

        auto rr = syntax->right;
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

    void Check(Ref<Syntax, true> const& target, bool isLeft, String const& part, bool isRight, bool isMatch){
        a_if(!target.IsEmpty, nd(target));
        a_is(target->name, == , part);
        a_if(!target->left.IsEmpty == isLeft, nd(target));
        a_is(!target->right.IsEmpty, == , isRight);
        a_is(target->isMatch, == , isMatch);
    }
}

