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
        p_function(Array<WeakRef<FeatureClass>>, featureClasses) override{ return base_p_name(featureClasses) + &feature.thisRef; }
    };

    class MinusToken final : public DefineableToken{
        using baseType = DefineableToken;
        using thisType = MinusToken;
    public:
        MinusToken() : baseType("-"){}
    private:
        GenericFeatureClass<thisType> feature;
        p_function(Array<WeakRef<FeatureClass>>, featureClasses) override{ return base_p_name(featureClasses) + &feature.thisRef; }
    };

    class PlusToken final : public DefineableToken{
        using baseType = DefineableToken;
        using thisType = PlusToken;
    public:
        PlusToken() : baseType("+"){}
    private:
        GenericFeatureClass<thisType> feature;
        p_function(Array<WeakRef<FeatureClass>>, featureClasses) override{ return base_p_name(featureClasses) + &feature.thisRef; }
    };

    class StarToken final : public DefineableToken{
        using baseType = DefineableToken;
        using thisType = StarToken;
    public:
        StarToken() : baseType("*"){}
    private:
        GenericFeatureClass<thisType> feature;
        p_function(Array<WeakRef<FeatureClass>>, featureClasses) override{ return base_p_name(featureClasses) + &feature.thisRef; }
    };

    class InstanceToken final : public DefineableToken {
        typedef DefineableToken baseType; 
        typedef InstanceToken  thisType;
    public:
        InstanceToken() : baseType("instance") {}
    private:
        GenericFeatureClass<thisType> feature;
        featureClasses_override;
    };
}