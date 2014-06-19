#include "Import.h"
#include "Global.h"

#include "FunctionCallResultCache.h"

using namespace Reni;
static bool Trace = true;

struct Global::internal
{
    mutable Array<FunctionCallResultCache const*> functionIndexCache;
    p(Array<Function>, functions)
    {
        auto newEntries = false;
        do
        {
            for(auto index = 0; index < functionIndexCache.Count; index++)
            {
                FunctionCallResultCache const&resultCache = *functionIndexCache[index];
                auto& item = resultCache.function;
                newEntries = item.Ensure(resultCache);
            }
        } while(newEntries);

        return functionIndexCache.Select<Function>
            (
                [&]
                (FunctionCallResultCache const*item)
                {
                    return item->function;
                }
            )
            ->ToArray;
    };
};

struct Global::Function::Xetter final : public DumpableObject
{
    using baseType = DumpableObject; 
    using thisType = Xetter;
    mutable bool isRequired;
    mutable Optional<Ref<CodeItem>> code;
    
    Xetter(): isRequired(false){}
    
    bool Ensure(function<Ref<CodeItem>()> getCode) const;
private:
    p_function(Array<String>, DumpData) override{ return{nd(isRequired),nd(code)}; }
};

Global::Global()
    : bitType(this)
      , voidType(this)
      , _internal(new internal)
{
}

int const Global::FunctionIndex(FunctionCallResultCache const& target) const
{
    auto result =_internal
        ->functionIndexCache
        .FirstIndex([&](FunctionCallResultCache const*item)
            {
                return item == &target;
            });
    if(result.IsValid)
        return result.Value;

    _internal->functionIndexCache += &target;
    return _internal->functionIndexCache.Count - 1;
}

p_implementation(Global, Array<Global::Function>, functions)
{
    return _internal->functions;
}

p_implementation(Global, Array<String>, DumpData)
{
    return{
        nd(bitType),
        nd(voidType)
    };
};


Global::Function::Function(): setter(new Xetter), getter(new Xetter)
{}

p_implementation(Global::Function, Array<String>, DumpData)
{
    return{ };
}

bool Global::Function::Ensure(FunctionCallResultCache const&source)const
{
    return
        getter->Ensure(l_(source.codeGetter))
        & // intensional non-short-circuid
        setter->Ensure(l_(source.codeSetter));
};

class Global::Function::CodeVisitor final : public Reni::CodeVisitor
{
    using baseType = Reni::CodeVisitor; using thisType = CodeVisitor;
    p_function(Array<String>, DumpData) override{ return{}; };
};

p_implementation(Global::Function, String, cppCode)
{
    CodeVisitor visitor;
    auto g = getter->code.IsValid ? getter->code.Value->ToCpp(visitor) : "";
    auto s = setter->code.IsValid ? setter->code.Value->ToCpp(visitor) : "";
    return g + s;
}

void Global::Function::GetterIsUsed()const{ getter->isRequired = true; };
void Global::Function::SetterIsUsed()const{ setter->isRequired = true; };

bool Global::Function::Xetter::Ensure(function<Ref<CodeItem>()> getCode) const
{
    if(!isRequired)
        return false;

    if(code.IsValid)
        return false;

    code = getCode();
    return true;
}

