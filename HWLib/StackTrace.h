#pragma once

#include "Array.h"
#include <dbghelp.h>
#include "String.h"
#include "MemBox.h"

namespace HWLib
{
    class StackTrace;
    class Thread;
    struct StackFrameData;
    std::string FormatStackTraceOfCurrentThread(int levelToIgnore);

    struct StackFrameData{
        DWORD64 const instructionPointer{};
        std::string fileName{};
        int lineNumber;
        int lineOffset{};
        std::string moduleName{};
        std::string errorMessage{};

        StackFrameData(DWORD64 pcOffset);

        std::string Format()const;
    };

    class StackTrace
    {
        Array<StackFrameData> frames;
    public:
        StackTrace(Thread const& thread);
        std::string Format(size_t StartLevel = 0)const;
    private:
        p(HANDLE, Process){ return GetCurrentProcess(); };
    };


    class Thread
    {
        typedef Thread thisType;
        class StackTraceThread;

        HANDLE handle;

        Thread(HANDLE handle)
            : handle(handle)
        {};

    public:
        ~Thread()
        {
            ::CloseHandle(handle);
        }

        HW_DO_PLACEMENT_ASSIGN;

        bool operator ==(Thread const& x) const { return handle == x.handle; };
        bool operator !=(Thread const& x) const { return !(*this == x); };

        p(HANDLE, Handle){ return handle; };

        void Suspend() const;
        void Resume() const;

        static Thread get_Current(); // returns a real handle (not a pseudohandle, as GetCurrentThread() does)
        static std::string FormatStackTraceOfCurrentThread(int levelToIgnore);
    };


}