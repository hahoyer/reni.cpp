#pragma once
//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")

#include "_.h"
#include "BreakHandling.h"
#include "Common.h"
#include "Enumerable.h"

namespace HWLib
{
    template<typename T> class Array : public Enumerable<T>
    {
        using baseType = Enumerable<T>;
        using thisType = Array<T>;

        int const _count;
        T * const _data;
    public:
        Array() : _count(0), _data(0){ }

        Array(Array<T> && other)
            : _count(other.Count)
            , _data(other._data)
        {
            const_cast<T const *&> (other._data) = nullptr;
        };

        Array(int count, function<T(int)> creator)
            : _count(count)
            , _data(reinterpret_cast<T * const>(new __int8[sizeof(T)*count]))
        {
            auto data = const_cast<remove_const<T>::type*>(_data);
            for (auto index = 0; index < count; index++)
                new (data + index) T(creator(index));
        }

        Array(Array<T> const&other)
            : thisType(other.Count, [&](int index){return other[index]; })
        {
        }

        ~Array(){ _(_data).SmartDeleteArray();}

        DefaultAssignmentOperator;

        p(int, Count){ return _count; }
        p(T const*, RawData){ return _data; }

        T const& operator[](int Index)const{ return _data[Index]; }
        T& operator[](int Index){ return _data[Index]; }
        thisType const operator+(thisType const& other)const{ return baseType::operator+(other)->ToArray; }

        bool const Compare(Array<T> const& other)const;
    private:
        class LocalIterator final : public Iterator
        {
            using baseType = Iterator;
            using thisType = LocalIterator;
            Array<T> const& _parent;
            int _index;
        public:
            LocalIterator(Array<T> const& parent)
                : _parent(parent)
                , _index(0)
            {
            }

            p_function(bool, IsValid) override{ return _index >= 0 && _index < _parent.Count; }
            void operator++(int) override{ _index++; }
            T const operator*()const override{ return _parent[_index]; }
            p_function(Ref<Iterator>, Clone) override{ return new thisType(*this); }

            void operator=(LocalIterator const&) = delete;
        };

        mutable_p_function(Ref<Iterator>, ToIterator) const override{ return new LocalIterator(*this); }

    };

        template<typename T>
        bool const Array<T>::Compare(Array<T> const& other)const
        {
            if (Count != other.Count)
                return false;
            for (auto index = 0; index < Count; index++)
            if ((*this)[index] != other[index])
                return false;
            return true;
        };

}
//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")
