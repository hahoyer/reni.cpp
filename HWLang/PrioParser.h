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
                if (item.IsEnd && relation == PrioTableConst::MatchTag){
                    stack.Pop();
                    a_if_(stack.IsEmpty);
                    return result;
                }

                if (relation != PrioTableConst::HigherTag)
                    result = stack.Pop().CreateSyntax(result);

                if (relation != PrioTableConst::LowerTag){
                    stack.Push(OpenItem(result, item, relation == PrioTableConst::MatchTag));
                    result = {};
                }
            } while (!result.IsEmpty);
        } while (true);
    };

    template <class TSyntax, class TSyntaxOption, class TToken>
    TSyntax const CreateSyntax(TSyntaxOption const&left, TToken const&token, TSyntaxOption const&right, bool isMatch){
        if(!token.Class.AcceptsMatch(isMatch))
            return token.Class.Mismatch(left, token.Part, right);
        if(left.IsEmpty){
            if(right.IsEmpty)
                return token.Class.CreateSyntax(token.Part);
            else
                return token.Class.CreateSyntax(token.Part, right);
        }
        else{
            if(right.IsEmpty)
                return token.Class.CreateSyntax(left, token.Part);
            else{
                TSyntax xl = left;
                TSyntax xr = right;
                return token.Class.CreateSyntax(xl, token.Part, xr);
            }
        }
    }
}