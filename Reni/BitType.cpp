#include "Import.h"
#include "BitType.h"

#include "ArrayType.h"
#include "Size.h"
#include "Feature.h"
#include "FeatureProvider.h"
#include "../HWLib/RefCountContainer.instance.h"

static bool Trace = true;

using namespace Reni;

class DumpPrintBitArray final : public FeatureProvider<DumpPrintToken, ArrayType> {
    typedef FeatureProvider<DumpPrintToken, ArrayType> baseType;
    typedef DumpPrintBitArray thisType;

    
    class Feature final : public Reni::Feature{
        ArrayType const& value;
    public:
        Feature(ArrayType const&value) : value(value){}
    private:
        virtual ResultData const FunctionResult(
            Context const&context,
            Category category,
            ExpressionSyntax const& expressionSyntax
            )const override;

        override_p_function(Array<String>, DumpData) {
            return{ nd(value) };
        }
    };

    
    class For final : public FeatureProvider<DumpPrintToken>{
        typedef FeatureProvider<DumpPrintToken> baseType;
        typedef For thisType;
        ArrayType const& value;
    public:
        For(ArrayType const&value) : value(value) {}
    private:

        override_p_function(Ref<Reni::Feature>, feature){
            return new Feature(value);
        }
        override_p_function(Array<String>, DumpData) {
            return{nd(value)};
        }
    };

    
    virtual Ref<FeatureProvider<DumpPrintToken>, true>const Convert(ArrayType const&top)const override {
        return new For(top);
    }
    
    override_p_function(Array<String>, DumpData) {
        return{};
    }

};

override_p_implementation(BitType, Size, size){ return Size(1); }

BitType::operator Ref<FeatureProvider<DumpPrintToken, ArrayType>, true>()const{
    return new DumpPrintBitArray();
}


class InvalidArgumentList final : public DumpableObject{
    WeakRef<ExpressionSyntax const> const expressionSyntax;
public:
    InvalidArgumentList(ExpressionSyntax const& expressionSyntax) : expressionSyntax(&expressionSyntax){}
private:
    override_p_function(Array<String>, DumpData){ return{nd(expressionSyntax)}; };
};

ResultData const DumpPrintBitArray::Feature::FunctionResult(
    Context const&context,
    Category category,
    ExpressionSyntax const& expressionSyntax
    )const{
    if(expressionSyntax.right.IsValid)
        throw InvalidArgumentList(expressionSyntax);


    md(context, category, expressionSyntax);
    b_;
    return{};
};


