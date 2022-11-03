#pragma once
#include "core_defs.h"
#include <map>
#include <string>

namespace ratio::core
{
  class enum_item;
  class local_field_statement;
  class assignment_statement;
  class formula_statement;
  class return_statement;

  class var_map
  {
    friend class core;
    friend class predicate;
    friend class constructor;
    friend class method;
    friend class enum_item;
    friend class local_field_statement;
    friend class assignment_statement;
    friend class formula_statement;
    friend class return_statement;

  public:
    var_map();
    var_map(var_map *ctx);
    var_map(const var_map &orig) = delete;

    /**
     * @brief Get the expression having the given name, searching in the enclosing environments if not found in the current environment.
     *
     * @param name The name of the variable.
     * @return expr The expression having the given name.
     */
    RATIOCORE_EXPORT virtual expr get(const std::string &name);
    const std::map<std::string, expr> &get_vars() const noexcept { return vars; }

  private:
    var_map *ctx; // the enclosing context..
    std::map<std::string, expr> vars;
  };

  class env
  {
  public:
    env();
    env(context ctx);
    env(const env &orig) = delete;
    virtual ~env() = default;

    /**
     * @brief Get the enclosing context of this environment.
     *
     * @return context& The enclosing context of this environment.
     */
    inline context &get_context() noexcept { return ctx; }

    /**
     * @brief Get the expression having the given name, searching in the enclosing environments if not found in the current environment.
     *
     * @param name The name of the variable.
     * @return expr The expression having the given name.
     */
    virtual inline expr get(const std::string &name) { return ctx->get(name); }
    const inline std::map<std::string, expr> &get_vars() const noexcept { return ctx->get_vars(); }

  private:
    context ctx;
  };
} // namespace ratio::core
