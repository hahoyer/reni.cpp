#pragma once

#include "PrioTable.h"

namespace HWLang{
    template <class Syntax, class ScannerInstance>
    OptRef<Syntax const> const Parse(PrioTable const&prioTable, ScannerInstance&scanner){
        using TokenClass = Syntax::TokenClass;
        using Token = Token<TokenClass>;
        Stack<OpenItem<Syntax, Token>> stack;
        stack.Push(OpenItem<Syntax, Token>(null, scanner.Step()));

        do{
            auto item = scanner.Step();
            OptRef<Syntax const> result;
            do{
                auto topItem = stack.Top;
                auto relation = topItem.Relation(item.Name, prioTable);

                if (relation != PrioTableConst::HigherTag)
                    result = stack.Pop().CreateSyntax(result);

                if (relation == PrioTableConst::LowerTag)
                    continue;

                if (item.IsEnd){
                    a_if_(stack.IsEmpty);
                    return result;
                };
                stack.Push(OpenItem<Syntax, Token>(result, item));
                result = null;
            } while (result.IsValid);
        } while (true);
    };
}   