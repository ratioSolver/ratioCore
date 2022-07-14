#include "predicate.h"

namespace ratio::core
{
    predicate::predicate(core &cr, const std::string &name) : type(cr, name) {}

    void predicate::apply_rule(atom &a)
    {
        for (const auto &sp : supertypes)
            static_cast<predicate *>(sp)->apply_rule(a);
    }
} // namespace ratio::core
