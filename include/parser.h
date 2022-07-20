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

  class bool_literal_expression final : public riddle::ast::bool_literal_expression, public expression
  {
  public:
    bool_literal_expression(const riddle::bool_token &l) : riddle::ast::bool_literal_expression(l) {}
    bool_literal_expression(const bool_literal_expression &orig) = delete;

    expr evaluate(scope &scp, const context &ctx) const override;
  };

  class int_literal_expression final : public riddle::ast::int_literal_expression, public expression
  {
  public:
    int_literal_expression(const riddle::int_token &l) : riddle::ast::int_literal_expression(l) {}
    int_literal_expression(const int_literal_expression &orig) = delete;

    expr evaluate(scope &scp, const context &ctx) const override;
  };

  class real_literal_expression final : public riddle::ast::real_literal_expression, public expression
  {
  public:
    real_literal_expression(const riddle::real_token &l) : riddle::ast::real_literal_expression(l) {}
    real_literal_expression(const real_literal_expression &orig) = delete;

    expr evaluate(scope &scp, const context &ctx) const override;
  };

  class string_literal_expression final : public riddle::ast::string_literal_expression, public expression
  {
  public:
    string_literal_expression(const riddle::string_token &l) : riddle::ast::string_literal_expression(l) {}
    string_literal_expression(const string_literal_expression &orig) = delete;

    expr evaluate(scope &scp, const context &ctx) const override;
  };

  class cast_expression final : public riddle::ast::cast_expression, public expression
  {
  public:
    cast_expression(const std::vector<riddle::id_token> &tp, std::unique_ptr<const riddle::ast::expression> e) : riddle::ast::cast_expression(tp, std::move(e)) {}
    cast_expression(const cast_expression &orig) = delete;

    expr evaluate(scope &scp, const context &ctx) const override;
  };

  class plus_expression final : public riddle::ast::plus_expression, public expression
  {
  public:
    plus_expression(std::unique_ptr<const riddle::ast::expression> e) : riddle::ast::plus_expression(std::move(e)) {}
    plus_expression(const plus_expression &orig) = delete;

    expr evaluate(scope &scp, const context &ctx) const override;
  };

  class minus_expression final : public riddle::ast::minus_expression, public expression
  {
  public:
    minus_expression(std::unique_ptr<const riddle::ast::expression> e) : riddle::ast::minus_expression(std::move(e)) {}
    minus_expression(const minus_expression &orig) = delete;

    expr evaluate(scope &scp, const context &ctx) const override;
  };

  class not_expression final : public riddle::ast::not_expression, public expression
  {
  public:
    not_expression(std::unique_ptr<const riddle::ast::expression> e) : riddle::ast::not_expression(std::move(e)) {}
    not_expression(const not_expression &orig) = delete;

    expr evaluate(scope &scp, const context &ctx) const override;
  };

  class constructor_expression final : public riddle::ast::constructor_expression, public expression
  {
  public:
    constructor_expression(std::vector<riddle::id_token> it, std::vector<std::unique_ptr<const riddle::ast::expression>> es) : riddle::ast::constructor_expression(std::move(it), std::move(es)) {}
    constructor_expression(const constructor_expression &orig) = delete;

    expr evaluate(scope &scp, const context &ctx) const override;
  };

  class eq_expression final : public riddle::ast::eq_expression, public expression
  {
  public:
    eq_expression(std::unique_ptr<const riddle::ast::expression> l, std::unique_ptr<const riddle::ast::expression> r) : riddle::ast::eq_expression(std::move(l), std::move(r)) {}
    eq_expression(const eq_expression &orig) = delete;

    expr evaluate(scope &scp, const context &ctx) const override;
  };

  class neq_expression final : public riddle::ast::neq_expression, public expression
  {
  public:
    neq_expression(std::unique_ptr<const riddle::ast::expression> l, std::unique_ptr<const riddle::ast::expression> r) : riddle::ast::neq_expression(std::move(l), std::move(r)) {}
    neq_expression(const neq_expression &orig) = delete;

    expr evaluate(scope &scp, const context &ctx) const override;
  };

  class lt_expression final : public riddle::ast::lt_expression, public expression
  {
  public:
    lt_expression(std::unique_ptr<const riddle::ast::expression> l, std::unique_ptr<const riddle::ast::expression> r) : riddle::ast::lt_expression(std::move(l), std::move(r)) {}
    lt_expression(const lt_expression &orig) = delete;

    expr evaluate(scope &scp, const context &ctx) const override;
  };

  class leq_expression final : public riddle::ast::leq_expression, public expression
  {
  public:
    leq_expression(std::unique_ptr<const riddle::ast::expression> l, std::unique_ptr<const riddle::ast::expression> r) : riddle::ast::leq_expression(std::move(l), std::move(r)) {}
    leq_expression(const leq_expression &orig) = delete;

    expr evaluate(scope &scp, const context &ctx) const override;
  };

  class geq_expression final : public riddle::ast::geq_expression, public expression
  {
  public:
    geq_expression(std::unique_ptr<const riddle::ast::expression> l, std::unique_ptr<const riddle::ast::expression> r) : riddle::ast::geq_expression(std::move(l), std::move(r)) {}
    geq_expression(const geq_expression &orig) = delete;

    expr evaluate(scope &scp, const context &ctx) const override;
  };

  class gt_expression final : public riddle::ast::gt_expression, public expression
  {
  public:
    gt_expression(std::unique_ptr<const riddle::ast::expression> l, std::unique_ptr<const riddle::ast::expression> r) : riddle::ast::gt_expression(std::move(l), std::move(r)) {}
    gt_expression(const gt_expression &orig) = delete;

    expr evaluate(scope &scp, const context &ctx) const override;
  };

  class statement : public riddle::ast::statement
  {
  public:
    statement() = default;
    statement(const statement &orig) = delete;
    virtual ~statement() = default;

    virtual void execute(const scope &scp, const context &ctx) const = 0;
  };
} // namespace ratio::core
