#pragma once
#include "item.h"

namespace ratio::core
{
  class predicate;

  class atom : public complex_item
  {
  public:
    ORATIOCORE_EXPORT atom(predicate &pred);
    atom(const atom &orig) = delete;
    ORATIOCORE_EXPORT virtual ~atom() = default;
  };
} // namespace ratio::core
