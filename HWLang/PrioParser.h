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
        auto& tc = token.Class;

        if(!tc.AcceptsMatch(isMatch)
            || tc.HasLeft.IsValid && tc.HasLeft == left.IsEmpty
            || tc.HasRight.IsValid && tc.HasRight == right.IsEmpty)
            return tc.Mismatch(left, token.Part, right);

        if(!tc.HasLeft.IsValid){
            if(!tc.HasRight.IsValid)
                return tc.Anyfix(left, token.Part, right);
            if(tc.HasRight)
                return tc.Preanyfix(left, token.Part, right);
            return tc.NoPrefix(left, token.Part);
        }

        if(tc.HasLeft){
            if(!tc.HasRight.IsValid)
                return tc.Sufanyfix(left, token.Part, right);
            if(tc.HasRight)
                return tc.Infix(left, token.Part, right);
            return tc.Suffix(left, token.Part);
        }

        if(!tc.HasRight.IsValid)
            return tc.NoSuffix(token.Part, right);
        if(tc.HasRight)
            return tc.Prefix(token.Part, right);
        return tc.Terminal(token.Part);

    }
}