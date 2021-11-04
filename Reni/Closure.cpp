#include "Import.h"
#include "Closure.h"

#include "External.h"
#include "ReplaceVisitor.h"
#include "../HWLib/DumpToString.h"


using namespace Reni;
static bool Trace = true;

External::This const External::This::Instance;
External::Args const External::Args::Instance;
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

Closure const External::Replace(ReplaceVisitor const&arg) const
{
    md(arg);
    b_;
    return{};
}

bool External::This::IsProvided(ReplaceVisitor const& arg) const
{
    return !arg.GetResults(Instance).IsEmpty;
}

Closure const External::This::Replace(ReplaceVisitor const& arg) const
{
    return arg.GetClosure(Instance);
}

bool External::Args::IsProvided(ReplaceVisitor const& arg) const
{
    return !arg.GetResults(Instance).IsEmpty;
}

Closure const External::Args::Replace(ReplaceVisitor const& arg) const
{
    return arg.GetClosure(Instance);
}


bool External::Function::Arg::IsProvided(ReplaceVisitor const& arg) const
{
    return arg.GetResults(Instance).IsEmpty;
}                                                                 

Closure const External::Function::Arg::Replace(ReplaceVisitor const& arg) const
{
    return arg.GetClosure(Instance);
}


Closure::Closure(External const& item)
    : data({&item.thisRef})
{
    SetDumpString();
}

Closure const Closure::Aggregate(CtrlRef<Enumerable<Closure>> const& other)
{
    return Closure
        (
            other
            ->Aggregate<Closure>([&](Closure result, Closure next)
                {
                    return result + next;
                })
        );
}

Optional<Closure> const Closure::Replace(ReplaceVisitor const& arg) const
{
    return Aggregate
        (
            data
            .Select<Closure>([&](WeakRef<External> const&item)
                {
                    return item->Replace(arg);
                })
        );
}

Closure::Closure(Array<WeakRef<External>> const& other)
    : data(other)
{
    SetDumpString();
}

Closure::Closure(Closure const& other)
    : data(other.data)
{
    SetDumpString();
}

bool const Closure::operator==(Closure const& other) const
{
    return data.Compare(other.data);
}

bool const Closure::operator==(External const& other) const
{
    return data.Count == 1 && *data[0] == other;
}

bool const Closure::Contains(External const& item) const
{
    return data
        .Where
        (
            [&]
            (WeakRef<External> const& dataItem)
            {
                return *dataItem == item;
            }
        )
        ->Any;
}

p_implementation(Closure, Array<string>, DumpData)
{
    return data
        .Select<string>
        (
            [&]
            (WeakRef<External> item)
            {
                return item->DumpShort;
            }
        )
        ->ToArray;
}


Closure const Closure::operator+(Closure const& other) const
{
    auto result = data.Merge
        (
            other.data,
            [](WeakRef<External> left, WeakRef<External> right)
            {
                return *left < *right;
            },
            true
        );
    return Closure(result->ToArray);
}

Closure const Closure::operator-(Closure const& other) const
{
    auto result = data
        .Where
        (
            [&](WeakRef<External> item)
            {
                return !other.Contains(*item);
            }
        );
    return Closure(result->ToArray);
}