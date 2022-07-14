#pragma once
#include "oratiocore_export.h"
#include <map>
#include <string>
#include <memory>
#include <vector>

namespace ratio::core
{
  class core;
  class env;
  using context = std::shared_ptr<env>;
  class item;
  using expr = std::shared_ptr<item>;
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
    ORATIOCORE_EXPORT scope(scope &scp);
    scope(const scope &orig) = delete;
    ORATIOCORE_EXPORT virtual ~scope() = default;

    /**
     * @brief Get the core in which this scope is defined.
     *
     * @return core& The core in which this scope is defined.
     */
    inline virtual core &get_core() const { return cr; }
    /**
     * @brief Get the enclosing scope.
     *
     * @return scope& The enclosing scope
     */
    inline scope &get_scope() const { return scp; }

    /**
     * @brief Get the type in the current scope with the given name, searching in the enclosing scopes if not found in the current scope.
     *
     * @param name The name of the desired type.
     * @return type& The type in the current scope with the given name.
     * @throws std::out_of_range Thrown if there is no type with the given name.
     */
    ORATIOCORE_EXPORT virtual type &get_type(const std::string &name) const;
    ORATIOCORE_EXPORT virtual const std::map<std::string, type *> &get_types() const noexcept; // returns a map of types defined within this scope having the types' names as keys..

  private:
    core &cr;   // the core in which this scope is defined..
    scope &scp; // the enclosing scope..

  private:
    std::map<std::string, field> fields; // the fields within this scope..
  };
} // namespace ratio::core
