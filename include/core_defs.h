#pragma once
#include "oratiocore_export.h"
#include <memory>

#define THIS_KW "this"
#define RETURN_KW "return"
#define TAU_KW "tau"

namespace ratio::core
{
  class type;
  using type_ptr = std::shared_ptr<type>;
  class constructor;
  using constructor_ptr = std::shared_ptr<constructor>;
  class field;
  using field_ptr = std::shared_ptr<field>;
  class predicate;
  using predicate_ptr = std::shared_ptr<predicate>;
  class method;
  using method_ptr = std::shared_ptr<method>;
  class env;
  using context = std::shared_ptr<env>;
  class item;
  using expr = std::shared_ptr<item>;
} // namespace ratio::core
