#pragma once

#include "Array.h"
#include "DefaultAssignmentOperator.h"
#include "Properties.h"

; namespace HWLib
{
    class String
    {
        using thisType = String;
        Ref<Array<char>> const _data;
    public:
        String();
        String(char const* data);
        String(Enumerable<char> const& other);
        String(Array<char> const& other);

        DefaultAssignmentOperator;

        p(int, Count){ return _data->Count; }
        p(Array<char>, ToArray){ return *_data + Array<char>(1, [](int){return static_cast<char>(0); }); }

        bool           const BeginsWith(String const &target, int start = 0)const;
        OptRef<int>     const Find(String const &target, int start = 0)const;
        String           const Indent(bool          isLineStart = false, int count = 1, String const &tabString = "    ")const;
        String            const operator+  (String const& other)const;
        String             const operator*  (int           count)const;
        char                const operator[]  (int           count)const;
        bool                 const operator== (String const& other)const;
        String                const Part(int start, int length)const;
        String                 const Part(int start)const;
        String                  const Replace(String const& oldValue, String const&newValue)const;
        Ref<Enumerable<String> > const Split(String const& delimiter)const;

        static String const FilePosition(String const&fileName, int line, int column, String const&flag);
        void OutputDebug() const;

    };

    static String const operator+ (char const*left, String const& right){ return String(left) + right; };

}

