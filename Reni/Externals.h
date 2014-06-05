#pragma once

#include "../HWLib/_EditorTemplates.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/WeakPtr.h"
#include "../HWLib/CtrlRef.h"

using namespace HWLib;
using namespace std;

namespace Reni
{
    class External; 

    class Externals final : public DumpableObject
    {
        using baseType = DumpableObject; 
        using thisType = Externals;
        Array<WeakRef<External>> const data;
    public:
        Externals() { SetDumpString(); }
        Externals(External const&item);

    private:
        Externals(Array<WeakRef<External>> const&other);
    public:
        ThisRef;
        DefaultAssignmentOperator;

        Externals const operator+(Externals const&other)const;
    private:
        p_function(Array<String>, DumpData) override;
    };
}

namespace HWLib
{
    template<>
    class Optional<Reni::Externals > final
    {
        using thisType = Optional;
        using targetType = Reni::Externals ;

        targetType const value;
        bool const _isValid;
    public:
        Optional() : _isValid(false){}
        Optional(decltype(null)) : _isValid(false){}
        Optional(targetType const& value)
            :_isValid(true)
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