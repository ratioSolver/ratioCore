#pragma once
#include "scope.h"

namespace riddle
{
  class id_token;

  namespace ast
  {
    class expression;
    class statement;
  } // namespace ast

} // namespace riddle

namespace ratio::core
{
  class type;
  class complex_item;
  class constructor : public scope
  {
    friend class type;

  public:
    constructor(type &tp, std::vector<field_ptr> args, std::vector<riddle::id_token> ins, std::vector<std::vector<std::unique_ptr<const riddle::ast::expression>>> ivs, std::vector<std::unique_ptr<const riddle::ast::statement>> stmnts);
    constructor(const constructor &orig) = delete;

    expr new_instance(const std::vector<expr> &exprs) noexcept; // creates a new instance of an item whose type has this constructor invoking this constructor within the given context with the given expressions as arguments of the constructor..

  private:
    void invoke(complex_item &itm, const std::vector<expr> &exprs);

  private:
    const std::vector<field_ptr> args;                                                        // the arguments of this constructor..
    const std::vector<riddle::id_token> init_names;                                           // the parameter names in the init-list..
    const std::vector<std::vector<std::unique_ptr<const riddle::ast::expression>>> init_vals; // for each parameter name in the init-list, its initializzation values..
    const std::vector<std::unique_ptr<const riddle::ast::statement>> statements;              // the statements within the constructor's body..
  };
} // namespace ratio::core
