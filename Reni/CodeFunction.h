#pragma once
#include "../HWLib/DumpableObject.h"
#include "CodeVisitor.h"

using namespace HWLib;

namespace Reni
{
    class CodeFunction final : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = CodeFunction;
    public:
        class CodeVisitor;
    private:
        size_t const index;
        bool const isSetter;
        Ref<CodeItem> const body;
    public:
        static thisType const Getter(size_t index, Ref<CodeItem> const& body)
        {
            return thisType(false, index, body);
        }

    private:
        CodeFunction(bool isSetter, size_t index, Ref<CodeItem> const& body);
    public:
        bool const operator==(thisType const&other)const
        {
            return index == other.index
                && isSetter == other.isSetter;
        };

        p(string,cppCode);
        p(string, cppDeclaration);
    private:
        p_function(Array<string>, DumpData) override
        {
            return
                {
                    HW_D_VALUE(isSetter),
                    HW_D_VALUE(index),
                    HW_D_VALUE(body)
                };
        }
    };
}
