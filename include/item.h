#pragma once
#include "env.h"
#include "lit.h"
#include "lin.h"
#include <map>

namespace ratio::core
{
  class type;

  class item
  {
  public:
    item(type &tp);
    item(const item &orig) = delete;
    RATIOCORE_EXPORT virtual ~item() = default;

    type &get_type() const noexcept { return tp; }

  private:
    type &tp;
  };

  class bool_item final : public item
  {
  public:
    RATIOCORE_EXPORT bool_item(type &t, const semitone::lit &l);
    bool_item(const bool_item &that) = delete;

    inline semitone::lit get_value() const { return l; }

  private:
    semitone::lit l;
  };

  class arith_item final : public item
  {
  public:
    RATIOCORE_EXPORT arith_item(type &t, const semitone::lin &l);
    arith_item(const arith_item &that) = delete;

    inline semitone::lin get_value() const { return l; }

  private:
    const semitone::lin l;
  };

  class string_item final : public item
  {
  public:
    RATIOCORE_EXPORT string_item(type &t, const std::string &l);
    string_item(const string_item &that) = delete;

    inline std::string get_value() const { return l; }

  private:
    std::string l;
  };

  class complex_item : public item, public env
  {
  public:
    RATIOCORE_EXPORT complex_item(type &tp);
    complex_item(const complex_item &orig) = delete;
    RATIOCORE_EXPORT virtual ~complex_item() = default;

    RATIOCORE_EXPORT expr get(const std::string &name) noexcept override;
  };

  class enum_item final : public complex_item
  {
  public:
    RATIOCORE_EXPORT enum_item(type &t, semitone::var ev);
    enum_item(const enum_item &that) = delete;

    RATIOCORE_EXPORT expr get(const std::string &name) noexcept override;

    inline semitone::var get_var() const { return ev; }

  private:
    const semitone::var ev;
  };
} // namespace ratio::core
