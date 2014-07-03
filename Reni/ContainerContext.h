#pragma once
#include "Context.h"
#include "../HWLib/FunctionCache.h"
#include "../HWLib/ValueCache.h"
#include "../HWLib/Array.h"

namespace Reni
{
    class FunctionCallContext;
    class FunctionCallResultCache;

    class ContainerContext final : public ChildContext
    {
        typedef ContainerContext thisType;
        typedef ChildContext baseType;

        FunctionCache<WeakRef<FunctionCallContext>, Type const*> functionCallContext;
        FunctionCache<ContextFeature, int> accessFeature;
        ValueCache<WeakRef<Type>> dataTypeCache;
    public:
        Ref<SyntaxContainer> containerData;
    private:
        int const index;
    public:
        ContainerContext(RegularContext const&parent, SyntaxContainer const&containerData, int index);

        ContainerContext(ContainerContext const&) = delete;
        ThisRef;

        p(WeakRef<Type>, dataType){return dataTypeCache.Value;};
        p(Size, dataSize);

        Ref<FunctionCallResultCache> const FunctionCallResult(Type const& argsType, int const tokenIndex) const;
        SearchResult<ContextFeature> const DeclarationsForType(DefineableToken const&token) const override;

    private:
        p_function(Array<String>, DumpData) override
        {
            return base_p_name(DumpData) +
                _({
                    nd(containerData),
                    nd(index)
                });
        };

    };

};