#pragma once

#include "PrioTable.h"
#include "PrioTableConst.h"

namespace HWLang{
    template <class Syntax, class Token>
    class OpenItem final{
        using thisType = OpenItem;

        OptRef<Syntax const> const left;
        Token const token;
    public:
        OpenItem() = delete;
        OpenItem(OptRef<Syntax const> left, Token token)
            : left(left)
            , token(token)
        {}

        DefaultAssignmentOperator;

        static OpenItem const StartItem(){ 
            return OpenItem<Syntax>(null, Token::Start);
        };
        
        PrioTableConst::Tag const Relation(String const&newTokenName, PrioTable const&prioTable)const{ 
            return prioTable.Relation(newTokenName, token.Name); 
        };

        Ref<Syntax const> const CreateSyntax(OptRef<Syntax const> right)const{
            if (left.IsValid)
                if (right.IsValid)
                    return token.Class.CreateSyntax(left, token.Part, right);
                else
                    return token.Class.CreateSyntax(left, token.Part);
            else
                if (right.IsValid)
                    return token.Class.CreateSyntax(token.Part, right);
                else
                    return token.Class.CreateSyntax(token.Part);
        }
    };
}