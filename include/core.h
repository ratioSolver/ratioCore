#pragma once
#include "scope.h"
#include "env.h"

namespace ratio::core
{
  class type;

  class core : public scope, public env
  {
  public:
    core();
    core(const core &orig) = delete;
    ~core();

    inline core &get_core() override { return *this; }

    std::optional<expr> get(const std::string &name) noexcept override;

    std::optional<type &> get_type(const std::string &name) noexcept override;

  private:
    std::map<std::string, type *> types;
  };
} // namespace ratio::core
