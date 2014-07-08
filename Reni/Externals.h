#pragma once

#include "../HWLib/_EditorTemplates.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/WeakPtr.h"
#include "../HWLib/CtrlRef.h"
#include "../HWLib/DumpToString.h"
#include "../HWLib/Enumerable.h"
#include "../HWLib/Common.h"

using namespace HWLib;
using namespace std;

namespace Reni
{
    class CodeItem;
    class External; 
    class ReplaceVisitor;

    class Externals final : public DumpableObject
    {
        using baseType = DumpableObject; 
        using thisType = Externals;
        Array<WeakRef<External>> const data;
    public:
        Externals() { SetDumpString(); }
        explicit Externals(External const&item);
        Externals(Externals const&other);
        static Externals const Aggregate(CtrlRef<Enumerable<Externals>> const&other);
    private:
        explicit Externals(Array<WeakRef<External>> const&other);
    public:
        ThisRef;
        DefaultAssignmentOperator;

        p(bool, isEmpty){ return data.Count == 0; }

        Externals const operator+(Externals const&other)const;
        bool const operator==(Externals const&other)const;
        bool const operator==(External const&other)const;

        Optional<thisType> const Replace(ReplaceVisitor const&arg) const;
    private:
        p_function(String, DumpHeader) override { return ""; };
        p_function(Array<String>, DumpData) override;
        p_function(String, DumpShort) override{ return Dump; };
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
        p(bool, IsEmpty){ return !IsValid; }

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