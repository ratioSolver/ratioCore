#include "predicate.h"

namespace ratio::core
{
    ORATIOCORE_EXPORT predicate::predicate(core &cr, const std::string &name) : type(cr, name) {}

    ORATIOCORE_EXPORT void predicate::apply_rule(atom &a)
    {
        for (const auto &sp : supertypes)
            static_cast<predicate *>(sp)->apply_rule(a);
    }
} // namespace ratio::core
