#include "Import.h"
#include "Scanner.h"
#include "../HWLib/Optional.h"
#include "../HWLang/SourcePosition.h"
#include "../HWLang/Pattern.h"
#include "../HWLib/string.h"

static bool Trace = true;

using namespace Util;
using namespace HWLang;
using namespace HWLib;


Optional<size_t> ExceptionGuard(const SourcePosition& position, const Pattern& pattern)
{
  try
  {
    return pattern.Match(position);
  }
  catch(Exception<string> exception)
  {
    throw Scanner::Error(exception.Position - position, exception.Error);
  }
};


struct Scanner::internal
{
  const Pattern _any;
  const Pattern _whiteSpaces;
  const Pattern _number;
  const Pattern _text;

  static const internal* Create()
  {
    const auto invalidLineComment = HWLang::Error(string("EOFInLineComment"));
    const auto invalidComment = HWLang::Error(string("EOFInComment"));
    const auto invalidTextEnd = HWLang::Error(string("invalidTextEnd"));
    const auto invalidCharacter = HWLang::Error(string("invalidCharacter"));
    const auto alpha = Letter.Else("_");
    const auto symbol1 = AnyChar("({[)}];,");
    const auto textFrame = AnyChar("'\"");
    const auto symbol = AnyChar("°^!²§³$%&/=?\\@€*+~><|:.-");

    const auto identifier =
      (alpha + alpha.Else(Digit).Repeat())
      .Else(symbol.Repeat(1));

    const auto any = symbol1.Else(identifier);

    const auto whiteSpaces = HWLang::WhiteSpace
                             .Else("#"
                               + ("#" + AnyChar("\n\r").Find)
                                 .Else("(" + HWLang::WhiteSpace + (HWLang::WhiteSpace + ")#").Find)
                                 .Else("(" + any.Value([](string id) { return (HWLang::WhiteSpace + id + ")#").Find; }))
                                 .Else("(" + SourceEnd.Find + invalidComment)
                                 .Else("#" + SourceEnd.Find + invalidLineComment)
                                 .Else(invalidCharacter)
                             )
                             .Repeat();

    const auto number = Digit.Repeat(1);

    const auto text = textFrame
      .Value
      ([=](string head)
      {
        const auto textEnd = Box(head).Else(LineEnd + invalidTextEnd);
        return textEnd.Find + (head + textEnd.Find).Repeat();
      });

    return new internal(any, whiteSpaces, number, text);
  };

  internal(
    const Pattern& any,
    const Pattern& whiteSpaces,
    const Pattern& number,
    const Pattern& text
  )

    : _any(any)
      , _whiteSpaces(whiteSpaces)
      , _number(number)
      , _text(text)
  { };
};

Scanner::Scanner()
  : _internal(internal::Create())
{};

static Scanner instance;

size_t Scanner::WhiteSpace(const SourcePosition& position)
{
  auto result = ExceptionGuard(position, instance._internal->_whiteSpaces);
  HW_ASSERT_(result.IsValid);
  return result;
};


Optional<size_t> Scanner::Number(const SourcePosition& position)
{
  return ExceptionGuard(position, instance._internal->_number);
};

Optional<size_t> Scanner::Any(const SourcePosition& position)
{
  return ExceptionGuard(position, instance._internal->_any);
};

Optional<size_t> Scanner::Text(const SourcePosition& position)
{
  return ExceptionGuard(position, instance._internal->_text);
};
