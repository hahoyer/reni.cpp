#pragma once

#include "PrioTable.h"
#include "PrioTableConst.h"
#include "PrioParser.h"

using namespace HWLib;

namespace HWLang
{
    template <class TSyntax, class TToken>
    class OpenItem final
    {
        using thisType = OpenItem;

        Optional<TSyntax> const left;
        TToken const token;
        bool const isMatch;
    public:
        OpenItem() = delete;

        OpenItem(Optional<TSyntax> left, TToken const& token, bool isMatch)
            : left(left)
            , token(token)
            , isMatch(isMatch)
        {
        }

        DefaultAssignmentOperator;

        static OpenItem const StartItem(TToken const& token)
        {
            return OpenItem({}, token, false);
        };

        PrioTableConst::Tag const Relation(String const& newTokenName, PrioTable const& prioTable)const
        {
            return prioTable.Relation(newTokenName, token.Name);
        };

        TSyntax const CreateSyntax(Optional<TSyntax> const& right)const
        {
            return HWLang::CreateSyntax<TSyntax>(left, token, right, isMatch);
        }
    };
}