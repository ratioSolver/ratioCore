#pragma once
#include <map>
#include <string>
#include <memory>

#define THIS_KEYWORD "this"
#define RETURN_KEYWORD "return"
#define TAU "tau"

namespace ratio::core
{
  class core;
  class env;
  using context = std::shared_ptr<env>;
  class item;
  using expr = std::shared_ptr<item>;

  class env
  {
  public:
    env(core &cr);
    env(context ctx);
    env(const env &orig) = delete;

    /**
     * @brief Get the core in which this environment is defined.
     *
     * @return core& The core in which this environment is defined.
     */
    inline core &get_core() const noexcept { return cr; }

    /**
     * @brief Get the expression having the given name, searching in the enclosing environments if not found in the current environment.
     *
     * @param name The name of the variable.
     * @return expr The expression having the given name.
     */
    virtual expr get(const std::string &name) noexcept;

  private:
    core &cr;
    context ctx;

  protected:
    std::map<std::string, expr> vars;
  };
} // namespace ratio::core
