#include "Import.h"
#include "ReniScanner.h"

static bool Trace = true;

using namespace Reni;

Optional<int> const ExceptionGuard(SourcePosition const&position, Pattern pattern)
{
    try
    {
        return pattern.Match(position);
    }
    catch (Exception<String> exception)
    {
        throw ReniScanner::Error(exception.Position - position, exception.Error);
    }
};


struct ReniScanner::internal
{
    Pattern const _any;
    Pattern const _whiteSpaces;
    Pattern const _number;
    Pattern const _text;
    
    static internal const*const Create()
    {
        auto invalidLineComment = Match::Error("EOFInLineComment");
        auto invalidComment = Match::Error("EOFInComment");
        auto invalidTextEnd = Match::Error("invalidTextEnd");
        auto alpha = Letter.Else("_");
        auto symbol1 = AnyChar("({[)}];,");
        auto textFrame = AnyChar("'\"");
        auto symbol = AnyChar("°^!²§³$%&/=?\\@€*+~><|:.-");

        auto identifier =
            (alpha + (alpha.Else(Digit)).Repeat())
            .Else(symbol.Repeat(1));

        auto any = symbol1.Else(identifier);

        auto whiteSpaces = Match::WhiteSpace
            .Else("#" + AnyChar(" \t") + LineEnd.Find)
            .Else("#(" + Match::WhiteSpace + (Match::WhiteSpace + ")#").Find)
            .Else("#(" + any.Value([](String id){return (Match::WhiteSpace + id + ")#").Find; }))
            .Else("#(" + End.Find + invalidComment)
            .Else("#" + End.Find + invalidLineComment)
            .Repeat();

        auto number = Digit.Repeat(1);

        auto text = textFrame
            .Value
            ([=](String head)
        {
            auto textEnd = Box(head).Else(LineEnd + invalidTextEnd);
            return textEnd.Find + (head + textEnd.Find).Repeat();
        });

        return new internal(any, whiteSpaces, number, text);
    };

    internal(
        Pattern const &any,
        Pattern const &whiteSpaces,
        Pattern const &number,
        Pattern const &text
        )

        : _any(any)
        , _whiteSpaces(whiteSpaces)
        , _number(number)
        , _text(text)
    {
    };
};

ReniScanner::ReniScanner()
: _internal(internal::Create())
{};

static ReniScanner instance;

int const ReniScanner::WhiteSpace(SourcePosition const&position)
{
    auto result = ExceptionGuard(position, instance._internal->_whiteSpaces);
    assert(result.IsValid);
    return result;
};


Optional<int> const ReniScanner::Number(SourcePosition const&position) { return ExceptionGuard(position, instance._internal->_number); };
Optional<int> const ReniScanner::Any(SourcePosition const&position) { return ExceptionGuard(position, instance._internal->_any); };
Optional<int> const ReniScanner::Text(SourcePosition const&position) { return ExceptionGuard(position, instance._internal->_text); };

