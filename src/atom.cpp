#include "atom.h"
#include "predicate.h"

namespace ratio::core
{
    ORATIOCORE_EXPORT atom::atom(predicate &pred) : complex_item(pred) {}
} // namespace ratio::core
