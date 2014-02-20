#include "Import.h"
#include "MainTokenFactory.h"
#include "TokenClass.h"
#include "Syntax.h"

bool Trace = true;
using namespace Reni;

class TokenClassBase : public TokenClass{
    using baseType = TokenClass;
    using thisType = TokenClassBase;
public:
    virtual Ref<Syntax const> const CreateSyntax(Ref<Syntax const>const left, SourcePart const&part, Ref<Syntax const>const right, bool isMatch)const override{
        d_here;
        md(left, part, right, isMatch);
        b_;
        return OptRef<Syntax const>();
    };
    virtual Ref<Syntax const> const CreateSyntax(Ref<Syntax const>const left, SourcePart const&part, bool isMatch)const  override{
        d_here;
        md(left, part, isMatch);
        b_;
        return OptRef<Syntax const>();
    };
    virtual Ref<Syntax const> const CreateSyntax(SourcePart const&part, Ref<Syntax const>const right, bool isMatch)const  override{
        d_here;
        md(part, right, isMatch);
        b_;
        return OptRef<Syntax const>();
    };
    virtual Ref<Syntax const> const CreateSyntax(SourcePart const&part, bool isMatch)const  override{
        md(part, isMatch);
        b_;
        return OptRef<Syntax const>();
    };
private:
    override_p_function(Array<String>, DumpData){
        return{};
    };
};


class NumberToken final : public TokenClassBase{
    using baseType = TokenClassBase;
    using thisType = NumberToken;

    virtual Ref<Syntax const> const CreateSyntax(SourcePart const&part, bool isMatch)const  override{
        if (isMatch)
            return baseType::CreateSyntax(part, isMatch);
        return new TerminalSyntax(*this, part);
    };
};


class TextToken final : public TokenClassBase{
    using baseType = TokenClassBase;
    using thisType = TextToken;
};


class DefineableToken final : public TokenClassBase{
    using baseType = TokenClassBase;
    using thisType = DefineableToken;
    
    String const name;
public:
    DefineableToken(String const name) : name(name){}
private:
    virtual Ref<Syntax const> const CreateSyntax(Ref<Syntax const>const left, SourcePart const&part, bool isMatch)const  override{
        if (isMatch)
            return baseType::CreateSyntax(left, part, isMatch);
        return new SuffixSyntax(left, *this, part);
    };

    virtual Ref<Syntax const> const CreateSyntax(SourcePart const&part, bool isMatch)const  override{
        if (isMatch)
            return baseType::CreateSyntax(part, isMatch);
        return new TerminalSyntax(*this, part);
    };

    override_p_function(Array<String>, DumpData){
        return{ nd(name) };
    };
};


class SyntaxErrorToken final : public TokenClassBase{
    using baseType = TokenClassBase;
    using thisType = DefineableToken;

    String const text;
public:
    SyntaxErrorToken(String const text) : text(text){}
private:
    override_p_function(Array<String>, DumpData){
        return{nd(text)};
    };
};


class LeftParenthesisToken final : public TokenClassBase{
    using baseType = TokenClassBase;
    using thisType = DefineableToken;

    int const level;
public:
    LeftParenthesisToken(int level) : level(level){}
private:
    virtual Ref<Syntax const> const CreateSyntax(SourcePart const&part, Ref<Syntax const>const right, bool isMatch)const  override{
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
        Ref<Syntax const> const right;
    public:
        OpenSyntax(int level, SourcePart const part, Ref<Syntax const> const right)
            : baseType(part)
            , level(level)
            , right(right)
        {
            SetDumpString();
        }
    private:
        virtual Ref<Syntax const> const ParenthesisMatch(int level, SourcePart const&part)const override{
            if (level != this->level)
                return baseType::ParenthesisMatch(level, part);
            return right;
        };
        override_p_function(Array<String>, DumpData){
            return{ nd(level), nd(right) };
        }
    };
};


class RightParenthesisToken final : public TokenClassBase{
    using baseType = TokenClassBase;
    using thisType = DefineableToken;

    int const level;
public:
    RightParenthesisToken(int level) : level(level){}
private:
    virtual Ref<Syntax const> const CreateSyntax(Ref<Syntax const>const left, SourcePart const&part, bool isMatch)const  override{
        if (!isMatch)
            return baseType::CreateSyntax(left, part, isMatch);
        return left->ParenthesisMatch(level, part);
    };

    override_p_function(Array<String>, DumpData){
        return{ nd(level) };
    };
};


Ref<TokenClass const> const TokenClass::Pending = new SyntaxErrorToken("");



MainTokenFactory const MainTokenFactory::Instance;

TokenClass const& MainTokenFactory::Number = NumberToken();
TokenClass const& MainTokenFactory::Text = TextToken();
TokenClass const& MainTokenFactory::Start = LeftParenthesisToken(0);
TokenClass const& MainTokenFactory::End = RightParenthesisToken(0);

MainTokenFactory::MainTokenFactory()
:tokenClasses([](String const& key){return new DefineableToken(key); })
, errorClasses([](String const& key){return new SyntaxErrorToken(key); })
{}

TokenClass const& MainTokenFactory::GetTokenClass(String const&name){
    return *Instance.tokenClasses[name];
}

TokenClass const& MainTokenFactory::GetErrorClass(String const&name){
    return *Instance.errorClasses[name];
}


