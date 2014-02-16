#pragma once

#include "OpenItem.h"

namespace HWLang
{
#if 0
    template <typename Syntax, typename TScanner, class Token>
    class PositionManager
    {
        TScanner _scanner;
    public:
        PositionManager(Ref<Source const> source)
            : _scanner(source, factory)
            , StartToken(startToken)
        {};

        Token const StartToken;
        Token const Step(Stack<OpenItem<Syntax, Token>>*){ return _scanner.Step(); }
    };



#endif
}
