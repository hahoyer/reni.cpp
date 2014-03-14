#pragma once

using namespace HWLib;

namespace Reni{
    class ArrayType;
    class Context;
    class SearchResult;
    class Type;


    class FeatureClass
        : public WithId<DumpableObject, FeatureClass>
          , public RefCountProvider{
        using baseType = WithId<DumpableObject, FeatureClass >;
        using thisType = FeatureClass;
    public:
        AssumeConstObject;

        virtual SearchResult const GetDefinition(Type const&type)const = 0;
        virtual SearchResult const GetDefinition(Context const&context)const = 0;
    private:
        override_p_function(Array<String>, DumpData){
            return{};
        }
    };


    template <typename T>
    class GenericFeatureClass final: public FeatureClass{
        using baseType = FeatureClass;
        using thisType = GenericFeatureClass;
        virtual SearchResult const GetDefinition(Type const&type)const override;
        virtual SearchResult const GetDefinition(Context const&context)const override;
    };
}