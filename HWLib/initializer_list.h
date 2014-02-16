#pragma once
#define _INITIALIZER_LIST_

namespace std{
    template<class T>
    class initializer_list 
    {
    public:
        typedef T value_type;
        initializer_list() : _begin(0), _end(0){ }
        initializer_list(const T *begin, const T *end): _begin(begin), _end(end){}
        const T *begin() const { return _begin; }
        const T *end() const { return _end; }
        size_t size() const { return _end - _begin; }
        T const operator[](int index)const{ return _begin[index]; }
    private:
        const T *_begin;
        const T *_end;
    };
}

template<class T>
using initializer_list = std::initializer_list<T>;

template<class T> inline const T *begin(initializer_list<T> data) { return data.begin(); }
template<class T> inline const T *end(initializer_list<T> data){ return data.end(); }

