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
    HW_PR_GET(Array<Function>, functions)
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
    HW_PR_DECL_GETTER(Array<string>,DumpData) override{ return{HW_D_VALUE(isRequired),HW_D_VALUE(code)}; }
};

Global::Global()
    : boolType(this)
    , bitType(this)
    , voidType(this)
    , _internal(new internal)
{
}

size_t const Global::FunctionIndex(FunctionCallResultCache const& target) const
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

HW_PR_IMPL_GETTER(Global, Array<Global::Function>, functions)
{
    return _internal->functions;
}

HW_PR_IMPL_GETTER(Global, Array<string>,DumpData)
{
    return{
        HW_D_VALUE(bitType),
        HW_D_VALUE(voidType)
    };
};


Global::Function::Function(): setter(new Xetter), getter(new Xetter)
{}

HW_PR_IMPL_GETTER(Global::Function, Array<string>,DumpData)
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

HW_PR_IMPL_GETTER(Global::Function, string, cppCode)
{
    auto g = getter->code.IsValid ? getter->code.Value.cppCode : "";
    auto s = setter->code.IsValid ? setter->code.Value.cppCode : "";
    return g + s;
}

HW_PR_IMPL_GETTER(Global::Function, string, cppDeclarations)
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


CodeFunction::CodeFunction(
    bool isSetter,
    size_t index,
    Ref<CodeItem> const& body)
    : isSetter(isSetter)
      ,index(index)
      , body(body)
{
    SetDumpString();
    HW_ASSERT(index >= 0, HW_D_VALUE(index));
}

HW_PR_IMPL_GETTER(CodeFunction, string,cppCode)
{
    return MainCodeVisitor::GetterVisit(index, body);
}

HW_PR_IMPL_GETTER(CodeFunction, string, cppDeclaration)
{
    return MainCodeVisitor::GetterFunctionDeclaration(index) + ";\n";
}

