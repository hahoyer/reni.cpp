#include "Import.h"
#include "Externals.h"

#include "External.h"
#include "../HWLib/DumpToString.h"


using namespace Reni;
static bool Trace = true;

External const External::This;
External const External::Arg;

bool const External::operator<(External const& other) const
{
    return ObjectId < other.ObjectId;
}

p_implementation(External, String, DumpHeader){
    if(this == &This)
        return "This";
    if(this == &Arg)
        return "Arg";
    return "?";
};


Externals::Externals(External const& item)
    : data({&item.thisRef})
{
    SetDumpString();
}

Externals::Externals(Array<WeakRef<External>> const& other)
    : data(other)
{
    SetDumpString();
}

bool const Externals::operator==(Externals const& other) const
{
    return data.Compare(other.data);
}

p_implementation(Externals, Array<String>, DumpData)
{
    return HWLib::DumpData(data);
}


Externals const Externals::operator+(Externals const& other) const
{
    auto result = data.Merge(other.data, [](WeakRef<External> left, WeakRef<External> right){return *left < *right; });
    return  result->ToArray;
}