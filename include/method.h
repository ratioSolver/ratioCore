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
    method(scope &scp, type_ptr return_type, const std::string &name, std::vector<field_ptr> args, std::vector<std::unique_ptr<const riddle::ast::statement>> stmnts);
    method(const method &orig) = delete;

  private:
    type_ptr return_type;                                                        // the return type of this method (can be nullptr)..
    const std::string name;                                                      // the name of this method..
    const std::vector<field_ptr> args;                                           // the arguments of this method..
    const std::vector<std::unique_ptr<const riddle::ast::statement>> statements; // the statements within the method's body..
  };
} // namespace ratio::core
