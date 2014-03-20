#pragma once
#include "../HWLib/DumpableObject.h"

using namespace HWLib;

namespace Util{
    class Category final : public DumpableObject{
        using baseType = DumpableObject;
        using thisType = Category;
    public:
        bool const hasSize;
        bool const hasCode;
        bool const hasType;

        static Category const None;
        static Category const Size;
        static Category const Code;
        static Category const Type;
        static Category const Instance(bool hasSize, bool hasCode, bool hasType);

        Category();
    private:
        Category(bool hasSize, bool hasCode, bool hasType);

    public:
        DefaultAssignmentOperator;

    public:
        p(Category, typed);
        Category const operator+(Category const other)const;
        Category const operator-(Category const other)const;
        bool operator==(Category const other)const;
        bool operator<=(Category const other)const;
    private:
        p_function(Array<String>,DumpData) override;
        p_function(String, DumpShort) override{ return DumpData.Stringify(","); }
    };
}