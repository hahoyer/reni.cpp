#pragma once

#include "PrioTable.h"
#include "OpenItem.h"
#include "../HWLib/Stack.h"

namespace HWLang{
    template <class Syntax, class TokenClass, class Token, class ScannerInstance>
    CtrlPtr<Syntax> const Parse(PrioTable const&prioTable, ScannerInstance&scanner){
        Stack<OpenItem<Syntax, Token>> stack;
        stack.Push(OpenItem<Syntax, Token>::StartItem(scanner.Step()));

        do{
            auto item = scanner.Step();
            CtrlPtr<Syntax> result;
            do{
                auto topItem = stack.Top;
                auto relation = topItem.Relation(item.Name, prioTable);
                if (item.IsEnd && relation == PrioTableConst::MatchTag)
                {
                    stack.Pop();
                    a_if_(stack.IsEmpty);
                    return result;
                }
 
                if (relation != PrioTableConst::HigherTag)
                    result = stack.Pop().CreateSyntax(result);

                if (relation != PrioTableConst::LowerTag)
                {
                    stack.Push(OpenItem<Syntax, Token>(result, item, relation == PrioTableConst::MatchTag));
                    result = {};
                }
            } while (result.IsValid);
        } while (true);
    };

    template <class Syntax, class Token>
    CtrlRef<Syntax> const CreateSyntax(CtrlPtr<Syntax> const&left, Token const&token, CtrlPtr<Syntax> const&right, bool isMatch){
        if (left.IsValid)
            if (right.IsValid)
            {
                CtrlRef<Syntax> xl = left;
                CtrlRef<Syntax> xr = right;

                return token.Class.CreateSyntax(xl, token.Part, xr, isMatch);
            }
            else
                return token.Class.CreateSyntax(left, token.Part, isMatch);
        else
            if (right.IsValid)
                return token.Class.CreateSyntax(token.Part, right, isMatch);
            else
                return token.Class.CreateSyntax(token.Part, isMatch);
    }

}