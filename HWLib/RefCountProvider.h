#pragma once

namespace HWLib
{
    class RefCountProvider
    {
        typedef RefCountProvider thisType;
        mutable int value;
    public:
        RefCountProvider() : value(0){};
        RefCountProvider(RefCountProvider const&) = delete;
        
        RefCountProvider& operator=(RefCountProvider const&) = delete;

        void AddReference()const{value++;}

        void RemoveReference()const
        {
            HW_ASSERT_(value > 0);
            value--;
            if (value)
                return;
            delete this;
        }
    protected:
        virtual ~RefCountProvider(){HW_ASSERT_(value == 0);};
    };
}