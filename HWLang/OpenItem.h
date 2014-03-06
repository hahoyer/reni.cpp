#pragma once

#include "PrioTable.h"
#include "PrioTableConst.h"

using namespace HWLib;

namespace HWLang{
    template <class Syntax, class Token>
    class OpenItem final{
        using thisType = OpenItem;

        CtrlPtr<Syntax> const left;
        Token const token;
        bool const isMatch;
    public:
        OpenItem() = delete;
        OpenItem(CtrlPtr<Syntax> left, Token const&token, bool isMatch)
            : left(left)
            , token(token)
            , isMatch(isMatch)
        {}

        DefaultAssignmentOperator;

        static OpenItem const StartItem(Token const&token){
            return OpenItem({}, token, false);
        };
        
        PrioTableConst::Tag const Relation(String const&newTokenName, PrioTable const&prioTable)const{ 
            return prioTable.Relation(newTokenName, token.Name); 
        };

        CtrlRef<Syntax> const CreateSyntax(CtrlPtr<Syntax> right)const{
            return HWLang::CreateSyntax(left, token, right, isMatch);
        }
    };
}