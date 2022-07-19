#pragma once

#include "env.h"
#include "riddle_parser.h"

namespace ratio::core
{
  class expression : public riddle::ast::expression
  {
  public:
    expression() = default;
    expression(const expression &orig) = delete;
    virtual ~expression() = default;

    virtual expr evaluate(scope &scp, const context &ctx) const = 0;
  };

  class bool_literal_expression : public riddle::ast::bool_literal_expression, public expression
  {
  public:
    bool_literal_expression(const riddle::bool_token &l) : riddle::ast::bool_literal_expression(l) {}
    bool_literal_expression(const bool_literal_expression &orig) = delete;
    virtual ~bool_literal_expression() = default;

    expr evaluate(scope &scp, const context &ctx) const override;
  };

  class int_literal_expression : public riddle::ast::int_literal_expression, public expression
  {
  public:
    int_literal_expression(const riddle::int_token &l) : riddle::ast::int_literal_expression(l) {}
    int_literal_expression(const int_literal_expression &orig) = delete;
    virtual ~int_literal_expression() = default;

    expr evaluate(scope &scp, const context &ctx) const override;
  };

  class real_literal_expression : public riddle::ast::real_literal_expression, public expression
  {
  public:
    real_literal_expression(const riddle::real_token &l) : riddle::ast::real_literal_expression(l) {}
    real_literal_expression(const real_literal_expression &orig) = delete;
    virtual ~real_literal_expression() = default;

    expr evaluate(scope &scp, const context &ctx) const override;
  };

  class string_literal_expression : public riddle::ast::string_literal_expression, public expression
  {
  public:
    string_literal_expression(const riddle::string_token &l) : riddle::ast::string_literal_expression(l) {}
    string_literal_expression(const string_literal_expression &orig) = delete;
    virtual ~string_literal_expression() = default;

    expr evaluate(scope &scp, const context &ctx) const override;
  };

  class cast_expression : public riddle::ast::cast_expression, public expression
  {
  public:
    cast_expression(const std::vector<riddle::id_token> &tp, std::unique_ptr<const riddle::ast::expression> e) : riddle::ast::cast_expression(tp, std::move(e)) {}
    cast_expression(const cast_expression &orig) = delete;
    virtual ~cast_expression() = default;

    expr evaluate(scope &scp, const context &ctx) const override;
  };

  class plus_expression : public riddle::ast::plus_expression, public expression
  {
  public:
    plus_expression(std::unique_ptr<const riddle::ast::expression> e) : riddle::ast::plus_expression(std::move(e)) {}
    plus_expression(const plus_expression &orig) = delete;
    virtual ~plus_expression() = default;

    expr evaluate(scope &scp, const context &ctx) const override;
  };

  class minus_expression : public riddle::ast::minus_expression, public expression
  {
  public:
    minus_expression(std::unique_ptr<const riddle::ast::expression> e) : riddle::ast::minus_expression(std::move(e)) {}
    minus_expression(const minus_expression &orig) = delete;
    virtual ~minus_expression() = default;

    expr evaluate(scope &scp, const context &ctx) const override;
  };

  class not_expression : public riddle::ast::not_expression, public expression
  {
  public:
    not_expression(std::unique_ptr<const riddle::ast::expression> e) : riddle::ast::not_expression(std::move(e)) {}
    not_expression(const not_expression &orig) = delete;
    virtual ~not_expression() = default;

    expr evaluate(scope &scp, const context &ctx) const override;
  };

  class constructor_expression : public riddle::ast::constructor_expression, public expression
  {
  public:
    constructor_expression(std::vector<riddle::id_token> it, std::vector<std::unique_ptr<const riddle::ast::expression>> es) : riddle::ast::constructor_expression(std::move(it), std::move(es)) {}
    constructor_expression(const constructor_expression &orig) = delete;
    virtual ~constructor_expression() = default;

    expr evaluate(scope &scp, const context &ctx) const override;
  };

  class statement : public riddle::ast::statement
  {
  public:
    statement() = default;
    statement(const statement &orig) = delete;
    virtual ~statement() = default;

    virtual void execute(const scope &scp, context &ctx) const = 0;
  };
} // namespace ratio::core
