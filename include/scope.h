#pragma once
#include "core_defs.h"
#include <map>
#include <string>
#include <vector>

namespace ratio::core
{
  class local_field_statement;
  class field_declaration;

  class scope
  {
    friend class local_field_statement;
    friend class field_declaration;

  public:
    RATIOCORE_EXPORT scope(scope &scp);
    scope(const scope &orig) = delete;
    RATIOCORE_EXPORT virtual ~scope();

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
     * @brief Get the field in the current scope with the given name, searching in the enclosing scopes if not found in the current scope.
     *
     * @param name the name of the field.
     * @return const field& The field in the current scope with the given name.
     * @throws std::out_of_range Thrown if there is no field with the given name.
     */
    RATIOCORE_EXPORT virtual const field &get_field(const std::string &name) const;
    /**
     * @brief Get the fields defined within this scope.
     *
     * @return const std::map<std::string, field_ptr>& A map of fields defined within this scope having the fields' names as keys.
     */
    const std::map<std::string, field_ptr> &get_fields() const noexcept { return fields; }

    /**
     * @brief Get the method in the current scope with the given name and the given parameter types, searching in the enclosing scopes if not found in the current scope.
     *
     * @param name The name of the desired method.
     * @param ts The parameter types of the desired method.
     * @return method& The method in the current scope with the given name.
     * @throws std::out_of_range Thrown if there is no method with the given name.
     */
    RATIOCORE_EXPORT virtual method &get_method(const std::string &name, const std::vector<const type *> &ts) const;
    /**
     * @brief Get the methods defined within this scope.
     *
     * @return const std::map<std::string, std::vector<method_ptr>>& A map of methods defined within this scope having the methods' names as keys.
     */
    RATIOCORE_EXPORT virtual const std::map<std::string, std::vector<method_ptr>> &get_methods() const noexcept;

    /**
     * @brief Get the type in the current scope with the given name, searching in the enclosing scopes if not found in the current scope.
     *
     * @param name The name of the desired type.
     * @return type& The type in the current scope with the given name.
     * @throws std::out_of_range Thrown if there is no type with the given name.
     */
    RATIOCORE_EXPORT virtual type &get_type(const std::string &name) const;

    /**
     * @brief Get the types defined within this scope.
     *
     * @return const std::map<std::string, type_ptr>& A map of types defined within this scope having the types' names as keys.
     */
    RATIOCORE_EXPORT virtual const std::map<std::string, type_ptr> &get_types() const noexcept;

    /**
     * @brief Get the predicate in the current scope with the given name, searching in the enclosing scopes if not found in the current scope.
     *
     * @param name The name of the desired predicate.
     * @return predicate& The predicate in the current scope with the given name.
     * @throws std::out_of_range Thrown if there is no predicate with the given name.
     */
    RATIOCORE_EXPORT virtual predicate &get_predicate(const std::string &name) const;
    /**
     * @brief Get the predicates defined within this scope.
     *
     * @return const std::map<std::string, predicate_ptr>& A map of predicates defined within this scope having the predicates' names as keys.
     */
    RATIOCORE_EXPORT virtual const std::map<std::string, predicate_ptr> &get_predicates() const noexcept;

  protected:
    RATIOCORE_EXPORT virtual void new_field(field_ptr f);

  private:
    core &cr;   // the core in which this scope is defined..
    scope &scp; // the enclosing scope..

  private:
    std::map<std::string, field_ptr> fields; // the fields within this scope..
  };
} // namespace ratio::core
