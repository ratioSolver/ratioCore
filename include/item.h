#pragma once
#include "env.h"
#include <map>

namespace ratio::core
{
  class type;

  class item
  {
  public:
    item(type &tp);
    item(const item &orig) = delete;

    type &get_type() const noexcept { return tp; }

  private:
    type &tp;
  };

  class complex_item : public item, public env
  {
  public:
    complex_item(type &tp);

    std::optional<expr> get(const std::string &name) noexcept override;
  };
} // namespace ratio::core
