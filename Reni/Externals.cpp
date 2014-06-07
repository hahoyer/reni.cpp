#include "Import.h"
#include "Externals.h"

#include "External.h"
#include "ReplaceVisitor.h"
#include "../HWLib/DumpToString.h"


using namespace Reni;
static bool Trace = true;

External::This const External::This::Instance;
External::Arg const External::Arg::Instance;

bool const External::operator<(External const& other) const
{
    return ObjectId < other.ObjectId;
}

bool const External::This::IsProvided(ReplaceVisitor const& arg) const
{
    return arg.hasThis;
}

bool const External::Arg::IsProvided(ReplaceVisitor const& arg) const
{
    return arg.hasArg;
}

p_implementation(External, String, DumpHeader){
    if(this == &This::Instance)
        return "This";
    if(this == &Arg::Instance)
        return "Arg";
    return "?";
};


Externals::Externals(External const& item)
    : data({&item.thisRef})
{
    SetDumpString();
}

Optional<Externals> const Externals::Replace(ReplaceVisitor const& arg) const
{
    return data
        .Where([&](WeakRef<External> const&item){return !item->IsProvided(arg); })
        ->ToArray;
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