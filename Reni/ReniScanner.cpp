#include "Import.h"
#include "ReniScanner.h"

static bool Trace = true;

using namespace Reni;

class MatchException
{
public:
    MatchException(SourcePosition const &position, String const& id)
        : Position(position)
        , Id(id)
    {}

    SourcePosition const Position;
    String const Id;
};


String const ConvertErrorId(String const & matchId)
{
    fdumpc(matchId);
    bp;
    returndump(matchId);
};

Optional<int> const ExceptionGuard(SourcePosition const&position, PatternContainer pattern)
{
    try
    {
        return pattern.Match(position);
    }
    catch (MatchException exception)
    {
        throw ReniScanner::Error(exception.Position - position, ConvertErrorId(exception.Id));
    }
};

class SyntaxError : public Error
{
public:
    SyntaxError(String id);
};


class ReniScanner::internal
{
public:
    SyntaxError const _invalidLineComment;
    SyntaxError const _invalidComment;
    SyntaxError const _invalidTextEnd;
    PatternContainer const _any;
    PatternContainer const _whiteSpaces;
    PatternContainer const _number;
    PatternContainer const _text;

    static internal const*const Create()
    {
        auto invalidLineComment = SyntaxError("EOFInLineComment");
        auto invalidComment = SyntaxError("EOFInComment");
        auto invalidTextEnd = SyntaxError("invalidTextEnd");
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

        return new internal(invalidLineComment, invalidComment, invalidTextEnd, any, whiteSpaces, number, text);
    };

    internal(
        SyntaxError const&invalidLineComment,
        SyntaxError const&invalidComment,
        SyntaxError const&invalidTextEnd,
        PatternContainer const &any,
        PatternContainer const &whiteSpaces,
        PatternContainer const &number,
        PatternContainer const &text
        )

        : _invalidLineComment(invalidLineComment)
        , _invalidComment(invalidComment)
        , _invalidTextEnd(invalidTextEnd)
        , _any(any)
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

