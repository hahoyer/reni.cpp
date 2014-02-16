#pragma once
//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")

#include "DumpToString.h"
#include "DumpableObject.h"
#include "Ref.h"
#include <boost/numeric/ublas/vector.hpp>

using namespace HWLib;

inline String const HWLib::Dump(int target) { return String::Convert(target); };
inline String const HWLib::Dump(char const* target){ return Dump(String(target)); };

template <>
inline String const HWLib::Dump(String const&target){ return target.Quote; };


template <typename T>
inline String const HWLib::Dump(T const&target){ 
    auto dumpable = DynamicConvert<DumpableObject>(target);
    if (dumpable)
        return dumpable->Dump;
    return HWLib::DumpTypeName(target); 
};

template<typename T>
class SkipIterator final : public Enumerable<T>::Iterator
{
    Ref<typename Enumerable<T>::Iterator> _parent;
public:
    SkipIterator(Enumerable<T> const& parent, int count)
        : _parent(parent.ToIterator)
    {
        while (count > 0 && _parent->IsValid)
        {
            _parent->Step();
            --count;
        }
    }

protected:
    p_function(bool, IsValid) override{ return _parent->IsValid; }
    T const Step()override{ return _parent->Step(); }
};


template<typename T>
class TakeIterator final : public Enumerable<T>::Iterator
{
    using thisType = TakeIterator;
    Ref<typename Enumerable<T>::Iterator> _parent;
    int _count;
public:
    TakeIterator(Enumerable<T> const& parent, int count)
        : _parent(parent.ToIterator)
        , _count(count)
    {
    }

    TakeIterator(TakeIterator const& other)
        : _parent(other._parent->Clone)
        , _count(other._count)
    {
    }
protected:
    p_function(bool, IsValid) override{ return _count > 0 && _parent->IsValid; }
    T const Step()override{ --_count; return _parent->Step(); }
};


template<typename T>
class PlusIterator final : public Enumerable<T>::Iterator
{
    Ref<typename Enumerable<T>::Iterator> _left;
    Ref<typename Enumerable<T>::Iterator> _right;
public:
    PlusIterator(Enumerable<T> const& left, Enumerable<T> const& right)
        : _left(left.ToIterator)
        , _right(right.ToIterator)
    {
    }
private:
    p_function(bool, IsValid) override{ return _left->IsValid || _right->IsValid; }

    void operator++(int) override
    {
        if (_left->IsValid)
            (*_left)++;
        else
            (*_right)++;
        return *this;
    }

    T const operator*()const override
    {
        if (_left->IsValid)
            return **_left;
        return **_right;
    }
};


template<typename T>
class WhereIterator final : public Enumerable<T>::Iterator
{
    Ref<typename Enumerable<T>::Iterator> _parent;
    function<bool(T)> _selector;
public:
    WhereIterator(Enumerable<T> const& parent, function<bool(T)> selector)
        : _parent(parent.ToIterator)
        , _selector(selector)
    {
        Align();
    }

    void Align()
    {
        while (_parent->IsValid && !selector(**_parent))
            (*_parent)++;
    }
protected:
    p_function(bool, IsValid) override{ return _parent->IsValid; }
    void operator++(int) override{ (*_parent)++; Align(); return *this; }
    T const operator*()const override{ return **_parent; }
};


template<typename T, typename TResult>
class SelectIterator final : public Enumerable<TResult>::Iterator
{
    Ref<typename Enumerable<T>::Iterator> _parent;
    function<TResult(T)> _selector;
public:
    SelectIterator(Enumerable<T> const& parent, function<TResult(T)> selector)
        : _parent(parent.ToIterator)
        , _selector(selector)
    {
    }
protected:
    p_function(bool, IsValid) override{ return _parent->IsValid; }
    TResult const Step()override{ return _selector(_parent->Step()); }
};


template<typename T, typename TResult>
class ConvertManyIterator final : public Enumerable<TResult>::Iterator
{
    Ref<typename Enumerable<T>::Iterator> _parent;
    OptRef<typename T> _subData;
    OptRef<typename Enumerable<TResult>::Iterator> _subParent;
public:
    ConvertManyIterator(Enumerable<T> const& parent)
        : _parent(parent.ToIterator)
    {
    }
protected:
    p_function(bool, IsValid) override
    { 
        return _parent->IsValid
            || _subParent.IsValid && _subParent->IsValid;
    }

    TResult const Step()override
    {
        if (!(_subParent.IsValid && _subParent->IsValid))
        {
            _subData = new T(_parent->Step());
            _subParent = _subData->ToIterator;
        }
        return _subParent->Step(); 
    }
};


template<typename T, typename TResult>
class ConvertIterator final : public Enumerable<TResult>::Iterator
{
    Ref<typename Enumerable<T>::Iterator> _parent;
public:
    ConvertIterator(Enumerable<T> const& parent)
        : _parent(parent.ToIterator)
    {
    }
protected:
    p_function(bool, IsValid) override{ return _parent->IsValid; }
    TResult const Step()override{ return _parent->Step(); }
};


template<typename T>
Ref<Enumerable<T>> const Enumerable<T>::Skip(int count) const
{
    return new Container(new SkipIterator<T>(*this, count));
}

template<typename T>
Ref<Enumerable<T>> const Enumerable<T>::Take(int count) const
{
    return new Container(new TakeIterator<T>(*this, count));
}

template<typename T>
Ref<Enumerable<T>> const Enumerable<T>::operator+(thisType const& right)const
{
    return new Container(new PlusIterator<T>(*this, right));
}

template<typename T>
Ref<Enumerable<T>> const Enumerable<T>::Where(function<bool(T)> selector)const
{
    return new Container(new WhereIterator(*this, selector));
}

template<typename T>
inline p_implementation(Enumerable<T>, Array<T>, ToArray)
{
    return ToIterator->ToArray();
}

template<typename T>
inline Array<T> const Enumerable<T>::Iterator::ToArray()
{
    auto result = std::vector<T>();
    while (IsValid)
    {
        auto value = Step();
        result.push_back(value);
    }
    return Array<T>(result.size(), [=](int i){return result[i]; });

}

template<typename T>
inline T const Enumerable<T>::Stringify(T const&delimiter)const
{
    auto result = T();
    auto useDelimiter = false;
    for (auto element : *this)
    {
        if (useDelimiter)
            result = result + delimiter;
        useDelimiter = true;
        result = result + element;
    }
    return result;
}

template<typename T>
inline int const Enumerable<T>::Count(function<bool(T)> selector)const
{
    auto result = 0;
    for (auto element : *this)
        if(selector(element))
            result++;
    return result;
}

template<typename T>
OptRef<T> const Enumerable<T>::Max() const{
    OptRef<T> result;
    for (auto element : *this)
        if (!result.IsValid || *result < element)
            result = element;
    return result;
};

template<typename T>
template<typename TResult>
TResult const Enumerable<T>::Aggregate(TResult start, AggregateFunction<TResult> selector)const
{
    auto result = start;
    for (auto element : *this)
        result = selector(result,element);
    return result;
}

template<typename T>
typename Enumerable<T>::RangeBasedForLoopSimulator const Enumerable<T>::end()const
{
    return RangeBasedForLoopSimulator();
}

template <typename T>
String const HWLib::DumpTypeName(T const& object){
    auto localObject = &object;
    auto result = std::string(typeid(object).name());
    if (result.substr(0, 6) == "class ")
        result.erase(0, 6);
    else if (result.substr(0, 7) == "struct ")
            result.erase(0, 7);
    return
        String(result)
        ;
};

template<typename T>
template<typename TResult>
Ref<Enumerable<TResult>> const Enumerable<T>::Select(function<TResult(T)> selector) const{
    return new Enumerable<TResult>::Container(new SelectIterator<T, TResult>(*this, selector));
};

template<typename T>
template<typename TResult>
Ref<Enumerable<TResult>> const Enumerable<T>::ConvertMany() const{
    return new Enumerable<TResult>::Container(new ConvertManyIterator<T, TResult>(*this));
};

template<typename T>
template<typename TResult>
Ref<Enumerable<TResult>> const Enumerable<T>::Convert() const{
    return new Container(new ConvertIterator<TResult>(*this, selector));
};

template<typename T>
p_implementation(Ref<T>, Array<String>, DumpData){
    if (!value.get())
        return Array<String>();
    return Array<String>{ Ref<T>::traits::DumpValue(*value) };
};

template<typename T>
p_implementation(Ref<T>, String, DumpHeader){
    if (!value.get())
        return "null";
    return "Ref";
};

template<typename T>
p_implementation(OptRef<T>, String, DumpHeader){
    if (!value.get())
        return "null";
    return "OptRef";
};

template<typename T>
inline String const default_ref_traits<T>::DumpValueHeader(T const&value){
    return HWLib::DumpTypeName(value);
};

template<typename T>
inline String const default_ref_traits<T>::DumpValue(T const&value){
    return HWLib::Dump(value);
};

//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")
