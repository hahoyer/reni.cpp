#pragma once

#include "External.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/Ref.h"
#include "../HWLib/WeakRef.h"
#include "../HWLib/Optional.h"
#include "../HWLib/Enumerable.h"
#include "../HWLib/Map.h"

using namespace HWLib;

namespace Reni
{
    class Externals;
    class FunctionCallContext;
    class Type;
    class CodeItem;
    class ResultCache;

    class ReplaceVisitor final: public DumpableObject
    {
        typedef DumpableObject baseType;
        typedef ReplaceVisitor thisType;
    public:
        bool Trace;
    private:
        Map<External const*, Ref<ResultCache>> results;
    public:
        ReplaceVisitor()
        {
            SetDumpString();
        }

        void SetResults(External const&tag, ResultCache const& result);
        Optional<Ref<ResultCache>> const GetResults(External const&tag)const;
        Optional<Ref<CodeItem>> const GetCode(External const&tag)const;
        Optional<Externals> const GetExts(External const&tag)const;

        void Assume(External const&tag, Type const&type)const;
        void AssumeFunctionArg(External::Function const&tag, Type const&arg) const;

    private:
        p_function(Array<String>, DumpData) override;
    };
}

