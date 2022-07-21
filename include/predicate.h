#pragma once
#include "type.h"

namespace riddle::ast
{
  class statement;
} // namespace riddle

namespace ratio::core
{
  class atom;

  class predicate : public type
  {
  public:
    ORATIOCORE_EXPORT predicate(scope &scp, const std::string &name, std::vector<field_ptr> args, const std::vector<std::unique_ptr<const riddle::ast::statement>> &stmnts);
    predicate(const predicate &orig) = delete;

    inline std::string get_name() const noexcept { return name; }                 // returns the name of this predicate..
    inline const std::vector<field *> &get_args() const noexcept { return args; } // returns the list of arguments of this predicate..

    ORATIOCORE_EXPORT virtual expr new_instance(); // creates a new instance of this type..

    ORATIOCORE_EXPORT void apply_rule(atom &a); // applies the rule associated to this predicate to the given atom..

  private:
    std::vector<field *> args;                                                    // the arguments of this predicate..
    const std::vector<std::unique_ptr<const riddle::ast::statement>> &statements; // the statements within the predicate's body..
  };
} // namespace ratio::core
