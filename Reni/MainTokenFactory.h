#pragma once
#include "../HWLib/Map.h"
#include "../HWLib/FunctionCache.h"

using namespace HWLib;
namespace Reni
{
    class TokenClass;

    class MainTokenFactory 
    {
        FunctionCache<Ref<TokenClass>, string> tokenClasses;
        Map<string, Ref<TokenClass>> predefinedTokenClasses;
        FunctionCache<Ref<TokenClass>, string> errorClasses;

        MainTokenFactory();
    public:
        static MainTokenFactory const Instance;

        static TokenClass const& Start;
        static TokenClass const& End;
        static TokenClass const& Number;
        static TokenClass const& Text;
        static TokenClass const& GetTokenClass(string const&name);
        static TokenClass const& GetErrorClass(string const&name);
    private:
        TokenClass const& InternalGetTokenClass(string const&name)const;

        template<class TTokenClass>
        void AddTokenClass(TTokenClass const*tokenClass);
    };

}

