#include "Import.h"
#include "MainTokenFactory.h"

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

bool Trace = true;
using namespace Reni;
using namespace HWLang;


class TextToken final : public TokenClass{
    using baseType = TokenClass;
    using thisType = TextToken;
private:
    GenericFeatureClass<thisType> feature;
    override_p_function(WeakRef<FeatureClass>, featureClass){ return &feature.ref; }
};


class UserDefinedToken final : public DefineableToken {
    using baseType = DefineableToken;
    using thisType = UserDefinedToken;
public:
    UserDefinedToken(String const&name) : baseType(name){};
private:
    GenericFeatureClass<thisType> feature;
    override_p_function(WeakRef<FeatureClass>, featureClass){ return &feature.ref; }
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
    predefinedTokenClasses.Assign(key,&tokenClass->ref);
}

MainTokenFactory::MainTokenFactory()
:tokenClasses([](String const& key){return new UserDefinedToken(key);})
, errorClasses([](String const& key){return new SyntaxErrorToken(key); })
{
    AddTokenClass(new LeftParenthesisToken(1));
    AddTokenClass(new LeftParenthesisToken(2));
    AddTokenClass(new LeftParenthesisToken(3));
    AddTokenClass(new RightParenthesisToken(1));
    AddTokenClass(new RightParenthesisToken(2));
    AddTokenClass(new RightParenthesisToken(3));
    AddTokenClass(new SignToken("+"));
    AddTokenClass(new SignToken("-"));
    AddTokenClass(new DumpPrintToken);
}

TokenClass const& MainTokenFactory::GetTokenClass(String const&name){
    return Instance.InternalGetTokenClass(name);
}

TokenClass const& MainTokenFactory::GetErrorClass(String const&name){
    return *Instance.errorClasses[name];
}

TokenClass const& MainTokenFactory::InternalGetTokenClass(String const&name) const
{
    auto result = predefinedTokenClasses.Find(name);
    if(result.IsEmpty)
        return *tokenClasses[name];
    return *result;
}

#include "TemplateInstances.h"
#include "../HWLib/RefCountContainer.instance.h"
