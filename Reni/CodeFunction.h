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

        p(String,cppCode);
        p(String, cppDeclaration);
    private:
        p_function(Array<String>, DumpData) override
        {
            return
                {
                    nd(isSetter),
                    nd(index),
                    nd(body)
                };
        }
    };
}
