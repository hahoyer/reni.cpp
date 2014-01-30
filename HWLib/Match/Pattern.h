#pragma once

namespace HWLib
{
    namespace Match
    {
        class SourcePosition;
        class PatternContainer;
        using r = Optional<int> const;
        using pr = PatternContainer const;

        class Pattern
        {
        public:
            virtual r Match(SourcePosition const&position)const = 0;
        };

        class EndPattern : public Pattern
        {
        public:
            virtual r Match(SourcePosition const&position)const override;
        };

        class Error : public Pattern
        {
        public:
            virtual r Match(SourcePosition const&position)const override;
        };

        class PatternContainer
        {
            using thisType = PatternContainer;
            Ref<Pattern const> const _data;
        public:
            PatternContainer(Pattern const* data)
                : _data(data)
            {}

            DefaultAssignmentOperator;

            p(pr, Find);
            pr Else(PatternContainer const& right)const;
            pr Else(String right)const;
            pr Repeat(int minCount= 0, Optional<int> maxCount = null)const;
            pr Value(function<pr(String)> func)const;

            friend pr operator+(PatternContainer left, PatternContainer right);
            friend pr operator+(PatternContainer left, String right);
            friend pr operator+(String left, PatternContainer right);
            friend pr operator+(PatternContainer left, Error const& right);

            r Match(SourcePosition const&position)const;
        };


        pr Box(String);
        pr Box(function<r(char)>);
        pr AnyChar(char const*);
        pr Digit = Box([](char c){return ::isdigit(c); });
        pr Letter = Box([](char c){return ::isalpha(c); });
        pr WhiteSpace = Box([](char c){return ::isspace(c); });
        pr End = new EndPattern;
        pr LineEnd = AnyChar("\n\r").Else(End);

    }
}