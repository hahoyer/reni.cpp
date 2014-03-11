#include "Import.h"
#include "String.h"

#include "Common.h"
#include "DumpMacros.h"
#include "DumpToString.h"
#include "File.h"
#include "Optional.h"
#include "TemplateInstances.h"

using namespace HWLib;

String::String(char const data)
: _data(1,data)
{
}

String::String(char const* data)
: _data(data)
{
}

String::String(int count, char const* data)
: _data(data,count)
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
        + HWLib::Dump(line)
        + (column ? String(",") + HWLib::Dump(column - 1) : "")
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


p_implementation(String, char*, RawDataCopy)
{
    auto result = new char[Count + 1];
    auto data = _data.c_str();
    ::memcpy(result, data, Count);
    result[Count] = '\0'; // don't forget the terminating 0
    return result;
}


p_implementation(String, std::size_t, HashCode){
    return std::hash<std::string>()(_data);
};


String const CharQuote(char const c)
{
    switch (c)
    {
    case '\\':
        return "\\\\";
    case '"':
        return "\\\"";
    case '\n':
        return "\\n";
    case '\t':
        return "\\t";
    case '\r':
        return "\\r";
    case '\f':
        return "\\f";
    }

    static char const* hex = "0123456789ABCDEF";
    if (c < 16)
        return "\\0x0" + String(hex[c]);
    return String(c);
}

p_implementation(String, String, Quote)
{
    return  
        ToArray
        .Aggregate<String>
        (
            "\"", 
            [=](String head, char const next){return head + CharQuote(next); }
        )
        + "\"";
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
        result += *this;
    return result;
}

String const String::CastLeft(int count, char padChar)const
{
    if (count == Count)
        return *this;
    if (count < Count)
        return Part(Count - count);
    return String(padChar) * (count - Count) + *this;
}

String const String::CastRight(int count, char padChar)const
{
    if (count == Count)
        return *this;
    if (count < Count)
        return Part(0,count);
    return *this + String(padChar) * (count - Count);
}

char const String::operator[] (int index)const{ return _data[index]; }

String const String::Indent(bool isLineStart, int count, String const &tabString)const
{
    if (count == 0)
        return *this;

    auto effectiveTabString = tabString*count;
    return (isLineStart ? effectiveTabString : "") + Replace("\n", "\n" + effectiveTabString);
}

bool const String::Contains(String const &target, int start)const{
    return Find(target, start).IsValid;
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

bool const String::EndsWith(String const &target)const
{
    auto delta = Count - target.Count;
    if (delta < 0)
        return false;
    if (delta == 0)
        return *this == target;
    return Part(delta) == target;
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
    override_p_function(bool, IsValid){
        return _index < _parent.Count; 
    }
    
    String const Step() override
    {
        auto start = _index;
        auto newEnd = _parent.Find(_delimiter, start);
        if (!newEnd.IsValid)
            newEnd = _parent.Count;
        _index = newEnd + _delimiter.Count;
        return _parent.Part(start, newEnd - start);
    }

};

CtrlRef<Enumerable<String>> const String::Split(String const& delimiter)const
{ 
    a_is(delimiter, !=, "");
    return new Enumerable<String>::Container(new SplitIterator(*this, delimiter));
}

String const String::Convert(int value)
{
    return std::to_string(value);
};


String const String::Convert(unsigned __int32 value)
{
    return std::to_string(value);
};


String const String::Convert(unsigned __int64 value)
{
    return std::to_string(value);
};


String const String::Convert(bool value)
{
    return value? String("true"):"false";
};


String const String::Surround(String const&left, Array<String> const&list, String const&right, int maxCount)
{
    switch (list.Count)
    {
    case 0:
        return left + right;
    case 1:
        if (!list[0].Contains('\n') && list[0].Count < maxCount)
            return left + " " + list[0] +" " + right;
    };

    return left + ("\n" + list.Stringify("\n")).Indent() + "\n" + right;
};
