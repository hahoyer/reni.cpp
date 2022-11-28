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
        PrioTable(Tag tag, Array<string> const&tokens);
        PrioTable(
            Array<string>const& tokens,
            Array<Array<Tag>> const&base,
            TagTable const& subTable,
            size_t leftCount);
    public:
        HW_DO_PLACEMENT_ASSIGN;

        PrioTable Left(Array<string> const& tokens) const;
        PrioTable Right(Array<string> const& tokens) const;
        static PrioTable CreateLeft(Array<string> const& tokens);
        static PrioTable CreateRight(Array<string> const& tokens);
        PrioTable ParenthesisLevelRight(Array<string> leftToken, Array<string> rightToken) const;
        PrioTable ParenthesisLevelLeft(Array<string> leftToken, Array<string> rightToken) const;
        PrioTable ThenElseLevel(Array<string> leftToken, Array<string> rightToken) const;

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
        
        PrioTable Level(TagTable const& subTable, Array<string> const& leftToken,
                        Array<string> const& rightToken) const;
        Tag Relation(string const& newTokenName, string const& recentTokenName) const;
        HW_PR_DECL_GETTER(Array<string>,DumpData) override;

        static bool Trace;

    private:
        Array<string> const tokens{};
        Array<Array<Tag>> const data;

        HW_PR_GET(size_t, Count){ return tokens.Count; };

        size_t Index(string const& name) const;
        Tag Relation(size_t newIndex, size_t recentIndex) const;
        
        static Array<string> AllocTokens(Array<Array<string>> const& tokens);
        static Array<Array<Tag>> AllocData(size_t count, function<Tag(size_t, size_t)> getData);
        static Tag PrioChar(Array<Array<Tag>> const& base, TagTable const& subTable, size_t leftCount, size_t i,
                            size_t j);
        static size_t FindGroup(size_t i, Array<size_t>const&counts);
    };

}