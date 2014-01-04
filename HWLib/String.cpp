#include "HWLib.imp.h"
#include "String.h"

#include "windows.h"

using namespace HWLib;

void CString::OutputDebug()const{::OutputDebugStringA(_data);}