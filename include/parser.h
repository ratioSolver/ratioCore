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

    virtual expr evaluate(scope &scp, context &ctx) const = 0;
  };

  class bool_literal_expression final : public riddle::ast::bool_literal_expression, public expression
  {
  public:
    bool_literal_expression(const riddle::bool_token &l) : riddle::ast::bool_literal_expression(l) {}
    bool_literal_expression(const bool_literal_expression &orig) = delete;

   RATIOCORE_EXPORT expr evaluate(scope &scp, context &ctx) const override;
  };

  class int_literal_expression final : public riddle::ast::int_literal_expression, public expression
  {
  public:
    int_literal_expression(const riddle::int_token &l) : riddle::ast::int_literal_expression(l) {}
    int_literal_expression(const int_literal_expression &orig) = delete;

   RATIOCORE_EXPORT expr evaluate(scope &scp, context &ctx) const override;
  };

  class real_literal_expression final : public riddle::ast::real_literal_expression, public expression
  {
  public:
    real_literal_expression(const riddle::real_token &l) : riddle::ast::real_literal_expression(l) {}
    real_literal_expression(const real_literal_expression &orig) = delete;

  RATIOCORE_EXPORT  expr evaluate(scope &scp, context &ctx) const override;
  };

  class string_literal_expression final : public riddle::ast::string_literal_expression, public expression
  {
  public:
    string_literal_expression(const riddle::string_token &l) : riddle::ast::string_literal_expression(l) {}
    string_literal_expression(const string_literal_expression &orig) = delete;

  RATIOCORE_EXPORT  expr evaluate(scope &scp, context &ctx) const override;
  };

  class cast_expression final : public riddle::ast::cast_expression, public expression
  {
  public:
    cast_expression(const std::vector<riddle::id_token> &tp, std::unique_ptr<const riddle::ast::expression> e) : riddle::ast::cast_expression(tp, std::move(e)) {}
    cast_expression(const cast_expression &orig) = delete;

   RATIOCORE_EXPORT expr evaluate(scope &scp, context &ctx) const override;
  };

  class plus_expression final : public riddle::ast::plus_expression, public expression
  {
  public:
    plus_expression(std::unique_ptr<const riddle::ast::expression> e) : riddle::ast::plus_expression(std::move(e)) {}
    plus_expression(const plus_expression &orig) = delete;

   RATIOCORE_EXPORT expr evaluate(scope &scp, context &ctx) const override;
  };

  class minus_expression final : public riddle::ast::minus_expression, public expression
  {
  public:
    minus_expression(std::unique_ptr<const riddle::ast::expression> e) : riddle::ast::minus_expression(std::move(e)) {}
    minus_expression(const minus_expression &orig) = delete;

   RATIOCORE_EXPORT expr evaluate(scope &scp, context &ctx) const override;
  };

  class not_expression final : public riddle::ast::not_expression, public expression
  {
  public:
    not_expression(std::unique_ptr<const riddle::ast::expression> e) : riddle::ast::not_expression(std::move(e)) {}
    not_expression(const not_expression &orig) = delete;

   RATIOCORE_EXPORT expr evaluate(scope &scp, context &ctx) const override;
  };

  class constructor_expression final : public riddle::ast::constructor_expression, public expression
  {
  public:
    constructor_expression(std::vector<riddle::id_token> it, std::vector<std::unique_ptr<const riddle::ast::expression>> es) : riddle::ast::constructor_expression(std::move(it), std::move(es)) {}
    constructor_expression(const constructor_expression &orig) = delete;

   RATIOCORE_EXPORT expr evaluate(scope &scp, context &ctx) const override;
  };

  class eq_expression final : public riddle::ast::eq_expression, public expression
  {
  public:
    eq_expression(std::unique_ptr<const riddle::ast::expression> l, std::unique_ptr<const riddle::ast::expression> r) : riddle::ast::eq_expression(std::move(l), std::move(r)) {}
    eq_expression(const eq_expression &orig) = delete;

   RATIOCORE_EXPORT expr evaluate(scope &scp, context &ctx) const override;
  };

  class neq_expression final : public riddle::ast::neq_expression, public expression
  {
  public:
    neq_expression(std::unique_ptr<const riddle::ast::expression> l, std::unique_ptr<const riddle::ast::expression> r) : riddle::ast::neq_expression(std::move(l), std::move(r)) {}
    neq_expression(const neq_expression &orig) = delete;

   RATIOCORE_EXPORT expr evaluate(scope &scp, context &ctx) const override;
  };

  class lt_expression final : public riddle::ast::lt_expression, public expression
  {
  public:
    lt_expression(std::unique_ptr<const riddle::ast::expression> l, std::unique_ptr<const riddle::ast::expression> r) : riddle::ast::lt_expression(std::move(l), std::move(r)) {}
    lt_expression(const lt_expression &orig) = delete;

   RATIOCORE_EXPORT expr evaluate(scope &scp, context &ctx) const override;
  };

  class leq_expression final : public riddle::ast::leq_expression, public expression
  {
  public:
    leq_expression(std::unique_ptr<const riddle::ast::expression> l, std::unique_ptr<const riddle::ast::expression> r) : riddle::ast::leq_expression(std::move(l), std::move(r)) {}
    leq_expression(const leq_expression &orig) = delete;

   RATIOCORE_EXPORT expr evaluate(scope &scp, context &ctx) const override;
  };

  class geq_expression final : public riddle::ast::geq_expression, public expression
  {
  public:
    geq_expression(std::unique_ptr<const riddle::ast::expression> l, std::unique_ptr<const riddle::ast::expression> r) : riddle::ast::geq_expression(std::move(l), std::move(r)) {}
    geq_expression(const geq_expression &orig) = delete;

   RATIOCORE_EXPORT expr evaluate(scope &scp, context &ctx) const override;
  };

  class gt_expression final : public riddle::ast::gt_expression, public expression
  {
  public:
    gt_expression(std::unique_ptr<const riddle::ast::expression> l, std::unique_ptr<const riddle::ast::expression> r) : riddle::ast::gt_expression(std::move(l), std::move(r)) {}
    gt_expression(const gt_expression &orig) = delete;

   RATIOCORE_EXPORT expr evaluate(scope &scp, context &ctx) const override;
  };

  class function_expression final : public riddle::ast::function_expression, public expression
  {
  public:
    function_expression(std::vector<riddle::id_token> is, const riddle::id_token &fn, std::vector<std::unique_ptr<const riddle::ast::expression>> es) : riddle::ast::function_expression(std::move(is), fn, std::move(es)) {}
    function_expression(const function_expression &orig) = delete;

   RATIOCORE_EXPORT expr evaluate(scope &scp, context &ctx) const override;
  };

  class id_expression final : public riddle::ast::id_expression, public expression
  {
  public:
    id_expression(std::vector<riddle::id_token> is) : riddle::ast::id_expression(std::move(is)) {}
    id_expression(const id_expression &orig) = delete;

   RATIOCORE_EXPORT expr evaluate(scope &scp, context &ctx) const override;
  };

  class implication_expression final : public riddle::ast::implication_expression, public expression
  {
  public:
    implication_expression(std::unique_ptr<const riddle::ast::expression> l, std::unique_ptr<const riddle::ast::expression> r) : riddle::ast::implication_expression(std::move(l), std::move(r)) {}
    implication_expression(const implication_expression &orig) = delete;

   RATIOCORE_EXPORT expr evaluate(scope &scp, context &ctx) const override;
  };

  class disjunction_expression final : public riddle::ast::disjunction_expression, public expression
  {
  public:
    disjunction_expression(std::vector<std::unique_ptr<const riddle::ast::expression>> es) : riddle::ast::disjunction_expression(std::move(es)) {}
    disjunction_expression(const disjunction_expression &orig) = delete;

   RATIOCORE_EXPORT expr evaluate(scope &scp, context &ctx) const override;
  };

  class conjunction_expression final : public riddle::ast::conjunction_expression, public expression
  {
  public:
    conjunction_expression(std::vector<std::unique_ptr<const riddle::ast::expression>> es) : riddle::ast::conjunction_expression(std::move(es)) {}
    conjunction_expression(const conjunction_expression &orig) = delete;

   RATIOCORE_EXPORT expr evaluate(scope &scp, context &ctx) const override;
  };

  class exct_one_expression final : public riddle::ast::exct_one_expression, public expression
  {
  public:
    exct_one_expression(std::vector<std::unique_ptr<const riddle::ast::expression>> es) : riddle::ast::exct_one_expression(std::move(es)) {}
    exct_one_expression(const exct_one_expression &orig) = delete;

   RATIOCORE_EXPORT expr evaluate(scope &scp, context &ctx) const override;
  };

  class addition_expression final : public riddle::ast::addition_expression, public expression
  {
  public:
    addition_expression(std::vector<std::unique_ptr<const riddle::ast::expression>> es) : riddle::ast::addition_expression(std::move(es)) {}
    addition_expression(const addition_expression &orig) = delete;

   RATIOCORE_EXPORT expr evaluate(scope &scp, context &ctx) const override;
  };

  class subtraction_expression final : public riddle::ast::subtraction_expression, public expression
  {
  public:
    subtraction_expression(std::vector<std::unique_ptr<const riddle::ast::expression>> es) : riddle::ast::subtraction_expression(std::move(es)) {}
    subtraction_expression(const subtraction_expression &orig) = delete;

   RATIOCORE_EXPORT expr evaluate(scope &scp, context &ctx) const override;
  };

  class multiplication_expression final : public riddle::ast::multiplication_expression, public expression
  {
  public:
    multiplication_expression(std::vector<std::unique_ptr<const riddle::ast::expression>> es) : riddle::ast::multiplication_expression(std::move(es)) {}
    multiplication_expression(const multiplication_expression &orig) = delete;

  RATIOCORE_EXPORT  expr evaluate(scope &scp, context &ctx) const override;
  };

  class division_expression final : public riddle::ast::division_expression, public expression
  {
  public:
    division_expression(std::vector<std::unique_ptr<const riddle::ast::expression>> es) : riddle::ast::division_expression(std::move(es)) {}
    division_expression(const division_expression &orig) = delete;

   RATIOCORE_EXPORT expr evaluate(scope &scp, context &ctx) const override;
  };

  class statement : public riddle::ast::statement
  {
  public:
    statement() = default;
    statement(const statement &orig) = delete;
    virtual ~statement() = default;

   RATIOCORE_EXPORT virtual void execute(scope &scp, context &ctx) const = 0;
  };

  class local_field_statement final : public riddle::ast::local_field_statement, public statement
  {
  public:
    local_field_statement(std::vector<riddle::id_token> ft, std::vector<riddle::id_token> ns, std::vector<std::unique_ptr<const riddle::ast::expression>> es) : riddle::ast::local_field_statement(std::move(ft), std::move(ns), std::move(es)) {}
    local_field_statement(const local_field_statement &orig) = delete;

   RATIOCORE_EXPORT void execute(scope &scp, context &ctx) const override;
  };

  class assignment_statement final : public riddle::ast::assignment_statement, public statement
  {
  public:
    assignment_statement(std::vector<riddle::id_token> is, const riddle::id_token &i, std::unique_ptr<const riddle::ast::expression> e) : riddle::ast::assignment_statement(std::move(is), i, std::move(e)) {}
    assignment_statement(const assignment_statement &orig) = delete;

   RATIOCORE_EXPORT void execute(scope &scp, context &ctx) const override;
  };

  class expression_statement final : public riddle::ast::expression_statement, public statement
  {
  public:
    expression_statement(std::unique_ptr<const riddle::ast::expression> e) : riddle::ast::expression_statement(std::move(e)) {}
    expression_statement(const expression_statement &orig) = delete;

   RATIOCORE_EXPORT void execute(scope &scp, context &ctx) const override;
  };

  class disjunction_statement final : public riddle::ast::disjunction_statement, public statement
  {
  public:
    disjunction_statement(std::vector<std::vector<std::unique_ptr<const riddle::ast::statement>>> conjs, std::vector<std::unique_ptr<const riddle::ast::expression>> conj_costs) : riddle::ast::disjunction_statement(std::move(conjs), std::move(conj_costs)) {}
    disjunction_statement(const disjunction_statement &orig) = delete;

   RATIOCORE_EXPORT void execute(scope &scp, context &ctx) const override;
  };

  class conjunction_statement final : public riddle::ast::conjunction_statement, public statement
  {
  public:
    conjunction_statement(std::vector<std::unique_ptr<const riddle::ast::statement>> stmnts) : riddle::ast::conjunction_statement(std::move(stmnts)) {}
    conjunction_statement(const conjunction_statement &orig) = delete;

  RATIOCORE_EXPORT  void execute(scope &scp, context &ctx) const override;
  };

  class formula_statement final : public riddle::ast::formula_statement, public statement
  {
  public:
    formula_statement(const bool &isf, const riddle::id_token &fn, std::vector<riddle::id_token> scp, const riddle::id_token &pn, std::vector<riddle::id_token> assn_ns, std::vector<std::unique_ptr<const riddle::ast::expression>> assn_vs) : riddle::ast::formula_statement(isf, fn, std::move(scp), pn, std::move(assn_ns), std::move(assn_vs)) {}
    formula_statement(const formula_statement &orig) = delete;

  RATIOCORE_EXPORT  void execute(scope &scp, context &ctx) const override;
  };

  class return_statement final : public riddle::ast::return_statement, public statement
  {
  public:
    return_statement(std::unique_ptr<const riddle::ast::expression> e) : riddle::ast::return_statement(std::move(e)) {}
    return_statement(const return_statement &orig) = delete;

   RATIOCORE_EXPORT void execute(scope &scp, context &ctx) const override;
  };

  class type_declaration : public riddle::ast::type_declaration
  {
  public:
    type_declaration() = default;
    type_declaration(const type_declaration &orig) = delete;

   RATIOCORE_EXPORT virtual void declare(scope &) const {}
   RATIOCORE_EXPORT virtual void refine(scope &) const {}
  };

  class method_declaration final : public riddle::ast::method_declaration
  {
  public:
    method_declaration(std::vector<riddle::id_token> rt, const riddle::id_token &n, std::vector<std::pair<const std::vector<riddle::id_token>, const riddle::id_token>> pars, std::vector<std::unique_ptr<const riddle::ast::statement>> stmnts) : riddle::ast::method_declaration(std::move(rt), n, std::move(pars), std::move(stmnts)) {}
    method_declaration(const method_declaration &orig) = delete;

   RATIOCORE_EXPORT void refine(scope &scp) const;
  };

  class predicate_declaration final : public riddle::ast::predicate_declaration
  {
  public:
    predicate_declaration(const riddle::id_token &n, std::vector<std::pair<const std::vector<riddle::id_token>, const riddle::id_token>> pars, std::vector<std::vector<riddle::id_token>> pl, std::vector<std::unique_ptr<const riddle::ast::statement>> stmnts) : riddle::ast::predicate_declaration(n, std::move(pars), std::move(pl), std::move(stmnts)) {}
    predicate_declaration(const predicate_declaration &orig) = delete;

   RATIOCORE_EXPORT void declare(scope &scp) const;
   RATIOCORE_EXPORT void refine(scope &scp) const;
  };

  class typedef_declaration final : public riddle::ast::typedef_declaration, public type_declaration
  {
  public:
    typedef_declaration(const riddle::id_token &n, const riddle::id_token &pt, std::unique_ptr<const riddle::ast::expression> e) : riddle::ast::typedef_declaration(n, pt, std::move(e)) {}
    typedef_declaration(const typedef_declaration &orig) = delete;

  RATIOCORE_EXPORT  void declare(scope &scp) const override;
  };

  class enum_declaration final : public riddle::ast::enum_declaration, public type_declaration
  {
  public:
    enum_declaration(const riddle::id_token &n, std::vector<riddle::string_token> es, std::vector<std::vector<riddle::id_token>> trs) : riddle::ast::enum_declaration(n, std::move(es), std::move(trs)) {}
    enum_declaration(const enum_declaration &orig) = delete;

   RATIOCORE_EXPORT void declare(scope &scp) const override;
   RATIOCORE_EXPORT void refine(scope &scp) const override;
  };

  class variable_declaration final : public riddle::ast::variable_declaration
  {
    friend class field_declaration;

  public:
    variable_declaration(const riddle::id_token &n, std::unique_ptr<const riddle::ast::expression> e = nullptr) : riddle::ast::variable_declaration(n, std::move(e)) {}
    variable_declaration(const variable_declaration &orig) = delete;
  };

  class field_declaration final : public riddle::ast::field_declaration
  {
  public:
    field_declaration(std::vector<riddle::id_token> tp, std::vector<std::unique_ptr<const riddle::ast::variable_declaration>> ds) : riddle::ast::field_declaration(std::move(tp), std::move(ds)) {}
    field_declaration(const field_declaration &orig) = delete;

  RATIOCORE_EXPORT  void refine(scope &scp) const;
  };

  class constructor_declaration final : public riddle::ast::constructor_declaration
  {
  public:
    constructor_declaration(std::vector<std::pair<const std::vector<riddle::id_token>, const riddle::id_token>> pars, std::vector<riddle::id_token> ins, std::vector<std::vector<std::unique_ptr<const riddle::ast::expression>>> ivs, std::vector<std::unique_ptr<const riddle::ast::statement>> stmnts) : riddle::ast::constructor_declaration(std::move(pars), std::move(ins), std::move(ivs), std::move(stmnts)) {}
    constructor_declaration(const constructor_declaration &orig) = delete;

  RATIOCORE_EXPORT  void refine(scope &scp) const;
  };

  class class_declaration final : public riddle::ast::class_declaration, public type_declaration
  {
  public:
    class_declaration(const riddle::id_token &n, std::vector<std::vector<riddle::id_token>> bcs, std::vector<std::unique_ptr<const riddle::ast::field_declaration>> fs, std::vector<std::unique_ptr<const riddle::ast::constructor_declaration>> cs, std::vector<std::unique_ptr<const riddle::ast::method_declaration>> ms, std::vector<std::unique_ptr<const riddle::ast::predicate_declaration>> ps, std::vector<std::unique_ptr<const riddle::ast::type_declaration>> ts) : riddle::ast::class_declaration(n, std::move(bcs), std::move(fs), std::move(cs), std::move(ms), std::move(ps), std::move(ts)) {}
    class_declaration(const class_declaration &orig) = delete;

   RATIOCORE_EXPORT void declare(scope &scp) const override;
   RATIOCORE_EXPORT void refine(scope &scp) const override;
  };

  class compilation_unit final : public riddle::ast::compilation_unit
  {
  public:
    compilation_unit(std::vector<std::unique_ptr<const riddle::ast::method_declaration>> ms, std::vector<std::unique_ptr<const riddle::ast::predicate_declaration>> ps, std::vector<std::unique_ptr<const riddle::ast::type_declaration>> ts, std::vector<std::unique_ptr<const riddle::ast::statement>> stmnts) : riddle::ast::compilation_unit(std::move(ms), std::move(ps), std::move(ts), std::move(stmnts)) {}
    compilation_unit(const compilation_unit &orig) = delete;

   RATIOCORE_EXPORT void declare(scope &scp) const;
   RATIOCORE_EXPORT void refine(scope &scp) const;
   RATIOCORE_EXPORT void execute(scope &scp, context &ctx) const;
  };

  class parser : public riddle::parser
  {
  public:
   RATIOCORE_EXPORT parser(std::istream &is);
    parser(const parser &orig) = delete;
    virtual ~parser() = default;

  private:
    /**
     * The declarations.
     */
    std::unique_ptr<const riddle::ast::method_declaration> new_method_declaration(std::vector<riddle::id_token> rt, const riddle::id_token &n, std::vector<std::pair<const std::vector<riddle::id_token>, const riddle::id_token>> pars, std::vector<std::unique_ptr<const riddle::ast::statement>> stmnts) const noexcept { return std::make_unique<const ratio::core::method_declaration>(std::move(rt), n, std::move(pars), std::move(stmnts)); }
    std::unique_ptr<const riddle::ast::predicate_declaration> new_predicate_declaration(const riddle::id_token &n, std::vector<std::pair<const std::vector<riddle::id_token>, const riddle::id_token>> pars, std::vector<std::vector<riddle::id_token>> pl, std::vector<std::unique_ptr<const riddle::ast::statement>> stmnts) const noexcept { return std::make_unique<const ratio::core::predicate_declaration>(n, std::move(pars), std::move(pl), std::move(stmnts)); }
    std::unique_ptr<const riddle::ast::typedef_declaration> new_typedef_declaration(const riddle::id_token &n, const riddle::id_token &pt, std::unique_ptr<const riddle::ast::expression> e) const noexcept { return std::make_unique<const ratio::core::typedef_declaration>(n, pt, std::move(e)); }
    std::unique_ptr<const riddle::ast::enum_declaration> new_enum_declaration(const riddle::id_token &n, std::vector<riddle::string_token> es, std::vector<std::vector<riddle::id_token>> trs) const noexcept { return std::make_unique<const ratio::core::enum_declaration>(n, std::move(es), std::move(trs)); }
    std::unique_ptr<const riddle::ast::class_declaration> new_class_declaration(const riddle::id_token &n, std::vector<std::vector<riddle::id_token>> bcs, std::vector<std::unique_ptr<const riddle::ast::field_declaration>> fs, std::vector<std::unique_ptr<const riddle::ast::constructor_declaration>> cs, std::vector<std::unique_ptr<const riddle::ast::method_declaration>> ms, std::vector<std::unique_ptr<const riddle::ast::predicate_declaration>> ps, std::vector<std::unique_ptr<const riddle::ast::type_declaration>> ts) const noexcept { return std::make_unique<const ratio::core::class_declaration>(n, std::move(bcs), std::move(fs), std::move(cs), std::move(ms), std::move(ps), std::move(ts)); }
    std::unique_ptr<const riddle::ast::variable_declaration> new_variable_declaration(const riddle::id_token &n, std::unique_ptr<const riddle::ast::expression> e = nullptr) const noexcept { return std::make_unique<const ratio::core::variable_declaration>(n, std::move(e)); }
    std::unique_ptr<const riddle::ast::field_declaration> new_field_declaration(std::vector<riddle::id_token> tp, std::vector<std::unique_ptr<const riddle::ast::variable_declaration>> ds) const noexcept { return std::make_unique<const ratio::core::field_declaration>(std::move(tp), std::move(ds)); }
    std::unique_ptr<const riddle::ast::constructor_declaration> new_constructor_declaration(std::vector<std::pair<const std::vector<riddle::id_token>, const riddle::id_token>> pars, std::vector<riddle::id_token> ins, std::vector<std::vector<std::unique_ptr<const riddle::ast::expression>>> ivs, std::vector<std::unique_ptr<const riddle::ast::statement>> stmnts) const noexcept { return std::make_unique<const ratio::core::constructor_declaration>(std::move(pars), std::move(ins), std::move(ivs), std::move(stmnts)); }
    std::unique_ptr<const riddle::ast::compilation_unit> new_compilation_unit(std::vector<std::unique_ptr<const riddle::ast::method_declaration>> ms, std::vector<std::unique_ptr<const riddle::ast::predicate_declaration>> ps, std::vector<std::unique_ptr<const riddle::ast::type_declaration>> ts, std::vector<std::unique_ptr<const riddle::ast::statement>> stmnts) const noexcept { return std::make_unique<const ratio::core::compilation_unit>(std::move(ms), std::move(ps), std::move(ts), std::move(stmnts)); }

    /**
     * The statements.
     */
    std::unique_ptr<const riddle::ast::local_field_statement> new_local_field_statement(std::vector<riddle::id_token> ft, std::vector<riddle::id_token> ns, std::vector<std::unique_ptr<const riddle::ast::expression>> es) const noexcept { return std::make_unique<const ratio::core::local_field_statement>(std::move(ft), std::move(ns), std::move(es)); }
    std::unique_ptr<const riddle::ast::assignment_statement> new_assignment_statement(std::vector<riddle::id_token> is, const riddle::id_token &i, std::unique_ptr<const riddle::ast::expression> e) const noexcept { return std::make_unique<const ratio::core::assignment_statement>(std::move(is), i, std::move(e)); }
    std::unique_ptr<const riddle::ast::expression_statement> new_expression_statement(std::unique_ptr<const riddle::ast::expression> e) const noexcept { return std::make_unique<const ratio::core::expression_statement>(std::move(e)); }
    std::unique_ptr<const riddle::ast::disjunction_statement> new_disjunction_statement(std::vector<std::vector<std::unique_ptr<const riddle::ast::statement>>> conjs, std::vector<std::unique_ptr<const riddle::ast::expression>> conj_costs) const noexcept { return std::make_unique<const ratio::core::disjunction_statement>(std::move(conjs), std::move(conj_costs)); }
    std::unique_ptr<const riddle::ast::conjunction_statement> new_conjunction_statement(std::vector<std::unique_ptr<const riddle::ast::statement>> stmnts) const noexcept { return std::make_unique<const ratio::core::conjunction_statement>(std::move(stmnts)); }
    std::unique_ptr<const riddle::ast::formula_statement> new_formula_statement(const bool &isf, const riddle::id_token &fn, std::vector<riddle::id_token> scp, const riddle::id_token &pn, std::vector<riddle::id_token> assn_ns, std::vector<std::unique_ptr<const riddle::ast::expression>> assn_vs) const noexcept { return std::make_unique<const ratio::core::formula_statement>(isf, fn, std::move(scp), pn, std::move(assn_ns), std::move(assn_vs)); }
    std::unique_ptr<const riddle::ast::return_statement> new_return_statement(std::unique_ptr<const riddle::ast::expression> e) const noexcept { return std::make_unique<const ratio::core::return_statement>(std::move(e)); }

    /**
     * The expressions.
     */
    std::unique_ptr<const riddle::ast::bool_literal_expression> new_bool_literal_expression(const riddle::bool_token &l) const noexcept { return std::make_unique<const ratio::core::bool_literal_expression>(l); }
    std::unique_ptr<const riddle::ast::int_literal_expression> new_int_literal_expression(const riddle::int_token &l) const noexcept { return std::make_unique<const ratio::core::int_literal_expression>(l); }
    std::unique_ptr<const riddle::ast::real_literal_expression> new_real_literal_expression(const riddle::real_token &l) const noexcept { return std::make_unique<const ratio::core::real_literal_expression>(l); }
    std::unique_ptr<const riddle::ast::string_literal_expression> new_string_literal_expression(const riddle::string_token &l) const noexcept { return std::make_unique<const ratio::core::string_literal_expression>(l); }
    std::unique_ptr<const riddle::ast::cast_expression> new_cast_expression(std::vector<riddle::id_token> tp, std::unique_ptr<const riddle::ast::expression> e) const noexcept { return std::make_unique<const ratio::core::cast_expression>(std::move(tp), std::move(e)); }
    std::unique_ptr<const riddle::ast::plus_expression> new_plus_expression(std::unique_ptr<const riddle::ast::expression> e) const noexcept { return std::make_unique<const ratio::core::plus_expression>(std::move(e)); }
    std::unique_ptr<const riddle::ast::minus_expression> new_minus_expression(std::unique_ptr<const riddle::ast::expression> e) const noexcept { return std::make_unique<const ratio::core::minus_expression>(std::move(e)); }
    std::unique_ptr<const riddle::ast::not_expression> new_not_expression(std::unique_ptr<const riddle::ast::expression> e) const noexcept { return std::make_unique<const ratio::core::not_expression>(std::move(e)); }
    std::unique_ptr<const riddle::ast::constructor_expression> new_constructor_expression(std::vector<riddle::id_token> it, std::vector<std::unique_ptr<const riddle::ast::expression>> es) const noexcept { return std::make_unique<const ratio::core::constructor_expression>(std::move(it), std::move(es)); }
    std::unique_ptr<const riddle::ast::eq_expression> new_eq_expression(std::unique_ptr<const riddle::ast::expression> l, std::unique_ptr<const riddle::ast::expression> r) const noexcept { return std::make_unique<const ratio::core::eq_expression>(std::move(l), std::move(r)); }
    std::unique_ptr<const riddle::ast::neq_expression> new_neq_expression(std::unique_ptr<const riddle::ast::expression> l, std::unique_ptr<const riddle::ast::expression> r) const noexcept { return std::make_unique<const ratio::core::neq_expression>(std::move(l), std::move(r)); }
    std::unique_ptr<const riddle::ast::lt_expression> new_lt_expression(std::unique_ptr<const riddle::ast::expression> l, std::unique_ptr<const riddle::ast::expression> r) const noexcept { return std::make_unique<const ratio::core::lt_expression>(std::move(l), std::move(r)); }
    std::unique_ptr<const riddle::ast::leq_expression> new_leq_expression(std::unique_ptr<const riddle::ast::expression> l, std::unique_ptr<const riddle::ast::expression> r) const noexcept { return std::make_unique<const ratio::core::leq_expression>(std::move(l), std::move(r)); }
    std::unique_ptr<const riddle::ast::geq_expression> new_geq_expression(std::unique_ptr<const riddle::ast::expression> l, std::unique_ptr<const riddle::ast::expression> r) const noexcept { return std::make_unique<const ratio::core::geq_expression>(std::move(l), std::move(r)); }
    std::unique_ptr<const riddle::ast::gt_expression> new_gt_expression(std::unique_ptr<const riddle::ast::expression> l, std::unique_ptr<const riddle::ast::expression> r) const noexcept { return std::make_unique<const ratio::core::gt_expression>(std::move(l), std::move(r)); }
    std::unique_ptr<const riddle::ast::function_expression> new_function_expression(std::vector<riddle::id_token> is, const riddle::id_token &fn, std::vector<std::unique_ptr<const riddle::ast::expression>> es) const noexcept { return std::make_unique<const ratio::core::function_expression>(std::move(is), fn, std::move(es)); }
    std::unique_ptr<const riddle::ast::id_expression> new_id_expression(std::vector<riddle::id_token> is) const noexcept { return std::make_unique<const ratio::core::id_expression>(std::move(is)); }
    std::unique_ptr<const riddle::ast::implication_expression> new_implication_expression(std::unique_ptr<const riddle::ast::expression> l, std::unique_ptr<const riddle::ast::expression> r) const noexcept { return std::make_unique<const ratio::core::implication_expression>(std::move(l), std::move(r)); }
    std::unique_ptr<const riddle::ast::disjunction_expression> new_disjunction_expression(std::vector<std::unique_ptr<const riddle::ast::expression>> es) const noexcept { return std::make_unique<const ratio::core::disjunction_expression>(std::move(es)); }
    std::unique_ptr<const riddle::ast::conjunction_expression> new_conjunction_expression(std::vector<std::unique_ptr<const riddle::ast::expression>> es) const noexcept { return std::make_unique<const ratio::core::conjunction_expression>(std::move(es)); }
    std::unique_ptr<const riddle::ast::exct_one_expression> new_exct_one_expression(std::vector<std::unique_ptr<const riddle::ast::expression>> es) const noexcept { return std::make_unique<const ratio::core::exct_one_expression>(std::move(es)); }
    std::unique_ptr<const riddle::ast::addition_expression> new_addition_expression(std::vector<std::unique_ptr<const riddle::ast::expression>> es) const noexcept { return std::make_unique<const ratio::core::addition_expression>(std::move(es)); }
    std::unique_ptr<const riddle::ast::subtraction_expression> new_subtraction_expression(std::vector<std::unique_ptr<const riddle::ast::expression>> es) const noexcept { return std::make_unique<const ratio::core::subtraction_expression>(std::move(es)); }
    std::unique_ptr<const riddle::ast::multiplication_expression> new_multiplication_expression(std::vector<std::unique_ptr<const riddle::ast::expression>> es) const noexcept { return std::make_unique<const ratio::core::multiplication_expression>(std::move(es)); }
    std::unique_ptr<const riddle::ast::division_expression> new_division_expression(std::vector<std::unique_ptr<const riddle::ast::expression>> es) const noexcept { return std::make_unique<const ratio::core::division_expression>(std::move(es)); }
  };
} // namespace ratio::core
