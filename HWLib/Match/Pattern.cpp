#include "Import.h"
#include "Pattern.h"

#include "SourcePosition.h"

using namespace HWLib;
using namespace Match;

r EndPattern::Match(SourcePosition const&position)const
{
    return position.End;
};

class FindMatcher final: public IPattern 
{
    Ref<IPattern const> const _value;
public:
    FindMatcher(Ref<IPattern const> value) : _value(value){}
private:
    virtual r Match(SourcePosition const&position)const override
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

p_implementation(Pattern, pr, Find){ return new FindMatcher(_value); };

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
    virtual r Match(SourcePosition const&position)const override
    {
        return _left->Match(position)
            || [&]{return _right->Match(position); };
    }
};

pr Pattern::Else(Pattern const& right)const{ return new ElseMatcher(_value, right._value); }
pr Pattern::Else(String right)const{ return Else(Box(right)); }

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
        assert(_minCount >= 0);
        assert(!_maxCount.IsValid || _minCount <= _maxCount);
    }

private:
    virtual r Match(SourcePosition const&position)const override
    {
        auto current = position;
        for (auto count = 0;; count++)
        {
            auto result = current - position;
            if (_maxCount.IsValid && count == _maxCount)
                return result;

            auto length = _data->Match(current);
            if (!length.IsValid)
                return count < _minCount ? r() : result;
            if (current.IsEnd)
                return r();
            current += length;
        }
    }
};

pr Pattern::Repeat(int minCount, Optional<int> maxCount)const{ return new RepeatMatch(_value, minCount, maxCount); }

class ValueMatch final : public IPattern
{
    Ref<IPattern  const> const _data;
    function<pr(String)> _func;

public:
    ValueMatch(Ref<IPattern  const> data, function<pr(String)> func)
        : _data(data)
        , _func(func)
    {}

    virtual r Match(SourcePosition const&position)const override
    {
        auto length = _data->Match(position);
        if (!length.IsValid)
            return r();
        auto value = position.Part(length.Value);
        auto funcResult = _func(value).Match(position + length.Value);
        return funcResult.IsValid ? length.Value + funcResult.Value : r();
    }
};

pr Pattern::Value(function<pr(String)> func)const{ return new ValueMatch(_value, func); }

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
    virtual r Match(SourcePosition const&position)const override
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

pr Pattern::operator+ (Pattern right)const{ return new SequenceMatch(_value, right._value); };
pr Pattern::operator+(String right)const{ return *this + Box(right); };
pr HWLib::Match::operator+(String left, Pattern right){ return Box(left) + right; };

class CharMatch final : public IPattern
{
    String const _value;
public:
    CharMatch(String value) : _value(value) {}

private:
    virtual r Match(SourcePosition const&position)const override
    {
        return position.BeginsWith(_value) ? r(_value.Count) : r();
    }
};

pr HWLib::Match::Box(String value){ return new CharMatch(value); };

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
    virtual r Match(SourcePosition const&position)const override
    {
        return _func(position.First) == _isTrue ? r(1): r();
    }
};

pr HWLib::Match::Box(function<bool(char)> value){ return new FunctionalMatch(value, true); };

class AnyCharMatch final : public IPattern
{
    String const _value;
public:
    AnyCharMatch(char const* value): _value(value) { }
private:
    virtual r Match(SourcePosition const&position)const override
    {
        return _value.Contains(position.First) ? r(1) : r();
    }
};

pr HWLib::Match::AnyChar(char const* value){ return new AnyCharMatch(value); };
