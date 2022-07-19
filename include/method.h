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
    ORATIOCORE_EXPORT method(scope &scp, type *return_type, const std::string &name, std::vector<field_ptr> args, std::vector<std::unique_ptr<const riddle::ast::statement>> stmnts);
    method(const method &orig) = delete;
    ORATIOCORE_EXPORT virtual ~method() = default;

  private:
    type *return_type;                                                           // the return type of this method (can be nullptr)..
    const std::string name;                                                      // the name of this method..
    const std::vector<field_ptr> args;                                           // the arguments of this method..
    const std::vector<std::unique_ptr<const riddle::ast::statement>> statements; // the statements within the method's body..
  };
} // namespace ratio::core
