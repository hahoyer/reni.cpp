#pragma once

namespace HWLang
{
    namespace PrioTableConst{
        struct Tag{
            char const value;
            explicit Tag(char const value) : value(value){};
            bool operator==(Tag const& other)const{ return value == other.value; };
        };

        using TagTable = List<List<Tag const>const>;

        static char const* Any = "(any)";
        static char const* End = "(end)";
        static char const* Start = "(start)";
        static char const* Error = "(error)";

        Tag const LowerTag('-');
        Tag const HigherTag('+');
        Tag const MatchTag('=');
        Tag const UnknownTag(' ');

        static TagTable const ParenthesisTable =
        {
            { HigherTag, HigherTag, LowerTag },
            { HigherTag, UnknownTag, LowerTag },
            { UnknownTag, LowerTag, LowerTag }
        };

        static TagTable const ThenElseTable =
        {
            { HigherTag, LowerTag, LowerTag },
            { HigherTag, UnknownTag, HigherTag },
            { UnknownTag, LowerTag, HigherTag }
        };
    };
}

