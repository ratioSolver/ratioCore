#pragma once
#include "defs.h"
#include <map>

namespace ratio::core
{
  class core;

  class env
  {
  public:
    env(core &cr);
    env(context ctx);

    inline core &get_core() const noexcept { return cr; }

    virtual std::optional<expr> get(const std::string &name) noexcept;

  private:
    core &cr;
    context ctx;

  protected:
    std::map<std::string, expr> vars;
  };
} // namespace ratio::core
