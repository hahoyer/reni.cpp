#pragma once

namespace Reni
{
    class Category final : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = Category;
    public:
        bool const hasSize;
        bool const hasCode;
    public:
        static Category const None;
        static Category const Code;
        Category();
    private:
        Category(bool hasSize, bool hasCode);

    public:
        DefaultAssignmentOperator;
    public:
        Category const operator+(Category const other)const;
        Category const operator-(Category const other)const;
        bool operator==(Category const other)const;
    private:
        override_p_function(Array<String>, DumpData);
    };
}
