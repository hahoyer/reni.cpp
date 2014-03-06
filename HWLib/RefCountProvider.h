#pragma once

namespace HWLib{
    class RefCountProvider{
        typedef RefCountProvider thisType;
        mutable int value;
    public:

        RefCountProvider() : value(0){
        };

        RefCountProvider(RefCountProvider const&) = delete;

        ~RefCountProvider(){
            if (value) 
                throw "Invalid delete";
        };

        void AddReference()const{
            value++;
        }

        void RemoveReference()const{
            a_if_(value > 0);
            value--;
            if (value)
                return;
            delete this;
        }
    };
}