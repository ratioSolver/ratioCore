#pragma once
#include "scope.h"
#include <string>
#include <vector>

namespace ratio::core
{
  class item;
  class predicate;
  class constructor;

  class type : public scope
  {
    friend class predicate;

  public:
    ORATIOCORE_EXPORT type(core &cr, const std::string &name, bool primitive = false);
    type(const type &orig) = delete;
    ORATIOCORE_EXPORT virtual ~type() = default;

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

    ORATIOCORE_EXPORT virtual bool is_assignable_from(const type &t) const noexcept; // checks whether this type is assignable from the 't' type..

    ORATIOCORE_EXPORT virtual expr new_instance();                         // creates a new instance of this type..
    std::vector<expr> get_instances() const noexcept { return instances; } // returns the instances of this type..

    ORATIOCORE_EXPORT constructor &get_constructor(const std::vector<const type *> &ts) const;
    std::vector<constructor *> get_constructors() const noexcept { return constructors; }

  private:
    const std::string name;
    const bool primitive; // is this type a primitive type?

  protected:
    std::vector<type *> supertypes;          // the base types (i.e. the types this type inherits from)..
    std::vector<constructor *> constructors; // the constructors defined within this type..
    std::vector<expr> instances;             // a vector containing all the instances of this type..
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
} // namespace ratio::core
