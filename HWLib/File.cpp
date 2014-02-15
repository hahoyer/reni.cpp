#include "Import.h"
#include "File.h"

#include "BreakHandling.h"
#include "Common.h"
#include "Console.h"
#include "String.h"
#include "DumpMacros.h"
#include "DumpToString.h"

#include <sys/stat.h>
#include <io.h>
#include <fcntl.h>
#include <windows.h>

using namespace HWLib;

class File::internal
{
public:
    static bool const EnableDumpFromRef = true;
    String _name;
    internal(String const& name) : _name(name){};
};

File::File(String const& name)
: _internal(new internal(name))
{
}

p_implementation(File, String, FullName)
{
    auto tReturn = ::_fullpath(0, Name.RawData, 0);
    auto Return = String(tReturn);
    delete[] tReturn;
    return Return;
}

p_implementation(File, String, Name)
{
    return _internal->_name;
}

p_mutator_implementation(File, String, Name)
{
    if (Name == value)
        return;
    auto rc = ::rename(Name.RawData, value.RawData);
    if (rc == 0)
        _internal->_name = value;
    assertx(rc == 0, vardump(rc));
}

static int _openX(char const* name, int oflag, int pmode = _S_IREAD | _S_IWRITE)
{
    int result;
    auto rc = _sopen_s(&result, name, oflag, SH_DENYNO, pmode);
    return result;
};

String const File::get_Data()const
{
    auto Handle = ::_openX(Name.RawData, _O_RDONLY | _O_BINARY);
    auto Count = ::_filelength(Handle);
    if (Handle < 0) Count = 0;
    auto cReturn = new char[Count + 1];
    if (Handle >= 0) ::_read(Handle, cReturn, Count);
    cReturn[Count] = 0;
    ::_close(Handle);
    auto Return = String(cReturn);
    delete cReturn;
    return Return;
}

String const FormatErrorMessage()
{
    DWORD rc = ::GetLastError();
    char Buffer[3000];
    ::FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        rc,
        0,
        Buffer, 3000,
        NULL);
    return Buffer;
};

p_mutator_implementation(File, String, Data)
{
    auto Count = value.Count;

    int Handle;
    do Handle = _openX(Name.RawData, _O_CREAT | _O_TRUNC | _O_WRONLY | _O_BINARY);
    while (Handle < 0 && errno == EACCES);

    auto Error = FormatErrorMessage();
    auto e = errno;

    assertx(Handle >= 0, String("Error: ") + HWLib::Dump(e) + ":" + Error);

    auto WrLength = _write(Handle, value.RawData, Count);
    assert(WrLength == Count);

    _close(Handle);
}

