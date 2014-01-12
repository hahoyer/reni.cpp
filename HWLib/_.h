#pragma once

#include "Properties.h"

#undef _

namespace HWLib
{
    template<typename T>
    class Box_
    {
        T & _data;
    public:
        Box_(T & data) : _data(data){}
        p_definition(T&, Data);
        mutable_p_function(T&, Data){ return _data; }

        void Delete() { delete &_data; };
        template<typename TDestination>
        TDestination Convert(){ return convert<TDestination>(_data); };

    };

    template<typename TPointer>
    class Box_<TPointer*>
    {
        using T = TPointer*;

        T & _data;
    public:
        Box_(T & data) : _data(data){}
        p_definition(T&, Data);
        mutable_p_function(T&, Data){ return _data; }

        void SaveDelete() { assert(_data); delete _data; _data = null; };
        void SmartDelete() { if (_data) delete _data; _data = null; };
        void SaveDeleteArray() { assert(_data); delete[] _data; _data = null; };
        void SmartDeleteArray() { if (_data) delete[] _data; _data = null; };
    };

    template<typename TPointer>
    class Box_<TPointer*const>
    {
        using T = TPointer*const;

        T & _data;
    public:
        Box_(T & data) : _data(data){}
        p_definition(T&, Data);
        mutable_p_function(T&, Data){ return _data; }

        void SaveDelete() { assert(_data); delete _data; };
        void SmartDelete() { if (_data) delete _data; };
        void SaveDeleteArray() { assert(_data); delete[] _data; };
        void SmartDeleteArray() { if (_data) delete[] _data; };
    };

    template<typename T>
    Box_<T> _(T&data){ return data; }
}
