#pragma once
#include "item.h"

namespace ratio::core
{
  class predicate;

  class atom : public complex_item
  {
  public:
    RATIOCORE_EXPORT atom(predicate &pred);
    atom(const atom &orig) = delete;
    RATIOCORE_EXPORT virtual ~atom() = default;
  };
} // namespace ratio::core
