#pragma once
//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")

#include "Ref.h"
#include <vector>

using namespace HWLib;

template<typename T>
String const HWLib::DumpToString(T const&) { return String(typeid(T).name()); }
template<typename T>
String const HWLib::DumpToString(T const&target, int radix) = delete;
template<typename T>
String const Box_<T>::DumpToString()const{ return HWLib::DumpToString(_data); }
template<>
inline String const HWLib::DumpToString<int>(int const&target) { return String::Convert(target); }


template<typename T>
class Enumerable<T>::TakeIterator final : public Enumerable<T>::Iterator
{
    Ref<Iterator> _parent;
    int _count;
public:
    TakeIterator(Enumerable<T> const& parent, int count)
        : _parent(parent.ToIterator)
        , _count(count)
    {
    }
protected:
    p_function(bool, IsValid) override{ return _count > 0 && _parent->IsValid; }
    Iterator& operator++(int) override{ --_count; (*_parent)++; return *this; }
    T const operator*()const override{ return **_parent; }
};


template<typename T>
class Enumerable<T>::PlusIterator final : public Enumerable<T>::Iterator
{
    Ref<Iterator> _left;
    Ref<Iterator> _right;
public:
    PlusIterator(Enumerable<T> const& left, Enumerable<T> const& right)
        : _left(left.ToIterator)
        , _right(right.ToIterator)
    {
    }
private:
    p_function(bool, IsValid) override{ return _left->IsValid || _right->IsValid; }

    Iterator& operator++(int) override
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
class Enumerable<T>::WhereIterator final : public Enumerable<T>::Iterator
{
    Ref<Iterator> _parent;
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
    Iterator& operator++(int) override{ (*_parent)++; Align(); return *this; }
    T const operator*()const override{ return **_parent; }
};


template<typename T>
Ref<Enumerable<T>> const Enumerable<T>::Skip(int count) const
{
    return new Container([=](){return SkipIterator(count); });
}

template<typename T>
Ref<Enumerable<T>> const Enumerable<T>::Take(int count) const
{
    return new Container([=](){return new TakeIterator(*this, count); });
}

template<typename T>
Ref<Enumerable<T>> const Enumerable<T>::operator+(thisType const& right)const
{
    return new Container([=](){return new PlusIterator(*this, right); });
}

template<typename T>
Ref<Enumerable<T>> const Enumerable<T>::Where(function<bool(T)> selector)const
{
    return new Container([=](){return new WhereIterator(*this, selector); });
}

template<typename T>
inline p_implementation(Enumerable<T>, Array<T>, ToArray)
{
    auto result = std::vector<T>();
    auto i = ToIterator;
    for (; i->IsValid; (*i)++)
        result.push_back(**i);
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
template<typename TSplitter>
inline Ref<Enumerable<int>> const Enumerable<T>::Split()const
{
    return new Enumerable<int>::Container(new TSplitter(*this));
}


//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")
