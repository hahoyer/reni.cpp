#pragma once

#include "PrioTableConst.h"

namespace HWLang
{
    using namespace PrioTableConst;

    class PrioTable final : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = PrioTable;

    public:
        PrioTable();
        PrioTable(PrioTable const&other);

    private:
        PrioTable(PrioTableConst::Tag tag, Array<String const> const&tokens);
        PrioTable(
            Array<String const>const& tokens,
            Array<Array<PrioTableConst::Tag const>const> const&base,
            PrioTableConst::TagTable const& subTable,
            int leftCount);
    public:
        DefaultAssignmentOperator;

        PrioTable const Left(Array<String const> const& tokens)const;
        PrioTable const Right(Array<String const> const& tokens)const;
        static PrioTable const CreateLeft(Array<String const> const& tokens);
        static PrioTable const CreateRight(Array<String const> const& tokens);
        PrioTable const ParenthesisLevel(char const* leftToken, char const* rightToken)const;
        PrioTable const ParenthesisLevel(Array<String const> leftToken, Array<String const> rightToken)const;
        
        /// <summary>
        ///     Define a prio table that adds a parenthesis level.
        ///     LToken and RToken should have the same number of elements.
        ///     Elements of these lists that have the same index are considered as matching
        /// </summary>
        /// <param name="data">
        ///     contains a 3 by 3 character table.
        ///     The characters have the following meaning:
        ///     0,0: left TokenClass finds left TokenClass;
        ///     0,1: left TokenClass finds TokenClass defined so far;
        ///     0,2: left TokenClass finds right TokenClass;
        ///     1,0: TokenClass defined so far finds left TokenClass;
        ///     1,1: ignored, Table for already defined Tokens used (use '?');
        ///     1,2: TokenClass defined so far finds right TokenClass;
        ///     2,0: ignored, "-=+"-Table generated (use '?');
        ///     2,1: right TokenClass finds TokenClass defined so far;
        ///     2,2: right TokenClass finds right TokenClass
        /// </param>
        /// <param name="lToken"> list of strings that play the role of left parenthesis </param>
        /// <param name="rToken"> list of strings that play the role of right parenthesis </param>
        /// <returns> </returns>
        PrioTable const Level(PrioTableConst::TagTable const& subTable, Array<String const> const&leftToken, Array<String const>const&rightToken)const;
        PrioTableConst::Tag const Relation(String const&newTokenName, String const&recentTokenName)const;
        override_p_function(Array<String>, DumpData);

        static bool Trace;

    private:
        Array<String const> const tokens;
        Array<Array<PrioTableConst::Tag const>const> const data;

        p(int, Count){ return tokens.Count; };

        int const Index(String const&name)const;
        PrioTableConst::Tag const Relation(int newIndex, int recentIndex)const;
        
        static Array<String const> const AllocTokens(initializer_list<Array<String const>> const &tokens);
        static Array<Array<PrioTableConst::Tag const>const> const AllocData(int count, function<PrioTableConst::Tag(int, int)> getData);
        static PrioTableConst::Tag const PrioChar(Array<Array<PrioTableConst::Tag const>const> const&base, PrioTableConst::TagTable const& subTable, int leftCount, int i, int j);
        static int FindGroup(int i, Array<int>const&counts);
    };

}

namespace HWLib{
    template<>
    struct Ref<HWLang::PrioTable const>::traits
        : public default_ref_traits<HWLang::PrioTable const>{
        static bool const EnableSetDumpString = true;
    };

}

