#pragma once

#include "Array.h"
#include "DefaultAssignmentOperator.h"
#include "Properties.h"

namespace HWLib
{
    template<typename T>
    class Nullable
    {
        T const* _value;
    public: 
        Nullable() :_value(0){}
        Nullable(T const& value) :_value(HeapAllocate T(value)){}
        Nullable(Nullable<T> const& value) :_value(value.IsNull ? 0 : HeapAllocate T(value.Data)){}

        DefaultAssignmentOperator(Nullable<T>);

        p(bool, IsValid){ return _value; }
        p(bool, IsNull){ return !_value; }
        p(T, Value){ assert(_value); return *_value; }
    };



    class String
    {
        Array<char> _data;
    public:
        String();
        String(char const* data);
        String(IEnumerable<char> const& other);

        DefaultAssignmentOperator(String);

        p(int, Length){ return _data.Length; }
        p(Array<char>, ToArray){ return _data; }

        String const   Indent     (bool          isLineStart = false, int count = 1, String const &tabString = "    ")const;
        String const    operator+  (String const& other)const;
        String const     operator*  (int           count)const;
        bool const        operator== (String const& other)const;
        String const       Replace  (String const& oldValue,      String const&newValue)const;
        IEnumerable<String> Split  (String const& delimiter)const;

        static String const FilePosition(String const&fileName, int line, int column, String const&flag);
        void OutputDebug() const;

    };

    static String const operator+ (char const*left, String const& right){ return String(left) + right; }

}

