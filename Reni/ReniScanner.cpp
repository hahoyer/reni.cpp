#include "Import.h"
#include "ReniScanner.h"

static bool Trace = true;

using namespace Reni;
using namespace HWLang;


Optional<int> const ExceptionGuard(SourcePosition const&position, Pattern pattern)
{
    try
    {
        return pattern.Match(position);
    }
    catch (Exception<String> exception)
    {
        throw Scanner::Error(exception.Position - position, exception.Error);
    }
};


struct Scanner::internal
{
    Pattern const _any;
    Pattern const _whiteSpaces;
    Pattern const _number;
    Pattern const _text;
    
    static internal const*const Create()
    {
        auto invalidLineComment = HWLang::Error(String("EOFInLineComment"));
        auto invalidComment = HWLang::Error(String("EOFInComment"));
        auto invalidTextEnd = HWLang::Error(String("invalidTextEnd"));
        auto invalidCharacter = HWLang::Error(String("invalidCharacter"));
        auto alpha = Letter.Else("_");
        auto symbol1 = AnyChar("({[)}];,");
        auto textFrame = AnyChar("'\"");
        auto symbol = AnyChar("°^!²§³$%&/=?\\@€*+~><|:.-");

        auto identifier =
            (alpha + (alpha.Else(Digit)).Repeat())
            .Else(symbol.Repeat(1));

        auto any = symbol1.Else(identifier);

        auto whiteSpaces = HWLang::WhiteSpace
            .Else("#"
                + ("#" + AnyChar("\n\r").Find)
                  .Else("(" + HWLang::WhiteSpace + (HWLang::WhiteSpace + ")#").Find)
                  .Else("(" + any.Value([](String id){return (HWLang::WhiteSpace + id + ")#").Find; }))
                  .Else("(" + End.Find + invalidComment)
                  .Else("#" + End.Find + invalidLineComment)
                  .Else(invalidCharacter)
                  )
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

Scanner::Scanner()
: _internal(internal::Create())
{};

static Scanner instance;

int const Scanner::WhiteSpace(SourcePosition const&position)
{
    auto result = ExceptionGuard(position, instance._internal->_whiteSpaces);
    a_if_(result.IsValid);
    return result;
};


Optional<int> const Scanner::Number(SourcePosition const&position) { return ExceptionGuard(position, instance._internal->_number); };
Optional<int> const Scanner::Any(SourcePosition const&position) { return ExceptionGuard(position, instance._internal->_any); };
Optional<int> const Scanner::Text(SourcePosition const&position) { return ExceptionGuard(position, instance._internal->_text); };

