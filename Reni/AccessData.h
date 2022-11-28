#pragma once
#include "../HWLib/DumpableObject.h"
#include "../HWLib/Ref.h"
#include "../HWLib/CtrlRef.h"
#include "../HWLib/RefCountProvider.h"
#include "../Util/Category.h"
#include "../HWLib/ValueCache.h"
#include "../Util/Size.h"
#include "../HWLib/WeakRef.h"

using namespace HWLib;
using namespace Util;

namespace Reni
{
    class AccessFeature;
    class CodeItem;
    class ContainerContext;
    class Closure;
    class Global;
    class ResultCache;
    class ResultData;
    class Type;

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

        size_t const statementIndex;
        ContainerContext const& container;

        AccessData(AccessData const&) = delete;
        AccessData(ContainerContext const& container, size_t statementIndex);

        HW_PR_THISREF;

        HW_PR_DECL_GETTER(Array<string>, DumpData) override{ return{HW_D_VALUE(statementIndex), HW_D_VALUE(container)}; }
        HW_PR_GET(WeakRef<Global>, global);

        ResultData const GetResultData(Category category) const;
        ResultData const SetResultData(Category category) const;
    private:
        HW_PR_GET(bool, GetHollow);
        HW_PR_GET(Size, GetSize);
        HW_PR_GET(Ref<CodeItem>, GetCode);
        HW_PR_GET(WeakRef<Type>, GetType);

        HW_PR_GET(Closure, XetClosure);

        HW_PR_GET(bool, SetHollow);
        HW_PR_GET(Size, SetSize);
        HW_PR_GET(Ref<CodeItem>, SetCode);
        HW_PR_GET(WeakRef<Type>, SetType);

        HW_PR_GET(Ref<CodeItem>, ThisCode);
    };
}