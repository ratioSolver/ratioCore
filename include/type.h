#pragma once
#include "defs.h"
#include "scope.h"
#include <string>
#include <vector>

namespace ratio::core
{
  class item;

  class type : public scope
  {
  public:
    type(core &cr, const std::string &name, bool primitive = false);
    type(const type &orig) = delete;
    virtual ~type() = default;

    virtual expr new_instance(); // creates a new instance of this type..

  private:
    const std::string name;
    const bool primitive; // is this type a primitive type?

  protected:
    std::vector<type *> supertypes; // the base types (i.e. the types this type inherits from)..
    std::vector<expr> instances;    // a vector containing all the instances of this type..
  };
} // namespace ratio::core
