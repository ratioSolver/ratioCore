#pragma once
#include "type.h"

namespace ratio::core
{
  class atom;

  class predicate : public type
  {
  public:
    predicate(core &cr, const std::string &name);
    predicate(const predicate &orig) = delete;

    void apply_rule(atom &a); // applies the rule associated to this predicate to the given atom..
  };
} // namespace ratio::core
