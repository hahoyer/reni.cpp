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

namespace TestHWLang {

    typedef SimpleTokenFactory TokenFactory;
    using ScannerInstance = TokenFactory::ScannerInstance;
    typedef TokenFactory::TokenClass TokenClass;
    typedef TokenFactory::Syntax Syntax;

    void Check(Optional<Ref<Syntax>> const& target, bool isLeft, string const& part, bool isRight, bool isMatch);

    HW_TM_IMPLEMENTATION(ParserBaseStructure) {
        auto pt = PrioTable::CreateLeft({Any}).ParenthesisLevelLeft({Start}, {End});

        string text = "asdf";
        auto sc = ScannerInstance(text);
        auto syntax = Parse<Ref<Syntax>, TokenClass, HWLang::Token<TokenClass>>(pt, sc);

        HW_ASSERT(!syntax.IsEmpty, HW_D_VALUE(syntax));
        HW_ASSERT(syntax.Value->left.IsEmpty, HW_D_VALUE(syntax));
        HW_ASSERT(syntax.Value->right.IsEmpty, HW_D_VALUE(syntax));
        HW_ASSERT_IS(syntax.Value->name, == , text);
    };

    HW_TM_IMPLEMENTATION(Parenthesis, ParserBaseStructure) {

        string text = "({)} [(asdf)as][yxcv]";

        auto pt = PrioTable::CreateLeft({ Any })
            .ParenthesisLevelRight({ "(", "[", "{" }, { ")", "]", "}" })
            .ParenthesisLevelLeft({Start}, {End})
            ;

        auto sc = ScannerInstance(text);
        auto syntax = Parse<Ref<Syntax>, TokenClass, HWLang::Token<TokenClass>>(pt, sc);
        Check(syntax, false, "(", true, false);

        auto rr = syntax.Value->right;
        Check(rr, true, "]", false, true);

        auto rrl = rr.Value->left;
        Check(rrl, true, "[", true, false);

        auto rrll = rrl.Value->left;
        Check(rrll, true, "]", false, true);

        auto rrlll = rrll.Value->left;
        Check(rrlll, true, "[", true, false);

        auto rrllll = rrlll.Value->left;
        Check(rrllll, true, "}", false, true);

        auto rrlllll = rrllll.Value->left;
        Check(rrlllll, false, "{", true, false);

        auto rrlllllr = rrlllll.Value->right;
        Check(rrlllllr, false, ")", false, false);

        auto rrlllr = rrlll.Value->right;
        Check(rrlllr, true, "as", false, false);

        auto rrlllrl = rrlllr.Value->left;
        Check(rrlllrl, true, ")", false, true);

        auto rrlllrll = rrlllrl.Value->left;
        Check(rrlllrll, false, "(", true, false);

        auto rrlllrllr = rrlllrll.Value->right;
        Check(rrlllrllr, false, "asdf", false, false);

        auto rrlr = rrl.Value->right;
        Check(rrlr, false, "yxcv", false, false);
    }

    HW_TM_IMPLEMENTATION(PlusTimes, ParserBaseStructure) {
        string text = "a*b+c*d+e*f";

        auto pt = PrioTable::CreateLeft({ Any })
            .Left({ "*" })
            .Left({ "+" })
            .ParenthesisLevelRight({ "(", "[", "{" }, { ")", "]", "}" })
            .ParenthesisLevelLeft({Start}, {End})
            ;

        auto sc = ScannerInstance(text);
        auto syntax = Parse<Ref<Syntax>, TokenClass, HWLang::Token<TokenClass>>(pt, sc);
        Check(syntax, true, "+", true, false);

        auto rl = syntax.Value->left;
        Check(rl, true, "+", true, false);

        auto rll = rl.Value->left;
        Check(rll, true, "*", true, false);
        
        auto rlll = rll.Value->left;
        Check(rlll, false, "a", false, false);

        auto rllr = rll.Value->right;
        Check(rllr, false, "b", false, false);

        auto rlr = rl.Value->right;
        Check(rlr, true, "*", true, false);

        auto rlrl = rlr.Value->left;
        Check(rlrl, false, "c", false, false);

        auto rlrr = rlr.Value->right;
        Check(rlrr, false, "d", false, false);

        auto rr = syntax.Value->right;
        Check(rr, true, "*", true, false);

        auto rrl = rr.Value->left;
        Check(rrl, false, "e", false, false);

        auto rrr = rr.Value->right;
        Check(rrr, false, "f", false, false);
    }

    HW_TM_IMPLEMENTATION(ThenElse, ParserBaseStructure) {

        bool Trace = true;
        string text = "x then y else z";

        auto pt = PrioTable::CreateLeft({Any})
            .ThenElseLevel({"then"}, {"else"})
            .ParenthesisLevelLeft({Start}, {End})
            ;

        //HW_D_METHOD(pt);

        auto sc = ScannerInstance(text);
        auto syntax = Parse<Ref<Syntax>, TokenClass, HWLang::Token<TokenClass>>(pt, sc);
        Check(syntax, true, "else", true, true);

        auto rl = syntax.Value->left;
        Check(rl, true, "then", true, false);

        auto rr = syntax.Value->right;
        Check(rr, false, "z", false, false);

        auto rll = rl.Value->left;
        Check(rll, false, "x", false, false);

        auto rlr = rl.Value->right;
        Check(rlr, false, "y", false, false);
    }

    HW_TM_IMPLEMENTATION(TrainWreck, ParserBaseStructure) {

        bool Trace = true;
        string text = "x y z";

        auto pt = PrioTable()
            .Left({Any})
            .ParenthesisLevelLeft({Start}, {End})
            ;

        //HW_D_METHOD(pt);

        auto sc = ScannerInstance(text);
        auto syntax = Parse<Ref<Syntax>, TokenClass, HWLang::Token<TokenClass>>(pt, sc);
        Check(syntax, true, "z", false, false);

        auto rl = syntax.Value->left;
        Check(rl, true, "y", false, false);

        auto rll = rl.Value->left;
        Check(rll, false, "x", false, false);
    }

    void Check(Optional<Ref<Syntax>> const& target, bool isLeft, string const& part, bool isRight, bool isMatch) {
        HW_ASSERT(!target.IsEmpty, HW_D_VALUE(target));
        HW_ASSERT_IS(target.Value->name, == , part);
        HW_ASSERT(!target.Value->left.IsEmpty == isLeft, HW_D_VALUE(target));
        HW_ASSERT_IS(!target.Value->right.IsEmpty, == , isRight);
        HW_ASSERT_IS(target.Value->isMatch, == , isMatch);
    }
}

