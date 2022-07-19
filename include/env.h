#pragma once
#include "oratiocore_export.h"
#include <map>
#include <string>
#include <memory>

#define THIS_KEYWORD "this"
#define RETURN_KEYWORD "return"
#define TAU "tau"

namespace ratio::core
{
  class core;
  class constructor;
  class env;
  using context = std::shared_ptr<env>;
  class item;
  using expr = std::shared_ptr<item>;

  class env
  {
    friend class constructor;

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
