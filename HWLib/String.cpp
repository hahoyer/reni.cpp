#include "Import.h"
#include "String.h"

#include "Storage.h"

using namespace HWLib;

class String::local
{
public:
    char const* _data;
    int const _length;

    local(char const* data);
    local(String::local const& other);
};

String::String(char const* data)
: _data(HeapAllocate local(data))
{
}

String::String(String const& other)
: _data(HeapAllocate local(other._data))
{
}

