#pragma once

#include "../HWLib/DumpableObject.h"
#include "../HWLib/Ref.h"
#include "../HWLib/WeakRef.h"
#include "../HWLib/Optional.h"
#include "../HWLib/Enumerable.h"
#include "../HWLib/Map.h"

using namespace HWLib;

namespace Reni
{
    class External;
    class Externals;
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

        ReplaceVisitor& Assign(External const&tag, ResultCache const& result);

        Optional<Ref<CodeItem>> const Arg(Type const&type, int depth)const;
        Optional<Ref<CodeItem>> const This(Type const&type, int depth)const;

        p(Optional<Externals>, ArgExts);
        p(Optional<Externals>, ThisExts);

        p(bool, hasArg);
        p(bool, hasThis);

    private:
        p_function(Array<String>, DumpData) override;
    };
}

