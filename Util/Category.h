#pragma once
#include "../HWLib/DumpableObject.h"

using namespace HWLib;

namespace Util{
    class Category final : public DumpableObject{
        using baseType = DumpableObject;
        using thisType = Category;
    public:
        bool const hasHollow;
        bool const hasSize;
        bool const hasCode;
        bool const hasType;
        bool const hasClosure;

        static Category const None;
        static Category const Hollow;
        static Category const Size;
        static Category const Code;
        static Category const Type;
        static Category const Closure;
        static Category Instance(bool hasHollow, bool hasSize, bool hasCode, bool hasType, bool hasClosure);

        Category(Category const& other);

        Category();
    private:
        Category(bool hasHollow, bool hasSize, bool hasCode, bool hasType, bool hasClosure);

    public:
        HW_DO_PLACEMENT_ASSIGN;

        p(Category, typed);
        p(Category, replenished);
        Category operator|(const Category& other) const;
        Category operator&(Category const& other) const;
        Category operator-(Category const& other) const;
        bool operator==(Category const& other)const;
        bool operator<=(Category const& other)const;

        void operator|= (Category const other){ *this = *this | other; }
        void operator-= (Category const other){ *this = *this - other; }
    private:
        p_function(string, DumpHeader) override{ return ""; }
        p_function(Array<string>, DumpData) override;
        p_function(string, DumpShort) override{ return DumpData.Stringify(","); }
    };
}