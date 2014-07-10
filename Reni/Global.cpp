#include "Import.h"
#include "Global.h"

#include "CodeFunction.h"
#include "ContextIncludes.h"
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
    mutable Optional<CodeFunction> code;
    
    Xetter(): isRequired(false){}
    
    bool Ensure(function<CodeFunction()> getCode) const;
private:
    p_function(Array<String>, DumpData) override{ return{nd(isRequired),nd(code)}; }
};

Global::Global()
    : boolType(this)
    , bitType(this)
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
        getter->Ensure(l_(source.getter))
        & // intensional non-short-circuid
        setter->Ensure(l_(source.setter));
};

p_implementation(Global::Function, String, cppCode)
{
    auto g = getter->code.IsValid ? getter->code.Value.cppCode : "";
    auto s = setter->code.IsValid ? setter->code.Value.cppCode : "";
    return g + s;
}

p_implementation(Global::Function, String, cppDeclarations)
{
    auto g = getter->code.IsValid ? getter->code.Value.cppDeclaration : "";
    auto s = setter->code.IsValid ? setter->code.Value.cppDeclaration : "";
    return g + s;
}

void Global::Function::GetterIsUsed()const{ getter->isRequired = true; };
void Global::Function::SetterIsUsed()const{ setter->isRequired = true; };

bool Global::Function::Xetter::Ensure(function<CodeFunction()> getCode) const
{
    if(!isRequired)
        return false;

    if(code.IsValid)
        return false;

    code = getCode();
    return true;
}


CodeFunction const Constants<CodeFunction >::NotValid;

CodeFunction::CodeFunction(
    bool isSetter,
    int index,
    Ref<CodeItem> const& body)
    : isSetter(isSetter)
      ,index(index)
      , body(body)
{
    SetDumpString();
    a_if(index >= 0, nd(index));
}

p_implementation(CodeFunction, String,cppCode)
{
    return MainCodeVisitor::GetterVisit(index, body);
}

p_implementation(CodeFunction, String, cppDeclaration)
{
    return MainCodeVisitor::GetterFunctionDeclaration(index) + ";\n";
}

