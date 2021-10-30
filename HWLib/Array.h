#pragma once
//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")

#include "_.h"
#include "Common.h"
#include "Enumerable.h"
#include <vector>
#include <queue>

namespace HWLib
{
    template<typename T> 
    class Array final : public Enumerable<T>
    {
        using baseType = Enumerable<T>;
        using thisType = Array<T>;

        size_t const _count;
        T * const _data;
    public:
        Array() : _count(0), _data(0){ }

        Array(Array<T> && other)
            : _count(other.Count)
            , _data(other._data)
        {
            const_cast<T const *&> (other._data) = nullptr;
            const_cast<int&> (other._count) = 0;
        };

        Array(Array<T> const& other)
            : _count(other.Count)
            , _data(reinterpret_cast<T * const>(new __int8[sizeof(T)*other.Count]))
        {
            auto data = const_cast<remove_const<T>::type*>(_data);
            for (auto index = 0; index < Count; index++)
                new (data + index) T(other[index]);
        };

        Array(std::queue<std::unique_ptr<T>> & other)
            : _count(other.size())
            , _data(reinterpret_cast<T * const>(new __int8[sizeof(T)*other.size()]))
        {
            auto data = const_cast<remove_const<T>::type*>(_data);
            for(auto index = 0; index < _count; index++)
            {
                new (data + index) T(*other.front());
                other.pop();
            }
        }

        explicit Array(std::vector<T> const&other)
            : _count(other.size())
            , _data(reinterpret_cast<T * const>(new __int8[sizeof(T)*other.size()]))
        {
            auto data = const_cast<remove_const<T>::type*>(_data);
            auto index = 0;
            for(auto const& element : other)
            {
                new (data + index) T(element);
                index++;
            }
        }

        Array(std::initializer_list<T> const&other)
            : _count(other.size())
            , _data(reinterpret_cast<T * const>(new __int8[sizeof(T)*other.size()]))
        {
            auto data = const_cast<remove_const<T>::type*>(_data);
            auto index = 0;
            for (auto element : other)
            {
                new (data + index) T(element);
                index++;
            }
        }

        ~Array()
        {
            for (auto i = 0; i < _count; i++)
                _data[i].~T();
            if (_data)
                delete[] reinterpret_cast<__int8 const*>(_data);
        }

        DefaultAssignmentOperator;

        p(size_t
          
          
          
          , Count){ return _count; }
        p(T const*, RawData){ return _data; }

        T const& operator[](int Index)const{ return _data[Index]; }
        T& operator[](int Index){ return _data[Index]; }
        thisType const operator+(thisType const& other)const{ return baseType::operator+(other)->ToArray; }
        thisType const operator+(T const& other)const{ return (*this) + _({ other }); }
        void operator+=(T const& other){ *this = *this + other; }
        void operator+=(thisType const& other){ *this = *this + other; }

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
            T const Step() override{ return _parent[_index++]; }

            void operator=(LocalIterator const&) = delete;
        };

        mutable_p_function(CtrlRef<Iterator>, ToIterator) const override{ return new LocalIterator(*this); }

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


        template<typename T>
        Array<T> const _(std::initializer_list<T> const&data){ return data; };
}
//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")
