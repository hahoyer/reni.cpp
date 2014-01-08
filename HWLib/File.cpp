#include "Import.h"
#include "File.h"

#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include <share.h>
#include <sys/stat.h>
#include <windows.h>

#include "BreakHandling.h"
#include "Console.h"
#include "DumpMacros.h"
#include "DumpToString.h"
#include "Storage.h"
#include "String.h"

using namespace HWLib;

class File::local
{
public:
    String _name;
    local(String const& other);
};

File::File(String const& other)
: _data(HeapAllocate local(other))
{
}

File::~File(){ HeapFree _data; }

String const File::get_FullName()const
{
    auto tReturn = ::_fullpath(0, Name.ToArray.RawData, 0);
    auto Return = String(tReturn);
    delete [] tReturn;
    return Return;
}

void File::set_Name(String const& value)
{
    if (Name == value)
        return;
    auto rc = ::rename(Name.ToArray.RawData, value.ToArray.RawData);
    if (rc == 0)
        _data._name = value;
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
    auto Handle = ::_openX(Name.ToArray.RawData, _O_RDONLY | _O_BINARY);
    auto Length = ::_filelength(Handle);
    if (Handle < 0) Length = 0;
    auto cReturn = new char[Length + 1];
    if (Handle >= 0) ::_read(Handle, cReturn, Length);
    cReturn[Length] = 0;
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


void File::set_Data(String const& value)
{
    auto Length = value.Length;

    int Handle;
    do Handle = _openX(Name.ToArray.RawData, _O_CREAT | _O_TRUNC | _O_WRONLY | _O_BINARY);
    while (Handle < 0 && errno == EACCES);

    auto Error = FormatErrorMessage();
    auto e = errno;

    assertx(Handle >= 0, String("Error: ") + DumpToString(e) + ":" + Error);

    auto WrLength = _write(Handle, value.ToArray.RawData, Length);
    assert(WrLength == Length);

    _close(Handle);
}
