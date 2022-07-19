#pragma once
#include "oratiocore_export.h"
#include "riddle_parser.h"

namespace ratio::core
{
  class type;

  class field final
  {
  public:
    field(type &tp, const std::string &name, std::unique_ptr<const riddle::ast::expression> e = nullptr, bool synthetic = false) : tp(tp), name(name), xpr(std::move(e)), synthetic(synthetic) {}
    field(const field &orig) = delete;

    inline type &get_type() const { return tp; }                                                        // returns the type of the field..
    inline const std::string &get_name() const { return name; }                                         // returns the name of the field..
    inline const std::unique_ptr<const riddle::ast::expression> &get_expression() const { return xpr; } // returns, if any, the initialization expression..
    inline bool is_synthetic() const { return synthetic; }                                              // returns whether the field is synthetic or not..

  private:
    type &tp;                                           // the type of the field..
    const std::string name;                             // the name of the field..
    std::unique_ptr<const riddle::ast::expression> xpr; // the initialization expression..
    const bool synthetic;                               // the field is synthetic (a synthetic field is a field which is not created by the user, e.g. 'this')..
  };
} // namespace ratio::core
