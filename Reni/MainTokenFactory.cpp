#include "Import.h"
#include "MainTokenFactory.h"

#include "CodeItems.h"
#include "ConditionalSyntax.h"
#include "ContainerContext.h"
#include "ExpressionSyntax.h"
#include "External.h"
#include "DumpPrintToken.h"
#include "Feature.h"
#include "FunctionToken.h"
#include "NumberToken.h"
#include "Syntax.h"
#include "Terminal.h"
#include "TokenClass.h"
#include "SyntaxErrorToken .h"
#include "ParenthesisToken.h"
#include "SyntaxVisitor.h"
#include "SyntaxContainer.h"
#include "UserDefinedToken.h"

#include "../HWLib/RefCountContainer.instance.h"


bool Trace = true;
using namespace Reni;
using namespace HWLang;


class TextToken final : public TerminalTokenClass{
    using baseType = TerminalTokenClass;
    using thisType = TextToken;

    Ref<Syntax> const Create(SourcePart const&part)const override final{
        HW_D_METHOD(part);
        HW_BREAK_AND_THROW;
    }
};


class ArgToken final : public TerminalTokenClass {
    using baseType = TerminalTokenClass;
    using thisType = ArgToken;
public:
    HW_PR_GET(string, name){ return "^"; };

    ResultData const GetResultData(Context const&context, Category category, SourcePart const&)const;

    Optional<Ref<Syntax>> Replace(ReplaceSyntaxVisitor const&visitor) const { return visitor.arg; };
private:
    Ref<Syntax> const Create(SourcePart const&part)const override final{
        return new TerminalSyntax<ArgToken>(*this, part);
    }
};


class NewValueToken final : public TerminalTokenClass {
    using baseType = TerminalTokenClass;
    using thisType = NewValueToken;
public:
    HW_PR_GET(string, name){ return "new_value"; };
    ResultData const GetResultData(Context const&context, Category category, SourcePart const&)const{
        return context.ReferenceResult(category, External::Function::NewValue::Instance);
    }
private:
    Ref<Syntax> const Create(SourcePart const&part)const override final{
        return new TerminalSyntax<NewValueToken >(*this, part);
    }
};


class ElseToken final : public InfixTokenClass {
    using baseType = InfixTokenClass;
    using thisType = ElseToken;
public:
    HW_PR_GET(string, name){ return "else"; };

private:
    Ref<Syntax> const Create(Ref<Syntax>const left, SourcePart const& part, Ref<Syntax>const right)const override final
    {
        auto const& conditional = dynamic_cast<IfThenSyntax const&>(*left);
        return conditional.AddElse(part, right);
    }
};


class ThenToken final : public InfixTokenClass {
    using baseType = InfixTokenClass;
    using thisType = ThenToken;
public:
    HW_PR_GET(string, name){ return "then"; };

private:
    Ref<Syntax> const Create(Ref<Syntax>const left, SourcePart const&part, Ref<Syntax>const right)const override final{
        return new IfThenSyntax(left, part, right);
    }
};


class Colon final : public InfixTokenClass{
    typedef InfixTokenClass baseType;
    typedef Colon thisType;
public:
    HW_PR_GET(string, name){return ":";}
private:
    Ref<Syntax> const Create(Ref<Syntax>const left, SourcePart const&part, Ref<Syntax>const right)const override final{
        Ref<SyntaxContainer> result = new SyntaxContainer(part);
        result->Add(left, right);
        return *result;
    }
};


class List final : public TokenClass{
    typedef TokenClass baseType;
    typedef List thisType;
public:
    string const name;
    List(string const&name) : name(name){}
private:
    Ref<Syntax> const CreateSyntax(Optional<Ref<Syntax>>const left, SourcePart const&part, Optional<Ref<Syntax>>const right)const override final
    {
        Ref<SyntaxContainer> result = new SyntaxContainer(part);
        result->Add(left);
        result->Add(right);
        return(*result);
    };
};


class TypeToken final : public SuffixTokenClass{
    typedef SuffixTokenClass baseType; typedef TypeToken thisType;
public:
    HW_PR_GET(string, name){ return "type"; }
private:
    Ref<Syntax > const Create(Ref<Syntax >const left, SourcePart const&part)const  override final{
        return left->TypeOperator(part);
    };
};


MainTokenFactory const MainTokenFactory::Instance;

TokenClass const& MainTokenFactory::Number = *new NumberToken();
TokenClass const& MainTokenFactory::Text = *new TextToken();
TokenClass const& MainTokenFactory::Start = *new LeftParenthesisToken(0);
TokenClass const& MainTokenFactory::End = *new RightParenthesisToken(0);

template<class TTokenClass>
void MainTokenFactory::AddTokenClass(TTokenClass const*tokenClass){
    auto key = tokenClass->name;
    HW_ASSERT(!predefinedTokenClasses.ContainsKey(key), HW_D_VALUE(key) + HW_D_VALUE(predefinedTokenClasses[key]));
    predefinedTokenClasses.Assign(key,&tokenClass->thisRef);
}

MainTokenFactory::MainTokenFactory()
:tokenClasses([](string const& key){return new UserDefinedToken(key);})
, errorClasses([](string const& key){return new SyntaxErrorToken(key); })
{
    AddTokenClass(new ArgToken);
    AddTokenClass(new Colon);
    AddTokenClass(new ColonEqual);
    AddTokenClass(new DumpPrintToken);
    AddTokenClass(new EnableCutToken);
    AddTokenClass(new ElseToken);
    AddTokenClass(new EqualToken);
    AddTokenClass(new FunctionToken);
    AddTokenClass(new GreaterToken);
    AddTokenClass(new InstanceToken);
    AddTokenClass(new LeftParenthesisToken(1));
    AddTokenClass(new LeftParenthesisToken(2));
    AddTokenClass(new LeftParenthesisToken(3));
    AddTokenClass(new List("."));
    AddTokenClass(new List(","));
    AddTokenClass(new List(";"));
    AddTokenClass(new MinusToken);
    AddTokenClass(new NewValueToken);
    AddTokenClass(new PlusToken);
    AddTokenClass(new RightParenthesisToken(1));
    AddTokenClass(new RightParenthesisToken(2));
    AddTokenClass(new RightParenthesisToken(3));
    AddTokenClass(new StarToken);
    AddTokenClass(new ThenToken);
    AddTokenClass(new TypeToken);
}

TokenClass const& MainTokenFactory::GetTokenClass(string const&name){
    return Instance.InternalGetTokenClass(name);
}

TokenClass const& MainTokenFactory::GetErrorClass(string const&name){
    return *Instance.errorClasses(name);
}

TokenClass const& MainTokenFactory::InternalGetTokenClass(string const&name) const
{
    auto result = predefinedTokenClasses.Find(name);
    if(result.IsEmpty)
        return *tokenClasses(name);
    return *result.Value;
}


Ref<Syntax> const SyntaxErrorToken::Create(SourcePart const&part)const{
    HW_D_METHOD(part);
    HW_BREAK_AND_THROW;
}


ResultData const ArgToken::GetResultData(Context const& context, Category category, SourcePart const&) const
{
    bool Trace = false;
    HW_D_METHOD(context, category);
    auto result = context.ReferenceResult(category, External::Function::Arg::Instance);
    return_db(result);
}

