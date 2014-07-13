#pragma once
#include "../HWLib/DumpableObject.h"
#include "../HWLib/Ref.h"
#include "../HWLib/CtrlRef.h"
#include "../HWLib/RefCountProvider.h"
#include "../Util/Category.h"
#include "../HWLib/ValueCache.h"
#include "../Util/Size.h"

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

        int const statementIndex;
        ContainerContext const& container;

        AccessData(AccessData const&) = delete;
        AccessData(ContainerContext const& container, int statementIndex);

        ThisRef;

        p_function(Array<String>, DumpData) override{ return{nd(statementIndex), nd(container)}; }
        p(WeakRef<Global>, global);

        ResultData const GetResultData(Util::Category category) const;
        CtrlRef<Type> const get_type() const;
    private:
        bool const GetHllw()const;
        Size const GetSize()const;
        Ref<CodeItem> const GetCode()const;
        WeakRef<Type> const GetType()const;
        Externals const GetExts()const;

    };
}