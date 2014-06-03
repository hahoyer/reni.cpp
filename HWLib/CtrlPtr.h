#pragma once

#include "DefaultAssignmentOperator.h"
#include <boost/shared_ptr.hpp>
#include "RefBase.h"

namespace HWLib
{
    template<typename T>class CtrlRef;

    template<typename T>
    class CtrlPtr final : public RefBase<T, boost::shared_ptr<T>>{
        typedef RefBase<T, boost::shared_ptr<T>> baseType;
        typedef CtrlPtr thisType;
        friend class CtrlRef<T>;
        friend class CtrlPtr<T>;
    public:
        CtrlPtr() : baseType() { }
        CtrlPtr(T *value) :baseType(value){ }
        CtrlPtr(CtrlPtr<T> const&other) : baseType(other){ };
        CtrlPtr(CtrlRef<T> const&other) : baseType(other.value){ };
        template<typename TOther>
        CtrlPtr(CtrlPtr<TOther> const&other) : baseType(other){ };

        DefaultAssignmentOperator;

        p(bool, IsEmpty){ return baseType::IsEmpty; }

        friend thisType const operator||(thisType left, std::function<thisType()> right){
            if(left.IsEmpty)
                return right();
            return left;
        }

        friend thisType const operator||(thisType left, thisType right){
            if(left.IsEmpty)
                return right;
            return left;
        }
    };
}

