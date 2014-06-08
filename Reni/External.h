#pragma once

#include "../HWLib/_EditorTemplates.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/RefCountProvider.h"

using namespace HWLib;
using namespace std;

namespace Reni
{
    class ReplaceVisitor;

    class External 
        : public WithId<DumpableObject, External>
    {
        using baseType = DumpableObject;
        using thisType = External;

    public:
        class This;
        class Arg;
        class Function;

        ThisRef;
        bool const operator < (thisType const& other)const;
        virtual bool const IsProvided(ReplaceVisitor const&) const { return false; };
        p_function(String, DumpShort) override{ return{internalDump()}; };
    private:
        p_function(Array<String>, DumpData) override{ return{internalDump()}; };
        p_function(String, DumpHeader) override{ return ""; };
    protected:
        virtual String const internalDump()const = 0;
    };

    class External::This final : public External
    {
        using baseType = External;
        using thisType = This;
    public:
        static thisType const Instance;
    private:
        This() {};    
        bool const IsProvided(ReplaceVisitor const& arg) const override;
        String const internalDump() const override{ return "This"; };
    };

    class External::Arg final : public External
    {
        using baseType = External;
        using thisType = Arg;
    public:
        static thisType const Instance;
    private:
        Arg() {};     
        bool const IsProvided(ReplaceVisitor const& arg) const override;
        String const internalDump() const override{ return "Arg"; };
    };

    class External::Function : public External
    {
        using baseType = External;
        using thisType = Function;
    public:
        class Arg;
        class NewValue;
    protected:
        Function() {};
        String const internalDump() const override{ return "/\\"; };
    };

    class External::Function::Arg final : public Function
    {
        using baseType = Function;
        using thisType = Arg;
    public:
        static thisType const Instance;
    private:
        Arg() {};
        String const internalDump() const override{ return baseType::internalDump() + "arg"; };
    };

    class External::Function::NewValue final : public Function
    {
        using baseType = Function;
        using thisType = NewValue;
    public:
        static thisType const Instance;
    private:
        NewValue() {};
        String const internalDump() const override{ return baseType::internalDump() + "new_value"; };
    };
}
