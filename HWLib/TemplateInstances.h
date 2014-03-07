#pragma once
//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")

#include "DumpToString.h"
#include "DumpableObject.h"
#include "FunctionCacheBase.h"
#include "CtrlRef.h"
#include <boost/numeric/ublas/vector.hpp>

using namespace HWLib;

inline String const HWLib::Dump(int target) { return String::Convert(target); };
inline String const HWLib::Dump(size_t target) { return String::Convert(target); };
inline String const HWLib::Dump(bool target) { return String::Convert(target); };

template <>
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

template <typename T>
inline String const HWLib::DumpShort(T const&target){
    auto dumpable = DynamicConvert<DumpableObject>(target);
    if (dumpable)
        return dumpable->DumpShort;
    return HWLib::DumpTypeName(target);
};

template <typename T>
inline String const HWLib::Dump(T const*target){
    return target? HWLib::Dump(*target): "null";
};

template <typename T>
inline String const HWLib::DumpShort(T const*target){
    return target ? HWLib::DumpShort(*target) : "null";
};

template <typename T>
inline String const HWLib::Dump(T*target){
    return target ? HWLib::Dump(*target) : "null";
};

template <typename T>
inline String const HWLib::DumpShort(T*target){
    return target ? HWLib::DumpShort(*target) : "null";
};

template<typename T>
class SkipIterator final : public Enumerable<T>::Iterator
{
    CtrlRef<typename Enumerable<T>::Iterator> _parent;
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
    override_p_function(bool, IsValid) {
        return _parent->IsValid; 
    }
    T const Step()override{ return _parent->Step(); }
};


template<typename T>
class TakeIterator final : public Enumerable<T>::Iterator
{
    using thisType = TakeIterator;
    CtrlRef<typename Enumerable<T>::Iterator> _parent;
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
    override_p_function(bool, IsValid){
        return _count > 0 && _parent->IsValid; 
    }
    T const Step()override{ --_count; return _parent->Step(); }
};


template<typename T>
class PlusIterator final : public Enumerable<T>::Iterator
{
    CtrlRef<typename Enumerable<T>::Iterator> _left;
    CtrlRef<typename Enumerable<T>::Iterator> _right;
public:
    PlusIterator(Enumerable<T> const& left, Enumerable<T> const& right)
        : _left(left.ToIterator)
        , _right(right.ToIterator)
    {
    }
private:
    override_p_function(bool, IsValid){
        return _left->IsValid || _right->IsValid; 
    }

    T const Step()override{ 
        if (_left->IsValid)
            return _left->Step();
        return _right->Step();
    }
};


template<typename T>
class WhereIterator final : public Enumerable<T>::Iterator
{
    CtrlRef<typename Enumerable<T>::Iterator> parent;

    CtrlPtr<T> current;
    function<bool(T)> selector;
public:
    WhereIterator(Enumerable<T> const& parent, function<bool(T)> selector)
        : parent(parent.ToIterator)
        , selector(selector)
    {
        Align();
    }

    void Align()
    {
        while (parent->IsValid){
            current = new T(parent->Step());
            if (selector(*current))
                return;
        }
        current= {};
    }
protected:
    override_p_function(bool, IsValid){ return current.IsValid; }
    T const Step()override{ 
        CtrlRef<T> result = current;
        Align();
        return T(*result); 
    }
};


template<typename T, typename TResult>
class SelectIterator final : public Enumerable<TResult>::Iterator
{
    CtrlRef<typename Enumerable<T>::Iterator> _parent;
    function<TResult(T)> _selector;
public:
    SelectIterator(Enumerable<T> const& parent, function<TResult(T)> selector)
        : _parent(parent.ToIterator)
        , _selector(selector)
    {
    }
protected:
    override_p_function(bool, IsValid){ return _parent->IsValid; }
    TResult const Step()override{ return _selector(_parent->Step()); }
};


template<typename T, typename TResult>
class ConvertManyIterator final : public Enumerable<TResult>::Iterator
{
    CtrlRef<typename Enumerable<T>::Iterator> _parent;
    CtrlPtr<typename T> _subData;
    CtrlPtr<typename Enumerable<TResult>::Iterator> _subParent;
public:
    ConvertManyIterator(Enumerable<T> const& parent)
        : _parent(parent.ToIterator){
        Align();
    }
protected:
    override_p_function(bool, IsValid){
        return _subParent.IsValid && _subParent->IsValid;
    }

    TResult const Step()override
    {
        TResult const& result = _subParent->Step();
        Align();
        return result;
    }

private:
    void Align(){
        while (true){
            if (_subParent.IsValid && _subParent->IsValid)
                return;
            if (!_parent->IsValid)
                return;
            _subData = new T(_parent->Step());
            _subParent = _subData->ToIterator;
        }
    }
};

template<typename T, typename TOther>
class PairIterator final : public Enumerable<std::pair<T, TOther>>::Iterator{
    typedef std::pair<T, TOther> resultType;
    CtrlRef<typename Enumerable<T>::Iterator> leftIterator;          
    CtrlPtr<T> leftResult;
    Enumerable<TOther> const&right;
    CtrlPtr<typename Enumerable<TOther>::Iterator> rightIterator;
public:
    PairIterator(Enumerable<T> const&left, Enumerable<TOther> const&right)
        : leftIterator(left.ToIterator)
        , right(right)
        , leftResult({})
    {
        Align();
    };

protected:
    override_p_function(bool, IsValid){
        a_if_(rightIterator.IsValid);
        return rightIterator->IsValid;
    };

    resultType const Step()override
    {
        a_if_(leftResult.IsValid);
        a_if_(rightIterator.IsValid);
        a_if_(rightIterator->IsValid);
        resultType result(*leftResult, rightIterator->Step());
        Align();
        return result;
    };
private:
    void Align(){
        if (leftIterator->IsValid){
            if (leftResult.IsValid){
                if (!rightIterator.IsValid){ b_; }
                else if (rightIterator->IsValid){ b_; }
                else { b_; }
            }else{
                if (!rightIterator.IsValid){
                    leftResult = new T(leftIterator->Step());
                    rightIterator = right.ToIterator;
                    return;
                }
                else if (rightIterator->IsValid){ b_; }
                else { b_; }
            }
        }else{
            if (leftResult.IsValid){
                if (!rightIterator.IsValid){ b_; }
                else if (rightIterator->IsValid){ b_; }
                else { return; }
            }
            else{
                if (!rightIterator.IsValid){ b_; }
                else if (rightIterator->IsValid){ b_; }
                else { b_; }
            }
        }
    };
};



template<typename T, typename TResult>
class ConvertIterator final : public Enumerable<TResult>::Iterator
{
    CtrlRef<typename Enumerable<T>::Iterator> _parent;
public:
    ConvertIterator(Enumerable<T> const& parent)
        : _parent(parent.ToIterator)
    {
    }
protected:
    override_p_function(bool, IsValid){ return _parent->IsValid; }
    TResult const Step()override{ return _parent->Step(); }
};


template<typename T>
CtrlRef<Enumerable<T>> const Enumerable<T>::Skip(int count) const
{
    return new Container(new SkipIterator<T>(*this, count));
}

template<typename T>
CtrlRef<Enumerable<T>> const Enumerable<T>::Take(int count) const
{
    return new Container(new TakeIterator<T>(*this, count));
}

template<typename T>
CtrlRef<Enumerable<T>> const Enumerable<T>::operator+(thisType const& right)const
{
    return new Container(new PlusIterator<T>(*this, right));
}

template<typename T>
CtrlRef<Enumerable<T>> const Enumerable<T>::Where(function<bool(T)> selector)const
{
    return new Container(new WhereIterator<T>(*this, selector));
}

template<typename T>
inline p_implementation(Enumerable<T>, Array<T>, ToArray)
{
    return ToIterator->ToArray;
}

template<typename T>
inline mutable_p_implementation(Enumerable<T>::Iterator, Array<T>const, ToArray){
    auto result = std::vector<T>();
    while (IsValid){
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
CtrlPtr<T> const Enumerable<T>::Max() const{
    CtrlPtr<T> result;
    for (auto element : *this)
        if (!result.IsValid || *result < element)
            result = new T(element);
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
template<typename TOther>
CtrlRef<Enumerable<std::pair<T, TOther>>> const Enumerable<T>::operator*(Enumerable<TOther>const&other)const{
    return new Enumerable<std::pair<T, TOther>>
        ::Container(new PairIterator<T, TOther>(*this, other));

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
CtrlRef<Enumerable<TResult>> const Enumerable<T>::Select(function<TResult(T)> selector) const{
    return new Enumerable<TResult>::Container(new SelectIterator<T, TResult>(*this, selector));
};

template<typename T>
template<typename TResult>
CtrlRef<Enumerable<TResult>> const Enumerable<T>::ConvertMany() const{
    return new Enumerable<TResult>::Container(new ConvertManyIterator<T, TResult>(*this));
};

template<typename T>
template<typename TResult>
CtrlRef<Enumerable<TResult>> const Enumerable<T>::Convert() const{
    return new Container(new ConvertIterator<TResult>(*this, selector));
};

template<typename TBase, typename TRealm>
inline override_p_implementation(WithId<TBase COMMA TRealm>, String, DumpHeader){
    auto objectId = HWLib::Dump(ObjectId);
    return baseType::virtual_p_name(DumpHeader)() + ".Id" + objectId;
};


template <typename T>
inline String const HWLib::Dump(CtrlPtr<T> const&target){
    if (target.IsValid)
        return "CtrlPtr{ " + HWLib::Dump(*target)+" }";
    return "CtrlPtr{}";
}

template <typename T>
inline String const HWLib::DumpShort(CtrlPtr<T> const&target){
    if (target.IsValid)
        return "CtrlPtr{ " + HWLib::DumpShort(*target) + " }";
    return "CtrlPtr{}";
}

template <typename T>
inline String const HWLib::Dump(CtrlRef<T> const&target){
    return "CtrlRef{ " + HWLib::Dump(*target) + " }";
}

template <typename T>
inline String const HWLib::Dump(WeakRef<T> const&target){
    return "WeekRef{ " + HWLib::Dump(*target) + " }";
}

template <typename T>
inline String const HWLib::Dump(Ptr<T> const&target){
    if (target.IsValid)
        return "Ptr{ " + HWLib::Dump(*target) + " }";
    return "Ptr{}";
}

template <typename T>
inline String const HWLib::Dump(Ref<T> const&target){
    return "Ref{ " + HWLib::Dump(*target) + " }";
}

template <typename T>
inline String const HWLib::Dump(WeakPtr<T> const&target){
    if (target.IsValid)
        return "WeakPtr{ " + HWLib::Dump(*target) + " }";
    return "WeakPtr{}";
}

template <typename T>
inline String const HWLib::Dump(Array<T> const&target){
    auto result = "Array["+ HWLib::Dump(target.Count)+ "]";
    auto index = 0;
    auto dataResult = target
        .Select<String>([&](T const&element){
            return "[" + HWLib::Dump(index++) + "] " + HWLib::Dump(element);
        })
        ->ToArray;

    return result
        + String::Surround("{", dataResult,"}");
}

template <typename T1, typename T2>
inline String const HWLib::Dump(std::pair<T1, T2> const&target){
    auto dataResult = _({
        "first = " + HWLib::Dump(target.first),
        "second = " + HWLib::Dump(target.second),
    });
    return String::Surround(
        "{", 
        dataResult, 
        "}");
}


//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")
