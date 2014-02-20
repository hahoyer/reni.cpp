#include "Import.h"
#include "TokenClass.h"
#include "Context.h"
#include "Syntax.h"

static bool Trace = true;

using namespace Reni;

int TokenClass::nextObjectId = 0;
