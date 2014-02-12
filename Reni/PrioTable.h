#pragma once

namespace Reni
{
    namespace PrioTableConst{
        struct Tag{
            char const value;
            explicit Tag(char const value) : value(value){};
            bool operator==(Tag const& other)const{ return value == other.value; };
        };

        using TagTable = List<List<Tag const>const>;

        static char const* Any = "(any)";
        static char const* EndOfText = "(eot)";
        static char const* BeginOfText = "(bot)";
        static char const* Error = "(err)";

        Tag const LeftTag('-');
        Tag const RightTag('+');
        Tag const MatchTag('=');
        Tag const UnknownTag(' ');

        static TagTable const ParenthesisTable =
        { 
            { RightTag, RightTag,   LeftTag }, 
            { RightTag,  UnknownTag, LeftTag }, 
            { UnknownTag, LeftTag,  LeftTag }
        };

        static TagTable const ThenElseTable = 
        {
            { RightTag, LeftTag,    LeftTag  },
            { RightTag,  UnknownTag, RightTag },
            { UnknownTag, LeftTag,  RightTag }
        };
    };

    class PrioTable final
    {
        using thisType = PrioTable;

    public:
        PrioTable()
        {
        }

        PrioTable(PrioTable const&other)
            : tokens(other.tokens)
            , data(other.data)
        {
        };

    private:
        PrioTable(PrioTableConst::Tag tag, List<String const> tokens)
            : tokens(tokens)
            , data(AllocData(tokens.size(), [=](int,int){return tag; }))
        {
        }

        PrioTable(Array<String const>const& tokens, PrioTable const&base, PrioTableConst::TagTable const& subTable, int leftCount)
            : tokens(tokens)
            , data(AllocData(tokens.Count, [=](int i, int j){return PrioChar(base, subTable, leftCount, i, j); }))
        {
        };
    public:
        DefaultAssignmentOperator;

        static PrioTable const Left(List<String const> const& tokens){
            return PrioTable(PrioTableConst::LeftTag, tokens);
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

        PrioTableConst::Tag const Relation(String newTokenName, String recentTokenName)const
        {
            if (newTokenName == PrioTableConst::BeginOfText || recentTokenName == PrioTableConst::EndOfText)
                return PrioTableConst::UnknownTag;

            return data[Index(newTokenName)][Index(recentTokenName)];
        }
    private:
        Array<String const> const tokens;
        Array<Array<PrioTableConst::Tag const>const> const data;

        p(int, Count){ return tokens.Count; };

        int const Index(String name)const
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
                    return PrioTableConst::LeftTag;
                case 0:
                    return PrioTableConst::MatchTag;
                case 1:
                    return PrioTableConst::RightTag;
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

