#pragma once
#include <stdlib.h>
#include <functional>
#include "../SetupWarnings.h"
#include "../BreakHandling.h"

using std::function;

#define STRING2(x) #x
#define STRING(x) STRING2(x)
#define ID(x) x
#define COMMA ,
#define QUINE(x) #x COMMA x
