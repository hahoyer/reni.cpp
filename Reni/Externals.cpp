#include "Import.h"
#include "Externals.h"

#include "External.h"
#include "ReplaceVisitor.h"
#include "../HWLib/DumpToString.h"


using namespace Reni;
static bool Trace = true;

External::This const External::This::Instance;
External::Arg const External::Arg::Instance;
External::Function::Arg const External::Function::Arg::Instance;
External::Function::NewValue const External::Function::NewValue::Instance;

bool const External::operator<(External const& other) const
{
    return ObjectId < other.ObjectId;
}

bool const External::operator==(External const& other) const
{
    return ObjectId ==other.ObjectId;
}

Externals const External::Replace(ReplaceVisitor const&arg) const
{
    md(arg);
    b_;
    return{};
}

bool const External::This::IsProvided(ReplaceVisitor const& arg) const
{
    return !arg.GetResults(External::This::Instance).IsEmpty;
}

Externals const External::This::Replace(ReplaceVisitor const& arg) const
{
    return arg.GetExts(External::This::Instance);
}

bool const External::Arg::IsProvided(ReplaceVisitor const& arg) const
{
    return !arg.GetResults(External::Arg::Instance).IsEmpty;
}

Externals const External::Arg::Replace(ReplaceVisitor const& arg) const
{
    return arg.GetExts(External::Arg::Instance);
}


bool const External::Function::Arg::IsProvided(ReplaceVisitor const& arg) const
{
    return arg.GetResults(External::Function::Arg::Instance).IsEmpty;
}                                                                 

Externals const External::Function::Arg::Replace(ReplaceVisitor const& arg) const
{
    return arg.GetExts(External::Function::Arg::Instance);
}


Externals::Externals(External const& item)
    : data({&item.thisRef})
{
    SetDumpString();
}

Externals const Externals::Aggregate(CtrlRef<Enumerable<Externals>> const& other)
{
    return Externals
        (
            other
            ->Aggregate<Externals>([&](Externals result, Externals next)
                {
                    return result + next;
                })
        );
}

Optional<Externals> const Externals::Replace(ReplaceVisitor const& arg) const
{
    return Aggregate
        (
            data
            .Select<Externals>([&](WeakRef<External> const&item)
                {
                    return item->Replace(arg);
                })
        );
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

bool const Externals::operator==(External const& other) const
{
    return data.Count == 1 && *data[0] == other;
}

p_implementation(Externals, Array<String>, DumpData)
{
    return data
        .Select<String>
        (
            [&]
            (WeakRef<External> item)
            {
                return item->DumpShort;
            }
        )
        ->ToArray;
}


Externals const Externals::operator+(Externals const& other) const
{
    auto result = data.Merge(other.data, [](WeakRef<External> left, WeakRef<External> right){return *left < *right; });
    return Externals(result->ToArray);
}