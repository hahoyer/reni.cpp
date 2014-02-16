#include "Import.h"
#include "Pattern.h"

#include "Source.h"
#include "SourcePosition.h"

using namespace HWLang;


MatchResult EndPattern::Match(SourcePosition const&position)const
{
    return position.End;
};

class FindMatcher final: public IPattern 
{
    Ref<IPattern const> const _value;
public:
    FindMatcher(Ref<IPattern const> value) : _value(value){}
private:
    virtual MatchResult Match(SourcePosition const&position)const override
    {
        for (auto current = position;; current += 1)
        {
            auto result = _value->Match(current);
            if (result.IsValid)
                return (current - position) + result;

            if (current.IsEnd)
                return null;
        }
    }
};

p_implementation(Pattern, Pattern, Find){ return new FindMatcher(_value); };

class ElseMatcher final : public IPattern
{
    Ref<IPattern  const> const _left;
    Ref<IPattern  const> const _right;
public:
    ElseMatcher(Ref<IPattern  const> left, Ref<IPattern  const> right)
        : _left(left)
        , _right(right)
    {}
private:
    virtual MatchResult Match(SourcePosition const&position)const override
    {
        return _left->Match(position)
            || [&]{return _right->Match(position); };
    }
};

Pattern const Pattern::Else(Pattern const& right)const{ return new ElseMatcher(_value, right._value); }
Pattern const Pattern::Else(String right)const{ return Else(Box(right)); }

class RepeatMatch final : public IPattern
{
    Ref<IPattern  const> const _data;
    int  const _minCount;
    Optional<int> const _maxCount;

public:
    RepeatMatch(Ref<IPattern  const> data, int minCount, Optional<int> maxCount)
        :_data(data)
        , _minCount(minCount)
        , _maxCount(maxCount)
    {
        a_is(_minCount, >=, 0);
        a_if_(!_maxCount.IsValid || _minCount <= _maxCount);
    }

private:
    virtual MatchResult Match(SourcePosition const&position)const override
    {
        auto current = position;
        for (auto count = 0;; count++)
        {
            auto result = current - position;
            if (_maxCount.IsValid && count == _maxCount)
                return result;

            auto length = _data->Match(current);
            if (!length.IsValid)
                return count < _minCount ? MatchResult() : result;
            if (current.IsEnd)
                return MatchResult();
            current += length;
        }
    }
};

Pattern const Pattern::Repeat(int minCount, Optional<int> maxCount)const{ return new RepeatMatch(_value, minCount, maxCount); }

class ValueMatch final : public IPattern
{
    Ref<IPattern  const> const _data;
    function<Pattern const(String)> _func;

public:
    ValueMatch(Ref<IPattern  const> data, function<Pattern const(String)> func)
        : _data(data)
        , _func(func)
    {}

    virtual MatchResult Match(SourcePosition const&position)const override
    {
        auto length = _data->Match(position);
        if (!length.IsValid)
            return MatchResult();
        auto value = position.Part(length.Value);
        auto funcResult = _func(value).Match(position + length.Value);
        return funcResult.IsValid ? length.Value + funcResult.Value : MatchResult();
    }
};

Pattern const Pattern::Value(function<Pattern const(String)> func)const{ return new ValueMatch(_value, func); }

class SequenceMatch final : public IPattern
{
    Ref<IPattern  const> const _left;
    Ref<IPattern  const> const _right;
public:
    SequenceMatch(Ref<IPattern  const> left, Ref<IPattern  const> right)
        : _left(left)
        , _right(right)
    {}
private:
    virtual MatchResult Match(SourcePosition const&position)const override
    {
        auto leftResult = _left->Match(position);
        if (!leftResult.IsValid)
            return null;

        auto rightResult = _right->Match(position + leftResult.Value);
        if (!rightResult.IsValid)
            return null;

        return leftResult.Value + rightResult.Value;
    }
};

Pattern const Pattern::operator+ (Pattern right)const{ return new SequenceMatch(_value, right._value); };
Pattern const Pattern::operator+(String right)const{ return *this + Box(right); };
Pattern const HWLang::operator+(String left, Pattern right){ return Box(left) + right; };

class CharMatch final : public IPattern
{
    String const _value;
public:
    CharMatch(String value) : _value(value) {}

private:
    virtual MatchResult Match(SourcePosition const&position)const override
    {
        return position.BeginsWith(_value) ? MatchResult(_value.Count) : MatchResult();
    }
};

Pattern const HWLang::Box(String value){ return new CharMatch(value); };

class FunctionalMatch final : public IPattern
{
    function<bool(char)> const _func;
    bool const _isTrue;
public:
    FunctionalMatch(function<bool(char)> func, bool isTrue)
        :_func(func)
        , _isTrue(isTrue)
    {
    }
private:
    virtual MatchResult Match(SourcePosition const&position)const override
    {
        return _func(position.First) == _isTrue ? MatchResult(1): MatchResult();
    }
};

Pattern const HWLang::Box(function<bool(char)> value){ return new FunctionalMatch(value, true); };

class AnyCharMatch final : public IPattern
{
    String const _value;
public:
    AnyCharMatch(char const* value): _value(value) { }
private:
    virtual MatchResult Match(SourcePosition const&position)const override
    {
        return _value.Contains(position.First) ? MatchResult(1) : MatchResult();
    }
};

Pattern const HWLang::AnyChar(char const* value){ return new AnyCharMatch(value); };
