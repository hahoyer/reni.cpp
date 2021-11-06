#include "Import.h"
#include "Pattern.h"

#include "Source.h"
#include "SourcePosition.h"
#include "../HWLib/Optional.h"

using namespace HWLib;
using namespace HWLang;


MatchResult EndPattern::Match(const SourcePosition& position) const
{
  return position.End;
};

class FindMatcher final : public IPattern
{
  const CtrlRef<const IPattern> _value;
public:
  FindMatcher(CtrlRef<const IPattern> value) : _value(value) {}
private:
  MatchResult Match(const SourcePosition& position) const override
  {
    for(auto current = position;; current += 1)
    {
      const auto result = _value->Match(current);
      if(result.IsValid)
        return current - position + result;

      if(current.IsEnd)
        return {};
    }
  }
};

p_implementation(Pattern, Pattern, Find) { return new FindMatcher(_value); };

class ElseMatcher final : public IPattern
{
  const CtrlRef<const IPattern> _left;
  const CtrlRef<const IPattern> _right;
public:
  ElseMatcher(CtrlRef<const IPattern> left, CtrlRef<const IPattern> right)
    : _left(left)
      , _right(right)
  {}

private:
  MatchResult Match(const SourcePosition& position) const override
  {
    return _left->Match(position)
      || [&] { return _right->Match(position); };
  }
};

Pattern Pattern::Else(const Pattern& right) const { return new ElseMatcher(_value, right._value); }
Pattern Pattern::Else(std::string const& right) const { return Else(Box(right)); }

class RepeatMatch final : public IPattern
{
  const CtrlRef<const IPattern> _data;
  const size_t _minCount;
  const Optional<size_t> _maxCount;

public:
  RepeatMatch(CtrlRef<const IPattern> data, size_t minCount, Optional<size_t> maxCount)
    : _data(data)
      , _minCount(minCount)
      , _maxCount(maxCount)
  {
    HW_ASSERT_IS(_minCount, >=, 0);
    HW_ASSERT_(!_maxCount.IsValid || _minCount <= _maxCount);
  }

private:
  MatchResult Match(const SourcePosition& position) const override
  {
    auto current = position;
    for(auto count = 0;; count++)
    {
      auto result = current - position;
      if(_maxCount.IsValid && count == _maxCount.Value)
        return result;

      const auto length = _data->Match(current);
      if(!length.IsValid)
        return count < _minCount ? MatchResult() : result;
      if(current.IsEnd)
        return MatchResult();
      current += length;
    }
  }
};

const Pattern Pattern::Repeat(size_t minCount, Optional<size_t> maxCount) const
{
  return new RepeatMatch(_value, minCount, maxCount);
}

class ValueMatch final : public IPattern
{
  const CtrlRef<const IPattern> _data;
  function<const Pattern (std::string)> _func;

public:
  ValueMatch(CtrlRef<const IPattern> data, function<const Pattern (std::string)> func)
    : _data(data)
      , _func(func)
  {}

  MatchResult Match(const SourcePosition& position) const override
  {
    const auto length = _data->Match(position);
    if(!length.IsValid)
      return MatchResult();
    const auto value = position.Part(length.Value);
    const auto funcResult = _func(value).Match(position + length.Value);
    return funcResult.IsValid ? length.Value + funcResult.Value : MatchResult();
  }
};

const Pattern Pattern::Value(function<const Pattern (std::string)> func) const { return new ValueMatch(_value, func); }

class SequenceMatch final : public IPattern
{
  const CtrlRef<const IPattern> _left;
  const CtrlRef<const IPattern> _right;
public:
  SequenceMatch(CtrlRef<const IPattern> left, CtrlRef<const IPattern> right)
    : _left(left)
      , _right(right)
  {}

private:
  MatchResult Match(const SourcePosition& position) const override
  {
    const auto leftResult = _left->Match(position);
    if(!leftResult.IsValid)
      return {};

    const auto rightResult = _right->Match(position + leftResult.Value);
    if(!rightResult.IsValid)
      return {};

    return leftResult.Value + rightResult.Value;
  }
};

Pattern Pattern::operator+(Pattern const& right) const { return new SequenceMatch(_value, right._value); };
Pattern Pattern::operator+(std::string const& right) const { return *this + Box(right); };
Pattern HWLang::operator+(std::string const& left, const Pattern& right) { return Box(left) + right; };

class CharMatch final : public IPattern
{
  const std::string _value;
public:
  CharMatch(std::string value) : _value(value) {}

private:
  MatchResult Match(const SourcePosition& position) const override
  {
    return position.BeginsWith(_value) ? MatchResult(_value.size()) : MatchResult();
  }
};

Pattern HWLang::Box(std::string value) { return new CharMatch(value); };

class FunctionalMatch final : public IPattern
{
  const function<bool(char)> _func;
  const bool _isTrue;
public:
  FunctionalMatch(function<bool(char)> func, bool isTrue)
    : _func(func)
      , _isTrue(isTrue)
  { }

private:
  MatchResult Match(const SourcePosition& position) const override
  {
    return _func(position.First) == _isTrue ? MatchResult(1) : MatchResult();
  }
};

Pattern HWLang::Box(function<bool(char)> value) { return new FunctionalMatch(value, true); };

class AnyCharMatch final : public IPattern
{
  const std::string _value;
public:
  AnyCharMatch(const char* value): _value(value) { }
private:
  MatchResult Match(const SourcePosition& position) const override
  {
    return _value | Contains(position.First) ? MatchResult(1) : MatchResult();
  }
};

Pattern HWLang::AnyChar(const char* value) { return new AnyCharMatch(value); };
