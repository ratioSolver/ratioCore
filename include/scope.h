#pragma once
#include <map>

namespace ratio::core
{
  class core;
  class type;

  class field
  {
  public:
    field(type &tp, const std::string &name) : tp(tp), name(name) {}

    inline type &get_type() const { return tp; }                // returns the type of the field..
    inline const std::string &get_name() const { return name; } // returns the name of the field..

  private:
    type &tp;               // the type of the field..
    const std::string name; // the name of the field..
  };

  class scope
  {
  public:
    scope(scope &scp);
    scope(const scope &orig) = delete;
    virtual ~scope() = default;

    inline virtual core &get_core() { return cr; }  // returns the core in which this scope is defined..
    inline scope &get_scope() const { return scp; } // returns the enclosing scope..

  private:
    core &cr;   // the core in which this scope is defined..
    scope &scp; // the enclosing scope..

  private:
    std::map<std::string, field> fields; // the fields of this scope..
  };
} // namespace ratio::core
