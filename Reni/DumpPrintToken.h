#pragma once
#include "DefineableToken .h"
#include "FeatureClass.h"
#include "../HWLib/WeakRef.h"
using namespace HWLib;

namespace Reni{

    class DumpPrintToken final : public DefineableToken{
        using baseType = DefineableToken;
        using thisType = DumpPrintToken;
    public:
        DumpPrintToken() : baseType("dump_print"){}
    private:
        GenericFeatureClass<thisType> feature;
        override_p_function(WeakRef<FeatureClass>, featureClass){
            return &feature.ref;
        }
    };


}