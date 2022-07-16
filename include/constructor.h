#pragma once
#include "scope.h"

namespace riddle::ast
{
  class expression;
  class statement;
} // namespace riddle::ast

namespace ratio::core
{
  class type;
  class constructor : public scope
  {
  public:
    constructor(type &tp, const std::vector<field_ptr> &args, std::vector<std::pair<const std::string, const std::vector<std::unique_ptr<riddle::ast::expression>>>> il, std::vector<std::unique_ptr<riddle::ast::statement>> stmnts);
    constructor(const constructor &orig) = delete;

  private:
    const std::vector<field_ptr> args;
    const std::vector<std::pair<const std::string, const std::vector<std::unique_ptr<riddle::ast::expression>>>> init_list;
    const std::vector<std::unique_ptr<riddle::ast::statement>> statements;
  };
} // namespace ratio::core
