#pragma once

#include "PrioTableConst.h"
#include "../HWLib/Array.h"
#include "../HWLib/DumpableObject.h"

using namespace HWLib;

namespace HWLang
{
    using namespace PrioTableConst;

    class PrioTable final : public DumpableObject
    {
        typedef DumpableObject baseType;
        typedef PrioTable thisType;

    public:
        class MissingFallbackEntryException : public std::exception{};
        class UnexpectedSignException : public std::exception{};

        PrioTable();
        PrioTable(PrioTable const&other);

    private:
        PrioTable(Tag tag, Array<String> const&tokens);
        PrioTable(
            Array<String>const& tokens,
            Array<Array<Tag>> const&base,
            TagTable const& subTable,
            int leftCount);
    public:
        DefaultAssignmentOperator;

        PrioTable const Left(Array<String> const& tokens)const;
        PrioTable const Right(Array<String> const& tokens)const;
        static PrioTable const CreateLeft(Array<String> const& tokens);
        static PrioTable const CreateRight(Array<String> const& tokens);
        PrioTable const ParenthesisLevelRight(Array<String> leftToken, Array<String> rightToken)const;
        PrioTable const ParenthesisLevelLeft(Array<String> leftToken, Array<String> rightToken)const;
        PrioTable const ThenElseLevel(Array<String> leftToken, Array<String> rightToken)const;

        // <summary>
        //     Define a prio table that adds a parenthesis level.
        //     LToken and RToken should have the same number of elements.
        //     Elements of these lists that have the same index are considered as matching
        // </summary>
        // <param name="data">
        //     contains a 3 by 3 character table.
        //     The characters have the following meaning:
        //     0,0: left TokenClass finds left TokenClass;
        //     0,1: left TokenClass finds TokenClass defined so far;
        //     0,2: left TokenClass finds right TokenClass;
        //     1,0: TokenClass defined so far finds left TokenClass;
        //     1,1: ignored, Table for already defined Tokens used (use '?');
        //     1,2: TokenClass defined so far finds right TokenClass;
        //     2,0: ignored, "-=+"-Table generated (use '?');
        //     2,1: right TokenClass finds TokenClass defined so far;
        //     2,2: right TokenClass finds right TokenClass
        // </param>
        // <param name="lToken"> list of strings that play the role of left parenthesis </param>
        // <param name="rToken"> list of strings that play the role of right parenthesis </param>
        // <returns> </returns>
        
        PrioTable const Level(TagTable const& subTable, Array<String> const&leftToken, Array<String>const&rightToken)const;
        Tag const Relation(String const&newTokenName, String const&recentTokenName)const;
        p_function(Array<String>,DumpData) override;

        static bool Trace;

    private:
        Array<String> const tokens;
        Array<Array<Tag>> const data;

        p(int, Count){ return tokens.Count; };

        int const Index(String const&name)const;
        Tag const Relation(int newIndex, int recentIndex)const;
        
        static Array<String>const AllocTokens(Array<Array<String>> const &tokens);
        static Array<Array<Tag>> const AllocData(int count, function<Tag(int, int)> getData);
        static Tag const PrioChar(Array<Array<Tag>> const&base, TagTable const& subTable, int leftCount, int i, int j);
        static int FindGroup(int i, Array<int>const&counts);
    };

}