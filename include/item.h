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
    ORATIOCORE_EXPORT virtual ~item() = default;

    type &get_type() const noexcept { return tp; }

  private:
    type &tp;
  };

  class complex_item : public item, public env
  {
  public:
    ORATIOCORE_EXPORT complex_item(type &tp);
    complex_item(const complex_item &orig) = delete;
    ORATIOCORE_EXPORT virtual ~complex_item() = default;

    ORATIOCORE_EXPORT expr get(const std::string &name) noexcept override;
  };
} // namespace ratio::core
