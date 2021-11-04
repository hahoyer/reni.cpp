#pragma once
#include <initializer_list>

namespace HWLang
{
  namespace PrioTableConst
  {
    struct Tag
    {
      const char value;
      explicit Tag(const char value) : value(value) {};
      bool operator==(const Tag& other) const { return value == other.value; };
      p(string, Dump)
      {
        switch(value)
        {
        case '+': return "HigherTag";
        case '-': return "LowerTag";
        case ' ': return "UnknownTag";
        default:
          return string(0, value) | Quote();
        }
      }

      p(bool, IsValid)
      {
        switch(value)
        {
        case '+':
        case '-':
        case ' ':
          return true;
        default:
          return false;
        }
      };
    };

    typedef std::initializer_list<const initializer_list<const Tag>> TagTable;

    static const char* Any = "(any)";
    static const char* End = "(end)";
    static const char* Start = "(start)";
    static const char* Error = "(error)";

    const Tag HigherTag('+');
    const Tag LowerTag('-');
    const Tag UnknownTag(' ');

    static const TagTable ParenthesisTableLeft =
    {
      {HigherTag, HigherTag, LowerTag},
      {HigherTag, UnknownTag, LowerTag},
      {UnknownTag, LowerTag, LowerTag}
    };

    static const TagTable ParenthesisTableRight =
    {
      {HigherTag, HigherTag, HigherTag},
      {HigherTag, UnknownTag, LowerTag},
      {UnknownTag, LowerTag, LowerTag}
    };

    static const TagTable ThenElseTable =
    {
      {HigherTag, LowerTag, LowerTag},
      {HigherTag, UnknownTag, HigherTag},
      {UnknownTag, LowerTag, HigherTag}
    };

    static const TagTable LeftTable =
    {
      {UnknownTag, UnknownTag, UnknownTag},
      {UnknownTag, UnknownTag, HigherTag},
      {UnknownTag, LowerTag, LowerTag}
    };

    static const TagTable RightTable =
    {
      {UnknownTag, UnknownTag, UnknownTag},
      {UnknownTag, UnknownTag, HigherTag},
      {UnknownTag, LowerTag, HigherTag}
    };
  };
};

namespace HWLib
{
  inline string Dump(Array<const Array<const HWLang::PrioTableConst::Tag>> target);
  inline string Dump(const HWLang::PrioTableConst::TagTable& target);

  inline string Dump(HWLang::PrioTableConst::Tag target)
  {
    return target.Dump;
  };

  template <class TResult>
  const TResult* DynamicConvert(const HWLang::PrioTableConst::Tag&)
  {
    return nullptr;
  };

  template <class TResult>
  const TResult* DynamicConvert(const HWLang::PrioTableConst::TagTable&)
  {
    return nullptr;
  };
}
