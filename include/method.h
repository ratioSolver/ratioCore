#pragma once
#include "scope.h"

namespace ratio::core
{
  class method : public scope
  {
  public:
    method(scope &scp);
    method(const method &orig) = delete;

  private:
  };
} // namespace ratio::core
