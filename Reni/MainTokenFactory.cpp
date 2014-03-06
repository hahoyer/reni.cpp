#include "Import.h"
#include "MainTokenFactory.h"

#include "ExpressionSyntax.h"
#include "DumpPrintToken.h"
#include "NumberToken.h"
#include "Syntax.h"
#include "Terminal.h"
#include "TokenClass.h"

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
    UserDefinedToken(String const name) : baseType(name){}
private:
    GenericFeatureClass<thisType> feature;
    override_p_function(WeakRef<FeatureClass>, featureClass){ return &feature.ref; }
};


class SyntaxErrorToken final : public TokenClass{
    using baseType = TokenClass;
    using thisType = DefineableToken;

    String const text;
public:
    SyntaxErrorToken(String const text) : text(text){}
private:
    override_p_function(Array<String>, DumpData){
        return{nd(text)};
    };
private:
    GenericFeatureClass<thisType> feature;
    override_p_function(WeakRef<FeatureClass>, featureClass){ return &feature.ref; }
};


class LeftParenthesisToken final : public TokenClass{
    using baseType = TokenClass;
    using thisType = DefineableToken;

    int const level;
public:
    LeftParenthesisToken(int level) : level(level){}
private:
    virtual CtrlRef<Syntax > const CreateSyntax(SourcePart const&part, CtrlRef<Syntax >const right, bool isMatch)const  override{
        if (isMatch)
            return baseType::CreateSyntax(part, right, isMatch);
        return new OpenSyntax(level, part, right);

    };
    override_p_function(Array<String>, DumpData){
        return{ nd(level) };
    };

    class OpenSyntax final : public Syntax{
        using baseType = Syntax;
        int const level;
        CtrlRef<Syntax > const right;
    public:
        OpenSyntax(int level, SourcePart const part, CtrlRef<Syntax > const right)
            : baseType(part)
            , level(level)
            , right(right)
        {
            SetDumpString();
        }
    private:
        virtual CtrlRef<Syntax > const ParenthesisMatch(int level, SourcePart const&part)const override{
            if (level != this->level)
                return baseType::ParenthesisMatch(level, part);
            return right;
        };
        override_p_function(Array<String>, DumpData){
            return{ nd(level), nd(right) };
        }
    };
private:
    GenericFeatureClass<thisType> feature;
    override_p_function(WeakRef<FeatureClass>, featureClass){ return &feature.ref; }
};


class RightParenthesisToken final : public TokenClass{
    using baseType = TokenClass;
    using thisType = DefineableToken;

    int const level;
public:
    RightParenthesisToken(int level) : level(level){}
private:
    virtual CtrlRef<Syntax > const CreateSyntax(CtrlRef<Syntax >const left, SourcePart const&part, bool isMatch)const  override{
        if (!isMatch)
            return baseType::CreateSyntax(left, part, isMatch);
        return left->ParenthesisMatch(level, part);
    };

    override_p_function(Array<String>, DumpData){
        return{ nd(level) };
    };
private:
    GenericFeatureClass<thisType> feature;
    override_p_function(WeakRef<FeatureClass>, featureClass){ return &feature.ref; }
};

MainTokenFactory const MainTokenFactory::Instance;

TokenClass const& MainTokenFactory::Number = NumberToken();
TokenClass const& MainTokenFactory::Text = TextToken();
TokenClass const& MainTokenFactory::Start = LeftParenthesisToken(0);
TokenClass const& MainTokenFactory::End = RightParenthesisToken(0);

MainTokenFactory::MainTokenFactory()
:tokenClasses([](String const& key){return new UserDefinedToken(key); })
, errorClasses([](String const& key){return new SyntaxErrorToken(key); })
{
    predefinedTokenClasses = 
        std::unordered_map<String const, CtrlRef<TokenClass const>>({ 
        { "dump_print", new DumpPrintToken } ,
        {"{", new LeftParenthesisToken(1)},
        { "[", new LeftParenthesisToken(2) },
        { "(", new LeftParenthesisToken(3) },
        { "}", new RightParenthesisToken(1) },
        { "]", new RightParenthesisToken(2) },
        { ")", new RightParenthesisToken(3) }
    });
}

TokenClass const& MainTokenFactory::GetTokenClass(String const&name){
    return Instance.InternalGetTokenClass(name);
}

TokenClass const& MainTokenFactory::GetErrorClass(String const&name){
    return *Instance.errorClasses[name];
}

TokenClass const& MainTokenFactory::InternalGetTokenClass(String const&name) const
{
    auto pt = predefinedTokenClasses.find(name);
    if (pt != predefinedTokenClasses.end())
        return *pt->second;

    return *tokenClasses[name];
}

#include "../HWLib/RefCountContainer.instance.h"
