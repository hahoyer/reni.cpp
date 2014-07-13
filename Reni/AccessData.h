#pragma once
#include "../HWLib/DumpableObject.h"
#include "../HWLib/Ref.h"
#include "../HWLib/CtrlRef.h"
#include "../HWLib/RefCountProvider.h"

using namespace HWLib;

namespace Reni
{
    class AccessFeature;
    class AccessType;
    class ContainerContext;
    class Global;
    class ResultCache;
                 
    class AccessData final 
        : public DumpableObject
        , public RefCountProvider
    {
        using baseType = DumpableObject;
        using thisType = AccessData;
    public:
        Ref<ResultCache> const resultCache;
        Ref<ResultCache> const dataResultCache;
        CtrlRef<AccessFeature> const feature;
        CtrlRef<AccessType> const type;

        int const statementIndex;
        ContainerContext const& container;

        AccessData(AccessData const&) = delete;
        AccessData(ContainerContext const& container, int statementIndex);

        ThisRef;

        p_function(Array<String>, DumpData) override{ return{nd(statementIndex), nd(container)}; }
        p(WeakRef<Global>, global);

    };
}