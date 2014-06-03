#pragma once

#include "../HWLib/_EditorTemplates.h"
#include "../HWLib/DumpableObject.h"

using namespace HWLib;
using namespace std;

namespace Reni
{
    class External final
        : public DumpableObject
        , public RefCountProvider
    {
        using baseType = DumpableObject;
        using thisType = External;

    };

}

namespace HWLib
{
    template<>
    class Optional<Array<Ref<Reni::External>>> final
    {
        using thisType = Optional;
        using targetType = Array<Ref<Reni::External>>;

        targetType const value;
        bool const _isValid;
    public:
        Optional() : _isValid(false){}
        Optional(decltype(null)) : _isValid(false){}
        Optional(targetType const& value)
            :_isValid(false)
            , value(value)
        {
        }

        p(bool, IsValid){return _isValid;}

        DefaultAssignmentOperator;

        p(targetType, Value){
            a_if_(IsValid);
            return value;
        };

        operator targetType const ()const{ return Value; };

        friend thisType const operator||(thisType left, function<thisType()> right){
            if(left.IsValid)
                return left;
            return right();
        }
        friend thisType const operator||(thisType left, thisType right){
            if(left.IsValid)
                return left;
            return right;
        }
    };
}