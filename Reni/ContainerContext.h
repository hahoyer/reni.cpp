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
        FunctionCache<Ref<AccessData>, int> const accessData;
        ValueCache<WeakRef<Type>> const dataTypeCache;
    public:
        Ref<SyntaxContainer> const containerData;
    private:
        int const viewIndex;
    public:
        ContainerContext(RegularContext const&parent, SyntaxContainer const&containerData, int index);

        ContainerContext(ContainerContext const&) = delete;
        ThisRef;

        p(WeakRef<Type>, dataType){return dataTypeCache.Value;};
        p(Size, dataSize);

        WeakRef<Type> const AccessType(int const statementIndex) const;
        Ref<ResultCache> const AccessResult(Type const& argsType, int const tokenIndex) const;
        SearchResult<AccessFeature> const DeclarationsForType(DefineableToken const&token) const override;

    private:
        p_function(Array<String>, DumpData) override
        {
            return base_p_name(DumpData) +
                _({
                    nd(containerData),
                    nd(viewIndex)
                });
        };

    };

};
//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")
