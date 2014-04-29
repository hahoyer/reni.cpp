#pragma once
#include "DefineableToken .h"
#include "FeatureClass.h"
#include "../HWLib/WeakRef.h"

using namespace HWLib;

namespace Reni
{
    class DumpPrintToken final : public DefineableToken
    {
        using baseType = DefineableToken;
        using thisType = DumpPrintToken;
    public:

        DumpPrintToken() : baseType("dump_print")
        {
        }

    private:
        GenericDefinableFeatureClass<thisType> feature;
        featureClasses_override;
    };


    class MinusToken final : public DefineableToken
    {
        using baseType = DefineableToken;
        using thisType = MinusToken;
    public:

        MinusToken() : baseType("-")
        {
        }

    private:
        GenericDefinableFeatureClass<thisType> feature;
        featureClasses_override;
    };

    
    class PlusToken final : public DefineableToken
    {
        using baseType = DefineableToken;
        using thisType = PlusToken;
    public:

        PlusToken() : baseType("+")
        {
        }

    private:
        GenericDefinableFeatureClass<thisType> feature;
        featureClasses_override;
    };

    
    class StarToken final : public DefineableToken
    {
        using baseType = DefineableToken;
        using thisType = StarToken;
    public:

        StarToken() : baseType("*")
        {
            feature.SetDumpString();
        }

    private:
        GenericDefinableFeatureClass<thisType> feature;
        featureClasses_override;
    };


    class InstanceToken final : public DefineableToken
    {
        typedef DefineableToken baseType;
        typedef InstanceToken thisType;
    public:

        InstanceToken() : baseType("instance")
        {
        }

    private:
        GenericDefinableFeatureClass<thisType> feature;
        featureClasses_override;
    };
}