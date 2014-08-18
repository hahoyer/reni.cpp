#pragma once

#include "PrioTable.h"
#include "OpenItem.h"
#include "../HWLib/Stack.h"

namespace HWLang
{
    template <class TSyntax, class TTokenClass, class TToken, class TScannerInstance>
    Optional<TSyntax> const Parse(PrioTable const& prioTable, TScannerInstance& scanner)
    {
        typedef OpenItem<TSyntax, TToken> OpenItem;
        Stack<OpenItem> stack;
        stack.Push(OpenItem::StartItem(scanner.Step()));

        do
        {
            auto item = scanner.Step();
            Optional<TSyntax> result;
            do
            {
                auto topItem = stack.Top;
                auto relation = topItem.Relation(item.Name, prioTable);
                if (item.IsEnd && relation == MatchTag)
                {
                    stack.Pop();
                    a_if_(stack.IsEmpty);
                    return result;
                }

                if (relation != HigherTag)
                    result = stack.Pop().CreateSyntax(result);

                if (relation != LowerTag)
                {
                    stack.Push(OpenItem(result, item, relation == MatchTag));
                    result ={};
                }
            }
            while (!result.IsEmpty);
        }
        while (true);
    };

    template <class TSyntax, class TToken>
    TSyntax const CreateSyntax(Optional<TSyntax> const& left, TToken const& token, Optional<TSyntax> const& right, bool isMatch)
    {
        auto& tc = token.Class;
        if (tc.AcceptsMatch == isMatch)
            return tc.CreateSyntax(left, token.Part, right);
        return tc.Mismatch(left, token.Part, right);
    }
}