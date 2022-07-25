#include "atom.h"
#include "predicate.h"

namespace ratio::core
{
    RATIOCORE_EXPORT atom::atom(predicate &pred) : complex_item(pred) {}
} // namespace ratio::core
