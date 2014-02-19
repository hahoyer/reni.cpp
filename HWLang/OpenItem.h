#pragma once

#include "PrioTable.h"
#include "PrioTableConst.h"

namespace HWLang{
    template <class Syntax, class Token>
    class OpenItem final{
        using thisType = OpenItem;

        OptRef<Syntax> const left;
        Token const token;
        bool const isMatch;
    public:
        OpenItem() = delete;
        OpenItem(OptRef<Syntax> left, Token const&token, bool isMatch)
            : left(left)
            , token(token)
            , isMatch(isMatch)
        {}

        DefaultAssignmentOperator;

        static OpenItem const StartItem(Token const&token){
            return OpenItem(null, token, false);
        };
        
        PrioTableConst::Tag const Relation(String const&newTokenName, PrioTable const&prioTable)const{ 
            return prioTable.Relation(newTokenName, token.Name); 
        };

        Ref<Syntax> const CreateSyntax(OptRef<Syntax> right)const{
            if (left.IsValid)
                if (right.IsValid)
                    return token.Class.CreateSyntax(left, token.Part, right, isMatch);
                else
                    return token.Class.CreateSyntax(left, token.Part, isMatch);
            else
                if (right.IsValid)
                    return token.Class.CreateSyntax(token.Part, right, isMatch);
                else
                    return token.Class.CreateSyntax(token.Part, isMatch);
        }
    };
}