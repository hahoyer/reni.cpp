#include "Import.h"
#include "String.h"

#include "DumpToString.h"
#include "Storage.h"
#include "DumpMacros.h"

using namespace HWLib;

String::String(char const* data)
: _data(::strlen(data), [=](int i){return data[i]; })
{
}

String::String(IEnumerable<char> const& other)
: _data(other.ToArray)
{
}

String const String::FilePosition(String const&fileName, int line, int column, String const&flag)
{
    return fileName
        + "("
        + DumpToString(line)
        + (column ? String(",") + DumpToString(column - 1) : "")
        + "): "
        + flag
        + ": ";
};

bool const String::operator== (String const& other)const{ return _data.Compare(other._data); }

String const String::Indent(bool isLineStart, int count, String const &tabString)const
{
    auto effectiveTabString = tabString*count;
    return (isLineStart ? effectiveTabString : "") + Replace("\n", "\n" + effectiveTabString);
}


String const String::Replace(String const &oldValue, String const&newValue)const
{
    return Split(oldValue).Stringify(newValue);
}

