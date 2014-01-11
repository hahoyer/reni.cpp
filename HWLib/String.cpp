#include "Import.h"
#include "String.h"

#include "DumpMacros.h"
#include "DumpToString.h"
#include "Ref.h"
#include "Storage.h"

using namespace HWLib;

String::String(char const* data)
: _data(*new Array<char>(::strlen(data), [=](int i){return data[i]; }))
{
}

String::String(Enumerable<char> const& other)
: _data(*new Array<char>(other.ToArray))
{
}

String::String()
: _data(*new Array<char>())
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

bool const String::operator== (String const& other)const{ return _data->Compare(*other._data); }

String const String::operator+ (String const& other)const{
    Array<char> const&a = *_data;
    Array<char> const&b = *other._data;
    Array<char> const r = a + b;
    return String(r);
}

String const String::operator* (int count)const
{
    String result;
    for (auto i = 0; i < count; i++)
        result = result + *this;
    return result;
}

char const String::operator[] (int index)const{ return (*_data)[index]; }

String const String::Indent(bool isLineStart, int count, String const &tabString)const
{
    auto effectiveTabString = tabString*count;
    return (isLineStart ? effectiveTabString : "") + Replace("\n", "\n" + effectiveTabString);
}

OptRef<int> const String::Find(String const &target, int start)const
{
    for (auto end = Count - target.Count; start < end; start++)
    if (BeginsWith(target, start))
        return *new int(start);
    return null();

}

bool const String::BeginsWith(String const &target, int start)const
{
    for (auto i = 0; i < target.Count; i++)
    if ((*this)[start + i] != target[i])
        return false;
    return true;
}


String const String::Replace(String const &oldValue, String const&newValue)const
{
    return Split(oldValue)->Stringify(newValue);
}

String const String::Part(int start)const{ return String(*_data->Skip(start)); }
String const String::Part(int start, int count)const{ return String(*_data->Skip(start)->Take(count)); }

class SplitEnumerable final : public Enumerable<String>
{
    using baseType = Enumerable<String>;
    using thisType = SplitEnumerable;

    String const _target;
    String const _delimiter;

public:
    SplitEnumerable(String const& target, String const& delimiter) : _target(target), _delimiter(delimiter){}

private:
    mutable_p_function(Var<Iterator>, ToIterator) const override;


};

Ref<Enumerable<String>> const String::Split(String const& delimiter)const{ return *new SplitEnumerable(*this, delimiter); }

