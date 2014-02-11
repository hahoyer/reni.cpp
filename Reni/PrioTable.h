#pragma once

namespace Reni
{
    namespace PrioTableConst{
        static char const* Any = "(any)";
        static char const* EndOfText = "(eot)";
        static char const* BeginOfText = "(bot)";
        static char const* Error = "(err)";
        char const LeftTag = '-';
        char const RightTag = '+';
        char const MatchTag = '=';
        char const UnknownTag = ' ';
    };

    struct PrioTable final
    {
        using thisType = PrioTable;

        PrioTable()
            : data(AllocData(0, PrioTableConst::UnknownTag))
        {
        }

        PrioTable(PrioTable const&other)
            : tokens(other.tokens)
            , data(other.data)
        {
        };

    private:
        PrioTable(char tag, List<char const*> tokens)
            : tokens(tokens)
            , data(AllocData(tokens.end() - tokens.begin(), tag))
        {
        }

    public:
        DefaultAssignmentOperator;

        static PrioTable const Left(List<char const*> tokens){
            auto result = PrioTable(PrioTableConst::LeftTag, tokens);
            return result;
        }

        char const Relation(String newTokenName, String recentTokenName)const
        {
            if (newTokenName == PrioTableConst::BeginOfText || recentTokenName == PrioTableConst::EndOfText)
                return ' ';

            return data[Index(newTokenName)][Index(recentTokenName)];
        }
    private:
        Array<String const> const tokens;
        Array<Array<char>> const data;

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

        static Array<Array<char>> const AllocData(int count, char tag)
        {
            return Array<Array<char>>(count, [=](int)
            {
                return Array<char>(count, [=](int)
                {
                    return tag; 
                }); 
            });
        }

    };

}

