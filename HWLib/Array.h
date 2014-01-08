#pragma once

#include "Enumerable.h"

namespace HWLib
{
    template<typename T> class Array : public IEnumerable<T>
    {
        T*const _data;
    public:
        Array(int length, function<T(int)> creator)
            : _data(new T[length])
        {
            for (auto i = 0; i < length; i++)
                _data[i] = creator(i);
        }
        Array(Array<T> const&other)
            : _data(new T[other.Length])
        {
            for (auto i = 0; i < other.Length; i++)
                _data[i] = other[i];
        }

        ~Array(){ delete[] _data; }

        DefaultAssignmentOperator(Array);

        p(T const*, RawData){ return _data; }

        T& operator[](int Index){ return _data[Index]; }
        T const& operator[](int Index)const{ return _data[Index]; }

        bool const Compare(Array<T> const& other)const;

        virtual Position begin()const override;
        virtual Position const end()const override;
    };

        template<typename T>
        bool const Array<T>::Compare(Array<T> const& other)const
        {
            if (Length != other.Length)
                return false;
            for (auto i = 0; i < Length; i++)
            if ((*this)[i] != other[i])
                return false;
            return true;
        };


}
