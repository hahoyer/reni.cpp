#include "Import.h"
#include "Code.h"

using namespace Reni;
static bool Trace = true;

static int nextObjectId = 0;


CodeItem::CodeItem()
: baseType(nextObjectId++)
{}


CodeView::CodeView()
: baseType(nextObjectId++)
{}

