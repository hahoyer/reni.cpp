#pragma once

#include "PrioTable.h"
#include "OpenItem.h"
#include "../HWLib/Stack.h"

namespace HWLang{
    template <class TSyntax, class TSyntaxOption, class TTokenClass, class TToken, class TScannerInstance>
    TSyntaxOption const Parse(PrioTable const&prioTable, TScannerInstance&scanner){
        typedef OpenItem<TSyntax, TSyntaxOption, TToken> OpenItem;
        Stack<OpenItem> stack;
        stack.Push(OpenItem::StartItem(scanner.Step()));

        do{
            auto item = scanner.Step();
            TSyntaxOption result;
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
                    stack.Push(OpenItem(result, item, relation == PrioTableConst::MatchTag));
                    result = {};
                }
            } while (result.IsValid);
        } while (true);
    };

    template <class TSyntax, class TSyntaxOption, class TToken>
    TSyntax const CreateSyntax(TSyntaxOption const&left, TToken const&token, TSyntaxOption const&right, bool isMatch){
        if (left.IsValid)
            if (right.IsValid)
            {
                TSyntax xl = left;
                TSyntax xr = right;

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