#pragma once
#include "DumpPrintToken.h"
#include "Feature.h"
#include "FeatureProvider.h"
#include "SearchResult.h"
#include "ArgVisitor.h"
#include "UserDefinedToken.h"

using namespace Reni;
using namespace HWLib;


template<class TTokenClass, class TTargetTypeHandler>
class ArglessFunctionProvider<TTokenClass, TTargetTypeHandler> ::Feature final : public Reni::Feature{
    typedef Reni::Feature baseType;
    typedef Feature thisType;
    targetType const& value;
public:
    Feature(targetType const&value) : value(value){}
    AssumeConstObject;
private:
    ResultData const FunctionResult(
        Context const&context,
        Category category,
        ExpressionSyntax const& expressionSyntax
    )const override{
        a_throw_(!expressionSyntax.left.IsEmpty);
        a_throw_(expressionSyntax.right.IsEmpty);
        ArgVisitor visitor;
        auto result = expressionSyntax.left->GetResultCache(context);
        visitor.Assign(&ArgVisitor::Tag::expressionArg, *result);
        return TTargetTypeHandler::Result(category, value)
            .Replace(visitor);
    };
    p_function(Array<String>, DumpData) override{return{nd(value)};}
};


template<class TTokenClass, class TTargetTypeHandler>
class InfixFunctionProvider<TTokenClass, TTargetTypeHandler> ::Feature final : public Reni::Feature{
    typedef Reni::Feature baseType;
    typedef Feature thisType;
    targetType const& value;
public:
    Feature(targetType const&value) : value(value){}
    AssumeConstObject;
private:
    ResultData const FunctionResult(
        Context const&context,
        Category category,
        ExpressionSyntax const& expressionSyntax
        )const override{
        a_throw_(!expressionSyntax.right.IsEmpty);
        auto thisResult = expressionSyntax.left->GetResultCache(context);
        auto argResult = expressionSyntax.right->GetResultCache(context);
        ArgVisitor visitor;
        visitor.Assign(&ArgVisitor::Tag::expressionThis, *thisResult);
        visitor.Assign(&ArgVisitor::Tag::expressionArg, *argResult);
        return TTargetTypeHandler::Result(category, value, *argResult->type)
            .Replace(visitor);
    };
    p_function(Array<String>, DumpData) override{ return{nd(value)}; }
};


template<class TTokenClass, class TTargetTypeHandler>
ArglessFunctionProvider<TTokenClass, TTargetTypeHandler>::ArglessFunctionProvider(targetType const&value) 
: feature(new Feature(value)) {}

template<class TTokenClass, class TTargetTypeHandler>
p_implementation(ArglessFunctionProvider<TTokenClass COMMA TTargetTypeHandler>, Array<String>, DumpData) { return{nd(feature)}; }

template<class TTokenClass, class TTargetTypeHandler>
p_implementation(ArglessFunctionProvider<TTokenClass COMMA TTargetTypeHandler>, Ref<Reni::Feature>, feature){ return feature->ref; }


template<class TTokenClass, class TTargetTypeHandler>
InfixFunctionProvider<TTokenClass, TTargetTypeHandler>::InfixFunctionProvider(targetType const&value)
: feature(new Feature(value)) {}

template<class TTokenClass, class TTargetTypeHandler>
p_implementation(InfixFunctionProvider<TTokenClass COMMA TTargetTypeHandler>, Array<String>, DumpData) { return{nd(feature)}; }

template<class TTokenClass, class TTargetTypeHandler>
p_implementation(InfixFunctionProvider<TTokenClass COMMA TTargetTypeHandler>, Ref<Reni::Feature>, feature){ return feature->ref; }


template <typename T>
inline SearchResult const GenericFeatureClass<T>::GetDefinition(Type const&type)const{
    md(type);
    mb;
};

template <typename T>
inline SearchResult const GenericFeatureClass<T>::GetDefinition(Context const&context)const{
    md(context);
    mb;
};


#define TypeFeature(tokenClass)\
template <>\
inline SearchResult const GenericFeatureClass<tokenClass>::GetDefinition(Type const&target)const {\
    return target.GetGenericDefinition<tokenClass>();\
}

#define ContextFeature(tokenClass)\
template <>\
inline SearchResult const GenericFeatureClass<tokenClass>::GetDefinition(Context const&target)const {\
    return target.GetGenericDefinition<tokenClass>();\
}

TypeFeature(UserDefinedToken);
TypeFeature(DumpPrintToken);
TypeFeature(MinusToken);
TypeFeature(PlusToken);
TypeFeature(StarToken);
ContextFeature(UserDefinedToken);
ContextFeature(MinusToken);

template <typename T>
pure_p_implementation(FeatureProvider<T>, Ref<Feature>, feature) ;

template <typename T>
pure_p_implementation(ContextFeatureProvider<T>, Ref<ContextFeature>, feature) ;
