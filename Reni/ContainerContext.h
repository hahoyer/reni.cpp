#pragma once
//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")
#include "Context.h"
#include "../HWLib/FunctionCache.h"
#include "../HWLib/ValueCache.h"
#include "../HWLib/Array.h"

namespace Reni
{
    class AccessData;
    class AccessType;
    class FunctionCallContext;
    class ResultCache;
                            
    class ContainerContext final : public ChildContext
    {
        typedef ContainerContext thisType;
        typedef ChildContext baseType;

        FunctionCache<WeakRef<FunctionCallContext>, Type const*> const functionCallContext;
        ValueCache<WeakRef<Type>> const dataTypeCache;
    public:
        FunctionCache<Ref<AccessData>, size_t> const accessData;
        Ref<SyntaxContainer> const containerData;
        External const& asExternal;
    private:
        size_t const viewIndex;
    public:
        ContainerContext(RegularContext const&parent, SyntaxContainer const&containerData, size_t index);

        ContainerContext(ContainerContext const&) = delete;
        ThisRef;

        p(WeakRef<Type>, dataType){return dataTypeCache.Value;};
        p(Size, dataSize);
        p(WeakRef<External>, asExternal);

        Ref<ResultCache> const AccessResult(Type const& argsType, size_t tokenIndex) const;
        SearchResult<AccessFeature> const DeclarationsForType(DefineableToken const&token) const override;
        Size const AlignedPartSize(size_t position) const;
        Size const PartSize(size_t position) const;
    private:
        p_function(Array<string>, DumpData) override;
    };

};
//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")
