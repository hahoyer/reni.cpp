#pragma once

namespace Reni
{
    class CodeView 
    {
        using thisType = CodeView;
        String const cppCode;

    public:
        CodeView(String const& cppCode);
        void Execute();
        p(String, program);
    private:
        p(String, fileName);
        void InitializeFile();
    };

}

