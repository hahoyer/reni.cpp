#pragma once
//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")

#include "Properties.h"

#undef _

namespace HWLib
{
    class String; 

    template<typename T>
    class Box_
    {
        using thisType = Box_<T>;
        T & _data;
    public:
        Box_(T & data) : _data(data){}
        Box_(thisType const& data) = delete;

        p_definition(T&, Data);
        mutable_p_function(T&, Data){ return _data; }

        void Delete() { delete &_data; };
        template<typename TDestination>
        TDestination Convert(){ return convert<TDestination>(_data); };
        String const DumpToString()const;
    };

    template<typename TPointer>
    class Box_<TPointer*>
    {
        using thisType = Box_<TPointer*>;
        using T = TPointer*;

        T & _data;
    public:
        Box_(T & data) : _data(data){}
        Box_(thisType const& data) = delete;

        p_definition(T&, Data);
        mutable_p_function(T&, Data){ return _data; }

        void SaveDelete() { a_if_(_data); delete _data; _data = null; };
        void SmartDelete() { if (_data) delete _data; _data = null; };
        void SaveDeleteArray() { a_if_(_data); delete[] _data; _data = null; };
        void SmartDeleteArray() { if (_data) delete[] _data; _data = null; };
    };

    template<typename TPointer>
    class Box_<TPointer*const>
    {
        using thisType = Box_<TPointer*const>;
        using T = TPointer*const;

        T & _data;
    public:
        Box_(T & data) : _data(data){}
        Box_(thisType const& data) = delete;

        thisType& operator=(thisType const&x) = delete;

        p_definition(T&, Data);
        mutable_p_function(T&, Data){ return _data; }

        void SaveDelete() { a_if_(_data); delete _data; };
        void SmartDelete() { if (_data) delete _data; };
        void SaveDeleteArray() { a_if_(_data); delete[] _data; };
        void SmartDeleteArray() { if (_data) delete[] _data; };
    };

    template<typename T>
    Box_<T> _(T&data){ return data; }

}

//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")
