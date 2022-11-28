#pragma once

using namespace HWLang;

template <class Token, class TokenFactory, class Scanner>
Token
ScannerInstance<Token, TokenFactory, Scanner>
::Step()
{
  try
  {
    if(!startTokenReturned)
    {
      startTokenReturned = true;
      SetDumpString();
      return Token(TokenFactory::Start, position.Span(0));
    }

    static auto Trace = true;
    position += Scanner::WhiteSpace(position);
    SetDumpString();

    auto count = position.End;
    if(count.IsValid)
    {
      endTokenReturned = true;
      return Step(count, TokenFactory::End);
    }

    count = Scanner::Number(position);
    if(count.IsValid)
      return Step(count, TokenFactory::Number);

    count = Scanner::Text(position);
    if(count.IsValid)
      return Step(count, TokenFactory::Text);

    count = Scanner::Any(position);
    if(count.IsValid)
      return Step(count, TokenFactory::GetTokenClass(position.Part(count)));

    {
      auto HW_D_LOG_HEADER_outerIndentLevel = Console::Instance.IndentLevel;
      if(Trace)
      {
        Console::Instance.Write(
          String::FilePosition("TemplateInstances.h", 42, 0, __FUNCTION__));
        Console::Instance.IndentCount++;
        Console::Instance.Write("\n");
        Console::Instance.Write(std::string("*this") + " = " + HWLib::Dump(*this) + ";" + "\n");
      };
    };
    HW_FAIL_;
    return Optional<Token>().Value;
  }
  catch(const typename Scanner::Error& error)
  {
    return Step(error.Count, TokenFactory::GetErrorClass(error.Id));
  }
}


template <class Token, class TokenFactory, class Scanner>
Token
ScannerInstance<Token, TokenFactory, Scanner>
::Step(size_t count, const TokenClass& tokenClass)
{
  auto part = position.Span(count);
  position += count;
  SetDumpString();
  return Token(tokenClass, part);
};


template <class Token, class TokenFactory, class Scanner>
HW_PR_IMPL_GETTER(ScannerInstance<Token COMMA TokenFactory COMMA Scanner>, Array<string>, DumpData)
{
  return {
    HW_D_VALUE(position)
  };
}
