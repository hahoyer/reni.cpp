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
    class Externals;
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

        ThisRef;

        p_function(Array<String>, DumpData) override{ return{nd(statementIndex), nd(container)}; }
        p(WeakRef<Global>, global);

        ResultData const GetResultData(Category category) const;
        ResultData const SetResultData(Category category) const;
    private:
        p(bool, GetHllw);
        p(Size, GetSize);
        p(Ref<CodeItem>, GetCode);
        p(WeakRef<Type>, GetType);

        p(Externals, XetExts);

        p(bool, SetHllw);
        p(Size, SetSize);
        p(Ref<CodeItem>, SetCode);
        p(WeakRef<Type>, SetType);

        p(Ref<CodeItem>, ThisCode);
    };
}