#pragma once
#include "type.h"

namespace ratio::core
{
  class atom;

  class predicate : public type
  {
  public:
    ORATIOCORE_EXPORT predicate(core &cr, const std::string &name, std::vector<field_ptr> args);
    predicate(const predicate &orig) = delete;

    inline std::string get_name() const noexcept { return name; }                 // returns the name of this predicate..
    inline const std::vector<field *> &get_args() const noexcept { return args; } // returns the list of arguments of this predicate..

    ORATIOCORE_EXPORT virtual expr new_instance(); // creates a new instance of this type..

    ORATIOCORE_EXPORT void apply_rule(atom &a); // applies the rule associated to this predicate to the given atom..

  private:
    std::vector<field *> args; // the arguments of this predicate..
  };
} // namespace ratio::core
