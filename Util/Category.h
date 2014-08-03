#pragma once
#include "../HWLib/DumpableObject.h"

using namespace HWLib;

namespace Util{
    class Category final : public DumpableObject{
        using baseType = DumpableObject;
        using thisType = Category;
    public:
        bool const hasHllw;
        bool const hasSize;
        bool const hasCode;
        bool const hasType;
        bool const hasExts;

        static Category const None;
        static Category const Hllw;
        static Category const Size;
        static Category const Code;
        static Category const Type;
        static Category const Exts;
        static Category const Instance(bool hasHllw, bool hasSize, bool hasCode, bool hasType, bool hasExts);

        Category(Category const& other);

        Category();
    private:
        Category(bool hasHllw, bool hasSize, bool hasCode, bool hasType, bool hasExts);

    public:
        DefaultAssignmentOperator;

        p(Category, typed);
        p(Category, replenished);
        Category const operator|(Category const other)const;
        Category const operator&(Category const other)const;
        Category const operator-(Category const other)const;
        bool operator==(Category const other)const;
        bool operator<=(Category const other)const;

        void operator|= (Category const other){ *this = *this | other; }
        void operator-= (Category const other){ *this = *this - other; }
    private:
        p_function(String, DumpHeader) override{ return ""; }
        p_function(Array<String>, DumpData) override;
        p_function(String, DumpShort) override{ return DumpData.Stringify(","); }
    };
}