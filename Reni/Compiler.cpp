#include "Import.h"
#include "Compiler.h"

#include "BitType.h"
#include "ContainerContext.h"
#include "Result.h"
#include "SyntaxContainer.h"
#include "TokenClass.h"

#include "../HWLib/RefCountContainer.instance.h"

static bool Trace = true;
#include "Compiler.internal.h"

using namespace Reni;

Compiler::Compiler(string const&fileName)
: _internal(new internal(fileName))
{}

HW_PR_IMPL_GETTER(Compiler, Ref<Syntax>, syntax)
{
    return _internal->syntaxCache.Value;
};

HW_PR_IMPL_GETTER(Compiler, CodeBase, code)
{
    return _internal->codeCache.Value;
};

HW_PR_IMPL_GETTER(Compiler, Ref<CodeItem>, main)
{
    return _internal->codeCache.Value.main;
};

HW_PR_IMPL_GETTER(Compiler, string, cppCode)
{
    return _internal->cppCode;
};

ExecutionResult Compiler::Execute()
{
    return _internal->Execute();
}

Ref<Syntax> Compiler::GetSyntaxFromText(string const& text)
{
    return internal::GetSyntaxFromText(text);
};


CodeBase::CodeBase(Ref<CodeItem> const& main, Array<Global::Function> const& functions) : main(main), functions(functions)
{
    SetDumpString();
}

HW_PR_IMPL_GETTER(CodeBase, string, cppMain){ return MainCodeVisitor::MainVisit(main); };

HW_PR_IMPL_GETTER(CodeBase, string, cppFunctions)
{
    return functions
        .Select<string>([&](Global::Function const&function){return function.cppCode; })
        ->Aggregate<string>();
};

HW_PR_IMPL_GETTER(CodeBase, string, cppDeclarations)
{
    return functions
        .Select<string>([&](Global::Function const&function){return function.cppDeclarations; })
        ->Aggregate<string>();
};

