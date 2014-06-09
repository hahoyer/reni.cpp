#pragma once

#include "DefaultAssignmentOperator.h"
#include <boost/smart_ptr/shared_ptr.hpp>

namespace HWLib
{
    template<class T>
    bool IsEmptyValue(T const&value);

    template<class T, class TDataContainer>
    class RefBase{
        using thisType = RefBase;
    public:
        typedef TDataContainer dataContainerType;
    protected:
        dataContainerType value;

        RefBase() { }
        RefBase(T &value) : RefBase(&value){}
        RefBase(T *value) : value(value){};
        template<typename TOther, class TOtherData>
        RefBase(RefBase<TOther, TOtherData> const&other) : RefBase(other.value){};
        RefBase(dataContainerType const&value) : value(value){}

    public:
        DefaultAssignmentOperator;
        T const& operator*()const { return *value; };
        T const* operator->()const { return &*value; };
        T & operator*(){ return *value; };
        T * operator->(){ return &*value; };
        bool const operator==(thisType const&other)const
        {
            if(IsEmpty)
                return other.IsEmpty;
            return *value == *other.value;
        };

    protected:
        p(bool, IsEmpty){ return IsEmptyValue(value); }
    };

    template<class T>
    bool IsEmptyValue(boost::shared_ptr<T> const&value){
        return value.get() == null;
    }
}

