#pragma once
#include "..\HWLib\FunctionCache.h"
#include "..\HWLib\String.h"
#include "../HWLib/Map.h"

using namespace HWLib;
namespace Reni
{
    class TokenClass;

    class MainTokenFactory 
    {
        FunctionCache<Ref<TokenClass>, String> tokenClasses;
        Map<String, Ref<TokenClass>> predefinedTokenClasses;
        FunctionCache<Ref<TokenClass>, String> errorClasses;

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

        template<class TTokenClass>
        void AddTokenClass(TTokenClass const*tokenClass);
    };

}

