#pragma once
#include "scope.h"
#include "env.h"
#include "inf_rational.h"
#include "var_value.h"
#include <unordered_set>
#ifdef COMPUTE_NAMES
#include <unordered_map>
#endif

#ifdef COMPUTE_NAMES
#define RECOMPUTE_NAMES() recompute_names()
#else
#define RECOMPUTE_NAMES()
#endif

namespace riddle::ast
{
  class compilation_unit;
} // namespace riddle::ast

namespace ratio::core
{
  class conjunction;
  class bool_item;
  class arith_item;
  class enum_item;
  class atom;
  class formula_statement;
  class method_declaration;
  class predicate_declaration;
  class typedef_declaration;
  class enum_declaration;
  class class_declaration;
  class compilation_unit;
#ifdef BUILD_LISTENERS
  class core_listener;
#endif

#ifdef BUILD_LISTENERS
#define FIRE_LOG(msg) fire_log(msg)
#define FIRE_READ(rddl) fire_read(rddl)
#define FIRE_STATE_CHANGED() fire_state_changed()
#define FIRE_STARTED_SOLVING() fire_started_solving()
#define FIRE_SOLUTION_FOUND() fire_solution_found()
#define FIRE_INCONSISTENT_PROBLEM() fire_inconsistent_problem()
#else
#define FIRE_LOG(msg)
#define FIRE_READ(rddl)
#define FIRE_STATE_CHANGED()
#define FIRE_STARTED_SOLVING()
#define FIRE_SOLUTION_FOUND()
#define FIRE_INCONSISTENT_PROBLEM()
#endif
  class core : public scope, public env
  {
    friend class formula_statement;
    friend class method_declaration;
    friend class predicate_declaration;
    friend class typedef_declaration;
    friend class enum_declaration;
    friend class class_declaration;
#ifdef BUILD_LISTENERS
    friend class core_listener;
#endif

  public:
    RATIOCORE_EXPORT core();
    core(const core &orig) = delete;
    RATIOCORE_EXPORT virtual ~core();

    /**
     * @brief Parses the given riddle script.
     *
     * @param script The riddle script to parse.
     */
    RATIOCORE_EXPORT virtual void read(const std::string &script);
    /**
     * @brief Parses the given riddle files.
     *
     * @param files The riddle files to parse.
     */
    RATIOCORE_EXPORT virtual void read(const std::vector<std::string> &files);

    inline type &get_bool_type() const noexcept { return *bt; }
    inline type &get_int_type() const noexcept { return *it; }
    inline type &get_real_type() const noexcept { return *rt; }
    inline type &get_time_type() const noexcept { return *tt; }
    inline type &get_string_type() const noexcept { return *st; }

    /**
     * @brief Creates a new boolean variable.
     *
     * @return expr The new boolean variable.
     */
    virtual expr new_bool() noexcept { return nullptr; }

    /**
     * @brief Creates a new boolean literal.
     *
     * @param val The value of the literal.
     * @return expr The new boolean literal.
     */
    RATIOCORE_EXPORT expr new_bool(const bool &val) noexcept;

    /**
     * @brief Creates a new integer variable.
     *
     * @return expr The new integer variable.
     */
    virtual expr new_int() noexcept { return nullptr; }

    /**
     * @brief Creates a new integer literal.
     *
     * @param val The value of the literal.
     * @return expr The new integer literal.
     */
    RATIOCORE_EXPORT expr new_int(const semitone::I &val) noexcept;

    /**
     * @brief Creates a new real variable.
     *
     * @return expr The new real variable.
     */
    virtual expr new_real() noexcept { return nullptr; }

    /**
     * @brief Creates a new real literal.
     *
     * @param val The value of the literal.
     * @return expr The new real literal.
     */
    RATIOCORE_EXPORT expr new_real(const semitone::rational &val) noexcept;

    /**
     * @brief Creates a new time-point variable.
     *
     * @return expr The new time-point variable.
     */
    virtual expr new_time_point() noexcept { return nullptr; }

    /**
     * @brief Creates a new time-point literal.
     *
     * @param val The value of the literal.
     * @return expr The new time-point literal.
     */
    RATIOCORE_EXPORT expr new_time_point(const semitone::rational &val) noexcept;

    /**
     * @brief Creates a new string variable.
     *
     * @return expr The new string variable.
     */
    virtual expr new_string() noexcept { return nullptr; }

    /**
     * @brief Creates a new string literal.
     *
     * @param val The value of the literal.
     * @return expr The new string literal.
     */
    RATIOCORE_EXPORT expr new_string(const std::string &val) noexcept;

    /**
     * @brief Creates a new enumerative variable.
     *
     * @param tp The type of the enumerative variable.
     * @param allowed_vals The initial domain of the enumerative variable.
     * @return expr The new enumerative variable.
     */
    virtual expr new_enum([[maybe_unused]] type &tp, [[maybe_unused]] const std::vector<expr> &allowed_vals) { return nullptr; }
    /**
     * @brief Computes, if not already present, the `name` field of the enumerative variable `var`, introducing proper constraints for managing consistency.
     *
     * @param var The enumerative variable whose field has to be computed.
     * @param name The name of the field which has to be computed.
     * @return expr The value ov the `name` field.
     */
    virtual expr get([[maybe_unused]] enum_item &var, [[maybe_unused]] const std::string &name) { return nullptr; }
    /**
     * @brief Removes the `val` value from the `var` variable's domain.
     *
     * @param var The variable whose domain is to be shrinked.
     * @param val The value to be removed from the domain.
     */
    virtual void remove([[maybe_unused]] expr &var, [[maybe_unused]] semitone::var_value &val) {}

    /**
     * @brief Creates an expression which is the negation of the given expression.
     *
     * @param var The expression to be negated.
     * @return expr The negated expression.
     */
    virtual expr negate([[maybe_unused]] const expr &var) noexcept { return nullptr; }
    /**
     * @brief Creates an expression which the conjunction of the given expressions.
     *
     * @param exprs The expressions of the conjunction.
     * @return expr The conjunction expression of the given expressions.
     */
    virtual expr conj([[maybe_unused]] const std::vector<expr> &exprs) noexcept { return nullptr; }
    /**
     * @brief Creates an expression which the disjunction of the given expressions.
     *
     * @param exprs The expressions of the disjunction.
     * @return expr The disjunction expression of the given expressions.
     */
    virtual expr disj([[maybe_unused]] const std::vector<expr> &exprs) noexcept { return nullptr; }
    /**
     * @brief Creates an expression which the exact-one of the given expressions.
     *
     * @param exprs The expressions of the exact-one.
     * @return expr The exact-one expression of the given expressions.
     */
    virtual expr exct_one([[maybe_unused]] const std::vector<expr> &exprs) noexcept { return nullptr; }

    virtual expr add([[maybe_unused]] const std::vector<expr> &exprs) noexcept { return nullptr; }
    virtual expr sub([[maybe_unused]] const std::vector<expr> &exprs) noexcept { return nullptr; }
    virtual expr mult([[maybe_unused]] const std::vector<expr> &exprs) noexcept { return nullptr; }
    virtual expr div([[maybe_unused]] const std::vector<expr> &exprs) noexcept { return nullptr; }
    virtual expr minus([[maybe_unused]] const expr &ex) noexcept { return nullptr; }

    virtual expr lt([[maybe_unused]] const expr &left, [[maybe_unused]] const expr &right) noexcept { return nullptr; }
    virtual expr leq([[maybe_unused]] const expr &left, [[maybe_unused]] const expr &right) noexcept { return nullptr; }
    virtual expr eq([[maybe_unused]] const expr &left, [[maybe_unused]] const expr &right) noexcept { return nullptr; }
    virtual expr geq([[maybe_unused]] const expr &left, [[maybe_unused]] const expr &right) noexcept { return nullptr; }
    virtual expr gt([[maybe_unused]] const expr &left, [[maybe_unused]] const expr &right) noexcept { return nullptr; }

    inline core &get_core() const override { return const_cast<core &>(*this); }

    RATIOCORE_EXPORT expr get(const std::string &name) noexcept override;

    /**
     * @brief Evaluates the given boolean expression.
     *
     * @param x The boolean expression to evaluate.
     * @return semitone::lbool The value of the given boolean expression.
     */
    RATIOCORE_EXPORT semitone::lbool bool_value([[maybe_unused]] const expr &x) const noexcept;
    virtual semitone::lbool bool_value([[maybe_unused]] const bool_item &x) const noexcept { return semitone::Undefined; }
    RATIOCORE_EXPORT bool is_constant(const bool_item &x) const noexcept;
    /**
     * @brief Evaluates the bounds of the given arithmetic expression.
     *
     * @param x The arithmetic expression to evaluate.
     * @return std::pair<semitone::inf_rational, semitone::inf_rational> The bounds of the given arithmetic expression.
     */
    RATIOCORE_EXPORT std::pair<semitone::inf_rational, semitone::inf_rational> arith_bounds([[maybe_unused]] const expr &x) const noexcept;
    virtual std::pair<semitone::inf_rational, semitone::inf_rational> arith_bounds([[maybe_unused]] const arith_item &x) const noexcept { return std::make_pair<semitone::inf_rational, semitone::inf_rational>(semitone::inf_rational(semitone::rational::NEGATIVE_INFINITY), semitone::inf_rational(semitone::rational::POSITIVE_INFINITY)); }
    /**
     * @brief Evaluates the given arithmetic expression.
     *
     * @param x The arithmetic expression to evaluate.
     * @return semitone::inf_rational The value of the given arithmetic expression.
     */
    RATIOCORE_EXPORT semitone::inf_rational arith_value([[maybe_unused]] const expr &x) const noexcept;
    virtual semitone::inf_rational arith_value([[maybe_unused]] const arith_item &x) const noexcept { return semitone::inf_rational(0); }
    RATIOCORE_EXPORT bool is_constant([[maybe_unused]] const arith_item &x) const noexcept;
    /**
     * @brief Evaluates the given enumerative expression.
     *
     * @param x The arithmetic enumerative to evaluate.
     * @return std::unordered_set<semitone::var_value *> The current domain of the given enumerative expression.
     */
    RATIOCORE_EXPORT std::unordered_set<semitone::var_value *> enum_value([[maybe_unused]] const expr &x) const noexcept;
    virtual std::unordered_set<semitone::var_value *> enum_value([[maybe_unused]] const enum_item &x) const noexcept { return std::unordered_set<semitone::var_value *>(); }
    RATIOCORE_EXPORT bool is_constant([[maybe_unused]] const enum_item &x) const noexcept;

    /**
     * @brief Creates a new disjunction of conjunctions.
     *
     * @param conjs A vector of conjunctions representing the disjunction.
     */
    RATIOCORE_EXPORT virtual void new_disjunction(std::vector<std::unique_ptr<conjunction>> conjs);

  private:
    virtual void new_atom([[maybe_unused]] atom &atm, [[maybe_unused]] const bool &is_fact = true) {}

  public:
    virtual void assert_facts([[maybe_unused]] std::vector<expr> facts) {}

  protected:
    RATIOCORE_EXPORT type &get_type(const std::vector<expr> &exprs) const;
    RATIOCORE_EXPORT type &get_type(const std::vector<const item *> &exprs) const;
    RATIOCORE_EXPORT void new_method(method_ptr m) noexcept;
    RATIOCORE_EXPORT void new_type(type_ptr t) noexcept;
    RATIOCORE_EXPORT void new_predicate(predicate_ptr p) noexcept;

  public:
    RATIOCORE_EXPORT const field &get_field(const std::string &name) const override;
    RATIOCORE_EXPORT method &get_method(const std::string &name, const std::vector<const type *> &ts) const override;
    const std::map<std::string, std::vector<method_ptr>> &get_methods() const noexcept override { return methods; }
    RATIOCORE_EXPORT type &get_type(const std::string &name) const override;
    const std::map<std::string, type_ptr> &get_types() const noexcept override { return types; }
    RATIOCORE_EXPORT predicate &get_predicate(const std::string &name) const override;
    const std::map<std::string, predicate_ptr> &get_predicates() const noexcept override { return predicates; }

#ifdef COMPUTE_NAMES
  public:
    const std::string &guess_name(const item &itm) const noexcept { return expr_names.at(&itm); }

  private:
    void recompute_names() noexcept;

    std::unordered_map<const item *, const std::string> expr_names;
#endif

  private:
    type *bt, *it, *rt, *tt, *st;
    std::vector<std::unique_ptr<const riddle::ast::compilation_unit>> cus; // the compilation units..

    std::map<std::string, std::vector<method_ptr>> methods; // the methods, indexed by their name, defined within this core..
    std::map<std::string, type_ptr> types;                  // the inner types, indexed by their name, defined within this core..
    std::map<std::string, predicate_ptr> predicates;        // the inner predicates, indexed by their name, defined within this core..

#ifdef BUILD_LISTENERS
  private:
    std::vector<core_listener *> listeners; // the core listeners..

  protected:
    RATIOCORE_EXPORT void fire_log(const std::string msg) const noexcept;
    RATIOCORE_EXPORT void fire_read(const std::string &script) const noexcept;
    RATIOCORE_EXPORT void fire_read(const std::vector<std::string> &files) const noexcept;
    RATIOCORE_EXPORT void fire_state_changed() const noexcept;
    RATIOCORE_EXPORT void fire_started_solving() const noexcept;
    RATIOCORE_EXPORT void fire_solution_found() const noexcept;
    RATIOCORE_EXPORT void fire_inconsistent_problem() const noexcept;
#endif
  };

  class inconsistency_exception : public std::exception
  {
    const char *what() const noexcept override { return "an inconsistency has been found.."; }
  };

  class unsolvable_exception : public std::exception
  {
    const char *what() const noexcept override { return "the problem is unsolvable.."; }
  };
} // namespace ratio::core
