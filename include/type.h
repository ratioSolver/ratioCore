#pragma once
#include "scope.h"
#include <string>
#include <vector>

namespace riddle::ast
{
  class expression;
} // namespace riddle

namespace ratio::core
{
  class method_declaration;
  class predicate_declaration;
  class typedef_declaration;
  class enum_declaration;
  class constructor_declaration;
  class class_declaration;

  class type : public scope
  {
    friend class predicate;
    friend class method_declaration;
    friend class predicate_declaration;
    friend class typedef_declaration;
    friend class enum_declaration;
    friend class constructor_declaration;
    friend class class_declaration;

  public:
    RATIOCORE_EXPORT type(scope &scp, const std::string &name, bool primitive = false);
    type(const type &orig) = delete;
    RATIOCORE_EXPORT virtual ~type();

    inline const std::string get_name() const noexcept { return name; } // returns the name of this type..
    std::string get_full_name() const noexcept
    {
      std::string tp_name = name;
      const type *t = this;
      while (const type *sc = dynamic_cast<const type *>((&t->get_scope())))
      {
        tp_name.insert(0, sc->get_name() + ":");
        t = sc;
      }
      return tp_name;
    }                                                                                 // returns the full name of this type..
    inline bool is_primitive() const noexcept { return primitive; }                   // returns whether this type is primitive..
    const std::vector<type *> &get_supertypes() const noexcept { return supertypes; } // returns the base types of this type..

    RATIOCORE_EXPORT virtual bool is_assignable_from(const type &t) const noexcept; // checks whether this type is assignable from the 't' type..

    RATIOCORE_EXPORT virtual expr new_instance();                         // creates a new instance of this type..
    RATIOCORE_EXPORT virtual expr new_existential();                      // creates a new existential of this type (i.e. an object variable whose allowed values are all the current instances of this type)..
    std::vector<expr> get_instances() const noexcept { return instances; } // returns the instances of this type..

  protected:
    RATIOCORE_EXPORT void new_supertype(type &t) noexcept;
    RATIOCORE_EXPORT void new_constructor(constructor_ptr c) noexcept;
    RATIOCORE_EXPORT void new_method(method_ptr m) noexcept;
    RATIOCORE_EXPORT void new_type(type_ptr t) noexcept;
    RATIOCORE_EXPORT void new_predicate(predicate_ptr p) noexcept;

  public:
    RATIOCORE_EXPORT const field &get_field(const std::string &name) const override;
    RATIOCORE_EXPORT constructor &get_constructor(const std::vector<const type *> &ts) const;
    const std::vector<constructor_ptr> &get_constructors() const noexcept { return constructors; }
    RATIOCORE_EXPORT method &get_method(const std::string &name, const std::vector<const type *> &ts) const override;
    const std::map<std::string, std::vector<method_ptr>> &get_methods() const noexcept override { return methods; }
    RATIOCORE_EXPORT type &get_type(const std::string &name) const override;
    const std::map<std::string, type_ptr> &get_types() const noexcept override { return types; }
    RATIOCORE_EXPORT predicate &get_predicate(const std::string &name) const override;
    const std::map<std::string, predicate_ptr> &get_predicates() const noexcept override { return predicates; }

  private:
    const std::string name;
    const bool primitive; // is this type a primitive type?

  protected:
    std::vector<type *> supertypes;                         // the base types (i.e. the types this type inherits from)..
    std::vector<constructor_ptr> constructors;              // the constructors defined within this type..
    std::map<std::string, std::vector<method_ptr>> methods; // the methods, indexed by their name, defined within this type..
    std::map<std::string, type_ptr> types;                  // the inner types, indexed by their name, defined within this type..
    std::map<std::string, predicate_ptr> predicates;        // the inner predicates, indexed by their name, defined within this type..
    std::vector<expr> instances;                            // a vector containing all the instances of this type..
  };

  class bool_type final : public type
  {
  public:
    bool_type(core &cr);
    bool_type(const bool_type &that) = delete;

    bool is_assignable_from(const type &t) const noexcept override { return &t == this; }
    expr new_instance() noexcept override;
  };

  class int_type final : public type
  {
  public:
    int_type(core &cr);
    int_type(const int_type &that) = delete;

    bool is_assignable_from(const type &t) const noexcept override;
    expr new_instance() noexcept override;
  };

  class real_type final : public type
  {
  public:
    real_type(core &cr);
    real_type(const real_type &that) = delete;

    bool is_assignable_from(const type &t) const noexcept override;
    expr new_instance() noexcept override;
  };

  class time_type final : public type
  {
  public:
    time_type(core &cr);
    time_type(const time_type &that) = delete;

    bool is_assignable_from(const type &t) const noexcept override;
    expr new_instance() noexcept override;
  };

  class string_type final : public type
  {
  public:
    string_type(core &cr);
    string_type(const string_type &that) = delete;

    bool is_assignable_from(const type &t) const noexcept override { return &t == this; }
    expr new_instance() noexcept override;
  };

  class typedef_type final : public type
  {
  public:
    typedef_type(scope &scp, const std::string &name, const type &base_type, const std::unique_ptr<const riddle::ast::expression> &e);
    typedef_type(const typedef_type &orig) = delete;

    const type &get_base_type() const noexcept { return base_type; }

    expr new_instance() noexcept override;

  private:
    const type &base_type;
    const std::unique_ptr<const riddle::ast::expression> &xpr;
  };

  class enum_type : public type
  {
    friend class enum_declaration;

  public:
    enum_type(scope &scp, std::string name);
    enum_type(const enum_type &orig) = delete;
    virtual ~enum_type() = default;

    expr new_instance() override;

  private:
    std::vector<expr> get_all_instances() const noexcept;

  private:
    std::vector<enum_type *> enums;
  };
} // namespace ratio::core
