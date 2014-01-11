#pragma once

#include "Enumerable.h"
#include "BreakHandling.h"

namespace HWLib
{
    template<typename T> class Array : public Enumerable<T>, public Enumerable<T>::WithCount
    {
        using baseType = Enumerable<T>;
        using thisType = Array<T>;

        int const _count;
        T const *const _data;
    public:
        Array() : _count(0), _data(0){}

        Array(Array<T> const&& other)
            : _count(other.Count)
            , _data(other._data)
        {};

        Array(int count, function<T(int)> creator)
            : _count(count)
            , _data(new T[count])
        {
            auto data = const_cast<T*>(_data);
            for (auto index = 0; index < count; index++)
                data[index] = creator(index);
        }

        Array(Array<T> const&other)
            : thisType(other.Count, [=](int index){return other[index]; })
        {
        }

        ~Array(){ delete _data; }
        
        DefaultAssignmentOperator;

        p(int, Count){ return _count; }
        p(T const*, RawData){ return _data; }

        T& operator[](int Index){ return _data[Index]; }
        T const& operator[](int Index)const{ return _data[Index]; }
        thisType const operator+(thisType const& other)const
        {
            return Array<T>(Count + other.Count,
                [=]
            (int index)->T
            {
                if (index < Count)
                    return (*this)[index];
                index -= Count;
                return other[index];
            }
            );
        }

        bool const Compare(Array<T> const& other)const;
    private:
        mutable_p_function(Var<Iterator>, ToIterator) const override;

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
