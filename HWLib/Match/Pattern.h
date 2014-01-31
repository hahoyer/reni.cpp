#pragma once

#include "../Ref.h"
#include "SourcePosition.h"

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
            virtual r Match(SourcePosition const&position)const override;
        };

        template<typename T>
        class Exception
        {
        public:
            SourcePosition const Position;
            T const& Error;
            Exception(SourcePosition const position, T const& error)
                : Error(error)
                , Position(position)
            {}
        };

        class PatternContainer
        {
            using thisType = PatternContainer;
            Ref<Pattern const> const _value;
        public:
            PatternContainer(Pattern const* value)
                : _value(value)
            {}

            DefaultAssignmentOperator;

            p(pr, Find);
            pr Else(PatternContainer const& right)const;
            pr Else(String right)const;
            pr Repeat(int minCount= 0, Optional<int> maxCount = null)const;
            pr Value(function<pr(String)> func)const;

            pr operator+(PatternContainer right)const;
            pr operator+(String right)const;

            template<typename T>
            pr operator+(T const& right)const;
            friend pr operator+(String left, PatternContainer right);

            r Match(SourcePosition const&position)const{ return _value->Match(position); }
        };


        pr Box(String);
        pr Box(function<bool(char)>);

        template<typename T>
        pr Error(T const&);
        
        pr AnyChar(char const*);
        pr Digit = Box([](char c){return !!::isdigit(c); });
        pr Letter = Box([](char c){return !!::isalpha(c); });
        pr WhiteSpace = Box([](char c){return !!::isspace(c); });
        pr End = new EndPattern;
        pr LineEnd = AnyChar("\n\r").Else(End);

        template<typename T>
        class ErrorMatch : public Pattern
        {
            T const& _value;
        public:
            ErrorMatch(T const&value) : _value(value) {}
        private:
            virtual r Match(SourcePosition const&position)const override
            {
                throw Exception<T>(position, _value);
            }
        };


    }
}

using namespace HWLib;
using namespace Match;

template<typename T>
pr PatternContainer::operator+(T const& right)const
{ 
    return *this + Error(right);
};

template<typename T>
pr HWLib::Match::Error(T const&value)
{ 
    return new ErrorMatch<T>(value); 
};
