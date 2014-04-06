#pragma once

using namespace HWLib;

namespace Reni{
    class ArrayType;
    class Context;
    class SearchResult;
    class Type;
    class DefineableToken;

    class FeatureClass
        : public WithId<DumpableObject, FeatureClass>
          , public RefCountProvider{
        using baseType = WithId<DumpableObject, FeatureClass >;
        using thisType = FeatureClass;
    public:
        ThisRef;

        virtual SearchResult const GetDefinition(Type const&type)const = 0;
        virtual SearchResult const GetDefinition(Context const&context)const = 0;
    private:
        p_function(Array<String>,DumpData) override{
            return{};
        }
    };


    template <typename T>
    class GenericFeatureClass final: public FeatureClass{
        using baseType = FeatureClass;
        using thisType = GenericFeatureClass;
        SearchResult const GetDefinition(Type const&type)const override final;
        SearchResult const GetDefinition(Context const&context)const override final;
    public:
        GenericFeatureClass(){ SetDumpString(); }
    };


    class DefinableFeatureClass final : public FeatureClass{
        using baseType = FeatureClass;
        using thisType = DefinableFeatureClass;

        DefineableToken const&parent;
    public:
        DefinableFeatureClass(DefineableToken const&parent) : parent(parent){ }
    private:
        SearchResult const GetDefinition(Type const&type)const override final;
        SearchResult const GetDefinition(Context const&context)const override final;

        p_function(Array<String>, DumpData) override;
    };
}