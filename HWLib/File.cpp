#include "Import.h"
#include "File.h"

#include "Common.h"
#include "Console.h"
#include "DumpableObject.h"
#include "DumpMacros.h"
#include "DumpToString.h"
#include "String.h"
#include "Optional.h"
#include "System.h"
#include "TemplateInstances.h"

#include <direct.h>
#include <fcntl.h>
#include <io.h>
#include <stdexcept>
#include <sys/stat.h>

#undef DeleteFile

using namespace HWLib;

class File::internal final
{
public:
  std::string Name;

  internal(const std::string& name)
    : Name(name)
  {};

  p_mutable(string, Data);

  p_mutable(bool, IsValid)
  {
    struct _stat buf{};
    const auto result = _stat(Name.c_str(), &buf);
    return result == 0;
  }

  p_mutable(bool, IsValidFolder)
  {
    struct _stat buf{};
    const auto result = _stat(Name.c_str(), &buf);
    return result == 0 && (buf.st_mode & _S_IFDIR) != 0;
  }

private:
  void CreateFolder() const
  {
    if(_mkdir(Name.c_str()) == 0)
      return;
    System::ThrowLastErrorMessage();
  }

  void CreateFolderChain() const
  {
    string head;
    for(auto element : (Name | Split("\\"))->ToArray)
    {
      if(head == "")
        head = element;
      else
        head += "\\" + element;
      if(head | EndsWith(":"))
        continue;

      internal headFile = head;
      if(!headFile.IsValid)
        headFile.CreateFolder();
      else if(!headFile.IsValidFolder)
        throw runtime_error("Error: file exits at requested location.");
    };
  };

  void DeleteFolder() const
  {
    if(_rmdir(Name.c_str()) == 0)
      return;
    System::ThrowLastErrorMessage();
  };

  void DeleteFile() const
  {
    if(::remove(Name.c_str()) == 0)
      return;
    System::ThrowLastErrorMessage();
  };
};


p_mutator_implementation(File::internal, bool, IsValidFolder)
{
  if(value)
    CreateFolderChain();
  else
    DeleteFolder();
}

p_mutator_implementation(File::internal, bool, IsValid)
{
  if(value)
    Data = "";
  else
    DeleteFile();
}

FILE* OpenFile(const char* name, bool isWrite)
{
  FILE* file;
  while(file = _fsopen(name, isWrite ? "wbS" : "rbS", _SH_DENYNO), file == nullptr && errno == EACCES)
    continue;
  return file;
}

p_implementation(File::internal, string, Data)
{
  const auto file = OpenFile(Name.c_str(), false);
  if(file == nullptr)
    return "";

  _fseeki64(file, 0,SEEK_END);
  const auto count = _ftelli64(file);
  string result(count, 0);
  fread(result.data(), result.size(), count, file);
  fclose(file);
  return result;
}

p_mutator_implementation(File::internal, string, Data)
{
  FILE* file = OpenFile(Name.c_str(), true);
  HW_ASSERT(file, string("Error: ") + HWLib::Dump(errno) + ":" + System::FormatLastErrorMessage());

  const auto count = value.size();
  const auto countWritten = fwrite(value.c_str(), count, count, file);
  fclose(file);
  HW_ASSERT(count == countWritten,
       string("Error: ") + HWLib::Dump(errno) + ":" + System::FormatLastErrorMessage() + "\ncount=" + to_string(count) +
       " countWritten=" + to_string(countWritten));
}


File::File(const string& name)
  : _internal(new internal(name)) {}

p_implementation(File, string, FullName)
{
  const auto temporaryResult = _fullpath(nullptr, Name.c_str(), 0);
  const auto result = string(temporaryResult);
  delete[] temporaryResult;
  return result;
}

p_implementation(File, string, Name)
{
  return _internal->Name;
}

p_mutator_implementation(File, string, Name)
{
  if(Name == value)
    return;
  const auto rc = rename(Name.c_str(), value.c_str());
  if(rc == 0)
    _internal->Name = value;
  HW_ASSERT_IS(rc, ==, 0);
}

p_implementation(File, string, Data)
{
  return _internal->Data;
}

p_mutator_implementation(File, string, Data)
{
  _internal->Data = value;
}

p_implementation(File, bool, IsValid)
{
  return _internal->IsValid;
}

p_implementation(File, bool, IsValidFolder)
{
  return _internal->IsValidFolder;
}

p_mutator_implementation(File, bool, IsValidFolder)
{
  _internal->IsValidFolder = value;
}
