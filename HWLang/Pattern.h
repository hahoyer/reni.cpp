#pragma once

#include "SourcePosition.h"

namespace HWLang
{
    class SourcePosition;
    class Pattern;
    using MatchResult = Optional<int> const;

    class IPattern
    {
    protected:
        IPattern(){};
    public:
        virtual MatchResult Match(SourcePosition const&position)const = 0;
        virtual ~IPattern(){};
    };

    class EndPattern final : public IPattern
    {
        virtual MatchResult Match(SourcePosition const&position)const override;
    };

    template<typename T>
    class Exception final
    {
    public:
        SourcePosition const Position;
        T const& Error;
        Exception(SourcePosition const position, T const& error)
            : Error(error)
            , Position(position)
        {}
    };

    class Pattern final
    {
        using thisType = Pattern;
        Ref<IPattern const> const _value;
    public:
        Pattern(IPattern const* value)
            : _value(value)
        {}

        virtual ~Pattern(){};

        DefaultAssignmentOperator;

        p(Pattern, Find);
        Pattern const Else(Pattern const& right)const;
        Pattern const Else(String right)const;
        Pattern const Repeat(int minCount = 0, Optional<int> maxCount = null)const;
        Pattern const Value(function<Pattern const(String)> func)const;

        Pattern const operator+(Pattern right)const;
        Pattern const operator+(String right)const;
        friend Pattern const operator+(String left, Pattern right);

        MatchResult Match(SourcePosition const&position)const{ return _value->Match(position); }
    };


    Pattern const Box(String);
    Pattern const Box(function<bool(char)>);

    template<typename T>
    Pattern const Error(T const&);

    Pattern const AnyChar(char const*);
    Pattern const Digit = Box([](char c){return !!::isdigit(c); });
    Pattern const Letter = Box([](char c){return !!::isalpha(c); });
    Pattern const WhiteSpace = Box([](char c){return !!::isspace(c); });
    Pattern const SourceEnd = new EndPattern;
    Pattern const LineEnd = AnyChar("\n\r").Else(SourceEnd);

    template<typename T>
    class ErrorMatch final : public IPattern
    {
        T const _value;
    public:
        ErrorMatch(T const&value) : _value(value) {}
    private:
        virtual MatchResult Match(SourcePosition const&position)const override
        {
            throw Exception<T>(position, _value);
        }
    };
}

using namespace HWLang;

template<typename T>
Pattern const HWLang::Error(T const&value)
{
    return new ErrorMatch<T>(value);
};
