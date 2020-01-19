#pragma once
#include <initializer_list>
#include "../HWLib/String.h"

namespace HWLang
{
    namespace PrioTableConst{
        struct Tag{
            char const value;
            explicit Tag(char const value) : value(value){};
            bool operator==(Tag const& other)const{ return value == other.value; };
            p(String, Dump){
                switch (value)
                {
                case '+': return "HigherTag";
                case '-': return "LowerTag";
                case ' ': return "UnknownTag";
                }
                return String(value).Quote;
            }
            p(bool, IsValid){
                switch (value)
                {
                case '+':
                case '-':
                case ' ':
                    return true;
                }
                return false;
            };
        };

        typedef std::initializer_list<initializer_list<Tag const> const> TagTable;

        static char const* Any = "(any)";
        static char const* End = "(end)";
        static char const* Start = "(start)";
        static char const* Error = "(error)";

        Tag const HigherTag('+');
        Tag const LowerTag('-');
        Tag const UnknownTag(' ');

        static TagTable const ParenthesisTableLeft =
        {
            { HigherTag, HigherTag, LowerTag },
            { HigherTag, UnknownTag, LowerTag },
            { UnknownTag, LowerTag, LowerTag }
        };

        static TagTable const ParenthesisTableRight =
        {
            {HigherTag, HigherTag, HigherTag},
            {HigherTag, UnknownTag, LowerTag},
            {UnknownTag, LowerTag, LowerTag}
        };

        static TagTable const ThenElseTable =
        {
            { HigherTag, LowerTag, LowerTag },
            { HigherTag, UnknownTag, HigherTag },
            { UnknownTag, LowerTag, HigherTag }
        };

        static TagTable const LeftTable =
        {
            { UnknownTag, UnknownTag, UnknownTag },
            { UnknownTag, UnknownTag, HigherTag },
            { UnknownTag, LowerTag , LowerTag  }
        };

        static TagTable const RightTable =
        {
            { UnknownTag, UnknownTag, UnknownTag },
            { UnknownTag, UnknownTag, HigherTag },
            { UnknownTag, LowerTag , HigherTag }
        };
    };

};

namespace HWLib
{
    inline String const Dump(Array<Array<HWLang::PrioTableConst::Tag const>const> target);
    inline String const Dump(HWLang::PrioTableConst::TagTable const&target);

    inline String const Dump(HWLang::PrioTableConst::Tag target){
        return target.Dump;
    };

    template <class TResult>
    TResult const* const DynamicConvert(HWLang::PrioTableConst::Tag const&){
        return null;
    };

    template <class TResult>
    TResult const* const DynamicConvert(HWLang::PrioTableConst::TagTable const&){
        return null;
    };
}
