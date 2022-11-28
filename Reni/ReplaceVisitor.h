#pragma once

#include "External.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/Ref.h"
#include "../HWLib/WeakRef.h"
#include "../HWLib/Optional.h"
#include "../HWLib/Enumerable.h"
#include "../HWLib/Map.h"
#include "../HWLib/Pod.h"

using namespace HWLib;

namespace Reni
{
    class Closure;
    class FunctionCallContext;
    class Type;
    class CodeItem;
    class ResultCache;

    class ReplaceVisitor final: public DumpableObject
    {
        typedef DumpableObject baseType;
        typedef ReplaceVisitor thisType;
    public:
        pod<bool> Trace;
    private:
        Map<External const*, Ref<ResultCache>> results;
    public:
        ReplaceVisitor()
            :Trace(false)
        {
            SetDumpString();
        }
        ReplaceVisitor(External const&tag, ResultCache const& result)
            :Trace(false)
        {
            SetResults(tag, result);
        }
        ReplaceVisitor(ReplaceVisitor const& other)
            : results(other.results)
            , Trace(false)
        {
            SetDumpString();
        }

        void SetResults(External const&tag, ResultCache const& result);
        Optional<Ref<ResultCache>> GetResults(External const& tag) const;
        Optional<Ref<CodeItem>> GetCode(External const& tag) const;
        Closure GetClosure(External const& tag) const;

        void Assume(External const&tag, Type const&type)const;
        void AssumeFunctionArg(External::Function const&tag, Type const&arg) const;

    private:
        HW_PR_DECL_GETTER(Array<string>, DumpData) override;
    };
}

