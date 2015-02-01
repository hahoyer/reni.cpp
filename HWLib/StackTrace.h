#pragma once

#include "Array.h"
#include <dbghelp.h>
#include "String.h"
#include "MemBox.h"

namespace HWLib
{
    class StackTrace;
    class Thread;
    class StackFrameData;
    String const FormatStackTraceOfCurrentThread(int levelToIgnore);

    struct StackFrameData{
        DWORD64 const instructionPointer;
        String fileName;
        int lineNumber;
        int lineOffset;
        String modulName;
        String errorMessage;

        StackFrameData(DWORD64 pcOffset);

        String const Format()const;
    };

    class StackTrace
    {
        Array<StackFrameData> frames;
    public:
        StackTrace(Thread const& thread);
        String const Format(int StartLevel = 0)const;
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

        DefaultAssignmentOperator;

        bool const operator ==(Thread const&x)const{ return handle == x.handle; };
        bool const operator !=(Thread const&x)const{ return !(*this == x); };

        p(HANDLE, Handle){ return handle; };

        void Suspend();
        void Resume();

        static Thread get_Current(); // returns a real handle (not a pseudohandle, as GetCurrentThread() does)
        static String const FormatStackTraceOfCurrentThread(int levelToIgnore);
    };


}