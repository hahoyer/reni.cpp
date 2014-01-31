#include "Import.h"
#include "String.h"

#include "Common.h"
#include "DumpMacros.h"
#include "DumpToString.h"
#include "Ref.h"
#include "TemplateInstances.h"

using namespace HWLib;

String::String(char const* data)
: _data(data)
{
}

String::String(std::string const& data)
: _data(data)
{
}

String::String(Array<char const> const& other)
: _data(other.RawData, other.Count)
{
}

String::String()
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

p_implementation(String, Array<char const>, ToArray)
{
    char const* d = _data.c_str();
    return Array<char const>(Count, [&](int i){return d[i]; }); 
}

p_implementation(String, char const*, RawData)
{
    return _data.c_str();
}

bool const String::operator== (String const& other)const{ return _data == (other._data); }

String const String::operator+ (String const& other)const
{
    auto result = _data + other._data;
    return result;
}

String const String::operator* (int count)const
{
    String result;
    for (auto i = 0; i < count; i++)
        result = result + *this;
    return result;
}

char const String::operator[] (int index)const{ return _data[index]; }

String const String::Indent(bool isLineStart, int count, String const &tabString)const
{
    if (count == 0)
        return *this;

    auto effectiveTabString = tabString*count;
    return (isLineStart ? effectiveTabString : "") + Replace("\n", "\n" + effectiveTabString);
}

Optional<int> const String::Find(String const &target, int start)const
{
    for (auto end = Count - target.Count; start < end; start++)
    if (BeginsWith(target, start))
        return Optional<int>(start);
    return empty;
}

bool const String::Contains(char const &target, int start)const
{
    for (; start < Count; start++)
        if ((*this)[start] == target)
            return true;
    return false;
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
    auto split = Split(oldValue);
    return split->Stringify(newValue);
}

String const String::Part(int start)const{ return ToArray.Skip(start)->ToArray; }
String const String::Part(int start, int count)const
{ 
    return ToArray.Skip(start)->Take(count)->ToArray;
}

class SplitIterator final : public Enumerable<String>::Iterator
{
    using baseType = Enumerable<String>::Iterator;
    using thisType = SplitIterator;
    using parentType = Enumerable<char>::Iterator;
    String const _parent;
    String const _delimiter;
    int _index;

public:
    SplitIterator(String const& parent, String const& delimiter)
        : _parent(parent)
        , _delimiter(delimiter)
        , _index(0)
    {
    }

protected:
    p_function(bool, IsValid) override
    {
        return _index + _delimiter.Count <= _parent.Count; 
    }
    
    p_function(Ref<Iterator>, Clone) override{ return new thisType(*this); }

    void operator++(int) override
    {
        auto newEnd = _parent.Find(_delimiter, _index);
        if (newEnd.IsValid)
            _index = newEnd + _delimiter.Count;
        else
            _index = _parent.Count;
    }
    
    String const operator*()const override
    {
        auto newEnd = _parent.Find(_delimiter, _index);
        if (newEnd.IsValid)
            return _parent.Part(_index, newEnd - _index);
        return _parent.Part(_index);
    }

};

Ref<Enumerable<String>> const String::Split(String const& delimiter)const
{ 
    assert(delimiter != "");
    return new Enumerable<String>::Container(new SplitIterator(*this, delimiter));
}

String const String::Convert(int const&value)
{
    return std::to_string(value);
};
