#pragma once
#include "scope.h"

namespace riddle::ast
{
  class expression;
  class statement;
} // namespace riddle::ast

namespace ratio::core
{
  class method : public scope
  {
  public:
    RATIOCORE_EXPORT method(scope &scp, type *return_type, const std::string &name, std::vector<field_ptr> args, const std::vector<std::unique_ptr<const riddle::ast::statement>> &stmnts);
    method(const method &orig) = delete;
    RATIOCORE_EXPORT virtual ~method();

    inline type *get_return_type() const noexcept { return return_type; }         // returns the return type of this method (can be nullptr)..
    inline std::string get_name() const noexcept { return name; }                 // returns the name of this method..
    inline const std::vector<field *> &get_args() const noexcept { return args; } // returns the list of arguments of this method..

    expr invoke(context &ctx, std::vector<expr> exprs);

  private:
    type *return_type;                                                            // the return type of this method (can be nullptr)..
    const std::string name;                                                       // the name of this method..
    std::vector<field *> args;                                                    // the arguments of this method..
    const std::vector<std::unique_ptr<const riddle::ast::statement>> &statements; // the statements within the method's body..
  };
} // namespace ratio::core
