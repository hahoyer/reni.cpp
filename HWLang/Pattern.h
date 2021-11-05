#pragma once

#include "SourcePosition.h"

namespace HWLang
{
    class SourcePosition;
    class Pattern;
    using MatchResult = Optional<size_t> const;

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
    class Exception final : public exception
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
        CtrlRef<IPattern const> const _value;
    public:
        Pattern(IPattern const* value)
            : _value(value)
        {}

        virtual ~Pattern(){};

        HW_DO_PLACEMENT_ASSIGN;

        p(Pattern, Find);
        Pattern Else(Pattern const& right) const;
        Pattern Else(std::string const& right) const;
        Pattern const Repeat(size_t minCount = 0, Optional<size_t> maxCount = {})const;
        Pattern const Value(function<Pattern const(std::string)> func)const;

        Pattern operator+(Pattern const& right)const;
        Pattern operator+(std::string const& right) const;
        friend Pattern operator+(std::string const& left, Pattern const& right);

        MatchResult Match(SourcePosition const&position)const{ return _value->Match(position); }
    };


    Pattern Box(std::string);
    Pattern Box(function<bool(char)>);

    template<typename T>
    Pattern const Error(T const&);

    Pattern AnyChar(char const*);
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
