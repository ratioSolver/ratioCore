#pragma once
#include "core_defs.h"
#include <map>
#include <string>

namespace ratio::core
{
  class local_field_statement;
  class assignment_statement;
  class formula_statement;
  class return_statement;

  class env
  {
    friend class core;
    friend class predicate;
    friend class constructor;
    friend class method;
    friend class local_field_statement;
    friend class assignment_statement;
    friend class formula_statement;
    friend class return_statement;

  public:
    env(env &e);
    env(context ctx);
    env(const env &orig) = delete;
    virtual ~env() = default;

    /**
     * @brief Get the enclosing environment of this environment.
     *
     * @return env& The enclosing environment of this environment.
     */
    inline env &get_env() const noexcept { return e; }

    /**
     * @brief Get the expression having the given name, searching in the enclosing environments if not found in the current environment.
     *
     * @param name The name of the variable.
     * @return expr The expression having the given name.
     */
    RATIOCORE_EXPORT virtual expr get(const std::string &name);
    const std::map<std::string, expr> &get_vars() const noexcept { return vars; }

  private:
    env &e;
    context ctx;

  protected:
    std::map<std::string, expr> vars;
  };
} // namespace ratio::core
