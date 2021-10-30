#pragma once
//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")

#include "DumpToString.h"
#include "DumpableObject.h"
#include "FunctionCacheBase.h"
#include "CtrlRef.h"
#include <queue>

using namespace HWLib;
using namespace std;

inline String const HWLib::Dump(__int64 target, int radix) { return String::Convert(target, radix); };
inline String const HWLib::Dump(int target, int radix) { return String::Convert(target, radix); };
inline String const HWLib::Dump(size_t target, int radix) { return String::Convert(target, radix); };
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
class Enumerable<T>::RangeBasedForLoopSimulator final
{
    Optional<CtrlRef<Iterator>> _data;
    mutable bool _hasBeenAccessed;

public:
    RangeBasedForLoopSimulator(CtrlRef<Iterator> data)
        : _data(data)
        , _hasBeenAccessed(false)
    {
    }
    RangeBasedForLoopSimulator(){}

    virtual ~RangeBasedForLoopSimulator(){}

    void operator++()
    {
        if(_hasBeenAccessed)
            _hasBeenAccessed = false;
        else
            _data.Value->Step();
    };

    T const operator *()const
    {
        a_if_(!_hasBeenAccessed);
        _hasBeenAccessed = true;
        return const_cast<RangeBasedForLoopSimulator&>(*this)._data.Value->Step();
    }

    bool operator !=(RangeBasedForLoopSimulator other)const
    {
        a_if_(other._data.IsEmpty);
        return _data.Value->IsValid;
    }

};

template<typename T>
class LookAheadIterator final : public Enumerable<T>::Iterator
{
    CtrlRef<typename Enumerable<T>::Iterator> parent;
    Optional<CtrlRef<T>> _current;
public:
    LookAheadIterator(Enumerable<T> const& parent)
        : parent(parent.ToIterator)
    {
        Align();
    }

    p(CtrlRef<T>, current){ return _current.Value; }

    p_function(bool, IsValid) override{ return !_current.IsEmpty; }
    T const Step()override{
        auto const result = _current.Value;
        Align();
        return *result;
    }
private:
    void Align()
    {
        if(parent->IsValid)
            _current = CtrlRef<T>(new T(parent->Step()));
        else
            _current = {};
    }
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
    p_function(bool,IsValid) override {
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
    p_function(bool,IsValid) override{
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
    p_function(bool,IsValid) override{
        return _left->IsValid || _right->IsValid; 
    }

    T const Step()override{ 
        if (_left->IsValid)
            return _left->Step();
        return _right->Step();
    }
};


template<typename T>
class MergeIterator final : public Enumerable<T>::Iterator
{
    LookAheadIterator<T> _left;
    LookAheadIterator<T> _right;
    function<bool(T, T)> isLess;
    bool const removeDuplicates;
public:
    MergeIterator(Enumerable<T> const& left, Enumerable<T> const& right, function<bool(T, T)> isLess, bool removeDuplicates)
        : _left(left)
        , _right(right)
        , isLess(isLess)
        , removeDuplicates(removeDuplicates)
    {
    }
private:
    p_function(bool, IsValid) override{return _left.IsValid || _right.IsValid;}

    T const Step()override
    {
        if(!_left.IsValid)
            return _right.Step();
        if(!_right.IsValid)
            return _left.Step();
        if(isLess(*_left.current,*_right.current))
            return _left.Step();
        if(removeDuplicates && !isLess(*_right.current, *_left.current))
            _left.Step();
        return _right.Step();
    }
};


template<typename T>
class WhereIterator final : public Enumerable<T>::Iterator
{
    LookAheadIterator<T> parent;
    function<bool(T)> selector;
public:
    WhereIterator(Enumerable<T> const& parent, function<bool(T)> selector)
        : parent(parent)
        , selector(selector)
    {
        Align();
    }

    void Align()
    {
        while(parent.IsValid && !selector(*parent.current))
            parent.Step();
    }
protected:
    p_function(bool,IsValid) override{ return parent.IsValid; }
    T const Step()override
    {
        auto result = parent.Step();
        Align();
        return result;
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
    p_function(bool,IsValid) override{ return _parent->IsValid; }
    TResult const Step()override{ return _selector(_parent->Step()); }
};


template<typename T, typename TResult>
class ConvertManyIterator final : public Enumerable<TResult>::Iterator
{
    CtrlRef<typename Enumerable<T>::Iterator> _parent;
    Optional<CtrlRef<T>> _subData;
    Optional<CtrlRef<typename Enumerable<TResult>::Iterator>> _subParent;
public:
    ConvertManyIterator(Enumerable<T> const& parent)
        : _parent(parent.ToIterator){
        Align();
    }
protected:
    p_function(bool,IsValid) override{
        return !_subParent.IsEmpty && _subParent.Value->IsValid;
    }

    TResult const Step()override
    {
        TResult const& result = _subParent.Value->Step();
        Align();
        return result;
    }

private:
    void Align(){
        while (true){
            if(!_subParent.IsEmpty && _subParent.Value->IsValid)
                return;
            if (!_parent->IsValid)
                return;
            _subData = CtrlRef<T>(new T(_parent->Step()));
            _subParent = _subData.Value->ToIterator;
        }
    }
};

template<typename T, typename TOther>
class PairIterator final : public Enumerable<std::pair<T, TOther>>::Iterator{
    typedef std::pair<T, TOther> resultType;
    CtrlRef<typename Enumerable<T>::Iterator> leftIterator;          
    Optional<CtrlRef<T>> leftResult;
    Enumerable<TOther> const&right;
    Optional<CtrlRef<typename Enumerable<TOther>::Iterator>> rightIterator;
public:
    PairIterator(Enumerable<T> const&left, Enumerable<TOther> const&right)
        : leftIterator(left.ToIterator)
        , right(right)
        , leftResult({})
    {
        Align();
    };

protected:
    p_function(bool,IsValid) override{
        a_if_(rightIterator.IsValid);
        return rightIterator.Value->IsValid;
    };

    resultType const Step()override
    {
        a_if_(leftResult.IsValid);
        a_if_(rightIterator.IsValid);
        a_if_(rightIterator.Value->IsValid);
        resultType result(*leftResult.Value, rightIterator.Value->Step());
        Align();
        return result;
    };
private:
    void Align(){
        if (leftIterator->IsValid){
            if (leftResult.IsValid){
                if (!rightIterator.IsValid){ b_; }
                else if(rightIterator.Value->IsValid){ b_; }
                else { b_; }
            }else{
                if (!rightIterator.IsValid){
                    leftResult = new T(leftIterator.Value->Step());
                    rightIterator = right.ToIterator;
                    return;
                }
                else if(rightIterator.Value->IsValid){ b_; }
                else { b_; }
            }
        }else{
            if (leftResult.IsValid){
                if (!rightIterator.IsValid){ b_; }
                else if(rightIterator.Value->IsValid){ b_; }
                else { return; }
            }
            else{
                if (!rightIterator.IsValid){ b_; }
                else if(rightIterator.Value->IsValid){ b_; }
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
    p_function(bool,IsValid) override{ return _parent->IsValid; }
    TResult const Step()override{ return _parent->Step(); }
};

template<typename T>
inline p_implementation(Enumerable<T>, T, Last){
    auto i = ToIterator;
    if(!i->IsValid)
        throw EmptySequenceException();
    while(true){
        auto result = i->Step();
        if(!i->IsValid)
            return result;
    }
}


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
CtrlRef<Enumerable<T>> const Enumerable<T>::Merge(thisType const& right, function<bool(T, T)> isLess, bool removeDuplicates)const
{
    return new Container(new MergeIterator<T>(*this, right, isLess, removeDuplicates));
}

template<typename T>
CtrlRef<Enumerable<T>> const Enumerable<T>::Where(function<bool(T)> selector)const
{
    return new Container(new WhereIterator<T>(*this, selector));
}

template<typename T>
Optional<int> const Enumerable<T>::FirstIndex(function<bool(T)> selector)const
{
    auto i = ToIterator;
    for(auto result = 0; i->IsValid; result++)
        if(selector(i->Step()))
            return result;
    return{};
}

template<typename T>
inline p_implementation(Enumerable<T>, Array<T>, ToArray)
{
    return ToIterator->ToArray;
}

template<typename T>
inline mutable_p_implementation(Enumerable<T>::Iterator, Array<T>const, ToArray){
    auto result = queue<unique_ptr<T>>();
    while (IsValid)
        result.push(unique_ptr<T>(new T(Step())));
    return result;
}


template<typename T>
inline T const Enumerable<T>::Stringify(T const&delimiter)const
{
    auto result = T();
    auto useDelimiter = false;
    for (auto element : *this)
    {
        if (useDelimiter)
            result += delimiter;
        useDelimiter = true;
        result += element;
    }
    return result;
}


template<>
inline String const Enumerable<String>::Stringify(String const&delimiter)const
{
    return String::Stringify(*this, delimiter);
}

template<typename T>
inline p_implementation(Enumerable<T>, int, Count){
    auto result = 0;
    for (auto element : *this)
            result++;
    return result;
}

template<typename T>
inline p_implementation(Enumerable<T>, bool, Any){
    return ToIterator->IsValid;
}

template<typename T>
inline p_implementation(Enumerable<T>, T, First){
    return ToIterator->Step();
}


template<typename T>
Optional<T> const Enumerable<T>::Max() const{
    Optional<T> result;
    for (auto element : *this)
        if(result.IsEmpty || result.Value < element)
            result = element;
    return result;
};

template<typename T>
T const Enumerable<T>::Sum() const{
    T result = 0;
    for(auto element : *this)
        result += element;
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
inline p_implementation(WithId<TBase COMMA TRealm>, String, DumpHeader){
    auto objectId = HWLib::Dump(ObjectId);
    return base_p_name(DumpHeader) + ".Id" + objectId;
};


template <typename T>
inline String const HWLib::DumpShort(Ref<T> const&target){
    return "*{ " + HWLib::DumpShort(*target) + " }";
}

template <typename T>
inline String const HWLib::Dump(CtrlRef<T> const&target){
    return "*{ " + HWLib::Dump(*target) + " }";
}

template <typename T>
inline String const HWLib::Dump(WeakRef<T> const&target){
    return "*{ " + HWLib::Dump(*target) + " }";
}

template <typename T>
inline String const HWLib::Dump(Ref<T> const&target){
    return "*{ " + HWLib::Dump(*target) + " }";
}

template <typename T>
inline String const HWLib::Dump(Array<T> const&target)
{
    return "Array["+ HWLib::Dump(target.Count)+ "]"
        + DumpList(DumpData(target));
}

inline String const HWLib::DumpList(Array<String> const&target)
{
    return String::Surround("{", target, "}");
}

template <typename T>
inline Array<String> const HWLib::DumpData(Array<T> const&target){
    auto index = 0;
    return target
        .Select<String>([&](T const&element){
        return "[" + Dump(index++) + "] " + HWLib::Dump(element);
    })
        ->ToArray;
}

template <typename T1, typename T2>
inline String const HWLib::Dump(pair<T1, T2> const&target){
    auto dataResult = _({
        "first = " + HWLib::Dump(target.first),
        "second = " + HWLib::Dump(target.second),
    });
    return String::Surround(
        "{", 
        dataResult, 
        "}");
}


template<typename T>
CtrlRef<Enumerable<T>> const Enumerable<T>::Sort(function<bool(T, T)>isLeftSmaller) const{
    struct sorter{
        function<bool(T, T)> const isLeftSmaller;
        sorter(function<bool(T, T)>isLeftSmaller) :isLeftSmaller(isLeftSmaller){}
        bool const operator()(T const&left, T const&right)const{ return isLeftSmaller(left, right); };
    };
    Array<T> result = ToArray;
    T * dataForSort = const_cast<T*>(result.RawData);
    std::sort<T*, sorter>(dataForSort, dataForSort + result.Count, sorter(isLeftSmaller));
    return new Array<T>(result);
}

class NumbersIterator final : public Enumerable<int>::Iterator
{
    using baseType = Enumerable<int>::Iterator;
    using thisType = NumbersIterator;

    int const count;
    int index;
public:
    explicit NumbersIterator(int count)
        : count(count)
        , index(0)
    {}
private:
    p_function(bool, IsValid) override{ return index < count; }
    int const Step() override{ return index++; }
};

inline CtrlRef<Enumerable<int>> const HWLib::Numbers(size_t count)
{
    return new Enumerable<int>::Container(new NumbersIterator(count));
}


//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")
