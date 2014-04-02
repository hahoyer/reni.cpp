#include "Import.h"
#include "MainTokenFactory.h"

#include "ConditionalSyntax.h"
#include "ExpressionSyntax.h"
#include "DumpPrintToken.h"
#include "Feature.h"
#include "FeatureClass.h"
#include "NumberToken.h"
#include "Syntax.h"
#include "Terminal.h"
#include "TokenClass.h"
#include "SyntaxErrorToken .h"
#include "ParenthesisToken.h"
#include "TemplateInstances.h"
#include "SyntaxVisitor.h"
#include "SyntaxContainer.h"
#include "../HWLib/RefCountContainer.instance.h"


bool Trace = true;
using namespace Reni;
using namespace HWLang;


class TextToken final : public TerminalTokenClass{
    using baseType = TerminalTokenClass;
    using thisType = TextToken;
private:
    GenericFeatureClass<thisType> feature;
    p_function(WeakRef<FeatureClass>,featureClass) override{ return &feature.thisRef; }
    
    Ref<Syntax> const Create(SourcePart const&part)const override final{
        md(part);
        mb;
    }
};


class ArgToken final : public TerminalTokenClass {
    using baseType = TerminalTokenClass;
    using thisType = ArgToken;
    GenericFeatureClass<thisType> feature;
public:
    p(String, name){ return "arg"; };

    ResultData const GetResultData(Context const&context, Category category, SourcePart const&part)const{
        md(context, category, part);
        b_;
        return{};
    }
    Ref<Syntax, true> Replace(ReplaceSyntaxVisitor const&visitor) const { return visitor.arg; };
private:
    Ref<Syntax> const Create(SourcePart const&part)const override final{
        return new TerminalSyntax<ArgToken>(*this, part);
    }
    p_function(WeakRef<FeatureClass>,featureClass) override{ return &feature.thisRef; }
};


class ElseToken final : public InfixTokenClass {
    using baseType = InfixTokenClass;
    using thisType = ElseToken;
    GenericFeatureClass<thisType> feature;
public:
    p(String, name){ return "else"; };

private:
    p_function(bool, AcceptsMatch) override { return true; };
    Ref<Syntax> const Create(Ref<Syntax>const left, SourcePart const&part, Ref<Syntax>const right)const override final{
        auto const&conditional = dynamic_cast<IfThenSyntax const&>(*left);
        return conditional.AddElse(part, right);
    }
    p_function(WeakRef<FeatureClass>, featureClass) override{ return &feature.thisRef; }
};


class ThenToken final : public InfixTokenClass {
    using baseType = InfixTokenClass;
    using thisType = ThenToken;
    GenericFeatureClass<thisType> feature;
public:
    p(String, name){ return "then"; };

private:
    Ref<Syntax> const Create(Ref<Syntax>const left, SourcePart const&part, Ref<Syntax>const right)const override final{
        return new IfThenSyntax(left, part, right);
    }
    p_function(WeakRef<FeatureClass>, featureClass) override{ return &feature.thisRef; }
};


class Colon final : public InfixTokenClass{
    typedef InfixTokenClass baseType;
    typedef Colon thisType;
public:
    p(String, name){return ":";}
private:
    GenericFeatureClass<thisType> feature;
    p_function(WeakRef<FeatureClass>, featureClass) override{
        return &feature.thisRef;
    }
    Ref<Syntax> const Create(Ref<Syntax>const left, SourcePart const&part, Ref<Syntax>const right)const override final{
        Ref<SyntaxContainer> result = new SyntaxContainer(part);
        result->Add(left, right);
        return *result;
    }
};


class List final : public TokenClass{
    typedef List baseType;
    typedef TokenClass thisType;
public:
    String const name;
    List(String const&name) : name(name){}
private:
    GenericFeatureClass<thisType> feature;
    p_function(WeakRef<FeatureClass>, featureClass) override{
        return &feature.thisRef;
    }

    Ref<Syntax> const CreateSyntax(Ref<Syntax, true>const left, SourcePart const&part, Ref<Syntax, true>const right)const override final{
        Ref<SyntaxContainer> result = new SyntaxContainer(part);
        left->AddTo(*result);
        right->AddTo(*result);
        return *result;
    };
};


MainTokenFactory const MainTokenFactory::Instance;

TokenClass const& MainTokenFactory::Number = NumberToken();
TokenClass const& MainTokenFactory::Text = TextToken();
TokenClass const& MainTokenFactory::Start = LeftParenthesisToken(0);
TokenClass const& MainTokenFactory::End = RightParenthesisToken(0);

template<class TTokenClass>
void MainTokenFactory::AddTokenClass(TTokenClass const*tokenClass){
    auto key = tokenClass->name;
    a_if(!predefinedTokenClasses.ContainsKey(key), nd(key) + nd(predefinedTokenClasses[key]));
    predefinedTokenClasses.Assign(key,&tokenClass->thisRef);
}

MainTokenFactory::MainTokenFactory()
:tokenClasses([](String const& key){return new UserDefinedToken(key);})
, errorClasses([](String const& key){return new SyntaxErrorToken(key); })
{
    AddTokenClass(new ArgToken);
    AddTokenClass(new Colon);
    AddTokenClass(new DumpPrintToken);
    AddTokenClass(new ElseToken);
    AddTokenClass(new LeftParenthesisToken(1));
    AddTokenClass(new LeftParenthesisToken(2));
    AddTokenClass(new LeftParenthesisToken(3));
    AddTokenClass(new List(","));
    AddTokenClass(new List(";"));
    AddTokenClass(new MinusToken);
    AddTokenClass(new PlusToken);
    AddTokenClass(new RightParenthesisToken(1));
    AddTokenClass(new RightParenthesisToken(2));
    AddTokenClass(new RightParenthesisToken(3));
    AddTokenClass(new StarToken);
    AddTokenClass(new ThenToken);
}

TokenClass const& MainTokenFactory::GetTokenClass(String const&name){
    return Instance.InternalGetTokenClass(name);
}

TokenClass const& MainTokenFactory::GetErrorClass(String const&name){
    return *Instance.errorClasses(name);
}

TokenClass const& MainTokenFactory::InternalGetTokenClass(String const&name) const
{
    auto result = predefinedTokenClasses.Find(name);
    if(result.IsEmpty)
        return *tokenClasses(name);
    return *result;
}


Ref<Syntax> const SyntaxErrorToken::Create(SourcePart const&part)const{
    md(part);
    mb;
}
