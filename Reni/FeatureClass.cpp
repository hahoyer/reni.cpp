#include "Import.h"
#include "FeatureClass.h"

#include "SearchResult.h"

static bool Trace = true;

using namespace Reni;
using namespace HWLib;

bool const DefinitionPoint::Defines(FeatureClass const&feature)const{
    md(feature);
    b_;
    return false;
};

SearchResult const DefinitionPoint::Apply(FeatureClass const&feature)const{
    md(feature);
    b_;
    return {};
}

