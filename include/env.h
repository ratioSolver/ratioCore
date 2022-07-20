#pragma once
#include "core_defs.h"
#include <map>
#include <string>

namespace ratio::core
{
  class env
  {
    friend class constructor;
    friend class method;

  public:
    env(env &e);
    env(context ctx);
    env(const env &orig) = delete;
    virtual ~env() = default;

    /**
     * @brief Get the expression having the given name, searching in the enclosing environments if not found in the current environment.
     *
     * @param name The name of the variable.
     * @return expr The expression having the given name.
     */
    ORATIOCORE_EXPORT virtual expr get(const std::string &name) const noexcept;

  private:
    env &e;
    context ctx;

  protected:
    std::map<std::string, expr> vars;
  };
} // namespace ratio::core
