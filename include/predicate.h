#pragma once
#include "type.h"

namespace riddle::ast
{
  class statement;
} // namespace riddle

namespace ratio::core
{
  class atom;
  class predicate_declaration;

  class predicate : public type
  {
    friend class predicate_declaration;

  public:
    RATIOCORE_EXPORT predicate(scope &scp, const std::string &name, std::vector<field_ptr> args, const std::vector<std::unique_ptr<const riddle::ast::statement>> &stmnts);
    predicate(const predicate &orig) = delete;

    inline std::string get_name() const noexcept { return name; }                 // returns the name of this predicate..
    inline const std::vector<field *> &get_args() const noexcept { return args; } // returns the list of arguments of this predicate..

    RATIOCORE_EXPORT virtual expr new_instance() override; // creates a new instance of this type..

    RATIOCORE_EXPORT void apply_rule(atom &a); // applies the rule associated to this predicate to the given atom..

  protected:
    RATIOCORE_EXPORT void new_field(field_ptr f) noexcept override;

  private:
    std::vector<field *> args;                                                    // the arguments of this predicate..
    const std::vector<std::unique_ptr<const riddle::ast::statement>> &statements; // the statements within the predicate's body..
  };
} // namespace ratio::core
