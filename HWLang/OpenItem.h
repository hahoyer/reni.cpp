#pragma once

#include "PrioTable.h"
#include "PrioTableConst.h"
#include "PrioParser.h"

using namespace HWLib;

namespace HWLang{
    template <class TSyntax, class TSyntaxOption, class TToken>
    class OpenItem final{
        using thisType = OpenItem;

        TSyntaxOption const left;
        TToken const token;
        bool const isMatch;
    public:
        OpenItem() = delete;
        OpenItem(TSyntaxOption left, TToken const&token, bool isMatch)
            : left(left)
            , token(token)
            , isMatch(isMatch)
        {}

        DefaultAssignmentOperator;

        static OpenItem const StartItem(TToken const&token){
            return OpenItem({}, token, false);
        };
        
        PrioTableConst::Tag const Relation(String const&newTokenName, PrioTable const&prioTable)const{ 
            return prioTable.Relation(newTokenName, token.Name); 
        };

        TSyntax const CreateSyntax(TSyntaxOption const& right)const{
            return HWLang::CreateSyntax<TSyntax>(left, token, right, isMatch);
        }
    };
}