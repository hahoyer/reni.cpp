#pragma once
#include "ContextFeatureProvider.h"
#include "DumpPrintToken.h"

using namespace HWLib;

namespace Reni{
    class MinusFeatureProvider final : public ContextFeatureProvider<MinusToken>{
        typedef FeatureProvider<MinusToken> baseType;
        typedef MinusFeatureProvider thisType;

        class Feature final : public ContextFeature{
            typedef ContextFeature baseType;
            typedef Feature thisType;
        private:
            virtual ResultData const FunctionResult(
                Context const&context,
                Category category,
                Ref<Syntax, true> const& right
                )const override;
            p_function(Array<String>,DumpData) override { return{}; }
        };
    public:
        ThisRef;
    private:
        p_function(Ref<ContextFeature>,feature) override{ return new Feature(); }
        p_function(Array<String>,DumpData) override { return{}; }
    };

    class SymbolFeatureProvider final : public ContextFeatureProvider<UserDefinedToken> {
        typedef FeatureProvider<UserDefinedToken> baseType;
        typedef SymbolFeatureProvider thisType;

        class Feature final : public ContextFeature {
            typedef ContextFeature baseType;
            typedef Feature thisType;
        private:
            virtual ResultData const FunctionResult(
                Context const&context,
                Category category,
                Ref<Syntax, true> const& right
                )const override;
            p_function(Array<String>, DumpData) override {
                return{};
            }
        };
    public:
        ThisRef;
    private:
        p_function(Ref<ContextFeature>, feature) override {
            return new Feature();
        }
        p_function(Array<String>, DumpData) override {
            return{};
        }
    };


}
