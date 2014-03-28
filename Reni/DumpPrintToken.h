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
        p_function(WeakRef<FeatureClass>,featureClass) override{
            return &feature.thisRef;
        }
    };

    class MinusToken final : public DefineableToken{
        using baseType = DefineableToken;
        using thisType = MinusToken;
    public:
        MinusToken() : baseType("-"){}
    private:
        GenericFeatureClass<thisType> feature;
        p_function(WeakRef<FeatureClass>,featureClass) override{
            return &feature.thisRef;
        }
    };

    class PlusToken final : public DefineableToken{
        using baseType = DefineableToken;
        using thisType = PlusToken;
    public:
        PlusToken() : baseType("+"){}
    private:
        GenericFeatureClass<thisType> feature;
        p_function(WeakRef<FeatureClass>, featureClass) override{
            return &feature.thisRef;
        }
    };

    class StarToken final : public DefineableToken{
        using baseType = DefineableToken;
        using thisType = StarToken;
    public:
        StarToken() : baseType("*"){}
    private:
        GenericFeatureClass<thisType> feature;
        p_function(WeakRef<FeatureClass>, featureClass) override{
            return &feature.thisRef;
        }
    };
}