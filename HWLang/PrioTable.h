#pragma once

#include "PrioTableConst.h"

namespace HWLang
{
    class PrioTable final : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = PrioTable;

    public:
        PrioTable()
        {
            SetDumpString();
        }

        PrioTable(PrioTable const&other)
            : tokens(other.tokens)
            , data(other.data)
        {
            SetDumpString();
        };

    private:
        PrioTable(PrioTableConst::Tag tag, List<String const> tokens)
            : tokens(tokens)
            , data(AllocData(tokens.size(), [=](int,int){return tag; }))
        {
            SetDumpString();
        }

        PrioTable(Array<String const>const& tokens, PrioTable const&base, PrioTableConst::TagTable const& subTable, int leftCount)
            : tokens(tokens)
            , data(AllocData(tokens.Count, [=](int i, int j){return PrioChar(base, subTable, leftCount, i, j); }))
        {
            SetDumpString();
        };
    public:
        DefaultAssignmentOperator;

        static PrioTable const Left(List<String const> const& tokens){
            return PrioTable(PrioTableConst::LowerTag, tokens);
        }

        PrioTable const ParenthesisLevel(char const* leftToken, char const* rightToken){
            return Level(PrioTableConst::ParenthesisTable, { leftToken }, { rightToken });
        }

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
        PrioTable const Level(PrioTableConst::TagTable const& subTable, List<String const> const&leftToken, List<String const>const&rightToken) {
            return PrioTable(AllocTokens(leftToken, tokens, rightToken), *this, subTable, leftToken.size());
        }

        PrioTableConst::Tag const Relation(String const&newTokenName, String const&recentTokenName)const
        {
            if (newTokenName == PrioTableConst::Start || recentTokenName == PrioTableConst::End)
                return PrioTableConst::UnknownTag;

            return data[Index(newTokenName)][Index(recentTokenName)];
        }

        p_function(Array<String>, DumpData)override
        {
            auto maxlen = *tokens.Select<int>([](String const&t){return t.Count; })->Max();
            auto head0 = String().CastLeft(maxlen);
            head0 += "    ";
            auto head1 = head0;
            String result;
            for (auto i = 0; i < Count; i++)
            {
                auto ii = HWLib::Dump(i + 10000);
                head0 += ii[3];
                head1 += ii[4];
                result += tokens[i].CastLeft(maxlen) + " " + ii.Part(3, 2) + " ";
                for (auto j = 0; j < Count; j++)
                    result += String(data[i][j].value);
                result += "\n";
            }
            return{ "\n" + head0 + "\n" + head1 + "\n" + result + "\n" };
        }

    private:
        Array<String const> const tokens;
        Array<Array<PrioTableConst::Tag const>const> const data;

        p(int, Count){ return tokens.Count; };

        int const Index(String const&name)const
        {
            for (auto i = 0; i < Count; i++)
                if (tokens[i] == name)
                    return (i);

            for (auto i = 0; i < Count; i++)
                if (tokens[i] == PrioTableConst::Any)
                    return (i);

            throw String("missing ") + PrioTableConst::Any + " entry in priority table";

        }


        static Array<String const> const AllocTokens(List<String const>const&left, Array<String const> const &tokens, List<String const>const&right)
        {
            return Array<Array<String const>const>{left, tokens, right}
            .ConvertMany<String const>()
                ->ToArray;
        }

        static Array<Array<PrioTableConst::Tag const>const> const AllocData(int count, function<PrioTableConst::Tag(int, int)> getData)
        {
            return Array<Array<PrioTableConst::Tag const>const>(count, [=](int i)
            {
                return Array<PrioTableConst::Tag const>(count, [=](int j)
                {
                    return getData(i,j);
                });
            });
        }

        static PrioTableConst::Tag const PrioChar(PrioTable const&base, PrioTableConst::TagTable const& subTable, int leftCount, int i, int j)
        {
            auto baseCount = base.Count;
            auto iGroup = FindGroup(i, { leftCount, baseCount });
            auto jGroup = FindGroup(j, { leftCount, baseCount });

            if (iGroup == 1 && jGroup == 1)
                return base.data[i - leftCount][j - leftCount];

            if (iGroup == 2 && jGroup == 0)
            {
                switch (sign(leftCount + baseCount - i + j))
                {
                case -1:
                    return PrioTableConst::LowerTag;
                case 0:
                    return PrioTableConst::MatchTag;
                case 1:
                    return PrioTableConst::HigherTag;
                default:
                    throw "Unexpected sign";
                }
            }
            return (subTable.begin()[iGroup]).begin()[jGroup];
        };

        static int FindGroup(int i, Array<int>const&counts)
        {
            auto result = 0;
            for (auto count : counts)
            {
                i -= count;
                if (i < 0)
                    return result;
                result++;
            };
            return result;
        }
    };

}

namespace HWLib{
    template<>
    struct Ref<HWLang::PrioTable const>::traits
        : public default_ref_traits<HWLang::PrioTable const>{
        static bool const EnableSetDumpString = true;
    };

}

