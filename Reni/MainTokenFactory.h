#pragma once
#include "..\HWLib\FunctionCache.h"
#include <unordered_map>
#include "..\HWLib\String.h"

using namespace HWLib;
namespace Reni
{
    class TokenClass;

    class MainTokenFactory 
    {
        FunctionCache<String const, CtrlRef<TokenClass const>> tokenClasses;
        std::unordered_map<String const, CtrlRef<TokenClass const>> predefinedTokenClasses;
        FunctionCache<String const, CtrlRef<TokenClass const>> errorClasses;

        MainTokenFactory();
    public:
        static MainTokenFactory const Instance;

        static TokenClass const& Start;
        static TokenClass const& End;
        static TokenClass const& Number;
        static TokenClass const& Text;
        static TokenClass const& GetTokenClass(String const&name);
        static TokenClass const& GetErrorClass(String const&name);
    private:
        TokenClass const& InternalGetTokenClass(String const&name)const;

    };

}

