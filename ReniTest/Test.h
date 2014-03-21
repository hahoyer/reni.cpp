#pragma once
#include <functional>
using std::function;

#define quine(x) #x COMMA x
#define DefineTest(name) void name(); RuntimeChain<TestFixture> RuntimeChain_##name(name);void name()


namespace HWLib{
    template<class T>
    class RuntimeChain{
        static RuntimeChain const* root;
        typedef typename T::handlerType handlerType;
        handlerType _handler;
    public:
        RuntimeChain const*const next;
        RuntimeChain(handlerType handler);

        static RuntimeChain const* Root(){
            return root;
        };

        p(handlerType, handler){
            return _handler;
        }

    };

    template<class T>
    RuntimeChain<T> const* RuntimeChain<T>::root = {};

    template<class T>
    RuntimeChain<T>::RuntimeChain(handlerType handler)
        : next(root)
          , _handler(handler){
        root = this;
    }

    struct TestFixture{
        typedef function<void()> handlerType;

        static void RunAll(){
            auto rrr = RuntimeChain<TestFixture>::Root();
            while(rrr){
                rrr->handler();
                rrr = rrr->next;
            }
        };
    };
}