#pragma once

#include "../Ref.h"
#include "SourcePosition.h"

namespace HWLib
{
    namespace Match
    {
        class SourcePosition;
        class Pattern;
        using r = Optional<int> const;
        using pr = Pattern const;

        class IPattern
        {
        public:
            virtual ~IPattern(){};
            virtual r Match(SourcePosition const&position)const = 0;
        };

        class EndPattern : public IPattern
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

        class Pattern
        {
            using thisType = Pattern;
            Ref<IPattern const> const _value;
        public:
            Pattern(IPattern const* value)
                : _value(value)
            {}
            
            virtual ~Pattern(){};

            DefaultAssignmentOperator;

            p(pr, Find);
            pr Else(Pattern const& right)const;
            pr Else(String right)const;
            pr Repeat(int minCount= 0, Optional<int> maxCount = null)const;
            pr Value(function<pr(String)> func)const;

            pr operator+(Pattern right)const;
            pr operator+(String right)const;
            friend pr operator+(String left, Pattern right);

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
        class ErrorMatch : public IPattern 
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
pr HWLib::Match::Error(T const&value)
{
    return new ErrorMatch<T>(value); 
};
