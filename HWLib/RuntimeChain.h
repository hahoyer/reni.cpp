#pragma once
#include <functional>
#include "../HWLib/String.h"
using std::function;

namespace HWLib{
    template<class T>
    class RuntimeChain final{
        typedef typename T::dataType dataType;

        class Iterator final : public Enumerable<dataType>::Iterator{
            RuntimeChain<T> const* current;
        public:

            Iterator()
                : current(begin){
            }

        protected:
            p_function(bool, IsValid) override{
                return !!current;
            };

            dataType const Step() override;
        };

        static RuntimeChain const* begin;
        dataType data;
        RuntimeChain const*const next;

    public:
        RuntimeChain(dataType data);

        static CtrlRef<Enumerable<dataType>> const All(){
            return new Enumerable<dataType>::Container(new Iterator);
        };

    };

    template<class T>
    RuntimeChain<T> const* RuntimeChain<T>::begin = {};

    template<class T>
    RuntimeChain<T>::RuntimeChain(dataType data)
        : next(begin)
          , data(data){
        begin = this;
    }

    template<class T>
    typename T::dataType const RuntimeChain<T>::Iterator::Step() {
        auto result = current->data;
        current = current->next;
        return result;
    }
}