#pragma once
#include "scope.h"
#include "env.h"
#include "rational.h"

namespace ratio::core
{
  class core : public scope, public env
  {
  public:
    ORATIOCORE_EXPORT core();
    core(const core &orig) = delete;

    /**
     * @brief Parses the given riddle script.
     *
     * @param script the riddle script to parse.
     */
    ORATIOCORE_EXPORT virtual void read(const std::string &script);
    /**
     * @brief Parses the given riddle files.
     *
     * @param files the riddle files to parse.
     */
    ORATIOCORE_EXPORT virtual void read(const std::vector<std::string> &files);

    ORATIOCORE_EXPORT virtual expr new_bool() noexcept;                              // creates a new boolean variable..
    ORATIOCORE_EXPORT virtual expr new_bool(const bool &val) noexcept;               // creates a new boolean literal..
    ORATIOCORE_EXPORT virtual expr new_int() noexcept;                               // creates a new integer variable..
    ORATIOCORE_EXPORT virtual expr new_int(const semitone::I &val) noexcept;         // creates a new integer literal..
    ORATIOCORE_EXPORT virtual expr new_real() noexcept;                              // creates a new real variable..
    ORATIOCORE_EXPORT virtual expr new_real(const semitone::rational &val) noexcept; // creates a new real literal..
    ORATIOCORE_EXPORT virtual expr new_tp() noexcept;                                // creates a new time-point variable..
    ORATIOCORE_EXPORT virtual expr new_tp(const semitone::rational &val) noexcept;   // creates a new time-point literal..
    ORATIOCORE_EXPORT virtual expr new_string() noexcept;                            // creates a new string variable..
    ORATIOCORE_EXPORT virtual expr new_string(const std::string &val) noexcept;      // creates a new string literal..
    ORATIOCORE_EXPORT virtual expr new_enum(type &tp, std::vector<expr> allowed_vals);

    ORATIOCORE_EXPORT virtual expr negate(expr var) noexcept;
    ORATIOCORE_EXPORT virtual expr conj(const std::vector<expr> &exprs) noexcept;
    ORATIOCORE_EXPORT virtual expr disj(const std::vector<expr> &exprs) noexcept;
    ORATIOCORE_EXPORT virtual expr exct_one(const std::vector<expr> &exprs) noexcept;

    ORATIOCORE_EXPORT virtual expr add(const std::vector<expr> &exprs) noexcept;
    ORATIOCORE_EXPORT virtual expr sub(const std::vector<expr> &exprs) noexcept;
    ORATIOCORE_EXPORT virtual expr mult(const std::vector<expr> &exprs) noexcept;
    ORATIOCORE_EXPORT virtual expr div(const std::vector<expr> &exprs) noexcept;
    ORATIOCORE_EXPORT virtual expr minus(expr ex) noexcept;

    ORATIOCORE_EXPORT virtual expr lt(expr left, expr right) noexcept;
    ORATIOCORE_EXPORT virtual expr leq(expr left, expr right) noexcept;
    ORATIOCORE_EXPORT virtual expr eq(expr left, expr right) noexcept;
    ORATIOCORE_EXPORT virtual expr geq(expr left, expr right) noexcept;
    ORATIOCORE_EXPORT virtual expr gt(expr left, expr right) noexcept;

    inline core &get_core() const override { return const_cast<core &>(*this); }

    ORATIOCORE_EXPORT expr get(const std::string &name) noexcept override;

    ORATIOCORE_EXPORT type &get_type(const std::string &name) const override;

  private:
    std::map<std::string, type_ptr> types;
  };
} // namespace ratio::core
