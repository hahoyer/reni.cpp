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
    public:
        OpenItem() = delete;

        OpenItem(Optional<TSyntax> left, TToken const& token)
            : left(left)
            , token(token)
        {
        }

        HW_DO_PLACEMENT_ASSIGN;

        static OpenItem StartItem(TToken const& token)
        {
            return OpenItem({}, token);
        };

        PrioTableConst::Tag Relation(string const& newTokenName, PrioTable const& prioTable) const
        {
            return prioTable.Relation(newTokenName, token.Name);
        };

        TSyntax CreateSyntax(Optional<TSyntax> const& right) const
        {
            return HWLang::CreateSyntax<TSyntax>(left, token, right);
        }
    };
}